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
	Revision 2.1  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/02 17:59:24  giucas
	Modified obtToTime and timeToObt interfaces to handle long double instead of double
	
	Revision 1.1.1.1  2004/12/07 10:06:15  giucas
	Import libOBTConverter
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/10/12 12:53:05  giucas
	Added reference to OBFFileParser and not to file name
	
	Revision 1.1  2004/09/16 16:57:32  giucas
	First release
	


*/

#ifndef _OBTConverter_H_
#define _OBTConverter_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <OnBoardTime.h>

_ACS_BEGIN_NAMESPACE(acs)

class OBTFileParser;

/**
 * This class handles operations on Telemetry OnBoardTime
 */
class OBTConverter // PRQA S 2109
{
public:

	/*! class OBTConverterException declaration */
	exDECLARE_EXCEPTION(OBTConverterException, exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Class constructor
	 */	
	OBTConverter();

	/**
	 * \brief  Class constructor
	 * \param  The TimeCorrelationFile parser
	 */	
	explicit OBTConverter(OBTFileParser*);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~OBTConverter() throw();

	/**
	 * \brief  Set the OBTFileParser object to use for parsing
	 * \param  Input OBTFileParser
	 */
	virtual void setParser(OBTFileParser*)=0;

	/**
	 * \brief  Convert OnBoardTime to time
	 * \param  Input OnBoardTime
	 * \retval Output UTC Time
	 */
	virtual long double obtToTime(const OnBoardTime&)=0;

	/**
	 * \brief  Convert Time to OnBoardTime
	 * \param  The input UTC time
	 * \param  The output OnBoardTime object
	 * \retval The output OnBoardTime object
	 */		
	virtual OnBoardTime& timeToObt(const long double&,OnBoardTime&)=0;
private:
	OBTConverter(const OBTConverter &); // not implemented
	OBTConverter& operator= (const OBTConverter &); // not implemented

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OBTConverter) ;

};
_ACS_END_NAMESPACE

#endif //_OBTConverter_H_
