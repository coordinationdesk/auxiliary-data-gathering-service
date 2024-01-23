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
	Revision 5.0  2013/07/07 17:35:32  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 1.7  2012/06/06 13:37:23  marfav
	Removing std::string as base class
	
	Revision 1.6  2012/04/02 09:48:53  marfav
	Assignment operator fixed
	
	Revision 1.5  2012/02/21 13:23:52  marpas
	removing compiler warnings and fixing test Maketile
	
	Revision 1.4  2011/09/29 19:11:38  marpas
	fixing codecheck crashes
	
	Revision 1.3  2004/12/03 18:00:18  marfav
	Header fixed
	
	Revision 1.2  2004/09/07 09:21:40  marfav
	Using case insensitive compare and lessthan operator in StringKey class
	
	Revision 1.1.1.1  2004/04/07 13:29:09  marfav
	Import libStringFactoryChain
	

*/

#include <StringKey.h>

#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


// ***********************************
//     STRING KEY UTILITIES CLASS
//************************************
StringKey
StringKeyUtils::makeKey (const string & a, const string & b, const string & c, const string & d, const string & e)
{
	return
		StringKey (
			StringKeyElement(a), // PRQA S 3081 5
			StringKeyElement(b),
			StringKeyElement(c),
			StringKeyElement(d),
			StringKeyElement(e)
		);
}


// ***********************************
//     STRING KEY ELEMENT CLASS
//************************************

StringKeyElement::StringKeyElement () : 
    _myString() 
{}

StringKeyElement::StringKeyElement (const string& s) : 
    _myString(s)
{
}

StringKeyElement::StringKeyElement (const StringKeyElement& a) :
    _myString (a._myString)
{
	*this = a;
}

StringKeyElement&
StringKeyElement::operator= (const StringKeyElement& a)
{
	if (this != &a)
	{
		_myString = a._myString ;
	}
	return *this;
}

int cmp_nocase (const string& s1, const string& s2) // PRQA S 4020
{
	string::const_iterator p1 = s1.begin();
	string::const_iterator p2 = s2.begin();
	while (p1!=s1.end() && p2!=s2.end())
	{
		if (toupper(*p1) != toupper(*p2)) {
			return (toupper(*p1) < toupper(*p2)) ? -1 : 1; // PRQA S 3382
        }
		++p1;
		++p2;
	}
	return (s1.size() == s2.size()) ? 0 : (s1.size() < s2.size()) ? -1 : 1;  // PRQA S 3382, 3383
}

bool
StringKeyElement::operator== (const StringKeyElement& a) const
{
//	return ( cmp_nocase(string(*this), string( a) ) == 0 );
	return ( cmp_nocase( _myString, a._myString ) == 0 );
}


bool
StringKeyElement::operator!= (const StringKeyElement& a) const
{
	return ! (operator==(a));
}

bool
StringKeyElement::operator< (const StringKeyElement& a) const
{
//	return ( cmp_nocase(string(*this), string( a))  < 0 );
    return ( cmp_nocase( _myString, a._myString )  < 0 );
}


const string StringKeyElement::_wildCard = "*" ;

StringKeyElement
StringKeyElement::getWildCard()
{
	return StringKeyElement(_wildCard); // PRQA S 3081
}

exostream& operator<< (exostream& os, const StringKeyElement& key) // PRQA S 4222
{
	os << string(key); // PRQA S 3050, 3081
	return os;  // PRQA S 4028
}

std::ostream& operator<< (std::ostream& os, const StringKeyElement& key)
{
	os << string(key);// PRQA S 3050, 3081
	return os;  // PRQA S 4028
}

StringKeyElement::operator const std::string& () const
//const string& StringKeyElement::operator() () const
{
    return _myString;
}


_ACS_END_NAMESPACE

