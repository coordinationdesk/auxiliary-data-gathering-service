// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 2.14  2014/03/27 12:44:39  marpas
	coding best practices applied
	interface rationalization
	warnings fixed (qa & g++)
	
	Revision 2.13  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.12  2010/12/09 17:56:49  masdal
	added Sacd parsers
	
	Revision 2.11  2009/11/26 10:35:59  danalt
	added Oceansat parsers
	
	Revision 2.10  2009/03/11 08:46:09  giucas
	Radarsat PO parser specific added
	
	Revision 2.9  2009/02/23 16:44:06  danalt
	added Radarsat PO and OSF parser
	
	Revision 2.8  2008/12/16 13:51:49  giucas
	Alos PO and OS registrations moved into specific libAlosAuxFileParsing factory object
	
	Revision 2.7  2008/12/16 10:16:24  giucas
	Added Alos PO and OS file parsers
	
	Revision 2.6  2006/11/16 14:16:04  giucas
	Fixed ERS registration keys: satellite name must be Ers and not ERS
	
	Revision 2.5  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.4  2006/09/27 10:11:11  crivig
	added new parser for Spot State vector
	
	Revision 2.3  2006/07/10 12:30:22  giucas
	Added OS keys for Landsat, Ers, Jers, Eosam and Eospm returning CryosatOSFileParser
	
	Revision 2.2  2006/07/10 12:25:26  giucas
	Added PO keys for Landsat, Ers, Jers, Eosam and Eospm returning ACSSVFileParser object
	
	Revision 2.1  2006/03/20 13:05:40  seralb
	Added comment
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/11/16 18:23:28  giucas
	Added EnvisatOSFileParser
	
	Revision 1.10  2005/11/16 10:33:19  giucas
	Added EnvisatPOFileParser
	
	Revision 1.9  2005/11/07 14:33:11  seralb
	Modify Spot and Ers build methods
	
	Revision 1.8  2005/10/26 09:19:36  giucas
	Cosmo factory methods moved to CosmoAuxFileParsing
	
	Revision 1.7  2005/10/26 08:03:54  giucas
	Modified to use singleton
	
	Revision 1.6  2005/10/25 14:05:36  giucas
	Added PO and OSF parsers for COSMO.They use CRYOSAT implementations
	
	Revision 1.5  2005/09/16 11:51:50  seralb
	Added ErsOrbitDataFileParser
	
	Revision 1.4  2005/09/12 14:36:42  seralb
	Added Spot satellite
	
	Revision 1.3  2005/07/20 14:09:31  seralb
	 Ers changed in ERS
	
	Revision 1.2  2005/07/01 14:10:36  seralb
	Added ERS satellite
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.4  2004/12/07 14:34:25  giucas
	Removed commented methods
	
	Revision 1.3  2004/12/07 13:44:41  giucas
	Added factory method for OrbitScenarioFileParser
	
	Revision 1.2  2004/12/07 10:09:55  giucas
	Added newObtFileParser method
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	

*/

#include <AuxFileParserFactory.h>

#include <AuxFileParser.h>
#include <AuxFileParserSingleton.h>

#include <CryosatPOFileParser.h>
#include <EnvisatPOFileParser.h>
#include <AOCSacsFileParser.h>
#include <ACSSVFileParser.h>
#include <GoceOBTFileParser.h>
#include <EnvisatOBTFileParser.h>
#include <CryosatOSFileParser.h>
#include <EnvisatOSFileParser.h>
#include <auxFileParsV.h>
#include <ErsOBTFileParser.h>
#include <ErsOrbitDataFileParser.h>
#include <RadarsatPOFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AuxFileParserFactory);
namespace {
    auxFileParsV vers;
}
//factory constructor
AuxFileParserFactory::AuxFileParserFactory()
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "START.") ;

