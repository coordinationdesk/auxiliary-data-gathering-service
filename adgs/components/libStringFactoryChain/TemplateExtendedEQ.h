// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Enhanced String Factory Chain Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/12/16 19:29:07  marpas
	coding best practices applied
	
	Revision 5.0  2013/07/07 17:35:32  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 1.4  2012/02/21 13:23:52  marpas
	removing compiler warnings and fixing test Maketile
	
	Revision 1.3  2004/12/03 18:00:18  marfav
	Header fixed
	
	Revision 1.2  2004/09/07 09:21:40  marfav
	Using case insensitive compare and lessthan operator in StringKey class
	
	Revision 1.1.1.1  2004/04/07 13:29:09  marfav
	Import libStringFactoryChain
	

*/


#ifndef _TemplateExtendedEQ_H_
#define _TemplateExtendedEQ_H_

#include <ApplicationLock.h>
#include <TemplateKey.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

// Expandes the meaning of "equals" using the wildcard
// stored in the key objects

template <class K>
class TemplateExtendedEQ // PRQA S 2109
{
public:
	explicit TemplateExtendedEQ (const TemplateKey<K> &k) : _key (k) {}
	TemplateExtendedEQ (const TemplateExtendedEQ& r): _key (r._key) {}
	TemplateExtendedEQ& operator= (const TemplateExtendedEQ& r)
    {
	    if (this != &r)
	    {
		    _key = r._key;
	    }
	    return *this;
    }

    ~TemplateExtendedEQ() throw() {} // PRQA S 2131

	bool operator() (const TemplateKey<K> &k)
    {
	    std::vector<K> key = k.getKey();
	    std::vector<K> internalKey = _key.getKey();
	    typename std::vector<K>::iterator intKey = internalKey.begin();
	    bool result = true;

	    for (typename std::vector<K>::iterator extKey = key.begin(); extKey != key.end(); ++extKey)
	    {
		    result &= checkKey (*intKey, *extKey);
		    ++intKey;
	    }

	    return result;
    }


private:
    TemplateExtendedEQ() ; // not implemented
	bool checkKey (const K& toFind, const K& toCompare) const
    {
	    if (toFind.operator== ( toCompare) )
	    {
		    return true;
	    }
	    else if (toCompare.operator== (K::getWildCard()) )
	    {
		    return true;
	    }
	    else
	    {
		    return false;
	    }
    }

private:
	TemplateKey<K> _key;
};


_ACS_END_NAMESPACE


#endif //_TemplateExtendedEQ_H_



