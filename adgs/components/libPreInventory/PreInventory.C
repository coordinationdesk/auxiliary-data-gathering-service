// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 2.1  2017/12/07 15:59:04  lucio.pulvirenti
	ECICMF-186: If no product to preinventory check configuration before throwing exc.
	
	Revision 2.0  2016/11/21 16:13:43  marfav
	Major release
	
	Revision 1.19  2016/11/21 16:12:17  marfav
	S3MPM-847
	Added functionality to show plugins capabilities
	Subclass specific plugin loading moved in a dedicated virtual operation
	
	Revision 1.18  2016/02/18 16:35:44  nicvac
	PDSEV-27. Multiple items handling.
	
	Revision 1.17  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.16  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.15  2012/12/17 12:36:00  marpas
	compilation warning removed
	test compilation partially fixed
	
	Revision 1.14  2012/12/10 10:16:48  chicas
	 '
	
	Revision 1.13  2012/04/02 08:26:16  giucas
	Load plugins modified to load from a list of paths
	
	Revision 1.12  2009/03/27 10:48:41  nicvac
	log added.
	
	Revision 1.11  2009/01/29 16:39:31  nicvac
	Empty LIST file (no files to preinventory) causes Error.
	
	Revision 1.10  2008/12/19 17:07:28  nicvac
	Inventory working dir moving.
	
	Revision 1.9  2008/11/28 15:15:06  nicvac
	Working dir in context.
	
	Revision 1.8  2008/11/28 11:49:25  nicvac
	log added.
	
	Revision 1.7  2008/11/28 11:43:31  nicvac
	log added.
	
	Revision 1.6  2008/11/14 14:15:33  marfav
	ConfigurablePluginLoader interface changed
	
	Revision 1.5  2008/10/23 09:09:03  nicvac
	program_options used instead of argv.
	
	Revision 1.4  2008/10/13 14:44:34  nicvac
	context added.
	
	Revision 1.3  2008/10/10 16:18:29  nicvac
	Plugin loading implemented.
	
	Revision 1.2  2008/10/08 13:50:30  nicvac
	Implementation.
	
	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
	Import libPreInventory
	
        
*/ 

#include <PreInventory.h>
#include <PreInventoryV.h>


#include <StringKey.h>

#include <PreInventoryFactoryChain.h>

#include <DirPluginSet.h>

#include <dbConnectionPool.h>

#include <File.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PreInventory)

namespace {
    PreInventoryV version;
}

PreInventory::PreInventory():
    _satellite(),
    _mission(),
    _procName(),
    _procVersion(),
    _fileTypes(),
    _workingDir( File::getAbsolutePath( "./" ) ),
    _scanMode(true),
    _dirNameToScan(),
    _excludeList(),
    _itemsToTake(),
    _context(),
    _outListFilename(),
    _preInventoryList(),
    _pluginLoader (),
    _doShowCap (false)
{
    static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: "<< Id) ;
}

PreInventory::~PreInventory() throw () {
    // PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

   //Delete all the Referenced PreInventory
   for ( map<string,PreInventoryBase*>::iterator elem=_preInventoryList.begin(); elem!=_preInventoryList.end(); ++elem ) { // PRQA S 4238
       delete elem->second;
   }
    // PRQA L:L1

}

void PreInventory::loadSpecificPlugins()
{
	// No-op by default
	// It will be overriden in derived classes, if needed
}

