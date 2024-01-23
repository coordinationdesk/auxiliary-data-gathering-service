// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2016/04/22 10:40:59  marpas
	changes to ease doxygen extraction in progress
	
	Revision 5.4  2016/03/10 14:51:49  marpas
	useless comparison if no debug removed
	
	Revision 5.3  2013/07/12 08:58:16  marpas
	reverting all column methods (older had a possible regression and misuse)
	
	Revision 5.2  2013/07/09 11:53:05  marpas
	new method
	    template <typename T> bool column(uint16_t idx, T& val) const ;
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.10  2013/03/26 00:58:19  marpas
	dbRow columns are accessed throu a uint16_t
	other interfaces modified accordingly
	
	Revision 4.9  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.8  2013/01/22 17:54:37  marpas
	qa warnings
	
	Revision 4.7  2012/12/17 18:41:18  marpas
	MemoryGuard changed with acs::pattern::CMemoryScopeGuard
	
	Revision 4.6  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.3  2010/09/11 17:45:41  marpas
	obsolete include removed
	
	Revision 3.2  2010/09/08 17:49:11  marpas
	the cursor is now mutable
	
	Revision 3.1  2010/03/17 14:12:03  enrcar
	EC:: ORA_FAST_BCD_HANDLING. Methods OCINumberToInt, OCINumberToText added
	
	Revision 3.0  2009/10/05 14:47:52  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.5  2009/07/14 13:02:06  enrcar
	EC:: c++rules
	
	Revision 2.4  2008/07/09 15:01:15  enrcar
	EC:: bcd_to_string Method added
	
	Revision 2.3  2007/12/04 10:11:39  crivig
	EC:: suggestedType handled
	
	Revision 2.2  2006/06/26 13:04:01  marpas
	new DateTime / dbExpress interface changed
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.20  2005/11/21 15:04:49  clanas
	wrapper included from dbRow.h, but no longer from dbColumn.h
	
	Revision 1.19  2005/10/25 12:20:05  enrcar
	- Removed unused structure (for dbExpress BCD converter)
	- Added MemoryGuard
	
	Revision 1.18  2005/10/24 12:11:55  enrcar
	dbExpress conversion methods removed (were not really used)
	
	Revision 1.17  2005/10/24 12:09:05  enrcar
	BCD to long conversion: added dbExpress methods (but not really used).
	Since they performs several conversion (bcd to decimal, decimal to long) our methods will be used.
	dbExpress methods will be removed in next cvs version
	
	Revision 1.15  2005/10/21 09:06:29  enrcar
	Column filled only if not empty
	
	Revision 1.14  2005/10/19 15:02:23  enrcar
	Since data are stored as dbColumn, C types are no more used: dbColumn is always used also in getAnyColumn and getStoredColumn
	column(long) method added
	column(long, type T,...) rewritten using Templates
	getLocalRow renamed as getStoredRow
	fillRow renamed as storeCurrentRow and made private
	_localRow renamed as _storedRow
	
	Revision 1.13  2005/09/09 14:32:20  paoscu
	Column(XXX) methods enhanced: expected type is matched against column type.
	
	Revision 1.12  2005/09/06 13:08:38  enrcar
	Slightly changed, since dbColumn now handles BOOLEAN as type apart
	
	Revision 1.11  2005/09/05 15:04:20  enrcar
	getColumnType
	getIfColumnIsNullable
	some method moved from public to private
	
	Revision 1.10  2005/04/27 10:06:40  enrcar
	work in progress
	
	Revision 1.9  2005/04/13 12:22:18  enrcar
	*** empty log message ***
	
	Revision 1.8  2005/04/11 16:05:53  enrcar
	*** empty log message ***
	
	Revision 1.7  2005/04/11 12:21:39  enrcar
	*** empty log message ***
	
	Revision 1.6  2005/04/07 09:38:15  enrcar
	*** empty log message ***
	
	Revision 1.5  2005/04/06 13:35:41  enrcar
	*** empty log message ***
	
	Revision 1.4  2005/03/17 11:09:26  enrcar
	*** empty log message ***
	
	Revision 1.3  2005/03/15 13:58:52  enrcar
	*** empty log message ***
	
	Revision 1.2  2005/03/14 17:17:07  enrcar
	*** empty log message ***
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.11  2004/05/28 10:27:19  marpas
	temporary lock removed ... going in regression mode
	
	Revision 1.10  2004/05/27 15:58:19  marpas
	mutex lock when using ITString
	
	Revision 1.9  2004/04/13 08:30:54  marpas
	documentation updated
	
	Revision 1.8  2003/11/06 16:28:48  marpas
	column intefaces changed, now they all accept a bool pointer to notify the null values.
	In the latter case, with the pointer not null, they do not throw any longer the NullValueException.
	columnSet inline template method has been added to revert the logic having in the 3rd param
	a boolean indicating wether the value has been set (true) or is null (false)
	
	Revision 1.7  2003/09/11 13:59:44  paoscu
	intervalString method added.
	
	Revision 1.6  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.5  2003/06/09 17:11:16  marpas
	Potential leak removed, efficiency improved
	
	Revision 1.4  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.3  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.2  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#ifndef _dbRow_H_
#define _dbRow_H_

#include <acs_c++config.hpp>
#include <dbColumn.h>
#include <dbException.h>


