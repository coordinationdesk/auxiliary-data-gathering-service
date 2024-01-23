// PRQA S 1050 EOF
//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 1995-2013, Advanced Computer Systems , Inc.
//    Via Della Bufalotta, 378 - 00139 Roma - Italy
//    http://www.acsys.it
//
//    All Rights Reserved.
//
//    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
//    the contents of this file may not be disclosed to third parties, copied or
//    duplicated in any form, in whole or in part, without the prior written
//    permission of Advanced Computer Systems, Inc.
//
//    $Prod: A.C.S. Operation03 class $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 5.0  2013/06/20 16:43:54  marpas
//    adoption of libException 5.x in progress
//
//    Revision 1.4  2012/11/29 19:31:30  marpas
//    implementing qa rules
//    optimizing code and coding best practices
//    coverage framework initiated
//
//    Revision 1.3  2012/02/13 10:59:05  marpas
//    refactoring in progress
//
//    Revision 1.2  2009/05/14 13:19:29  enrcar
//    EC:: c++ rules
//
//    Revision 1.1  2008/07/01 13:14:05  enrcar
//    EC::Imported source
//
//    
//////////////////////////////////////////////////////////////////////////////////////



#ifndef _Operation03_H_
#define _Operation03_H_

#include <acs_c++config.hpp>
#include <ACS_SSL.h> 
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class Operation03
{
public:


	/*! class Operation03_InternalError declaration. */
	exDECLARE_EXCEPTION(Operation03_InternalError, exException) ;   // PRQA S 2131, 2502


	Operation03() ;	    
	~Operation03() throw() ;	/* Nothing to do */


	static unsigned char* DecodePkg(ACS_SSL::CipherStyle cipher, 
                                 unsigned int* returnLength, 
                                 const RSA* key,
                                 unsigned char* buffer, 
                                 unsigned int len, 
                                 bool emitExcept=true, 
                                 bool* success=0);

	static unsigned char* EncodePkg(ACS_SSL::CipherStyle cipher, 
                                 unsigned int* returnLength, 
                                 const RSA* key,
                                 unsigned char* buffer, 
                                 unsigned int len, 
                                 bool emitExcept=true, 
                                 bool* success=0);
	
    static unsigned int	GetCipheredPkgSize(ACS_SSL::CipherStyle cipher, 
                                         const RSA* key, 
                                         unsigned int len ) ;


private:	

	Operation03(const Operation03&) ;
	Operation03 &operator=(const Operation03&) ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Operation03) ;

} ;

_ACS_END_NAMESPACE

#endif	/* _Operation03_H_ */
