/* 
  I/O handling top-level
  Copyright (c) 2016, 2017. Francis G. McCabe

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
  except in compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software distributed under the
  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, either express or implied. See the License for the specific language governing
  permissions and limitations under the License.
*/
#ifndef _IO_LIB_H_
#define _IO_LIB_H_

#include "utils.h"
#include "unicode.h"
#include "retcode.h"
#include "integer.h"
#include "logical.h"
#include "object.h"
#include "lockable.h"

/* Status return type ... most API functions return one of these */
typedef enum {
  ioFile, ioChar, ioBlock, ioDir, ioPipe, ioString, ioLog, ioUDP
} ioType;
typedef enum {
  ioNULL = 000, ioREAD = 001, ioWRITE = 002
} ioDirection;

typedef struct _io_object_ *ioPo;
extern classPo ioClass;

#include "unicode.h"

#ifndef MAXLINE
#define MAXLINE 1024                  /* Size of a standard line buffer */
#endif

#ifndef MAXFILELEN
#define MAXFILELEN 512                /* Maximum length of a file name */
#endif

void initIo(void);                      /* Initialize I/O system */
void closeIo(void);                     /* Close down the I/O system */

byte inB(ioPo f);                     /* read a single byte */
retCode inByte(ioPo f, byte *ch);      /* read a single byte -- with status */
retCode inBytes(ioPo f, byte *buffer, long len, long *actual); /* read a block of bytes */
retCode putBackByte(ioPo f, byte b);

retCode markIo(ioPo f, long *mark);          /* record a mark in the file, return Ok if allowed */
retCode resetToMark(ioPo f, long mark);      /* Rewind file to mark point, if possible */

retCode isLookingAt(ioPo f, char *prefix);    /* Is prefix the first thing in the file? */

retCode inChar(ioPo f, codePoint *ch);     /* read a character */
retCode unGetChar(ioPo f, codePoint ch);   /* put a single character back */
retCode inLine(ioPo f, char *buffer, long len, long *actual, char *term);

retCode inBlock(ioPo f, byte *buffer, long len);
retCode inBytes(ioPo f, byte *buffer, long len, long *act);

retCode pushBack(ioPo f, char * str, long from, long len);
retCode skipBlanks(ioPo f);

retCode outByte(ioPo f, byte c);
retCode outChar(ioPo f, codePoint ch);
retCode outBlock(ioPo f, byte *data, long len);
retCode outBytes(ioPo f, byte *data, long len, long *actual);

retCode outText(ioPo f, char * text, long len);
retCode outStr(ioPo f, char *str);
long outColumn(ioPo f);                 /* return number of chars since lf */

retCode closeFile(ioPo f);            /* generic file closer */
retCode flushFile(ioPo f);            /* generic file flush */
retCode preFlushFile(ioPo f, int count); /* file flush */
void flushOut(void);                    /* flush all files */

void setEncoding(ioPo f, ioEncoding encoding);
retCode isFileOpen(ioPo f);
retCode isReadingFile(ioPo f);
retCode isWritingFile(ioPo f);
retCode isInReady(ioPo f);
retCode isOutReady(ioPo f);
retCode isFileAtEof(ioPo f);
retCode fileStatus(ioPo f);

char * fileName(ioPo f);
ioDirection fileMode(ioPo f);
long inBPos(ioPo f);
long inCPos(ioPo f);
long outBPos(ioPo f);
long outCPos(ioPo f);
retCode ioSeek(ioPo f, long pos);

typedef retCode (*ioPropertyFun)(ioPo f, void *k, void *v, void *c); /* Processing func */

retCode setFileProperty(ioPo f, void *key, void *val);
void removeFileProperty(ioPo f, void *key);
retCode fileProperty(ioPo f, void *key, void **val);
retCode processFileProperties(ioPo f, ioPropertyFun fn, void *c);
retCode processAllFileProperties(ioPropertyFun f, void *c);

retCode ioErrorMsg(objectPo io, char *fmt, ...);

#ifdef VERIFY_OBJECT
objectPo checkCast(void *c, classPo class);

#define O_IO(c) ((ioPo)(checkCast((c),ioClass)))
#else
#define O_IO(c) ((ioPo)(c))
#endif

#endif
