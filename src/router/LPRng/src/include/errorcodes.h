/***************************************************************************
 * LPRng - An Extended Print Spooler System
 *
 * Copyright 1988-2003, Patrick Powell, San Diego, CA
 *     papowell@lprng.com
 * See LICENSE for conditions of use.
 * $Id: errorcodes.h,v 1.1.1.1 2007/02/15 12:13:01 jiahao Exp $
 ***************************************************************************/



#ifndef _ERRORCODES_H_
#define _ERRORCODES_H_ 1
/*
 * filter return codes and job status codes
 * - exit status of the filter process 
 * If a printer filter fails, then we assume JABORT status and
 * will record information about failure
 */

                       /* process exit codes */
#define JSUCC    0     /* 0 done */
/* from 1 - 31 are signal terminations */
#define JFAIL    32    /* 1 failed - retry later */
#define JABORT   33    /* 2 aborted - do not try again, but keep job */
#define JREMOVE  34    /* 3 failed - remove job */
#define JHOLD    37    /* 6 hold this job */
#define JNOSPOOL 38    /* 7 no spooling to this queue */
#define JNOPRINT 39    /* 8 no printing from this queue  */
#define JSIGNAL  40    /* 9 killed by unrecognized signal */
#define JFAILNORETRY 41 /* 10 no retry on failure */
#define JSUSP    42		/* 11 process suspended successfully */
#define JTIMEOUT 43		/* 12 timeout */
#define JWRERR   44		/* 13 write error */
#define JRDERR   45		/* 14 read error  */
#define JCHILD   46		/* 15 no children */
#define JNOWAIT  47		/* 16 no wait status */

/* PROTOTYPES */

#endif