// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:05  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.0  2013/09/30 12:31:05  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:33  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:51  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/30 22:48:00  paoscu
	headers fixed
	
	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
	Import libDbCryosat
	

*/

#ifndef _dbcsV_H_
#define _dbcsV_H_

namespace acs {

// support class for versioning

class dbcsV { // PRQA S 2109
public:
	dbcsV() throw() ;
	~dbcsV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not implemnted to prevent their use
	dbcsV(const dbcsV &) ;
	dbcsV & operator=(const dbcsV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dbcsV_H_

