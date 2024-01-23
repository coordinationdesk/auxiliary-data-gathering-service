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
	Revision 2.3  2014/03/20 11:16:59  marpas
	best coding practices applied
	warnings fixed (qa & g++)
	
	Revision 2.2  2014/02/13 11:03:12  giucas
	New log macros adopted
	
	Revision 2.1  2011/11/18 14:34:54  giucas
	Added OrbitPropagatorLocalInitializer
	

*/

#ifndef _OrbitPropagatorLocalInitializer_H_
#define _OrbitPropagatorLocalInitializer_H_ 


#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class OrbitPropagatorLocalInitializer 
{
public:

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	explicit OrbitPropagatorLocalInitializer(const std::string &satName="");

	/**
	 * \brief  Class destructor 
	 *    
	 */
	~OrbitPropagatorLocalInitializer() throw();

	/**
	 * \brief Search predicted and OSF matching the input regexpr in path and initialise the library
	 *  	  Returns the validation interval  
	 */
	void initFromPredicted(const std::string& predFileRegExpr,
						   const std::string& orbScenRegExpr,
						   double start,
						   double stop, 
						   long double& valStart, 
						   long double& valStop, 
						   std::string& predictedFile,
						   std::string& orbScenFile,
						   const std::string &path="./");


private:

	OrbitPropagatorLocalInitializer(const OrbitPropagatorLocalInitializer & ); // not implemented
	OrbitPropagatorLocalInitializer &operator=(const OrbitPropagatorLocalInitializer &); // not implemented


	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrbitPropagatorLocalInitializer) ;

};
	
_ACS_END_NAMESPACE


#endif //_OrbitPropagatorLocalInitializer_H_
