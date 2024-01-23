// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

*/

#ifndef _NameIdAssociator_H_
#define _NameIdAssociator_H_ 

#include <acs_c++config.hpp>
#include <ResourceSpaceFactory.h>
#include <dbQuery.h>
#include <dbStat.h>
#include <dbSet.h>
#include <dbConnectionPool.h>
#include <SQLString.h>
#include <File.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <RegExp.h>
#include <Statistics.h>
#include <fstream>
#include <string>
#include <map>

#include <Singleton.hpp>
#include <ThreadSafe.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * class NameIdAssociatorConfiguration
 * 
 * provides a singleton implementation to allow global NameIdAssociator configurations
 * 
 * list of attributes:
 * - enableDiskMap: bool enables or disables disk persisntency of map files
 **/ 

using pattern::Singleton;
 
class NameIdAssociatorConfiguration : public Singleton < NameIdAssociatorConfiguration >
{
protected:
	NameIdAssociatorConfiguration() ;

public:
	void setEnableDiskMaps (bool enable) ;
	bool getEnableDiskMaps () const;
	virtual ~NameIdAssociatorConfiguration () ACS_NOEXCEPT ;

private:
	Mutex _mutex;
	bool _enableDiskMaps;
	
	// friend declaration needed to use the singleton pattern
	friend NameIdAssociatorConfiguration* Singleton < NameIdAssociatorConfiguration >::instance();
};

