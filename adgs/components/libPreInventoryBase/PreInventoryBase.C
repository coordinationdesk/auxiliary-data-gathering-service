// PRQA S 1050 EOF
/*
	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPreInventoryBase$

	$Id$

	$Author$

	$Log$
	Revision 1.48  2017/07/27 16:02:17  lucio.pulvirenti
	ECICMF-108: doVersioningByFileInventoryVersion method added to compute version using FileInventoryVersion class.
	
	Revision 1.47  2016/04/11 13:35:22  marpas
	adopting new db interfaces
	
	Revision 1.46  2016/03/26 21:39:01  marpas
	using new db I/F
	
	Revision 1.45  2016/02/18 16:33:49  nicvac
	PDSEV-27: Multiple items handling.
	
	Revision 1.44  2015/12/21 10:40:39  francesco.avanzi
	libPreInventoryBase no longer takes in account orderId
	OrderId will be set by each Plugin in need of setting it
	
	Revision 1.43  2015/12/10 14:07:12  francesco.avanzi
	now handling orderId, if needed[C
	
	Revision 1.42  2015/10/06 15:15:40  lucio.pulvirenti
	S3MPM-628: convertUncorrectDate IF changed: bool ptr added (default 0) to be valorized to true if date has been converted.
	
	Revision 1.41  2015/07/17 15:24:37  lucio.pulvirenti
	S3MPM-549: DS context added.
	
	Revision 1.40  2015/06/12 10:46:03  francesco.avanzi
	best code practices applied
	getContext() method added
	such method is non-essential, but precedes the privatization of _context variable
	
	Revision 1.39  2015/05/25 14:16:05  marpas
	coding best practice applied
	
	Revision 1.38  2015/03/19 11:33:27  lucio.pulvirenti
	S3MPM-359: getSatMissionAssociatorFactoryKey public method added to IF.
	
	Revision 1.37  2015/03/04 10:47:41  marpas
	removing qa warnings
	
	Revision 1.36  2015/01/27 16:15:56  lucio.pulvirenti
	removeSpuriousCharsFromStringDate method added. In base class remove just trailing 'Z' char.
	
	Revision 1.35  2014/09/17 13:46:24  marfav
	S1PDGS-31538: stripping the path from the filename when inserting inventory anomaly
	
	Revision 1.34  2014/06/23 10:50:58  lucio.pulvirenti
	ADC-265: processFile: if versioning is enabled NameValue, node added to metadata with Versioning key and true value.
	
	Revision 1.33  2014/05/21 08:34:07  marfav
	Fixed a string creation confusing the compiler in some cases
	
	Revision 1.32  2014/02/11 11:26:35  marpas
	fixing compilation warning
	
	Revision 1.31  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.30  2013/11/13 13:08:19  lucio.pulvirenti
	dbConnection key passed to both dbQueryParameters and dbParam CTORs.
	
	Revision 1.29  2013/11/13 10:55:57  lucio.pulvirenti
	convertUncorrectDate method added to convert wrong dates (e.g. 9999-99-99T99:99:99) according to a configured LUT.
	
	Revision 1.28  2013/11/12 16:42:01  lucio.pulvirenti
	Method added devoted to convert file class read from file to inventory into file class occurring in db. LUT for conversion is
	extracted from configuration.
	
	Revision 1.27  2013/10/29 16:27:13  marfav
	Anomaly insertion does not cause failures
	
	Revision 1.26  2013/10/29 16:07:40  marfav
	Filterable warnings removed
	
	Revision 1.25  2013/10/29 14:48:07  marfav
	Inventory anomalies support added
	
	Revision 1.24  2013/04/29 16:27:18  marpas
	interface changed
	file type recognition class interface changed
	
	Revision 1.23  2013/03/01 15:41:03  lucio.pulvirenti
	processFile: TrustOnPluginFiletype got from configuration to decide to trust on metadata file type
	from plugin or not. Default behaviour is to trust, but if flag is false, file type from Framework is set.
	
	Revision 1.22  2013/02/21 11:13:24  marpas
	redesign to use connection pool and explicit connections
	coding best practices added
	test implementation completely rewritten
	qa rules
	
	Revision 1.21  2012/12/17 11:24:04  marpas
	inserter for PreInventoryBase::Context::Which added
	
	Revision 1.20  2012/12/11 17:28:01  lucio.pulvirenti
	fillMetadataFilename method developed filling filename field of Metadata class by writing the file with or without ext
	depending on mngsngext value of the proper t_filetypes records.
	getCoupledPath: not empty coupled path returned if number of expected extension is 2 and not only
	if the current file is of HEADER type.
	
	Revision 1.19  2012/12/06 14:50:07  lucio.pulvirenti
	getCoupledPath private method implemented getting coupled data path for header path input and empty string otherwise.
	doPackaging now accepts as input two filepath ( either (hdr, dat) or (dat, "") ) and get the filepath to inventory.
	getAbsoluteMtdFName private method implemented adding mtd extension to input filepath.
	getFileTypeRegEx: boost removed.
	
	Revision 1.18  2012/02/21 13:02:35  marpas
	refactoring
	
	Revision 1.17  2011/08/31 10:37:01  chicas
	bug fixed in getMtdFileName (use getGlobalFileName)
	
	Revision 1.16  2011/07/06 10:45:52  giufor
	implemented the virtual method getMtdFileName to get the correct filename of mtd file and updated the dowork method
	
	Revision 1.15  2011/05/30 13:12:10  chicas
	Now use metadata.getGlobalfileName for managing product dir
	
	Revision 1.14  2010/09/22 09:11:28  marpas
	debug improved (merge from PROSA_V2_BR1)
	
	Revision 1.13  2009/09/28 17:02:11  marpas
	dbParam interface obsolecence
	
	Revision 1.12  2009/03/25 15:40:54  nicvac
	Added overrideVersionHandling to let plugins to override doVersioning decision.
	
	Revision 1.11  2008/12/19 17:08:35  nicvac
	Mtd extension from Inventory conf space.
	
	Revision 1.10  2008/11/28 15:14:12  nicvac
	Deep Search implemented.
	
	Revision 1.9  2008/11/28 11:44:32  nicvac
	Log added.
	
	Revision 1.8  2008/11/07 16:51:34  nicvac
	preProcessing implemented.
	
	Revision 1.7  2008/10/29 17:10:53  nicvac
	doPackaging method added.
	
	Revision 1.6  2008/10/29 15:44:08  nicvac
	computeMetadata signature changed.
	
	Revision 1.5  2008/10/23 09:00:29  nicvac
	Check on _path fixed.
	
	Revision 1.4  2008/10/13 14:49:48  nicvac
	Path with file passed to plugin.
	Context struct added.
	
	Revision 1.3  2008/10/10 16:27:55  nicvac
	getRegExp implemented.
	
	Revision 1.2  2008/10/08 13:55:29  nicvac
	Attributes and interface updated.
	
	Revision 1.1.1.1  2008/09/30 14:35:50  nicvac
	Import libPreInventoryBase
	
        
*/ 

