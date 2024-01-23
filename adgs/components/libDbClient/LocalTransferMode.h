// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2016, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *
 *
 *
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
 	virtual ~LocalTransferMode() throw() ;
protected:
	explicit LocalTransferMode(const std::string &key) ;
private:
	LocalTransferMode &operator=(const LocalTransferMode &) ; 	// declared but not implemented
	LocalTransferMode (const LocalTransferMode &) ; 			// declared but not implemented
	LocalTransferMode () ; 									// declared but not implemented
};

_ACS_END_NAMESPACE;


#endif /* LocalTransferMode_H_ */
