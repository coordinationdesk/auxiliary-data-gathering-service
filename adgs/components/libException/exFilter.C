// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/06/06 08:32:36  marpas
	includes and namespaces fixes
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.7  2012/12/20 13:25:44  marpas
	coding best practices
	explain used instead of what()
	tests improved
	
	Revision 4.6  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/22 13:41:47  marpas
	pr qa instrumentation
	
	Revision 4.4  2012/06/12 17:37:00  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.2  2012/06/05 17:07:00  marpas
	resolving some quality issues and documentation
	
	Revision 4.1  2012/03/19 15:49:25  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.0  2011/10/18 11:21:59  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.4  2006/02/03 14:56:06  marpas
	copy ctor fixed
	
	Revision 1.3  2004/04/14 09:05:09  marpas
	many changes
	
	Revision 1.2  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.1  2002/10/03 15:05:38  marpas
	exFilter and exFilterable added
	

*/

#include <exFilter.h>
#include <exception>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)



//
//
// exFilter
//
//


exFilter::exFilter(exostream *s) :
	_filteredStream(s)
{
	if (0 != _filteredStream) {
		_filteredStream->installFilter(this) ;
    }
}

exFilter::~exFilter() ACS_NOEXCEPT
{
	try { // no exception out of this method
		if (_filteredStream) {
			_filteredStream->uninstallFilter(this) ; // PRQA S 4631
        }	
	}
	catch(std::exception &x) { // PRQA S 4631 3
		std::cerr << explain(x) ;
	}
}

exFilter::exFilter(const exFilter &) : _filteredStream(0)
{
}

exFilter &exFilter::operator=(const exFilter &) // PRQA S 4072, 4250 
{
    // no self assignement check needed
	return *this ;
}

bool exFilter::filterInsertion(exFilterable &) 
{
	return true ;
}

void exFilter::streamDestroying(exostream const *s) ACS_NOEXCEPT 
{
	if (s == _filteredStream) {
		_filteredStream=0 ;
    }
}

void exFilter::reassignTo(exostream *s) 
{
	if (_filteredStream) { // attached to a stream ? 
		_filteredStream->uninstallFilter(this) ;// remove from it 
    }
	_filteredStream = s ; // attach to the given one 
	
	if (_filteredStream) {
		_filteredStream->installFilter(this) ;			
    }
}

_ACS_END_NAMESPACE
