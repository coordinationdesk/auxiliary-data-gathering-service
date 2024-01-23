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

	$Prod: A.C.S. libEOFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.23  2015/11/27 16:49:37  francesco.avanzi
	MPM EOF files now added
	
	Revision 1.22  2015/07/09 16:25:43  matteo.airoldi
	Versioning in the ADM context has been fixed
	XSLT is now used to change HDR and EEF contents
	
	Revision 1.21  2015/06/03 12:27:46  francesco.avanzi
	conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.20  2015/05/28 14:31:42  francesco.avanzi
	best code practices applied in overrideVersionHandling
	
	Revision 1.19  2015/05/27 15:08:41  francesco.avanzi
	modified to best derivate EOFADMPreInventory
	
	Revision 1.18  2014/07/23 10:05:57  lucio.pulvirenti
	S2PDGS-738: bool parameter (OUT) passed to getQualifiedValue private method.
	
	Revision 1.17  2014/05/15 10:35:25  lucio.pulvirenti
	S2PDGS-738: getNoSchemaAvailableFlag method added.
	
	Revision 1.16  2014/02/11 11:08:29  francesco.avanzi
	better code practices applied
	
	Revision 1.15  2013/11/13 10:58:02  lucio.pulvirenti
	_pluginConfSpaceId static const member added to interface.
	
	Revision 1.14  2013/09/02 13:00:05  lucio.pulvirenti
	convertMissionId2INVMission method erased.
	getSatMissionAssociatorFactoryKey method and _defaultSatMissionAssociatorFactoryKey static const var introduced.
	
	Revision 1.13  2013/04/29 16:28:33  marpas
	base class interface changed
	file type recognition interface changed
	
	Revision 1.12  2013/04/04 10:57:29  lucio.pulvirenti
	getQualifiedValue method added.
	
	Revision 1.11  2013/04/03 15:39:30  lucio.pulvirenti
	Work in progress.
	
	Revision 1.10  2013/02/25 09:38:45  lucio.pulvirenti
	Base class doVersioning method re-implemented: versioning performed only if no inventory record
	with both the same filename and the same version occurs.
	
	Revision 1.9  2013/02/20 16:45:46  lucio.pulvirenti
	Added dbConnection to preProcess, computeMetadata, prepareMetadata, getSensorIdFromSensorCode, getSatelliteIdFromSatelliteCode.
	_currentFileType private member added.
	qa rules.
	
	Revision 1.8  2012/12/12 11:45:34  lucio.pulvirenti
	_manageSngExt member removed since now useless.
	
	Revision 1.7  2012/12/06 11:29:20  lucio.pulvirenti
	doPackage modified according of the new IF of the base class.
	Private members added to save both data file extension and package file extension.
	Private member added to store information on management of sng extension.
	
	Revision 1.6  2012/11/30 16:30:01  lucio.pulvirenti
	work in progress.
	
	Revision 1.5  2012/11/29 16:59:32  lucio.pulvirenti
	work in progress.
	
	Revision 1.4  2012/10/17 10:26:04  giufor
	reimplemented preProcess and doPackaging methods to consider case of HDR/DBL files
	
	Revision 1.3  2012/10/12 14:04:36  giufor
	implemented readEOFFile to read FixedHeader section of generic EahrtObservationFile) .EOF or .HDR in a structure
	
	Revision 1.2  2012/10/11 10:39:59  giufor
	created empty class
	
	Revision 1.1.1.1  2012/10/11 09:56:12  giufor
	Import libEOFPreInventory
	
        
*/

#ifndef _EOFPreInventory_H_
#define _EOFPreInventory_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <PreInventoryBase.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for EOF(Earth Observation File) products
 **/

class EOFPreInventory : public PreInventoryBase // PRQA S 2109
{ 
public:

	/** EOFPreInventory Exceptions */
	exDECLARE_EXCEPTION(exEOFPreInventoryException, exException) ; // Base EOFPreInventory Exception. // PRQA S 2131, 2502

	/** Default Class constructor */
	EOFPreInventory();
	/** Destructor */
	virtual ~EOFPreInventory() throw();

	virtual std::string getClassId();
	
    const std::string & getCurrentFileName()const throw(){ return _currentFileName; }
    void setCurrentFileName(const std::string &s){  _currentFileName = s; }

