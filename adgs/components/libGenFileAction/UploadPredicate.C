// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
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
	Revision 7.3  2017/11/16 14:43:52  lucio.pulvirenti
	S3EVOL-78: getConfigurationNode return built with root node and _confTag member.
	
	Revision 7.2  2017/11/15 16:23:35  lucio.pulvirenti
	S3EVOL-78: minor changes.
	
	Revision 7.1  2017/11/14 16:35:01  lucio.pulvirenti
	S3EVOL-78: work in progress.
	
	

*/


#include <UploadPredicate.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
	const string confRootTag = "UploadPredicate";
} // empty namespace

UploadPredicate::UploadPredicate(const string & confTag):
									_confTag(confTag)
{
}
	
UploadPredicate::~UploadPredicate() throw()
{
}

UploadPredicate::UploadPredicate(const UploadPredicate &r ) : _confTag(r._confTag) {}

UploadPredicate &UploadPredicate::operator=(const UploadPredicate &r) 
{
    if (this != &r) {
        _confTag = r._confTag ;
    }
    return *this ;
}


std::string UploadPredicate::getConfigurationNode() const 
{ 
	return (confRootTag + '.' + _confTag); 
}


_ACS_END_NAMESPACE
