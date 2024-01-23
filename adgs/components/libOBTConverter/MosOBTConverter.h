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
	Revision 2.2  2009/07/13 14:46:42  marant
	Class MosOBTConverter aligned to the new parser's implementation
	
	Revision 2.1  2007/07/25 17:47:25  nicvac
	Support for Mos added.
	
        
*/

#ifndef _MosOBTConverter_H_
#define _MosOBTConverter_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <OBTConverter.h>
#include <OBTFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)
    
/**
 * \brief
 * This class handles operations on Mos On Board Time 
 **/
class MosOBTConverter : public OBTConverter
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	MosOBTConverter();

	/**
	 * \brief  Class constructor 
	 * \param  The time correlation file parser
	 */	
	MosOBTConverter(OBTFileParser*);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~MosOBTConverter() throw();

	/**
	 * \brief  Class copy constructor 
	 */ 
	MosOBTConverter(const MosOBTConverter & ); 

	/**
	 * \brief  Operator= 
	 */
	MosOBTConverter &operator=(const MosOBTConverter &);

	/**
	 * \brief  Set the OBTFileParser object to use for parsing
	 * \param  Input OBTFileParser
	 */
	virtual void setParser(OBTFileParser* p){if(p!=0) {_parser=p;} };

	/**
	 * \brief  Convert OnBoardTime to time (jd50)
	 * \param  Input OnBoardTime
	 * \retval Output UTC time
	 */
	virtual long double obtToTime(const OnBoardTime&);

	/**
	 * \brief  Convert Time to OnBoardTime
	 * \param  The input time (jd50)
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

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MosOBTConverter) ;

};

_ACS_END_NAMESPACE

#endif //_MosOBTConverter_H_

