// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DIWorkOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/01/25 11:16:20  marpas
	using Singleton pattern form libException
	coding best practices
	qa rules
	
	Revision 2.2  2012/11/06 17:44:33  marpas
	Quality improved, coverage test done
	
	Revision 2.1  2012/02/09 14:30:28  marpas
	refactoring
	
	Revision 2.0  2006/02/28 08:29:41  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/05/06 07:39:24  marpas
	headers fixed
	
	Revision 1.3  2003/03/21 11:56:04  clanas
	doxygen comments
	
	Revision 1.2  2003/01/30 16:50:44  clanas
	fixed typos
	
	Revision 1.1.1.1  2003/01/26 11:53:39  clanas
	imported sources
	
	Revision 1.1.1.1  2003/01/24 19:38:44  clanas
	imported sources
	
	Revision 1.1.1.1  2002/12/29 16:43:47  clanas
	imported sources
	
	Revision 1.1.1.1  2002/11/26 14:11:20  clanas
	imported source
	
	Revision 1.1.1.1  2002/09/30 19:36:15  marpas
	Import libException
	

*/

#ifndef _SignalV_H_
#define _SignalV_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

/*! \brief support class for versioning.

*/




class SignalV { // PRQA S 2109
public:
	SignalV() ACS_NOEXCEPT ;
	~SignalV() ACS_NOEXCEPT ;
	static const char* tag() ACS_NOEXCEPT ;
	static const char* name() ACS_NOEXCEPT ;
private:
    SignalV(const SignalV & ) = delete ;
    SignalV& operator= (const SignalV & ) = delete ; 
    SignalV(SignalV && ) = delete ; // PRQA S 2038 2
    SignalV& operator= (SignalV && ) = delete ; 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE


#endif // _SignalV_H_
