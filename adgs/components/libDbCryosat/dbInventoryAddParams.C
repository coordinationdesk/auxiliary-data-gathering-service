// PRQA S 1050 EOF
/*

	Copyright 2021-, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$

*/

#include <dbInventoryAddParams.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <dbConnectionPool.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryAddParams)


/*
 * Constructor
 */
dbInventoryAddParams::dbInventoryAddParams(dbConnection& conn) :
	dbSmartPersistent(table(), conn)
{ }


string dbInventoryAddParams::className() const
{
	return staticClassName() ;
}


string dbInventoryAddParams::staticClassName()
{
	return "dbInventoryAddParams" ;
}

// export the class to stream
void dbInventoryAddParams::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryAddParams") ;
	dbPersistent::exportObj(s) ;
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(getKey())) ;
    dbConnection &c=cw ; // PRQA S 3050
    bool null = false ;
	
	if(isSet("inv_id"))
	{
		s.setValue("_inv_id",getInventoryId(null, c)) ;
	}
	s.setValue("_inv_idSet",isSet("inv_id")) ;

	if(isSet("timeliness_key_id"))
	{
		s.setValue("_timeliness_key_id",getTimelinessId(null, c)) ;
	}
	s.setValue("_timeliness_key_idSet",isSet("timeliness_key_id")) ;
	
	if(isSet("data_take_id"))
	{
		s.setValue("_data_take_id",getMissionDatatakeId(null, c)) ;
	}
	s.setValue("_data_take_idSet",isSet("data_take_id")) ;

	if(isSet("granule_number"))
	{
		s.setValue("_granuleNumber",getGranuleNumber(null, c)) ;
	}
	s.setValue("_granuleNumberSet",isSet("granule_number")) ;
	
	if(isSet("granule_position"))
	{
		s.setValue("_granulePosition",getGranulePosition(null, c)) ;
	}
	s.setValue("_granulePositionSet",isSet("granule_position")) ;
	
	if(isSet("dump_start"))
	{
		s.setValue("_dumpStart",getDumpStart(null, c)) ;
	}
	s.setValue("_dumpStartSet",isSet("dump_start")) ;

	if(isSet("nextequatorxtime"))
	{
		s.setValue("_nextEquatorXTime",getNextEquatorXTime(null, c)) ;
	}
	s.setValue("_nextEquatorXTimeSet",isSet("nextequatorxtime")) ;

}


// import the class from a stream
void dbInventoryAddParams::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryAddParams") ;
	dbSmartPersistent::importObj(s) ;
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(getKey())) ;
    dbConnection &c=cw ; // PRQA S 3050
	bool is = false ;

	s.getValue("_inv_idSet",is) ;
	if(is)
	{
		dbPersistent::IdType v ;
		s.getValue("_inv_id",v) ;
		setInventoryId(v, c) ;
	}
	else {
		unsetInventoryId() ;
    }

	s.getValue("_timeliness_key_idSet",is) ;
	if(is)
	{
		dbPersistent::IdType v ;
		s.getValue("_timeliness_key_id",v) ;
		setTimelinessId(v, c) ;
	}
	else {
		unsetTimelinessId() ;
    }
		
    s.getValue("_data_take_idSet",is) ;
    if(is)
    {
    	dbPersistent::IdType v ;
    	s.getValue("_data_take_id",v) ;
    	setMissionDatatakeId(v, c) ;
    }
    else {
    	unsetMissionDatatakeId() ;
    }
		
		
	s.getValue("_granuleNumberSet",is) ;
	if(is)
	{
		int v ;
		s.getValue("_granuleNumber",v) ;
		setGranuleNumber(v, c) ;
	}
	else {
		unsetGranuleNumber() ;
    }
		
	s.getValue("_granulePositionSet",is) ;
	if(is)
	{
		string v ;
		s.getValue("_granulePosition",v) ;
		setGranulePosition(v, c) ;
	}
	else {
		unsetGranulePosition() ;
    }
	
	s.getValue("_dumpStartSet",is) ;
	if(is)
	{
		DateTime v ;
		s.getValue("_dumpStart",v) ;
		setDumpStart(v, c) ;
	}
	else {
		unsetDumpStart() ;
    }

	s.getValue("_nextEquatorXTimeSet",is) ;
	if(is)
	{
		DateTime v ;
		s.getValue("_nextEquatorXTime",v) ;
		setNextEquatorXTime(v, c) ;
	}
	else {
		unsetNextEquatorXTime() ;
    }


}

