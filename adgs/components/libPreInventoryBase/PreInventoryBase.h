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

	$Prod: A.C.S. libPreInventoryBase$

	$Id$

	$Author$

	$Log$
	Revision 1.32  2016/03/09 12:10:07  marpas
	coding best practices applied
	
	Revision 1.31  2016/02/18 16:33:49  nicvac
	PDSEV-27: Multiple items handling.
	
	Revision 1.30  2015/10/06 15:15:40  lucio.pulvirenti
	S3MPM-628: convertUncorrectDate IF changed: bool ptr added (default 0) to be valorized to true if date has been converted.
	
	Revision 1.29  2015/07/17 15:24:37  lucio.pulvirenti
	S3MPM-549: DS context added.
	
	Revision 1.28  2015/06/12 10:46:03  francesco.avanzi
	best code practices applied
	getContext() method added
	such method is non-essential, but precedes the privatization of _context variable
	
	Revision 1.27  2015/03/19 17:40:36  marpas
	warning added
	
	Revision 1.26  2015/03/19 11:33:27  lucio.pulvirenti
	S3MPM-359: getSatMissionAssociatorFactoryKey public method added to IF.
	
	Revision 1.25  2015/01/27 16:15:56  lucio.pulvirenti
	removeSpuriousCharsFromStringDate method added. In base class remove just trailing 'Z' char.
	
	Revision 1.24  2014/06/05 11:17:31  marpas
	test define introduced - for white box tests
	
	Revision 1.23  2014/02/11 11:26:35  marpas
	fixing compilation warning
	
	Revision 1.22  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.21  2013/11/13 10:55:57  lucio.pulvirenti
	convertUncorrectDate method added to convert wrong dates (e.g. 9999-99-99T99:99:99) according to a configured LUT.
	
	Revision 1.20  2013/11/12 16:42:01  lucio.pulvirenti
	Method added devoted to convert file class read from file to inventory into file class occurring in db. LUT for conversion is
	extracted from configuration.
	
	Revision 1.19  2013/10/29 16:07:40  marfav
	Filterable warnings removed
	
	Revision 1.18  2013/10/29 14:48:07  marfav
	Inventory anomalies support added
	
	Revision 1.17  2013/04/29 16:27:18  marpas
	interface changed
	file type recognition class interface changed
	
	Revision 1.16  2013/02/21 11:13:24  marpas
	redesign to use connection pool and explicit connections
	coding best practices added
	test implementation completely rewritten
	qa rules
	
	Revision 1.15  2012/12/17 11:24:04  marpas
	inserter for PreInventoryBase::Context::Which added
	
	Revision 1.14  2012/12/11 17:25:07  lucio.pulvirenti
	FillMetadataFilename protected method added.
	
	Revision 1.13  2012/12/06 13:13:02  lucio.pulvirenti
	doPackaging pure virtual method IF changed.
	getAbsoluteMtdFName and getCoupledPath private methods added.
	
	Revision 1.12  2012/02/21 13:02:35  marpas
	refactoring
	
	Revision 1.11  2011/07/06 10:45:52  giufor
	implemented the virtual method getMtdFileName to get the correct filename of mtd file and updated the dowork method
	
	Revision 1.10  2009/03/25 15:40:54  nicvac
	Added overrideVersionHandling to let plugins to override doVersioning decision.
	
	Revision 1.9  2008/11/28 15:14:12  nicvac
	Deep Search implemented.
	
	Revision 1.8  2008/11/07 16:51:34  nicvac
	preProcessing implemented.
	
	Revision 1.7  2008/10/29 17:10:53  nicvac
	doPackaging method added.
	
	Revision 1.6  2008/10/29 15:44:08  nicvac
	computeMetadata signature changed.
	
	Revision 1.5  2008/10/23 08:59:37  nicvac
	program_options used. Enum modified.
	
	Revision 1.4  2008/10/13 14:49:48  nicvac
	Path with file passed to plugin.
	Context struct added.
	
	Revision 1.3  2008/10/10 16:27:56  nicvac
	getRegExp implemented.
	
	Revision 1.2  2008/10/08 13:55:29  nicvac
	Attributes and interface updated.
	
	Revision 1.1.1.1  2008/09/30 14:35:50  nicvac
	Import libPreInventoryBase
	
        
*/

#ifndef _PreInventoryBase_H_
#define _PreInventoryBase_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <Metadata.h>
#include <dbQueryParameters.h>

#include <RegExp.h>

#include <boost/program_options.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * Base class for Preinventory.
 * Define the interface for all the preinvetories
 **/

/** Couple definition */
typedef std::pair<std::string, std::string> Pair_FileName_FileType;

