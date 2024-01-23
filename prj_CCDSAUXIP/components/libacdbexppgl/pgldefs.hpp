// PRQA S 1050 EOF
/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DBXpress for DB2 $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.1  2008/03/19 15:34:37  enrcar
	EC:: Imported source
	

*/

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include <malloc.h>
#include <stdlib.h>
#include <memory>

typedef unsigned int uint4 ;

#ifdef WIN32
#include <sqlhdr.h>
#define freeSQLDA(sqlda)				SqlFreeMem(sqlda, SQLDA_FREE)
#define freeSimpleLOBuffer(pbuffer)		SqlFreeMem(pbuffer, STRING_FREE)
#else
#define freeSQLDA(sqlda)				free(sqlda)
#define freeSimpleLOBuffer(buffer)		free(buffer)
#endif

/*
/////////////////////////////////////////////////////////////////////////////
// CDB2Activity - holder of some activity

struct CDB2Activity {
	virtual ~CDB2Activity () {}
};

typedef std::auto_ptr<CDB2Activity> CDB2ActivityPtr;

*/

#if(0)
#define IFX_SERVER_BUSY					-439
#define IFX_ALREADY_IN_TRANSACTION		-535
#define IFX_INVALID_TRANSACTION_STATE	-1800
#endif

#define INTERVAL_AS_VARCHAR_LENGTH	28
#define INT8_AS_DECIMAL_PRECISION	20
#define FLOATING_DECIMAL_PRECISION	32
#define FLOATING_DECIMAL_SCALE		16