_ACS_BEGIN_NAMESPACE(acs)


//////////////////////////////////////////////
//              							// 
// class dbRow								// 
//              							//
//////////////////////////////////////////////

/*!
 * \brief A row retrieved by a query
 *
 * The row is composed by one or more columns of data
 *
 */
class dbRow // PRQA S 2109
{
public:	

	/*! class ExecuteException declaration */
	exDECLARE_EXCEPTION(ExecuteException,dbException) ; // PRQA S 2131, 2153, 2502

	/*! class NullValueException declaration */
	exDECLARE_EXCEPTION(NullValueException,dbException) ; // PRQA S 2131, 2153, 2502

#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	explicit dbRow(const std::string &key="") ;
#else
	explicit dbRow(const std::string &key) ;
#endif

	virtual	~dbRow() throw() ;
    dbRow(const dbRow &) ; 			
	dbRow &operator=(const dbRow &) ; 
    const std::string &getKey() const throw() { return _key ; }

	//! \return The number of columns composing the row
	uint16_t columns() const ;


	/*!
	 *	Used to get the specified column as a dbColumn object
	 *	\param idx : The column number
	 *	\return : a dbColumn value
	 */
	dbColumn const & column(uint16_t idx) const ;


    /*! 
	 * \fn template<typename T> T& columnSet(size_t col, T &val, bool &isSet ) const
	 *	Template function.
	 *  Used to get the specified column as specific type
	 *	\param col : The column number
	 *	\param val : The variable to fill
	 * 	\param isSet : is filled with true if the value 
	 *  is null else it is filled with false
	 *	\return : The bool value
	 */
	template<typename T> T& columnSet(uint16_t col, T& val, bool &isSet) const ;

    template <typename T> T& column(uint16_t idx, T& val, bool *isNull=0) const ;
    

	/*!
	 * \return column type as dbSpecificColumnType::Type
     */
    dbSpecificColumnType::Type getColumnType(uint16_t idx) const;


	/*!
	 * \return True if column is nullable, o/w False
	 */
    bool getIfColumnIsNullable(uint16_t) const;


	/*!
	 * \return The length of the column
	 */
	unsigned int columnLength(uint16_t) const ;


	/*!
	 * \return The std::string containing the column data type
     * THIS METHOD ONLY RETURNS INFORMIX TYPES, AS STRING (plz. ref. to Informix manuals).
     * - SHOULD NOT BE USED
     * - IS NOT PORTABLE
     *
     * TODO: SHOULD BE REMOVED   
     *
     */
	std::string columnType(uint16_t) const ;

	/*!
	 * \return The std::string containing the column name
	 */
	std::string columnName(uint16_t) const ;



	/*!
	 * \return A pointer to the Informix ITRow
	 */
    // dbSQLCursor row() const ;

	/*!
	 *  Fill the row with an Informix ITRow. 
	 *  NOTE the pointer passed to is stored into the class and from now on is completely managed by it
	 */
	void row(	dbSQLCursor &itrow, 
                const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>(),
                bool storedRow=false) ;
    void row(const dbRow& row) ;

    void dumpStoredRow() ;

	/*!
	 * Reset the class to the original state. It also disposes the internal ITRow pointer if any.
	 */
	void reset() ;

private:
    dbSpecificColumnType::Type 
        getAnyColumn(uint16_t idx, 
                     dbColumn& column, 
                     bool *isNull, 
                     bool checkType=false, 
                     bool forceReadDb_IN=false, 
                     dbSpecificColumnType::Type suggestedType = dbSpecificColumnType::UNDEFINED) const ;

    dbSpecificColumnType::Type getStoredColumn(uint16_t idx, dbColumn& column, bool *isNull) const ;
    static void bcd_dump(const FMTBcd& bcd) ;
    static signed long long int bcd_to_long(const FMTBcd& bcd) ;
    static char* bcd_to_string(const FMTBcd& bcd, char buffer[64]) ;    // PRQA S 4403
    static std::string bcd_to_string(const FMTBcd& bcd) ;
    void storeCurrentRow(const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>()) ;
	void OCINumberToInt(const void*, signed long long&) const ;
	void OCINumberToText(const void*, std::string&) const ;

private:
    mutable dbSQLCursor _cursor ;
    std::vector <dbColumn> _dbColumnVector ;
    bool _storedRow ; /* TRUE if _dbColumnVector must be filled ; otherwise is FALSE */

    std::string _key ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbRow) ;
} ;


template<typename T> 
inline 
T& dbRow::columnSet(uint16_t col, T& val, bool &isSet) const // PRQA S 4214
{
	bool isNull ; // PRQA S 4101
	column(col,val,&isNull) ;
	isSet = !isNull ;
	return val ;    // PRQA S 4028
}
    
template <typename T> 
inline 
T& dbRow::column(uint16_t idx, T& val, bool *isNull) const // PRQA S 4214
{   
    dbColumn col(_key) ;
    getAnyColumn(idx, col, isNull) ;

    if( !col.isNull() ) {
		col.column(val) ; // value is filled
    }
    
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        if ( (!isNull) || (!(*isNull)) ) 
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returned value: " << col ) ;
        }
        else
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dbRow::column. Null Value") ;
        }
    ACS_CLASS_END_DEBUG

    return val ; // PRQA S 4028
}


_ACS_END_NAMESPACE

#endif // _dbRow_H_
