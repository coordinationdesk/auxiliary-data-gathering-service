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

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 5.6  2014/06/27 16:53:13  marpas
	qa warning
	
	Revision 5.5  2014/06/19 11:21:21  marpas
	convenience class to group optional metadata names added (MetadataNameValues)
	
	Revision 5.4  2014/06/19 10:09:08  marpas
	NameValue class added to Metadata
	They will be written (if present) in an optional section and retrieved when the MTD is read.
	class metadata returns the vector of NameValue read.
	
	Revision 5.3  2014/06/05 11:37:36  marpas
	qa warning fixed
	
	Revision 5.2  2014/05/08 10:27:10  lucio.pulvirenti
	S2PDGS-664: Added methods to set/get geographic coords precision: if set GeoCoords_Precision tag is inserted into MTD file.
	
	Revision 5.1  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.0  2013/06/20 07:48:55  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.24  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.23  2013/02/20 17:10:00  marpas
	template method to substitute overloaded ones
	
	Revision 2.22  2012/06/11 13:16:03  marpas
	template method signature improved
	
	Revision 2.21  2012/06/11 13:07:58  marpas
	template method definition fixed
	
	Revision 2.20  2012/03/06 15:54:04  marpas
	messages improved and standardized
	
	Revision 2.19  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.18  2011/08/08 12:50:23  marfav
	Added configurable setting for mandatoryFlag used when writing on file
	
	Revision 2.17  2011/05/30 13:57:04  chicas
	added getGlobalFileName
	
	Revision 2.16  2011/04/19 16:15:18  marpas
	interface improved,
	useless messages removed
	
	Revision 2.15  2011/04/18 13:11:57  chicas
	Dir inventory added
	
	Revision 2.14  2011/04/13 13:09:10  marpas
	work in progress
	
	Revision 2.13  2011/04/05 13:22:05  chicas
	Added tree files (directory) management in Metadata classgvim ../libFileInventory.old/FileInventory.C
	
	Revision 2.12  2010/04/23 16:46:47  marpas
	cloud percentage and product specification management added.
	Test (testMetadata) now outputs a <file>.labels with the mandatory flag (to be assumed the tag is mandatory, its value cannot)
	
	Revision 2.11  2009/01/29 09:42:18  marpas
	ource field no longer mandatory
	
	Revision 2.10  2009/01/28 16:19:03  marpas
	useless include removed
	
	Revision 2.9  2009/01/19 11:44:22  marfav
	Optional field management improved
	
	Revision 2.8  2008/11/10 17:43:27  marpas
	Comments added
	
	Revision 2.7  2008/11/07 17:59:45  marpas
	Band, Tdi and Sites introduction. Partially tested...
	
	Revision 2.6  2008/11/06 14:12:00  marpas
	work in progress
	
	Revision 2.5  2008/08/27 16:50:02  giucas
	Mission parameter added to default 1
	
	Revision 2.4  2007/03/20 16:35:00  valspa
	VDS: added in Metadata class the management of MDS sections
	
	Revision 2.3  2006/10/23 10:06:45  marpas
	variables and/or methods related to file size, header size and blob sized now take into account 64 bits
	
	Revision 2.2  2006/10/12 08:51:51  enrcar
	setMetadataValue for HUGE files:  unsigned long long int supported
	
	Revision 2.1  2006/04/21 15:41:09  giucas
	Added handling of Inventory GAPs
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.16  2005/10/18 11:54:26  giucas
	Added overload for writeMetadataFile and operator>>
	
	Revision 1.15  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.14  2004/11/08 16:15:27  giucas
	Removed method that uses local files to initialize library
	


*/

#ifndef _Metadata_H_
#define _Metadata_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <dbGeoObject.h>
#include <Filterables.h>
#include <string>
#include <map>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class AsciiTime ;
//class XMLOstream;

class Metadata // PRQA S 2109
{

public:

	/*! class MetadataException declaration */
	exDECLARE_EXCEPTION(MetadataException, exException) ; // PRQA S 2131, 2502

    struct NameValue { // PRQA S 2173, 2175
        NameValue() : p_("", "") {}
        NameValue(const std::string &name, const std::string &value) : p_(name, value) {}
		NameValue(const NameValue & r) : p_(r.p_) {}
		NameValue &operator=(const NameValue &r ) {
            if (this != &r) {
                p_ = r.p_ ;
            }
            return *this ;
        }
		~NameValue() throw() {}
        const std::string &name() const throw() { return p_.first ; }
        const std::string &value() const throw() { return p_.second ; }
        
