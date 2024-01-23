/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.5  2016/07/04 15:17:44  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.4  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:57  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:25  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/05/14 19:06:22  marpas
	PostGRES/PostGIS support
	
	Revision 2.5  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.4  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2011/05/13 14:54:29  chicas
	changed processing tag
	
	Revision 2.2  2011/05/02 13:45:38  chicas
	work in progress
	
	Revision 2.1  2011/04/29 13:35:00  chicas
	work in progress for GeoValIntersect special query
	

*/

#include <dbInventoryGeoValIntersectQuery.h>
#include <dbInventoryObject.h>
#include <StringUtils.h>

#include <dbGeoPnt.h>
#include <dbGeoPoint.h>
#include <dbGeoPolygon.h>
#include <dbGeoBox.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <XMLIstringstream.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
    const string _polygonTag      = "PRODUCT_EXTENT_POLYGON";
    const string _centreLatLonTag = "PRODUCT_EXTENT_AREA_CENTER";
    const string _linesNumberTag  = "PRODUCT_EXTENT_AREA_NB_ROW";
    const string _integrationTime = "INTEGRATION_TIME";
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryGeoValIntersectQuery)

string dbInventoryGeoValIntersectQuery::staticClassName()
{
	return "dbInventoryGeoValIntersectQuery" ;
}



dbInventoryGeoValIntersectQuery::dbInventoryGeoValIntersectQuery(const string &dbkey) :
    dbInventoryValCoverOrIntersectQuery(false , true, dbkey)
{


}


dbInventoryGeoValIntersectQuery::dbInventoryGeoValIntersectQuery(dbInventoryGeoValIntersectQuery const &r) :
    dbInventoryValCoverOrIntersectQuery(r) // these will be override by assignement
{
}

dbInventoryGeoValIntersectQuery &dbInventoryGeoValIntersectQuery::operator=(dbInventoryGeoValIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryGeoValIntersectQuery::~dbInventoryGeoValIntersectQuery() throw() {}


unsigned long dbInventoryGeoValIntersectQuery::doQuery(dbConnection &conn,bool incremental,bool enabled)
{

	bool found = true; //if no clob, call usual intersect

	if( !getParametersCLOB().empty() )
	{
		XMLIstringstream clobStream (  getParametersCLOB ()  ) ;
		rsResourceSet clobParameters;
		clobStream >> clobParameters ;
		
		//check parameters and recompute time interval according geographic input
		if ( hasProcessingOption(clobParameters,_linesNumberTag))
			found = setTimeFromLatLonCentre(conn, clobParameters);
		else if ( hasProcessingOption(clobParameters,_polygonTag))
			found = setTimeFromPolygon(conn, clobParameters);
	}
	//call
	if (! found) 
		return 0;
	else
		return dbInventoryValCoverOrIntersectQuery::doQuery ( conn, incremental, enabled);
}


bool dbInventoryGeoValIntersectQuery::setTimeFromLatLonCentre(dbConnection &conn, const rsResourceSet & clobParameters)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

//<processing>
//<optionName>PRODUCT_EXTENT_AREA_NB_ROW</optionName>
//<optionSelectedValues>34</optionSelectedValues>
//</processing>
//<processing>
//<optionName>PRODUCT_EXTENT_AREA_CENTER</optionName>
//<optionSelectedValues>30.7 12</optionSelectedValues>
//</processing>
//<processing>
//<optionName>INTEGRATION_TIME</optionName>
//<optionSelectedValues>1</optionSelectedValues>
//</processing>

	double lines;
	getProcessingOption(clobParameters,_linesNumberTag , lines) ;

	string centreLatLon = getProcessingOption(clobParameters,_centreLatLonTag) ;
	//extract coordinates..
	double lat,lon;
	istringstream coords(centreLatLon) ;
	coords >> lat >> lon;
	
	dbGeoPoint point(dbGeoPnt(lat,lon));
	point.timeRange() = dbGeoTimeRange(t0(),t1());
		
	double integrationTime;
	getProcessingOption(clobParameters,_integrationTime, integrationTime) ;//TOSEE
	
	//min start and max stop time of all files of given filetype (?) covering point (lat,lon) 
   // and intersecting order start and stop time

	//string queryString ;
	DateTime firstTime;
	DateTime lastTime;
//TOSEE

    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8"  )
   	{
        SQLString sqlString(dbKey,SQLString::SELECT) ;
       
        sqlString.addSelectList("MIN( " + getValidityStartField() + " ) ");
        sqlString.addSelectList("MAX( " + getValidityStopField() + " ) ");

        sqlString.addTableList(dbInventoryObject::getTable());
    // Build Where clause
        {
            dbQueryParameters pp(dbKey) ;
            pp.addParameter(dbParam(dbKey, dbInventoryObject::getTable() ,"geographiclocalization" , dbParam::INTERSECT , point )) ;
            sqlString.addANDWhereParameter(pp) ;
        }
		if ( getParameters() != "")// filetype,.. 
		{
			sqlString.addANDWhereParameter(getParameters());
		}

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << sqlString.getSQL() << "\"") ;

		dbQuery query(conn, sqlString, "dbInventoryGeoValIntersectQuery::setTimeFromLatLonCentre") ;
		try {
			dbRow row(dbKey) ;
            dbSet set ;
		    query.execForSet(set) ;
            ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
            ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
            set.getNextRow(row) ; // first and only 

			bool isNull;
			row.column(0,firstTime,&isNull);
			row.column(1,lastTime,&isNull);

			if (isNull) { return false; }

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "min validitystart "  << firstTime.YYYYMMDDThhmmss() << " max validitystop " << lastTime.YYYYMMDDThhmmss()) ;
		}
		catch(	dbQuery::QueryNoDataFound & x) {
			return false;
		}

	} else {
		ACS_THROW( exIllegalValueException("Invalid dialect: "+dialectName ));
	}
	double deltaT = lines * integrationTime;  //sec

	firstTime -= DateTime::JD50(deltaT / 86400.0);
	lastTime  += DateTime::JD50(deltaT / 86400.0);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "validitystart for query  start: "  << firstTime.YYYYMMDDThhmmss() << " stop " << lastTime.YYYYMMDDThhmmss()) ;
	
	//ok. Update time interval	
	t0(firstTime);
	t1(lastTime);

	return true;

}

