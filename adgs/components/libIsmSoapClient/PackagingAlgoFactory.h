// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	
	

*/


#ifndef _PackagingAlgoFactory_H_
#define _PackagingAlgoFactory_H_ 

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class PackagingAlgo;

class PackagingAlgoFactory // PRQA S 2109
{
public:

	PackagingAlgoFactory();

	~PackagingAlgoFactory() throw() ;

	static PackagingAlgo* buildTarAlgo(const std::map<std::string, std::string> & arcOpts, const std::map<std::string, std::string> & xtrOpts );		
	static PackagingAlgo* buildTarGzAlgo(const std::map<std::string, std::string> & arcOpts, const std::map<std::string, std::string> & xtrOpts);		

private:
    PackagingAlgoFactory(const PackagingAlgoFactory &) ; // not implemented
    PackagingAlgoFactory& operator= (const PackagingAlgoFactory &) ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagingAlgoFactory)

};

_ACS_END_NAMESPACE

#endif // _PackagingAlgoFactory_H_