        std::pair<std::string, std::string> p_ ; // PRQA S 2100
    } ;

	struct GeoPointInfo // PRQA S 2173, 2175
	{
		GeoPointInfo() : latitude(), longitude() {} 
		GeoPointInfo(const GeoPointInfo & ) ;
		GeoPointInfo &operator=(const GeoPointInfo & ); 
		~GeoPointInfo() throw() ;

		std::string latitude;   // PRQA S 2100 2 
		std::string longitude;
	};

	struct InventoryGap // PRQA S 2173, 2175
	{
		InventoryGap():  startTime(), stopTime() {} 
		InventoryGap(const InventoryGap&);
		InventoryGap& operator=(const InventoryGap&);
		~InventoryGap() throw() ;

		/* Gap start time expressed as UTC=YYYY-MM-DDTHH:NN:SS.UUUUUU */
		std::string startTime;  // PRQA S 2100 
		/* Gap stop time expressed as UTC=YYYY-MM-DDTHH:NN:SS.UUUUUU */
		std::string stopTime;   // PRQA S 2100 
	};

	struct InventoryComment // PRQA S 2173, 2175
	{
		InventoryComment() : text(), date(),author() {} 
		InventoryComment(const InventoryComment&);
		InventoryComment& operator=(const InventoryComment&);
		~InventoryComment() throw() ;

		std::string text;   // PRQA S 2100 
		/* comment datetime expressed as UTC=YYYY-MM-DDTHH:NN:SS.UUUUUU */
		std::string date;   // PRQA S 2100 
		std::string author ;// PRQA S 2100 
	};

	/*********************************************************************************/
	/***** Class and methods dealing with Measurement Data Sets	******************/
	/*********************************************************************************/
	struct MeasDataSetDescriptor // PRQA S 2173, 2175
	{
		MeasDataSetDescriptor() : Ds_Name(), Ds_Type(), Num_Dsr() {} 
		MeasDataSetDescriptor(const MeasDataSetDescriptor&);
		MeasDataSetDescriptor& operator=(const MeasDataSetDescriptor&);
		~MeasDataSetDescriptor() throw() {}

		/* Name of DS as extracted from the product */
		std::string Ds_Name;   // PRQA S 2100 
		/* Type of DS as extracted from the product */
		std::string Ds_Type;   // PRQA S 2100 
		/* Num of records of DS as extracted from the product */
		std::string Num_Dsr;   // PRQA S 2100 
	};

	/**
	 * \brief  Set the Metadata list of Measurement Data Set
	 * \param Vector of InventoryGap objects
	 */
	void setMDSs(const std::vector<MeasDataSetDescriptor>&);

	/**
	 * \brief  Set the Metadata list of sites
	 * \param Vector of site's identifiers
	 */
	void setSites(const std::vector<int>&);


	/**
	 * \brief  Get the list of Measurement Data Set associated to this Metadata object.
	 * \retval A vector of InventoryGap objects.
	 */
	 void getMDSs(std::vector<MeasDataSetDescriptor> &) const; 


	/**
	 * \brief  Get the list of sites identifiers associated to this Metadata object.
	 * \retval A vector of sites identifiers.
	 */
	 void getSites(std::vector<int> &) const; 


	/*********************************************************************************/
	/***** End of Move Measurement Data Sets		**************************/
	/*********************************************************************************/

	/**
	 * \brief  Class constructor 
	 *
	 */	
	Metadata();

	/**
	 * \brief  Class destructor   
	 */ 
	~Metadata() throw() ;


	/**
	 * \brief  Class copy constructor   
	 */ 
	Metadata(const Metadata & ); 

	/**
	 * \brief Operator=   
	 */
	Metadata &operator=(const Metadata &);


	//methods

	/**
	 * \brief  Clean-up of the object
	 */
	void clean();
	
    /**
	 * \brief  Set a specific tag name with a string value.
	 * \param tag name
	 * \param tag value
	 */
    template<typename T>
	void setMetadataValue(const std::string & name, const T & value) // PRQA S 4211
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setting Metadata field " << name << " with value " << value) ;
	    std::ostringstream os;
	    os << value;

