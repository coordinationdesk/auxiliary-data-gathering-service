// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/01 15:28:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2012/02/09 13:31:36  marpas
	refactoring
	
	Revision 2.5  2012/02/08 09:52:54  marpas
	refactoring
	
	Revision 2.4  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.3  2009/03/04 12:18:08  marpas
	new CODECHECK instrumentation - no regressions
	
	Revision 2.2  2006/10/16 10:52:07  marpas
	some make up
	documentation added
	
	Revision 2.1  2006/06/22 14:27:39  marpas
	Temporary version
	Uses boost::date_time
	allows microsecs management
	interface slightly changed
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/09/23 16:43:42  marpas
	bug fixed.
	Release note: actually the class acts on a daily based interval.
	This means that an action to be started at 17:00, will be signaled
	until 23:59 of the same day.
	a collateral effect of this is that an action to be started at 23:59,
	will be signaled only in that minute. If the application dos not invoke
	the check before the next day starts, will never receive the trigger to start the action.
	This is compatible with the former release of this library, but probably was never explained.
	
	Revision 1.5  2005/03/10 13:19:42  marpas
	message fixed
	
	Revision 1.4  2005/03/10 13:02:08  marpas
	VariableDateTime resolution is minutes now
	test added
	
	Revision 1.3  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.2  2003/07/11 13:03:48  marpas
	exException::clone implemented
	
	Revision 1.1  2003/06/10 09:30:00  marpas
	new class added
	
	Revision 1.3  2003/06/03 17:00:54  paoscu
	std namespace management changed.
	Extra info on log file about master/slave.
	
	Revision 1.2  2003/03/19 19:34:41  paoscu
	Working version. No Patrol support.
	
	Revision 1.1.1.1  2003/03/14 19:11:24  paoscu
	Import StatisticalReportGenerator
	
		

*/


#include <VariableDateTime.h>

#include <DateTime.h>
#include <Filterables.h>

#include <sstream>
#include <cstdlib>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(VariableDateTime) 

acs::VariableDateTime::VariableDateTime() throw() :
    _year(),
    _month(),
    _day(),
    _dayOfTheWeek(),
    _hour(),
    _min()
{
}

acs::VariableDateTime::VariableDateTime(string const & time) :
    _year(),
    _month(),
    _day(),
    _dayOfTheWeek(),
    _hour(),
    _min()
{
	load(time);
}


acs::VariableDateTime::VariableDateTime(VariableDateTime const& other) throw() :
	_year(other._year),
	_month(other._month),
	_day(other._day),
	_dayOfTheWeek(other._dayOfTheWeek),
	_hour(other._hour),
	_min(other._min)

{
}

VariableDateTime &acs::VariableDateTime::operator=(VariableDateTime const& other) throw() 
{
	if(this != &other) {
    	_year=other._year	;
	    _month=other._month;
	    _day=other._day;
	    _dayOfTheWeek=other._dayOfTheWeek;
	    _hour=other._hour;
	    _min=other._min ;
    }
	return *this;
}



acs::VariableDateTime::~VariableDateTime() throw() {}


void acs::VariableDateTime::load(string const & time)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input string is \""<<time<<"\" ") ;
	
	clear();
	
	istringstream is(time);
	string temp;
	
	//year
	is>>temp;
	ACS_COND_THROW(is.bad(),InvalidFormat(string("Can't find year in \"")+time+"\" .")) ; // PRQA S 3081
	parseValues(4,temp,_year); // PRQA S 4400

	temp = "" ;
	//month
	is>>temp;
	ACS_COND_THROW(is.bad(),InvalidFormat(string("Can't find month in \"")+time+"\" .")) ; // PRQA S 3081
	parseValues(2,temp,_month); // PRQA S 4400

	temp = "" ;
	//day
	is>>temp;
	ACS_COND_THROW(is.bad(),InvalidFormat(string("Can't find day in \"")+time+"\" .")) ; // PRQA S 3081
	parseValues(2,temp,_day); // PRQA S 4400

	temp = "" ;
	//dayOfTheWeek
	is>>temp;
	ACS_COND_THROW(is.bad(),InvalidFormat(string("Can't find dayOfTheWeek in \"")+time+"\" .")) ; // PRQA S 3081
	parseValues(1,temp,_dayOfTheWeek);

	temp = "" ;
	//hour
	is>>temp;
	ACS_COND_THROW(is.bad(),InvalidFormat(string("Can't find hour in \"")+time+"\" .")) ; // PRQA S 3081
	parseValues(2,temp,_hour); // PRQA S 4400
	sort(_hour.begin() , _hour.end() );

	temp = "" ;
	//min
	is>>temp;
	ACS_COND_THROW(is.bad(),InvalidFormat(string("Can't find min in \"")+time+"\" .")) ; // PRQA S 3081

	parseValues(2,temp,_min, true); // PRQA S 4400
	sort(_min.begin() , _min.end() );

	salloCheck(time) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignature) ;
		print();
	ACS_CLASS_END_DEBUG
}


