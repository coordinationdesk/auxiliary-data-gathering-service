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
	Revision 2.2  2006/10/18 18:09:07  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:39:38  marpas
	new interfaces to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/01/09 16:11:20  giucas
	Leap Second handling implemented
	
	Revision 1.2  2005/11/02 17:59:24  giucas
	Modified obtToTime and timeToObt interfaces to handle long double instead of double
	
	Revision 1.1.1.1  2004/12/07 10:06:15  giucas
	Import libOBTConverter
	
	Revision 1.4  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.3  2004/11/16 08:55:03  giucas
	Added getFileName method and removed free of memory after conversion, because now info are stored in Parser object
	
	Revision 1.2  2004/10/12 12:51:53  giucas
	Refer to OBTFileParser class interface and not to specific implementation.
	
	Revision 1.1  2004/09/16 16:57:17  giucas
	First release
	


*/

#ifndef _GoceOBTConverter_H_
#define _GoceOBTConverter_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <rsResourceSet.h>
#include <OBTConverter.h>
#include <OBTFileParser.h>
#include <CFGOTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class handles operations on GOCE On Board Time
 */
class GoceOBTConverter : public OBTConverter
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	GoceOBTConverter();

	/**
	 * \brief  Class constructor 
	 * \param  The time correlation file parser
	 */	
	GoceOBTConverter(OBTFileParser*);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~GoceOBTConverter() throw();

	/**
	 * \brief  Class copy constructor 
	 */ 
	GoceOBTConverter(const GoceOBTConverter & ); 

	/**
	 * \brief  Operator= 
	 */
	GoceOBTConverter &operator=(const GoceOBTConverter &);

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

	long double getUtcLeft(const double&,const std::vector<AuxFileParser::Parameters*>&,CFGOTimeCorrParams&);
	long double getUtcRight(const double&,const std::vector<AuxFileParser::Parameters*>&,CFGOTimeCorrParams&);

	std::string			_fileName;
	OBTFileParser*		_parser;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(GoceOBTConverter) ;

};//end GoceOBTConverter class
_ACS_END_NAMESPACE
#endif //_GoceOBTConverter_H_
