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

    $Prod: $

    $Id$

    $Author$

    $Log$
    Revision 1.21  2016/04/27 16:33:47  nicvac
    Parameter file not mandatory

    Revision 1.20  2016/04/15 08:54:22  nicvac
    Add log message about the toinventory interface file.

    Revision 1.19  2016/04/11 13:35:38  marpas
    adopting new db interfaces

    Revision 1.18  2016/03/03 13:34:52  nicvac
    Fixed xpath handling.

    Revision 1.17  2016/02/19 15:07:55  tergem
    Default jobOrder dependency removed

    Revision 1.16  2016/02/18 16:35:44  nicvac
    PDSEV-27. Multiple items handling.

    Revision 1.15  2015/12/21 10:38:17  francesco.avanzi
    TaskInPreInventory no longer takes in account orderId

    Revision 1.14  2015/07/17 15:26:20  lucio.pulvirenti
    S3MPM-549: DS context set.

    Revision 1.13  2015/06/26 08:08:16  matteo.airoldi
    Added check on JobOrder type to identify the context

    Revision 1.12  2015/06/10 08:53:37  matteo.airoldi
    Added flag to use or not use the Preinventory file name from configuration

    Revision 1.11  2015/06/09 15:12:03  marpas
    computing LIST name conditioned to configuration

    Revision 1.10  2015/05/27 12:53:40  clanas
    removed fake warning put in debug

    Revision 1.9  2014/03/19 19:20:54  marpas
    coding best practices applied
    warning fixed (qa & g++)

    Revision 1.8  2013/04/08 14:43:37  chicas
    useless include removed

    Revision 1.7  2013/02/21 18:21:20  marpas
    interface redesign to allow dbConnectionPool and explicit dbConnections
    debug improved
    coding best practices applied
    qa rules
    test fixing

    Revision 1.6  2013/01/23 16:57:05  chicas
    Minor changes

    Revision 1.5  2013/01/22 15:50:02  chicas
    Minor changes

    Revision 1.4  2013/01/10 14:13:00  chicas
    Getting Input Descriptor from ALL tasks; no more check of my taskname inside joborder

    Revision 1.3  2013/01/08 10:06:09  chicas
    minor changes

    Revision 1.2  2012/12/14 16:59:45  chicas
    removed plugin; use DefaultJobOrder

    Revision 1.1  2012/12/14 16:49:41  chicas
    added


*/ 

#include <TaskInPreInventory.h>

#include <StringKey.h>
#include <StringUtils.h>

#include <dbConnection.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <dbGenQueryScopeGuard.h>
#include <dbFileTypeQuery.h>
#include <dbFileType.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <Filterables.h>

#include <XmlDoc.h>

#include <boost/format.hpp> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskInPreInventory)

TaskInPreInventory::TaskInPreInventory() :
    PreInventory()
{
    static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: "<< Id) ;
}

TaskInPreInventory::~TaskInPreInventory() throw() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}



void TaskInPreInventory::filterFileTypes(dbConnection &conn) {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

    //Filtered fileTypes
    vector< string > filteredList;
    const string &dbKey = conn.getKey() ;
    //Check each filetype
    for ( vector<string>::iterator itFileType=_fileTypes.begin(); itFileType!=_fileTypes.end(); ++itFileType ) { // PRQA S 4238
        //Execute Query
        dbFileTypeQuery* dbFileTypeQry=dynamic_cast<dbFileTypeQuery*>( dbQueryFactoryChain::instance()->newObject("dbFileTypeQuery", dbKey) ); // PRQA S 3081, 4412
        dbQueryParameters parameters(dbKey) ;
        parameters.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::EQUAL_TO, db::toSQL(dbKey, *itFileType ))) ;
        dbFileTypeQry->addParameter( parameters );
        unsigned long countRow = dbFileTypeQry->doQuery(conn);

        //FileType not exist in DB ==> Remove it
        if ( countRow==0 ) { 
            ACS_LOG_WARNING("FileType "<<*itFileType<<" not registered in DB => ignored.")  ;
        } else {
            filteredList.push_back( *itFileType );
        }
    }
    _fileTypes = filteredList;

}