class dbConnection ; 
class dbConnectionPool ; 


class PreInventoryBase  {   // PRQA S 2109
public:

    /** PreInventoryBase Exceptions */
    exDECLARE_EXCEPTION(exPreInventoryBaseException, exException) ; // Base PreInventoryBase Exception. // PRQA S 2131, 2502 2
    exDECLARE_EXCEPTION(exPreInventoryBaseCriticalException, exPreInventoryBaseException) ; // Critical Exception. // PRQA S 2153
    exDECLARE_EXCEPTION(exMetadataComputationException, exPreInventoryBaseException) ; // Metadata extraction exception // PRQA S 2153

    /** Default constructor */
    PreInventoryBase();

    /** Destructor */
    virtual ~PreInventoryBase() throw() ;

    /** Parameter Settings 
    * to call for example in Task Table context.
    * path: path to scan for preinventory
    * excludeList: filenames to exclude in scan. In Wd you can have input and output files with the same FileType. Only out to Scan. */
    void setPath( const std::string& path, const std::vector<std::string>& excludeList );
    /** Parameter Settings to call for example in Import context.
    *  filename: filename to preInvetory */
    void addItem(const std::string& pathFileName, const std::string& fileType);

    /** Processing Context */
    struct Context { // PRQA S 2173, 2175
        Context() : 
            which(), 
            argvParameters(), 
            workingDir(), 
            metadataInfo(), 
            mainConfNamespace(), 
            parameterFile() {}
        /** Processing Context */
        enum Which { DIS, TL, PROD, DS };
        Which which ;                           // PRQA S 2100 
        static std::string toString(Which) ;     // PRQA S 2502
        /** Command lines parameters */
        boost::program_options::variables_map argvParameters;   // PRQA S 2100

        /** Working directory */
        std::string workingDir;                 // PRQA S 2100

        /** Metadata field retrived from interface 
        *  Use this structure only if some information cannot be obtained from the Product to PreInventory! */
        struct MetadataInfo { // PRQA S 2173, 2175
            MetadataInfo(): satelliteName(), mission(), processorName(), processorVersion(), orderId(), stationConf(),fileClassInfo()  {}
            std::string satelliteName;  // PRQA S 2100 5
            std::string mission;
            std::string processorName;
            std::string processorVersion;
            std::string orderId;
            /** Set only in DIS and TL context. In PROD Context the Product could have been created on other station */
            struct StationConf { // PRQA S 2173, 2175
                StationConf(): name(), stationId() {}
                std::string name;   // PRQA S 2100 2
                std::string stationId;
            } stationConf;              // PRQA S 2100
            struct FileClassInfo { // PRQA S 2173, 2175
                FileClassInfo(): test(false), troubleShooting(false), processingType() {}
                bool test;          // PRQA S 2100 3
                bool troubleShooting; 
                std::string processingType;
            } fileClassInfo;            // PRQA S 2100
        } metadataInfo;                 // PRQA S 2100

      /** Configuration Namespace containing main settings. PreInvetoryBase could access to Main options in configuration */
      std::string mainConfNamespace;    // PRQA S 2100
      
      std::string parameterFile;   // PRQA S 2100
    };
	
	struct FileClassLUTElement { // PRQA S 2173, 2175
		
		FileClassLUTElement() ; 
		~FileClassLUTElement() throw() {} 
		FileClassLUTElement(const FileClassLUTElement& ) ;
		FileClassLUTElement& operator=(const FileClassLUTElement& ) ;
		
		struct Conversion { // PRQA S 2173, 2175
			
			Conversion() ; 
			~Conversion() throw() {} 
			Conversion(const Conversion& ) ;
			Conversion& operator=(const Conversion& ) ;
			
			std::string originalFileClass; // PRQA S 2100 2
			std::string dbFileClass;
			
		};
		std::vector<std::string> fileTypes; // PRQA S 2100 2
		std::vector< Conversion > conversions;
	} ;

	struct TimeLUTElement { // PRQA S 2173, 2175
		
		TimeLUTElement() ; 
		~TimeLUTElement() throw() {} 
		TimeLUTElement(const TimeLUTElement& ) ;
		TimeLUTElement& operator=(const TimeLUTElement& ) ;
		
		std::vector<std::string> originalDates; // PRQA S 2100 2
		std::string convertedDate;
	} ;


    /** Return the PreInventory Id. Useful to understand to the Factory Client if the same preinventory is returned more 
    * than once.*/
    virtual std::string getClassId()=0;

    /** Add FileType to the scan list */
    void addFileType(const std::string& fileType);

    /** Create metadata and return the list of preInventoried files */
    std::vector<std::string> doWork(dbConnectionPool &);

