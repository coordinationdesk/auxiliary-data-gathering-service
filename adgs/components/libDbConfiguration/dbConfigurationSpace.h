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
	Revision 5.2  2016/04/11 13:27:13  marpas
	adopting new db interfaces
	
	Revision 5.1  2016/03/26 21:42:59  marpas
	using new db I/F
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.6  2013/03/29 19:51:19  marpas
	removing html related functions
	
	Revision 2.5  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.4  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.3  2012/06/15 09:59:53  marpas
	fixing inherited methods
	
	Revision 2.2  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.1  2011/03/02 18:23:44  marpas
	serial 8 support
	
	Revision 2.0  2006/02/28 09:09:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/07/21 10:24:43  marpas
	dbConfigurationSpace and its query imported from libDbCryosat
	
	Revision 1.19  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.18  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.17  2004/05/03 14:55:33  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.16  2004/04/13 16:57:08  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.15  2004/03/04 18:54:08  paoscu
	Class generated again. New clob management.
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbConfigurationSpace_H_
#define _dbConfigurationSpace_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbPersistent.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbRow ;
class rsResourceSet ;
class XMLOstream ;

//
//
//
// class dbConfigurationSpace
//
//
//
class dbConfigurationSpace :
	public dbPersistent
{
public:
	friend class dbResourceSpaceFactory ; // PRQA S 2107

	virtual ~dbConfigurationSpace() throw() ;
	dbConfigurationSpace &operator=(const dbConfigurationSpace &) ; // operator =

	static  std::string staticClassName() ;
	virtual dbPersistent *clone() const ; // PRQA S 2502

	// inherited from dbPersistent
	virtual std::string className() const ;
	virtual void reset() ; // initialize the class
	virtual dbPersistent::IdType save(dbConnection &)  ; // save the data in database
	virtual bool load(dbConnection & , IdType id=0)  ; // load the class from the database. Return true if obect exist
	virtual dbPersistent::IdType update(dbConnection &)  ; // update an existing instance in database
	virtual dbPersistent::IdType remove(dbConnection &)  ; // remove the class from the database // PRQA S 2502
	virtual bool validate() const  ; // True if the class have valid parameters, false otherwise
	virtual void exportObj(rsResourceSet &s) const ; // export the class to rsResourceSet
	virtual void importObj(rsResourceSet &s)  ; // import the class from a rsResourceSet
	virtual void exportXML(XMLOstream &xst) const ; // export the class to XML stream

	int loadRow(const dbRow &, int offset=0) ;

	void setNamespace(const std::string &) ;
	const std::string &getNamespace(bool *set=0) const ;
	void unsetNamespace();

	void setVersion(const std::string &) ;
	const std::string &getVersion(bool *set=0) const ;
	void unsetVersion();

	// replace the file of an existing record
	void replaceSpaceFile(const std::string &path, dbConnection &) ;
	// save the file on disk
	void saveSpaceFile(const std::string &path, const std::string &filename, dbConnection &) ;

	// This method return a hint for the filename
	std::string getSpaceFileName() const ;
	// set/get the original file name
	void setSpaceUploadFileName(const std::string &) ;
	std::string getSpaceUploadFileName(bool *set=0) const;

	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;


protected:
	dbConfigurationSpace() ;
	dbConfigurationSpace(const dbConfigurationSpace &) ;


private:
	std::string _namespace ;
	bool _namespaceSet ;

	std::string _version ;
	bool _versionSet ;

	std::string _spaceUploadFileName ;
	bool _spaceUploadFileNameSet ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConfigurationSpace) ;
} ;


_ACS_END_NAMESPACE

#endif // _dbConfigurationSpace_H_