void TaskInPreInventory::initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &conn) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	//Task PreInventory do no works in scan mode
	_scanMode= false;

	_satellite = argvParameters["satellite"].as<string>();
	_mission = argvParameters["mission"].as<string>();

	_procName = argvParameters["procname"].as<string>();
	_procVersion = argvParameters["procversion"].as<string>();

	//Fill _fileTypes from JobOrder
	//Read JobOrder
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	string joborderfile = argvParameters["jobOrder"].as<string>();
	{
	
		string toInventoryFile = File::getDirPath(joborderfile) + "/ToInventory.xml";
	
		string fileToRead = joborderfile;

		if ( File::exists(toInventoryFile) ) {

			fileToRead = toInventoryFile;
			ACS_LOG_INFO( "FOUND TOINVENTORY INTERFACE FILE. TAKE INPUT LIST FROM ["<<toInventoryFile<<"] INSTEAD OF ["<<joborderfile<<"]." );
		}

		XmlDocSptr xmlDocSptr( new XmlDoc );

		xmlDocSptr->setParameters(XmlDoc::parDefault).initWithFile( fileToRead );
		
		vector<string> inputsFileType;
		string xpathfiletype="//Input/File_Type/text()";
		conf.getValue("TaskInPreInventory.Xpath.FileType", xpathfiletype, false);
		xmlDocSptr->getValue(xpathfiletype, inputsFileType);

		_fileTypes   = inputsFileType;

		//Refine from db the filetype list to process
		this->filterFileTypes(conn);

		//Get the list of items to preinventory - ${file_type} will be resolved here
		string xpathFilenameTemplate="//Input//File_Name[../../File_Type/text()='${file_type}']/text()";
		conf.getValue("TaskInPreInventory.Xpath.FileName", xpathFilenameTemplate, false);
		for ( vector<string>::const_iterator it = _fileTypes.begin(); it!=_fileTypes.end(); ++it ) {

			string xpathfilename = boost::replace_all_copy(xpathFilenameTemplate, "${file_type}", *it);
			vector<string> inputFilenames;
			xmlDocSptr->getValue(xpathfilename, inputFilenames);

			_itemsToTake[ *it ] = inputFilenames;
		}

		//no _excludeList
	}

	{//Fill Context

		XmlDocSptr xmlDocSptr( new XmlDoc );
		xmlDocSptr->setParameters(XmlDoc::parDefault).initWithFile( joborderfile );

		_context.which = PreInventoryBase::Context::DS;

		_context.metadataInfo.satelliteName = _satellite;
		_context.metadataInfo.mission = _mission;
		_context.metadataInfo.processorName = _procName;
		_context.metadataInfo.processorVersion = _procVersion;

		// get parameterFile, if exists
		string xpath = "//Parameter_File/File_Name/text()";
		conf.getValue("TaskInPreInventory.Xpath.ParameterFile", xpath, false);
		xmlDocSptr->getValue(xpath, _context.parameterFile, false);

		//_context.metadataInfo.fileClassInfo.processingType = jo.getOrderType();
		xpath="//Order_Type/text()";
		conf.getValue("TaskInPreInventory.Xpath.OrderType", xpath, false);
		xmlDocSptr->getValue(xpath, _context.metadataInfo.fileClassInfo.processingType);

        	//_context.metadataInfo.fileClassInfo.test = jo.isTest();
		xpath="//Test/text()";
		conf.getValue("TaskInPreInventory.Xpath.Test", xpath, false);
		xmlDocSptr->getValue(xpath, _context.metadataInfo.fileClassInfo.test);

        	//_context.metadataInfo.fileClassInfo.troubleShooting = jo.isTroubleshooting();
		xpath="//Troubleshooting/text()";
		conf.getValue("TaskInPreInventory.Xpath.Troubleshooting", xpath, false);
		xmlDocSptr->getValue(xpath, _context.metadataInfo.fileClassInfo.troubleShooting);

		conf.getValue("Station.Name",   _context.metadataInfo.stationConf.name );
		conf.getValue("Station.Id",     _context.metadataInfo.stationConf.stationId );

		_context.mainConfNamespace = "TaskInPreInventory";

	}

	{
		bool useConfName = false ;
		conf.getValue("TaskInPreInventory.UsePreInvName", useConfName, false );
		conf.getValue("TaskInPreInventory.PreInventoryListFileName", _outListFilename);

		//If just one file to Preinventory - Retrocompatibility
		if (!useConfName && _itemsToTake.size() == 1 && _itemsToTake.begin()->second.size() == 1 ) {

			string filename = _itemsToTake.begin()->second.at(0);
			_outListFilename = filename + ".LIST";

		} else {
			_outListFilename="OrderId_"+_context.metadataInfo.orderId+"_"+_outListFilename;
		}
	}


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		string elems ;
		for ( vector<string>::iterator elem=_fileTypes.begin(); elem != _fileTypes.end(); ++elem  ) { // PRQA S 4238
			elems+=*elem+" ";
		}

		string exclude ;
		for ( vector<string>::iterator elem=_excludeList.begin(); elem != _excludeList.end(); ++elem ) {  // PRQA S 4238
			exclude+=*elem+" ";
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Procname: "<<_procName<<". ProcVersion: "<<_procVersion<<". FileTypes to check: "<<elems<<". Exclude list: "<<exclude)  ;
	ACS_CLASS_END_DEBUG
}




_ACS_END_NAMESPACE