///////////////////////////////////////////////////////////////////////////////////////
//		Predicted Orbit File Parsers					     //							
///////////////////////////////////////////////////////////////////////////////////////
	registerAuxFileParserFactoryMethod("CryosatPOFileParser"	,&buildCryosatPOFileParser );
	registerAuxFileParserFactoryMethod("EnvisatPOFileParser"	,&buildEnvisatPOFileParser );
	registerAuxFileParserFactoryMethod("ProbavPOFileParser"		,&buildAOCSacsFileParser );
	registerAuxFileParserFactoryMethod("Alsat1bPOFileParser"	,&buildAOCSacsFileParser );
	registerAuxFileParserFactoryMethod("LandsatPOFileParser"	,&buildACSSVFileParser );
	registerAuxFileParserFactoryMethod("ErsPOFileParser"		,&buildACSSVFileParser );
	registerAuxFileParserFactoryMethod("EosamPOFileParser"		,&buildACSSVFileParser );
	registerAuxFileParserFactoryMethod("EospmPOFileParser"		,&buildACSSVFileParser );
	registerAuxFileParserFactoryMethod("JersPOFileParser"		,&buildACSSVFileParser );
	registerAuxFileParserFactoryMethod("RadarsatPOFileParser"	,&buildRadarsatPOFileParser );
	registerAuxFileParserFactoryMethod("OceansatPOFileParser"	,&buildACSSVFileParser );
	registerAuxFileParserFactoryMethod("SacdPOFileParser"       ,&buildACSSVFileParser );
	
///////////////////////////////////////////////////////////////////////////////////////
//		Time Correlation File Parsers	                                     //							
///////////////////////////////////////////////////////////////////////////////////////
	registerAuxFileParserFactoryMethod("GoceOBTFileParser"		,&buildGoceOBTFileParser       );	
	registerAuxFileParserFactoryMethod("EnvisatOBTFileParser"	,&buildEnvisatOBTFileParser    );	
	registerAuxFileParserFactoryMethod("ErsOBTFileParser"		,&buildErsOBTFileParser        ); /*This method build a ErsOrbitDataFileParser*/	

