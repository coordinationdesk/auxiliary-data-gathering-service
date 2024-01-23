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
	
	Revision 1.5  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 1.4  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 1.3  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 1.2  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 1.1  2006/04/20 12:25:45  clanas
	imported sources
	
		

*/


#ifndef _StandaloneResourceSpaceFactory_H_
#define _StandaloneResourceSpaceFactory_H_ 


#include <acs_c++config.hpp>
#include <ResourceSpaceFactory.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent;
class dbGeneralQuery;
//
//
//
// class StandaloneResourceSpaceFactory
//
//
//
class StandaloneResourceSpaceFactory : public ResourceSpaceFactory // PRQA S 2109, 2153
{
public:

	StandaloneResourceSpaceFactory();
	virtual ~StandaloneResourceSpaceFactory() ACS_NOEXCEPT ;

	virtual ResourceSpace *createItem(const std::string &space, const std::string &version, bool absPath = false) ;
		
private:
	// declared but not implemented to prevent their use
	StandaloneResourceSpaceFactory(const StandaloneResourceSpaceFactory &) ;
	StandaloneResourceSpaceFactory &operator=(const StandaloneResourceSpaceFactory &) ;
		
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StandaloneResourceSpaceFactory) ;
	
} ; 

_ACS_END_NAMESPACE


#endif // _StandaloneResourceSpaceFactory_H_
