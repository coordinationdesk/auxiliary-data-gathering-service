// PRQA S 1050 EOF
/*
	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libPreInventoryBase $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2015/05/25 14:16:05  marpas
	coding best practice applied
	
	Revision 1.3  2015/04/28 13:49:31  marpas
	static string could be deleted twice.
	
	Revision 1.2  2015/03/05 10:41:45  marpas
	removed ACSTimeConv dependency
	added DefaultJobOrder dependency
	
	Revision 1.1  2015/03/04 10:48:18  marpas
	importing PDGSExtDataPreInventory class from libPDGSExtDataPreInventory
	
	Revision 1.6  2014/03/26 15:33:59  lucio.pulvirenti
	Static const maxStopValidityTime added with corrispondent method.
	
	Revision 1.5  2014/03/25 16:13:40  lucio.pulvirenti
	convertGpsTime: format now given in input.
	
	Revision 1.4  2014/03/21 16:04:34  lucio.pulvirenti
	convertGpsTimeToUTC method moved to base class.
	
	Revision 1.3  2014/03/17 11:39:41  lucio.pulvirenti
	filleMetadataFilenamePED method introduced in IF to let re-definition of file names into base class.
	Generating Centre got by base class by configuration.
	
	Revision 1.2  2014/03/14 16:41:11  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2014/03/14 12:22:32  lucio.pulvirenti
	Import libPDGSExtDataPreInventory
	
	
        
*/

#ifndef _PDGSExtDataPreInventory_H_
#define _PDGSExtDataPreInventory_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <PreInventoryBase.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * PreInventory plugin for EOF(Earth Observation File) products
 **/

class PDGSExtDataPreInventory : public PreInventoryBase // PRQA S 2109
{ 
public:
		
	struct FileInfo // PRQA S 2173, 2175
    {
        FileInfo(): SatelliteId(), Mission(), ClassId(), File_Type(), StartDate(), StopDate(), File_Version(), GeneratingCentre(), 
							CreationDate(), StartOrbitNumber(), StopOrbitNumber(), geoCoords(), additionalInfo()
        {}
		FileInfo(const FileInfo & ); 
		FileInfo &operator=(const FileInfo &); 
		~FileInfo() throw() {}

        // PRQA S 2100 L1
		int SatelliteId;
        int Mission;
        std::string ClassId;				
        std::string File_Type;
        std::string StartDate;
        std::string StopDate;
        std::string File_Version;
        std::string GeneratingCentre;
        std::string CreationDate;
		std::string StartOrbitNumber;
		std::string StopOrbitNumber;
		std::vector<std::string> geoCoords; // latFirst, lonFirst, latLast, lonLast
		std::map<std::string, std::string> additionalInfo;
        // PRQA L:L1                                
    };

	/** PDGSExtDataPreInventory Exceptions */
	exDECLARE_EXCEPTION(exPDGSExtDataPreInventoryException, exException) ; // Base PDGSExtDataPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exPDGSExtDataPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	PDGSExtDataPreInventory();
	/** Destructor */
	virtual ~PDGSExtDataPreInventory() throw();

	virtual std::string getClassId() = 0;
	
	void setConfigurationRootTag(const std::string & s) { _configurationRootTag = s;  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _configurationRootTag);}
	std::string getConfigurationRootTag() const { return _configurationRootTag; } 
		
	const std::string & getCurrentFileName() const throw() { return _currentFileName;}
	const std::string & getCurrentFileType() const throw() { return _currentFileType;}
	const std::string & getFilenameId()      const throw() { return _filenameId ;} // string identifying the filename for name checks


	void  setCurrentFileType(const std::string &s) throw() { _currentFileType = s ;}
    void  setCurrentFileName(const std::string &s) throw() { _currentFileName = s ;}
    void  setFilenameId     (const std::string &s) throw() { _filenameId = s ;}


	
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
	 * \brief Use fillMetadataFilename base class method to fill correctly the metadata filename.
	 * \param File path
	 * \param File type
	 * \param File info as filled by prepareMetadata method
	 * \param Metadata
	 * \param dbConnection
	 * \retval The metadata filename
	 */    	
	virtual std::string fillMetadataFilenamePED( const std::string&,  const std::string&, const FileInfo &, Metadata &, dbConnection &);
	

	static std::string getMaxStopValidityTime() ; 

protected:    

    virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata);

	/** \brief 
	 * \brief  doVersioning returned value is true/false depending on if versioning must be performed on the current product type 
     */
	virtual bool overrideVersionHandling(bool& doVersioning);
			
    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &);


	 /**
	 * \brief  Package the Product (eg. attaching header to bin, create a tar, etc., or just do nothing).
	 * \param Metadata obj
	 * \param File full path corresponding either to header or single file depending on the next parameter is valorized
	 * \param File type
	 * \param File full path corresponding to data only if the previous parameter is valorized (otherwise it must be null)
	 * \retval Package file path
	 */    	
    virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection & );
	Metadata prepareMetadata( const std::string& fileType, const FileInfo &,  dbConnection &);
private:
	//extracts information from file
	virtual void extractFileInfo( const std::string &, const std::string&, dbConnection &, FileInfo & ) = 0;

	

	std::string getUniqueId();
	std::string getParametersFile();


	
	void setFtypeParentTag(const std::string &, const rsResourceSet &);

	PDGSExtDataPreInventory(const PDGSExtDataPreInventory & ); // not implemented
	PDGSExtDataPreInventory &operator=(const PDGSExtDataPreInventory &); // not implemented

private:
	
	std::string _currentFileName;
	std::string _currentFileType;
	std::string _configurationRootTag;
	std::string _ftypeParentTag;
	std::string _filenameId; // string identifying the filename for name checks
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PDGSExtDataPreInventory)

};

_ACS_END_NAMESPACE

#endif //_PDGSExtDataPreInventory_H_

