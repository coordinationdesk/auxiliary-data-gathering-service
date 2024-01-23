/*
	Copyright 1995-2007, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libOBTConverter$

	$Id$

	$Author$

	$Log$
	Revision 2.4  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 2.3  2012/04/18 08:28:16  giucas
	Compilation warnings fixed
	
	Revision 2.2  2009/07/13 14:46:42  marant
	Class MosOBTConverter aligned to the new parser's implementation
	
	Revision 2.1  2007/07/25 17:47:25  nicvac
	Support for Mos added.
	
        
*/ 

#include <MosOBTConverter.h>


#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <MosTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MosOBTConverter)

//Default Class constructor
MosOBTConverter::MosOBTConverter() :
	_fileName(),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"MosOBTConverter::Id: " << Id )
}

//Default Class constructor
MosOBTConverter::MosOBTConverter(OBTFileParser* parser) :
	_fileName(),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"MosOBTConverter::Id: " << Id )
	if(parser != 0) {
		_parser =parser;
	}
}


//Class destructor
MosOBTConverter::~MosOBTConverter() throw()
{
}



//copy constructor
MosOBTConverter::MosOBTConverter(const MosOBTConverter & e):
	_fileName(e._fileName),
	_parser(e._parser)
{
}

//operator = 
MosOBTConverter&  MosOBTConverter::operator=(const MosOBTConverter & e)
{
	if (this != &e) 
	{
		_fileName	= e._fileName;
		_parser 	= e._parser;
	}
	return *this ;
}

/*
The function computes the format UTC time simply subtracting the two satellite continous bynary counters and 
using the current clock step period to convert it in time.	
*/

long double MosOBTConverter::obtToTime(const OnBoardTime& onBoardTime)
{
	//estract input
	vector<unsigned long> obtInVec = onBoardTime.getValue();
	ACS_COND_THROW(obtInVec.size() != 1 , OBTConverterException("Error : cannot execute conversion because of wrong input OnBoardTime!") ) ;

	unsigned long obt = obtInVec[0];
	double referenceTime=0.0; //jd50
	unsigned long timeCode=0;
	unsigned long clockStepLength=0; 

	ACS_COND_THROW(_parser == 0 , OBTConverterException("Error : No OBTFileParser object set to compute conversions") ) ;
	//use parser to calculate the parameters
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);
	ACS_COND_THROW(validParams.size() != 1 , OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved!") ) ;
	for(vector<AuxFileParser::Parameters*>::iterator it = validParams.begin(); it != validParams.end(); it++)
	{
		MosTimeCorrParams *itc = dynamic_cast<MosTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to MosTimeCorrParams")) ;
		referenceTime = itc->reference_j50;	 
		timeCode = itc->time_code; 
		clockStepLength= itc->clock_step_length; //nanosec
	}	
 		
    double time = referenceTime*86400000.0; /* in msecs */
    const long MAX_TIMECODE = 16777216; //2^24
    long timeCodeDiff = static_cast<long>(obt) - static_cast<long>(timeCode);
    if (timeCodeDiff > MAX_TIMECODE/2) timeCodeDiff -= MAX_TIMECODE;
    else if (timeCodeDiff < -MAX_TIMECODE/2) timeCodeDiff += MAX_TIMECODE;
    
    time += double(timeCodeDiff)*(double(clockStepLength) / 1.0e6);
    double jdt = time/86400000.0; /* in days */

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"jdt is		   " << setprecision(20) << jdt )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"referenceUTC is	   " << setprecision(20) << referenceTime)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"obt is		   " << setprecision(20) << obt) 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"satBinTime is	   " << setprecision(20) << timeCode)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"clockStepLength is " << setprecision(20) << clockStepLength)

	return(jdt);
}


OnBoardTime& MosOBTConverter::timeToObt(const long double& utcIn, OnBoardTime& onBoardTime)
{
	ACS_COND_THROW(_parser == 0 , OBTConverterException("Error : No OBTFileParser object set to compute conversions") )
	
    double referenceUTC=0.0;
	unsigned long timeCode=0;
	unsigned long clockStepLength=0; 

	//use parser to calculate the parameters
//	vector<MosTimeCorrParams*> validParams;
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);
	ACS_COND_THROW(validParams.size() != 1 , OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved!") ) ;

	for(vector<AuxFileParser::Parameters*>::iterator it = validParams.begin(); it != validParams.end(); it++)
	{
		MosTimeCorrParams *itc = dynamic_cast<MosTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to MosTimeCorrParams")) ;
		referenceUTC = itc->reference_j50;	 
		timeCode = itc->time_code; 
		clockStepLength= itc->clock_step_length;
	}	

	unsigned int obt = timeCode + static_cast<unsigned int>(utcIn-referenceUTC)*clockStepLength;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"utcIn is " << utcIn << " and obt is " << obt )

	vector<unsigned long> obtVec;
	obtVec.push_back(obt);
	onBoardTime.setValue(obtVec);

	return onBoardTime;
}
_ACS_END_NAMESPACE

