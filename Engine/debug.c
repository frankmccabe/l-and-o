/* 
 Instruction-level debugging of L&O code
 Copyright (c) 2016, 2017. Francis G. McCabe

 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 except in compliance with the License. You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software distributed under the
 License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied. See the License for the specific language governing
 permissions and limitations under the License.
 */
#include "lo.h"
#include "debug.h"
#include "disass.h"
#include "esc.h"

logical tracing = True;  /* do we show each step */

typedef struct _break_point_ *breakPointPo;

typedef struct _break_point_ {
  short arity;
  char name[MAX_SYMB_LEN];
} BreakPoint;

static retCode addBreakPoint(breakPointPo bp);
static logical breakPointHit(char *name, short arity);
static retCode clearBreakPoint(breakPointPo bp);
static retCode parseBreakPoint(char *buffer, long bLen, breakPointPo bp);

retCode g__ins_debug(processPo P, ptrPo a) {
  debugging = interactive = True;
  return Ok;
}

/* Parse a command line to display a register value */
void showReg(ptrPo a, char *name, integer reg) {
  ptrPo ptr = &a[reg];

  outMsg(logFile, "%s[%d] = ", name, (int) reg);

  if (*ptr == 0)
    outMsg(logFile, "<<NULL>>\n");
  else {
    while (isvar(*ptr)) {
      if ((ptrPo) *ptr == ptr) {
        if (isSuspVar(ptr))
          outMsg(logFile, "_*%x", ptr);
        else
          outMsg(logFile, "_%x", ptr);
        return;
      } else {
        outMsg(logFile, "_%x->", ptr);
        ptr = (ptrPo) *ptr;
      }
    }

    outMsg(logFile, "%.40w\n", ptr);
  }
}

/* waiting for next instruction */
long cmdCounter = 0;

#ifdef EXECTRACE

static long cmdCount(char *cmdLine) {
  long count = (long) parseInteger(cmdLine, strlen((char *) cmdLine));
  if (count == 0)
    return 1; /* never return 0 */
  else
    return count;
}

static processPo focus = NULL;

static pthread_mutex_t debugMutex = PTHREAD_MUTEX_INITIALIZER;

static void clrCmdLine(char *cmdLine, long len) {
  strMsg(cmdLine, len, "n\n"); /* default to next instruction */
}

static unsigned long stackSpace(processPo P, callPo C, choicePo B) {
  ptrPo base = ((ptrPo) C < (ptrPo) B ? (ptrPo) C : (ptrPo) B);
  return base - (ptrPo) P->proc.trail;

}

