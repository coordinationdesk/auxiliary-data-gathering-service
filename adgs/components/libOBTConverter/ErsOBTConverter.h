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
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/02 17:59:24  giucas
	Modified obtToTime and timeToObt interfaces to handle long double instead of double
	
	Revision 1.1  2005/07/06 09:59:16  seralb
	New class
	

*/

#ifndef _ErsOBTConverter_H_
#define _ErsOBTConverter_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <rsResourceSet.h>
#include <OBTConverter.h>
#include <OBTFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

class ErsTimeCorrParams;

/**
 * This class handles operations on Ers On Board Time
 */
class ErsOBTConverter : public OBTConverter
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	ErsOBTConverter();

	/**
	 * \brief  Class constructor 
	 * \param  The time correlation file parser
	 */	
	ErsOBTConverter(OBTFileParser*);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsOBTConverter() throw();

	/**
	 * \brief  Class copy constructor 
	 */ 
	ErsOBTConverter(const ErsOBTConverter & ); 

	/**
	 * \brief  Operator= 
	 */
	ErsOBTConverter &operator=(const ErsOBTConverter &);

	/**
	 * \brief  Set the OBTFileParser object to use for parsing
	 * \param  Input OBTFileParser
	 */
	virtual void setParser(OBTFileParser* p){if(p!=0) _parser=p;};

	/**
	 * \brief  Convert OnBoardTime to time
	 * \param  Input OnBoardTime
	 * \retval Output UTC time
	 */
	virtual long double obtToTime(const OnBoardTime&);

	/**
	 * \brief  Convert Time to OnBoardTime
	 * \param  The input UTC time
	 * \param  The output OnBoardTime object
	 * \retval The output OnBoardTime object
	 */		
	virtual OnBoardTime& timeToObt(const long double&,OnBoardTime&);

	/**
	 * \brief  Return the FileName of the TimeCorrelation file used to execute the last conversion
	 */		
	virtual std::string getFileName(){return _fileName;};


private:

	std::string			_fileName;
	OBTFileParser*		_parser;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ErsOBTConverter) ;

};//end ErsOBTConverter class
_ACS_END_NAMESPACE
#endif //_ErsOBTConverter_H_
