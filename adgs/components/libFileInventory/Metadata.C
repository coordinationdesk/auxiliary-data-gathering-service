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

	$Prod: 

	$Id$

	$Author$

	$Log$
	Revision 5.9  2017/07/13 15:30:43  lucio.pulvirenti
	ECICMF-108: InventoryBaseline field managed.
	
	Revision 5.8  2017/05/12 10:09:18  lucio.pulvirenti
	PRS-16: Relation and RelationType fields added.
	
	Revision 5.7  2016/03/14 12:18:48  marpas
	logging improved - converting a nominal message from warning to private info
	
	Revision 5.6  2015/05/25 13:45:05  marpas
	coding best practice applied
	
	Revision 5.5  2015/03/13 17:50:59  marfav
	S2PDGS-1137 Added support to bulk extra parameters and quality value
	
	Revision 5.4  2015/02/20 16:22:25  marfav
	APF-256 introducing baseline metadata
	
	Revision 5.3  2014/06/19 10:09:08  marpas
	NameValue class added to Metadata
	They will be written (if present) in an optional section and retrieved when the MTD is read.
	class metadata returns the vector of NameValue read.
	
	Revision 5.2  2014/05/08 10:27:10  lucio.pulvirenti
	S2PDGS-664: Added methods to set/get geographic coords precision: if set GeoCoords_Precision tag is inserted into MTD file.
	
	Revision 5.1  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.39  2013/05/09 13:38:24  marpas
	fixing getArraySize calls for points, sites, gaps and comments
	fixing test compilation (almost all)
	Some debug more
	
	Revision 2.38  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.37  2013/02/20 17:10:00  marpas
	template method to substitute overloaded ones
	
	Revision 2.36  2012/12/20 09:47:11  marpas
	INVENTORY_OPTIONAL_SATELLITE define added to allow optional satellite in metadata.
	
	Revision 2.35  2012/11/28 12:06:49  chicas
	Added PDGS mandatory flag settings
	
	Revision 2.34  2012/05/14 07:44:57  chicas
	Added Logical_Id field
	
	Revision 2.33  2012/04/02 08:07:21  chicas
	Minor changes
	
	Revision 2.32  2012/03/30 13:50:46  chicas
	Bug fixed in  writeMetadataBlock :  Unique_id handled as optional (no conditioned to hasSerialId)
	
	Revision 2.31  2012/03/14 09:53:15  marfav
	clean metod fixed
	
	Revision 2.30  2012/03/12 09:41:45  marpas
	uniqueid conditioned to dbInventoryObject::hasSerialField
	
	Revision 2.29  2012/03/06 15:54:04  marpas
	messages improved and standardized
	
	Revision 2.28  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.27  2011/08/30 10:24:27  lucia.galli
	Assignment operator now sets the current file path
	
	Revision 2.26  2011/08/08 12:50:23  marfav
	Added configurable setting for mandatoryFlag used when writing on file
	
	Revision 2.25  2011/05/30 13:57:03  chicas
	added getGlobalFileName
	
	Revision 2.24  2011/04/19 16:15:17  marpas
	interface improved,
	useless messages removed
	
	Revision 2.23  2011/04/18 13:11:57  chicas
	Dir inventory added
	
	Revision 2.22  2011/04/13 13:09:10  marpas
	work in progress
	
	Revision 2.21  2011/04/05 13:22:05  chicas
	Added tree files (directory) management in Metadata classgvim ../libFileInventory.old/FileInventory.C
	
	Revision 2.20  2010/05/25 12:47:38  ivafam
	Added ifdef for sectronic
	
	Revision 2.19  2010/04/23 16:46:47  marpas
	cloud percentage and product specification management added.
	Test (testMetadata) now outputs a <file>.labels with the mandatory flag (to be assumed the tag is mandatory, its value cannot)
	
	Revision 2.18  2009/03/06 11:55:48  marpas
	local headers included with double quotes
	
	Revision 2.17  2009/01/19 11:44:22  marfav
	Optional field management improved
	
	Revision 2.16  2009/01/15 13:49:15  manuel.maccaroni
	another PROJECT_FLAG, PROSA, is usend in compilation
	
	Revision 2.15  2009/01/15 10:12:14  manuel.maccaroni
	added Qualified metadata field
	
	Revision 2.14  2008/12/16 11:04:26  marpas
	PROJECTFLAG used in compilation (OCADA)
	
	Revision 2.13  2008/11/10 17:43:26  marpas
	Comments added
	
	Revision 2.12  2008/11/10 11:00:45  marpas
	debug improved
	
	Revision 2.11  2008/11/07 17:59:45  marpas
	Band, Tdi and Sites introduction. Partially tested...
	
	Revision 2.10  2008/11/07 17:26:13  marpas
	make-up and introduction of band and tdi
	
	Revision 2.9  2008/11/06 14:12:00  marpas
	work in progress
	
	Revision 2.8  2008/08/27 16:50:02  giucas
	Mission parameter added to default 1
	
	Revision 2.7  2007/03/20 16:35:00  valspa
	VDS: added in Metadata class the management of MDS sections
	
	Revision 2.6  2007/03/16 14:16:34  aleber
	field System handling comments improved
	
	Revision 2.5  2007/03/16 14:13:24  aleber
	field <System> of Metadata file is handled as optional
	
	Revision 2.4  2007/01/10 11:36:12  giucas
	Updated TimeConverter lib calls for IF changes (CFI 3.5 porting)
	
	Revision 2.3  2006/10/12 08:51:51  enrcar
	setMetadataValue for HUGE files:  unsigned long long int supported
	
	Revision 2.2  2006/04/21 15:41:09  giucas
	Added handling of Inventory GAPs
	
	Revision 2.1  2006/03/08 15:03:50  giucas
	string2Type called only if GeoType is not empty
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.15  2006/02/21 13:40:37  nicvac
	Geo_Type handled only by means the get/setGeoType() method. getMetadataValue access deprecated.
	
	Revision 1.14  2005/10/18 11:54:26  giucas
	Added overload for writeMetadataFile and operator>>
	
	Revision 1.13  2005/04/21 12:06:39  giucas
	Minor modifications
	
	Revision 1.12  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.11  2004/11/08 16:15:12  giucas
	Changed computeGeoPoinst method implementation to use GeoPointIterator class. Removed method that uses local files to initialize library
	

