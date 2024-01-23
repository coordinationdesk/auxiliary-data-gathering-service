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
	Revision 2.3  2014/03/18 12:08:34  marpas
	Coding best practices applied
	warnings removed (g++ & qa)
	interface rationalization
	
	Revision 2.2  2013/10/22 06:20:42  giucas
	New logging macros adopted
	
	Revision 2.1  2011/11/18 14:12:36  giucas
	Added TimeConverterLocalInitializer
	

*/

#ifndef _TimeConverterLocalInitializer_H_
#define _TimeConverterLocalInitializer_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class TimeConverterLocalInitializer 
{
	public:

		/**
		 * \brief  Default Class constructor 
		 *    
		 */	
		TimeConverterLocalInitializer();

		/**
		 * \brief  Class destructor 
		 *    
		 */
		~TimeConverterLocalInitializer();

		/**
		 * \brief Search predicted matching the input predFileRegExpr in path and initialise the library
		 *  	  Returns the validation interval  
		 */
		void initFromPredicted(const std::string& predFileRegExpr,
							   double start,
							   double stop, 
							   long double& valStart, 
							   long double& valStop, 
							   std::string& predictedFile,
							   const std::string &path="./");


	private:

		/**
		 * \brief  Class copy constructor 
		 *    
		 */ 
		TimeConverterLocalInitializer(const TimeConverterLocalInitializer & ); 

		/**
		 * \brief  Operator= 
		 *    
		 */
		TimeConverterLocalInitializer &operator=(const TimeConverterLocalInitializer &);


		ACS_CLASS_DECLARE_DEBUG_LEVEL(TimeConverterLocalInitializer) ;

};
	
_ACS_END_NAMESPACE

#endif //_TimeConverterLocalInitializer_H_
