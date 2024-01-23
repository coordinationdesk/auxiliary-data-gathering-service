/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

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
	
	Revision 1.4  2006/01/13 11:43:18  giucas
	Fixed bug in obtToTime method
	
	Revision 1.3  2005/11/18 16:16:38  giucas
	Fixed obtToTime algorithm
	
	Revision 1.2  2005/11/18 10:22:54  clanas
	math.h included
	
	Revision 1.1  2005/11/17 18:51:57  giucas
	First release
	
	

*/

#include <EnvisatOBTConverter.h>
#include <USCTimeCorrParams.h>
#include <Filterables.h>
#include <File.h>
#include <unistd.h>
#include <math.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EnvisatOBTConverter)

const long long int EnvisatOBTConverter::BIG_NEGATIVE_THREASHOLD = static_cast<long long int>( - (pow(2.0,20.0)) );

//Default Class constructor
EnvisatOBTConverter::EnvisatOBTConverter() :
	_fileName(""),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"EnvisatOBTConverter::Id: " << Id )
}

//Default Class constructor
EnvisatOBTConverter::EnvisatOBTConverter(OBTFileParser* parser) :
	_fileName(""),
	_parser(0)
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"EnvisatOBTConverter::Id: " << Id )
	if(parser != 0) {
		_parser = parser;
	}
}


//Class destructor
EnvisatOBTConverter::~EnvisatOBTConverter() throw() 
{
}


//copy constructor
EnvisatOBTConverter::EnvisatOBTConverter(const EnvisatOBTConverter & e):
	_fileName(e._fileName),
	_parser(e._parser)
{
}

//operator = 
EnvisatOBTConverter&  EnvisatOBTConverter::operator=(const EnvisatOBTConverter & e)
{
	if (this != &e) 
	{
		_fileName  = e._fileName;
		_parser    = e._parser;
	}
	return *this ;
}


long double EnvisatOBTConverter::obtToTime(const OnBoardTime& onBoardTime)
{
	ACS_COND_THROW(_parser == 0 , OBTConverterException("Error : No OBTFileParser object set to compute conversions") )

	//extract input
	vector<unsigned long> obcVec = onBoardTime.getValue();
	ACS_COND_THROW(obcVec.size() < 2 , OBTConverterException("Error : cannot execute conversion because of wrong input OnBoardTime.") ) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Input OBT[0] : " << obcVec[0] )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Input OBT[1] : " << obcVec[1] )
	
	//use parser to calculate the parameters
	vector<AuxFileParser::Parameters*> validParams;
	_parser->parse(validParams);

	ACS_COND_THROW(validParams.size() == 0 , OBTConverterException("Error : NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved") ) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"validParams size is : " << validParams.size())
	
	///////////////////////////////////////////
	//ONLY 1 ELEMENT EXPECTED from validParams
	///////////////////////////////////////////
	
	USCTimeCorrParams *vp0 = dynamic_cast<USCTimeCorrParams *>(validParams[0]) ;
	ACS_COND_THROW(!vp0,exIllegalValueException("null pointer to USCTimeCorrParams")) ;
	vector<unsigned long int> refSBTVec = vp0->refSBT;
	ACS_COND_THROW( refSBTVec.size() == 0 , OBTConverterException("Error : NO VALID refSBT PARAMETERS in TimeCorrelationFile parsing retrieved") )

	long double currentDiff = static_cast<long double>(obcVec[0]) - static_cast<long double>(refSBTVec[0]);
	long double clockStepInSec = static_cast<long double>(vp0->clockStep) * pow(10.0, -12.0);
	long double factor = (static_cast<long double>( obcVec[1] ) / pow(2.0 , 16.0) );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"refTime is " << setprecision(17) << vp0->refTime )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"refSBT[0] is : " << refSBTVec[0] )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ClockStep  is : " << vp0->clockStep )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"currentDiff (obcVec[0] - refSBTVec[0])  is : " << currentDiff )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"clockStepInSec is : " << clockStepInSec )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"factor is : " << factor )		

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//calculate UTC Time (in days ) applying the following:
	// UTC = refTime + (  (clockStep * 10^-12 * ( OBT[0] - refSBT[0] )) + ( (OBT[1] >> 24) / 2^16 ) ) / 86400
	// OBT[1] is shifted because when it is extracted from the packet, the value is put in the MSB
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double utcTime = vp0->refTime + ( ( clockStepInSec * currentDiff ) + factor ) / 86400.0;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"returning " << setprecision(17) << utcTime );

	return utcTime;
}

OnBoardTime& EnvisatOBTConverter::timeToObt(const long double& utcIn, OnBoardTime& onBoardTime)
{
	ACS_THROW( exIllegalValueException("EnvisatOBTConverter::timeToObt method NOT IMPLEMENTED") );
}
_ACS_END_NAMESPACE
