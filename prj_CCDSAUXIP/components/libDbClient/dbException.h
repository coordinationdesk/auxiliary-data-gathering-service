// PRQA S 1050 EOF
/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 4.2  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.1  2012/04/13 12:23:13  marpas
	moving dbException.h from libDbBasic
	
	

*/

#ifndef _dbException_H_
#define _dbException_H_

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/*! class dbException declaration - base class for all database exceptions 
 */
exDECLARE_EXCEPTION(dbException,exException) ;      // PRQA S 2131, 2502



/*! class DbCompatibilityException declaration - This exception should everytime a method cannot be implemented because of database compatibility
 */
exDECLARE_EXCEPTION(DbCompatibilityException,dbException) ; // PRQA S 2131, 2153, 2502

_ACS_END_NAMESPACE

#endif // _dbException_H_
