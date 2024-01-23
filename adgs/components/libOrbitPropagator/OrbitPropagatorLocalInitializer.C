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
	Revision 2.4  2014/03/20 11:16:59  marpas
	best coding practices applied
	warnings fixed (qa & g++)
	
	Revision 2.3  2014/02/13 11:03:11  giucas
	New log macros adopted
	
	Revision 2.2  2012/04/18 08:43:43  giucas
	Compilation warnings removed
	
	Revision 2.1  2011/11/18 14:34:54  giucas
	Added OrbitPropagatorLocalInitializer
	

*/

#include <OrbitPropagatorLocalInitializer.h>
#include <OrbitPropagatorSingleton.h>
#include <TimeConverterSingleton.h>
#include <TimeFormat.h>
#include <RegExp.h>
#include <Dir.h>
#include <ProcessingTime.h>
#include <AsciiTime.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrbitPropagatorLocalInitializer)


//Class constructors
OrbitPropagatorLocalInitializer::OrbitPropagatorLocalInitializer(const string &satName)
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"OrbitPropagatorLocalInitializer::Id: " << Id )
	if( !satName.empty() ) {
		OrbitPropagatorSingleton::Instance()->getPropagator()->setSatelliteID(satName) ;
	}
} 

//Class destructor
OrbitPropagatorLocalInitializer::~OrbitPropagatorLocalInitializer() throw()
{
}

void OrbitPropagatorLocalInitializer::initFromPredicted(const string& predFileRegExpr,const string& orbScenRegExpr,
														double start, 
														double stop, 
														long double& valStart, 
														long double& valStop, 
														string& predictedFile,
														string& orbScenFile,
														const string &path
													  )
{
	ACS_COND_THROW( predFileRegExpr.empty(), exIllegalValueException("Input predFileRegExpr is empty") ) ; // PRQA S 3081 2
	ACS_COND_THROW( orbScenRegExpr.empty(), exIllegalValueException("Input orbScenRegExpr is empty") ) ;
	RegExp regExpPred(predFileRegExpr) ; 
    RegExp regExpSct(orbScenRegExpr);
	Dir dir(path) ;
	bool predFound=false ; 
    bool osfFound=false;
	for(size_t i=0; i<dir.size(); i++)
	{
		if( !predFound && regExpPred.match( dir[i] ) )
		{
			predictedFile = dir[i] ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"FOUND PREDICTED ORBIT FILE " << predictedFile )
			predFound = true ;
		}
		if( !osfFound && regExpSct.match( dir[i] ) )
		{
			orbScenFile = dir[i] ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"FOUND ORBIT SCENARIO FILE " << orbScenFile )
			osfFound = true ;
		}
		if( predFound && osfFound) { 
			break ;
		}
	}

	ACS_COND_THROW( !predFound, // PRQA S 3081 6
		exCriticalException("UNABLE TO FIND PREDICTED ORBIT FILE IN PATH "+path+" FROM REGEXPR "+predFileRegExpr) 
		) ;
	ACS_COND_THROW( !osfFound, 
		exCriticalException("UNABLE TO FIND ORBIT SCENARIO FILE IN PATH "+path+" FROM REGEXPR "+orbScenRegExpr) 
		) ;
	
	int propagModel = int(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL + PropagFormat::OP_PROPAG_MODEL_AUTO + PropagFormat::OP_PROPAG_MODEL_DOUBLE); // PRQA S 3081
	OrbitPropagatorSingleton::Instance()->getPropagator()->initPropagFromFile(  predictedFile,
																				orbScenFile,
																				PropagFormat::OP_TIMEMOD_FOS_PREDICTED, 
																				start, 
																				stop, 
																				propagModel,
																				PropagFormat::OP_SEL_FILE);

	{
		OrbitPropagatorSingleton::Instance()->getPropagator()->getValidationInterval(valStart, valStop);
		ProcessingTime pStart(valStart,TimeFormat::TC_TIME_UTC,TimeFormat::TC_PROC) ;
        ProcessingTime pStop(valStop,TimeFormat::TC_TIME_UTC,TimeFormat::TC_PROC);
		AsciiTime atStart("", TimeFormat::TC_TIME_UTC,TimeFormat::TC_ASCII_STD_REF_MICROSEC) ;
        AsciiTime atStop( "", TimeFormat::TC_TIME_UTC,TimeFormat::TC_ASCII_STD_REF_MICROSEC) ;

		TimeConverterSingleton::Instance()->getConverter()->convert(pStart,atStart) ;
		TimeConverterSingleton::Instance()->getConverter()->convert(pStop,atStop) ;

		ACS_LOG_INFO("OrbitPropagatorLocalInitializer : LIBRARY INITIALIZED. VALIDATION INTERVAL IS : ["
		   				<< atStart.getValue() << " , " << atStop.getValue() << "] "
		   				<< "[" << setprecision(10) << valStart << " , " << setprecision(10) << valStop << "]") // PRQA S 4400
	}
}
_ACS_END_NAMESPACE
