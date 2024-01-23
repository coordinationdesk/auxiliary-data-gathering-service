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

	$Prod: A.C.S. libPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.13  2016/02/18 16:35:44  nicvac
	PDSEV-27. Multiple items handling.
	
	Revision 1.12  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.11  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.10  2012/12/17 12:36:00  marpas
	compilation warning removed
	test compilation partially fixed
	
	Revision 1.9  2009/03/27 10:49:13  nicvac
	List filename fixed.
	
	Revision 1.8  2009/03/26 10:49:22  nicvac
	List file renamed to be unique.
	
	Revision 1.7  2008/11/07 16:53:01  nicvac
	Main namespace in context.
	
	Revision 1.6  2008/11/06 15:22:43  nicvac
	Context extended.
	
	Revision 1.5  2008/10/23 09:07:55  nicvac
	program_options used for argv.
	
	Revision 1.4  2008/10/13 14:44:34  nicvac
	context added.
	
	Revision 1.3  2008/10/10 16:17:57  nicvac
	Current Working dir assumed.
	
	Revision 1.2  2008/10/08 13:50:30  nicvac
	Implementation.
	
	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
	Import libPreInventory
	
        
*/ 

#include <DisPreInventory.h>

#include <disWorkOrder.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>
#include <XMLIstream.h>
#include <RegExp.h>
#include <Filterables.h>

#include <boost/format.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DisPreInventory)


DisPreInventory::DisPreInventory() :
    PreInventory()
{
    static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: "<< Id) ;
}

DisPreInventory::~DisPreInventory() throw() {
    // PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
    // PRQA L:L1
}

void DisPreInventory::initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &) {

   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"workorder="<<argvParameters["disWorkOrder"].as<string>()) ;

   //Dis Preinventory works in scan mode
   _scanMode=true;

   //Dis Preinventory works in Working Dir
   _dirNameToScan="./";

   //Interface with disWorkOrder class: needed to be loaded in Configuration Space
   {
       string disWorkOrderFilename( argvParameters["disWorkOrder"].as<string>() );

       //Merge Dis Workorder in Config: used by disWorkOrder Obj
       rsResourceSet& conf = ConfigurationSingleton::instance()->get();
       rsResourceSet::MergeMode savedMergeMode( conf.mergeMode() ); conf.mergeMode( rsResourceSet::ModeError );
       {  XMLIstream xml( disWorkOrderFilename ); xml >> conf; }
       conf.mergeMode( savedMergeMode );
       ConfigurationSingleton::instance()->set( conf );
   }

   {
       //dwo read conf
       disWorkOrder dwo;
       _satellite = dwo.getSatelliteAcsName();
       ostringstream missionStr; missionStr<<dwo.getMission();
       _mission = missionStr.str();
       _procName = dwo.getSensor();
   }


   //Get Other Info from Configuration
   rsResourceSet& conf = ConfigurationSingleton::instance()->get();

   {
       rsPushNSpace confNs(conf,"DisPreInventoryProcDef.ProcessorList"); //Lock not needed, rsPushNSpace handle mutex

       size_t countProcessor( conf.getArraySize("Processor") ); // PRQA S 4412

       ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,countProcessor<<" Entries to check") ;

       //Found the Satellite, Mission in config Space
       size_t procIdx = 0 ; 
       bool procFound(false);
       for ( size_t i=0; ( i<countProcessor) && !procFound; ++i ) { // PRQA S 4235, 4236

           string satellite; conf.getValue(boost::str(boost::format("Processor[%d].satelliteRE.value") % i), satellite ); // PRQA S 3081 2
           string mission;   conf.getValue(boost::str(boost::format("Processor[%d].missionRE.value") % i), mission );

           ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"checking ("<<_satellite<<","<<_mission<<") Vs ("<<satellite<<","<<mission<<")") ;

           RegExp satRE; satRE.setReg(satellite); RegExp misRE; misRE.setReg(mission); 
           if ( satRE.match(_satellite) && misRE.match(_mission) ) {
               procIdx=i;
               procFound=true;
           }
       }

       ACS_COND_THROW(!procFound, exDisPreInventoryCriticalException(boost::str(boost::format("DisPreInventory::initFromInput %d : ERROR -> %s %s not found in Conf Space") % __LINE__ % _satellite % _mission  ))) ; // PRQA S 3081

       //Get Other Key's Entries
       conf.getValue( boost::str(boost::format("Processor[%d].ProcessorVersion") % procIdx) , _procVersion); // PRQA S 3081
       //Get FileTypes
       conf.getArray( boost::str(boost::format("Processor[%d].FileTypes.FileType") % procIdx) , _fileTypes); // PRQA S 3081

       {
           //Fill Context
           
           _context.which  = PreInventoryBase::Context::DIS;
           _context.metadataInfo.satelliteName = _satellite;
           _context.metadataInfo.mission = _mission;
           conf.getValue( boost::str(boost::format("Processor[%d].ProcessorName") % procIdx) , _context.metadataInfo.processorName); // PRQA S 3081
           _context.metadataInfo.orderId="";
           _context.metadataInfo.processorVersion = _procVersion;
           disWorkOrder dwo;
           _context.metadataInfo.fileClassInfo.processingType = dwo.getProcessingStage();

           _context.mainConfNamespace = "DISPreInventory";

       }

   }


   {
       conf.getValue("Station.Name",   _context.metadataInfo.stationConf.name );
       conf.getValue("Station.Id",     _context.metadataInfo.stationConf.stationId );
       conf.getValue("DISPreInventory.PreInventoryListFileName", _outListFilename);
	   _outListFilename = File::getFileName( argvParameters["disWorkOrder"].as<string>() )+"_"+_outListFilename;
   }

   ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        string elems ; 
        for ( vector<string>::iterator elem=_fileTypes.begin(); elem != _fileTypes.end(); ++elem) { // PRQA S 4238
            elems+=*elem+" ";
        }
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Procname: "<<_procName<<". ProcVersion: "<<_procVersion<<". FileTypes to check: "<<elems) ;
   ACS_CLASS_END_DEBUG

}

_ACS_END_NAMESPACE
