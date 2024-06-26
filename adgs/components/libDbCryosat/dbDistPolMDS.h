// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	
	Revision 5.4  2016/04/19 13:53:46  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:53  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:21  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.5  2013/03/29 19:51:28  marpas
	removing html related functions
	
	Revision 2.4  2012/11/16 13:01:12  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.2  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2007/04/18 22:12:55  marpas
	Management of dbDistributionpolicies relations with respect to data sets and originators
	
	
*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbDistPolMDS_H_
#define _dbDistPolMDS_H_

#include <exException.h>
#include <dbSmartPersistent.h>
#include <DateTime.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbCryosatFactory ;

class dbRow ;
class rsResourceSet ;
class XMLOstream ;

//
//
//
// class dbDistPolMDS
//
//
//
class dbDistPolMDS : // PRQA S 2109, 2153
	public dbSmartPersistent
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	virtual ~dbDistPolMDS() throw() ;
	dbDistPolMDS &operator=(const dbDistPolMDS &) ; // operator =

	static  std::string staticClassName() ;
	virtual dbPersistent *clone() const ; // PRQA S 2502

	// inherited from dbPersistent
	virtual std::string className() const ;

	virtual void exportObj(rsResourceSet &s) const ; // export the class to rsResourceSet
	virtual void importObj(rsResourceSet &s)  ; // import the class from a rsResourceSet
	virtual void exportXML(XMLOstream &xst) const ; // export the class to XML stream


	void setPolicy(IdType, dbConnection &) ;
	IdType getPolicy(bool &, dbConnection &) const ;
	void unsetPolicy();

	void setMeasDataSetId(int, dbConnection &) ;
	int getMeasDataSetId(bool  &, dbConnection &) const ;
	void unsetMeasDataSetId();

	void setCount(int, dbConnection &) ;
	int getCount(bool  &, dbConnection &) const ;
	void unsetCount();

	static std::string getFields() ;
	static std::string getTable() ;


protected:
	explicit dbDistPolMDS(dbConnection &) ;
	dbDistPolMDS(const dbDistPolMDS &) ;

private:
    dbDistPolMDS() ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDistPolMDS)
} ;


_ACS_END_NAMESPACE

#endif // _dbDistPolMDS_H_