#include <PreInventoryBase.h>
#include "PreInventoryBaseV.h"
#include <Filterables.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <RegExp.h>
#include <Dir.h>
#include <File.h>

#include <FileInventoryVersion.h>
#include <dbConnectionPool.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>
#include <dbFileTypeQuery.h>
#include <dbFileType.h>
#include <dbFileTypeRecognition.h>
#include <dbSmartPersistent.h>

#include <AnomalyTypes.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    PreInventoryBaseV PreInventoryBaseVersion;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(PreInventoryBase)


PreInventoryBase::PreInventoryBase():
   _context(),
   _scanMode(false),
   _path(""),
   _excludeList(),
   _filesPreInventoried(),
   _selectedFiletypes(),
   _inputFiles(),
   _manageInventoryAnomalies(false),
   _timeLUTElements(),
   _pluginConfSpaceId(),
   _factoryConfSpaceId()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;
}

void PreInventoryBase::setPath( const std::string& path, const std::vector<std::string>& excludeList ) {

    ACS_COND_THROW( _inputFiles.size() > 0, exPreInventoryBaseCriticalException("PreInventoryBase::setPath : Parameters already set.") ); // PRQA S 3081

    _scanMode = true,
    _path = path;
    _excludeList = excludeList;
}
void PreInventoryBase::addItem(const std::string& pathFileName, const std::string& fileType) {

    ACS_COND_THROW( _path!="", exPreInventoryBaseCriticalException("PreInventoryBase::setFilename : Parameters already set.") ); // PRQA S 3081

    _scanMode = false;
    _inputFiles.push_back( Pair_FileName_FileType(pathFileName,fileType) );
}

PreInventoryBase::~PreInventoryBase() throw() { 
// PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;
// PRQA L:L1
}

void PreInventoryBase::setContext( const Context& context ) { // PRQA S 4121
    _context = context;
}


void PreInventoryBase::addFileType(const std::string& fileType) {
    _selectedFiletypes.push_back(fileType);
}


vector< Pair_FileName_FileType > PreInventoryBase::preProcess( const Pair_FileName_FileType& inputFile, dbConnection & ) {
    //Default Beaviour: the returned vector has only the inputFile passed.
    vector< Pair_FileName_FileType > filesToInventory( 1, inputFile );
    return filesToInventory;
}

vector< Pair_FileName_FileType > PreInventoryBase::preProcess( const vector< Pair_FileName_FileType >& inputFiles, dbConnection & conn ) {
    //Default Beaviour: the returned vector is the input one 
    vector< Pair_FileName_FileType > filesToInventory;

    for ( vector< Pair_FileName_FileType >::const_iterator inFile=inputFiles.begin(); inFile!=inputFiles.end(); inFile++ ) {

        vector< Pair_FileName_FileType > parentAndChildProducts;
        parentAndChildProducts = preProcess(*inFile, conn);

        filesToInventory.insert( filesToInventory.end(), parentAndChildProducts.begin(), parentAndChildProducts.end() );
    }

    return filesToInventory;
}

