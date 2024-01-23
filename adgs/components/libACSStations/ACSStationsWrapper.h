// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SgiIncludes Library - ACS_CPP_station_conf.h $

	$Id$

	$Author$

	$Log$
	Revision 1.8  2014/02/13 15:04:54  giucas
	New log macros adopted.
	
	Revision 1.7  2012/03/01 16:43:19  marpas
	using acs::base for default static paths
	
	Revision 1.6  2009/07/22 10:17:38  marpas
	c++ rules applied
	
	Revision 1.5  2008/11/17 15:16:40  giucas
	Static constant added
	
	Revision 1.4  2008/05/23 07:23:02  giucas
	Id added to StationInfo inner class
	
	Revision 1.3  2008/05/23 07:13:56  giucas
	getInfo method added
	
	Revision 1.2  2008/05/22 16:36:08  giucas
	New methods added
	
	Revision 1.1.1.1  2008/05/22 12:34:01  giucas
	Moved ACSStationsWrapper and python wrapper from libSgiIncludes
	
	Revision 1.3  2008/04/24 09:39:15  giucas
	StationInfo class extended and made public
	
	Revision 1.2  2007/02/05 11:41:01  giucas
	Copy ctor and operator= added
	
	Revision 1.1  2006/11/14 16:28:53  giucas
	Added CPP Wrapper class for acsStations.h include file
	
	

*/


#ifndef _ACSStationsWrapper_H_
#define _ACSStationsWrapper_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <rsResourceSet.h>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief This class wraps the SitesConf.conf configuration file, exporting some utility methods to retrieve
 * 	      information about Stations
  */
class ACSStationsWrapper // PRQA S 2109
{
public:


/* Example of a Station informaiton in XML format
<Station>
	<Name>Fucino</Name>
	<Id>1</Id>
	<Country>Italy</Country>
	<CentreID>FUI</CentreID>
	<ShortName>FS</ShortName>
	<Latitude unit="deg">41.9782</Latitude>
	<Longitude unit="deg">13.6036</Longitude>
	<Altitude unit="m">650.8</Altitude>
	<HorizonElevation unit="deg"> </HorizonElevation>
	<AgencySiteIdentifier>Fucino</AgencySiteIdentifier>
	<AgencyOrganizationId></AgencyOrganizationId>
</Station>
*/

	/**
	 * \brief Container class for station information
	  */
	class StationInfo // PRQA S 2109
	{
		public:
			StationInfo() ;
			~StationInfo() ;
			StationInfo(const StationInfo&) ;
			StationInfo& operator=(const StationInfo&);
			void dump() const ;
		public:
			int				id ;				//<Id> // PRQA S 2100 11
			std::string		name ;				//<Name>
			std::string		country ;			//<Country>
			std::string		centreId ;			//<CentreID>
			std::string		shortName ;			//<ShortName>
			double 			latitude ;			//<Latitude unit="deg">
			double 			longitude;			//<Longitude unit="deg">
			double 			altitude ;			//<Altitude unit="m">
			double 			orizonElevation ;	//<HorizonElevation unit="deg">
			std::string		agencySiteId ;		//<AgencySiteIdentifier>
			std::string		agencyOrgId ;		//<AgencyOrganizationId>
	} ;

	/**
	 * \brief Class constructor
	  */
	explicit ACSStationsWrapper(const std::string &confFile=std::string(base::defaultConfPath()) + "/local") ; // PRQA S 3081

	/**
	 * \brief Class destructor
	  */
	virtual ~ACSStationsWrapper() throw();

	/**
	 * \brief Given a StationId (i.e 1 (Fucino) , 2 (Kiruna), etc...), return the Station name as string
	 * \param id The Station ID
	 * \retval The station Name
	 */
	const std::string &getName(int id) const ;

	/**
	 * \brief Given a StationId (i.e 1 (Fucino) , 2 (Kiruna), etc...), return the Station name SHORT as string
	 * \param id The Station ID
	 * \retval The station Name SHORT
	 */
	const std::string &getNameShort(int id) const ;