// export the class to XML stream
void dbInventoryAddParams::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ; // PRQA S 3050

	XMLOstream::Tag txml(xst, "dbInventoryAddParams");
	dbSmartPersistent::exportXML(xst) ;
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(getKey())) ;
    dbConnection &c=cw ; // PRQA S 3050
    bool null = false ;
	

	if (isSet("inv_id"))
	{ XMLOstream::Tag tag(xst, "_inv_id", getInventoryId(null, c), "label=\"Session Id\""); }
	else
	{ XMLOstream::Tag tag(xst, "_inv_id", "<null>", "label=\"Session Id\""); }
	{ XMLOstream::Tag tag(xst, "_inv_idSet", (isSet("inv_id") ? "true" : "false")) ; }

	if (isSet("timeliness_key_id"))
	{ XMLOstream::Tag tag(xst, "_timeliness_key_id", getTimelinessId(null, c), "label=\"Session Id\""); }
	else
	{ XMLOstream::Tag tag(xst, "_timeliness_key_id", "<null>", "label=\"Session Id\""); }
	{ XMLOstream::Tag tag(xst, "_timeliness_key_idSet", (isSet("timeliness_key_id") ? "true" : "false")) ; }

	if (isSet("data_take_id"))
	{ XMLOstream::Tag tag(xst, "_data_take_id", getMissionDatatakeId(null, c), "label=\"Mission Datatake Id\""); }
	else
	{ XMLOstream::Tag tag(xst, "_data_take_id", "<null>", "label=\"Session Id\""); }
	{ XMLOstream::Tag tag(xst, "_data_take_idSet", (isSet("data_take_id") ? "true" : "false")) ; }

	if (isSet("granule_number"))
	{ XMLOstream::Tag tag(xst, "_granuleNumber", getGranuleNumber(null, c), "label=\"Granule Number\""); }
	else
	{ XMLOstream::Tag tag(xst, "_granuleNumber", "<null>", "label=\"Granule Number\""); }
    { XMLOstream::Tag tag(xst, "_granuleNumberSet", (isSet("granule_number") ? "true" : "false")) ; }

	{ XMLOstream::Tag tag(xst, "_granulePosition", isSet("granule_position")? getGranulePosition(null, c) : "<null>", "label=\"Granule Position\""); }
    { XMLOstream::Tag tag(xst, "_granulePositionSet", (isSet("granule_position") ? "true" : "false")) ; }

	if(isSet("dump_start")) {
		XMLOstream::Tag tag(xst, "_dumpStart", "", "label=\"Dump Start\"");
		getDumpStart(null, c).exportXML(xst) ;
	}
	else {
		XMLOstream::Tag tag(xst, "_dumpStart", "<null>", "label=\"Dump Start\"");
	}
    { XMLOstream::Tag tag(xst, "_dumpStartSet", (isSet("dump_start") ? "true" : "false")) ; }

	if(isSet("nextequatorxtime")) {
		XMLOstream::Tag tag(xst, "_nextEquatorXTime", "", "label=\"Next equator X Time\"");
		getNextEquatorXTime(null, c).exportXML(xst) ;
	}
	else {
		XMLOstream::Tag tag(xst, "_nextEquatorXTime", "<null>", "label=\"Next equator X Time\""); 
	}
    { XMLOstream::Tag tag(xst, "_nextEquatorXTimeSet", (isSet("nextequatorxtime") ? "true" : "false")) ; }

}

void dbInventoryAddParams::setInventoryId(IdType id, dbConnection &c)
{
        set("inv_id",id, c) ;
}


dbPersistent::IdType dbInventoryAddParams::getInventoryId(bool &set, dbConnection &c) const // PRQA S 4214
{
        long long id = 0 ;

        get("inv_id",id, set, c) ;

        return id ;
}

void dbInventoryAddParams::unsetInventoryId()
{
        unset("inv_id") ;
}


void dbInventoryAddParams::setTimelinessId(IdType id, dbConnection &c)
{
    set("timeliness_key_id",id, c) ;
}


dbPersistent::IdType dbInventoryAddParams::getTimelinessId(bool &set, dbConnection &c) const // PRQA S 4214
{
    long long id = 0 ;

    get("timeliness_key_id",id, set, c) ;

    return id ;
}


void dbInventoryAddParams::unsetTimelinessId()
{
	unset("timeliness_key_id") ;
}


void dbInventoryAddParams::setMissionDatatakeId(IdType id, dbConnection &c)
{
	set("data_take_id",id, c) ;
}


dbPersistent::IdType dbInventoryAddParams::getMissionDatatakeId(bool &set, dbConnection &c) const // PRQA S 4214
{
    long long id = 0 ;

    get("data_take_id",id, set, c) ;

    return id ;
}


void dbInventoryAddParams::unsetMissionDatatakeId()
{
	unset("data_take_id") ;
}


void dbInventoryAddParams::setGranuleNumber(const int &v, dbConnection &c)
{
	set<int>("granule_number",v,c) ;
}


int dbInventoryAddParams::getGranuleNumber(bool &null, dbConnection &c) const // PRQA S 4214
{
	int v ; 

	get<int>("granule_number",v, null, c) ;

	return v ;
}


void dbInventoryAddParams::unsetGranuleNumber()
{
	unset("granule_number") ;
}



void dbInventoryAddParams::setGranulePosition(const string &v, dbConnection &c)
{
	set<string>("granule_position",v,c) ;
}


string dbInventoryAddParams::getGranulePosition(bool &null, dbConnection &c) const // PRQA S 4214
{
	string v ; 

	get<string>("granule_position",v, null, c) ;

	return v ;
}


void dbInventoryAddParams::unsetGranulePosition()
{
	unset("granule_position") ;
}



void dbInventoryAddParams::setDumpStart(const DateTime &v, dbConnection &c)
{
	set<DateTime>("dump_start",v,c) ;
}


DateTime dbInventoryAddParams::getDumpStart(bool &null, dbConnection &c) const // PRQA S 4214
{
	DateTime v ; 

	get<DateTime>("dump_start",v, null, c) ;

	return v ;
}


void dbInventoryAddParams::unsetDumpStart()
{
	unset("dump_start") ;
}


void dbInventoryAddParams::setNextEquatorXTime(const DateTime &v, dbConnection &c)
{
	set<DateTime>("nextequatorxtime",v,c) ;
}


DateTime dbInventoryAddParams::getNextEquatorXTime(bool &null, dbConnection &c) const // PRQA S 4214
{
	DateTime v ; 

	get<DateTime>("nextequatorxtime",v, null, c) ;

	return v ;
}


void dbInventoryAddParams::unsetNextEquatorXTime()
{
	unset("nextequatorxtime") ;
}



_ACS_END_NAMESPACE
