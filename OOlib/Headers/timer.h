/*
   Timer interface
  Copyright (c) 2016. Francis G. McCabe

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
  except in compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software distributed under the
  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, either express or implied. See the License for the specific language governing
  permissions and limitations under the License.
*/ 

#ifndef _OOIO_TIMER_H_
#define _OOIO_TIMER_H_

#include "config.h"

typedef void (*timeFun)(void *cl);

retCode setAlarm(double time,timeFun onWakeup,void *cl);
void cancelAlarm(void);

#ifdef _WIN32			/* Windows'95 specific stuff */
#include <winsock.h>
struct  itimerval {		/* duplicate Unix declaration */
  struct  timeval it_interval;	/* timer interval */
  struct  timeval it_value;	/* current value */
};
void gettimeofday(struct timeval *t, void *ignored);
#else				/* UNIX only */
#include <sys/time.h>
#endif

#endif