bool dbInventoryGeoValIntersectQuery::setTimeFromPolygon(dbConnection &conn, const rsResourceSet & clobRs)
{
//<processing>
//<optionName>PRODUCT_EXTENT_POLYGON</optionName>
//<optionSelectedValues>lat lon lat lon ...</optionSelectedValues>
//</processing>

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	vector<float> coordinates ;
	//getProcessingOptionArray(clobRs,_polygonTag, coordinates, false) ;
	string latlons= getProcessingOption(clobRs,_polygonTag) ;

	istringstream latLonCoords(latlons);
	while ( ! latLonCoords.eof())
	{
		float value;
		latLonCoords >> value;
		coordinates.push_back(value);
	}
	if (coordinates.size() % 2 || ! coordinates.size())
		ACS_THROW( exIllegalValueException(string("Bad data format for processing ; an even number of coordinates should be specified.")));
		
	//fill pntList
	dbGeoPntList area;
	for (size_t i = 1; i< coordinates.size() ; i+=2) {
		area.append(coordinates[i-1],coordinates[i]);
    }
	
	//bounding box
	dbGeoBox box;
	area.bbox(box,0,0);
	box.timeRange() = dbGeoTimeRange(t0(),t1());

	//Query
	string queryString ;
	DateTime firstTime;
	DateTime lastTime;

    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( (dialectName == "Informix9") || (dialectName == "Postgres8") | (dialectName == "Oracle11") )
   	{   
   		SQLString sqlString(dbKey, SQLString::SELECT) ;
       
        sqlString.addSelectList("MIN( " + getValidityStartField() + " ) ");
        sqlString.addSelectList("MAX( " + getValidityStopField() + " ) ");

        sqlString.addTableList(dbInventoryObject::getTable());
    // Build Where clause
        {
            dbQueryParameters pp(dbKey) ;
            pp.addParameter(dbParam(dbKey, dbInventoryObject::getTable() ,"geographiclocalization" , dbParam::INTERSECT , box )) ;
            sqlString.addANDWhereParameter(pp) ;
        }
		if ( getParameters() != "")// filetype,.. return SQLString.getWhereParameters() ;SQLString.addANDWhereParameter(p) ;
		{
			sqlString.addANDWhereParameter(getParameters());
		}

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << sqlString.getSQL() << "\"") ;

		dbQuery query(conn, sqlString, "dbInventoryGeoValIntersectQuery::setTimeFromPolygon") ;
		try {

			dbRow row(dbKey) ;
            dbSet set ;
		    query.execForSet(set) ;
            ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
            ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
            set.getNextRow(row) ; // first and only 

			bool isNull;
			row.column(0,firstTime,&isNull);
			row.column(1,lastTime,&isNull);

			if (isNull) 
				return false;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "min validitystart "  << firstTime.YYYYMMDDThhmmss() << " max validitystop " << lastTime.YYYYMMDDThhmmss()) ;

		}
		catch(	dbQuery::QueryNoDataFound & x) {
			return false;
		}
	} else {
		ACS_THROW( exIllegalValueException("Invalid dialect: "+dialectName )); // PRQA S 3081
	}

	//update time interval	
	t0(firstTime);
	t1(lastTime);

	return true;

}	


