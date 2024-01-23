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
	
	Revision 1.5  2005/11/02 17:59:24  giucas
	Modified obtToTime and timeToObt interfaces to handle long double instead of double
	
	Revision 1.4  2005/10/03 14:46:02  seralb
	Changed algorithm to calculate obtToTime conversion
	
	Revision 1.3  2005/09/30 13:21:13  seralb
	Modify ObtToTime conversion parameters
	
	Revision 1.2  2005/07/06 13:33:52  seralb
	getvalue() instead of getValueInDouble() for onBoardTime object
	
	Revision 1.1  2005/07/06 09:59:16  seralb
	New class
	


*/

#include <ErsOBTConverter.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <ErsTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ErsOBTConverter)

//Default Class constructor
ErsOBTConverter::ErsOBTConverter() :
	_fileName(),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ErsOBTConverter::Id: " << Id )
}

//Default Class constructor
ErsOBTConverter::ErsOBTConverter(OBTFileParser* parser) :
	_fileName(),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ErsOBTConverter::Id: " << Id )
	if(parser != 0) {
		_parser =parser;
	}
}


//Class destructor
ErsOBTConverter::~ErsOBTConverter() throw()
{
}



//copy constructor
ErsOBTConverter::ErsOBTConverter(const ErsOBTConverter & e):
	_fileName(e._fileName),
	_parser(e._parser)
{
}

//operator = 
ErsOBTConverter&  ErsOBTConverter::operator=(const ErsOBTConverter & e)
{
	if (this != &e) 
	{
		_fileName	= e._fileName;
		_parser 	= e._parser;
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

long double ErsOBTConverter::obtToTime(const OnBoardTime& onBoardTime)
{
	//estract input
	vector<unsigned long> obtInVec = onBoardTime.getValue();
	ACS_COND_THROW(obtInVec.size() != 1 , OBTConverterException("Error : cannot execute conversion because of wrong input OnBoardTime!") )

	unsigned long obt = obtInVec[0];
	//double ascNodeUTC=0.0;
	double referenceUTC=0.0;
	unsigned long satBinTime=0;
	unsigned long clockStepLength=0; 

	ACS_COND_THROW(_parser == 0, OBTConverterException("Error : No OBTFileParser object set to compute conversions") ) ;

	//use parser to calculate the parameters
//	vector<ErsTimeCorrParams*> validParams;
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);
	ACS_COND_THROW(validParams.size() != 1 , OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved!") ) ;
	for(vector<AuxFileParser::Parameters*>::iterator it = validParams.begin(); it != validParams.end(); it++)
	{
		ErsTimeCorrParams *itc = dynamic_cast<ErsTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to ErsTimeCorrParams")) ;
		//ascNodeUTC = itc->AscNodeUTC;	 
		referenceUTC = itc->ReferenceUTC;	 
		satBinTime = itc->SatBinTime; 
		clockStepLength= itc->ClockStepLength;
	}	
 		
    double time = referenceUTC*86400000.0; /* in msecs */
    time += static_cast<double>(static_cast<long>(static_cast<long>(obt) - static_cast<long>(satBinTime) ))*(static_cast<double>(clockStepLength) / 1.0e6);
    double jdt = time/86400000.0; /* in days */
 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"jdt is		   " << setprecision(20) << jdt )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"referenceUTC is	   " << setprecision(20) << referenceUTC)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"obt is		   " << setprecision(20) << obt ) 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"satBinTime is	   " << setprecision(20) << satBinTime)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"clockStepLength is " << setprecision(20) << clockStepLength	)

	return(jdt);
}


OnBoardTime& ErsOBTConverter::timeToObt(const long double& utcIn, OnBoardTime& onBoardTime)
{
	ACS_COND_THROW(_parser == 0 , OBTConverterException("Error : No OBTFileParser object set to compute conversions") ) ;
	//double ascNodeUTC=0.0;
	double referenceUTC=0.0;
	unsigned long satBinTime=0;
	unsigned long clockStepLength=0; 

	//use parser to calculate the parameters
//	vector<ErsTimeCorrParams*> validParams;
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);
	ACS_COND_THROW(validParams.size() != 1 , OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved!") ) ;
	for(vector<AuxFileParser::Parameters*>::iterator it = validParams.begin(); it != validParams.end(); it++)
	{
		ErsTimeCorrParams *itc = dynamic_cast<ErsTimeCorrParams *>(*it) ;
		ACS_COND_THROW(!itc,exIllegalValueException("null pointer to ErsTimeCorrParams")) ;
		//ascNodeUTC = itc->AscNodeUTC;	 
		referenceUTC = itc->ReferenceUTC;	 
		satBinTime = itc->SatBinTime; 
		clockStepLength= itc->ClockStepLength;
	}	
	
	unsigned int obt = satBinTime + static_cast<unsigned int>(utcIn-referenceUTC)*clockStepLength;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"utcIn is " << utcIn << " and obt is " << obt ) ;
	
	vector<unsigned long> obtVec;
	obtVec.push_back(obt);
	
	onBoardTime.setValue(obtVec);

	return onBoardTime;
}
_ACS_END_NAMESPACE
