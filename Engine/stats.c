/* 
  Statistics measurement for the L&O engine
  Copyright (c) 2016, 2017. Francis G. McCabe

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
  except in compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software distributed under the
  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, either express or implied. See the License for the specific language governing
  permissions and limitations under the License.
*/

#include <stdlib.h>
#include <math.h>
#include "lo.h"			/* main header file */
#include "escodes.h"


long pcCount = 0;
long insCount[256];
long escCount[256];

#ifdef STATSTRACE
/* A count of the escapes executed */

logical traceCount = False;

/* if we want to dump instruction counts */

void countEscape(insWord PCX) {
  escCount[op_o_val(PCX)]++;
}

void countIns(insWord PCX) {
  pcCount++;
  insCount[op_code(PCX)]++;
}


#undef lastInstruction
#define lastInstruction

static void sortCounts(long count, long base[], long sorted[]) {
  long i, j;

  for (i = 0; i < count; i++)
    sorted[i] = i;

  for (i = 0; i < count; i++) { /* simple sort -- only executed once */
    long max = base[sorted[i]];
    long maxI = i;

    for (j = i + 1; j < count; j++) {
      if (base[sorted[j]] > max) {
        max = base[sorted[j]];
        maxI = j;
      }
    }

    if (maxI != i) {
      long swap = sorted[i];
      sorted[i] = sorted[maxI];
      sorted[maxI] = swap;
    }
  }
}

#undef instruction
#define instruction(Op, Cd, A1, A2, Cmt) \
    case Op:                      /* Cmt */\
      outMsg(logFile,#Op ": %d = %5.2g%%\n",insCount[Op], (insCount[Op]/(double)pcCount)*100);\
      break;

#undef escape
#define escape(Nm, Sc, Priv, Tp, Cmt) \
    case Esc##Nm:                      /* Cmt */\
      outMsg(logFile,#Nm ": %d " Cmt "\n",escCount[Esc##Nm]);\
      break;
#undef constr
#define constr(Nm, Tp, T)

#undef tdf
#define tdf(Nm, Sp, Tp)

void dumpInsCount(void) {
  if (traceCount) {
    unsigned long i;
    long sorted[1024];    /* used to sort the instruction counts */
    long count = 0;

    outMsg(logFile, "%d instructions executed\n", pcCount);

    sortCounts(NumberOf(escCount), escCount, sorted);

    outMsg(logFile, "Escape functions\n");

    for (i = 0; i < NumberOf(escCount); i++)
      if (escCount[sorted[i]] != 0) {
        count += escCount[sorted[i]];

        switch (sorted[i]) {
#include "escapes.h"

          default:
            outMsg(logFile, "unknown[%x]", i);
        }
      }

    outMsg(logFile, "%d escapes called\n", count);

    sortCounts(NumberOf(insCount), insCount, sorted);

    for (i = 0; i < NumberOf(insCount); i++)
      if (insCount[sorted[i]] != 0) {
        switch (sorted[i]) {
#include "instructions.h"

          default:
            outMsg(logFile, "unknown[%x]", i);
        }
      }
    outMsg(logFile, "%d instructions executed\n", pcCount);

#ifdef MEMTRACE
    {
      extern long gcCount, extendCount;
      outMsg(logFile, "%d gc collections, %d stack extensions\n", gcCount, extendCount);
    }
#endif

    flushFile(logFile);
  }
}

#endif
