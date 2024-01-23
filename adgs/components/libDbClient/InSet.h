// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2018/06/14 11:51:11  marpas
	template method declared inline to avoid multiple definitions
	
	Revision 5.3  2018/02/14 11:41:12  marpas
	fixes a crash when the vector given to the InSet is empty
	
	Revision 5.2  2017/11/24 16:27:08  marpas
	InSet template class release candidate
	
	Revision 5.1  2017/11/23 19:11:50  marpas
	InSet template class adedd - working in progress
	
	
	
*/

#ifndef _InSet_H_
#define _InSet_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <dbConnection.h>
#include <dbConv.h>
#include <dbQuery.h>
#include <Filterables.h>
#include <exStackTrace.h>
#include <exException.h>
#include <vector>
#include <set>
#include <list>
#include <limits> 
#include <sstream> 

_ACS_BEGIN_NAMESPACE(acs)

_ACS_BEGIN_NAMESPACE(math)
class StatNode ;
_ACS_END_NAMESPACE 

_ACS_BEGIN_NAMESPACE(db)

#define INSET_OVER_CONTAINER

/*! \addtogroup db 
 *  @{
 */

/*! \namespace db
*/

/**
 * \brief Provide a method to use IN ( xxx ) in where clause where the xxx can be either substituted 
 * by an enumeration of every value or by a subselect from a temporary table opportunely filled
 */
template <typename Container>
class InSet // PRQA S 2109
{
public:
	/*!
	 * \brief ctor
     * \param c the connection (in case a temporary table should be created)
     * \param v the data 
     * \param max_sql_len the max sql length beyond which the class will build the temporary table (defaulted to 8192 chars)
 	 *
     * the temporary table (if any) life scope is the same of the InSet instance
     *
     * the example here below clarifies its use.
     * \code
       list<string> v ; 
       // fill v some way
       InSet<list<string> > inset(connection, v) ; // use the default max length for the in parameter below
       
       // build the query (here is just an example)
       SQLString sqlstring(inset.getKey(), SQLString::SELECT ) ;
       sqlstring.addSelectList("selected_column");
       sqlstring.addTableList("from_table");
       {
          dbQueryParameters pp(inset.getKey()) ;
          pp.addParameter(dbParam(inset.getKey(), "table","column" , dbParam::IN, db::toSQL(inset) )) ;
          sqlstring.addANDWhereParameter(pp) ;
       }
        // eventually use the sqlstring
       \endcode
     */
	InSet(dbConnection &c, const Container &v, size_t max_sql_len=8192) : 
        conn_(c),
        data_(v),
        max_sql_len_(max_sql_len),
        tmpTableName_(),
        tmpTable_()
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "InSet<" << exStackTrace::demangle(typeid(Container).name()) <<">") ;
        
        size_t len = approx_len(v) ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Computed len: " << len << " approx.") ;
        
        if (len < max_sql_len) {
            // no need to store values on temporary table
        }
        else {
            tmpTable_.reset(new dbTemporaryTable(conn_, true)) ; // self removal
            tmpTableName_ = tmpTable_->createTable(data_) ; 
        }
    } 	
    					
	/**
	 * \brief dtor
	 * \li drop the temporary table if it was built
	 */
	~InSet() throw() {} 										

private:
    /** internal use only, returns the approximate lenght in the sql string 
     */
    size_t approx_len(const Container &v) const
    {
        int max_digit_to_represent = std::numeric_limits<typename Container::value_type>::digits10 + 1;
 	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "max_digit_to_represent: " << max_digit_to_represent) ;
        return v.size() * ( max_digit_to_represent + 1) ; // add colon
    }

public:    
    /** returns the correct sql to be put inside a IN () expression (or a NOT IN one).
     * it could be a correctly quoted enumeration of every value inside the data (at ctor time) or 
     * a sub-select from a temporary table oppurtunely built and filled with the data
     */
    std::string getSQL() const
    {
        if (tmpTableName_.empty()) { // no temporary table, unroll the vector
            if (data_.empty()) { return "NULL" ; }
            std::ostringstream sql ;
            const std::string &k = conn_.getKey() ;
            auto it = data_.begin();
            sql << db::toSQL(k, *it) ;
            for (++it; it != data_.end(); ++it) {
                sql << ',' << db::toSQL(k, *it) ;
            }
            return sql.str() ;
        }
        else {
    	    SQLString sqlstring(conn_.getKey(), SQLString::SELECT ) ;
            sqlstring.addSelectList("*"); 
            sqlstring.addTableList(tmpTableName_);
            return sqlstring.getSQL(true) ; // not semicolon termination
        }
    }
    
    /** returns the db key (the very same of the connection passed at ctor) */
    std::string getKey() const { return conn_.getKey() ; }
    /** returns the temporary table name. In case it was not built, returns an empty string */
    const std::string &getTmpTableName() const throw() { return tmpTableName_ ; }

private:
    size_t approx_len_strs(const Container &v) const // only for strings
    {
        size_t l = 0 ;
        for(auto it =v.begin(); it != v.end(); ++it) {
            l += it->length() + 3 ; // add colon and quotes
        } 
 	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "approx_len_strs: " << l) ;
        return l ;
        
    }

//    static math::StatNode &statN(const std::string &key) ;
// declared but not implemented
	InSet() = delete ;
	InSet(const InSet &) = delete ;
	InSet &operator=(const InSet &) = delete ;
	InSet(InSet &&) = delete ;
	InSet &operator=(InSet &&) = delete ;
private:
    dbConnection &conn_ ;
    const Container &data_ ;
    size_t max_sql_len_ ;
    std::string tmpTableName_ ;
    ACS_SMARTPTR<dbTemporaryTable> tmpTable_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InSet<Container>) ;
} ;

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(InSet, Container) ;

/** \brief private specialization for std::vector<std::string> */
template<> inline
size_t InSet<std::vector<std::string> >::approx_len(const std::vector<std::string> &v) const
{ return approx_len_strs(v) ; }

/** \brief private specialization for std::set<std::string> */
template<> inline
size_t InSet<std::set<std::string> >::approx_len(const std::set<std::string> &v) const
{ return approx_len_strs(v) ; }

/** \brief private specialization for std::list<std::string> */
template<> inline
size_t InSet<std::list<std::string> >::approx_len(const std::list<std::string> &v) const
{ return approx_len_strs(v) ; }


/** \brief helper function to be used inside the dbParam 

 * \code
   list<string> v ; 
   // fill v some way
   InSet<list<string> > inset(connection, v) ; // use the default max length for the in parameter below

   // build the query (here is just an example)
   SQLString sqlstring(inset.getKey(), SQLString::SELECT ) ;
   sqlstring.addSelectList("selected_column");
   sqlstring.addTableList("from_table");
   {
      dbQueryParameters pp(inset.getKey()) ;
      pp.addParameter(dbParam(inset.getKey(), "table","column" , dbParam::IN, db::toSQL(inset) )) ;
      sqlstring.addANDWhereParameter(pp) ;
   }
    // eventually use the sqlstring
   \endcode

*/
template <typename T>
inline
const std::string toSQL (const InSet<T> & inset)
{
    return inset.getSQL();
}

/*! @}*/ 

_ACS_END_NESTED_NAMESPACE

#endif /* _InSet_H_ */
