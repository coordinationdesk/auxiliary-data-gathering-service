/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2007/02/05 11:45:31  marant
	Ers satellite string corrected
	
	Revision 2.1  2006/10/18 16:39:38  marpas
	new interfaces to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/11/17 18:52:27  giucas
	Added EnvisatOBTConverter
	
	Revision 1.5  2005/11/02 17:53:43  giucas
	Modified to use Singleton
	
	Revision 1.4  2005/09/12 14:57:32  seralb
	Added Spot satellite
	
	Revision 1.3  2005/07/20 14:08:57  seralb
	 Ers changed in ERS
	
	Revision 1.2  2005/07/06 10:00:15  seralb
	Added Ers classes
	
	Revision 1.1.1.1  2004/12/07 10:06:15  giucas
	Import libOBTConverter
	
	Revision 1.4  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.3  2004/10/08 09:02:33  giucas
	Added EnvisatOBTFileParser
	
	Revision 1.2  2004/09/17 12:37:57  giucas
	Changed interfaces and added builder methods for OBTFileParser objects
	
	Revision 1.1  2004/09/16 16:57:32  giucas
	First release
*/

#include <OBTConverterFactory.h>
#include <GoceOBTConverter.h>
#include <ErsOBTConverter.h>
#include <OBTConverterSingleton.h>
#include <EnvisatOBTConverter.h>

#include <obtConvV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OBTConverterFactory)

namespace{
	obtConvV versObj;
}

//factory constructor
OBTConverterFactory::OBTConverterFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	OBTConverterSingleton::instance()->registerFactory(string("GoceOBTConverter") 	, &buildGoceOBTConverter    );
	OBTConverterSingleton::instance()->registerFactory(string("ErsOBTConverter") 	, &buildErsOBTConverter    	);
	OBTConverterSingleton::instance()->registerFactory(string("EnvisatOBTConverter") , &buildEnvisatOBTConverter );
}

//factory destructor
OBTConverterFactory::~OBTConverterFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	OBTConverterSingleton::instance()->unregisterFactory(string("GoceOBTConverter")    	, &buildGoceOBTConverter    );
	OBTConverterSingleton::instance()->unregisterFactory(string("ErsOBTConverter")    	, &buildErsOBTConverter     );
	OBTConverterSingleton::instance()->unregisterFactory(string("EnvisatOBTConverter") 	, &buildEnvisatOBTConverter );
}

///////////////////////
// builder methods
///////////////////////

OBTConverter* OBTConverterFactory::buildGoceOBTConverter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new GoceOBTConverter;
}

OBTConverter* OBTConverterFactory::buildErsOBTConverter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new ErsOBTConverter;
}

OBTConverter* OBTConverterFactory::buildEnvisatOBTConverter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new EnvisatOBTConverter;
}
_ACS_END_NAMESPACE
