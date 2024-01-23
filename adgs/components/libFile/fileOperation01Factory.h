/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. fileOperation01 Handler Factory - HEADER FILE $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/29 19:31:30  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/13 10:59:05  marpas
	refactoring in progress
	
	Revision 2.1  2008/06/04 15:11:24  enrcar
	EC:: fileOperation01, fileOperation01Factory added
	

*/


#ifndef _fileOperation01Factory_H_
#define _fileOperation01Factory_H_

#ifdef __cplusplus
	#include <Operation01Factory.h>
	extern "C" {
#endif
		void ACS_fileOperation01Factory_Instantiate() ;	
#ifdef __cplusplus
	}



namespace acs {

//
//
//
// class fileOperation01Factory
//
//
//
class fileOperation01Factory : public Operation01Factory 
{
public:				  
	fileOperation01Factory() ;
	virtual ~fileOperation01Factory() throw() ;

protected:
	virtual Operation01 *createOperation(const std::string &feature, const std::string &instance) ;

private:
	fileOperation01Factory(const fileOperation01Factory &) ;
	fileOperation01Factory &operator=(const fileOperation01Factory &) ; // operator =
} ;


static fileOperation01Factory *_theDbOperationFactory2 ; // PRQA S 2313


} // namespace

#endif // __cplusplus

#endif // _fileOperation01Factory_H_
