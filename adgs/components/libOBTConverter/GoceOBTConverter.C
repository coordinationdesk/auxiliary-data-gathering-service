/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.4  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 2.3  2012/04/18 08:28:16  giucas
	Compilation warnings fixed
	
	Revision 2.2  2006/10/18 18:09:07  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:39:38  marpas
	new interfaces to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2006/01/09 16:11:20  giucas
	Leap Second handling implemented
	
	Revision 1.3  2005/11/02 17:59:24  giucas
	Modified obtToTime and timeToObt interfaces to handle long double instead of double
	
	Revision 1.2  2005/02/21 17:08:51  giucas
	Fixed bug in timeToObt as specified by DanAlt
	
	Revision 1.1.1.1  2004/12/07 10:06:15  giucas
	Import libOBTConverter
	
	Revision 1.6  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.5  2004/11/16 08:55:03  giucas
	Added getFileName method and removed free of memory after conversion, because now info are stored in Parser object
	
	Revision 1.4  2004/10/19 07:41:37  giucas
	Added check on size of parameters list
	
	Revision 1.3  2004/10/12 12:51:53  giucas
	Refer to OBTFileParser class interface and not to specific implementation.
	
	Revision 1.2  2004/09/17 13:59:04  giucas
	Implemented timeToObt method
	
	Revision 1.1  2004/09/16 16:57:17  giucas
	First release
	


*/

#include <GoceOBTConverter.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <math.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GoceOBTConverter)

//Default Class constructor
GoceOBTConverter::GoceOBTConverter() :
	_fileName(),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"GoceOBTConverter::Id: " << Id )
}

//Default Class constructor
GoceOBTConverter::GoceOBTConverter(OBTFileParser* parser) :
	_fileName(),
	_parser(parser)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"GoceOBTConverter::Id: " << Id )
}


//Class destructor
GoceOBTConverter::~GoceOBTConverter() throw()
{
}



//copy constructor
GoceOBTConverter::GoceOBTConverter(const GoceOBTConverter & e) :
	_fileName(e._fileName),
	_parser(e._parser)
{
}

//operator = 
GoceOBTConverter&  GoceOBTConverter::operator=(const GoceOBTConverter & e)
{
	if (this != &e) 
	{
		_fileName	= e._fileName;
		_parser 	= e._parser;
	}
	return *this ;
}


long double GoceOBTConverter::obtToTime(const OnBoardTime& onBoardTime)
{
	ACS_COND_THROW(_parser == 0 , OBTConverterException("Error : No OBTFileParser object set to compute conversions") );

	//estract input
	vector<unsigned long> obtInVec = onBoardTime.getValue();
	ACS_COND_THROW(obtInVec.size() < 2 , OBTConverterException("Error : cannot execute conversion because of wrong input OnBoardTime.") ) ;
	unsigned long obtInCoarse = obtInVec[0];
	unsigned long obtInFine   = obtInVec[1];
	double obtIn = (double(obtInCoarse)) + ((double(obtInFine))/pow(2.0,16.0));
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Input OBT_C : " << obtInCoarse )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Input OBT_F : " << obtInFine )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Input OBT   : " << setprecision(17) << obtIn)
	
	//use parser to calculate the parameters
//	vector<CFGOTimeCorrParams*> validParams;
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);
	ACS_COND_THROW(validParams.size() == 0 , OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved") );
	
	CFGOTimeCorrParams paramObjLeft , paramObjRight;

	long double utcLeft = getUtcLeft(obtIn,validParams,paramObjLeft);
	
	long double utcRight = getUtcRight(obtIn,validParams,paramObjRight);
	
	if( utcRight != 0.0 && fabsl(utcRight - utcLeft) == 1 )
	{
		_fileName = paramObjRight.fileName ;
		return utcRight ;
	}
	
	_fileName = paramObjLeft.fileName ;
	return utcLeft ;
}

