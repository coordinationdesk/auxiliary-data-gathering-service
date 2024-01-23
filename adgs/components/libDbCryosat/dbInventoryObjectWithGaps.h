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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.6  2016/07/04 15:09:16  damdec
	CSGACQ-82: new getFields interface adopted.
	
	Revision 5.5  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.4  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2015/02/20 16:23:53  marfav
	Fixing export object in rs virtual operation signature
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:59  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.2  2013/09/26 19:32:52  marpas
	qa rules
	coding best practices
	compilation warnings removed
	using libException 5.x interface
	
	Revision 3.1  2013/09/24 17:36:10  marpas
	introducing libException 5.x interface
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.5  2012/06/15 09:54:30  marpas
	removing warning related to virtual method hiding
	
	Revision 2.4  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.2  2006/04/28 14:28:36  marpas
	loadComments & loadGaps now have a defaulted connection parameter to allow reloading while transaction is active
	
	Revision 2.1  2006/04/28 14:14:57  marpas
	Gaps & Comments amanagement improved.
	adding those elements to an inventory does not require this has been previously saved into the database.
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/20 10:36:11  marpas
	gaps management added
	 
		

*/


#ifndef _dbInventoryObjectWithGaps_H_
#define _dbInventoryObjectWithGaps_H_

#include <acs_c++config.hpp>
#include <dbInventoryObject.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)


class dbCryosatFactory;
class dbInventoryGapQuery;
class dbInventoryGap;


//
//
//
// class dbInventoryObjectWithGaps
//
//
//
class dbInventoryObjectWithGaps : // PRQA S 2109, 2153
	public dbInventoryObject
{
	friend class dbCryosatFactory; // PRQA S 2107
public:

	exDECLARE_EXCEPTION(dbInventoryObjectWithGapsException,exException) ; // PRQA S 2131, 2153, 2502

	virtual ~dbInventoryObjectWithGaps() throw() ;
	dbInventoryObjectWithGaps &operator=(const dbInventoryObjectWithGaps &) ; // operator =

	// inherited from dbPersistent
	virtual dbPersistent *clone() const ; // PRQA S 2502
	virtual std::string className() const ;
	static  std::string staticClassName() ;
	virtual void reset() ; // initialize the class
	virtual IdType save(dbConnection &)  ; // save the data in database
	virtual bool load(dbConnection &, IdType id=0)  ; // load the class from the database. Return true if obect exist
	virtual IdType remove(dbConnection &)  ; // remove the class from the database // PRQA S 2502

	virtual void exportObj(rsResourceSet &s)  const ; // export the class to stream
	virtual void importObj(rsResourceSet &s)  ; // import the class from a stream
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
	virtual void exportXML(XMLOstream &xst)  ; // export the class to XML stream
#pragma GCC diagnostic pop

	int loadRow(dbConnection &, const dbRow &, int offset=0) ;

	static std::string getFields() ;
	static std::string getFields(dbConnection&) ;
	static std::deque<std::string> getFieldArray(dbConnection & conn) ;


	// Methods used to access the gaps
	unsigned long	gapsNum(dbConnection &)	;
	dbInventoryGap * gap(unsigned long ind, dbConnection &) ; 
	void addGap(const DateTime &begin, const DateTime &end, dbConnection &) ; 


	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;


protected:
	dbInventoryObjectWithGaps() ;
	dbInventoryObjectWithGaps(const dbInventoryObjectWithGaps &) ;

private:

	unsigned long loadGaps(dbConnection &) ; 
	void saveGaps(dbConnection &) ;
	void deleteGaps2Save() throw() ;
	
private:
	
	bool					_gapsNumSet ; 
	int						_gapsNum ; 
	dbInventoryGapQuery		*_gaps ; 
	bool					_gapsLoaded ; 
	std::vector<dbInventoryGap *> _gaps2Save ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObjectWithGaps);
} ;


_ACS_END_NAMESPACE

#endif // _dbInventoryObjectWithGaps_H_