    /** Set Metadata Info from application level (from app's inputs) */
    void setContext( const Context& context );
	
    const Context & getContext()const throw(){ return _context ;}

	void setPluginConfSpaceId( const std::string &v ) throw() { _pluginConfSpaceId = v; }
	
	std::string getPluginConfSpaceId() const throw() { return _pluginConfSpaceId; }

	void setFactoryConfSpaceId( const std::string &v ) throw() { _factoryConfSpaceId = v; }
	
	std::string getFactoryConfSpaceId() const throw() { return _factoryConfSpaceId; }

protected:

    /** Scan Working directory, setting the attribute containing the list of files to preInventory */
    virtual void scanWd(dbConnection &);

    //Interface to implement

    /** Preprocess a detected input file.
    * Input: detected input File
    * Output: Files to inventory (from an input File could be possible generate a set of files to Inventory)
    * Default Beaviour: Preproc to nothing and the returned vector has only the inputFile passed. 
    * Plugin can override the method if
    *  - The detected file must be preprocessed.
    *  - The preprocess generates a other files to be Inventoried. 
    */
    virtual std::vector< Pair_FileName_FileType > preProcess( const Pair_FileName_FileType& inputFile, dbConnection & );

    /** Preprocess the set of detected input files.
    * Input: Set of Input files detected.
    * Output: Set of Files to inventory
    * Default Beaviour: Preproc do nothing and the returned vector is the input one.
    * Plugin can override the method if the preprocessing must be done on the whole set of input files detected.
    *   If this method is overridden, the default "FileToInventory[] preProcess(inputFile)" IS NOT CALLED!
    */
    virtual std::vector< Pair_FileName_FileType > preProcess( const std::vector< Pair_FileName_FileType >& inputFiles, dbConnection &  );

    /** Return the Metadata object from a product */
    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &)=0;

    /** Return the dbQueryParameters useful to get the following:
    * select * from t_inventory where filename (LIKE|is|...) ProductFilename
    * I.e. the query to get all the existing version of a ProductFilename
    * IN MOST CASE THIS METHOD MUST BE OVERIDDEN:
    * Note: This query cannot be standard. Eg. in Cryosat the version is in the Filename, so the query must strip the 
    *       version from the filename and use LIKE */
    virtual dbQueryParameters getProductFilenameInInventoryQueryParameters( dbConnection &, const Metadata& metadata );

    /** Update the Product (header, attributes, etc.) and Metadata, according to the new version */
    virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata )=0;

    /**
     * \brief  Package the Product (eg. attaching header to bin, create a tar, etc., or just do nothing).
     * \param Metadata obj
     * \param File full path corresponding either to header or single file depending on the next parameter is valorized
     * \param File type
     * \param File full path corresponding to data only if the previous parameter is valorized (otherwise it must be null)
     * \retval Package file full path
     */    	
    virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection & )=0;

    /** Using Metadata, check if needed a new version in Db, quering the Inventory.
    * Return true if the new version is needed.
    * Return the new version to be used.  */
    virtual bool doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &);

    /** Beside the general setting, the plugin can override the Versioning handling.
     * Return true if the plugin want override the FrameWork's version general settings
     * Return doVersioning = true/false to enable/disable version handling */
    virtual bool overrideVersionHandling(bool& doVersioning);

    /** Return the name of metadata file without extension */
    virtual std::string getMtdFileName( const std::string& pathFileName, const std::string& fileType, Metadata& metadata);

	/**
	 * \brief Fill correctly the metadata filename. Based on the content of t_filetypes proper record
	 * \param File path
	 * \param File type
	 * \param Metadata
	 * \param t_filetypes record mngsngext value ptr
	 * \param t_filetypes record sng value ptr: valorized with the proper string only if mngsngext value on db is FALSE, otherwise valorized with empty string
	 * \retval The metadata filename
	 */    	
	std::string fillMetadataFilename( const std::string&, 
									  const std::string&, 
									  Metadata& metadata,
                                      dbConnection &,
									  bool * = 0,
									  std::string * = 0);

	/**
	 * \brief Format version string accordingly to ProdPreInventory configuration
	 * \param Unformatted version string
	 * \retval Formatted version string
	 */    	
	std::string getFormattedVersion( int );
	
	/**
	 * \brief Convert file class got from file to preinventory into a value occurring in db according to a LUT defined in plugin conf space.
	 * \param Plugin identifier in conf space
	 * \param File type
	 * \param Original file class
	 * \retval Converted file class
	 */    	
	virtual std::string convertOriginalFileClass( const std::string&, const std::string&, const std::string&) const;

	/**
	 * \brief Convert date got from file to preinventory if uncorrect (e.g. UTC=9999-99-99T99:99:99) into a correct value according to a LUT defined in plugin conf space.
	 * \param Plugin identifier in conf space
	 * \param Original date
	 * \param bool Ptr: if valorized, true if converted
	 * \retval Converted date
	 */    	
	virtual std::string convertUncorrectDate( const std::string&, const std::string&, bool *convertedPtr = 0);
	
	/**
	 * \brief Remove spurious characters, e.g. trailing 'Z', from date string
	 * \param Original date
	 * \retval Converted date
	 */    	
	virtual std::string removeSpuriousCharsFromStringDate( const std::string& );

	/**
	 * \brief Get SatelliteMissionAssociator factory key from configuration
	 * \param The satellite code
	 * \param The tag where SatMissionsAssociatorKeys node is located
	 * \retval The factory key, empty if not found
	 */    	
	virtual std::string getSatMissionAssociatorFactoryKey(const std::string & satCode, const std::string & parentTag );
	
	/**
	 * \brief Using Metadata, check if needed a new version in Db, by using FileInventoryVersion obj
	 * \param The metadata
	 * \param The key of ProductFileNameFactory object
	 * \param The new version to be used
	 * \param db Connection
	 * \param If valorized, the returned filename
	 * \retval true if versioning to be done
	 */    	
    bool doVersioningByFileInventoryVersion(const Metadata& , const std::string & key, std::string& newVersion, dbConnection &, std::string * retFilename = 0);
	
