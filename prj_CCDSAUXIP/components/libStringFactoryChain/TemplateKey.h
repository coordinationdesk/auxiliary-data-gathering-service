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
	
	Revision 1.3  2007/07/19 13:42:11  giucas
	exStream include added
	
	Revision 1.2  2004/12/03 18:00:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:29:09  marfav
	Import libStringFactoryChain
	

*/

#ifndef _TemplateKey_H_
#define _TemplateKey_H_

#include <acs_c++config.hpp>

#include <exException.h>
#include <exStream.h>
#include <string>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

template <class K>
class TemplateKey // PRQA S 2109
{
public:
	explicit TemplateKey (const K & = K(), const K & = K(), const K & = K(), const K & = K(), const K & = K());
	~TemplateKey() throw() {}

	void satellite(K sat) { _key[0] = sat; }
	K satellite() const { return _key[0]; }

	void mission( K mis) { _key[1] = mis; }
	K mission() const { return _key[1]; }
    
	void processor(K pro) { _key[2] = pro; }
	K processor() const { return _key[2]; }
    
	void version(K ver) { _key[3] = ver; }
	K version() const { return _key[3]; }
    
	void object(K obj) { _key[4] = obj; }
    K object() const { return _key[4]; }
	
    std::vector<K> getKey() const { return _key; }

public: // ctors and operators
	TemplateKey (const TemplateKey&);
	TemplateKey& operator= (const TemplateKey&);
	bool operator== (const TemplateKey&) const; // PRQA S 2070 3
	bool operator!= (const TemplateKey&) const;
	bool operator< (const TemplateKey&) const;

private: //methods
	bool lessThan (typename std::vector<K>::const_iterator, typename std::vector<K>::const_iterator) const;
	void init (const K &, const K &, const K &, const K &, const K &);

private:
	std::vector<K> _key;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TemplateKey) ;

};

template <class K>
exostream& operator<< (exostream& os, const TemplateKey<K>& key) // PRQA S 2072, 4222
{
	std::vector<K> theKey = key.getKey();
	os << "[";
	for (typename std::vector<K>::iterator i=theKey.begin(); i!=theKey.end(); ++i) // PRQA S 4238
	{
		if (i != theKey.begin()) {
			os << ", ";
        }
		os << *i;
	}
	os << "]";
	return os; // PRQA S 4028
}

template <class K>
std::ostream& operator<< (std::ostream& os, const TemplateKey<K>& key)
{
	std::vector<K> theKey = key.getKey();
	os << "[";
	for (typename std::vector<K>::iterator i=theKey.begin(); i!=theKey.end(); ++i) // PRQA S 4238
	{
		if (i != theKey.begin()) {
			os << ", ";
        }
		os << *i;
	}
	os << "]";
	return os; // PRQA S 4028
}


// ******************************************
//    CTOR
// ******************************************
template <class K>
TemplateKey<K>::TemplateKey (const K & sat, const K & mis, const K & pro, const K & ver, const K & obj) : _key()
{
	init (sat, mis, pro, ver, obj);
}

// ******************************************
//   INITIALIZATION
// ******************************************
template <class K>
void
TemplateKey<K>::init (const K & sat, const K & mis, const K & pro, const K & ver, const K & obj)
{
	_key.clear();
	_key.push_back (sat);
	_key.push_back (mis);
	_key.push_back (pro);
	_key.push_back (ver);
	_key.push_back (obj);
}


// ******************************************
//    OPERATOR ==
// ******************************************

template <class K>
bool
TemplateKey<K>::operator== (const TemplateKey<K>& a) const
{
	return _key == a._key;
}


// ******************************************
//    OPERATOR !=
// ******************************************

template <class K>
bool
TemplateKey<K>::operator!= (const TemplateKey<K>& a) const
{
	return !(*this==a);
}

// ******************************************
//    COPY CTOR
// ******************************************

template <class K>
TemplateKey<K>::TemplateKey (const TemplateKey<K>& a) : _key(a._key) 
{
	// empty
}

// ******************************************
//    OPERATOR =
// ******************************************

template <class K>
TemplateKey<K>&
TemplateKey<K>::operator=(const TemplateKey<K>& a)
{
	if (this != &a)
	{
		_key = a._key;
	}
	return *this;
}

// ******************************************
//    OPERATOR <
// ******************************************

template <class K>
bool
TemplateKey<K>::operator< (const TemplateKey<K>& a) const
{
	return lessThan (_key.begin(), a._key.begin());
}

template <class K>
bool
TemplateKey<K>::lessThan (typename std::vector<K>::const_iterator a, typename std::vector<K>::const_iterator b) const
{
	if (a == _key.end())  // Return false for equal keys
	{
		return false;
	}
	else if (*a == *b)
	{
		return lessThan (++a,++b); // Recursively iterate with the next key element
	}
	else if (*a == K::getWildCard()) // a is the wildcard and b not --> [ b < a ]
	{
		return false;
	}
	else if (*b == K::getWildCard()) // b is the wildcard and a not --> [ a < b ]
	{
		return true;
	}
	else return *a < *b; // no wildcards detected. Use the default < operator
}


_ACS_END_NAMESPACE

#endif //_TemplateKey_H_
