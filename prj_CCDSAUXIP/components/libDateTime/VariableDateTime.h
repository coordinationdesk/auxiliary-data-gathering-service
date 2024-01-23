/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
	
	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/01 15:28:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2012/12/02 20:04:10  marpas
	qa rules
	
	Revision 2.5  2012/02/08 09:52:54  marpas
	refactoring
	
	Revision 2.4  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.3  2006/10/16 10:52:07  marpas
	some make up
	documentation added
	
	Revision 2.2  2006/06/22 14:27:39  marpas
	Temporary version
	Uses boost::date_time
	allows microsecs management
	interface slightly changed
	
	Revision 2.1  2006/05/02 07:38:51  giucas
	Added commenents
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/09/23 16:43:42  marpas
	bug fixed.
	Release note: actually the class acts on a daily based interval.
	This means that an action to be started at 17:00, will be signaled
	until 23:59 of the same day.
	a collateral effect of this is that an action to be started at 23:59,
	will be signaled only in that minute. If the application dos not invoke
	the check before the next day starts, will never receive the trigger to start the action.
	This is compatible with the former release of this library, but probably was never explained.
	
	Revision 1.4  2005/03/10 13:02:08  marpas
	VariableDateTime resolution is minutes now
	test added
	
	Revision 1.3  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.2  2003/07/11 13:03:48  marpas
	exException::clone implemented
	
	Revision 1.1  2003/06/10 09:30:00  marpas
	new class added
	
	Revision 1.2  2003/06/03 17:00:54  paoscu
	std namespace management changed.
	Extra info on log file about master/slave.
	
	Revision 1.1.1.1  2003/03/14 19:11:24  paoscu
	Import StatisticalReportGenerator
	
		

*/

#ifndef _VariableDateTime_H_
#define _VariableDateTime_H_ 


#include <acs_c++config.hpp>
#include <exException.h>

#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class DateTime;

class
__attribute__((deprecated("will be removed soon")))
 VariableDateTime // PRQA S 2109
{
	public:
		/*! class InvalidFormat declaration */
		exDECLARE_EXCEPTION(InvalidFormat,exException) ;    // PRQA S 2131, 2502

		/*! class InvalidNumber declaration */
		exDECLARE_EXCEPTION(InvalidNumber,exException) ;    // PRQA S 2131, 2502

 
		__attribute__((deprecated("will be removed soon"))) VariableDateTime() noexcept ;
		explicit __attribute__((deprecated("will be removed soon"))) VariableDateTime(std::string const &);
		~VariableDateTime() noexcept = default ;
		void load(std::string const &);

		/**
		 * Evaluates if the current time satisfies the condition.
		 * If so, it returns true and fills the variable dueTime with the last (elapsed) time that satisfied the condition
		 * The rule to say is the current time satisfied the condition is the following:
		 * Current year, month, day, dayOfTheWeek should match
		 * hour should match and minutes should be elapsed.
		 */
		bool last(DateTime & dueTime) const;

#ifndef TESTVARDATETIME
	private:
#else
	public:
#endif
		void print() const;
	private:
		void clear() noexcept ;
		unsigned long readNumber(std::string const &);
		void parseValues(unsigned long digits, std::string const & dateString, std::vector<unsigned long> & data, bool ignoreEmpty=false);
		bool matchValue(unsigned long value, std::vector<unsigned long> const & data) const;
		void salloCheck(std::string const &) const ;		
	private:
		std::vector<unsigned long> _year;
		std::vector<unsigned long> _month;
		std::vector<unsigned long> _day;
		std::vector<unsigned long> _dayOfTheWeek;
		std::vector<unsigned long> _hour;
		std::vector<unsigned long> _min;


		ACS_CLASS_DECLARE_DEBUG_LEVEL(VariableDateTime) ;
	
};


_ACS_END_NAMESPACE


#endif // _VariableDateTime_H_