*/

#include "Metadata.h"
#include <XMLOstream.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <TimeConverterSingleton.h>
#include <XMLIstringstream.h>
#include <GeoPointIterator.h>
#include <dbInventoryObject.h>
#include <Filterables.h>
#include <auto_version_file>
#include <sstream>
#include <algorithm>
#include <sys/types.h> // PRQA S 1013
#include <unistd.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Metadata)

const char* Metadata::_satelliteMandatoryKey = ("Metadata.MandatoryFlag");
const std::string Metadata::_mtdkey_Session_Id 				= "Session_Id" ;
const std::string Metadata::_mtdkey_Granule_Number			= "Granule_Number" ;
const std::string Metadata::_mtdkey_Granule_Position		= "Granule_Position" ;
const std::string Metadata::_mtdkey_Dump_Start				= "Dump_Start" ;
const std::string Metadata::_mtdkey_Mission_Data_Take_Id	= "Mission_Data_Take_Id" ;
const std::string Metadata::_mtdkey_Next_Equator_X_Time		= "Next_Equator_X_Time" ;


/*
 * Class constructor
 */
Metadata::Metadata() :
    _metadataInfo(),
    _geoPoints(),
    _inventoryGaps(),
    _inventoryComments(),
    _sites(),
    _mdsData(),
	_outLabels(false),
	_geoType(""),
    _startGeoPoint(),
    _stopGeoPoint(),
	_geoCoordsPrecision(0),
    _nameValues()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id) ;

}

/*
 * Class destructor
 */
Metadata::~Metadata() throw() {}



//copy constructor
Metadata::Metadata(const Metadata & r) :
    _metadataInfo(r._metadataInfo),
    _geoPoints(r._geoPoints),
    _inventoryGaps(r._inventoryGaps),
    _inventoryComments(r._inventoryComments),
    _sites(r._sites),
    _mdsData(r._mdsData),
	_outLabels(r._outLabels),
	_geoType(r._geoType),
    _startGeoPoint(r._startGeoPoint),
    _stopGeoPoint(r._stopGeoPoint),
	_geoCoordsPrecision(r._geoCoordsPrecision),
    _nameValues(r._nameValues)

{
}

//operator = 
Metadata&  Metadata::operator=(const Metadata & e)
{
	if (this != &e) {
		_metadataInfo= e._metadataInfo;
		_geoPoints		= e._geoPoints;	
		_startGeoPoint	= e._startGeoPoint;
		_stopGeoPoint	= e._stopGeoPoint;
		_geoType        = e._geoType;
		_inventoryGaps	= e._inventoryGaps;
		_inventoryComments	= e._inventoryComments;
		_sites	= e._sites; 
		_outLabels = e._outLabels ;
		_geoCoordsPrecision = e._geoCoordsPrecision;
        _nameValues = e._nameValues ;
	}
	return *this ;
}


////////////////////////////////////////////////////////////////////
// GeoPointInfo struct
////////////////////////////////////////////////////////////////////


Metadata::GeoPointInfo::GeoPointInfo(const GeoPointInfo &e) :
	latitude(e.latitude),
	longitude(e.longitude)
{
	// empty
}

Metadata::GeoPointInfo::~GeoPointInfo() throw() {}

Metadata::GeoPointInfo& 
Metadata::GeoPointInfo::operator=(const GeoPointInfo &e) 
{
	if (this != &e) 
	{
		latitude 	= e.latitude ;
		longitude 	= e.longitude ;
	}
	return *this ;
}


////////////////////////////////////////////////////////////////////
// InventoryGap class
////////////////////////////////////////////////////////////////////

Metadata::InventoryGap::InventoryGap(const InventoryGap &e) :
	startTime(e.startTime),
	stopTime(e.stopTime)

{
    // empty
}


Metadata::InventoryGap::~InventoryGap() throw() {}

Metadata::InventoryGap& 
Metadata::InventoryGap::operator=(const InventoryGap &e) 
{
	if (this != &e) 
	{
		startTime 	= e.startTime ;
		stopTime 	= e.stopTime ;
	}
	return *this ;
}

////////////////////////////////////////////////////////////////////
// InventoryComment class
////////////////////////////////////////////////////////////////////

Metadata::InventoryComment::InventoryComment(const InventoryComment &e) :
	text(e.text),
	date(e.date),
	author(e.author)

{
	// empty 
}


Metadata::InventoryComment::~InventoryComment() throw() {}

Metadata::InventoryComment& 
Metadata::InventoryComment::operator=(const InventoryComment &e) 
{
	if (this != &e) 
	{
		text = e.text ;
		date = e.date ;
		author = e.author ;
	}
	return *this ;
}

/*********************************************************************************/
/***** Added by VDS			******************************************/
/*********************************************************************************/

////////////////////////////////////////////////////////////////////
// MeasDataSetDescriptor class
////////////////////////////////////////////////////////////////////

Metadata::MeasDataSetDescriptor::MeasDataSetDescriptor(const MeasDataSetDescriptor &e) :
	Ds_Name(e.Ds_Name),
	Ds_Type(e.Ds_Type),
	Num_Dsr(e.Num_Dsr)

{
    // empty
}