string PreInventoryBase::processFile(const string &first, const string &second, dbConnection &theConnection)
{
    Metadata metadata;

	try
    {
	        this->computeMetadata( first, second, metadata, theConnection );
	}
	catch (exception& ex)
	{
        ostringstream os;
        os << "Unable to extract metadata from product " << first;
		ACS_THROW (exMetadataComputationException (ex, os.str()));
	}
	
	if( getContext().which == getContext().TL ) {

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TL context, going to set Order_Id in metadata");
		string orderId = metadata.getMetadataValue("Order_Id", false);
		if( orderId.empty() ) {
			int contextOrderId = 0;
			istringstream is( getContext().metadataInfo.orderId );
			is >> contextOrderId;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Order Id from context=" << contextOrderId );
			if( contextOrderId != 0 ) {
				metadata.setMetadataValue("Order_Id", contextOrderId );
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Order Id set in metadata");
			}
			else {
				// order id not set
			}
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Order_Id already set in metadata: \"" << orderId << '\"');
		}
	}
	else {
		// empty
	}

        bool isVersioningEnabled(true); 
        {
        	//Get from Configuration if I must manage Versioning
            rsResourceSet& conf = ConfigurationSingleton::instance()->get();
            rsPushNSpace confNs(conf, _context.mainConfNamespace ); 
            conf.getValue("Versioning.isEnabled.value", isVersioningEnabled);
			
 			// get from configuration if file type must be either imposed from framework or left the one written from plugin (default) 
			bool trustOnPluginFiletype = true;
			try {
        		conf.getValue("TrustOnPluginFiletype", trustOnPluginFiletype);
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "TrustOnPluginFiletype from configuration is " << boolalpha << trustOnPluginFiletype) ;
			}
			catch(rsResourceSet::NotFoundException &e)
			{}
			
			if( !trustOnPluginFiletype ) {
				metadata.setMetadataValue( "File_Type", second);
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "metadata File_Type field just set to \"" << second << "\"") ;
			}
       }

		//Beside the general setting, the plugin can override the Versioning handling
		bool doVersioningByPlugin(false);
		if ( this->overrideVersionHandling( doVersioningByPlugin ) ) {
			isVersioningEnabled=doVersioningByPlugin;
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Versioning enabled: " << boolalpha << isVersioningEnabled) ;
		}

        if ( isVersioningEnabled ) {
			
// write it into metadata file
			metadata.addNameValue(MetadataNameValues::getVersioning(), "true") ;
			
            //Check and Get for a new Version
            string newVersion ;
            bool isNewVersion( this->doVersioning( metadata, newVersion, theConnection ) ); 

            if ( isNewVersion ) {
                this->updateProductAndMetadata( newVersion, metadata );
            }
        }

        //Packaging
		string fileOnDiskToInventory;
		{
			string coupledPath = getCoupledPath(first, second, theConnection); 
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, ( coupledPath.empty() ? "No file" : string("\"" + coupledPath + "\" path is") ) // PRQA S 3081, 3380, 3385
					<< " coupled to \"" << first << "\" path\n") ;

			fileOnDiskToInventory = this->doPackaging( metadata, first, second, coupledPath, theConnection) ;
// path of fileOnDiskToInventory could depend on the particular implementation of doPackaging method.
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,"File path ON DISK to inventory: \"" << fileOnDiskToInventory << "\".")

		}

		{
		// just for compatibility with old code: some class could have re-defined the following method to do some job...
			string filenameToInventory = this->getMtdFileName( first, second, metadata);
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Filename that will be inventoried: \"" << filenameToInventory << "\"")
			
		}
 
	
// The name of metadata file must result from addition of proper extension to the file path on disk to inventory that is written in the .LIST file 
// (e.g. myFile.EOF -> myFile.EOF.MTD)
		string mtdFileName = getAbsoluteMtdFName(fileOnDiskToInventory) ;
// **********
// ********* WARNING: path of mtdFileName MUST BE the same of fileOnDiskToInventory	
// **********
        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 

	        ACS_LOG_DEBUG(SimpleDebugSignature << "writing metadata in: \"" << mtdFileName << "\"") ; 

            string path = "/tmp/" + File::getFileName(mtdFileName) ;
	        ACS_LOG_DEBUG(SimpleDebugSignature << "leaving a copy of metadata in: " << path ) 

            try {
                metadata.writeMetadataFile(path) ;
            }
            catch(exception &x) {
                ACS_LOG_NOTIFY_EX (x)
            }

        ACS_CLASS_END_DEBUG

        metadata.writeMetadataFile(mtdFileName);
        return fileOnDiskToInventory ;
}

