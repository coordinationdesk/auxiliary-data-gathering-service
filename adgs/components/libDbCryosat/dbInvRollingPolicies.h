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
	
	Revision 5.5  2016/05/05 13:13:25  marpas
	improving maintenance adopting dbPersistent convenience macros
	coding best practice application in progress
	
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
	
	Revision 1.10  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.9  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.8  2004/09/30 09:30:59  ivafam
	unsetParametersFileName method added and bugs fixed
	
	Revision 1.7  2004/09/23 10:43:14  ivafam
	LatencyDays--->LatencyHours
	
	Revision 1.6  2004/09/22 13:06:02  ivafam
	Added FileClass attribute
	
	Revision 1.5  2004/09/08 16:30:49  ivafam
	I/F changed
	
	Revision 1.4  2004/09/07 15:37:07  ivafam
	Work in progress
	
	Revision 1.3  2004/09/06 16:13:04  ivafam
	Work in progress
	
	Revision 1.2  2004/09/03 15:29:27  ivafam
	Work in progress
	

*/


#ifndef _dbInvRollingPolicies_H_
#define _dbInvRollingPolicies_H_

#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbCryosatFactory ;

class dbRow ;
class rsResourceSet ;
class XMLOstream ;

//
//
//
// class dbInvRollingPolicies
//
//
//
class dbInvRollingPolicies :
	public dbPersistent
{
public:
	friend class dbCryosatFactory ;

	virtual ~dbInvRollingPolicies() throw() ;
	dbInvRollingPolicies &operator=(const dbInvRollingPolicies &) ; // operator =
	
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
	
	// replace the file of an existing record
	void replaceParametersFile(const std::string &, dbConnection &) ;
	// save the file on disk
	void saveParametersFile(const std::string &, dbConnection &) const ;

	// This method return a hint for the filename
	std::string getParametersFileName() const ;
	void setParametersFileName(const std::string &) ;
	void unsetParametersFileName();
	
	
	void setFileType(const std::string &) ;
	const std::string &getFileType(bool *set=0) const ;
	void unsetFileType();

	void setLatencyHours(int) ;
	int getLatencyHours(bool *set=0) const ;
	void unsetLatencyHours();

	void setRemoveEvenDistribution(bool) ;
	bool getRemoveEvenDistribution(bool *set=0) const ;
	void unsetRemoveEvenDistribution();

	void setRemoveEvenUsed (bool) ;
	bool getRemoveEvenUsed (bool *set=0) const ;
	void unsetRemoveEvenUsed ();

	void setAction(unsigned int) ;
	unsigned int getAction(bool *set=0) const ;
	void unsetAction();

	void setAlgorithm(unsigned int) ;
	unsigned int getAlgorithm(bool *set=0) const ;
	void unsetAlgorithm();
	
	void setPriority(int) ;
	int getPriority(bool *set=0) const ;
	void unsetPriority();
	
	void setFileClass(const std::string &) ;
	const std::string &getFileClass(bool *set=0) const ;
	void unsetFileClass();
	
	void setStorage(unsigned int) ;
	unsigned int getStorage(bool *set=0) const ;
	void unsetStorage();

	
	bool hasParameters() const;
	void setParameters(bool );

	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;	

protected:
	dbInvRollingPolicies() ;
	dbInvRollingPolicies(const dbInvRollingPolicies &) ;


private:
	
	std::string _fileType ;
	bool _fileTypeSet ;

	int _latencyHours ;
	bool _latencyHoursSet ;

	bool _removeEvenDistribution ;
	bool _removeEvenDistributionSet ;
	
	bool _removeEvenUsed ;
	bool _removeEvenUsedSet ;

	unsigned int _action ;
	bool _actionSet ;

	unsigned int _algorithm ;
	bool _algorithmSet ;
	
	int _priority ;
	bool _prioritySet ;
	
	std::string _fileClass ;
	bool _fileClassSet ;

	
	std::string _parametersFileName ;
	bool	    _parametersFileNameSet ;
	bool 	    _parameters ;
	
	unsigned int _storage ;
	bool _storageSet ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInvRollingPolicies)
} ;


_ACS_END_NAMESPACE


#endif // _dbInvRollingPolicies_H_