Metadata::MeasDataSetDescriptor& 
Metadata::MeasDataSetDescriptor::operator=(const MeasDataSetDescriptor &e) 
{
	if (this != &e) 
	{
		Ds_Name 	= e.Ds_Name ;
		Ds_Type 	= e.Ds_Type ;
		Num_Dsr 	= e.Num_Dsr ;
	}
	return *this ;
}

/*********************************************************************************/
/***** End of VDS addition			**********************************/
/*********************************************************************************/


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////
std::string Metadata::getGlobalFileName() const
{
    return getMetadataValue("File_Name", false) ;
}

string Metadata::getMetadataValue(const string& name, bool raiseExc) const // PRQA S 4214
{
	string result ;
	
	map<string,string>::const_iterator it = _metadataInfo.find(name);
	
	if (it == _metadataInfo.end())
	{
		ACS_COND_THROW(raiseExc ,MetadataException("Error : Metadata tag name " + name + " not found!!!")); // PRQA S 3081
	}
	else { result = (*it).second; }
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting Metadata field " 
			 << name
			 << " value is '"
			 << result << "'") ;
	return result;
}


Metadata::GeoPointInfo& Metadata::getGeoPoint(unsigned int num) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting Geo Point number " << num << "...") ;

	if ( num > _geoPoints.size() )
	{
		ostringstream msg;
		msg << "Metadata::getGeoPoint Number of Geo Points[" << _geoPoints.size()//_geoPoints.size()
			<< "] is less than the Geo Point asked[" << num << "]";
		ACS_THROW(exIllegalValueException(msg.str())) ; // PRQA S 3081
	}
	
	if(num == 0) {
		return _geoPoints[0]; // _geoPoints[0];
    }
	
	return _geoPoints[num-1];// _geoPoints[num-1]; // PRQA S 3084

	
}

void Metadata::writeMetadataFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing Metadata file " << fileName << "...") ;
	
	ofstream fileStream;
	fileStream.open(fileName.c_str());
	writeMetadata(fileStream) ;
	fileStream.close();	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata file " << fileName << " generated!!!") ;
}

string Metadata::packXMLString (const string &si)
{

  // remove all tabs changing them into '\n'
  string s = si ;
  replace (s.begin(), s.end() , '\t', '\n');
  // then remove all '\n' chars
  size_t pos = 0;
  while ((pos=s.find('\n')) != string::npos) { // PRQA S 4244
	  s.erase (pos,1);
  }
  return s;
}


void Metadata::writeMetadataFile(ostringstream& os)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing Metadata stringstream ") ;
	
	ostringstream osTemp;
	writeMetadata(osTemp) ;
	
	string packetString = packXMLString (osTemp.str());
	
	os << packetString;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata stream generated!!!") ;

}

void Metadata::outMandatoryLabels(bool v) // PRQA S 4121
{
	_outLabels = v ;
}

template<typename T>
struct first_t {
    string operator()( const pair<const string,T> & p )  { return     p.first; }
};


void Metadata::writeMetadata(ostream& os)
{
	//first write global data
    
	XMLOstream xmlOstr(os,"Inventory_Metadata");
	writeMetadataBlock(xmlOstr); //use current file path..(default)

    writeNameValue(xmlOstr) ;
}



XMLOstream & operator<< (XMLOstream &os, const Metadata::NameValue &v) // PRQA S 4222
{
    xml::NoNs noNameSpace(os);
    { xml::Tag ti (os, "Name", v.p_.first, "", true) ; }
    { xml::Tag ti (os, "Value", v.p_.second, "", true) ; }
    return os ; // PRQA S 4028
}

void Metadata::writeNameValue(XMLOstream &xos) // PRQA S 4020
{
    if (_nameValues.empty()) {
        return ;
    }
    xml::Tag ti (xos, "OptionalNameValues", "") ;
    size_t n_values = _nameValues.size() ;
    for (size_t i=0; i < n_values; ++i) {
        xml::Tag(xos, "NameValue", _nameValues[i]) ;
    }
}