///////////////////////////////////////////////////////////////////////////////////////
//		Orbit Scenario File Parsers					     //							
///////////////////////////////////////////////////////////////////////////////////////
	registerAuxFileParserFactoryMethod("CryosatOSFileParser"	,&buildCryosatOSFileParser );	
	registerAuxFileParserFactoryMethod("EnvisatOSFileParser"	,&buildEnvisatOSFileParser );		
	registerAuxFileParserFactoryMethod("LandsatOSFileParser"	,&buildCryosatOSFileParser );	
	registerAuxFileParserFactoryMethod("ErsOSFileParser"		,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("EosamOSFileParser"		,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("EospmOSFileParser"		,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("JersOSFileParser"		,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("RadarsatOSFileParser"	,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("OceansatOSFileParser"	,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("SacdOSFileParser"       ,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("ProbavOSFileParser"		,&buildCryosatOSFileParser );
	registerAuxFileParserFactoryMethod("Alsat1bOSFileParser"	,&buildCryosatOSFileParser );
	
///////////////////////////////////////////////////////////////////////////////////////
//		Orbit Data File Parsers						     //							
///////////////////////////////////////////////////////////////////////////////////////
	registerAuxFileParserFactoryMethod("ErsOrbitDataFileParser"		,&buildErsOrbitDataFileParser );	

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "END.") ;
}

//factory destructor
AuxFileParserFactory::~AuxFileParserFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "START.") ; // PRQA S 4631 L1

	unregisterAuxFileParserFactoryMethod("CryosatPOFileParser"   	,&buildCryosatPOFileParser		);
	unregisterAuxFileParserFactoryMethod("EnvisatPOFileParser"		,&buildEnvisatPOFileParser 		);
	unregisterAuxFileParserFactoryMethod("ProbavPOFileParser"		,&buildAOCSacsFileParser 		);
	unregisterAuxFileParserFactoryMethod("Alsat1bPOFileParser"		,&buildAOCSacsFileParser 		);
	unregisterAuxFileParserFactoryMethod("LandsatPOFileParser"		,&buildACSSVFileParser 			);
	unregisterAuxFileParserFactoryMethod("ErsPOFileParser"			,&buildACSSVFileParser 			);
	unregisterAuxFileParserFactoryMethod("EosamPOFileParser"		,&buildACSSVFileParser 			);
	unregisterAuxFileParserFactoryMethod("EospmPOFileParser"		,&buildACSSVFileParser 			);
	unregisterAuxFileParserFactoryMethod("JersPOFileParser"			,&buildACSSVFileParser 			);
	unregisterAuxFileParserFactoryMethod("RadarsatPOFileParser"		,&buildRadarsatPOFileParser 	);
	unregisterAuxFileParserFactoryMethod("OceansatPOFileParser"		,&buildACSSVFileParser 	);
	unregisterAuxFileParserFactoryMethod("SacdPOFileParser"         ,&buildACSSVFileParser  );
	unregisterAuxFileParserFactoryMethod("GoceOBTFileParser"		,&buildGoceOBTFileParser		);	
	unregisterAuxFileParserFactoryMethod("EnvisatOBTFileParser"		,&buildEnvisatOBTFileParser		);
	unregisterAuxFileParserFactoryMethod("CryosatOSFileParser"		,&buildCryosatOSFileParser		);
	unregisterAuxFileParserFactoryMethod("EnvisatOSFileParser"		,&buildEnvisatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("LandsatOSFileParser"		,&buildCryosatOSFileParser 		);	
	unregisterAuxFileParserFactoryMethod("ErsOSFileParser"			,&buildCryosatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("EosamOSFileParser"		,&buildCryosatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("EospmOSFileParser"		,&buildCryosatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("JersOSFileParser"			,&buildCryosatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("RadarsatOSFileParser"		,&buildCryosatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("ErsOBTFileParser"			,&buildErsOBTFileParser			);
	unregisterAuxFileParserFactoryMethod("ErsOrbitDataFileParser"	,&buildErsOrbitDataFileParser	);
	unregisterAuxFileParserFactoryMethod("OceansatOSFileParser"		,&buildCryosatOSFileParser 		);
	unregisterAuxFileParserFactoryMethod("SacdOSFileParser"			,&buildCryosatOSFileParser      );
	unregisterAuxFileParserFactoryMethod("ProbavOSFileParser"		,&buildCryosatOSFileParser      );
	unregisterAuxFileParserFactoryMethod("Alsat1bOSFileParser"		,&buildCryosatOSFileParser      );
	

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "END.") ; // PRQA L:L1
}

///////////////////////
// builder methods
///////////////////////

AuxFileParser* AuxFileParserFactory::buildCryosatPOFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new CryosatPOFileParser;
}

AuxFileParser* AuxFileParserFactory::buildEnvisatPOFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new EnvisatPOFileParser;
}

AuxFileParser* AuxFileParserFactory::buildAOCSacsFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new AOCSacsFileParser;
}

AuxFileParser* AuxFileParserFactory::buildGoceOBTFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new GoceOBTFileParser;
}

AuxFileParser* AuxFileParserFactory::buildEnvisatOBTFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new EnvisatOBTFileParser;
}

AuxFileParser* AuxFileParserFactory::buildCryosatOSFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new CryosatOSFileParser;
}

AuxFileParser* AuxFileParserFactory::buildEnvisatOSFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new EnvisatOSFileParser;
}

AuxFileParser* AuxFileParserFactory::buildErsOBTFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new ErsOrbitDataFileParser;
}

AuxFileParser* AuxFileParserFactory::buildErsOrbitDataFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new ErsOrbitDataFileParser;
}

AuxFileParser* AuxFileParserFactory::buildACSSVFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new ACSSVFileParser;
}

AuxFileParser* AuxFileParserFactory::buildRadarsatPOFileParser()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;		
	return new RadarsatPOFileParser;
}

_ACS_END_NAMESPACE
