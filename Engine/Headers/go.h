/* 
  Public header file for accessing April's abstract syntax
  Copyright (c) 2016. Francis G. McCabe

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
  except in compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software distributed under the
  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, either express or implied. See the License for the specific language governing
  permissions and limitations under the License.
*/
#ifndef _GO_H
#define _GO_H

#include <stdlib.h>
#include "config.h"
#include "dbgflags.h"		/* standard debugging flags */
#include "local.h"		  /* localization */
#include "retcode.h"
#include "unicode.h"
#include "iostr.h"
#include "file.h"
#include "formio.h"

typedef struct _process_ *processPo;

#ifndef GO_REGS			    /* do we know how many registers? */
#define GO_REGS 64		  /* should'nt be more than #bits in an integer*/
#endif

#include "word.h"		    /* standard definition of a cell & access fns */
#include "heap.h"
#include "global.h"
#include "symbols.h"		/* standard symbols available to the engine */
#include "errors.h"		  /* standard error codes */
#include "eval.h"
#include "str.h"        /* String manipulation */

#ifndef NULL
#define NULL            ((void*)0) /* The NULL pointer */
#endif

#undef NumberOf
#define NumberOf(a) (sizeof(a)/sizeof(a[0]))

#undef ElementSize
#define ElementSize(a) (sizeof(a[0]))

typedef retCode (*funpo)(processPo p,ptrPo args); /* Escape function pointer */

void runGo(register processPo P);

#include "dict.h"
#include "signals.h"

#define EXIT_SUCCEED 0		/* Normal exit */
#define EXIT_FAIL 1		    /* Failing exit */

#endif