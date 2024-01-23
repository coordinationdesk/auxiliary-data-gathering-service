// PRQA S 1050 EOF
/*
    Copyright 1995-2021, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

    $Prod: A.C.S. libPreInventory $

    $Id$

    $Author$

    $Log$
    Revision 1.21  2016/11/21 16:12:17  marfav
    S3MPM-847
    Added functionality to show plugins capabilities
    Subclass specific plugin loading moved in a dedicated virtual operation

    Revision 1.20  2016/05/10 20:54:18  danalt
    use of ACS_THROW and ACS_COND_THROW

    Revision 1.19  2016/04/11 13:35:38  marpas
    adopting new db interfaces

    Revision 1.18  2016/03/11 09:03:25  tergem
    parameter file read from context

    Revision 1.17  2016/02/18 16:35:44  nicvac
    PDSEV-27. Multiple items handling.

    Revision 1.16  2014/03/19 19:20:54  marpas
    coding best practices applied
    warning fixed (qa & g++)

    Revision 1.15  2013/02/21 18:21:21  marpas
    interface redesign to allow dbConnectionPool and explicit dbConnections
    debug improved
    coding best practices applied
    qa rules
    test fixing

    Revision 1.14  2012/12/17 12:36:00  marpas
    compilation warning removed
    test compilation partially fixed

    Revision 1.13  2012/03/20 10:29:47  giucas
    Modified loadJoPlugIns() to load from a list of plugin directories

    Revision 1.12  2009/09/28 17:01:47  marpas
    dbParam interface obsolecence

    Revision 1.11  2009/05/11 10:41:03  nicvac
    Debug msg added (order id from JobOrder).

    Revision 1.10  2009/04/08 15:41:14  nicvac
    Defauld JobOrder: if you want it, put the default plugin.

    Revision 1.9  2009/03/26 10:49:22  nicvac
    List file renamed to be unique.

    Revision 1.8  2009/02/02 16:45:05  nicvac
    getOrderId handling implemented.

    Revision 1.7  2008/12/12 14:33:56  nicvac
    Order Id Fixed.

    Revision 1.6  2008/11/14 14:15:33  marfav
    ConfigurablePluginLoader interface changed

    Revision 1.5  2008/11/07 16:53:01  nicvac
    Main namespace in context.

    Revision 1.4  2008/11/06 15:22:43  nicvac
    Context extended.

    Revision 1.3  2008/10/23 09:10:16  nicvac
    Implementation.

    Revision 1.2  2008/10/13 14:48:34  nicvac
    TlPreInventory implementation
    
    Revision 1.1  2008/10/13 14:42:25  nicvac
    PfPreInventory renamed in TlPreInventory
    
        
*/ 

#include <TlPreInventory.h>

#include <JobOrderFactoryChain.h>
#include <StringKey.h>
#include <StringUtils.h>

#include <dbConnection.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <dbGenQueryScopeGuard.h>
#include <dbFileTypeQuery.h>
#include <dbFileType.h>

#include <DirPluginSet.h>

#include <Filterables.h>

#include <boost/format.hpp> //  PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TlPreInventory)



TlPreInventory::TlPreInventory():
    PreInventory(),
    _joPluginLoader()
{
    static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: "<< Id) ;
}

TlPreInventory::~TlPreInventory() throw() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}

/*
 * STL Helper class
 * Merges all the Input Descritor using a strings array
 */
class accumulate_inputs {
public:
    explicit accumulate_inputs (vector<string>& v) : _theList(v) {} // PRQA S 2528 2
    accumulate_inputs (const accumulate_inputs &r) : _theList(r._theList) {}
    ~accumulate_inputs() throw() {}

    void operator() (const InputDescriptor& i) {
        for_each (i.begin<InputAlternative>(), i.end<InputAlternative>(), accumulate_inputs(_theList)); // PRQA S 3081
    }

