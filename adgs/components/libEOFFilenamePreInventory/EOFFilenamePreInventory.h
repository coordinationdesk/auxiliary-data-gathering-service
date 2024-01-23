// PRQA S 1050 EOF
/*
	Copyright 1995-2021 Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: libEOFFilenamePreInventory $

	$Id$

	$Author$

	$Log$
	Revision 1.6  2014/02/11 11:09:23  francesco.avanzi
	better code practices applied
	
	Revision 1.5  2013/04/29 17:43:09  marpas
	base class interface changed
	
	Revision 1.4  2013/03/19 17:50:36  lucio.pulvirenti
	Implementation is over. Test performed.
	
	Revision 1.3  2013/03/18 17:46:26  lucio.pulvirenti
	work in progress.
	
	Revision 1.2  2013/03/15 18:09:51  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/03/15 18:04:38  lucio.pulvirenti
	Import libEOFFilenamePreInventory
	
        
*/

#ifndef _EOFFilenamePreInventory_H_
#define _EOFFilenamePreInventory_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <PreInventoryBase.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for EOF(Earth Observation File) products
 **/

class EOFFilenamePreInventory : public PreInventoryBase // PRQA S 2109
{ 
public:

	/** EOFFilenamePreInventory Exceptions */
	exDECLARE_EXCEPTION(exEOFFilenamePreInventoryException, exException) ; // Base EOFFilenamePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exEOFFilenamePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	EOFFilenamePreInventory();
	/** Destructor */
	virtual ~EOFFilenamePreInventory() throw();

	virtual std::string getClassId();
#ifdef EOFFilenamePreInventory_TEST
public:
#else
private:
#endif
		
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

	EOFFilenamePreInventory(const EOFFilenamePreInventory & ); // not implemented
	EOFFilenamePreInventory &operator=(const EOFFilenamePreInventory &); // not implemented


	struct FilenameElements // PRQA S 2173, 2175
    {
        FilenameElements(): File_Name(), Mission(), File_Class(), File_Type(), Validity_Start(), Validity_Stop(), File_Version(), Site_Centre(), 
							Creation_Date(), additionalInfo()
        {}

        // PRQA S 2100 L1
        std::string File_Name;
        std::string Mission;
        std::string File_Class;				
        std::string File_Type;
        std::string Validity_Start;
        std::string Validity_Stop;
        std::string File_Version;
        std::string Site_Centre;
        std::string Creation_Date;
		std::map<std::string, std::string> additionalInfo;
        // PRQA L:L1                                
    };


	//extracts information from EOF format filename
	void parseFilename( const std::string &, const std::string&, FilenameElements & );


	std::string getUniqueId();
	//std::string getParametersFile();

	Metadata prepareMetadata( const std::string& fileType, const FilenameElements& , dbConnection &);

	int getSatelliteIdFromSatelliteCode( const std::string& satCode, dbConnection &);
	int convertMissionId2INVMission( const std::string& missionId);

#if(0)	
	/**
	* \brief  Using Metadata, check if needed a new version in Db by quering the Inventory: a new version is needed if an entry with the
	* same filename and version already occurs.
	* \param Metadata obj
	* \param OUT The new version to be used. 
	* \param dbConnection
	* \retval true if the new version is needed
	*/    	
    bool doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &);


	/** \brief It returns true.
	 * \brief  doVersioning returned value is true/false depending on if versioning must be performed on the current product type 
     */
	virtual bool overrideVersionHandling(bool& doVersioning);
#endif	
	
	/**
	* \brief Set Registration node of the current filetype from configuration.
	*/    	
	void setConfigRegistrationNode( const std::string & fileType );


	void getFilenameFieldInfo( const std::string & tagStart, const std::string & tagLen, int & fieldStart, int & fieldLen);

public:
    //classe di Test CPPUnit
#ifdef CLASS_UNDER_TEST
    friend class EOFFilenamePreInventoryTest;
#endif //CLASS_UNDER_TEST

private:
	std::string _currentFileName;
	std::string _configRegistrationNode; // configuration tag to point to the right object on the base of file type.
	bool _pdgsTailored;
	static const std::string _InventoryTimeForMissionBegin;
	static const std::string _InventoryTimeForMissionEnd;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFFilenamePreInventory);

};

_ACS_END_NAMESPACE

#endif //_EOFFilenamePreInventory_H_