void Metadata::writeMetadataBlock(	XMLOstream &xmlOstr)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing Metadata stream ") ;
	
	{
		string mandyes = _outLabels ? "mandatory=\"yes\"" : "" ; // PRQA S 3384 2
		string mandno = _outLabels ? "mandatory=\"no\"" : "" ;
		string key ;
	
		bool mandatoryFlag = true ;

#if defined (INVENTORY_OPTIONAL_SATELLITE) || defined(PDGS) || defined(OCADA) || defined (PROSA) || defined (SECTRONIC)
		mandatoryFlag = false ;
#endif
		rsResourceSet& confrs = ConfigurationSingleton::instance()->get();
		if (confrs.hasKey (_satelliteMandatoryKey))
		{
			confrs.getValue (_satelliteMandatoryKey, mandatoryFlag);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "found MandatoryFlag configuration [" << boolalpha << mandatoryFlag << "]") ;
		}
	
		//Note: global section has all tags ; files section have only tags of not empty fields.

		XMLOstream::Tag fileName(xmlOstr, "File_Name",  getMetadataValue("File_Name") ,mandyes,true);

		XMLOstream::Tag fileVersion(xmlOstr, "File_Version", getMetadataValue("File_Version") ,mandyes,true);

		// field Unique_Id is handled as optional
		XMLOstream::Tag uniqueId(xmlOstr, "Unique_Id", getMetadataValue("Unique_Id",false) ,mandno,true);

        // field Logical_Id is handled as optional
	    XMLOstream::Tag logicalId(xmlOstr, "Logical_Id", getMetadataValue("Logical_Id",false),mandno,true);
	
        // field Parent_Id is handled as optional
		XMLOstream::Tag parentId(xmlOstr, "Parent_Id", getMetadataValue("Parent_Id",false),mandno,true);

        // field Quality_Value is handled as optional
		XMLOstream::Tag qualityValue(xmlOstr, "Quality_Value", getMetadataValue("Quality_Value",false),mandno,true);

        // field Inventory_Extra is handled as optional
		XMLOstream::Tag inventoryExtra(xmlOstr, "Inventory_Extra", getMetadataValue("Inventory_Extra",false),mandno,true);

		// field Session_Id (for Inventory Additional Parameters) is handled as optional
		key = _mtdkey_Session_Id ; XMLOstream::Tag sessionId(xmlOstr, key, getMetadataValue(key,false) ,mandno,true);

		// field Granule_Number (for Inventory Additional Parameters) is handled as optional
		key = _mtdkey_Granule_Number ; XMLOstream::Tag granuleNumber(xmlOstr, key, getMetadataValue(key,false) ,mandno,true);

		// field Granule_Position (for Inventory Additional Parameters) is handled as optional
		key = _mtdkey_Granule_Position ; XMLOstream::Tag granulePosition(xmlOstr, key, getMetadataValue(key,false) ,mandno,true);

		// field Dump_Start (for Inventory Additional Parameters) is handled as optional
		key = _mtdkey_Dump_Start ; XMLOstream::Tag dumpStart(xmlOstr, key, getMetadataValue(key,false) ,mandno,true);

		// field Mission_Data_Take_Id (for Inventory Additional Parameters) is handled as optional
		key = _mtdkey_Mission_Data_Take_Id ; \
		XMLOstream::Tag missionDataTakeId(xmlOstr, key, getMetadataValue(key,false) ,mandno,true);

		// field Next_Equator_X_Time (for Inventory Additional Parameters) is handled as optional
		key = _mtdkey_Next_Equator_X_Time ; \
		XMLOstream::Tag nextEquatorXTime(xmlOstr, key, getMetadataValue(key,false) ,mandno,true);

        // field Group_Id is handled as optional
		XMLOstream::Tag groupId(xmlOstr, "Group_Id", getMetadataValue("Group_Id",false) ,mandno,true);

		// field System is handled as optional
		XMLOstream::Tag system(xmlOstr,"System",getMetadataValue("System", false),mandno,true);

		XMLOstream::Tag source(xmlOstr, "Source", getMetadataValue("Source",mandatoryFlag),mandyes,true);

		XMLOstream::Tag sourceSwVers(xmlOstr, "Source_Sw_Version",getMetadataValue("Source_Sw_Version",mandatoryFlag),mandyes,true);

		XMLOstream::Tag genTime(xmlOstr, "Generation_Time", getMetadataValue("Generation_Time") ,mandyes,true);

		XMLOstream::Tag valStart(xmlOstr, "Validity_Start",getMetadataValue("Validity_Start") ,mandyes,true);

		XMLOstream::Tag valStop(xmlOstr, "Validity_Stop",getMetadataValue("Validity_Stop") ,mandyes,true);

		XMLOstream::Tag startOrbNum(xmlOstr, "Start_Orbit_Number",getMetadataValue("Start_Orbit_Number", false) ,mandyes,true);

		XMLOstream::Tag stopOrbNum(xmlOstr, "Stop_Orbit_Number", getMetadataValue("Stop_Orbit_Number",false) ,mandyes,true);

		{
			XMLOstream::Tag geoLoc(xmlOstr, "Geographic_Localization", "", mandyes);
			XMLOstream::Tag geoType(xmlOstr, "Geo_Type", getGeoType(),mandyes,true);
			if( 0 != _geoCoordsPrecision ) {
				XMLOstream::Tag geoType1(xmlOstr, "GeoCoords_Precision", _geoCoordsPrecision, mandno,true);
			}
			{	
				XMLOstream::Tag timeRange(xmlOstr, "Time_Range","",mandyes);
				XMLOstream::Tag startTime(xmlOstr, "Start_Time", getMetadataValue("Start_Time", false),mandyes,true);		
				XMLOstream::Tag stopTime(xmlOstr, "Stop_Time", getMetadataValue("Stop_Time", false),mandyes,true);
			}
			//calculate the number of Geo Points

			vector<GeoPointInfo> geoPts;
			if(_startGeoPoint.latitude != "") {
				geoPts.push_back(_startGeoPoint);
            }

			for(vector<GeoPointInfo>::const_iterator it = _geoPoints.begin(); it !=_geoPoints.end(); ++it)
			{
				geoPts.push_back(*it);
			}

			if(_stopGeoPoint.latitude != "") {
				geoPts.push_back(_stopGeoPoint);
            }

			{
				ostringstream listCount;
				listCount << "count=\"" << geoPts.size() << "\"";
				//add the List of Geo Points to Metadata
				XMLOstream::Tag geoPntList(xmlOstr, "List_Of_Geo_Pnt", "", listCount.str());
                size_t gpsize = geoPts.size() ;
				for(unsigned int i=0; i<gpsize; ++i)
				{
					XMLOstream::Tag geoPnt(xmlOstr, "Geo_Pnt", "", mandyes);
					XMLOstream::Tag lat(xmlOstr, "Latitude", geoPts[i].latitude,mandyes,true);		
					XMLOstream::Tag longit(xmlOstr, "Longitude", geoPts[i].longitude,mandyes,true);
					geoPnt.close();
				}
			}

		}
	   
		XMLOstream::Tag quatInfo(xmlOstr, "Quality_Info", getMetadataValue("Quality_Info", false) ,mandyes,true);

		XMLOstream::Tag valFlag(xmlOstr, "Validity_Flag",  getMetadataValue("Validity_Flag") ,mandyes,true);

		XMLOstream::Tag valDate(xmlOstr, "Validation_Date", getMetadataValue("Validation_Date") ,mandyes,true);

		XMLOstream::Tag headSize(xmlOstr, "Header_Size",  getMetadataValue("Header_Size") ,mandyes,true);

		XMLOstream::Tag dataSize(xmlOstr, "Data_Size",  getMetadataValue("Data_Size") ,mandyes,true);

		XMLOstream::Tag fileType(xmlOstr, "File_Type",  getMetadataValue("File_Type") ,mandyes,true);

		XMLOstream::Tag fileClass(xmlOstr, "File_Class",  getMetadataValue("File_Class"),mandyes,true);

		XMLOstream::Tag sensId(xmlOstr, "Sensor_Id",  getMetadataValue("Sensor_Id", false) ,mandyes,true);

		XMLOstream::Tag statId(xmlOstr, "Acquisition_Station_Id",  getMetadataValue("Acquisition_Station_Id", false) ,mandyes,true);

 		XMLOstream::Tag procStatId(xmlOstr, "Processing_Station_Id",   getMetadataValue("Processing_Station_Id", false) ,mandyes,true);

 		XMLOstream::Tag sensMode(xmlOstr, "Sensor_Mode",  getMetadataValue("Sensor_Mode", false) ,mandyes,true);

		XMLOstream::Tag phase(xmlOstr, "Phase", getMetadataValue("Phase", false) ,mandyes,true);

		XMLOstream::Tag satId(xmlOstr, "Satellite_Id",  getMetadataValue("Satellite_Id",mandatoryFlag) ,mandyes,true);

		XMLOstream::Tag ascFlag(xmlOstr, "Ascending_Flag",  getMetadataValue("Ascending_Flag", false) ,mandyes,true);

		XMLOstream::Tag eqXLong(xmlOstr, "Equator_X_Longitude",  getMetadataValue("Equator_X_Longitude", false),mandyes,true);

		XMLOstream::Tag eqXTime(xmlOstr, "Equator_X_Time",  getMetadataValue("Equator_X_Time", false) ,mandyes,true);

		XMLOstream::Tag OrderId(xmlOstr, "Order_Id",  getMetadataValue("Order_Id", false) ,mandyes,true);

		XMLOstream::Tag mission(xmlOstr, "Mission",  getMetadataValue("Mission", false),mandyes,true);

		XMLOstream::Tag qualified(xmlOstr, "Qualified",  getMetadataValue("Qualified", false) ,mandno,true);

		XMLOstream::Tag band(xmlOstr, "Band",  getMetadataValue("Band", false) ,mandno,true);

		XMLOstream::Tag tdi(xmlOstr, "Tdi",  getMetadataValue("Tdi", false) ,mandno,true);

		XMLOstream::Tag cloudpctg(xmlOstr, "CloudPercentage",  getMetadataValue("CloudPercentage", false) ,mandno,true);

		XMLOstream::Tag prodspec(xmlOstr, "ProductSpecification",  getMetadataValue("ProductSpecification", false) ,mandno,true);

        XMLOstream::Tag bsline(xmlOstr, "Baseline",  getMetadataValue("Baseline", false) ,mandno,true);
		
		XMLOstream::Tag relation(xmlOstr, "Relation",  getMetadataValue("Relation", false) ,mandno,true);

		XMLOstream::Tag relationType(xmlOstr, "RelationType",  getMetadataValue("RelationType", false) ,mandno,true);

		{
			// CI-91 This tag should be only written. No need to read back.
			string skip_warn (auto_version_file::name) ;  // Just to avoid a warning at compiling time
			XMLOstream::Tag mtd_ver(xmlOstr, "Metadata_Version", auto_version_file::tag, mandno,true);
		}

		XMLOstream::Tag md5(xmlOstr, "Md5",  getMetadataValue("Md5", false) ,mandno,true);


		{
			//add the List of Comments to Metadata if any
			//
			ostringstream commListCount;
			commListCount << "count=\"" << _inventoryComments.size() << "\"";
			XMLOstream::Tag commList(xmlOstr, "List_Of_Comments", "", commListCount.str()+ " "+mandno);
			for(unsigned int i=0; i<_inventoryComments.size() ; i++)
			{
				XMLOstream::Tag comment(xmlOstr, "Comment");
				XMLOstream::Tag text(xmlOstr, "Text", _inventoryComments[i].text,"",true);
				XMLOstream::Tag date(xmlOstr, "Date",  _inventoryComments[i].date,"",true);
				XMLOstream::Tag author(xmlOstr, "Author", _inventoryComments[i].author,"",true);
			}
		}

		{
			ostringstream gapListCount;
			gapListCount << "count=\"" << _inventoryGaps.size() << "\"";
			XMLOstream::Tag gapList(xmlOstr, "List_Of_Gaps", "", gapListCount.str()+ " "+mandno);
			for(unsigned int i=0; i<_inventoryGaps.size(); i++)
			{
				XMLOstream::Tag gap(xmlOstr, "Gap");
				XMLOstream::Tag starttime(xmlOstr, "StartTime", _inventoryGaps[i].startTime,"",true);		
				XMLOstream::Tag stoptime(xmlOstr, "StopTime", _inventoryGaps[i].stopTime,"",true);
			}
		}

		{
			ostringstream mdsListCount;
			mdsListCount << "count=\"" << _mdsData.size() << "\"";
			XMLOstream::Tag mdsList(xmlOstr, "List_Of_MDSs", "", mdsListCount.str()+ " "+mandno);
			for(unsigned int i=0; i<_mdsData.size(); i++)
			{
				XMLOstream::Tag mds(xmlOstr, "MDS");
				XMLOstream::Tag name(xmlOstr, "Ds_Name", 	_mdsData[i].Ds_Name,"",true);		
				XMLOstream::Tag type(xmlOstr, "Ds_Type", 	_mdsData[i].Ds_Type,"",true);
				XMLOstream::Tag number(xmlOstr, "Num_Dsr", 	_mdsData[i].Num_Dsr,"",true);
			}
		}
		
		{
			ostringstream sitesListCount;
			sitesListCount << "count=\"" << _sites.size() << "\"";
			XMLOstream::Tag sitesList(xmlOstr, "List_Of_Sites", "", sitesListCount.str()+ " "+mandno);
			for(unsigned int i=0; i<_sites.size(); i++)
			{
				XMLOstream::Tag site(xmlOstr, "Site");
				XMLOstream::Tag siteId(xmlOstr, "Id", _sites[i],"",true);		
			}
			sitesList.close();
		}
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata stream written!!!") ;

}


