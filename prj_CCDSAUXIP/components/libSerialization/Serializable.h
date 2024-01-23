/*
 
    Copyright 2018-2019, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libSerialization $

    $Log$
    Release 1.0.1  2019/10/28 16:31:35  enrico.carta@acsys.it
    First release
    

*/

#ifndef _Serializable_H_
#define _Serializable_H_

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

exDECLARE_EXCEPTION(SerializableException, exException);
exDECLARE_EXCEPTION(SerializableKeyNotFoundException, exException);

/**
 * Base class for serialization.
 * Service library for data structure serialization exchange.
 * The base class implements to virtual pure methods, serialize and unserialize.
 */
// PRQA S 1703 L1

class Serializable {

public:

	Serializable() ;
	virtual ~Serializable() ACS_NOEXCEPT;

	virtual std::string serialize() const = 0;
	virtual void unserialize(const std::string& blob) = 0;	

	virtual void serializeToFile(const std::string& filename) const = 0;
	virtual void unserializeFromFile(const std::string& filename) = 0;	

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Serializable);
};

_ACS_END_NAMESPACE

#endif /* _Serializable_H_ */