void PreInventory::loadPlugIns(const PluginLoaderNotifier &pln) {
    rsResourceSet& conf ( ConfigurationSingleton::instance() -> get() );
    try {

		vector<string> pluginsPaths;
		string pluginRegExp;    
		{
			rsPushNSpace nspace_tld (conf, "PreInventory");
			conf.getArray("Plugins.List_of_Plugins_Path.Plugins_Path", pluginsPaths);
			conf.getValue("Plugins.SelectRE",   pluginRegExp);
		}
        size_t ppsize = pluginsPaths.size() ;
		for(size_t i=0; i<ppsize; ++i) {
			ACS_LOG_PRIVINFO("Loading plugins from "+pluginsPaths[i])  ;
        }
        
        DirPluginSet myPluginSet (pluginsPaths, pluginRegExp);
        myPluginSet.load (_pluginLoader);
        if (!_pluginLoader.load(pln)) {
            ACS_LOG_WARNING("WARNING Plugin loader failed. Some plugins have not been installed.") ;
        }

		// Load also application specific plugins that may depend on the existing context
		this->loadSpecificPlugins();

    } catch (exception& e) {
	
        ACS_THROW( exPreInventoryCriticalException(e, "Error while loading plugins") ); // PRQA S 3081
    }
}


void PreInventory::showCapabilities () const
{
	ACS_LOG_INFO ("Dumping registered plugins capabilities");
	FactoryChainSingletonHelper::instance()->dumpCapabilities();
}

void PreInventory::doShowCapabilities (bool b)
{
	_doShowCap = b;
}

bool PreInventory::doShowCapabilities () const
{
	return _doShowCap;
}


