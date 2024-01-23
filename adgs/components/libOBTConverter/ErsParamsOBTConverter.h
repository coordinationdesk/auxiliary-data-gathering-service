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
	Revision 1.1  2007/10/15 14:02:13  marant
	ErsParamsOBTConverter added
	
	

*/

#ifndef _ErsParamsOBTConverter_H_
#define _ErsParamsOBTConverter_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <rsResourceSet.h>
#include <ErsOBTConverter.h>
#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

class ErsTimeCorrParams;

/**
 * This class handles operations on Ers On Board Time
 */
class ErsParamsOBTConverter : public ErsOBTConverter
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	ErsParamsOBTConverter(const std::vector<AuxFileParser::Parameters*>& params);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsParamsOBTConverter() throw();

	/**
	 * \brief  Class copy constructor 
	 */ 
	ErsParamsOBTConverter(const ErsParamsOBTConverter & ); 

	/**
	 * \brief  Operator= 
	 */
	ErsParamsOBTConverter &operator=(const ErsParamsOBTConverter &);



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



private:

	std::vector<AuxFileParser::Parameters*> _params;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ErsParamsOBTConverter) ;

};//end ErsOBTConverter class

_ACS_END_NAMESPACE
#endif //_ErsOBTConverter_H_