    void operator() (const InputAlternative& a) {
        vector <string> thisList;
        a.getPathNames (thisList);
        for_each (thisList.begin(), thisList.end(), accumulate_inputs (_theList)); // PRQA S 3081
    }

    void operator() (const string& path) {
        string theFileName = File::getFileName (path);
        _theList.push_back (theFileName);
    }
private: 
    accumulate_inputs() ;
    accumulate_inputs & operator= (const accumulate_inputs &) ;


    vector<string>& _theList;

};

void TlPreInventory::loadSpecificPlugins() {
    rsResourceSet& conf ( ConfigurationSingleton::instance() -> get() );
    try {
		vector<string> pluginsPaths;
		string pluginRegExp;    
		{
			rsPushNSpace nspace_tld (conf, "TLPreInventory");
			conf.getArray("Plugins.List_of_Plugins_Path.Plugins_Path", pluginsPaths);
			conf.getValue("Plugins.SelectRE",   pluginRegExp);
		}
		size_t ppsize = pluginsPaths.size() ;
		for(size_t i=0; i<ppsize; ++i) {
			ACS_LOG_INFO("Loading plugins from "+pluginsPaths[i]) ;
        }

        DirPluginSet myPluginSet (pluginsPaths, pluginRegExp);
        myPluginSet.load (_joPluginLoader);
        if (!_joPluginLoader.load()) {
            exFWarning war; 
            war<<"Plugin loader failed. Some plugins have not been installed.";
            excerr << war ;
        }

    } catch (exception& e) {
        ACS_THROW( exPreInventoryCriticalException(e, "Error while loading plugins") ); // PRQA S 3081
    }
}

void TlPreInventory::filterFileTypes(dbConnection &conn) {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


    //Filtered fileTypes
    vector< string > filteredList;
    const string &dbKey = conn.getKey() ;
    //Check each filetype
    for ( vector<string>::iterator itFileType=_fileTypes.begin(); itFileType!=_fileTypes.end(); ++itFileType ) { // PRQA S 4238
        //Execute Query
        dbFileTypeQuery* dbFileTypeQry=dynamic_cast<dbFileTypeQuery*>( dbQueryFactoryChain::instance()->newObject("dbFileTypeQuery", dbKey) ); // PRQA S 3081, 4412
        dbQueryParameters parameters(dbKey);
        parameters.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::EQUAL_TO, db::toSQL(dbKey, *itFileType ))) ;
        dbFileTypeQry->addParameter( parameters );
        unsigned long countRow = dbFileTypeQry->doQuery(conn);

        //FileType not exist in DB ==> Remove it
        if ( countRow==0 ) { 
            ACS_LOG_WARNING("FileType "<<*itFileType<<" not registered in DB => ignored.") ;
        } else {
            filteredList.push_back( *itFileType );
        }
    }
    _fileTypes = filteredList;

}

