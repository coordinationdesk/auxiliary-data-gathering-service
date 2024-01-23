// PRQA S 1050 EOF
/*
	Copyright 2013-2019, Advanced Computer Systems , Inc.
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
	Revision 1.9  2016/03/11 09:08:04  tergem
	DefaultJobOrder dependency removed. Parameter File read from context
	
	Revision 1.8  2015/07/15 15:57:23  lucio.pulvirenti
	S3MPM-549: versioning now enabled. Temporary work around not to throw exception if context is TL and uniqueID is non available. It will be fixed
	in the same issue.
	
	Revision 1.7  2015/06/19 12:50:13  nicvac
	S3MPM-533: Support for S3 products. Added also Sensor id and Geolocation.
	
	Revision 1.6  2015/03/19 13:49:49  lucio.pulvirenti
	S3MPM-359: getSatMissionAssociatorFactoryKey method moved into base class.
	
	Revision 1.5  2013/11/13 17:24:30  lucio.pulvirenti
	Both StartValidity and StopValidity dates and creation date checked with base function convertUncorrectDate to let configure possibly a Time LUT.
	
	Revision 1.4  2013/08/09 13:13:36  lucio.pulvirenti
	convertMissionId2INVMission method removed.
	getSatMissionAssociatorFactoryKey method and _defaultSatMissionAssociatorFactoryKey static var added.
	
	Revision 1.3  2013/07/02 10:39:46  lucio.pulvirenti
	fileClass added to ManifestData struct.
	getFactoryKey and getFiletypeKey methods added.
	Methods reimplemented from base class now protected since they could be re-implemented by derived classes.
	
	Revision 1.2  2013/07/01 16:42:30  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	
	
        
*/

#ifndef _SafePreInventory_H_
#define _SafePreInventory_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <PreInventoryBase.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for SAFE products: metadata are extracted from manifest file
 **/

class SafePreInventory : public PreInventoryBase // PRQA S 2109
{ 
public:

// Manifest data needed for preInventory
	struct ManifestData // PRQA S 2173, 2175
    {
        // PRQA S 2100 L1
        std::string filename;
        std::string filetype;
        std::string mission;
        std::string instrumentName;
        std::string startTime;
        std::string stopTime;
		std::string productSize;
        std::string productVersion;
		std::string fileClass;
		std::string softwareName;
 		std::string softwareVersion;
       	std::string creationTime;
        std::string facilityName;
        std::string qualityFlag;
		std::string startOrbitNumber;
		std::string stopOrbitNumber;
		std::string footPrintPosList;
		std::vector< std::pair<std::string, std::string> > additionalData; // name -> value; name must be match a metadata field
        // PRQA L:L1 
		
		ManifestData();
		ManifestData (ManifestData const &) ;
		ManifestData &operator=(ManifestData const &) ;
		~ManifestData() throw() {}
                               
    };

	/** SafePreInventory Exceptions */
	exDECLARE_EXCEPTION(exSafePreInventoryException, exException) ; // Base SafePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exSafePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	SafePreInventory();
	/** Destructor */
	virtual ~SafePreInventory() throw();

	virtual std::string getClassId();
	
protected:
	
    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &);

	// fill ManifestData struct by reading the Manifest file
	virtual void readManifest( const std::string &, const std::string&, ManifestData & );

	virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata);

	 /**
	 * \brief  Do nothing
	 * \param Metadata obj
	 * \param File full path corresponding either to header or single file depending on the next parameter is valorized
	 * \param File type
	 * \param File full path corresponding to data only if the previous parameter is valorized (otherwise it must be null)
	 * \retval Package file path
	 */    	
    virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection & );

	 /** \brief It returns true if doVersioning tag occurs in configuration for the current product type.
	 * \brief  doVersioning returned value is true/false depending on if versioning must be performed on the current product type 
     */
	virtual bool overrideVersionHandling(bool& doVersioning);

	/**
	* \brief  Using Metadata, check if needed a new version in Db by quering the Inventory: a new version is needed if an entry with the
	* same filename and version already occurs.
	* \param Metadata obj
	* \param OUT The new version to be used. 
	* \param dbConnection
	* \retval true if the new version is needed
	*/    	
    virtual bool doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &);

	 /**
	 * \brief Compute the factory key on the base of product type and of satellited id extracted from e.g. filename
	 * \param Vector of strings needed to compute the factory key
	 * \retval The factory key
	 */    	
	virtual std::string getFactoryKey(const std::vector<std::string> &);
	
	/**
	 * \brief It responds to need of setting other metadata fields (or change something) in derivated classes
	 * \param Metadata object
	 * \param dbConnection object
	 */    	
	virtual void setFurther(Metadata &, dbConnection &) {}

	/**
	 * \brief It manages foot points list depending on SAFE format
	 * \param ManifestData object
	 * \param Metadata object
	 */    	
	virtual void manageFootPrintPosList(const ManifestData& , Metadata &) ;
	
	 /**
	 * \brief Get the key from filetype needed to compute factory key
	 * \param The filetype
	 * \retval The filetype key
	 */    	
	std::string getFiletypeKey(const std::string &);
	 /**
	 * \brief Get the manifest name configured in the specific filetype section
	 * \param The manifest filename
	 * \retval True if the manifest specific tag is found
	 */   	
	bool getManifestFilename(std::string & manifestname);

	/**
	 * \brief Extract the vector of pair ("XPath", "field name") from manifest: field name is a name of metadata parameter.
	 * \param (OUT) The vector of pair
	 */    	
	void extractAdditionalManifestFieldsXPaths(std::vector< std::pair<std::string, std::string> > &);

	Metadata prepareMetadata( const std::string& fileType, const ManifestData& , dbConnection &);
	
	void setCurrentFileName( const std::string & s) { _currentFileName = s ; }
	std::string getCurrentFileName() { return _currentFileName ; }

	void setCurrentFileType( const std::string & s) { _currentFileType = s ; }
	std::string getCurrentFileType() { return _currentFileType ; }

private:
		
	SafePreInventory(const SafePreInventory & ); // not implemented
	SafePreInventory &operator=(const SafePreInventory &); // not implemented

	std::string getUniqueId();

	int getSatelliteIdFromSatelliteCode( const std::string& satCode, dbConnection &);
	

public:
    //classe di Test CPPUnit
#ifdef CLASS_UNDER_TEST
    friend class SafePreInventoryTest;
#endif //CLASS_UNDER_TEST

private:
	std::string _currentFileName;
	std::string _currentFileType;
	std::string _ftypeParentTag;

	static const std::string _defaultSatMissionAssociatorFactoryKey;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SafePreInventory);

};

_ACS_END_NAMESPACE

#endif //_SafePreInventory_H_

