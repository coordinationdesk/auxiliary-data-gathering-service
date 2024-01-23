/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.2  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 1.1  2007/10/15 14:02:13  marant
	ErsParamsOBTConverter added
	
	

*/

#include <ErsParamsOBTConverter.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <ErsTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ErsParamsOBTConverter)

//Default Class constructor
ErsParamsOBTConverter::ErsParamsOBTConverter(const vector<AuxFileParser::Parameters*>& params) 	:
	_params()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ErsParamsOBTConverter::Id: " << Id )
	this->_params = params;
}

//Class destructor
ErsParamsOBTConverter::~ErsParamsOBTConverter() throw()
{
    for( unsigned short i=0; i<_params.size(); i++) {
        if (_params.at(i)!=0) {
            delete _params.at(i);
		}
    }
}

//copy constructor
ErsParamsOBTConverter::ErsParamsOBTConverter(const ErsParamsOBTConverter & e) :
	_params(e._params)
{
}

//operator = 
ErsParamsOBTConverter&  ErsParamsOBTConverter::operator=(const ErsParamsOBTConverter & e)
{
    
    for( unsigned short i=0; i<e._params.size(); i++) {
        this->_params.push_back(e._params.at(i));
    }
    
	
	return *this ;
}

/*	
This function compute a ERS SAR format (i.g. line) satellite continous bynary counter ('CBinCount') in a UTC time ('UTC').
The CBinCount is a counter reported in each format among the auxiliary bytes (236 byter per format). It a an unsigned long int 
counter (4 bytes) which increase each about 4 msecs (the exact clock is 3.94... and is updated by ERS Mission Control).
Since this counter is not absolute,  in the sense that can not give an absolute time from the beginning of the mission control but it
has a period if validity among (4.294.967.296(2^32) * 4) = 1.717986918e10 msecs ~= 198 days, one to compute the UTC time should use 
a reference within that 198 days period. The reference is given in terms of a value of the satellite continous bynary counter at a 
certain UTC time and a clock step period of which that counter increases. This three numbers, forming a time correlation element, 
are given by the ERS Mission control each orbit in some standard file formats with the suffix 'PATC'. This function assumes that a good 
reference time correlation element has been already found according to the date on which the pass has been acquired and already loaded 
in the structure 'sv'. The function computes the format UTC time simply subtracting the two satellite continous bynary counters and 
using the current clock step period to convert it in time.	
*/


long double ErsParamsOBTConverter::obtToTime(const OnBoardTime& onBoardTime)
{
	vector<unsigned long> obtInVec = onBoardTime.getValue();
	if(obtInVec.size() != 1)
	{
		ostringstream msg;
		msg << "Error : cannot execute conversion because of wrong input OnBoardTime!";
		OBTConverterException e(msg.str());
		ACS_THROW(e);	
	}
	unsigned long obt = obtInVec[0];

	// double ascNodeUTC=0.0;
	double referenceUTC=0.0;
	unsigned long satBinTime=0;
	unsigned long clockStepLength=0; 
 
	ACS_COND_THROW(_params.size() != 1, OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved!") )

	for(vector<AuxFileParser::Parameters*>::iterator it = _params.begin(); it != _params.end(); it++)
	{
		ErsTimeCorrParams *itc = dynamic_cast<ErsTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to ErsTimeCorrParams")) ;
		//ascNodeUTC = itc->AscNodeUTC;	 
		referenceUTC = itc->ReferenceUTC;	 
		satBinTime = itc->SatBinTime; 
		clockStepLength= itc->ClockStepLength;
	}	
 		
    double time = referenceUTC*86400000.0; /* in msecs */
    time += double( long( long(obt) - long(satBinTime) ))*( double(clockStepLength) / 1.0e6);
    double jdt = time/86400000.0; /* in days */ 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"jdt is		   " << setprecision(20) << jdt )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"referenceUTC is	   " << setprecision(20) << referenceUTC )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"obt is		   " << setprecision(20) << obt )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"satBinTime is	   " << setprecision(20) << satBinTime)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"clockStepLength is " << setprecision(20) << clockStepLength	)

	return(jdt);
}


OnBoardTime& ErsParamsOBTConverter::timeToObt(const long double& utcIn, OnBoardTime& onBoardTime)
{	
	//double ascNodeUTC=0.0;
	double referenceUTC=0.0;
	unsigned long satBinTime=0;
	unsigned long clockStepLength=0; 

	ACS_COND_THROW(_params.size() != 1, OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved!") )
	
	for(vector<AuxFileParser::Parameters*>::iterator it = _params.begin(); it != _params.end(); it++)
	{
		ErsTimeCorrParams *itc = dynamic_cast<ErsTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to ErsTimeCorrParams")) ;
		//ascNodeUTC = itc->AscNodeUTC;	 
		referenceUTC = itc->ReferenceUTC;	 
		satBinTime = itc->SatBinTime; 
		clockStepLength= itc->ClockStepLength;
	}	
	
	unsigned int obt = satBinTime + static_cast<unsigned int>(utcIn-referenceUTC)*clockStepLength;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"utcIn is " << utcIn << " and obt is " << obt );
	
	vector<unsigned long> obtVec;
	obtVec.push_back(obt);
	
	onBoardTime.setValue(obtVec);

	return onBoardTime;
}
_ACS_END_NAMESPACE
