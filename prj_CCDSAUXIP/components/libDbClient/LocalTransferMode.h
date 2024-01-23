// PRQA S 1050 EOF
/*
 *
 *
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $
 *
 */

#ifndef _LocalTransferMode_H_
#define _LocalTransferMode_H_

#include<NameIdAssociator.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs) ;


using pattern::ParamConstructibleSingletonMap ;

class LocalTransferMode : // PRQA S 2153
    public NameIdAssociator<short> ,
    public ParamConstructibleSingletonMap<LocalTransferMode, std::string> {

    friend LocalTransferMode* ParamConstructibleSingletonMap<LocalTransferMode, std::string>::instance(const std::string &); // PRQA S 2107
public:
	LocalTransferMode &operator=(LocalTransferMode &&) = delete ;
	LocalTransferMode (LocalTransferMode &&) = delete ;
	LocalTransferMode &operator=(const LocalTransferMode &) = delete ;
	LocalTransferMode (const LocalTransferMode &) = delete ;
	LocalTransferMode () = delete ;
 	virtual ~LocalTransferMode() = default ;
protected:
	explicit LocalTransferMode(const std::string &key) ;
};

_ACS_END_NAMESPACE;


#endif /* LocalTransferMode_H_ */
