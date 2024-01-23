// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Signature Algo Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2006/03/08 14:37:24  ivafam
	Added in PDS Rose Model
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	
		
*/


#ifndef _SignatureAlgo_H_
#define _SignatureAlgo_H_ 

#include <SignatureAlgoScopeGuard.h>

#include <acs_c++config.hpp>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)


//##Documentation
//## This class represents the common interface of all signature algorithms. 
class SignatureAlgo // PRQA S 2109
{
public:
	
    
	explicit SignatureAlgo(const std::string &filename) ;
	virtual ~SignatureAlgo() throw() ;

    
	virtual std::string computeSignature() = 0 ;
	
    
	const std::string &getFileName() const throw() ;
	
    
	void assignScopeGuard(SignatureAlgoScopeGuard *) const throw() ;
    
	SignatureAlgoScopeGuard * getScopeGuard() const throw() ;
	
	void setManageDirectories( bool ) ;
	const bool &getManageDirectories() const throw() ;
		
private:    
	SignatureAlgo() ; // not implemented
	SignatureAlgo(const SignatureAlgo &) ; // not implemented
	SignatureAlgo &operator=(const SignatureAlgo &) ; // not implemented

private:
    
	std::string _filename ;
	bool _manageDirectories;
    
	mutable SignatureAlgoScopeGuard *_theGuard ;	
	
} ; 


_ACS_END_NAMESPACE

#endif // _SignatureAlgo_H_
