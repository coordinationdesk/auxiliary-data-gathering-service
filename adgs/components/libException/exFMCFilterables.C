// PRQA S 1050 EOF
/*

	Copyright 2013-2018, Advanced Computer Systems , Inc.
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
	Revision 5.5  2015/08/12 16:00:09  marfav
	Adding new message metrics
	
	Revision 5.4  2015/08/12 09:02:24  marfav
	MCF messages format implemented
	Added the code per thread in the singleton
	
	Revision 5.3  2015/06/26 18:34:19  marpas
	introducing new MC framework
	
	Revision 5.2  2013/09/27 14:05:17  marpas
	streams have names
	
	Revision 5.1  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.0  2013/06/04 15:35:01  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.1  2013/05/24 11:10:54  marpas
	introducing exFMCFilterables concept
	
	
	
*/

#include <exFMCFilterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, mc)

using std::string ;

//
//
// exFMCCritical
//
//

exFMCCritical::exFMCCritical(const string &n) : exFError(n), MCScopedParams() {}

exFMCCritical::~exFMCCritical() ACS_NOEXCEPT {}

string exFMCCritical::message() const
{
	// Global streams insertion lock
	Lock const lock(globalMutex()) ;

	// Construct the message according to the M&C ICD
	string result;
	char sep = '|';
	result += "M&C";
	result += sep;
	result += MCParameters::instance()->getFunction();
	result += sep;
	result += MCParameters::instance()->getCode();
	result += sep;
	result += MCParameters::instance()->getMetricDefinition();
	result += sep;
	result += MCParameters::instance()->text(sep);
	result += sep;
	result += exFMessage::message();

	return result;
}

//
//
// exFMCError
//
//

exFMCError::exFMCError(const string &n) : exFError(n), MCScopedParams() {}

exFMCError::~exFMCError() ACS_NOEXCEPT {}

string exFMCError::message() const
{
	// Global streams insertion lock
	Lock const lock(globalMutex()) ;

	// Construct the message according to the M&C ICD
	string result;
	char const sep = '|';
	result += "M&C";
	result += sep;
	result += MCParameters::instance()->getFunction();
	result += sep;
	result += MCParameters::instance()->getCode();
	result += sep;
	result += MCParameters::instance()->getMetricDefinition();
	result += sep;
	result += MCParameters::instance()->text(sep);
	result += sep;
	result += exFError::message();

	return result;
}
//
//
// exFMCWarning
//
//

exFMCWarning::exFMCWarning(const string &n) : exFWarning(n), MCScopedParams() {}

exFMCWarning::~exFMCWarning() ACS_NOEXCEPT {}

string exFMCWarning::message() const
{
	// Global streams insertion lock
	Lock const lock(globalMutex()) ;

	// Construct the message according to the M&C ICD
	string result;
	char sep = '|';
	result += "M&C";
	result += sep;
	result += MCParameters::instance()->getFunction();
	result += sep;
	result += MCParameters::instance()->getCode();
	result += sep;
	result += MCParameters::instance()->getMetricDefinition();
	result += sep;
	result += MCParameters::instance()->text(sep);
	result += sep;
	result += exFWarning::message();

	return result;
}

//
//
// exFMCInfo
//
//

exFMCInfo::exFMCInfo(const string &n) : exFMessage(n), MCScopedParams() {}

exFMCInfo::~exFMCInfo() ACS_NOEXCEPT {}

string exFMCInfo::message() const
{
	// Global streams insertion lock
	Lock const lock(globalMutex()) ;

	// Construct the message according to the M&C ICD
	string result;
	char sep = '|';
	result += "M&C";
	result += sep;
	result += MCParameters::instance()->getFunction();
	result += sep;
	result += MCParameters::instance()->getCode();
	result += sep;
	result += MCParameters::instance()->getMetricDefinition();
	result += sep;
	result += MCParameters::instance()->text(sep);
	result += sep;
	result += exFMessage::message();

	return result;
}

_ACS_END_NESTED_NAMESPACE