retCode
debug_stop(processPo p, ptrI prog, insPo pc, ptrI cprog, insPo cpc, ptrPo a, ptrPo y, ptrPo S, long Svalid, rwmode mode,
           callPo C, choicePo B, choicePo SB, choicePo T) {
  char ch;
  static char cmdLine[256] = "n";
  codePo code = codeV(prog);
  codePo ccode = codeV(cprog);
  ptrPo Lits = codeLits(code);
  extern HeapRec globalHeap;

  ptrPo hBase = (ptrPo) p->proc.heap.base;
  ptrPo hCreate = (ptrPo) p->proc.heap.create;

  pthread_mutex_lock(&debugMutex);

  if (focus == NULL || focus == p) {
    insWord PCX = *pc;

    switch (op_code(PCX)) {
      case kawl:
      case lkawl:
      case dlkawl: {
        objPo prg = objV(Lits[op_o_val(PCX)]);
        prgLabelPo lbl = programName(prg);
        char *name = lbl->name;
        long arity = lbl->arity;

        if (breakPointHit(name, (short) arity)) {
          p->proc.waitFor = nextIns;
          cmdCounter = 0;
        }
        break;
      }
      case kawlO:
      case lkawlO:
      case dlkawlO: {
        objPo prg = objV(deRefI(&a[1]));
        char *name = objectClassName(prg);
        long arity = objectArity(prg);

        if (breakPointHit(name, (short) arity)) {
          p->proc.waitFor = nextIns;
          cmdCounter = 0;
        }
        break;
      }
      default:
        break;
    }

    if (p->proc.waitFor == nextIns)
      cmdCounter--;

    if (tracing || cmdCounter <= 0) {
      char pref[MAX_SYMB_LEN];

      strMsg(pref, NumberOf(pref), "%w "RED_ESC_ON "[%d]" RED_ESC_OFF " %w", &p->proc.thread, pcCount, &Lits[0]);
      dissass(pref, code, pc, a, y, S, mode, B, hBase, hCreate);

      outMsg(logFile, "\ncPC=%w[%d],C=%x,B=%x,SB=%x,T=%x,\nY=%x[%d],",
             &codeLits(ccode)[0], cpc - codeIns(ccode), C, B, SB, T, y, envSize(cpc));
      if (Svalid > 0)
        outMsg(logFile, "S=%x(%d),", S, Svalid);
      outMsg(logFile, "trail=%x,stack=%d", p->proc.trail, stackSpace(p, C, B));
      if (!identical(p->proc.trigger, emptyList))
        outMsg(logFile, ",%d triggered", ListLen(deRefI(&p->proc.trigger)));
      outMsg(logFile, ",H=%x[%d], ", hCreate, (ptrPo) p->proc.heap.end - hCreate);
      outMsg(logFile, "global=%x[%d]\n", globalHeap.create, globalHeap.end - (objPo) globalHeap.create);
      flushFile(logFile);
    }

    if (cmdCounter <= 0) { /* do we need to stop? */
      while (debugging && cmdCounter <= 0) { /* prompt the user */
        char *ln = cmdLine;
        outMsg(logFile, " => ");
        flushOut();

        retCode res = inByte(stdIn, (byte *) &ch);

        if (ch != '\n' && res == Ok) {
          do {
            *ln++ = ch;
            res = inByte(stdIn, (byte *) &ch);
          } while (ch != '\n' && res == Ok);
          *ln++ = '\0';
        }

        switch (cmdLine[0]) {
          case 'n':
            cmdCounter = cmdCount(cmdLine + 1);
            p->proc.waitFor = nextIns;
            tracing = True;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;
          case 'N':
            cmdCounter = cmdCount(cmdLine + 1);
            clrCmdLine(cmdLine, NumberOf(cmdLine));

            switch (op_code(*pc)) {
              case kawlO:
              case kawl:
              case lkawlO:
              case lkawl:
              case dlkawlO:
              case dlkawl:
                p->proc.waitFor = nextSucc;
                break;
              default:
                p->proc.waitFor = nextIns;
            }
            tracing = False;
            break;

          case '\n':
            cmdCounter = 1;
            p->proc.waitFor = nextIns;
            tracing = True;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case 'x': /* wait for a success */
            cmdCounter = cmdCount(cmdLine + 1);
            p->proc.waitFor = nextSucc;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case '+': { // Set up a new break point
            BreakPoint bp;
            retCode ret = parseBreakPoint(&cmdLine[1], NumberOf(cmdLine) - 1, &bp);
            if (ret == Ok)
              ret = addBreakPoint(&bp);

            if (ret != Ok)
              outMsg(logFile, "Could not set spy point on %s\n%_", &cmdLine[1]);
            else
              outMsg(logFile, "spy point set on %s\n%_", &cmdLine[1]);

            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;
          }

          case '-': {
            BreakPoint bp;
            retCode ret = parseBreakPoint(&cmdLine[1], NumberOf(cmdLine) - 1, &bp);

            if (ret == Ok && clearBreakPoint(&bp) == Ok)
              outMsg(logFile, "spy point cleared on %s\n%_", &cmdLine[1]);
            else
              outMsg(logFile, "Could not clear spy point on %s\n%_", &cmdLine[1]);

            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;
          }

          case 'l': {
            integer start, len;
            packagePo pkg;
            retCode ret = locateSourceFragment(code, pc, &pkg, &start, &len);
            if (ret == Ok) {
              outMsg(logFile, "PC in %d:%d from %s\n%_", start, len, pkgName(pkg));
            }
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;
          }

          case 'f':
            focus = p;
            outMsg(logFile, "Focussing on program %w\n", &p->proc.thread);
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case 'F':
            pthread_mutex_unlock(&debugMutex);
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            return Fail;

          case 'u':
            focus = NULL;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case 'q':
            outMsg(logFile, "terminating lo session");
            lo_exit(0);
            break;

          case 'c':
            cmdCounter = cmdCount(cmdLine + 1);
            p->proc.waitFor = nextBreak;
            tracing = False;
            traceCalls = False;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case 't':
            p->proc.waitFor = nextBreak;
            tracing = True;
            cmdCounter = 1;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case 'S':
            SymbolDebug = True;
            debugging = False;
            interactive = True;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            break;

          case 'a': { /* dump an argument register */
            showReg(a, "A", parseInteger(&cmdLine[1], uniStrLen(&cmdLine[1])));
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
          }

          case 'y': { /* dump a local variable */
            integer off = parseInteger(&cmdLine[1], uniStrLen(&cmdLine[1]));

            outMsg(logFile, "Y[%ld] = %w\n", off, &y[-off]);
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
          }

          case 'r': { /* show all registers */
            unsigned int i;
//            int Ylen = envSize(cpc);

            for (i = 1; i <= B->AX; i++)
              outMsg(logFile, "A[%d]=%w\n", i, &a[i]);

//          for (i = 1; i <= Ylen; i++)
//            outMsg(logFile, "%Y[%d]=%w\n", i, &y[-i]);


            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
          }

          case 'P': { /* Display all processes */
            displayProcesses();
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
          }
          case 's': /* Show a stack trace of this process */
            p->proc.B = B;
            p->proc.C = (callPo) y;
            p->proc.cPC = cpc;
            stackTrace(p);
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;

          case 'v': // Verify the stack
            p->proc.B = B;
            p->proc.C = (callPo) y;
            p->proc.cPC = cpc;
            verifyProc(p);
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;

          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9': {
            cmdCounter = cmdCount(cmdLine);
            p->proc.waitFor = nextIns;
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
          }

          case 'i': { /* Show the following instructions */
            long count = cmdCount(cmdLine + 1);
            insPo tmpPc = pc;
            insPo limit = &code->data[code->size];

            while (count-- > 0 && tmpPc < limit) {
              tmpPc = dissass(NULL, codeV(prog), tmpPc, a, y, S, dummyMode, B, NULL, NULL);
              outMsg(logFile, "\n");
            }
            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
          }

          default:
            outMsg(logFile, "'n' = step, 'N' = step over, 'c' = continue, 't' = trace mode, 'q' = stop\n");
            outMsg(logFile, "'x' = step until success, 'F' = force backtrack\n");
            outMsg(logFile, "'<n>' = step <n>\n");
            outMsg(logFile, "'i <count> = list <count> instructions\n");
            outMsg(logFile, "'S' = symbolic mode\n");
            outMsg(logFile, "'r' = show registers, 'a <n>' = show A[n], 'y <n>' = show Y[n]\n");
            outMsg(logFile, "'s' = show stack trace\n");
            outMsg(logFile, "'P' = show all processes\n");
            outMsg(logFile, "'f' = focus on this process\n");
            outMsg(logFile, "'u' = unfocus \n");

            clrCmdLine(cmdLine, NumberOf(cmdLine));
            continue;
        }
      }
      pthread_mutex_unlock(&debugMutex);
      return Ok;
    }
  }

  pthread_mutex_unlock(&debugMutex);
  return Ok;
}

