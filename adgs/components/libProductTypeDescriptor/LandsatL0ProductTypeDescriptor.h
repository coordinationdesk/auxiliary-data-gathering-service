// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.3  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 1.2  2008/03/27 08:40:43  giucas
	getNameByApidVcidClass method implemented.
	
	Revision 1.1  2006/10/05 12:35:56  giucas
	Added
	

*/

#ifndef _LandsatL0ProductTypeDescriptor_H_
#define _LandsatL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)

class LandsatL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	LandsatL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit LandsatL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~LandsatL0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	LandsatL0ProductTypeDescriptor(const LandsatL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	LandsatL0ProductTypeDescriptor &operator=(const LandsatL0ProductTypeDescriptor &);

};

_ACS_END_NAMESPACE

#endif //_LandsatL0ProductTypeDescriptor_H_
