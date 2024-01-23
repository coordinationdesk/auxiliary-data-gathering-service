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
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	

*/


#include <SignatureAlgo.h>
#include <saV.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    saV version ;
}

SignatureAlgo::SignatureAlgo(const string &filename) : 
	_filename(filename),
	_manageDirectories(false),
	_theGuard(0)
{

}
	
SignatureAlgo::~SignatureAlgo() throw()
{

}

const string &SignatureAlgo::getFileName() const throw() // PRQA S 4120
{
	return _filename;
}

void SignatureAlgo::assignScopeGuard(SignatureAlgoScopeGuard *pg) const throw() // PRQA S 4121
{
	_theGuard = pg ;
}

SignatureAlgoScopeGuard * SignatureAlgo::getScopeGuard() const throw() // PRQA S 4120
{
	return _theGuard ; // PRQA S 4628
}

void SignatureAlgo::setManageDirectories( bool v) 
{
	_manageDirectories = v ;
}

const bool &SignatureAlgo::getManageDirectories() const throw() // PRQA S 4120
{
	return _manageDirectories ;
}


_ACS_END_NAMESPACE
