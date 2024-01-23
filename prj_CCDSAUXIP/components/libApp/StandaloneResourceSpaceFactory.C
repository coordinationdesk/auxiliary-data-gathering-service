// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2018/09/28 09:34:39  marpas
	Standalone versions of resource space handler classes have been moved here from libDbConfiguration
	
	Revision 5.1  2017/09/25 15:45:13  lucio.pulvirenti
	S2PDGS-1843: Passed optional boolean parameter to be passed to objects CTOR
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 1.7  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 1.6  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 1.5  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 1.4  2010/02/22 15:36:03  enrcar
	EC:: DEBUG improved
	
	Revision 1.3  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 1.2  2008/06/04 16:02:44  enrcar
	EC:: Operation02::CheckData method called
	
	Revision 1.1  2006/04/20 12:25:45  clanas
	imported sources
	
	Revision 2.0  2006/02/28 09:09:04  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/11/04 15:34:20  marpas
	check moved away from ctor
	
	Revision 1.6  2005/11/03 13:51:51  marpas
	Operation02::chjeckData method called
	
	Revision 1.5  2005/07/21 10:50:16  marpas
	file actions for dbConfiguration implemented
	
	Revision 1.4  2005/07/21 10:24:43  marpas
	dbConfigurationSpace and its query imported from libDbCryosat
	
	Revision 1.3  2005/03/17 12:42:56  marpas
	headers fixed
	
	Revision 1.2  2003/04/30 14:01:54  marpas
	using namespace std no longer fount into acs includes
	
	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
	libDbConfiguration Import
		

*/


#include <StandaloneResourceSpaceFactory.h>
#include <StandaloneResourceSpace.h>
#include <Operation02.h>
#include <Application.h>
#include <Filterables.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(StandaloneResourceSpaceFactory)

// constructor
StandaloneResourceSpaceFactory::StandaloneResourceSpaceFactory() : 
	ResourceSpaceFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


StandaloneResourceSpaceFactory::~StandaloneResourceSpaceFactory() ACS_NOEXCEPT {}



ResourceSpace *StandaloneResourceSpaceFactory::createItem(const string &space, const string &version, bool absPath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

    Operation02::checkData(Application::appName()) ;
	return new StandaloneResourceSpace(space,version, absPath) ;
}



_ACS_END_NAMESPACE
