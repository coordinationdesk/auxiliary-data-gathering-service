// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 4.2  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.1  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:15  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:53  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

 
#ifndef _dbV_H_
#define _dbV_H_

#include <exException.h>
#include <string>

namespace acs {

// support class for versioning

class dbV { // PRQA S 2109
public:
	static const char* tag() noexcept ;
	static const char* name() noexcept ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dbV_H_



