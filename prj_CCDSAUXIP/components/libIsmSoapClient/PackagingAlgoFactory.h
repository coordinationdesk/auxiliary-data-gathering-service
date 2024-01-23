// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
	
	

*/


#ifndef _PackagingAlgoFactory_H_
#define _PackagingAlgoFactory_H_ 

#include <exException.h>
#include <PackagingAlgo.h>


_ACS_BEGIN_NAMESPACE(acs)

class PackagingAlgoFactory // PRQA S 2109
{
public:

	PackagingAlgoFactory();

	~PackagingAlgoFactory() throw() ;

	static PackagingAlgo* buildTarAlgo(const std::map<std::string, std::string> & arcOpts, const std::map<std::string, std::string> & xtrOpts, const PackagingAlgo::FilesChangeFixStrategy & );		
	static PackagingAlgo* buildTarGzAlgo(const std::map<std::string, std::string> & arcOpts, const std::map<std::string, std::string> & xtrOpts, const PackagingAlgo::FilesChangeFixStrategy &);		

private:
    PackagingAlgoFactory(const PackagingAlgoFactory &) ; // not implemented
    PackagingAlgoFactory& operator= (const PackagingAlgoFactory &) ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagingAlgoFactory)

};

_ACS_END_NAMESPACE

#endif // _PackagingAlgoFactory_H_

