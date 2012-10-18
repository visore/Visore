//
//  Common.h
//  Math
//
//  Created by Jason on 06/18/2009.
//  Copyright 2009 The Echo Nest. All rights reserved.
//

#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#ifndef _WIN32
	#include <sys/time.h>
#else
#include "win_funcs.h"
#include <sys/types.h>
/* for STL*/
	#ifdef max
		#undef max
	#endif
	#ifdef min
		#undef min
	#endif
#include <malloc.h>
#endif
#include <float.h>
#include <stdio.h>
#include <stdarg.h>

#define SEC2BPM(t) ( 60.0f / (double)t )
#define BPM2SEC(b) ( 1.0f / ((double)(b) / 60.0f) )
#define NORMCLIP(v) { v = v >= 1.0f ? 1.0f : v; v = v <= 0.0f ? 0.0f : v; }

#ifndef NULL
#define NULL 0
#endif

// TODO: Make this support variable arg lists and forward them to fprintf.
static inline void TRACE(const char* s)
{
    #ifdef DEBUG
    fprintf(stderr, s);
    #endif
}

// Returns the current date in seconds.  The precision is in microseconds.
static inline double now (void)
{
  struct timeval tv;
  double now;
  gettimeofday (&tv, NULL);
  now = 1e-6 * tv.tv_usec + tv.tv_sec;
  return now;
}

typedef unsigned int uint; 
#define NELEM(array) (sizeof(array) / sizeof(array[0]))

#ifndef _WIN32
#define EN_ARRAY(type,var,size) type var[size] 
#else
#define EN_ARRAY(type,var,size) type* var = (type*) _alloca((size)*sizeof(type)) 
#endif

#endif
