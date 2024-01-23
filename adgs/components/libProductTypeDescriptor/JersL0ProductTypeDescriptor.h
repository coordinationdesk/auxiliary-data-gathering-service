// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libProductTypeDescriptor$

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 2.1  2007/10/01 13:40:17  nicvac
	Jers added.
	
        
*/

#ifndef _JersL0ProductTypeDescriptor_H_
#define _JersL0ProductTypeDescriptor_H_

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)
    
/**
 * \brief
 * Jers L0Product Type Descriptor
 **/

class JersL0ProductTypeDescriptor: public L0ProductTypeDescriptor   // PRQA S 2153
{ 
public:

  /**
   * \brief  Default Class constructor. Using this constructor the class
   *		   data are read from database using Configuration Space
   */	
  JersL0ProductTypeDescriptor();

  /**
   * \brief  Class constructor
   * \param  Satellite Name
   */	
  explicit JersL0ProductTypeDescriptor(const std::string&);


  /** Destructor */
  virtual ~JersL0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	JersL0ProductTypeDescriptor(const JersL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	JersL0ProductTypeDescriptor &operator=(const JersL0ProductTypeDescriptor &);


};

_ACS_END_NAMESPACE

#endif //_JersL0ProductTypeDescriptor_H_

