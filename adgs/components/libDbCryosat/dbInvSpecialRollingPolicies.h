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
	
	Revision 5.5  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:56  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:24  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.5  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.4  2012/06/15 09:54:29  marpas
	removing warning related to virtual method hiding
	
	Revision 2.3  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.1  2008/09/12 17:24:03  ivafam
	Added storageid field
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.3  2004/09/07 09:10:16  ivafam
	_expitationDay --> _expirationDate
	
	Revision 1.2  2004/09/07 09:02:17  ivafam
	_fileName    --> _inventoryName
	_fileVersion --> _inventoryVersion
	
	Revision 1.1  2004/09/03 15:29:49  ivafam
	Added new classes
	

*/


#ifndef _dbInvSpecialRollingPolicies_H_
#define _dbInvSpecialRollingPolicies_H_

#include <exException.h>
#include <dbPersistent.h>
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
// class dbInvSpecialRollingPolicies
//
//
//
class dbInvSpecialRollingPolicies :
	public dbPersistent
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	virtual ~dbInvSpecialRollingPolicies() throw() ;
	dbInvSpecialRollingPolicies &operator=(const dbInvSpecialRollingPolicies &) ; // operator =
	
	virtual dbPersistent *clone() const ; // PRQA S 2502
	
	static  std::string staticClassName() ;

	// inherited from dbPersistent
	virtual std::string className() const ;
	virtual void reset() ; // initialize the class
	virtual IdType save(dbConnection &)  ; // save the data in database
	virtual bool load(dbConnection & , IdType id=0)  ; // load the class from the database. Return true if obect exist
	virtual IdType update(dbConnection &)  ; // update an existing instance in database
	virtual IdType remove(dbConnection &)  ; // remove the class from the database // PRQA S 2502
	virtual bool validate() const  ; // True if the class have valid parameters, false otherwise
	virtual void exportObj(rsResourceSet &s) const ; // export the class to rsResourceSet
	virtual void importObj(rsResourceSet &s)  ; // import the class from a rsResourceSet
	virtual void exportXML(XMLOstream &xst) const ; // export the class to XML stream

	int loadRow(const dbRow &, int offset=0) ;
	
	void setInventoryName(const std::string &) ;
	const std::string &getInventoryName(bool *set=0) const ;
	void unsetInventoryName();

	void setInventoryVersion(const std::string &) ;
	const std::string &getInventoryVersion(bool *set=0) const ;
	void unsetInventoryVersion();

	void setExpirationDate(const DateTime &) ;
	const DateTime & getExpirationDate(bool *set=0) const ;
	void unsetExpirationDate();

	void setAction(unsigned int) ;
	unsigned int getAction(bool *set=0) const ;
	void unsetAction();
	
	void setStorage(unsigned int) ;
	unsigned int getStorage(bool *set=0) const ;
	void unsetStorage();

	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;

protected:
	dbInvSpecialRollingPolicies() ;
	dbInvSpecialRollingPolicies(const dbInvSpecialRollingPolicies &) ;


private:
	
	std::string _inventoryName ;
	bool _inventoryNameSet ;
	
	std::string _inventoryVersion ;
	bool _inventoryVersionSet ;

	DateTime _expirationDate ;
	bool _expirationDateSet ;

	unsigned int _action ;
	bool _actionSet ;
	
	unsigned int _storage ;
	bool _storageSet ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInvSpecialRollingPolicies)
} ;


_ACS_END_NAMESPACE

#endif // _dbInvSpecialRollingPolicies_H_