void TlPreInventory::initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &conn) {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

    //TL PreInventory works in scan mode
    _scanMode=true;

    //TL PreInventory works in Working Dir
    _dirNameToScan="./";

    _satellite = argvParameters["satellite"].as<string>();
    _mission = argvParameters["mission"].as<string>();

    _procName = argvParameters["procname"].as<string>();
    _procVersion = argvParameters["procversion"].as<string>();

    //Fill _fileTypes and _excludeList from JobOrder
    ACS_SMARTPTR<JobOrder> jo;
    {//Get JobOrder from JobOrder Factory

        StringKey key = StringKeyUtils::makeKey( _satellite, _mission, _procName, _procVersion );
        try {
            jo.reset ((JobOrderFactoryChain::instance())->newObject(key) );
        } catch (exception& ex) {
            ACS_LOG_NOTIFY_EX(ex) ;
            ACS_LOG_WARNING("Unable to get a valid JobOrder from the factory. Skipping the processing.") ;

			throw;
        }

        //Read JobOrder
        XMLIstream jofileXml( argvParameters["jobOrder"].as<string>() ); 
        jofileXml >> *jo ; 

        vector<OutputDescriptor> outputs;
        vector<InputDescriptor> inputDescr;
        //Retrieve output descriptors from the tasks
        for ( vector<Task>::iterator it=jo->begin(); it!=jo->end(); it++ ) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getting output descriptors from task ["<<it->getName()<<"]") ;

            it->getOutputList( outputs );
            it->getInputList( inputDescr );
        }

        vector<string> outputFiletypes;
        // Transform the OutputDescrptor vector in vector of strings.
        // And do not insert duplicated filetypes
        for ( vector<OutputDescriptor>::iterator it=outputs.begin(); it!=outputs.end(); ++it ) { // PRQA S 4238
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"adding filetype ["<<it->getFileType()<<"]") ;

            if ( find( outputFiletypes.begin(), outputFiletypes.end(), it->getFileType() ) != outputFiletypes.end() ) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"filetype ["<<it->getFileType()<<"] exists - not inserted" ) ;
            } else {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"filetype ["<<it->getFileType()<<"] inserted") ;
                outputFiletypes.push_back (it->getFileType());
            }
        }
        _fileTypes = outputFiletypes;

        vector<string> inputsName;
        //Extract the input list from the descriptors
        for_each( inputDescr.begin(), inputDescr.end(), accumulate_inputs(inputsName) ); // PRQA S 3081

        _excludeList = inputsName;

    }

    //Refine the filetype list to process
    this->filterFileTypes(conn);

    rsResourceSet& conf = ConfigurationSingleton::instance()->get();
    {//Fill Context

        _context.which = PreInventoryBase::Context::TL;
        _context.metadataInfo.satelliteName = _satellite;
        _context.metadataInfo.mission = _mission;
        _context.metadataInfo.processorName = _procName;
	_context.parameterFile = jo->getParametersFileName();
        //Order Id handling
        unsigned int orderId(0); {
            orderId = jo->getOrderId();
            if ( 0 == orderId ) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Getting orderId from filename "<<argvParameters["jobOrder"].as<string>()) ;

                orderId = JobOrder::getOrderIdFromFilename( argvParameters["jobOrder"].as<string>() );
                if ( 0 == orderId ) {
                    ACS_LOG_WARNING("Cannot Retrieve Order Id, Set to "<<orderId) ;
                }
            }
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"orderId: "<<orderId) ;
        }
        ostringstream orderIdStr; orderIdStr<<orderId;
        _context.metadataInfo.orderId = orderIdStr.str();
        _context.metadataInfo.processorVersion = _procVersion;
        _context.metadataInfo.fileClassInfo.processingType = jo->getOrderType();
        _context.metadataInfo.fileClassInfo.test = jo->isTest();
        _context.metadataInfo.fileClassInfo.troubleShooting = jo->isTroubleshooting();

        conf.getValue("Station.Name",   _context.metadataInfo.stationConf.name );
        conf.getValue("Station.Id",     _context.metadataInfo.stationConf.stationId );

        _context.mainConfNamespace = "TLPreInventory";

    }

    {
        conf.getValue("TLPreInventory.PreInventoryListFileName", _outListFilename);
		_outListFilename="OrderId_"+_context.metadataInfo.orderId+"_"+_outListFilename;
    }


    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        string elems ;   
        for ( vector<string>::iterator elem=_fileTypes.begin(); elem != _fileTypes.end(); ++elem  ) { // PRQA S 4238 
            elems+=*elem+" ";
        }
        string exclude ; 
        for ( vector<string>::iterator elem=_excludeList.begin(); elem != _excludeList.end(); ++elem  ) { // PRQA S 4238 
            exclude+=*elem+" ";
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Procname: "<<_procName<<". ProcVersion: "<<_procVersion<<". FileTypes to check: "<<elems<<". Exclude list: "<<exclude) ;
    ACS_CLASS_END_DEBUG
}

_ACS_END_NAMESPACE
