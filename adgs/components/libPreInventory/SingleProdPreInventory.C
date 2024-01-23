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

	$Prod: A.C.S. libPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.11  2016/04/15 08:32:43  nicvac
	Backward compatibility: Parameter file parameter is not mandatory.
	
	Revision 1.10  2016/03/11 09:03:01  tergem
	parameter file adde to TaskIn args
	
	Revision 1.9  2016/02/18 16:35:44  nicvac
	PDSEV-27. Multiple items handling.
	
	Revision 1.8  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.7  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.6  2012/12/17 12:36:00  marpas
	compilation warning removed
	test compilation partially fixed
	
	Revision 1.5  2008/11/28 10:49:29  nicvac
	attribute renamed.
	
	Revision 1.4  2008/11/07 16:53:01  nicvac
	Main namespace in context.
	
	Revision 1.3  2008/11/06 15:22:43  nicvac
	Context extended.
	
	Revision 1.2  2008/10/23 09:11:38  nicvac
	Implementation
	
	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
	Import libPreInventory
	
        
*/ 

#include <SingleProdPreInventory.h>

#include <ConfigurationSingleton.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SingleProdPreInventory)

SingleProdPreInventory::Constants SingleProdPreInventory::c;

SingleProdPreInventory::SingleProdPreInventory() :
    PreInventory() 
{
    static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: "<< Id) ;
}

SingleProdPreInventory::~SingleProdPreInventory() throw() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}

void SingleProdPreInventory::initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &) {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; 

   //Prod PreInventory doesn't work in the scan mode
   _scanMode=false;

   _satellite = argvParameters[ c.frameworkParams[ c.satellitename ] ].as<string>(); // PRQA S 3000
   _mission = argvParameters[ c.frameworkParams[ c.missionId ] ].as<string>(); // PRQA S 3000

   rsResourceSet& conf = ConfigurationSingleton::instance()->get();
   conf.getValue( "ProdPreInventory.Definition.Name", _procName);
   conf.getValue( "ProdPreInventory.Definition.Version", _procVersion);

   //Prod PreInventory works with file
   string filetype =  argvParameters[ c.frameworkParams[ c.fileType ] ].as<string>(); // PRQA S 3000
   string itemFilename = argvParameters[ c.frameworkParams[ c.inputFile ] ].as<string>(); // PRQA S 3000
   _fileTypes.push_back( filetype );
   _itemsToTake[ filetype ].push_back( itemFilename );

   {//Fill Context

       _context.which = PreInventoryBase::Context::PROD;
       _context.metadataInfo.satelliteName = _satellite;
       _context.metadataInfo.mission = _mission;
       _context.metadataInfo.processorName = _procName;
       _context.metadataInfo.orderId = "";
       _context.metadataInfo.processorVersion = _procVersion;
       _context.metadataInfo.fileClassInfo.processingType = "";
       _context.metadataInfo.fileClassInfo.test = false;
       _context.metadataInfo.fileClassInfo.troubleShooting = false;

       //_context.metadataInfo.stationConf not set because the product to inventory could be generated on other stations

       _context.mainConfNamespace = "ProdPreInventory";
       
       _context.parameterFile = "";
       if (  argvParameters.count( c.frameworkParams[ c.parameterFile ]  )  ) {
    	   _context.parameterFile = argvParameters[ c.frameworkParams[ c.parameterFile ] ].as<string>();
       }
   }

   _outListFilename = itemFilename + ".LIST";

}



_ACS_END_NAMESPACE