bool dbInventoryGeoValIntersectQuery::hasProcessingOption( const rsResourceSet &plrs, const string &optName) 
{
	string prokey = "processing" ;
	
	bool isArray;
	plrs.hasKey(prokey, isArray);
	
	size_t numProcessing = 1; 	
	if (isArray)
		 numProcessing = plrs.getArraySize(prokey) ;

	// return true if a specific processing option has been found (optName)
	for (size_t i=0; i < numProcessing; i++) {
		ostringstream key ;
		if (isArray)
			key << prokey << "[" << i << "]" ;
		else
			key << prokey;
		string name ;
		plrs.getValue(key.str()+".optionName",name) ;
		if (StringUtils::equalsNoCase(name, optName)) {
			return true ;
		}
	}
	
	return false ;
}


bool dbInventoryGeoValIntersectQuery::getProcessingOption( const rsResourceSet &plrs, const string &optName, double &value, bool raiseExc) 
{
	// W8115_OFF: 2 returns: true or false !!!
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "optName: " << optName) ;
	string prokey = "processing" ;

	bool isArray;
	plrs.hasKey(prokey, isArray);
	
	size_t numProcessing = 1; 	
	if (isArray)
		 numProcessing = plrs.getArraySize(prokey) ;

	// return true if a specific processing option has been found (optName)
	// the value is expected as double and will be stored in the given double referenced variable
	// if the boolean raiseExc is true and the processing option
	// was not found an exception is raised, else the method returns false
	for (size_t i=0; i < numProcessing; i++) {
		ostringstream key ;
		if (isArray)
			key << prokey << "[" << i << "]" ;
		else
			key << prokey;
		string name ;
		plrs.getValue(key.str()+".optionName",name) ;
		if (StringUtils::equalsNoCase(name, optName)) {
			plrs.getValue(key.str()+".optionSelectedValues",value) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "OptionValue: " << value) ;
			return true ;
		}
	}
	
	if (raiseExc)
		ACS_THROW(rsResourceSet::NotFoundException(string("No processing option with name: \"")+optName+"(\" found in clob order")) ;
	
	return false ;
}

string dbInventoryGeoValIntersectQuery::getProcessingOption(const rsResourceSet &plrs, const string &optName, bool raiseExc) 
{
	// W8115_OFF: 2 returns: value or ""
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "optName: " << optName) ;

	string prokey = "processing" ;
	bool isArray;
	plrs.hasKey(prokey, isArray);
	
	size_t numProcessing = 1; 	
	if (isArray)
		 numProcessing = plrs.getArraySize(prokey) ;

	// return the string value for a specific processing option (optName)
	// if the boolean raiseExc is true and the processing option
	// was not found an exception is raised, else the method returns an empty string
	
	for (size_t i=0; i < numProcessing; i++) {
		ostringstream key ;	
		if (isArray) {
			key << prokey << "[" << i << "]" ;
        }
		else {
			key << prokey;
        }

		string name ;
		plrs.getValue(key.str()+".optionName",name) ;
		if (StringUtils::equalsNoCase(name, optName)) {
			string OptionValue ;
			plrs.getValue(key.str()+".optionSelectedValues",OptionValue) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "OptionValue: " << OptionValue) ;
			return OptionValue ;
		}
	}
	
	ACS_COND_THROW ( raiseExc, rsResourceSet::NotFoundException("No processing option with name: \""+optName+"(\" found in clob order")) ;
	
	return "" ;
}



void dbInventoryGeoValIntersectQuery::getProcessingOptionArray( const rsResourceSet &plrs, const string &optName, vector<float> &opts, bool raiseExc) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "optName: " << optName) ;
	string prokey = "processing" ;
	bool isArray;
	plrs.hasKey(prokey, isArray);

	size_t numProcessing = 1; 	
	if (isArray)
		 numProcessing = plrs.getArraySize(prokey) ;
	
	for (size_t i=0; i < numProcessing; i++) {
		ostringstream key ;
		if (isArray) {
			key << prokey << "[" << i << "]" ;
        }
		else {
			key << prokey;
        }
		string name ;
		plrs.getValue(key.str()+".optionName",name) ;
		if (StringUtils::equalsNoCase(name, optName)) {
			try {
				double value;
				plrs.getValue(key.str()+".optionSelectedValues",value) ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "OptionValue: " << value) ;
				opts.push_back(value) ; // just one
				return ;
			}
			catch(rsResourceSet::NotFoundException &) { // probably more than one
				;
			}
			plrs.getArray(key.str()+".optionSelectedValues",opts,0/*all array*/) ;
			return ;
		}
	}
	
	ACS_COND_THROW (raiseExc,rsResourceSet::NotFoundException("No processing option with name: \""+optName+"(\" found in clob order"));
	
	return ;
}


_ACS_END_NAMESPACE
