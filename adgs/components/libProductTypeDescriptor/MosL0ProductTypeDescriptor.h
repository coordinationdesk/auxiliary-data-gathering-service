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
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.1  2007/07/02 15:18:41  nicvac
	Mos added.
	

*/

#ifndef _MosL0ProductTypeDescriptor_H_
#define _MosL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)

class MosL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	MosL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit MosL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~MosL0ProductTypeDescriptor() throw();
private:

	/**
	 * \brief  Copy constructor
	 */
	MosL0ProductTypeDescriptor(const MosL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	MosL0ProductTypeDescriptor &operator=(const MosL0ProductTypeDescriptor &);

};


_ACS_END_NAMESPACE


#endif //_MosL0ProductTypeDescriptor_H_