Metadata& operator<<(Metadata& mtd, const ostringstream& os) // PRQA S 4222
{
	try {
		
		XMLIstringstream xis(os.str());
  		rsResourceSet xmlResource;
  		xis >> xmlResource ;
		mtd.readMetadata(xmlResource) ;
	}
	catch (exception& ex) {
    	
		ACS_THROW(Metadata::MetadataException(ex,"Metadata::readMetadataFile : Error reading metadata")); // PRQA S 3081
	}
	
	return mtd; // PRQA S 4028
}


void Metadata::readMetadataFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "reading Metadata file " << fileName << "...") ;
	
	rsResourceSet xmlResource;
	
	try {
		
		XMLIstream  xmlIstream(fileName);
		
		xmlIstream >> xmlResource;
		
		readMetadata(xmlResource) ;
		
	}
	catch (const exception& ex) {
		ACS_THROW(MetadataException(ex,"Metadata::readMetadataFile : Error reading file " + fileName+ ".")) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata file " << fileName << " read!!!") ;
}

string Metadata::extractValue(rsResourceSet& rs, const std::string& nm, bool mandatory) 
{
	string val ;
	try {
		//read from resourceset
		rs.getValue(nm,val);  		//use current namespace 
		// ..and copy inside local structure
		setMetadataValue(nm,val);   //use current _currentFilePath
	}
	catch(exception &e){
		if (mandatory) { //mandatory only if global section
			throw ;
        }
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "'" << nm << "' not found ") ;
		}
	}
	return val ;
}

