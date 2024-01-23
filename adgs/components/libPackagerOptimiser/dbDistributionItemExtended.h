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

	$Prod: A.C.S. Packager Optimiser Library $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2014/06/25 16:24:12  marpas
	coding best practices applied
	
	Revision 2.6  2014/03/10 15:00:33  francesco.avanzi
	better code practices applied
	
	Revision 2.5  2013/04/17 17:13:23  marpas
	new dbPersistent interface
	
	Revision 2.4  2013/03/29 19:52:07  marpas
	removing html related functions
	
	Revision 2.3  2012/02/13 17:10:55  marpas
	refactoring in progress
	
	Revision 2.2  2008/01/30 16:12:40  enrcar
	EC:: Extended to handle also T_Inventory.filetype and T_Inventory.qualifierfile
	
	Revision 2.1  2006/10/23 10:07:19  marpas
	variables and/or methods related to file size, header size and blob sized now take into account 64 bits
	
	Revision 2.0  2006/02/28 10:05:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/07/19 16:11:21  enrcar
	DBEXPRESS handled
	
	Revision 1.1  2004/09/14 16:28:37  marpas
	object extended to limit query number
	

*/

#ifndef _dbDistributionItemExtended_H_
#define _dbDistributionItemExtended_H_

#include <dbDistributionItem.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbQueryParameters ;


//
//
//
// class dbDistributionItemExtended
//
//
//
class dbDistributionItemExtended : // PRQA S 2109, 2153
	public dbDistributionItem
{

public:
	friend class dbPackagerOptimiserFactory ;  // PRQA S 2107

	virtual std::string className() const ;
	static  std::string staticClassName() ;
	virtual dbPersistent* clone() const ; // PRQA S 2502

	virtual ~dbDistributionItemExtended() throw() ;
	dbDistributionItemExtended &operator=(const dbDistributionItemExtended &) ; // operator =



	// inherited from dbPersistent
	virtual void reset() ; // initialize the class

	virtual bool load(dbConnection & , IdType id=0)  ; // load the class from the database. Return true if obect exist

	int loadRow(const dbRow &, int offset=0) ; // PRQA S 2121

	static std::string getWhere() ;
	static dbQueryParameters whereJoin(const dbConnection &) ;

	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	bool getValidityFlag(bool *set=0) const ;
	unsigned long long int getDataSize(bool *set=0) const ;
	unsigned long long int getHeaderSize(bool *set=0) const ;
	const std::string &getFileType(bool *set=0) const ;
	const std::string &getQualifierFile(bool *set=0) const ;

protected:
	dbDistributionItemExtended() ;
	dbDistributionItemExtended(const dbDistributionItemExtended &) ;


private:
	int privateLoadRow(const dbRow &, int offset=0) ; // PRQA S 2121
	static std::string privateGetFields() ;
	static std::string privateGetOrderBy() ;


private:
	bool _validityFlag ;
	bool _validityFlagSet ;

	unsigned long long int _dataSize ;
	bool _dataSizeSet ;

	unsigned long long int _headerSize ;
	bool _headerSizeSet ;

	std::string _fileType ;
	bool _fileTypeSet ;
	
    std::string _qualifierFile ;
	bool _qualifierFileSet ;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDistributionItemExtended) ;
} ;


_ACS_END_NAMESPACE

#endif // _dbDistributionItemExtended_H_
