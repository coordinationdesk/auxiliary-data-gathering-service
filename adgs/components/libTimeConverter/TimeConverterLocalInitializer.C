// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.3  2014/03/18 12:08:34  marpas
	Coding best practices applied
	warnings removed (g++ & qa)
	interface rationalization
	
	Revision 2.2  2013/10/22 06:20:42  giucas
	New logging macros adopted
	
	Revision 2.1  2011/11/18 14:12:35  giucas
	Added TimeConverterLocalInitializer
	

*/

#include <TimeConverterLocalInitializer.h>
#include <TimeConverterSingleton.h>
#include <TimeFormat.h>
#include <RegExp.h>
#include <Dir.h>
#include <ProcessingTime.h>
#include <AsciiTime.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TimeConverterLocalInitializer)


//Class constructors
TimeConverterLocalInitializer::TimeConverterLocalInitializer()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
} 

//Class destructor
TimeConverterLocalInitializer::~TimeConverterLocalInitializer() 
{
}

void TimeConverterLocalInitializer::initFromPredicted(const string& predFileRegExpr,
													   double start, 
													   double stop, 
													  long double& valStart, 
													  long double& valStop, 
													  string& predictedFile,
													  const string &path
													  )
{
	ACS_COND_THROW( predFileRegExpr.empty(), exIllegalValueException("Input regexpr is empty") ) ; // PRQA S 3081
	RegExp regExpPred(predFileRegExpr) ;
	Dir dir(path) ;
	bool found=false ;
	for(size_t i=0; i<dir.size(); i++)
	{
		if( regExpPred.match( dir[i] ) )
		{
			predictedFile = dir[i] ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"FOUND PREDICTED ORBIT FILE " << predictedFile );
			found = true ;
			break ;
		}
	}

	ACS_COND_THROW( !found, // PRQA S 3081
		exCriticalException("UNABLE TO FIND PREDICTED ORBIT FILE IN PATH "+path+" FROM REGEXPR \""+predFileRegExpr+ "\"") 
		) ;
	
	TimeConverterSingleton::Instance()->getConverter()->initFromFile(   predictedFile,
																		TimeFormat::TC_TIMEMOD_FOS_PREDICTED,
						    											start,
						    											stop
						    										   );

	{
		TimeConverterSingleton::Instance()->getConverter()->getValidationInterval(valStart, valStop);
		ProcessingTime pStart(valStart,TimeFormat::TC_TIME_UTC,TimeFormat::TC_PROC) ;
        ProcessingTime pStop(valStop,TimeFormat::TC_TIME_UTC,TimeFormat::TC_PROC);
		AsciiTime atStart ;
        AsciiTime atStop;
		atStart.setRef(TimeFormat::TC_TIME_UTC) ;
		atStart.setFormat(TimeFormat::TC_ASCII_STD_REF_MICROSEC) ;
		atStop.setRef(TimeFormat::TC_TIME_UTC) ;
		atStop.setFormat(TimeFormat::TC_ASCII_STD_REF_MICROSEC) ;

		TimeConverterSingleton::Instance()->getConverter()->convert(pStart,atStart) ;
		TimeConverterSingleton::Instance()->getConverter()->convert(pStop,atStop) ;

		ostringstream ss;
		ss << "TimeConverterLocalInitializer : TIME CONVERTER LIBRARY INITIALIZED. VALIDATION INTERVAL IS : ["
		   << atStart.getValue() << " , " << atStop.getValue() << "] "
		   << "[" << setprecision(10) << valStart << " , " << setprecision(10) << valStop << "]"; // PRQA S 4400
		ACS_LOG_INFO(ss.str());
	}
}
_ACS_END_NAMESPACE