std::vector<std::string> PreInventoryBase::doWork(dbConnectionPool &pool) {

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050

    if ( _scanMode ) { 

        this->scanWd(theConnection); 
    }

    static const string anomalyKey ("PreInventory.ManageInventoryAnomalies");
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    if (rs.hasKey (anomalyKey))
    {
        rs.getValue (anomalyKey, _manageInventoryAnomalies); 
    }

    //Product list to return
    vector<string> filesPreInventoried;

    //Preprocessing phase
    vector< Pair_FileName_FileType > filesToInventory;
    filesToInventory = this->preProcess(_inputFiles,theConnection);
    //For each detected product to preInventory
    for ( vector< Pair_FileName_FileType >::iterator fileToInvP=filesToInventory.begin(); fileToInvP!=filesToInventory.end(); fileToInvP++ ) { // PRQA S 4238
        //Get the metadata

        exFMessage msg ;
        msg << "Handling: "<<fileToInvP->first<<" FileType: "<<fileToInvP->second ;
        excerr << msg ; 
        try
        {
            string fileOnDiskToInventory = processFile(fileToInvP->first, fileToInvP->second, theConnection) ;
     		filesPreInventoried.push_back( File::getFileName(fileOnDiskToInventory) );
        }
        catch (exMetadataComputationException&)
        {
            try
            {
                if (_manageInventoryAnomalies)
                {
                    dbSmartPersistent anomaly("t_inventoryanomalies", theConnection);
		    // S1PDGS-31538 strip any path from the filename to be inserted in the anomaly list
                    anomaly.set( "filename", File::getFileName(fileToInvP->first), theConnection );
                    anomaly.set( "fileversion", string("N/A"), theConnection ); // PRQA S 3081 2
                    anomaly.set( "message", string("Unable to extract metadata from file"), theConnection );
                    anomaly.set( "anomaly_type_id", AnomalyTypes::instance(theConnection.getKey())->id("FAULT"), theConnection );
                    anomaly.save(theConnection);
                }
            }
            catch (exception&)
            {
                // No worries. Just ignore it.
            }

            // finally re-throw the exMetadataComputationException
            throw;
        }

    }

    return filesPreInventoried;

}


string PreInventoryBase::getMtdFileName( const std::string& pathFileName, const std::string& fileType, Metadata& metadata)
{
        return  metadata.getGlobalFileName();
}

/** Return the name of metadata file with extension */
string PreInventoryBase::getAbsoluteMtdFName( const std::string& path ) const // PRQA S 4214
{
    string mtdExtension ;
    ConfigurationSingleton::instance()->get().getValue( "Inventory.MetadataFileExtension", mtdExtension);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "mtdExtension: \"" << mtdExtension << "\" -> \"" << File::addExt(path, mtdExtension) << "\"")

	return File::addExt(path, mtdExtension) ;
}



void PreInventoryBase::findFiles(const string& fileType, const RegExp& productRegExp,   // PRQA S 4020
                                 const string& directory, 
                                 const unsigned int& currentDeepLevel,   const unsigned int& maxDeepLevel ) {

    if ( currentDeepLevel > maxDeepLevel) {
        return;
    } else {
        //Scan working dir for files/dir matching the regualar expression
        Dir currentDir( directory );
        for ( unsigned short i(0); i<currentDir.size(); i++ ) {
            string filename( currentDir[i] );

            if ( productRegExp.match( filename ) ) { //Product match regular expression

                bool found ( find( _excludeList.begin(), _excludeList.end(), filename ) != _excludeList.end() );
                if ( !found ) { //Product is not in exclude list
                    string filenamepath(directory+"/"+filename);
                    _inputFiles.push_back( Pair_FileName_FileType(filenamepath, fileType) );

                    ACS_LOG_INFO ("PreInventoryBase::scanWd : "<<filenamepath<<" ["<<fileType<<","<<productRegExp.getReg()<<"]")
                } else {
                    ACS_LOG_INFO ("PreInventoryBase::scanWd : "<<filename<<" ignored since in exclude list.")
                }
            }
            
            if ( File::isDir( directory+"/"+filename ) ) {
                this->findFiles( fileType, productRegExp, directory+"/"+filename, currentDeepLevel+1, maxDeepLevel ); // PRQA S 3084
            }
        }
    }
}


void PreInventoryBase::scanWd(dbConnection &conn) {

    //For each selected FileType
    for (vector<string>::iterator fileTypeP = _selectedFiletypes.begin(); fileTypeP != _selectedFiletypes.end(); fileTypeP++) { // PRQA S 4238

        //Retrieve the regular expression related to the fileType
        string fileTypeRegEx( this->getFileTypeRegEx(*fileTypeP, conn));

        RegExp productRegExp; productRegExp.setReg( fileTypeRegEx );

        ACS_LOG_INFO ("PreInventoryBase::scanWd : Files matching "<<"["<<*fileTypeP<<","<<fileTypeRegEx<<"]:")

        //Retrieve Products in current path and subdirs
        unsigned int maxDeepLevel(0);
        ConfigurationSingleton::instance()->get().getValue("PreInventory.ScanMode.maxDepth", maxDeepLevel);
        this->findFiles( *fileTypeP, productRegExp, _path, 0, maxDeepLevel);
    }

}

dbQueryParameters PreInventoryBase::getProductFilenameInInventoryQueryParameters( dbConnection &conn, const Metadata& metadata ) {

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB)
    const string &dbKey = conn.getKey() ;
    dbQueryParameters parameters(dbKey) ; 
    parameters.addParameter(dbParam(dbKey, "t_inventory","filename", dbParam::EQUAL_TO, db::toSQL(dbKey, metadata.getGlobalFileName() ))) ; // PRQA S 3050, 3081

    return parameters;

}

