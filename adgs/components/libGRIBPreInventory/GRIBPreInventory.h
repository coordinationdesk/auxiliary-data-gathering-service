// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libGRIBPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.14  2015/09/10 15:35:14  damdec
	ADC-315: System field set from configuration (not mandatory)
	
	Revision 1.13  2015/08/26 15:19:47  marfav
	ADC-316 added configuration for start validity offset
	
	Revision 1.12  2014/09/08 10:25:42  lucio.pulvirenti
	S1PDGS-31532: experimentVersionNumber key value no used: GRIBStruct ExperimVerNumber element removed.
	
	Revision 1.11  2014/06/27 16:54:30  marpas
	coding best practices application in progress
	
	Revision 1.10  2013/10/21 14:57:03  lucio.pulvirenti
	getFormattedVersion method removed.
	overrideVersionHandling method added.
	_currentFiletype attr added.
	
	Revision 1.9  2013/09/02 15:41:14  lucio.pulvirenti
	ADC-185
	getSatelliteIdFromSatelliteName method replaced by getSatelliteIdFromSatelliteCode.
	getSatMissionAssociatorFactoryKey method and static method createSatellitesIdMap added.
	static attributes added: satellite map and _defaultSatMissionAssociatorFactoryKey.
	
	Revision 1.8  2013/05/30 09:02:59  lucio.pulvirenti
	getFtypeFromFramework private method added.
	
	Revision 1.7  2013/05/29 15:44:28  lucio.pulvirenti
	getFtypeFromFilename private method added.
	
	Revision 1.6  2013/04/29 16:29:10  marpas
	base class interface changed
	
	Revision 1.5  2013/02/21 16:45:27  lucio.pulvirenti
	Added dbConnection parameter to prepareMetadata,getSatelliteIdFromSatelliteName,computeMetadata methods.
	
	Revision 1.4  2012/12/07 17:10:25  lucio.pulvirenti
	doPackaging method IF changed in the base class.
	
	Revision 1.3  2012/10/23 08:55:50  giufor
	updated prepareMetadata method
	
	Revision 1.2  2012/10/22 15:19:23  giufor
	implemented readGRIBFile method
	implemented computeMetadata method
	
	Revision 1.1.1.1  2012/10/18 15:07:33  giufor
	Import libGRIBPreInventory
	
        
*/

#ifndef _GRIBPreInventory_H_
#define _GRIBPreInventory_H_

#include <exException.h>
#include <PreInventoryBase.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* PreInventory plugin for GRIB products
**/

class GRIBPreInventory : public PreInventoryBase // PRQA S 2109
{ 

public:

	/** GRIBPreInventory Exceptions */
	exDECLARE_EXCEPTION(exGRIBPreInventoryException, exException) ; // PRQA S 2131, 2502


	/** Default Class constructor */
	GRIBPreInventory();
	/** Destructor */
	virtual ~GRIBPreInventory() throw();

	virtual std::string getClassId();

#ifndef ACS_TEST // white box testing
protected:
#else
public:
#endif

    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &);
    virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata);
 	/**
	 * \brief  Do nothing.
	 * \param Metadata obj
	 * \param File full path on disk of file to inventory
	 * \param File type
	 * \param Empty string; if valorized exception is thrown
	 * \retval The full path on disk to inventory
	 */    	
    virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection & );


private:
	 // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	GRIBPreInventory(const GRIBPreInventory & );
	/** Operator = */
	GRIBPreInventory &operator=(const GRIBPreInventory &);
#ifndef ACS_TEST // white box testing
private:
#else
public:
#endif

	struct GRIBStruct // PRQA S 2173, 2175
    {
        GRIBStruct() : 
            Centre(), 
            GenProcessIdentifier(), 
            DataDate(), 
            DataTime(), 
            LatOfFirst(), 
            LonOfFirst(), 
            LatOfLast(), 
            LonOfLast() {}

        std::string Centre; // PRQA S 2100 9
        std::string GenProcessIdentifier;
		std::string DataDate;
		std::string DataTime;
		std::string LatOfFirst;
		std::string LonOfFirst;
		std::string LatOfLast;
		std::string LonOfLast;
    };


	//extracts information from GRIB file 
	void readGRIBFile( const std::string& pathFileName, GRIBStruct& gribs);



	std::string getUniqueId();
	//std::string getParametersFile();

	Metadata prepareMetadata( const std::string& fileType, const GRIBStruct& gribs, dbConnection &);

	/** \brief It returns true.
	 * \brief  doVersioning returned value is true/false depending on if versioning must be performed on the current product type 
     */
	virtual bool overrideVersionHandling(bool& doVersioning);

	int getSatelliteIdFromSatelliteCode( const std::string& satCode, dbConnection &);

	// using double values to allow parts of hours
	// returned values are in hours
	double getDeltaValidity();
	// Get Generating Centre from configuration (not mandatory)
	std::string getGeneratingCentre();
	double getStartOffest();

	std::string getFtypeFromFilename(const std::string &, unsigned short) const;
	bool getFtypeFromFramework(const std::string &) const;

	/**
	 * \brief Get SatelliteMissionAssociator factory key from configuration
	 * \param The satellite code
	 * \retval The factory key
	 */    	
	std::string getSatMissionAssociatorFactoryKey(const std::string &);

	static std::map<std::string, std::string> createSatellitesIdMap()
    {
      std::map<std::string, std::string>  m;

	  m["SENTINEL-1"]  = "S1_";
	  m["SENTINEL-1A"] = "S1A";
	  m["SENTINEL-1B"] = "S1B";
	  m["SENTINEL-2"]  = "S2_";
	  m["SENTINEL-2A"] = "S2A";
	  m["SENTINEL-2B"] = "S2B";
	  m["SENTINEL-3"]  = "S3_";
	  m["SENTINEL-3A"] = "S3A";
	  m["SENTINEL-3B"] = "S3B";
	  m["SENTINEL-3C"] = "S3C";
	  m["SENTINEL-3D"] = "S3D";
      return m;
    }

private:
	std::string _currentFileName;
	std::string _currentFileType;

	static const std::map< std::string, std::string > _satellitesIdMap ;
	static const std::string _defaultSatMissionAssociatorFactoryKey;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(GRIBPreInventory);

};

_ACS_END_NAMESPACE

#endif //_GRIBPreInventory_H_