	/**
	 * \brief Given a StationName (i.e "Fucino", "Kiruna", etc...), return the Station Id
	 * \param name The Station name
	 * \retval The station Id
	 */
	int getIdFromName(const std::string& name) const ;

	/**
	 * \brief Given a Station Name SHORT (i.e "FS", "KS", etc...), return the Station Id
	 * \param name The Station SHORT name
	 * \retval The station Id
	 */
	int getIdFromShortName(const std::string& name) const ;

	/**
	 * \brief Given a StationName (i.e "Fucino", "Kiruna", etc...), return the station Latitude, Longitude and Altitude
	 * \param name The Station name
	 * \param lat The OUTPUT Station latitude
	 * \param lon The OUTPUT Station longitude
	 * \param alt The OUTPUT Station altitude
	 */
	void getInfoFromName(const std::string& name, double& lat, double& lon, double& alt) const ;

	/**
	 * \brief Given a StationName (i.e "Fucino", "Kiruna", etc...), return the ACSStationsWrapper::StationInfo object
	 * \param name The Station name
	 * \param info The OUTPUT ACSStationsWrapper::StationInfo
	 * \param lon The OUTPUT Station longitude
	 * \param alt The OUTPUT Station altitude
	 */
	void getInfoFromName(const std::string& name, ACSStationsWrapper::StationInfo& info) const ;


	/**
	 * \brief Given a Station Name SHORT (i.e "FS", "KS", etc...), return the station Latitude, Longitude and Altitude
	 * \param name The Station name
	 * \param lat The OUTPUT Station latitude
	 * \param lon The OUTPUT Station longitude
	 * \param alt The OUTPUT Station altitude
	 */
	void getInfoFromShortName(const std::string& name, double& lat, double& lon, double& alt) const ;

	/**
	 * \brief Given a Station Id (i.e 1 (Fucino) , 2 (Kiruna), etc...), return the station Latitude, Longitude and Altitude
	 * \param id The Station Id
	 * \param lat The OUTPUT Station latitude
	 * \param lon The OUTPUT Station longitude
	 * \param alt The OUTPUT Station altitude
	 */
	void getInfoFromId(int id, double& lat, double& lon, double& alt) const ;

	/**
	 * \brief Given a Station Id (i.e 1 (Fucino) , 2 (Kiruna), etc...), return the ACSStationsWrapper::StationInfo object
	 * \param id The Station Id
	 * \param info The OUTPUT ACSStationsWrapper::StationInfo
	 */
	void getInfoFromId(int id, ACSStationsWrapper::StationInfo& info) const ;

	/**
	 * \brief Returns the number of configured stations
	 * \retval The number of stations
	 */
	size_t getStationsAvailable() const ;

	/**
	 * \brief Returns as rsResourceSet the content of SitesConf.conf configuration file
	 * \param rs The OUTPUT rsResourceSet merged with the content of SitesConf.conf configuration file
	 */
	void getResources(rsResourceSet& rs) const ;

	/**
	 * \brief Return a vector containing all the station information
	 * \param info The OUTPUT ACSStationsWrapper::StationInfo vector
	 */
	void getInfo(std::vector<ACSStationsWrapper::StationInfo>& info) const ;

public:
	ACSStationsWrapper(const ACSStationsWrapper&);
	ACSStationsWrapper& operator=(const ACSStationsWrapper&);


public:
	static const std::string SITES_CONFIG_FILE_NAME ; // PRQA S 2100

private:

	int													_stationsAvailables ;
	std::map<int, std::string>    						_stationNames ;
	std::map<int, std::string>    						_stationNamesShort ;
	std::map<int, ACSStationsWrapper::StationInfo >		_stationInfo;
	rsResourceSet										_sitesConfRs ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACSStationsWrapper) ;

} ;

_ACS_END_NAMESPACE

#endif // _ACSStationsWrapper_H_