void Metadata::readMetadata(rsResourceSet& xmlResource)
{

	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
		string mtd =  File::tempname("/tmp/metadata", "rs") ; // PRQA S 3050
		ofstream tmprs( mtd.c_str() );
		tmprs << xmlResource;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "tmp metadata left in " << mtd) ;
    ACS_CLASS_END_DEBUG
	//clear all structure
	clean();

	//## first read global data

    try {
        xmlResource.getArray("OptionalNameValues.NameValue", _nameValues) ;
        if (_nameValues.empty()) {
            ACS_LOG_PRIVINFO("Optional name-value pairs not found") ;
        }
    }
    catch(exception &x) {
        ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
            ACS_LOG_NOTIFY_EX(x) ;
        ACS_CLASS_END_DEBUG
        ACS_LOG_PRIVINFO("Optional name-value pairs not found") ;
    }

	readMetadataBlock(xmlResource); 
}
	
void Metadata::readMetadataBlock(rsResourceSet& xmlResource)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "reading Metadata rs ... ") ;
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		try {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing /tmp/metadata.rs") ;
			ofstream rsin("/tmp/metadata.rs") ;
			rsin << xmlResource ;
		}
		catch(exception &e){
			ACS_LOG_NOTIFY_EX(e) ;
		} 
	ACS_CLASS_END_DEBUG

	string val ;

	//File_Name & File_Version are mandatory for *all* section (not only global one)
	string filename = extractValue(xmlResource, "File_Name", true ) ;
	if (filename.empty() )
	{
		ACS_THROW(exIllegalValueException("no File_Name found ")) ; // PRQA S 3081
	}	

	string fileversion = extractValue(xmlResource, "File_Version", true ) ;
	if (fileversion.empty() )
	{
		ACS_THROW(exIllegalValueException("no File_Version found ")) ; // PRQA S 3081
	}	
    
    //remember that mandatory flag is used only for global section
   
	extractValue(xmlResource, "Unique_Id", false ) ;

	extractValue(xmlResource, "Logical_Id", false ) ;

	extractValue(xmlResource, "Parent_Id", false ) ;

	extractValue(xmlResource, "Quality_Value", false ) ;

	extractValue(xmlResource, "Inventory_Extra", false ) ;

	extractValue(xmlResource, "Group_Id", false ) ;


	// Extract optional values for Additional Inventory Parameters
	extractValue(xmlResource, _mtdkey_Session_Id, false ) ;

	extractValue(xmlResource, _mtdkey_Granule_Number, false ) ;

	extractValue(xmlResource, _mtdkey_Granule_Position, false ) ;

	extractValue(xmlResource, _mtdkey_Dump_Start, false ) ;

	extractValue(xmlResource, _mtdkey_Mission_Data_Take_Id, false ) ;

	extractValue(xmlResource, _mtdkey_Next_Equator_X_Time, false ) ;


	// field System is handled as optional
	extractValue(xmlResource, "System", false ) ;
 
	extractValue(xmlResource, "Source", true ) ;

	extractValue(xmlResource, "Source_Sw_Version", true ) ;

	extractValue(xmlResource, "Generation_Time", true ) ;

	extractValue(xmlResource, "Validity_Start", true ) ;

	extractValue(xmlResource, "Validity_Stop", true ) ;

	extractValue(xmlResource, "Start_Orbit_Number", true ) ; //TOSEE

	extractValue(xmlResource, "Stop_Orbit_Number",  true) ;


	xmlResource.getValue("Geographic_Localization.Geo_Type",val);
    if(!val.empty() ) {
		setGeoType( dbGeoObject::string2Type(val) );
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Geographic_Localization.Geo_Type: " << val) ;
	
	const string geoCoorsPrecisionsKey = "Geographic_Localization.GeoCoords_Precision";
	if( xmlResource.hasKey(geoCoorsPrecisionsKey) ) {
		xmlResource.getValue(geoCoorsPrecisionsKey, val);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, geoCoorsPrecisionsKey << ": \"" << val << "\"") ;
		
		if(!val.empty() ) {
			istringstream iss(val);
			int precision = -1 ;
			iss >> precision;
			setGeoCoordsPrecision(precision);
		}

	}
	
	xmlResource.getValue("Geographic_Localization.Time_Range.Start_Time",val);
	setMetadataValue("Start_Time",val);

	xmlResource.getValue("Geographic_Localization.Time_Range.Stop_Time",val);
	setMetadataValue("Stop_Time",val);

	size_t numOfGeoPoints = xmlResource.getArraySize("Geographic_Localization.List_Of_Geo_Pnt.Geo_Pnt"); // PRQA S 4412
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Geographic_Localization.List_Of_Geo_Pnt count: " << numOfGeoPoints)  ;
	_geoPoints.clear() ;
	//fill it now
	for(size_t i=0; i<numOfGeoPoints; ++i)
	{
		string latVal ;
        string longVal ;
		ostringstream latFieldName ;
        ostringstream lonFieldName;
		latFieldName << "Geographic_Localization.List_Of_Geo_Pnt.Geo_Pnt[" << i << "].Latitude";
		lonFieldName << "Geographic_Localization.List_Of_Geo_Pnt.Geo_Pnt[" << i << "].Longitude";
		xmlResource.getValue(latFieldName.str(),latVal);
		xmlResource.getValue(lonFieldName.str(),longVal);
		GeoPointInfo point;
		point.latitude = latVal;
		point.longitude = longVal;
		_geoPoints.push_back(point); 
	}

	extractValue(xmlResource, "Quality_Info", true) ;

	extractValue(xmlResource, "Validity_Flag", true ) ;

	extractValue(xmlResource, "Validation_Date", true ) ;

	extractValue(xmlResource, "Header_Size", true ) ;

	extractValue(xmlResource, "Data_Size", true ) ;

	extractValue(xmlResource, "File_Type", true ) ;

	extractValue(xmlResource, "File_Class", true ) ;

	extractValue(xmlResource, "Sensor_Id", true ) ;

	extractValue(xmlResource, "Acquisition_Station_Id", true ) ;

	extractValue(xmlResource, "Processing_Station_Id", true ) ;

	extractValue(xmlResource, "Sensor_Mode", true ) ;

	extractValue(xmlResource, "Phase", true ) ;

	extractValue(xmlResource, "Satellite_Id", true ) ;

	extractValue(xmlResource, "Ascending_Flag", true ) ;

	extractValue(xmlResource, "Equator_X_Longitude", true ) ;

	extractValue(xmlResource, "Equator_X_Time", true ) ;

	extractValue(xmlResource, "Order_Id", true ) ;

	extractValue(xmlResource, "Mission", true ) ;

	// Optional field
	extractValue(xmlResource, "Qualified", false ) ;

	// Optional field
	extractValue(xmlResource, "Band", false ) ;

	// Optional field
	extractValue(xmlResource, "Tdi", false ) ;

	// Optional field
	extractValue(xmlResource, "CloudPercentage", false ) ;

	// Optional field
	extractValue(xmlResource, "ProductSpecification", false ) ;

	// Optional field
	extractValue(xmlResource, "Baseline", false ) ;
	
	// Optional field
	extractValue(xmlResource, "Relation", false ) ;
	
	// Optional field
	extractValue(xmlResource, "RelationType", false ) ;

	// Optional field
	extractValue(xmlResource, "Md5", false ) ;

	size_t numOfComments = 0;
	try
	{
		numOfComments = xmlResource.getArraySize("List_Of_Comments.Comment");
        _inventoryComments.clear() ;
		for(size_t i=0; i<numOfComments; ++i)
		{
			string text ;
            string date; 
            string author;
			ostringstream textFieldName ;
            ostringstream dateFieldName ;
            ostringstream authFieldName ;
			textFieldName << "List_Of_Comments.Comment[" << i << "].Text";
			dateFieldName << "List_Of_Comments.Comment[" << i << "].Date";
			authFieldName << "List_Of_Comments.Comment[" << i << "].Author";
			xmlResource.getValue(textFieldName.str(),text);
			xmlResource.getValue(dateFieldName.str(),date);
			xmlResource.getValue(authFieldName.str(),author);
			Metadata::InventoryComment comment;
			comment.text = text;
			comment.date = date;
			comment.author = author;
			_inventoryComments.push_back(comment);
		}
	}
	catch(exception&)
	{
        // nothing 
	}


	size_t numOfGaps =0 ;
	try
	{
		numOfGaps = xmlResource.getArraySize("List_Of_Gaps.Gap");
		_inventoryGaps.clear() ;
		
		for(unsigned int i=0; i<numOfGaps; ++i)
		{
			string start="" ;
            string stop="";
			ostringstream startFieldName ;
            ostringstream stopFieldName;
			startFieldName << "List_Of_Gaps.Gap[" << i << "].StartTime";
			stopFieldName  << "List_Of_Gaps.Gap[" << i << "].StopTime";
			xmlResource.getValue(startFieldName.str(),start);
			xmlResource.getValue(stopFieldName.str(),stop);
			Metadata::InventoryGap gap;
			gap.startTime = start;
			gap.stopTime = stop;
			_inventoryGaps.push_back(gap);
		}
	}
	catch(exception&)
	{
        // nothing 
	}

	// Measurement data Set section optional
	size_t numOfMDSs = 0;
	try
	{
		numOfMDSs = xmlResource.getArraySize("List_Of_MDSs.MDS");
		_mdsData.clear() ;
		
		//fill it now
		for(unsigned int i=0; i<numOfMDSs; ++i)
		{
			string Name="" ;
            string Type="" ; 
            string NumOfDsr="" ;
			ostringstream FieldName ;
            ostringstream FieldType ;
            ostringstream FieldNumber;
			FieldName 	<< "List_Of_MDSs.MDS[" << i << "].Ds_Name";
			FieldType  	<< "List_Of_MDSs.MDS[" << i << "].Ds_Type";
			FieldNumber	<< "List_Of_MDSs.MDS[" << i << "].Num_Dsr";

			xmlResource.getValue(FieldName.str(),Name);
			xmlResource.getValue(FieldType.str(),Type);
			xmlResource.getValue(FieldNumber.str(),NumOfDsr);

			Metadata::MeasDataSetDescriptor mds;
			mds.Ds_Name = Name;
			mds.Ds_Type = Type;
			mds.Num_Dsr = NumOfDsr;
			_mdsData.push_back(mds);
		}
	}
	catch(exception&)
	{
        // nothing 
	}

	// Measurement data Set section optional
	size_t numOfSites = 0;
	try
	{
		numOfSites = xmlResource.getArraySize("List_Of_Sites.Site");
		_sites.clear() ;

		//fill it now
		for(unsigned int i=0; i<numOfSites; ++i)
		{
			int id = 0 ;
			ostringstream SiteId ;
			SiteId << "List_Of_Sites.Site[" << i << "].Id";

			xmlResource.getValue(SiteId.str(),id);

			_sites.push_back(id);
		}
	}
	catch(exception& e)
	{
        // nothing 
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata rs read!!!") ;
}

	