//!
//!
//! class NameIdAssociator
//!
//! This is the base class that manage the Name<->Id association.
//! It provides the basic mechanism to load a specified table and is designed
//! to be implemented as a singleton pattern.
//! The column referred as Id is usually the primary key of the table, or at least it has the univocity property guaranteed
//! That column (Id) will be stored into the template T type.
//! The column referred as Name will be stored into a string.
//! The class is meant to be used to convert an Id to a Name and viceversa.
//!
//! The class saves is content in a file in order to load again it in case the DB should not respond. This behaviour guarantee that 
//! applications would not fail when converting satellite ids to satellite names providing that in the past the DB was reachable and the file was saved.
//! Different DB could have different tables, so files are mantained separated according the dbkey used to address the DB (dialect+schema)
template <class T>
class NameIdAssociator // PRQA S 2109
{
public:
	NameIdAssociator(const std::string &tbl, const std::string &pk, const std::string &col, const std::string &dbkey) :
	    _dbKey(dbkey),
        _tableName(tbl),
	    _pkey(pk),
	    _col(col),
	    _values(),
	    _oby(_pkey) 
    {
        statN(_dbKey)[_tableName][_pkey]["created #"] +=1 ;
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_dbKey: \"" << _dbKey << "\""
                << " _tableName: " << _tableName
                << " _pkey: " << _pkey
                << " _col: " << _col ) ;
   }

	virtual ~NameIdAssociator() throw() 
    {
    }


	//! returns the id associated with the given name using idExact
	//!
	//! an exception of type acs::exIllegalValueException is thrown if name is not found
	//! \param name : the name whose id shall be found and returned
	//! \param caseInsensitive : if true the case will not be used to find the name
	//!
	const T &id(const std::string &name, bool caseInsensitive=true) const // PRQA S 4020, 4214
	{
		return idExact (name, caseInsensitive);
	}

	//! returns the id associated with the given name using the StringUtils::sanitizeDbName method applied to both name and name entries.
	//!
	//! an exception of type acs::exIllegalValueException is thrown if name is not found
	//! \param name : the name whose id shall be found and returned
	//! \param caseInsensitive : if true the case will not be used to find the name
	//!
	const T &idSanitized (const std::string &name, bool caseInsensitive=true) const // PRQA S 4020, 4214
	{
		bool sanitize = true;
		return idInternal(name, sanitize, caseInsensitive);
	}
	//! returns the id associated with the given name 
	//! an exception of type exIllegalValueException is thrown if name is not found
	//! \param name : the name whose id shall be found and returned - not sanitized \see NameIdAssociator::id
	//! \param caseInsensitive : if true the case will not be used to find the name
	const T &idExact(const std::string &name, bool caseInsensitive=true) const // PRQA S 4020, 4214
	{
		bool sanitize = false;
		return idInternal(name, sanitize, caseInsensitive);
	}
 
    //! returns the name associated with the given id
    //! an exception of type acs::exBoundsException is thrown if id is not found
    const std::string &name(const T &id) const // PRQA S 2133, 2134, 4214
    {
	    typename std::map<T,std::string>::const_iterator iter = _values.find(id);
	    if (iter == _values.end() )
	    {
            statN(_dbKey)[_tableName][_pkey]["name #"]["error"] +=1 ;
		    std::ostringstream os ; 
		    os << " ID: " << id << " not found on table: " << _tableName ; 
		    ACS_THROW(exBoundsException(os.str())) ;  // PRQA S 3081
	    }
	    return iter->second ; 
    } 
 
	//! \brief numbers of row loaded
	unsigned int size() const throw() { return _values.size(); }



	//! \brief the table name 
	const std::string &tableName() const throw() { return _tableName ; }

	//! \brief the column used as Id 
	const std::string &pkeyName() const throw() { return _pkey ; }
 	
	//! \brief the column used as Name 
	const std::string &colName() const throw() {  return _col ; }

	//! returns the i-th name
    //! \param idx : the index - acs::exBoundsException will be thrown in case is out of bound
	const std::string &operator[](unsigned int idx) const  // PRQA S 2141, 2133, 2134
    {
	    if (idx >= size()) {
		    std::ostringstream os ;
		    os << "Cannot address element # " << idx << " in a collection of " << size() << " elements (table:  " << _tableName << ")";
		    ACS_THROW(exBoundsException(os.str())) ;  // PRQA S 3081
	    }

	    typename std::map<T,std::string>::const_iterator i=_values.begin() ;
        std::advance(i, idx) ;
	    return i->second ; 

    }

	//! returns the i-th id value
    //! \param idx : the index - acs::exBoundsException will be thrown in case is out of bound
	const T &idByPos(unsigned int idx) const // PRQA S 2133, 2134
    {
	    if (idx >= size()) {
		    std::ostringstream os ;
		    os << "Cannot address element # " << idx << " in a collection of " << size() << " elements (table: " << _tableName << ")" ;
		    ACS_THROW(exBoundsException(os.str())) ;  // PRQA S 3081
	    }

	    typename std::map<T,std::string>::const_iterator i=_values.begin() ;
        std::advance(i, idx) ;
	    return i->first ; 

    }

	void load()  // PRQA S 2133, 2134
    {
        statN(_dbKey)[_tableName][_pkey]["load #"] +=1 ;
	    if(_tableName.empty() || _pkey.empty() || _col.empty())
	    {
		    ACS_THROW(exIllegalValueException("NameIdAssociator::load error: invalid mandatory field (table,pkey,col)")) ;  // PRQA S 3081
	    }

	    try {
	        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_dbKey: \"" << _dbKey << "\"") ;

            dbConnectionWrapper dbcw(db::ConnPools::instance()->getPool(_dbKey)) ;
            dbConnection &theConn = dbcw ; // PRQA S 3050 
	        //WAS: string sqlString = "SELECT " + _pkey +", " + _col + " FROM " + _tableName + " ORDER BY " + _pkey ;
    	    SQLString sqlString( theConn.getKey(), SQLString::SELECT ) ;
    	    sqlString.addSelectList(_pkey); sqlString.addSelectList(_col);
    	    sqlString.addTableList(_tableName);
    	    sqlString.addOrderByList(_oby);

		    dbQuery query(theConn,sqlString, "NISEL " + _tableName +"|" + _pkey) ;
	        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "sqlString: \"" << sqlString.getSQL()) ;

		    dbSet set ; 
		    query.execForSet(set) ;
		    dbRow row(theConn.getKey()) ; 
            int nulls = 0 ;
            std::map<T,std::string> new_values;

		    while(set.getNextRow(row))
		    {
                bool isNull = false ;

			    T v ; // PRQA S 4101
			    row.column(0,v, &isNull) ;
                if (isNull) {
                    ACS_LOG_ERROR("Null value in lookup table \"" << _tableName <<'.'<<_pkey << " ignoring the row") ;
                    continue ;
                }   

                isNull = false ;
			    std::string tmp ; 
			    row.column(1,tmp, &isNull) ;
                if (isNull) { 
                    nulls++ ;
                    ACS_LOG_ERROR("Null value in lookup \"" << _tableName<< '.'<<_col << "\" for \"" << v << "\" in col " << _pkey << " keep going but results could be unpredictable")
                    if (nulls < 2) { // PRQA S 4400
			            new_values[v] = StringUtils::trim(tmp, StringUtils::is_not_space) ;
                    }
                    else {
                        ACS_LOG_ERROR("Ignoring \"" << _tableName <<'.'<<_pkey << "[" << v << "] value")
                    }
                }
                else {
                    new_values[v] = StringUtils::trim(tmp, StringUtils::is_not_space) ;
                }
		    }
            
            // here below the magic
            _values.swap(new_values) ; 
            
			// Save the MAP on disk only if allowed by configuration
			if ( NameIdAssociatorConfiguration::instance()->getEnableDiskMaps() )
			{
				saveMap() ;
			}
			if (ACS_CLASS_GET_DEBUG() == 135) { // PRQA S 4400
				ACS_THROW(exIllegalValueException("SimulatedException")) ; // PRQA S 3081
			}
		}
		catch(std::exception &x) {
			if ( NameIdAssociatorConfiguration::instance()->getEnableDiskMaps() )
			{
				// Try to recover only if allowed by configuration
				ACS_LOG_NOTIFY_EX(x) ;
				ACS_LOG_WARNING("Trying to recover loading from disk (if any and/or applicable)") ;
				loadMap() ;
				ACS_LOG_INFO("Recover succesful - please try to fix the above error ASAP !") ;
			}
			else
			{
				throw;
			}
		}
    }

	void orderBy(const std::string &oby) { _oby = oby ; }