void acs::VariableDateTime::salloCheck(string const & time) const
{
	if (matchValue(23,_hour) && !_hour.empty()) { // PRQA S 4400
		// just emit a warning on a potential dangerous check 
		// that could never be triggered
		ACS_LOG_WARNING("The string \"" << time << "\" is potentially dangerous because the action is scheduled in the last hour of the day. "
			<< "Should the application not check before midnight, the action will never be signaled.") ;
	}
}


bool acs::VariableDateTime::last(DateTime & dueTime) const // PRQA S 4020
{
	//Current year, month, day, dayOfTheWeek should match
	// hour should match 
	// minutes should be elapsed
	DateTime::UTCD utcdNow=DateTime(); // PRQA S 3050
	DateTime dateTimeNow=utcdNow;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current time is "<<dateTimeNow) ;
	// check against year ...
	if( !matchValue(utcdNow.year,_year) ) {
		return false;
    }
	//  ... month ... 
	if( !matchValue(utcdNow.month,_month) ) {
		return false;
    }
	// ... day ...
	if( !matchValue(utcdNow.day,_day) ) {
		return false;
    }
	// ... or weekday
	if( !matchValue(dateTimeNow.dayOfTheWeek(),_dayOfTheWeek) ) {
		return false;
    }
		
	unsigned long lastDueHour=25; //dummy value

	// here if no hours was specified ... lastDueHour is the actual one
	if (_hour.empty()) {
		lastDueHour = utcdNow.hour ;
    }
	else {
		// or check if actual hour is greater of any hour configured
		// last due hour is the first configured one less than the actual
		for(int i=(_hour.size() -1); i>=0; i-- ) // PRQA S 3000, 3010, 3084
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "hour[i]="<<_hour[i]) ; // PRQA S 3000
			if( _hour[i] <= static_cast<unsigned long>(utcdNow.hour)) // PRQA S 3000, 3081
			{
				lastDueHour=_hour[i]; // PRQA S 3000
				break;
			}
		}
	}
	
	if (lastDueHour == 25) { // PRQA S 4400
		return false ;
    }
		
	unsigned long lastDueMinutes=100; //dummy value
	if(_min.empty())
	{
		lastDueMinutes=utcdNow.min ;
	}
	else
	{
		for(int i=(_min.size() -1); i>=0; i-- ) // PRQA S 3000, 3010, 3084
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "min[i]="<<_min[i]) ; // PRQA S 3000
			if( _min[i]<= static_cast<unsigned long>(utcdNow.min)) // PRQA S 3000, 3081
			{
				lastDueMinutes=_min[i]; // PRQA S 3000
				break;
			}
		}
		
		if (lastDueMinutes == 100) { // against dummy value - see above // PRQA S 4400
			if (_hour.empty()) {
				lastDueHour-- ;
            }
				
			for(int i=(_min.size() -1); i>=0; i-- ) // PRQA S 3000, 3010, 3084
			{
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "min[i]="<<_min[i]) ; // PRQA S 3000
				if( _min[i] <= 59 ) // PRQA S 3000, 4400
				{
					lastDueMinutes=_min[i]; // PRQA S 3000
					break;
				}
			}
		}
	}
	
	if(lastDueMinutes!=100) //there was something to do // PRQA S 4400
	{
		// compute the last due time 
		dueTime=DateTime::UTCD(utcdNow.year, utcdNow.month, utcdNow.day, lastDueHour, lastDueMinutes); // PRQA S 3010
		
		// and return true if it is in the past
		if (dueTime > utcdNow) {
			return false ;
        }
		return true;
	}
	return false;
}


void acs::VariableDateTime::clear() throw() 
{
	// clear status variables
	// for this object
	_year.clear();
	_month.clear();
	_day.clear();
	_dayOfTheWeek.clear();
	_hour.clear();
	_min.clear();
}

void acs::VariableDateTime::print() const
{
	// ouput of a readable phrase 
    ostringstream os ;
	os << SimpleDebugSignature << "\n" ;

	// years (any or those configured)
	os << "	year(s)= ";
	if(_year.size()==0) {
		os << "any";
    }
	for(size_t i=0;i<_year.size(); i++) {
		os << _year[i]<<", ";
    }
	os << "\n" ;
	
	// months (any or those configured)
	os << "	month(s)= ";
	if(_month.size()==0) {
		os << "any";
    }
	for(size_t i=0;i<_month.size(); i++) {
		os << _month[i]<<", ";
    }
	os << "\n" ;


	// days (any or those configured)
	os << "	day(s)= ";
	if(_day.size()==0) {
		os << "any";
    }
	for(size_t i=0;i<_day.size(); i++) {
		os << _day[i]<<", ";
    }
	os << "\n" ;
	
	// weekdays (any or those configured)
	os << "	dayOfTheWeek(s)= ";
	if(_dayOfTheWeek.size()==0) {
		os << "any";
    }
	for(size_t i=0;i<_dayOfTheWeek.size(); i++) {
		os << _dayOfTheWeek[i]<<", ";
    }
	os << "\n" ;
	
	// hours (any or those configured)
	os << "	hour(s)= ";
	if(_hour.size()==0) {
		os << "any";
    }
	for(size_t i=0;i<_hour.size(); i++) {
		os << _hour[i]<<", ";
    }
	os << "\n" ;
	
	// minutes (any or those configured)
	os << "	minutes(s)= ";
	if(_min.size()==0) {
		os << "any";
    }
	for(size_t i=0;i<_min.size(); i++) {
		os << _min[i]<<", ";
    }
	os << "\n" ;
	ACS_LOG_DEBUG(os.str()) ;
}




