// PRQA S 1050 EOF
/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2013/11/19 13:36:14  ricfer
	Compiler warnings removed. New logging macros adopted.
	
	Revision 2.1  2013/03/12 17:49:01  marpas
	dbCOnnectionPool is mandatory in file actions
	
	Revision 2.0  2006/03/01 16:41:23  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.1.1.1  2006/01/31 13:44:10  seralb
	Imported libAuxFileHandlingBase
	

*/

#include <AuxDownloader.h>
#include <File.h>
#include <Timer.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>

#include <sstream>

//version
#include <auxFileHandlV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    auxFileHandlV vers;
}

//constructor
AuxDownloader::AuxDownloader(dbConnectionPool &pool) :
    _connectionPool(pool) // PRQA S 2528
{
}

//destructor
AuxDownloader::~AuxDownloader() throw()
{
}

dbConnectionPool &AuxDownloader::getPool() // PRQA S 4120
{
    return _connectionPool ; // PRQA S 4024
}

string AuxDownloader::checkFileType(const string& a, const string& b)
{
	return "" ;
}

_ACS_END_NAMESPACE

