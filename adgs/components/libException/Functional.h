// PRQA S 1050 EOF
/*

	Copyright 2012-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2015/11/27 15:52:34  marpas
	qa warning fixed
	
	Revision 5.1  2013/12/16 12:26:30  marpas
	qa warnings
	
    Revision 5.0  2013/06/04 15:34:59  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 1.3  2012/11/16 14:40:56  nicvac
	unary_function added.
	
	Revision 1.2  2012/06/06 16:40:31  marpas
	binary_function template added
	
	Revision 1.1  2012/06/06 16:15:06  marpas
	just added
	
	
*/

#ifndef _Func_Functional_H_
#define _Func_Functional_H_ 

#include <acs_c++config.hpp>


_ACS_BEGIN_NESTED_NAMESPACE(acs,func)

template < typename first_, typename second_, typename result_>
class binary_function // PRQA S 2502, 2614, 2617, 2632, 2633
{
public:

    virtual ~binary_function() ACS_NOEXCEPT {}
    
    typedef first_ first_argument_type;
    typedef second_ second_argument_type;
    typedef result_ result_type;
};

template < typename arg_, typename result_>
class unary_function // PRQA S 2502, 2614, 2617, 2632, 2633
{
public:

    virtual ~unary_function() ACS_NOEXCEPT {}

      typedef arg_ argument_type;
      typedef result_ result_type;
};

_ACS_END_NESTED_NAMESPACE



#endif // _Func_Functional_H_