	    _metadataInfo[name] = os.str() ;
    }


	/**
	 * \brief  Set the Metadata list of Geo Points
	 * \param Vector of GeoPointInfo objects
	 */
	void setGeoPoints(const std::vector<GeoPointInfo>&);

	/**
	 * \brief  Set the Metadata list of Geo Points
	 * \param Start Latitude
	 * \param Start Longitude
	 * \param Stop Latitude
	 * \param Stop Longitude
	 */
	void setStartStopGeoPoints(const std::string&,const std::string&,const std::string&,const std::string&);

	/**
	 * \brief  Set the Metadata list of Inventory Gaps
	 * \param Vector of InventoryGap objects
	 */
	void setGaps(const std::vector<InventoryGap>&); 


	/**
	 * \brief  Get the list of InventoryGap associated to this Metadata object.
	 * \retval A vector of InventoryGap objects.
	 */
	void getGaps( std::vector<InventoryGap> &) const; 


	void setComments(const std::vector<InventoryComment>&); 

	void getComments(std::vector<InventoryComment> &) const; 


	std::string getGlobalFileName() const;

	/**
	 * \brief  Get the value associated to a specific tag.
	 * 		   If the tag name doesn't exist 
	 * 			an exception is thrown if the raiseExc is true, else an empty string
	 * \param  tag name
	 * \param  raiseExc signal whether to raise an exception or not
	 * \retval tag value as string
	 */
	std::string getMetadataValue(const std::string&, bool raiseExc=true) const;

	/**
	 * \brief  Get the value associated to a specific tag.
	 * <b>WARNING: Bool type not supported!</b>
	 *		   If the tag name doesn't exist a MetadataException is thrown.
	 */
	template<typename T> const T& getFieldValue(const std::string& name, T& ) const ;

	/**
	 * \brief  Set the value associated to a specific tag.
	 *		   If the tag name doesn't exist a MetadataException is thrown.
	 */
	template<typename T> void setFieldValue(const std::string& name, const T& t);


	/**
	 * \brief  Get the list of Geo Points associated to this Metadata object.
	 * \retval A vector of GeoPointInfo structs.
	 */
	void getGeoPoints(std::vector<GeoPointInfo> &) const;  

	/**
	 * \brief  Get the Geo Point number specified by argument associated to this Metadata object.
	 * \retval A GeoPointInfo struct.
	 */
	GeoPointInfo& getGeoPoint(unsigned int);


	/**
	 * \brief  Write on disk the Metadata file in XML format.
	 * \param  metadatafile The output file name
	 */
	void writeMetadataFile(const std::string&);

	/**
	 * \brief  Write on ostringstream the Metadata in XML format.
	 * \param  o The input ostringstream
	 */
	void writeMetadataFile(std::ostringstream& );

	/**
	 * \brief  Read from disk the Metadata file in XML format.
	 * \param string the file name
	 */
	void readMetadataFile(const std::string&);

	/**
	 * \brief  Read from the resourceSet.
	 * \param the rsResourceSet
	 */
	void readMetadata(rsResourceSet &) ;
	/**
	 * \brief Compute the internale geo-points localization iterating from start to stop by the requested step.
	 *		  The FOS Predicted file is retrieved from database querying by start and stop times. 
	 * \param Start time in TC_ASCII_CCSDSA_REF format
	 * \param Stop time in TC_ASCII_CCSDSA_REF format
	 * \param iteration step expressed in days (MJD2000)
	 * \param satellite name
	 * \param mission default is 1
	 */
	void computeGeoPoints(const AsciiTime&, const AsciiTime&, const double&, const std::string&, int mission=1);

	/**
	 * \brief  Set the Metadata Geo Object type
	 * \param dbGeoObject::Type
	 */
	void setGeoType(const dbGeoObject::Type& type);

	/**
	 * \brief  Get the Metadata Geo Object type
	 * \retval A string 
	 */
	const std::string& getGeoType() const;

	void outMandatoryLabels(bool) ;
	
	/**
	 * \brief  Set the Metadata Geo coords precision
	 * \param precision
	 */
	void setGeoCoordsPrecision(int);

	/**
	 * \brief  Get the Metadata Geo Object type
	 * \retval An int 
	 */
	const int& getGeoCoordsPrecision() const;

    void addNameValue(const std::string &n, const std::string &v) ;
    const std::vector<NameValue> &getNameValues() const throw() { return _nameValues ; }

	static const std::string	_mtdkey_Session_Id ;
	static const std::string	_mtdkey_Granule_Number ;
	static const std::string	_mtdkey_Granule_Position ;
	static const std::string	_mtdkey_Dump_Start ;
	static const std::string	_mtdkey_Mission_Data_Take_Id ;
	static const std::string	_mtdkey_Next_Equator_X_Time ;