string PreInventoryBase::getFileTypeRegEx(const string& fileType, dbConnection &conn) {

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "fileType=" << fileType)


    string fileTypeRegex("");

    //Execute Query
    //@@@ Raffinare la query con satellite/mission?
	const string &dbKey = conn.getKey() ;
    dbFileTypeQuery* dbFileTypeQry = dynamic_cast<dbFileTypeQuery*>( dbQueryFactoryChain::instance()->newObject("dbFileTypeQuery", dbKey) ); // PRQA S 3081, 4412
    dbQueryParameters parameters(dbKey) ; 
    parameters.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::EQUAL_TO, dbConv( dbKey, fileType ))) ; // PRQA S 3050, 3081
    dbFileTypeQry->addParameter( parameters );
    unsigned long countRow = dbFileTypeQry->doQuery(conn);
    if ( countRow>0 ) {
        fileTypeRegex = ((*dbFileTypeQry)[0])->getNameRegularExpression();
    }

	if( countRow>1 ) {
		ostringstream exMsg;
		exMsg << "PreInventoryBase::getFileTypeRegEx : Error: retrieved more than one (" << countRow << ") \"" << fileType << "\" from Db.";
		ACS_THROW( exPreInventoryBaseCriticalException(exMsg.str()) );  // PRQA S 3081
	}
    ACS_COND_THROW( fileTypeRegex=="", exPreInventoryBaseCriticalException("PreInventoryBase::getFileTypeRegEx : Error retrieving "+fileType+" regexp from Db") ); // PRQA S 3081

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "fileTypeRegex="<<fileTypeRegex)

    return fileTypeRegex;
}

bool PreInventoryBase::overrideVersionHandling(bool& doVersioning ) {
	return false;
}

bool PreInventoryBase::doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &conn) {

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB)

    bool isTheVersionChanged(false);

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", conn.getKey()) ); // PRQA S 3081, 4412

    //Get specific dbQueryParametes
    dbQueryParameters parameters = this->getProductFilenameInInventoryQueryParameters(conn, metadata);

    dbInvObjectQuery->addParameter( parameters );

    unsigned long countRow = dbInvObjectQuery->doQuery(conn);

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "num = dbInventoryObjectQuery->doQuery() = " << countRow)

    if ( countRow > 0) {

        int maxVers= std::numeric_limits<int>::min();
        for(unsigned int i=0; i<countRow; i++) {

            string version = ((*dbInvObjectQuery)[i])->getFileVersion();
            int vers = atoi(version.c_str());

            ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "dbInvObjectQuery->getFileVersion-> version = " << vers)

            maxVers = max(vers,maxVers ) ; 

        }	

        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "maxVers = " << maxVers)

        int newVers = maxVers+1;
				
		newVersion = getFormattedVersion( newVers);

        dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);

        isTheVersionChanged=true;

    } else {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "No new Version")
    }

    return isTheVersionChanged;
}


string PreInventoryBase::getCoupledPath( const string & firstPath, const string & fileType, dbConnection &theConnection) const // PRQA S 4214
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_LOW_VERB)

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Filename=\"" << firstPath << "\", filetype=\"" << fileType << "\".")

					
	dbFileTypeRecognition::ExtensionInfo extInfo;
	if( !dbFileTypeRecognition::checkFNameExtension(fileType, File::getFileName(firstPath), extInfo, theConnection) ) {
		ostringstream exMsg;
		exMsg << "Extension of file \"" << firstPath << "\" does not match the one configured on db for file type \"" << fileType << "\"";
		ACS_THROW( exPreInventoryBaseCriticalException(exMsg.str()) ); // PRQA S 3081

	}

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Filename=\"" << firstPath << "\", filetype=\"" << fileType << "\".")

        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 

	        ACS_LOG_DEBUG(SimpleDebugSignature << "Extension occurring on db for given file type matches the input file one; extension id: "
			<< extInfo.extId << ".") ; 

    		if( extInfo.expectedExtNr > 1 ) {
			    ACS_LOG_DEBUG(SimpleDebugSignature << " Number of coupled files " << extInfo.expectedExtNr - 1  << ", coupled file extension \"" << extInfo.otherExt << "\".")
            }    

        ACS_CLASS_END_DEBUG
	
	string coupledPath ;
	if( extInfo.expectedExtNr == 2 ) { // PRQA S 4400
		coupledPath = File::addExt( File::removeExt(firstPath), extInfo.otherExt ) ;

        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Coupled file path is \"" << coupledPath << "\".")
	}

	return coupledPath;
	
}