private:
	NameIdAssociator() ;    // not implemented
	NameIdAssociator(const NameIdAssociator &) ;    // not implemented
	NameIdAssociator &operator=(const NameIdAssociator &) ;     // not implemented

    static math::StatNode &statN(const std::string &key) { return db::stat(key)["NameIdAssociator"] ; }

    std::string getMapPath() const // PRQA S 4214
    {
        std::string path = ResourceSpaceFactory::getConfigurationRepository() + "/" ;
        path += _dbKey + "_" + 
                StringUtils::lowercase(_tableName) + "_" + 
                StringUtils::lowercase(_pkey) + "_" + 
                StringUtils::lowercase(_col) + ".map" ;
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "path: " << path) ;
        return path ;

    }


	void saveMap() const throw() // PRQA S 2133, 2134, 4214
    {
	    try {
		    std::string path = getMapPath() ;

		    File::Lock lock(path) ;
		    File::Lock::tryLock(lock) ;// will try 10 times with 1 second timeout, then File::Lock::IOException

		    std::ofstream out(path.c_str()) ;
		    try {
			    out.exceptions (std::ios::badbit | std::ios::failbit) ;
			    typename std::map<T,std::string>::const_iterator i;
			    for(i=_values.begin(); i != _values.end() ; i++)
			    {
				    out << i-> first << "\n" ;
				    out << i-> second << "\n" ;
			    }

			    // change permissions allowing other users to override this file later.
			    try
			    {
				    File::chmod(path, 0664) ; // PRQA S 4400
			    }
			    catch (std::exception &ex1) {
				    // Do nothing...  this could happen and it is not a problem
			    }
		    }
		    catch(std::exception &e) {
			    // Even in case of troubles... try to change the permissions of the file
			    try
			    {
				    File::chmod(path, 0664) ; // PRQA S 4400
			    }
			    catch (std::exception &ex1) {
				    // Do nothing...  this could happen and it is not a problem
			    }

			    ACS_THROW(exIOException(e,"Writing file: \"" + path + "\"")) ;
		    }
	    }
	    catch(std::exception &x) {
		    // cannot rethrow, just notify
		    ACS_LOG_NOTIFY_EX(x) ;
	    }
    }
    
    //! internal method to load an Id of generic type T from a string
    //! this method is used by the loadMap method
    void loadId(T &id, const std::string &line)
    {
        std::istringstream ins1(line) ;
        ins1 >> id ;
    }

#ifdef TEST_NAMEIDASSOCIATOR
public:
#endif
	void loadMap() // PRQA S 2133, 2134
    {
        statN(_dbKey)[_tableName][_pkey]["loadMap #"] +=1 ;
        std::string path = getMapPath() ;
	    ACS_COND_THROW(not File::exists(path), // PRQA S 3081
		    exIOException(std::string("File \"") + path +"\" does not exists")) ;
	    try {
		    ACS_LOG_INFO("Loading map from file: \"" << path << "\"") ;
            File::Lock lock(path) ;
		    File::Lock::tryLock(lock) ;// will try 10 times with 1 second timeout, then File::Lock::IOException

		    std::ifstream in(path.c_str()) ;
		    in.exceptions (std::ios::badbit) ;
            int nulls = 0 ;
		    for(;;)
		    {
			    if (in.eof()) {
				    break ;
                }
			    char line[4096] ; // PRQA S 4403
			    in.getline(line,sizeof(line), '\n') ;
			    T first ; // PRQA S 4101
                loadId(first,line) ;
			    if (in.eof()) {
				    break ;
                }
			    in.getline(line,sizeof(line), '\n') ;
			    std::string second = line ;
                if (second.empty()) {
                    ACS_LOG_ERROR("Null value from map file  \"" << path << "\" corresponding to \"" << first << "\" keep going but results could be unpredictable")
			        nulls++ ;
                    if (nulls < 2) { // PRQA S 4400
                        _values[first] = second ;
                    }
                    else {
                        ACS_LOG_ERROR("\"" << first << "\" is ignored for file " << path)
                    }
                }
                else {
                    _values[first] = second ;
                }
                
		    }
	        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "map loaded") ;
	    }
	    catch(std::exception &x) {
		    ACS_THROW(exIOException(x,std::string("Loading file: \"") + path + "\"")) ; // PRQA S 3081
	    }
    }