private:

	void writeMetadata(std::ostream&) ;
	void writeMetadataBlock( XMLOstream &) ;
	void readMetadataBlock(rsResourceSet &) ;
	std::string extractValue(rsResourceSet&, const std::string&, bool mandatory) ;

	template<typename T> static T fromString(const std::string& s) ;
	template<typename T> static std::string toString(const T& t); // PRQA S 2502

	std::string packXMLString(const std::string &);
    void writeNameValue(XMLOstream &) ;

private:
	static const std::string _defaultPath;
	static const char* _satelliteMandatoryKey;

	//attributes
	//std::map<std::string,std::string>	_metadataInfo;

	//metadata map typedef (key-value)
	typedef std::map<std::string,std::string> MetaDataInfo;

	//a map of values for each given file	
	MetaDataInfo	_metadataInfo;

	std::vector<GeoPointInfo>	_geoPoints;
	std::vector<InventoryGap>	_inventoryGaps;
	std::vector<InventoryComment> _inventoryComments;
	std::vector<int>		_sites;
	// Measurement Data Sets vector data
	std::vector<MeasDataSetDescriptor> _mdsData;

	bool _outLabels ;

	std::string				_geoType;
	GeoPointInfo			_startGeoPoint;
	GeoPointInfo			_stopGeoPoint;
	
	int						_geoCoordsPrecision;
    std::vector<NameValue>  _nameValues ;
	
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Metadata) ;

};
	
/**
 *  \brief Fill a Metadata object with the content of ostringstream object representing an XML Metadata file
 */	
Metadata& operator<<(Metadata& , const std::ostringstream& ); // PRQA S 2072



template<typename T> inline
const T& Metadata::getFieldValue(const std::string& name, T& t) const // PRQA S 4214
{
	std::map<std::string,std::string>::const_iterator it = _metadataInfo.find(name);

	if(it == _metadataInfo.end())
	{
		ACS_THROW(MetadataException("Error : Metadata tag name " + name + " not found!!!")) ; // PRQA S 3081
	}
	t = fromString<T>((*it).second);
    return t ; // PRQA S 4028
}

template<> inline
const DateTime & Metadata::getFieldValue(const std::string& name, DateTime& t) const // PRQA S 2133, 2134, 4214
{
	std::map<std::string,std::string>::const_iterator it = _metadataInfo.find(name);

	if(it == _metadataInfo.end())
	{
		ACS_THROW(MetadataException("Error : Metadata tag name " + name + " not found!!!")) ; // PRQA S 3081
	}
	t = DateTime((*it).second); // PRQA S 3081
    return t ;                  // PRQA S 4028
}

template<typename T> inline
void Metadata::setFieldValue(const std::string& name, const T& t) // PRQA S 4211
{
	_metadataInfo[name] = toString(t);
}



template<typename T> inline
T Metadata::fromString(const std::string& s) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "s: \"" << s << "\"") ;
	std::istringstream is(s);
	T t = T();
	is >> t;
	return t;
}

template<typename T> inline
std::string Metadata::toString(const T& t) 
{
  std::ostringstream s;
  s << std::setprecision(17) << std::fixed << t; // PRQA S 4400
  return s.str();
}

template<> inline
void rsResourceSet::getValue(const std::string &res, Metadata::NameValue &v) const // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
    std::string name ;
	getValue(res+".Name")->get(name) ;
    std::string value ;
	getValue(res+".Value")->get(value) ;
	v = Metadata::NameValue(name, value) ;
} 

XMLOstream & operator<< (XMLOstream &, const Metadata::NameValue &) ; // PRQA S 2072

class MetadataNameValues {
public:
    static std::string getVersioning() { return "Versioning" ; }
private:
    MetadataNameValues() ;
} ;

_ACS_END_NAMESPACE

#endif //_Metadata_H_