string PreInventoryBase::fillMetadataFilename( const std::string& filePath, const std::string& fileType, Metadata& metadata, dbConnection &theConnection, bool *mngSngExtPtr, string *sngExtPtr)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_LOW_VERB)

	string filename = File::getFileName(filePath);

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Filename=\"" << filename << "\", filetype=\"" << fileType << "\".")
	
	dbFileTypeRecognition::ExtensionInfo extInfo;
	if( !dbFileTypeRecognition::checkFNameExtension(fileType, filename, extInfo, theConnection) ) {
		ostringstream exMsg;
		exMsg << "Extension of file \"" << filename << "\" does not match the one configured on db for file type \"" << fileType << "\"";
		ACS_THROW( exPreInventoryBaseCriticalException(exMsg.str()) ); // PRQA S 3081

	}

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Extension occurring on db for given file type matches the input file one; extension id: "
			<< extInfo.extId << "; manage sng ext: " << boolalpha << extInfo.mngSngExt 
			<< "; number of expected extension: " << extInfo.expectedExtNr)
		
	string sngExt; 
	if(	extInfo.mngSngExt ) {
		filename = File::removeExt(filename);
    }
	else if( extInfo.expectedExtNr == 2 ) { // PRQA S 4400
		dbFileTypeRecognition filetypeRecogniser;	
		sngExt = (filetypeRecogniser.getTraits(fileType, theConnection)).sngext;

        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "sngext configured for file type \"" << fileType << "\" is \"" << sngExt << "\".")

		filename = File::addExt( File::removeExt(filename), sngExt);
	}
	else if( extInfo.expectedExtNr == 1 ) {
		sngExt = File::getExt(filename);
    }
	else {
		ostringstream exMsg;
		exMsg << "Expected extensions number for file type \"" << fileType << "\" is " << extInfo.expectedExtNr << " (it should be <=2)";
		ACS_THROW( exPreInventoryBaseCriticalException(exMsg.str()) ); // PRQA S 3081
	}
	
	// filling the output if requested	
	if( 0 != mngSngExtPtr) { *mngSngExtPtr = extInfo.mngSngExt; }
	if( 0 != sngExtPtr) { *sngExtPtr = sngExt; }

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "File will be inventoried with the name \"" << filename << "\".")

	metadata.setMetadataValue( "File_Name", filename);
	
	return filename;

}

string PreInventoryBase::getFormattedVersion( int version) // PRQA S 4211
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_LOW_VERB)

    int confSetw(0); char confSetFill('0'); {
        rsResourceSet& conf = ConfigurationSingleton::instance()->get();
        rsPushNSpace confNs(conf, _context.mainConfNamespace ); 
        string confSetFillStr("");
        conf.getValue("Versioning.Format.setw.value",    confSetw);
        conf.getValue("Versioning.Format.setfill.value", confSetFillStr);
        ACS_COND_THROW( confSetFillStr=="", exPreInventoryBaseCriticalException("PreInventoryBase::doVersioning : Error: Versioning.Format.setfill is empty in Configuration Space") ); // PRQA S 3081
        confSetFill=confSetFillStr.at(0);
    }
    ostringstream os; 
    os << setw(confSetw) << setfill(confSetFill) << version;
    string fmttedVersion = os.str();

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Formatted version \"" << fmttedVersion << "\"")
			
	return fmttedVersion;
}	

string PreInventoryBase::convertOriginalFileClass( const string & pluginId, const string & filetype, const string & originalFileClass ) const // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_LOW_VERB)
			
	if( originalFileClass.empty() || filetype.empty() ) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Input file class / file type is empty: no conversion is possible");
		return originalFileClass;
	}
    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string baseNode = "PreInventory." + pluginId + ".FileClassLUT";
	if ( !rs.hasKey (baseNode) ) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << baseNode << "\" node not occurring in configuration");
		return originalFileClass;
    }
	vector<FileClassLUTElement> fileClassLUTElements;
	
	size_t LUTElements = rs.getArray(baseNode+".LUTElement", fileClassLUTElements); // PRQA S 4412
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "<" << LUTElements << "> LUTElement elements occurring in \"" << baseNode << "\" node");

	bool converted = false;
    size_t fclute_size = fileClassLUTElements.size();
	string convertedFileClass = originalFileClass;
	for( size_t i = 0; i < fclute_size; ++i ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "LUTElement #" << i << "\n" << fileClassLUTElements[i]);
		if( find( fileClassLUTElements[i].fileTypes.begin(), fileClassLUTElements[i].fileTypes.end(), filetype ) != fileClassLUTElements[i].fileTypes.end() ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "\"" << filetype << "\" filetype found in LUTElement #" << i);
			size_t i_size = fileClassLUTElements[i].conversions.size() ;
            for( size_t j = 0; (j < i_size) && !converted; ++j ) {
				if( originalFileClass == fileClassLUTElements[i].conversions[j].originalFileClass ) {
					convertedFileClass = fileClassLUTElements[i].conversions[j].dbFileClass;
					converted = true;
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << originalFileClass << "\" original file class will be converted into \"" << convertedFileClass << "\"");
				}
			}
			break;
		}
    }    

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		if( !converted ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "\"" << originalFileClass << "\" file class will not be converted.");
    	}    
	ACS_CLASS_END_DEBUG
		
	return convertedFileClass;
	
}


