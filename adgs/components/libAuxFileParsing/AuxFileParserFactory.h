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
	Revision 2.7  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.6  2009/03/11 08:46:09  giucas
	Radarsat PO parser specific added
	
	Revision 2.5  2008/12/16 13:51:49  giucas
	Alos PO and OS registrations moved into specific libAlosAuxFileParsing factory object
	
	Revision 2.4  2008/12/16 10:16:24  giucas
	Added Alos PO and OS file parsers
	
	Revision 2.3  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.2  2006/09/27 10:11:12  crivig
	added new parser for Spot State vector
	
	Revision 2.1  2006/07/10 12:25:26  giucas
	Added PO keys for Landsat, Ers, Jers, Eosam and Eospm returning ACSSVFileParser object
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2005/11/16 18:23:28  giucas
	Added EnvisatOSFileParser
	
	Revision 1.9  2005/11/16 10:33:19  giucas
	Added EnvisatPOFileParser
	
	Revision 1.8  2005/11/07 14:33:11  seralb
	Modify Spot and Ers build methods
	
	Revision 1.7  2005/10/26 09:19:36  giucas
	Cosmo factory methods moved to CosmoAuxFileParsing
	
	Revision 1.6  2005/10/26 08:03:54  giucas
	Modified to use singleton
	
	Revision 1.5  2005/10/25 14:05:36  giucas
	Added PO and OSF parsers for COSMO.They use CRYOSAT implementations
	
	Revision 1.4  2005/09/16 11:51:50  seralb
	Added ErsOrbitDataFileParser
	
	Revision 1.3  2005/09/12 14:36:42  seralb
	Added Spot satellite
	
	Revision 1.2  2005/07/01 14:10:36  seralb
	Added ERS satellite
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.3  2004/12/07 13:44:41  giucas
	Added factory method for OrbitScenarioFileParser
	
	Revision 1.2  2004/12/07 10:09:55  giucas
	Added newObtFileParser method
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	

*/

#ifndef _AuxFileParserFactory_H_
#define _AuxFileParserFactory_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class AuxFileParser;

class AuxFileParserFactory // PRQA S 2109
{
public:

	/**
	  * \brief Factory class constructor registers instances of OBTConverter objects
	  */
	AuxFileParserFactory();

	/**
	  * \brief Factory class destructor unregisters instances of OBTConverter objects
	  */				
	~AuxFileParserFactory();

////////////////////////////////////////////////////////////////////////////////////////////
// Specific builder methods
////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  * \brief Builder method for CRYOSAT PredictedOrbitFileParser object retrieving
	  */								
	static AuxFileParser* buildCryosatPOFileParser();

	/**
	  * \brief Builder method for ENVISAT PredictedOrbitFileParser object retrieving
	  */								
	static AuxFileParser* buildEnvisatPOFileParser();
	
	/**
	  * \brief Builder method for ACS AOCS PredictedOrbitFileParser object retrieving
	  */								
	static AuxFileParser* buildAOCSacsFileParser();
	
	/**
	  * \brief Builder method for GOCE OBTFileParser object retrieving
	  */								
	static AuxFileParser* buildGoceOBTFileParser();

	/**
	  * \brief Builder method for ENVISAT OBTFileParser object retrieving
	  */								
	static AuxFileParser* buildEnvisatOBTFileParser();

	/**
	  * \brief Builder method for CRYOSAT OrbitScenarioFile Parser object retrieving
	  */								
	static AuxFileParser* buildCryosatOSFileParser();

	/**
	  * \brief Builder method for ENVISAT OrbitScenarioFile Parser object retrieving
	  */								
	static AuxFileParser* buildEnvisatOSFileParser();

	/**
	  * \brief Builder method for ERS FileParser object retrieving
	  */								
	static AuxFileParser* buildErsOBTFileParser();

	/**
	  * \brief Builder method for ERS FileParser object retrieving
	  */								
	static AuxFileParser* buildErsOrbitDataFileParser();

	/**
	  * \brief Builder method for SPOT FileParser object retrieving
	  */								
	static AuxFileParser* buildACSSVFileParser();

	/**
	  * \brief Builder method for RADARSAT ORBIT DATA  FileParser object retrieving
	  */								
	static AuxFileParser* buildRadarsatPOFileParser();
private:
	AuxFileParserFactory(const AuxFileParserFactory &); // not implemented 
	AuxFileParserFactory& operator= (const AuxFileParserFactory &); // not implemented 

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AuxFileParserFactory) ;

};

_ACS_END_NAMESPACE

#endif // _AuxFileParserFactory_H_