static BreakPoint breakPoints[10];
static int breakPointCount = 0;

retCode addBreakPoint(breakPointPo bp) {
  for (int ix = 0; ix < breakPointCount; ix++) {
    if (breakPoints[ix].arity == -1) {
      breakPoints[ix] = *bp;
      return Ok;
    }
  }
  if (breakPointCount < NumberOf(breakPoints)) {
    breakPoints[breakPointCount++] = *bp;
    return Ok;
  } else
    return Fail;
}

logical breakPointHit(char *name, short arity) {
  for (int ix = 0; ix < breakPointCount; ix++) {
    if (breakPoints[ix].arity == arity && uniCmp(breakPoints[ix].name, name) == same)
      return True;
  }
  return False;
}

retCode clearBreakPoint(breakPointPo bp) {
  for (int ix = 0; ix < breakPointCount; ix++) {
    if (breakPoints[ix].arity == bp->arity && uniCmp(breakPoints[ix].name, bp->name) == same) {
      if (ix == breakPointCount - 1) {
        breakPointCount--;
        while (breakPointCount >= 0 && breakPoints[breakPointCount].arity == -1)
          breakPointCount--;
        return Ok;
      } else {
        breakPoints[ix].arity = -1;
        return Ok;
      }
    }
  }

  return Fail;
}

static retCode parseBreakPoint(char *buffer, long bLen, breakPointPo bp) {
  long b = 0;
  long ix = 0;

  while (ix < bLen && buffer[ix] == ' ')
    ix++;

  while (ix < bLen) {
    codePoint cp = nextCodePoint(buffer, &ix, bLen);
    switch (cp) {
      case '\n':
      case 0:
        appendCodePoint(bp->name, &b, NumberOf(bp->name), 0);
        bp->arity = 0;
        return Eof;
      case '/': {
        appendCodePoint(bp->name, &b, NumberOf(bp->name), 0);
        integer arity = parseInteger(&buffer[ix], bLen - ix);
        bp->arity = (short) arity;
        return Ok;
      }
      default:
        appendCodePoint(bp->name, &b, NumberOf(bp->name), cp);
        continue;
    }
  }
  return Error;
}

retCode breakPoint(processPo P) {
  return Ok;
}

void dC(ptrI w) {
  outMsg(logFile, "%w\n", &w);
  flushOut();
}

#endif /* EXECTRACE */

