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
	Revision 6.1  2016/07/27 08:26:25  damdec
	PDS_OLD_SCHEMA flag removed. Code updated: field Format is no mandatory.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/04/19 13:53:46  marpas
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
	
	Revision 4.0  2013/09/30 12:30:54  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:22  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.9  2013/03/29 19:51:28  marpas
	removing html related functions
	
	Revision 2.8  2013/02/05 10:24:45  marfav
	PackagingAlgoId field management added
	
	Revision 2.7  2012/12/21 12:33:15  marpas
	PDS_OLD_SCHEMA unchanged, new distributionrule_id mandatory field management added
	
	Revision 2.6  2012/11/16 13:01:13  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.4  2012/06/15 09:54:29  marpas
	removing warning related to virtual method hiding
	
	Revision 2.3  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2011/03/02 18:47:13  marpas
	serial 8 support
	
	Revision 2.1  2009/03/24 10:00:33  crivig
	cartid, subreq new fields handled
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.19  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.18  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.17  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.16  2004/04/19 13:38:43  paoscu
	Field "format" added.
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbDistributionItem_H_
#define _dbDistributionItem_H_

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
// class dbDistributionItem
//
//
//
class dbDistributionItem : // PRQA S 2109, 2153
	public dbPersistent
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	virtual ~dbDistributionItem() throw() ;
	dbDistributionItem &operator=(const dbDistributionItem &) ; // operator =

	static  std::string staticClassName() ;

	// inherited from dbPersistent
	virtual dbPersistent* clone() const ; // PRQA S 2502
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

	static std::string getWhere() ;
	static std::string getIdName() ;

	void setDueDate(const DateTime &) ;
	const DateTime &getDueDate(bool *set=0) const ;
	void unsetDueDate();

	void setDistributedDate(const DateTime &) ;
	const DateTime &getDistributedDate(bool *set=0) const ;
	void unsetDistributedDate();

	void setCreationDate(const DateTime &) ;
	const DateTime &getCreationDate(bool *set=0) const ;
	void unsetCreationDate();

	void setPackageId(dbPersistent::IdType) ;
	dbPersistent::IdType getPackageId(bool *set=0) const ;
	void unsetPackageId();

	void setFileName(const std::string &) ;
	const std::string &getFileName(bool *set=0) const ;
	void unsetFileName();

	void setFileVersion(const std::string &) ;
	const std::string &getFileVersion(bool *set=0) const ;
	void unsetFileVersion();

	void setMediaType(const std::string &) ;
	const std::string &getMediaType(bool *set=0) const ;
	void unsetMediaType();

	void setUserId(const std::string &) ;
	const std::string &getUserId(bool *set=0) const ;
	void unsetUserId();

	void setFormat(const std::string &) ;
	const std::string &getFormat(bool *set=0) const ;
	void unsetFormat();

	void setCartId(IdType) ;
	dbPersistent::IdType getCartId(bool *set=0) const ;
	void unsetCartId();

	void setSubReq(int) ;
	int getSubReq(bool *set=0) const ;
	void unsetSubReq();

	void setDistributionRuleId(IdType) ;
	dbPersistent::IdType getDistributionRuleId(bool *set=0) const ;
	void unsetDistributionRuleId();

	void setDisseminationPackingAlgoId (IdType) ;
	dbPersistent::IdType getDisseminationPackingAlgoId (bool *set =0) const ;
	void unsetDisseminationPackingAlgoId () ;

	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;


protected:
	dbDistributionItem() ;
	dbDistributionItem(const dbDistributionItem &) ;


private:
	int privateLoadRow(const dbRow &, int offset=0) ;
	static std::string privateGetFields() ;
	static std::string privateGetOrderBy() ;


private:
	bool _idSet ;

	DateTime _dueDate ;
	bool _dueDateSet ;

	DateTime _distributedDate ;
	bool _distributedDateSet ;

	DateTime _creationDate ;
	bool _creationDateSet ;

	IdType _packageId ;
	bool _packageIdSet ;

	std::string _fileName ;
	bool _fileNameSet ;

	std::string _fileVersion ;
	bool _fileVersionSet ;

	std::string _mediaType ;
	bool _mediaTypeSet ;

	std::string _userId ;
	bool _userIdSet ;

	std::string _format ;
	bool _formatSet ;

	IdType _cartId ;
	bool _cartIdSet ;

	int _subReq ;
	bool _subReqSet ;
    
	IdType _distRuleId ;
	bool _distRuleIdSet ;

	IdType _dissPackAlgoId ;
	bool _dissPackAlgoIdSet ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDistributionItem)
} ;


_ACS_END_NAMESPACE

#endif // _dbDistributionItem_H_
