// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/15 10:13:43  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
	Revision 5.1  2016/04/14 10:49:51  marpas
	implemented as singleton
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/10/31 17:07:17  marpas
	UniqueNameFormatter concept added
	getReportUniqueNameFormatter method added
	this allow a report to be generated using a unique name.
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:23  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.8  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.7  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.6  2012/11/16 13:01:13  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.5  2012/03/08 15:21:46  marpas
	refactoring
	
	Revision 2.4  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.2  2009/04/08 10:08:19  marpas
	system concept introduced in order to have a writer for a system with no satellite
	
	Revision 2.1  2006/04/12 15:33:09  paoscu
	Tread safety added.
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/09/08 17:46:06  paoscu
	dbFixedHeaderFactoryChain bug fixed. Now ignoreCase for SatelliteName is implemented
	CryosatFixedHeaderFactory minor bug fixed when returning satelliteId
	
	Revision 1.2  2004/05/05 18:23:38  paoscu
	Work in progress.
	
	Revision 1.1  2004/05/03 14:54:56  paoscu
	New header writers .
	

*/

#ifndef _dbFixedHeaderFactoryChain_H_
#define _dbFixedHeaderFactoryChain_H_


#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <map>
#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


class dbFixedHeaderWriter ;
class dbInventoryObject;
class XMLOstream;


// abstract class
class dbFixedHeaderFactory { // PRQA S 2109

public:

    class UniqueNameFormatter {
    public:
        UniqueNameFormatter() {}
	    virtual ~UniqueNameFormatter() throw() {} // PRQA S 2131
        UniqueNameFormatter(const UniqueNameFormatter &) {}
        UniqueNameFormatter& operator= (const UniqueNameFormatter &) { return *this ; } // PRQA S 4072
        virtual std::string next(const std::string &previous) { return previous ; } // PRQA S 2131
        virtual void reset() {}  // PRQA S 2131
    } ;

	virtual ~dbFixedHeaderFactory() throw() ; 
	const std::string &name() const ;
	const std::string &system() const ; // PRQA S 2502

	/**
	 * allocates a dbFixedHeaderWriter ... its destruction is left to the caller.
	 */
	virtual void getWriter(dbInventoryObject const &, std::string const &system, XMLOstream &, dbFixedHeaderWriter *&) const = 0 ;
	virtual std::string getFilename(dbInventoryObject const &) const = 0 ;
	virtual int getSatelliteId() const =0 ;
    virtual UniqueNameFormatter &getReportUniqueNameFormatter() const ;
	
protected:
	explicit dbFixedHeaderFactory(const std::string &name, const std::string &system="") ;
	
private:
	dbFixedHeaderFactory() ; // not implemented
	dbFixedHeaderFactory(const dbFixedHeaderFactory & ) ; // not implemented
	dbFixedHeaderFactory &operator=(const dbFixedHeaderFactory & ) ; // not implemented
	
private:
	std::string _name ;
	std::string _system ;
    mutable UniqueNameFormatter repunamefmt_ ;
} ;

using pattern::Singleton ;

class dbFixedHeaderFactoryChain : public Singleton <dbFixedHeaderFactoryChain> // PRQA S 2109, 2153
{
    friend dbFixedHeaderFactoryChain* Singleton<dbFixedHeaderFactoryChain>::instance(); // PRQA S 2107
public:
    size_t getFactoryNames(std::vector < std::string >  &) ;
    size_t getFactorySystems(std::vector < std::string >  &) ;

	void registerFactory(const dbFixedHeaderFactory &) ;
	void unregisterFactory(const dbFixedHeaderFactory &) ;
	const dbFixedHeaderFactory *getFactoryByName(const std::string &, bool ignoreCase = true) ;
	const dbFixedHeaderFactory *getFactoryBySystem(const std::string &, bool ignoreCase = true) ;

	virtual ~dbFixedHeaderFactoryChain() throw() {} // PRQA S 2131
private:
	dbFixedHeaderFactoryChain() :
        Singleton <dbFixedHeaderFactoryChain>(), 
        _nameFactories(), 
        _systemFactories() 
    {}
	dbFixedHeaderFactoryChain(const dbFixedHeaderFactoryChain & ) ; // not implemented
	dbFixedHeaderFactoryChain &operator=(const dbFixedHeaderFactoryChain & ) ; // not implemented

private:

	std::map<std::string,const dbFixedHeaderFactory *> _nameFactories;
	std::map<std::string,const dbFixedHeaderFactory *> _systemFactories ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbFixedHeaderFactoryChain) ;

};


_ACS_END_NAMESPACE


#endif // _dbFixedHeaderFactoryChain_H_