string PreInventoryBase::convertUncorrectDate( const string & pluginId, const string & originalDate, bool *convertedPtr ) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_LOW_VERB)
			
	string convertedDate = originalDate;
	if( convertedDate.empty() ) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Input time is empty: no conversion is possible");
		return convertedDate;
	}
	
	if( _timeLUTElements.empty() ) {
    	//read the configuration file from database
    	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
		string baseNode = "PreInventory." + pluginId + ".TimeLUT";
		if ( !rs.hasKey (baseNode) ) {
        	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << baseNode << "\" node not occurring in configuration");
			return convertedDate;
    	}
		size_t LUTElements = rs.getArray(baseNode+".LUTElement", _timeLUTElements); // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "<" << LUTElements << "> LUTElement elements just read from \"" << baseNode << "\" node");
	}

	bool converted = false;
    size_t tle_size = _timeLUTElements.size() ;
	for( size_t i = 0; i < tle_size; ++i ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "LUTElement #" << i << "\n" << _timeLUTElements[i]);
		if( find( _timeLUTElements[i].originalDates.begin(), _timeLUTElements[i].originalDates.end(), originalDate ) != _timeLUTElements[i].originalDates.end() ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << originalDate << "\" date found in LUTElement #" << i << " will be converted into \"" << _timeLUTElements[i].convertedDate << "\"");
			convertedDate = _timeLUTElements[i].convertedDate;
			converted = true;
			break;
		}
    }    

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		if( !converted ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "\"" << originalDate << "\" date will not be converted.");
    	}    
	ACS_CLASS_END_DEBUG
			
	if( 0 != convertedPtr) { 
		*convertedPtr = converted; 
	}
		
	return convertedDate;
	
}

string PreInventoryBase::removeSpuriousCharsFromStringDate( const string& originalDate ) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_LOW_VERB)
			
	if( not originalDate.empty() ) {
		size_t stringLength = originalDate.size();
		if( 'Z' == toupper(originalDate[stringLength-1]) ) { // PRQA S 3084
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Found 'Z' char at the end of time value");
			return originalDate.substr(0, stringLength-1); // PRQA S 3084
		}
	}
	return originalDate;
}

string PreInventoryBase::getSatMissionAssociatorFactoryKey( const string& satCode, const string & parentTag)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	string key;
    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string localRootTag = parentTag + ".SatMissionsAssociatorKeys.Pair";
    size_t elements = rs.getArraySize( localRootTag ); // PRQA S 3000, 3010, 4412
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << localRootTag << ", elements " << elements);

	bool found = false;
	for( size_t i=0; (i<elements) && !found; i++)
	{
		string tag = localRootTag;
		if( elements > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=\"" << tag << "\", satellite code to find=\"" << satCode << "\"");

		string satCodeValue;
		rs.getValue( tag + ".satelliteCode.value", satCodeValue);
		if( satCodeValue == satCode) {
			rs.getValue( tag + ".key.value", key);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "satellite code found=(\"" << satCode << "\"-->\"" << key << "\")" );
			found = true;
		}
	}
	if( !found)
	{	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "satellite code \"" << satCode << "\" not found in config file!!! Returning \"" << key << "\"");
	}
		
	return key;
}

bool PreInventoryBase::doVersioningByFileInventoryVersion(const Metadata & metadata, const string & key, string& newVersion, dbConnection & conn, std::string * retFilename)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current filename=\"" << metadata.getGlobalFileName() << "\", version=\"" << metadata.getMetadataValue("File_Version") << '\"');
		
	FileInventoryVersion invVers(key);
	pair<string, string> nameVersPair = invVers.versionControlByFileName(metadata.getGlobalFileName(), conn, "", true) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returned \"" << nameVersPair.first << "\" filename and \"" << nameVersPair.second << "\" version");
	string retVersion ;
	{
		int vers = 0;
		istringstream iss(nameVersPair.second);
		iss >> vers ;
		retVersion = getFormattedVersion( vers);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returned version right formatted: \"" << retVersion << '\"');

    bool versionChanged = not StringUtils::equals(metadata.getMetadataValue("File_Version"), retVersion);
	if( versionChanged ) {
		newVersion = retVersion;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New version: \"" << newVersion << '\"');
		if( retFilename != 0 ) {
			*retFilename = nameVersPair.first;
		}
	}
	return versionChanged;

}


string PreInventoryBase::Context::toString(PreInventoryBase::Context::Which w) // PRQA S 4020
{
    switch(w) {
    case DIS:
        { return "DIS" ; }
    case TL: 
        { return "TL" ; }
    case PROD:
        { return "PROD" ; }
    case DS:
        { return "DS" ; }
    default:
        { 
            ostringstream os ;
            os << "<unknown PreInventoryBase::Context::Which: " << int(w) << ">" ; // PRQA S 3081
            return os.str() ;
        }
    }
}

ostream &operator << (std::ostream &os,PreInventoryBase::Context::Which w)
{
    os << PreInventoryBase::Context::toString(w) ;
    return os ; // PRQA S 4028
}

acs::exostream &operator << (acs::exostream &os ,PreInventoryBase::Context::Which w ) // PRQA S 4222
{
    os << PreInventoryBase::Context::toString(w) ;
    return os ; // PRQA S 4028
}