#ifdef TEST_NAMEIDASSOCIATOR
private:
#endif
	//! returns the id associated with the given name
	//! an exception of type exIllegalValueException is thrown if name is not found
    //! \param name : the name whose id shall be found and returned
    //! \param sanitive : if true the given name and name entries will be sanitized through StringUtils::sanitizeDbName function before being compared
    //! \param caseInsensitive : if true the case will not be used to find the name
	const T &idInternal(const std::string &name, bool sanitize, bool caseInsensitive) const // PRQA S 4020, 4214
    {
		std::string localName = name;
		if( sanitize ) {		
        // Remove spaces and punctuations from the name
	    	localName = StringUtils::sanitizeDbName(name);
		}

	    typename std::map<T,std::string>::const_iterator i;
	    for(i=_values.begin(); i != _values.end() ; i++)
	    {
		    std::string toCheck = i->second;
			
			if( sanitize ) {
            // Remove spaces and punctuations from toCheck
		    	toCheck = StringUtils::sanitizeDbName (toCheck);
			}

		    if (!caseInsensitive) {
			    if ( toCheck == localName )
			    {
				    return i->first ; 
			    }
		    }
		    else {
			     if (StringUtils::equalsNoCase( toCheck, localName)) {
 				    return i->first ; 
			     }
		    }
	    }

        statN(_dbKey)[_tableName][_pkey]["name #"]["error"] +=1 ;
	    ACS_THROW(exIllegalValueException("String not found : \"" + name + "\"" + " in table " + _tableName)) ;  // PRQA S 3081
    }


// data section
private:
	std::string _dbKey ; 
	std::string _tableName ; 
	std::string _pkey ; 
	std::string _col ; 
	std::map<T,std::string> _values;
	std::string _oby ; 
    ACS_CLASS_DECLARE_DEBUG_LEVEL(NameIdAssociator) ;
} ; 

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(NameIdAssociator, T) ;



template <> 
inline
void NameIdAssociator<std::string>::loadId(std::string &id, const std::string &line) // PRQA S 2133, 2134
{
    id = line ;
}

template <class T> 
inline
std::ostream &operator<<(std::ostream &os, const acs::NameIdAssociator<T> &n) 	// output to stream // PRQA S 2133, 2134
{
	for (unsigned int i=0; i< n.size(); i++) {
		const std::string &sec = n[i] ;
		os << n.id(sec) << " - " << sec << "\n" ;
	}
	
	return os ; 
}



template <class T> 
inline
acs::exostream &operator<<(acs::exostream &os, const acs::NameIdAssociator<T> &n) 	// output to stream // PRQA S 2133, 2134
{
	for (unsigned int i=0; i< n.size(); i++) {
		const std::string &sec = n[i] ;
		os << n.id(sec) << " - " << sec << "\n" ;
	}
	
	return os ; 
}




using pattern::ParamConstructibleSingletonMap ;

//! This is the Singleton implementation for the NameIdAssociator.
template <const char* table, const char* idfield, const char* namefield, typename idtype> class NameIdAssociatorSingleton:   // PRQA S 2153
    public NameIdAssociator<idtype>, 
    public ParamConstructibleSingletonMap<NameIdAssociatorSingleton <table, idfield, namefield, idtype>, std::string>
{
    friend NameIdAssociatorSingleton* ParamConstructibleSingletonMap<NameIdAssociatorSingleton <table, idfield, namefield, idtype> , std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~NameIdAssociatorSingleton() throw() {}  // PRQA S 2131
	
protected:
	explicit NameIdAssociatorSingleton(const std::string &key) :
        NameIdAssociator<idtype>(table, idfield,namefield, key),
        ParamConstructibleSingletonMap<NameIdAssociatorSingleton<table, idfield, namefield, idtype>, std::string>()
        {
            NameIdAssociator<idtype>::load() ;
        }
	
private:
	NameIdAssociatorSingleton &operator=(const NameIdAssociatorSingleton &) ;	// declared but not implemented
	NameIdAssociatorSingleton (const NameIdAssociatorSingleton &) ; 			// declared but not implemented
	NameIdAssociatorSingleton () ; 								// declared but not implemented
} ; 


_ACS_END_NAMESPACE



#endif // _NameIdAssociator_H_