OnBoardTime& GoceOBTConverter::timeToObt(const long double& utcIn, OnBoardTime& onBoardTime)
{
	ACS_COND_THROW(_parser == 0 , OBTConverterException("Error : No OBTFileParser object set to compute conversions") );
	//Use the parser to calculate the parameters
//	vector<CFGOTimeCorrParams*> validParams;
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);
	
	//algorithm to calculate the best line of parameters to use
	//it must be selected the param obj whose UTC0 is the closest 
	//and in the past with respect to the UTC to be converted
	CFGOTimeCorrParams paramObj;
	double utcDiff=0.0;
	for(unsigned int i=0; i<validParams.size(); i++)
	{
		CFGOTimeCorrParams *itc = dynamic_cast<CFGOTimeCorrParams *>(validParams[i]) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to CFGOTimeCorrParams")) ;
		double currentDiff = utcIn - (itc->getUTC0());

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Calculated current diff : " << setprecision(17) << currentDiff)
		
		if(currentDiff < 0.0) {
			continue;
		}
		
		if( (utcDiff == 0.0) || (currentDiff <= utcDiff) )
		{
			utcDiff = currentDiff;
			paramObj = *itc;
		}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Best diff is : " << setprecision(17) << utcDiff )
	}
	//check if at least one object has been extracted
	if(utcDiff == 0.0)
	{
		ostringstream msg;
		msg << "Error : cannot calculate Time Correlations parameters because all UTC0 are in the future with respect to the OBT ["
			<< utcIn << "] to be converted";
		ACS_THROW(OBTConverterException(msg.str()));	
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Best parameters are : " << endl << paramObj.dump())

	double obt = (utcIn - paramObj.getUTC0() - paramObj.offset)/paramObj.gradient + paramObj.getOBT0();
	onBoardTime.setValue(obt);

	return onBoardTime;
}


////////////////////////////////////////////////////////////////////////////////
// private methods implementation
////////////////////////////////////////////////////////////////////////////////

//algorithm to calculate the best line of parameters to use
//it is selected the param obj whose OBT0 is the closest 
//and in the past with respect to the OBT to be converted
//long double GoceOBTConverter::getUtcLeft(const double& obtIn, const vector<CFGOTimeCorrParams*>& validParams, CFGOTimeCorrParams& paramObj)
long double GoceOBTConverter::getUtcLeft(const double& obtIn, const vector<AuxFileParser::Parameters*>& validParams, CFGOTimeCorrParams& paramObj)
{
	double obtDiff=0.0;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"validParams size is : " << validParams.size())

	for(vector<AuxFileParser::Parameters*>::const_iterator it = validParams.begin(); it != validParams.end(); it++)
	{
		CFGOTimeCorrParams *itc = dynamic_cast<CFGOTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to CFGOTimeCorrParams")) ;
		double currentDiff = obtIn - (itc->getOBT0());
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Calculated current diff : " << setprecision(17) << currentDiff )
		
		if(currentDiff < 0.0) {
			continue;
		}
		
		if( (obtDiff == 0.0) || (currentDiff <= obtDiff) )
		{
			obtDiff = currentDiff;
			paramObj = *itc;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Best diff is : " << setprecision(17) << obtDiff )
	}
	//check if at least one object has been extracted
	if(obtDiff == 0.0)
	{
		ostringstream msg;
		msg << "GoceOBTConverter::getUtcLef Error : cannot calculate Time Correlations parameters because all OBT0 are in the future with respect to the OBT ["
			<< obtIn << "] to be converted";
		OBTConverterException e(msg.str());
		ACS_THROW(e);	
	}
	//calculate the result
	return ( static_cast<long double> ( (paramObj.gradient) * ( obtIn - paramObj.getOBT0() ) + paramObj.offset + paramObj.getUTC0() ) );
}


//algorithm to calculate the best line of parameters to use
//it is selected the param obj whose OBT0 is the closest 
//and in the future with respect to the OBT to be converted
//long double GoceOBTConverter::getUtcRight(const double& obtIn, const vector<CFGOTimeCorrParams*>& validParams, CFGOTimeCorrParams& paramObj)
long double GoceOBTConverter::getUtcRight(const double& obtIn, const vector<AuxFileParser::Parameters*>& validParams, CFGOTimeCorrParams& paramObj)
{
	double obtDiff=0.0;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"validParams size is : " << validParams.size())
	for(vector<AuxFileParser::Parameters*>::const_iterator it = validParams.begin(); it != validParams.end(); it++)
	{
		CFGOTimeCorrParams *itc = dynamic_cast<CFGOTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to CFGOTimeCorrParams")) ;
		double currentDiff = obtIn - (itc->getOBT0());

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Calculated current diff : " << setprecision(17) << currentDiff)
		
		if(currentDiff > 0.0) {
			continue;
		}
		
		if( (obtDiff == 0.0) || (currentDiff >= obtDiff) )
		{
			obtDiff = currentDiff;
			paramObj = *itc;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Best diff is : " << setprecision(17) << obtDiff )
	}
	//check if at least one object has been extracted
	if(obtDiff == 0.0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"All OBT0 are in the past with respect to the OBT ["
				<< obtIn << "] to be converted -> returning 0" );
		return 0.0 ;	
	}
	//calculate the result
	return ( static_cast<long double> ( (paramObj.gradient) * ( obtIn - paramObj.getOBT0() ) + paramObj.offset + paramObj.getUTC0() ) );

}
_ACS_END_NAMESPACE