//
//
//-----------------------FileClassLUTElement::Conversion
//
//

PreInventoryBase::FileClassLUTElement::Conversion::Conversion() :
    originalFileClass(),
    dbFileClass()
{
}


PreInventoryBase::FileClassLUTElement::Conversion::Conversion(PreInventoryBase::FileClassLUTElement::Conversion const &o):
	originalFileClass(o.originalFileClass),
	dbFileClass(o.dbFileClass)
{
}
								
PreInventoryBase::FileClassLUTElement::Conversion & PreInventoryBase::FileClassLUTElement::Conversion::operator = (PreInventoryBase::FileClassLUTElement::Conversion const &o)
{
	if( this != &o ) {
		originalFileClass=o.originalFileClass;
		dbFileClass=o.dbFileClass;
	}
	return (*this);
}								

//
//
//-----------------------FileClassLUTElement 
//
//

PreInventoryBase::FileClassLUTElement::FileClassLUTElement() :
    fileTypes(),
    conversions()
{
}


PreInventoryBase::FileClassLUTElement::FileClassLUTElement(PreInventoryBase::FileClassLUTElement const &o):
	fileTypes(o.fileTypes),
	conversions(o.conversions)
{
}
								
PreInventoryBase::FileClassLUTElement & PreInventoryBase::FileClassLUTElement::operator = (PreInventoryBase::FileClassLUTElement const &o)
{
	if( this != &o ) {
		fileTypes=o.fileTypes;
		conversions=o.conversions;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const PreInventoryBase::FileClassLUTElement & v)
{
	s	<< "****************** FileClassLUTElement ****************\n";
	for( size_t i = 0; i < v.fileTypes.size(); i++ ) {
		s << "\tfileType #" << i << " =\"" << v.fileTypes[i] << "\"\n";
	}
	for( size_t i = 0; i < v.conversions.size(); i++ ) {
		s << "\tConversion #" << i << " originalFileClass=\"" << v.conversions[i].originalFileClass << "\" -> dbFileClass=\"" << v.conversions[i].dbFileClass << "\"\n";
	}

	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const PreInventoryBase::FileClassLUTElement & v) // PRQA S 4222
{
	s	<< "****************** FileClassLUTElement ****************\n";
	for( size_t i = 0; i < v.fileTypes.size(); i++ ) {
		s << "\tfileType #" << i << ": \"" << v.fileTypes[i] << "\"\n";
	}
	for( size_t i = 0; i < v.conversions.size(); i++ ) {
		s << "\tConversion #" << i << ": originalFileClass=\"" << v.conversions[i].originalFileClass << "\" -> dbFileClass=\"" << v.conversions[i].dbFileClass << "\"\n";
	}
	
	return s ; // PRQA S 4028

}

template<>
void rsResourceSet::getValue(const std::string &res, PreInventoryBase::FileClassLUTElement::Conversion &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".OriginalFileClass")->get(v.originalFileClass) ;
	getValue(res+".DBFileClass")->get(v.dbFileClass) ;
} 

template<>
void rsResourceSet::getValue(const std::string &res, PreInventoryBase::FileClassLUTElement &v) const 
{
	
    Lock lock(_mutex) ;
	getArray(res+".FileType", v.fileTypes);
	getArray(res+".Conversion", v.conversions);
	
} 


//
//
//-----------------------TimeLUTElement 
//
//

PreInventoryBase::TimeLUTElement::TimeLUTElement() :
    originalDates(),
    convertedDate()
{
}


PreInventoryBase::TimeLUTElement::TimeLUTElement(PreInventoryBase::TimeLUTElement const &o):
	originalDates(o.originalDates),
	convertedDate(o.convertedDate)
{
}
								
PreInventoryBase::TimeLUTElement & PreInventoryBase::TimeLUTElement::operator = (PreInventoryBase::TimeLUTElement const &o)
{
	if( this != &o ) {
		originalDates=o.originalDates;
		convertedDate=o.convertedDate;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const PreInventoryBase::TimeLUTElement & v)
{
	s	<< "****************** TimeLUTElement ****************\n";
	for( size_t i = 0; i < v.originalDates.size(); i++ ) {
		s << "\tOriginal date #" << i << " =\"" << v.originalDates[i] << "\"\n";
	}
	s << "\tConverted date =\"" << v.convertedDate << "\"\n";

	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const PreInventoryBase::TimeLUTElement & v) // PRQA S 4222
{
	s	<< "****************** TimeLUTElement ****************\n";
	for( size_t i = 0; i < v.originalDates.size(); i++ ) {
		s << "\tOriginal date #" << i << " =\"" << v.originalDates[i] << "\"\n";
	}
	s << "\tConverted date =\"" << v.convertedDate << "\"\n";

	return s ; // PRQA S 4028

}

template<>
void rsResourceSet::getValue(const std::string &res, PreInventoryBase::TimeLUTElement &v) const 
{
    Lock lock(_mutex) ;
	getArray(res+".OriginalDate", v.originalDates);
	getValue(res+".ConvertedDate")->get(v.convertedDate) ;
} 


_ACS_END_NAMESPACE