private:
    std::string processFile(const std::string &, const std::string &, dbConnection &) ;
    /** Return the name of metadata file with extension */
    std::string getAbsoluteMtdFName( const std::string& path ) const ;

    /** Query DB to get FileType's RegularExpression */
    std::string getFileTypeRegEx(const std::string& fileType, dbConnection &);

    /** Find files in Subdirectories matching the Regular Expression */
    void findFiles(const std::string& fileType, const RegExp& productRegExp,
                   const std::string& directory, 
                   const unsigned int& currentDeepLevel,   const unsigned int& maxDeepLevel );

#ifdef PREINVENTORYBASE_CLASS_TEST 
public: 
#endif
    /**
    * \brief Get the coupled path, if any, of the input path (e.g. HDR -> DBL). Based on the content of t_filetypes proper record
    * \param File path
    * \param File type
    * \retval Either coupled path or empty string
    */    	
    std::string getCoupledPath( const std::string &, const std::string &, dbConnection & ) const;

private:  // copy constructor and operator= defined but not implemented
    /** Copy Constructor */
    PreInventoryBase(const PreInventoryBase & );
    /** Operator = */
    PreInventoryBase &operator=(const PreInventoryBase &);

#ifdef PREINVENTORYBASE_CLASS_TEST 
public: 
#else
private:
#endif

   /** Metadata field retrieved from interface */
   Context _context;


   /** Preiventory Mode: Scan Dir or File access */
   bool _scanMode;

   //*/Scan Mode
   //Path to consider
   std::string _path;
   //In Wd out file with fileType F1 ==> to preInventory,
   //but in Wd it could be a input file with filetype F1 ==> to exclude
   std::vector<std::string> _excludeList;
   //*/

   /** PreInventoried files to put in the FileList */
   std::vector<std::string> _filesPreInventoried;

   /** List of selected fileType to preInvenotry (a sub set that can be handled by the plugin) */
   std::vector<std::string> _selectedFiletypes;

   /** Scanned files in input to preInventory: set of pairs "<filename, filetype>" */
   std::vector< Pair_FileName_FileType > _inputFiles;

   bool _manageInventoryAnomalies;
   
   std::vector<TimeLUTElement> _timeLUTElements;
   
	std::string _pluginConfSpaceId;
	std::string _factoryConfSpaceId;

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL (PreInventoryBase)

};

std::ostream &operator << (std::ostream &,PreInventoryBase::Context::Which) ; 
acs::exostream &operator << (acs::exostream &,PreInventoryBase::Context::Which) ; // PRQA S 2072

std::ostream &operator<<(std::ostream &, const PreInventoryBase::FileClassLUTElement &) ;
acs::exostream &operator<<(acs::exostream &, const PreInventoryBase::FileClassLUTElement &) ; // PRQA S 2072

std::ostream &operator<<(std::ostream &, const PreInventoryBase::TimeLUTElement &) ;
acs::exostream &operator<<(acs::exostream &, const PreInventoryBase::TimeLUTElement &) ; // PRQA S 2072


_ACS_END_NAMESPACE

#endif //_PreInventoryBase_H_

