// PRQA S 1050 EOF
/*
	Copyright 2013-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.10  2017/07/28 09:14:49  lucio.pulvirenti
	ECICMF-108: setCurrentFilename method added.
	
	Revision 1.9  2017/07/14 14:35:47  lucio.pulvirenti
	ECICMF-108: getCurrentFileName and getCurrentFileType methods added to IF. empty setFurther method added to be re-implemented by derived classes.
	
	Revision 1.8  2016/05/18 08:31:42  nicvac
	Unique id is passed in parameter file only in DS case. Also protected to be overridden by subclasses.
	
	Revision 1.7  2016/04/15 08:13:54  lucio.pulvirenti
	S3MPM-695: getBeginOfMission and getEndOfMission methods added to get BeginOfMission and EndOfMission from configuration if start/stop time not occurring in product name.
	
	Revision 1.6  2016/03/11 09:10:33  tergem
	DefaultJobOrder dependency removed. Parameter File read from context
	
	Revision 1.5  2015/06/05 13:53:24  lucio.pulvirenti
	S3MPM-504: getPluginConfSpaceId method introduced to return plugin node in configuration space.
	
	Revision 1.4  2014/03/12 11:13:34  lucio.pulvirenti
	New debug macros used.
	getSatelliteId virtual method used to be re-implemented by derived classes if needed.
	If FilenameElements::orbitnumber not empty, write it into metadata.
	getProductFilenameInInventoryQueryParameters now get connection as parameter.
	
	Revision 1.3  2014/03/11 17:02:14  lucio.pulvirenti
	Work in progress.
	
	Revision 1.2  2013/09/20 11:40:40  lucio.pulvirenti
	GeoCoords vector added to FilenameElements struct;
	doVersioning method added.
	If GeoCoords elements valorized, metadata properly filled.
	
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
		
	
        
*/

#ifndef _FilenamePreInventory_H_
#define _FilenamePreInventory_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <PreInventoryBase.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet;
/**
 * \brief
 * PreInventory plugin for EOF(Earth Observation File) products
 **/

class FilenamePreInventory : public PreInventoryBase // PRQA S 2109
{ 
public:
		
	struct FilenameElements // PRQA S 2173, 2175
    {
        FilenameElements(): Satellite(), Mission(), ClassId(), File_Type(), StartDate(), StopDate(), File_Version(), GeneratingCentre(), 
							CreationDate(), OrbitNumber(), geoCoords(), additionalInfo()
        {}
		FilenameElements(const FilenameElements & ); 
		FilenameElements &operator=(const FilenameElements &); 
		~FilenameElements() throw() {}

        // PRQA S 2100 L1
		std::string Satellite;
        std::string Mission;
        std::string ClassId;				
        std::string File_Type;
        std::string StartDate;
        std::string StopDate;
        std::string File_Version;
        std::string GeneratingCentre;
        std::string CreationDate;
		std::string OrbitNumber;
		std::vector<std::string> geoCoords; // latFirst, lonFirst, latLast, lonLast
		std::map<std::string, std::string> additionalInfo;
        // PRQA L:L1                                
    };

	/** FilenamePreInventory Exceptions */
	exDECLARE_EXCEPTION(exFilenamePreInventoryException, exException) ; // Base FilenamePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exFilenamePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	FilenamePreInventory();
	/** Destructor */
	virtual ~FilenamePreInventory() throw();

	virtual std::string getClassId() = 0;
	
	void setConfigurationRootTag(const std::string & s) { _configurationRootTag = s;  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _configurationRootTag);}
	std::string getConfigurationRootTag() const { return _configurationRootTag; } 
	
	std::string getPluginConfSpaceId() const;
	
	void setCurrentFileName(const std::string & v ) { _currentFileName = v; }
	std::string getCurrentFileName() const { return _currentFileName; }
	std::string getCurrentFileType() const { return _currentFileType; }

protected:
	
	std::string getFtypeParentTag(const std::string &, const rsResourceSet &);
	
    template <typename T>
	bool findConfigurationValue(const std::string & filetype, const rsResourceSet & rs, const std::string & tag, T & value)
	{
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to search \"" << tag << "\" value for \"" << filetype << "\" file type");

		std::string parentTag = getFtypeParentTag(filetype, rs);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filetype parent tag =\""  << parentTag << "\"");

		bool tagFound = false;
		try {
			std::string localTag = parentTag + "." + tag;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag=\"" << localTag << "\"");
			rs.getValue( localTag, value);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value=\"" << value << "\"");
			tagFound = true;
		}
		catch(rsResourceSet::NotFoundException &e)
		{}

		return tagFound;
	}
	
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
	* \brief  Get the satellite id according to db record given satellite acronym/name. To be specialized by derived classes if needed.
	* \param satellite name/acronym
	* \param dbConnection
	* \retval satelliteId
	*/    	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);
	
	/**
	* \brief  Get the begin of missione value in BeginOfMission tag.
	* \param rsResourceSet obj
	* \OUT begin of mission
	*/    	
	void getBeginOfMission(const rsResourceSet & , std::string &);
	
	/**
	* \brief  Get the end of missione value in EndOfMission tag.
	* \param rsResourceSet obj
	* \OUT end of mission
	*/    	
	void getEndOfMission(const rsResourceSet & , std::string & );


	/** compute Unique id for the item to be inventoried */
	virtual std::string getUniqueId();

    virtual void setFurther(Metadata &, dbConnection &) {}
	
	/**
	* \brief  Get a configuration value searching it first on filetype base and then in configuration root.
	* \param filetype
	* \param rsResourceSet obj
	* \param configuratiom tag the value is to be extracted from
	* \param configuratiom root tag the value is to be extracted from if not found in the proper filetype.
	* \param OUT configuration value found
	* \retval satelliteId
	*/    	
	template <typename T>
	bool getConfigurationValue( const std::string & filetype, const rsResourceSet & rs, const std::string & tag, const std::string & confRootTag,  T & value )
	{
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
		
		value = T(); // initizialize
		bool tagFound = findConfigurationValue(filetype, rs, tag, value);
		if( tagFound ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, tag << " value from " << filetype << " filetype configuration=" << value );
    	}
		else {
			try {
				rs.getValue( confRootTag, value);
				tagFound = true;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, tag << " value from " << confRootTag << " configuration=" << value );
			}
			catch(rsResourceSet::NotFoundException & ) {}
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << " tag " << ( tagFound ? "" : "NOT " ) << " found in configuration");
		return tagFound;

	}
	
	
	void checkFieldLenWithinFilename(const size_t startPos, const size_t fieldLength, const size_t filenameLength, const std::string & field ) ;

private:
		
	FilenamePreInventory(const FilenamePreInventory & ); // not implemented
	FilenamePreInventory &operator=(const FilenamePreInventory &); // not implemented

	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & ) = 0;

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


	Metadata prepareMetadata( const std::string& fileType, const FilenameElements &,  dbConnection &);

	/** \brief 
	 * \brief  doVersioning returned value is true/false depending on if versioning must be performed on the current product type 
     */
	virtual bool overrideVersionHandling(bool& doVersioning);
	

	void setFtypeParentTag(const std::string &, const rsResourceSet &);


public:
    //classe di Test CPPUnit
#ifdef CLASS_UNDER_TEST
    friend class FilenamePreInventoryTest;
#endif //CLASS_UNDER_TEST

private:
	
	std::string _currentFileName;
	std::string _currentFileType;
	std::string _configurationRootTag;
	std::string _ftypeParentTag;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(FilenamePreInventory)

};

_ACS_END_NAMESPACE

#endif //_FilenamePreInventory_H_

