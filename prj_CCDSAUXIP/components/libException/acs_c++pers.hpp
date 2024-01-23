/*

	Copyright 2012-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Base Library $

	$Id$

	$Author$

	$Log$

*/

// the following macros are intensively used in all the code

#ifndef _acs_cpp_pers_HPP_ 
#define _acs_cpp_pers_HPP_ 

// PRQA S 1020 EOF
/*! @addtogroup macro_misc */
//@{
/*!
    This macro can be used in a file to declare a function that throws no exception

    @code
    void f() ACS_NOEXCEPT ;
    @endcode
 */
#if __cplusplus >= 201103L
#define ACS_NOEXCEPT noexcept
#define ACS_NOEXCEPT_FALSE noexcept(false)
#else
#define ACS_NOEXCEPT throw()
#define ACS_NOEXCEPT_FALSE
#endif

/*!
    This macro can be used in a file to declare a smart pointer type

    @code
    ACS_SMARTPTR<T> ptr ;
    @endcode
 */
#if __cplusplus >= 201103L
#define ACS_SMARTPTR std::unique_ptr
#else
#define ACS_SMARTPTR std::auto_ptr
#endif

//@}

#endif // _acs_cpp_pers_HPP_