void Metadata::computeGeoPoints(const AsciiTime& outStartTime,const AsciiTime& outStopTime, const double& step, const string& id, int mission) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method called with step " << step) ;
	//convert start/stop time in processing format
	ProcessingTime startProcTime ;
    ProcessingTime stopProcTime ;
	startProcTime.setRef(TimeFormat::TC_TIME_UTC);
	startProcTime.setFormat(TimeFormat::TC_PROC);
	stopProcTime.setRef(TimeFormat::TC_TIME_UTC);
	stopProcTime.setFormat(TimeFormat::TC_PROC);
	TimeConverterSingleton::Instance()->getConverter()->convert(outStartTime,startProcTime);
	TimeConverterSingleton::Instance()->getConverter()->convert(outStopTime,stopProcTime);
	
	long double start = startProcTime.getValue();
	long double stop = stopProcTime.getValue();

	GeoPointIterator geoIt(id,mission);
	long double margin = ( (stop - start)/ 2.0 ) / 2.0;
	vector<Metadata::GeoPointInfo> geoPoints = geoIt.iterate(start,stop,step*86400.0, margin);
	if(geoPoints.size() > 0) {
		_geoPoints = geoPoints;
    }
	else
	{
        ACS_LOG_WARNING("Unable to calculate geoPoints. Start and Stop Time set as Geo Points....") ;
        return;			
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method executed!!!") ;

}

