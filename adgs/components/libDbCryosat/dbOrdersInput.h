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
	
	Revision 5.5  2016/05/16 15:03:57  marpas
	fixing potential bug when orderid will be very large
	coding best practice application in progress
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:02  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:30  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.4  2013/03/29 19:51:30  marpas
	removing html related functions
	
	Revision 2.3  2012/06/15 09:54:30  marpas
	removing warning related to virtual method hiding
	
	Revision 2.2  2012/02/14 12:52:50  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2011/03/02 18:47:15  marpas
	serial 8 support
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/17 10:31:55  marpas
	std namespaces took into consideration
	
	Revision 1.1  2004/09/24 12:21:14  ivafam
	Added dbOrdersInput class
	
	
*/


#ifndef _dbOrdersInput_H_
#define _dbOrdersInput_H_

#include <exException.h>
#include <dbPersistent.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbCryosatFactory ;

class dbRow ;
class rsResourceSet ;
class XMLOstream ;

//
//
//
// class dbOrdersInput
//
//
//
class dbOrdersInput :
	public dbPersistent
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	virtual ~dbOrdersInput() throw() ; 
	dbOrdersInput &operator=(const dbOrdersInput &) ; // operator =
	
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
	
	void setFileName(const std::string &) ;
	const std::string &getFileName(bool *set=0) const ;
	void unsetFileName();

	void setFileVersion(const std::string &) ;
	const std::string &getFileVersion(bool *set=0) const ;
	void unsetFileVersion();

	void setOrderId(IdType) ;
	IdType getOrderId(bool *set=0) const ;
	void unsetOrderId();

	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;

protected:
	dbOrdersInput() ;
	dbOrdersInput(const dbOrdersInput &) ;


private:
	
	std::string _fileName ;
	bool _fileNameSet ;
	
	std::string _fileVersion ;
	bool _fileVersionSet ;
	
	IdType _orderId ;
	bool _orderIdSet ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbOrdersInput)
} ;


_ACS_END_NAMESPACE

#endif // _dbOrdersInput_H_