	const std::string & getCurrentFileType()const throw(){ return _currentFileType; }
    void setCurrentFileType(const std::string &s){  _currentFileType = s; }

   bool isHdr() const throw();

	const std::string & getSngExtension()const throw(){ return _sngExtension; }

#ifdef ACS_TEST
public:
#else
protected:
#endif
	using PreInventoryBase::preProcess;
    virtual std::vector< Pair_FileName_FileType > preProcess( const Pair_FileName_FileType& inputFile, dbConnection &);

    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &);


    virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata);

	 /**
	 * \brief  Package the Product (eg. attaching header to bin, create a tar, etc., or just do nothing).
	 * \param Metadata obj
	 * \param File full path corresponding either to header or single file depending on the next parameter is valorized
	 * \param File type
	 * \param File full path corresponding to data only if the previous parameter is valorized (otherwise it must be null)
	 * \retval Package file path
	 */    	
    virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection & );

	/** \brief It returns true.
	 * \brief  doVersioning returned value is true/false depending on if versioning must be performed on the current product type 
     */
	virtual bool overrideVersionHandling(bool& doVersioning);

	struct FixedHeaderStruct // PRQA S 2173, 2175
    {
        FixedHeaderStruct(): File_Name(), File_Description(), Notes(), Mission(), File_Class(), File_Type(), Validity_Start(), Validity_Stop(),
	                         File_Version(), System(), Creator(), Creator_Version(), Creation_Date() 
        {}

        // PRQA S 2100 L1
        std::string File_Name;
        std::string File_Description;
        std::string Notes;
        std::string Mission;
        std::string File_Class;				
        std::string File_Type;
        std::string Validity_Start;
        std::string Validity_Stop;
        std::string File_Version;
        std::string System;
        std::string Creator;
        std::string Creator_Version;
        std::string Creation_Date;
        // PRQA L:L1                                
    };


	//extracts information from EOF file 
	void readEOFFile( const std::string& pathFileName, FixedHeaderStruct& fhs);


	virtual std::string getUniqueId();
	std::string getParametersFile();

	Metadata prepareMetadata( const std::string& fileType, const FixedHeaderStruct& fhs, dbConnection &);

	int getSensorIdFromSensorCode( const std::string& sensorCode, dbConnection &);
	int getSatelliteIdFromSatelliteCode( const std::string& satCode, dbConnection &);
	
	/**
	* \brief  Using Metadata, check if needed a new version in Db by quering the Inventory: a new version is needed if an entry with the
	* same filename and version already occurs.
	* \param Metadata obj
	* \param OUT The new version to be used. 
	* \param dbConnection
	* \retval true if the new version is needed
	*/    	
    virtual bool doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &);

	virtual std::string getQualifiedValue(bool & noSchemaAvailable);
	
	/**
	 * \brief Get SatelliteMissionAssociator factory key from configuration
	 * \param The satellite code
	 * \retval The factory key
	 */    	
	std::string getSatMissionAssociatorFactoryKey(const std::string &);
	
	/**
	 * \brief Get the flag corresponding to no xsd schema occurrence into the file to validate. To be put into t_inventory qualified col.
	 * \retval No xsd schema occurrence flag
	 */    	
	std::string getNoSchemaAvailableFlag();
	

	virtual void extractFurther(rsResourceSet &) {} //PRQA S 2131 2
    virtual void addFurtherNodes(std::vector<std::string> &) {}
    virtual void setFurther(Metadata &m) {}
	virtual std::string getFilepathToValidate() { return _currentFileName; }
	virtual bool extractMetadataFromHdr(const std::string & filepath ) ;
private:	
	EOFPreInventory(const EOFPreInventory & ); // not implemented
	EOFPreInventory &operator=(const EOFPreInventory &); // not implemented
	
	std::string getSatelliteAcronymFromConf( const std::string& satCode, int & mission);


private:
	std::string _currentFileName;
	std::string _currentFileType;
	std::map <std::string, bool> _isHDR;
	std::string _datExtension;  // extension of data file
	std::string _sngExtension;  // sng extension (to be used for package file)
	static const std::string _defaultSatMissionAssociatorFactoryKey;
	static const std::string _noSchemaAvailableFlagTag;
	static const std::string _defaultQualifiedFlagNoSchema;
	

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFPreInventory);
};

_ACS_END_NAMESPACE

#endif //_EOFPreInventory_H_

