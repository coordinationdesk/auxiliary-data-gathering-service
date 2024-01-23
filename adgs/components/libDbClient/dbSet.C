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
	Revision 5.3  2017/11/21 18:14:54  marpas
	rewind method added
	setEmptyRow  & getEmptyRow methods added to store an empty row (no value) in order to know columns and their types using dbQuery::execForSet wher no data is returned, so they cannot be get from the rows (0 returned rows).
	
	Revision 5.2  2014/02/07 18:36:53  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/02/26 17:12:25  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.4  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.3  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.2  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2012/02/13 17:37:18  marpas
	refactoring in progress
	
	Revision 4.0  2011/10/18 12:31:15  marpas
	new statistics adopted
	
	Revision 3.2  2010/09/11 17:39:57  marpas
	debug improved using libException 3.3
	
	Revision 3.1  2010/08/18 10:33:24  marpas
	bug fixed when dbConcurrency is disallowed (see dbLockGuard, dbconf::allowConcurrency and dialects)
	
	Revision 3.0  2009/10/05 14:47:53  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2009/06/22 12:36:36  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/08/11 07:54:48  marpas
	multiset patch removed due the fact the multiset is no longer supported by dbExpress approach
	
	Revision 1.6  2005/04/13 12:22:18  enrcar
	*** empty log message ***
	
	Revision 1.5  2005/04/11 16:06:06  enrcar
	*** empty log message ***
	
	Revision 1.4  2005/04/11 09:52:47  enrcar
	*** empty log message ***
	
	Revision 1.3  2005/04/11 08:46:46  enrcar
	*** empty log message ***
	
	Revision 1.14  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.13  2004/10/15 18:29:42  marpas
	debug improved
	
	Revision 1.12  2004/10/08 10:22:55  marpas
	pathced for multiset queries
	
	Revision 1.11  2004/10/07 01:24:17  marpas
	ITEssentialGuard I/F used
	
	Revision 1.10  2004/10/05 13:30:48  marpas
	multiset support added
	
	Revision 1.9  2004/05/31 13:17:46  marpas
	dbLockGuard revived
	
	Revision 1.7  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.6  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.5  2003/04/15 20:21:37  marpas
	GCC3.2
	
	Revision 1.4  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.3  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.2  2002/12/18 14:58:46  paoscu
	dbSet fixed
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#include <dbSet.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSet)

dbSet::dbSet() :
	_actualIndex(0),
    _set(),
    _emptyRow("")
{
}


dbSet::~dbSet() throw() { }


void dbSet::reset() throw() 
{
	try {
    	if (not _set.empty()) { 
			ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
		}
    	_set.clear() ; // clear it 
    	_actualIndex =0 ; // reset index and count
        _emptyRow.reset() ; 
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
}



void dbSet::set(const vector<dbRow>& itset) 
{
    // No check on itset.size() is performed, since is possible to have an EMPTY SET
	rewind() ;
	_set = itset ;
}

bool dbSet::getNextRow(dbRow &row)
{
	_actualIndex++ ;
	return getRow(_actualIndex,row) ;
}


bool dbSet::getRow(size_t index, dbRow &row) // PRQA S 2017, 4020
{
    /*
        Warning, index is 1-relative
    */

	bool ret = true ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting row # " << index) ;

	if(index > _set.size()) { // NB index is 1-relative

		return false ;
	}
   	row.row(_set[index-1]) ;    /* -1 since index is 1-relative, while _set is 0-relative */ // PRQA S 3084
	_actualIndex = index ;

	return ret ;
}

size_t dbSet::rowCount() const throw() { return _set.size() ; } // PRQA S 4120
size_t dbSet::size() const throw() { return _set.size() ; } // PRQA S 4120
bool dbSet::empty() const throw() { return _set.empty() ; }
const vector<dbRow> &dbSet::set() const throw() { return _set ; }   // PRQA S 4120


_ACS_END_NAMESPACE
