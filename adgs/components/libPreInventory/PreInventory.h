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
	Revision 1.9  2016/11/21 16:12:17  marfav
	S3MPM-847
	Added functionality to show plugins capabilities
	Subclass specific plugin loading moved in a dedicated virtual operation
	
	Revision 1.8  2016/02/18 16:35:44  nicvac
	PDSEV-27. Multiple items handling.
	
	Revision 1.7  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.6  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
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

#ifndef _PreInventory_H_
#define _PreInventory_H_

#include <acs_c++config.hpp>
#include <PreInventoryBase.h>

#include <ConfigurablePluginLoader.h>

#include <exException.h>

#include <boost/program_options.hpp> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)
    
/**
 * \brief
 * Main Preinventory Class 
 **/
class dbConnectionPool ;
class PluginLoaderNotifier ; 

class PreInventory  { // PRQA S 2109
public:

  /** PreInventory Exceptions */
  exDECLARE_EXCEPTION(exPreInventoryException, exException) ; // Base PreInventory Exception. // PRQA S 2131, 2502 2
  exDECLARE_EXCEPTION(exPreInventoryCriticalException, exPreInventoryException) ; // Critical Exception. // PRQA S 2153


  /** Default Class constructor */
  PreInventory();
  /** Destructor */
  virtual ~PreInventory()  throw() ;

protected:

    // PRQA S 2101 L1
   /** PreInventory Definition - Entry in the PreInventory's Factory*/
   std::string _satellite;  
   std::string _mission;
   std::string _procName;
   std::string _procVersion;
   std::vector<std::string> _fileTypes; //FileTypes to handle
   //*/

   /** Working dir */
   std::string _workingDir;

   /** Preinventory Modes: */
   /** Scan=true: Scan a path to Search files matching filetypes
    *  Scan=false: Specified a list of files to handle */
   bool _scanMode;

   /** 1) DIR SCAN MODE: Use filetypes' regular expression to get the list of files to be pre-inventoried */

   /** DirName to Scan. Contains items to be Preinventoried */
   std::string _dirNameToScan;
   /**In Wd out file with fileType F1 ==> to preInventory,
    * but in Wd it could be a input file with filetype F1 ==> to exclude */
   std::vector<std::string> _excludeList;

   /** 2) LIST OF ITEMS MODE: Explicit list of ( filetype, filename+ ) to be Preinventoried */
   std::map< std::string, std::vector < std::string > > _itemsToTake;

   /** Context & MetadataInfo fields to fill from input interface */
   PreInventoryBase::Context _context;

   /** Filename of List of PreInventoried Files */
   std::string _outListFilename;
   // PRQA L:L1

private:
    PreInventory(const PreInventory & ); // not implemented
    PreInventory &operator=(const PreInventory &); // not implemented
    /** List of preInventory to call */
    std::map<std::string, PreInventoryBase*> _preInventoryList;

    /** PlugIn Loader.  The loader must have the same lifetime of the application */
    ConfigurablePluginLoader _pluginLoader;

	// If set show capabilites and exits without performing any job
	bool _doShowCap;


private:
   /** Load Plugins from the configured path */
   void loadPlugIns(const PluginLoaderNotifier &);
   void showCapabilities () const;


protected:
  /** Initialization from available inputs. Set the Protected attributes */
  virtual void initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &)=0;
  virtual void loadSpecificPlugins();

public:
  /** Entry point using command line parameters */
  int execute(const boost::program_options::variables_map& , char const * const * , dbConnectionPool &, const PluginLoaderNotifier &pln=PluginLoaderNotifier());

  // Sets the _doShowCap attribute
  void doShowCapabilities(bool b);
  bool doShowCapabilities() const;


private:
  ACS_CLASS_DECLARE_DEBUG_LEVEL(PreInventory)

};


_ACS_END_NAMESPACE

#endif //_PreInventory_H_

