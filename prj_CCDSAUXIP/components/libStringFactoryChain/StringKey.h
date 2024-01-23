// PRQA S 1050 EOF
/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
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
	
	Revision 1.4  2012/06/06 13:37:23  marfav
	Removing std::string as base class
	
	Revision 1.3  2004/12/03 18:00:18  marfav
	Header fixed
	
	Revision 1.2  2004/09/07 09:21:40  marfav
	Using case insensitive compare and lessthan operator in StringKey class
	
	Revision 1.1.1.1  2004/04/07 13:29:09  marfav
	Import libStringFactoryChain
	

*/

#ifndef _StrigKeyElement_H_
#define _StrigKeyElement_H_

#include <acs_c++config.hpp>
#include <TemplateKey.h>
#include <TemplateExtendedEQ.h>

#include <exException.h>
#include <string>
#include <iostream>



_ACS_BEGIN_NAMESPACE(acs)

// Class used as basic element for composing string keys

class StringKeyElement //: public std::string // PRQA S 2109
{
public:
	StringKeyElement();
	explicit StringKeyElement(const std::string &);
	~StringKeyElement() throw() {}

public: // copy ctor and operators
	StringKeyElement (const StringKeyElement&);
	StringKeyElement& operator= (const StringKeyElement&);
	bool operator== (const StringKeyElement&) const; // PRQA S 2070 3
	bool operator!= (const StringKeyElement&) const;
	bool operator< (const StringKeyElement&) const;
    operator const std::string& () const;   // PRQA S 2181

public:
	static StringKeyElement getWildCard();
	static const std::string _wildCard; // PRQA S 2100 2
    std::string _myString;
};

exostream& operator<< (exostream& os, const StringKeyElement& key); // PRQA S 2072
std::ostream& operator<< (std::ostream& os, const StringKeyElement& key);


typedef TemplateKey<StringKeyElement> StringKey;
typedef TemplateExtendedEQ<StringKeyElement> StringExtendedEQ;

// Utility class
// - Creates a StringKey from 5 string parameters
class StringKeyUtils
{
public:
	static StringKey makeKey(
		const std::string & = "",
		const std::string & = "",
		const std::string & = "",
		const std::string & = "",
		const std::string & = "");
private:
	StringKeyUtils() {}

};

_ACS_END_NAMESPACE



#endif // _StrigKeyElement_H_