void Metadata::setGeoType(const dbGeoObject::Type& type) /*AAA*/
{
    _geoType = "";

    const string geoType( dbGeoObject::type2String( type ) );
    ACS_COND_THROW(( geoType == "Unrecognized" ), MetadataException("Error : Geo object type value is not recognized.!.") ); // PRQA S 3081

    _geoType = geoType;
			
}

//TOSEE can be global for all file?
void Metadata::setStartStopGeoPoints(const string& startLat,
									 const string& startLong,
									 const string& stopLat,
									 const string& stopLong)
{
	_startGeoPoint.latitude  = startLat;
	_startGeoPoint.longitude = startLong;
	_stopGeoPoint.latitude   = stopLat;
	_stopGeoPoint.longitude  = stopLong;
}

void Metadata::clean()
{
	_geoPoints.clear();
	_inventoryGaps.clear();
	_inventoryComments.clear();
	_sites.clear();
	_mdsData.clear();
	_startGeoPoint.latitude = _startGeoPoint.longitude = "";
	_stopGeoPoint.latitude = _stopGeoPoint.longitude = "";

	//allocate first MetadataInfo for global data
    _nameValues.clear() ;
}


void Metadata::setGeoPoints(const std::vector<GeoPointInfo>& points)
{
	_geoPoints = points;
}
 

void Metadata::getGeoPoints(std::vector<Metadata::GeoPointInfo> &v) const // PRQA S 4020
{
	v = _geoPoints ;
}	

const std::string& Metadata::getGeoType() const // PRQA S 4120
{
	return _geoType;
}	
 
void Metadata::setGeoCoordsPrecision(int p)
{
    ACS_COND_THROW( p <= 0, MetadataException("Error : Precision must be greater than 0") ); // PRQA S 3081
	
	_geoCoordsPrecision = p;
}

const int& Metadata::getGeoCoordsPrecision() const // PRQA S 4120
{
	return _geoCoordsPrecision;
}	

void Metadata::setGaps(const std::vector<InventoryGap>& gaps)
{
	_inventoryGaps = gaps;
}

void Metadata::getGaps(std::vector<Metadata::InventoryGap>& v) const // PRQA S 4020
{	
	v = _inventoryGaps ;
}
 
void Metadata::setComments(const std::vector<InventoryComment>& comm)
{
	_inventoryComments = comm;
}

void Metadata::getComments(std::vector<Metadata::InventoryComment>& v ) const // PRQA S 4020
{
	v = _inventoryComments ;
}
 

void Metadata::setMDSs(const std::vector<MeasDataSetDescriptor>& mdss)
{
	_mdsData = mdss;
}
 
void Metadata::getMDSs(std::vector<Metadata::MeasDataSetDescriptor>& v) const // PRQA S 4020
{
	v = _mdsData ;
}

void Metadata::setSites(const std::vector<int>& sites)
{
	_sites = sites;
}

void Metadata::getSites(std::vector<int>& v) const // PRQA S 4020
{
	v = _sites ;
}

void Metadata::addNameValue(const std::string &n, const std::string &v) 
{
    _nameValues.push_back(NameValue(n,v)) ;
}


_ACS_END_NAMESPACE
