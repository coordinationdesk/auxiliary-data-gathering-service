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

#ifndef _dbInventoryAddParams_H_
#define _dbInventoryAddParams_H_

#include <exException.h>
#include <dbSmartPersistent.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)


//
//
// class dbInventoryAddParams
//
//
class dbInventoryAddParams : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	static std::string table() { return "t_inv_additional_params"; } 

	/** Defaulted special functions */
	virtual ~dbInventoryAddParams() noexcept						= default;
	dbInventoryAddParams(const dbInventoryAddParams&)				= default;
	dbInventoryAddParams& operator=(const dbInventoryAddParams&)	= default;
	dbInventoryAddParams(dbInventoryAddParams&&)					= default;
	dbInventoryAddParams& operator=(dbInventoryAddParams&&)			= default;
	
	/** Deleted special functions */
	dbInventoryAddParams()											= delete;
	
	/** Constructor */
	explicit dbInventoryAddParams(dbConnection&);


	static std::string staticClassName() ;
    // * Returns the name of the class that performs queries on this table - 
    // * it is used by dbSmartQuery<derived-from-me>
    static std::string staticQueryClassName() { return "dbInventoryAddParamsQuery" ; }
    

	// inherited from dbPersistent
	virtual std::string className() const ;

	virtual void exportObj(rsResourceSet &s) const ; // export the class to rsResourceSet
	virtual void importObj(rsResourceSet &s)  ; // import the class from a rsResourceSet
	virtual void exportXML(XMLOstream &xst) const ; // export the class to XML stream

	void setInventoryId(IdType id, dbConnection &c) ;
	dbPersistent::IdType getInventoryId(bool &null, dbConnection &c) const ;
	void unsetInventoryId() ;

	void setTimelinessId(IdType id, dbConnection &) ;
	dbPersistent::IdType getTimelinessId(bool &null, dbConnection &) const ;
	void unsetTimelinessId();

	void setMissionDatatakeId(IdType id, dbConnection &) ;
	dbPersistent::IdType getMissionDatatakeId(bool &null, dbConnection &) const ;
	void unsetMissionDatatakeId();

	void setGranuleNumber(const int &, dbConnection &) ;
	int getGranuleNumber(bool &null, dbConnection &) const ;
	void unsetGranuleNumber();

 	void setGranulePosition(const std::string &, dbConnection &) ;
	std::string getGranulePosition(bool &null, dbConnection &) const ;
	void unsetGranulePosition();

 	void setDumpStart(const DateTime &, dbConnection &) ;
	DateTime getDumpStart(bool &null, dbConnection &) const ;
	void unsetDumpStart();
 
	void setNextEquatorXTime(const DateTime &, dbConnection &) ;
	DateTime getNextEquatorXTime(bool &null, dbConnection &) const ;
	void unsetNextEquatorXTime();

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryAddParams)
} ;


_ACS_END_NAMESPACE

#endif // _dbInventoryAddParams_H_
