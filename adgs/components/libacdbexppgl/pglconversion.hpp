// PRQA S 1050 EOF
/*

	Copyright 1995-2007, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DBXpress for Postgres $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/05/17 09:17:00  lucio.pulvirenti
	Methods: jd_to_YMD and jd2000secs_to_YMDhms added
	Unused, commented code removed
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.1  2008/02/25 17:20:55  marpas
	work in progres
	
	

*/

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include "dbxpress.hpp"


/////////////////////////////////////////////////////////////////////////////
// Data type conversion utilities

void int8_to_bcd(signed long long int *pint8, FMTBcd *pbcd);


/////////////////////////////////////////////////////////////////////////////
// Julian date conversion utilities

void jd_to_YMD(size_t jd, INT16& year, UINT16& month, UINT16& day) ;
void jd2000secs_to_YMDhms(signed long long int jd2000sec, INT16& year, UINT16& month, UINT16& day, UINT16& h, UINT16& m, UINT16& s) ;