int PreInventory::execute(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnectionPool &pool, const PluginLoaderNotifier &pln) 
{

   //LOAD PREINVENTORY PLUGINS
   this->loadPlugIns(pln);

   // If show capabilities is set this function returns immediately without performing any job
   if (_doShowCap)
   {
       this->showCapabilities();
       return 1;
   }


    {
        dbConnectionWrapper pw(pool) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
        //Interpret the Input and set protected attributes
        this->initFromInput(argvParameters, env, theConnection);
    }
    
   _context.argvParameters = argvParameters;

   _context.workingDir = _workingDir;
   if ( ! _dirNameToScan.empty() ) {
	   ACS_LOG_PRIVINFO("Dir path to scan: "<<_dirNameToScan) ;
   } else {
	   ACS_LOG_PRIVINFO("Items to process: "<<_itemsToTake.size());
   }
   ACS_LOG_PRIVINFO("Working dir: "<<_context.workingDir) ;


   //COLLECT PREINVENTORIES

   if ( _fileTypes.size() == 0 ) {
       ACS_LOG_WARNING("WARNING: Filetype List is empty!") ;
   }

   //For each FileType
   for ( vector<string>::iterator fileType = _fileTypes.begin(); fileType!=_fileTypes.end(); ++fileType ) { // PRQA S 4238

       StringKey key = StringKeyUtils::makeKey ( _satellite, _mission, _procName, _procVersion, *fileType);

       ACS_LOG_PRIVINFO("querying ["<<_satellite<<","<<_mission<<","<<_procName<<","<<_procVersion<<","<<*fileType<<"]") ;

       PreInventoryBase* preInventory = PreInventoryFactoryChain::instance()->newObject(key);

       //PreInventory Identification to discard if already obtained
       const string classId( preInventory->getClassId() );

       if ( _preInventoryList.find(classId) == _preInventoryList.end() ) { //Not found => New Preinventory
           ACS_LOG_PRIVINFO("got new PreInventory:"<<classId) ;

           //Init got PreInventory

           //DIR SCAN MODE: list of items to be computed by PreInventory
           if ( _scanMode ) {
        	   preInventory->setPath(_dirNameToScan,_excludeList);
           }

           //Pass processing context to the plugin
           preInventory->setContext( _context );

           //Remember about it
           _preInventoryList.insert( make_pair( classId , preInventory ) );

       } else { //Already got... thrash it and get reference to the old one
           ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"PreInventory already Stored. Ignored. ClassId="<<classId)  ;

           delete preInventory;
           preInventory = _preInventoryList.find(classId)->second;
       }

       //Tell PreInventory the fileType to handle
       preInventory->addFileType(*fileType);

       //LIST OF ITEMS MODE: list of items passed to PreInventory
       if ( ! _scanMode ) {
    	   vector<string>& filetypeItems = _itemsToTake[ *fileType ];
    	   for ( vector<string>::iterator it=filetypeItems.begin(); it!=filetypeItems.end(); ++it ) {
    		   preInventory->addItem( *it, *fileType );
    	   }
       }

       ACS_LOG_PRIVINFO("added fileType:"<<*fileType<<" to PreInventory "<<classId) ;
   }


   //RUN COLLECTED PREINVENTORIES

   //List of all preinventoried files
   vector<string> preInventoriedFiles;
   //For each PreInventory, let them to work
   for ( map<string,PreInventoryBase*>::iterator preInvElem=_preInventoryList.begin(); preInvElem!=_preInventoryList.end(); ++preInvElem ) { // PRQA S 4238
       PreInventoryBase* preInventory = preInvElem->second;

       //Get list from PreInventory called
       vector<string> somePreInventoriedFiles = preInventory->doWork(pool);
       //Update the global list
       preInventoriedFiles.insert( preInventoriedFiles.end(), somePreInventoriedFiles.begin(), somePreInventoriedFiles.end() );

   }

   
   //Write List File
   ACS_LOG_PRIVINFO("creating list file "<<_outListFilename) ;
   ofstream outListFile( _outListFilename.c_str(), ios::out );
   for ( vector<string>::iterator elem=preInventoriedFiles.begin(); elem!=preInventoriedFiles.end(); ++elem ) { // PRQA S 4238
	   ACS_LOG_PRIVINFO("file to inventory: "<<*elem) ;
       outListFile<<*elem<<endl;
   }
   //Error if PreInventory List is empty
   if ( preInventoriedFiles.size() == 0 ) {
       
		rsResourceSet& conf = ConfigurationSingleton::instance()->get();
		rsPushNSpace confNs(conf, _context.mainConfNamespace );
		bool errorForEmptyList = true;
		try {
			conf.getValue("ErrorForEmptyList", errorForEmptyList);
		}
		catch(rsResourceSet::NotFoundException &) { /* do nothing */ }
		ACS_COND_THROW( errorForEmptyList, exPreInventoryCriticalException("Nothing PreInventoried (product LIST is empty).") ); // PRQA S 3081
   }

   //Optional (by config) moving to Inventory Working Dir
   bool moveToInventoryPath(false); {
       rsResourceSet& conf = ConfigurationSingleton::instance()->get();
       rsPushNSpace confNs(conf, _context.mainConfNamespace );
       conf.getValue("MoveToInventoryPath", moveToInventoryPath);
   }

   if ( moveToInventoryPath ) {
       string inventoryWorkingDir("");
       string mtdExtension("");

       ConfigurationSingleton::instance()->get().getValue( "Inventory.WorkingDir", inventoryWorkingDir);
       ConfigurationSingleton::instance()->get().getValue( "Inventory.MetadataFileExtension", mtdExtension);

       for ( vector<string>::iterator elem=preInventoriedFiles.begin(); elem!=preInventoriedFiles.end(); ++elem ) { // PRQA S 4238
           string product( _context.workingDir+"/"+*elem );
           string productMtd( product +"."+mtdExtension );
           ACS_LOG_PRIVINFO("moving "<<product<< " product in Inventory dir: "<<inventoryWorkingDir) ;
           File::rename( product, inventoryWorkingDir+"/"+File::getFileName( product ) );
           ACS_LOG_PRIVINFO("moving "<<productMtd<<" metadata in Inventory dir: "<<inventoryWorkingDir) ;
           File::rename( productMtd, inventoryWorkingDir+"/"+File::getFileName( productMtd ) );
       }
       ACS_LOG_PRIVINFO("moving "<<_outListFilename<<" product list in Inventory dir: "<<inventoryWorkingDir) ;
       File::rename( _outListFilename, inventoryWorkingDir+"/"+File::getFileName( _outListFilename ) );
   }

   return 0;

}


_ACS_END_NAMESPACE
