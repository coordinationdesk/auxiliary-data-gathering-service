/*
 *
 * Copyright 1995-1996, Advanced Computer Systems , Inc.
 * Via Paolo Frisi, 7a Roma
 *
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Advanced Computer Systems, Inc.
 *
 * Author: M. Pastori
 *
 * @(#) A.C.S. Resource Management Library - %M% %I% %E%
 *
 */

#ifndef __LIBRM_H__
#define __LIBRM_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define RM_ERROR_OFFSET		-15000
#define RM_MAX_ERROR		-15099

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* PROVIDED TYPES */

typedef enum {
	RM_NOT_ENOUGH_MEMORY		= (RM_ERROR_OFFSET -  0),
	RM_INVALID_ENTRY			= (RM_ERROR_OFFSET -  1),
	RM_NOT_EXISTING_DB			= (RM_ERROR_OFFSET -  2),
	RM_ENTRY_ALREADY_EXISTS		= (RM_ERROR_OFFSET -  3),
	RM_NAME_ALREADY_USED		= (RM_ERROR_OFFSET -  4),
	RM_NEXT_FIELD_NOT_EMPTY		= (RM_ERROR_OFFSET -  5),
	RM_INVALID_PARAM			= (RM_ERROR_OFFSET -  6),
	RM_ENTRY_NOT_FOUND			= (RM_ERROR_OFFSET -  7),
	RM_NO_TOKEN_FOUND			= (RM_ERROR_OFFSET -  8),
	RM_ENTRY_NOT_CREATED		= (RM_ERROR_OFFSET -  9),
	RM_END_OF_DB				= (RM_ERROR_OFFSET - 10),
	RM_CANNOT_CREATE_FILE		= (RM_ERROR_OFFSET - 11),
	RM_CANNOT_OPEN_FILE			= (RM_ERROR_OFFSET - 12),
	RM_CANNOT_REWIND_FILE		= (RM_ERROR_OFFSET - 13),
	RM_CANNOT_CLOSE_FILE		= (RM_ERROR_OFFSET - 14),
	RM_CANNOT_WRITE_RECORD		= (RM_ERROR_OFFSET - 15),
	RM_CANNOT_MERGE_DB			= (RM_ERROR_OFFSET - 16),
	RM_CANNOT_READ_RECORD		= (RM_ERROR_OFFSET - 17),
	RM_CANNOT_DELETE_FILE		= (RM_ERROR_OFFSET - 18),
	RM_NULL_PARAMETER			= (RM_ERROR_OFFSET - 19),
	RM_NULL_VALUE				= (RM_ERROR_OFFSET - 20),
	RM_NULL_LINE				= (RM_ERROR_OFFSET - 21)
} RM_ERROR_TYPE ;


#if ( defined(linux) && !defined(__cplusplus) && !defined (bool) )
	/* Linux: TRUE happens to be already defined (even if BOOL is not), 
	so following block will not run. The include of curses is forced */
	#include <curses.h>
#endif

#if !defined(CURSES_H) && !defined(_CURSES_H) && !defined(__cplusplus) /* curses.h makes a typedef char bool */
 #ifndef TRUE
  #ifndef bool
   #ifndef _bool
    #define _bool
     
     typedef enum { FALSE = 0, TRUE  = 1 } bool ;
    
   #endif /* _bool */
  #else /* bool */
   #ifndef TRUE
    #define TRUE 1
   #endif /* TRUE */
   #ifndef FALSE
    #define FALSE 0
   #endif /* FALSE */
  #endif /* bool */
	
 #else /* ! TRUE */
  #if !defined bool
/*   typedef int bool ; */
  #else /* bool */
   #ifndef TRUE
    #define TRUE 1
   #endif /* TRUE */
   #ifndef FALSE
    #define FALSE 0
   #endif /* FALSE */
  #endif /* bool */  
 #endif /* TRUE */
#endif /* CURSES_H */

typedef enum {
	rmInt,
	rmFloat,
	rmDouble,
	rmChar,
	rmString,
	rmIntArray,
	rmFloatArray,
	rmDoubleArray,
	rmCharArray,
	rmMaxType
} rmTypeT ;

#define rmIsTypeValid(v)	((v) >= 0 || (v) < rmMaxType))

typedef enum {
	rmUppercase,
	rmLowercase,
	rmNoEncoding
} rmEncodeModeT ;

#define rmIsEncodeModeValid(v)	((v) >= 0 || (v) <= rmNoEncoding)

#ifdef __STDC__
#define rmRangeError(a,e)	((e) <= a ## _ERROR_OFFSET && (e) >= a ## _MAX_ERROR)

int	    	 rmCreateDb(const char *) ;
int	    	 rmDeleteDb(int) ;
int	    	 rmGetValue(int, const char *, rmTypeT, void*) ;
int	    	 rmPutValue(int, const char *, rmTypeT, const void*, rmEncodeModeT) ;
int	    	 rmSetValue(int, const char *, rmTypeT, const void *, rmEncodeModeT) ;
int	    	 rmMergeDb(int, int, rmEncodeModeT) ;
int	    	 rmChangeValue(int, const char *, const char *) ;
int	    	 rmDeleteEntry(int, const char *) ;
int	    	 rmPutEntry(int, const char *, rmEncodeModeT) ;
int	    	 rmScanParams(char **, int, int) ;
int	    	 rmGetDbId(const char *) ;
const char  *rmGetDbName(int) ;
int	    	 rmGetEntriesNum(int) ;
int	    	 rmWrite(int, const char *, const char *, bool) ;
int	    	 rmRemoveFile(const char *, const char *) ;
int	    	 rmRead(int *, int, const char *, const char *, rmEncodeModeT, bool, bool) ;
int	    	 rmPutArray(int, const char *, rmTypeT, void *, unsigned int, rmEncodeModeT) ;
int	    	 rmGetArraySize(int, const char *) ;
int	    	 rmGetArray(int, const char *, rmTypeT, void *, int, unsigned int, unsigned int) ;
int	    	 rmDeleteArray(int, const char *) ;
const char  *rmGetErrorString(int) ;

#else /* __STDC__ */

int	    rmCreateDb() ;
int	    rmDeleteDb() ;
int	    rmGetValue() ;
int	    rmPutValue() ;
int	    rmSetValue() ;
int	    rmMergeDb() ;
int	    rmChangeValue() ;
int	    rmDeleteEntry() ;
int	    rmPutEntry() ;
int	    rmScanParams() ;
int	    rmGetDbId() ;
char   *rmGetDbName() ;
int	    rmGetEntriesNum() ;
int	    rmWrite() ;
int	    rmRemoveFile() ;
int	    rmRead() ;
int	    rmPutArray() ;
int	    rmGetArraySize() ;
int	    rmGetArray() ;
int	    rmDeleteArray() ;
char   *rmGetErrorString() ;
#define rmGetErrorString(a)	(a, "Ansi compilation required to use 'RmGetErroString'") 

#endif /* __STDC__ */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !__LIBRM_H__ */