unsigned long acs::VariableDateTime::readNumber(string const & s)
{
	// try to read a number from a string
	size_t len = s.length() ;
	ACS_COND_THROW(len == 0, InvalidNumber("String length is 0")) ; // PRQA S 3081
		
	const char *startConv = s.c_str() ;
	const char *endConv = s.c_str()+len ; // PRQA S 3084
	char *realEnd = 0 ;
	unsigned long res = 0 ;
	res=static_cast<unsigned long>(strtol(startConv,&realEnd,10)) ;	// PRQA S 3081, 4400
	// here checks if the conversion reached the end of the string 
	// else ther should be character non convertible into an integral 
	// number
	if (realEnd != endConv)
	{
		ostringstream os;	
		os << "In string \""<<s<<"\" wrong char \""<<*realEnd<<"\" at pos "<<realEnd-s.c_str(); 
		ACS_THROW(InvalidNumber(os.str())); // PRQA S 3081
	}
	return res;

}

void acs::VariableDateTime::parseValues( // PRQA S 4020
	unsigned long digits, 
	string const & dateString, 
	vector<unsigned long> & data, 
	bool ignoreEmpty
)
{
	try
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "input string is \""<<dateString<<"\" .") ;
		data.clear();
		if(dateString=="*") {
			return;
        }
		size_t position=0;
		string toParse=dateString;

		while(true)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Loop Begin position="<<position<< " l:" << toParse.length()) ;
			if( (toParse.length()-position) < digits) // PRQA S 3084
			{
				if (ignoreEmpty) {
					data.push_back(0) ;
					return ;
				}
				ostringstream os;
				os<<"String \""<<toParse<<"\" is wrong: there should be at least "<<digits<<" chars from position "<<position;
				ACS_THROW(InvalidFormat(os.str())); // PRQA S 3081
			}
			string numberString(toParse, position, digits);
			unsigned long number=readNumber(numberString);
			data.push_back(number);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "inserted value="<<number) ;
			//toParse = string(toParse,digits); //remove the first number 
			position+=digits; // PRQA S 3084
			//if(toParse.length()>0) //There's something else?
			if( (toParse.length()-position) > 0) //There's something else? // PRQA S 3084
			{
				if(toParse[position]=='-') //It's the beginning of a sequence?
				{
					if( (toParse.length() - position) < (digits+1) ) // PRQA S 3084
					{
						ostringstream os;
						os<<"String \""<<toParse<<"\" is wrong: there should be at least "<<(digits+1)<<" chars from position "<<position; // PRQA S 3084
						ACS_THROW(InvalidFormat(os.str())) ; // PRQA S 3081
					}
					string number2String(toParse, position+1, digits);// PRQA S 3084
					unsigned long number2=readNumber(number2String);
					for(unsigned long i=number+1;i<=number2;i++) {// PRQA S 3084
						data.push_back(i);
                    }
					//toParse = string(toParse,digits+1); //remove '-' and the second number.
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "inserted interval="<<number<<"-"<<number2) ;
					position+=(digits+1);// PRQA S 3084
				}

				if( (toParse.length()-position) > 0 ) //Is there a ',' ? // PRQA S 3084
				{
					if(toParse[position]==',')
					{
						//toParse=string(toParse,1); //remove ','.
						position++;
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "comma skipped") ;

					}
					else
					{
						ostringstream os;
						os<<"String \""<<toParse<<"\" is wrong: I was expecting a ',' at position "<<position;
						ACS_THROW(InvalidFormat(os.str())) ; // PRQA S 3081
					}
				}
				else {
					return;
                }
			}//if((toParse.length()-position) > 0))
			else {
				return;
            }
		} // end while
	}
	catch(InvalidNumber &e)
	{
		ACS_THROW(InvalidFormat(e,string("Bad number string in string \"")+dateString+"\" .")); // PRQA S 3081
	}
}


bool acs::VariableDateTime::matchValue(unsigned long value, vector<unsigned long> const & data) const // PRQA S 4020
{
	// if vector is empty -> match is true
	if(data.size()==0) {
		return true;
    }
	// else if value is not found in the vector: no match
	if(find(data.begin(),data.end(),value) == data.end() ) {
		return false;
    }
	return true;
}

_ACS_END_NAMESPACE
