// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer WS Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/03/11 16:24:49  marfav
	Compiler warning reduced
	
	Revision 2.0  2006/02/28 10:10:54  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/28 08:31:42  marfav
	Multiple WSConfig schemas supported
	OrderType translation supported by libTlOrder manager
	
	Revision 1.1  2004/12/23 17:35:01  marfav
	Importing WSConfigMessage
	

*/

#ifndef _WSConfigMessage_H_
#define _WSConfigMessage_H_

#include <WSConfig.h>

_ACS_BEGIN_NAMESPACE(acs)


// This class derives from WSConfigElements
// and adds some elements to its structure

// NOTE: XMLResources are not used to avoid new library dependency

class WSConfigMessageElement : public WSConfigElement // PRQA S 2109, 2153
{
public:
	enum statusType { OK, NO_TT, BAD_TT, DISABLED};
	static std::string statusType2string (statusType);

public:
	WSConfigMessageElement ();
	explicit WSConfigMessageElement (const WSConfigElement&);
	WSConfigMessageElement (const WSConfigMessageElement&);
	WSConfigMessageElement& operator = (const WSConfigMessageElement &e);
	virtual ~WSConfigMessageElement() throw() {} // PRQA S 2131

	// attributes are public
	std::string message; // PRQA S 2100 4
	statusType status;
	std::string defConfigVersion;
	std::string defConfigFile;
};

bool operator == (const WSConfigMessageElement &, const WSConfigMessageElement &);

// This class is used only as a container for ConfigMessageElements
// providing copy ctor from a WSConfig object and operator >>
// for writing the xml structure

class WSConfigMessage  // PRQA S 2109, 2153
{
public:
	explicit WSConfigMessage (const WSConfig&);
	virtual ~WSConfigMessage() throw() {} // PRQA S 2131
	void writeTo(std::ostream&) const ;

    const WSConfigMessageElement& operator [] (const int& s) const; // PRQA S 2141
    const WSConfigMessageElement& at (const int & s) const; // PRQA S 2141

	size_t size() const;
	void clear();
	void push_back(WSConfigMessageElement & t);
	std::vector<WSConfigMessageElement>::const_iterator begin() const;
	std::vector<WSConfigMessageElement>::const_iterator end() const;
	
	std::vector<WSConfigMessageElement>::iterator begin(); 
	std::vector<WSConfigMessageElement>::iterator end(); 
	// operator implicit conversion
	operator const std::vector<WSConfigMessageElement>& () const;
	operator std::vector<WSConfigMessageElement>& () ;


private:
    WSConfigMessage() ; // not implemented
    WSConfigMessage(const WSConfigMessage &) ; // not implemented
    WSConfigMessage& operator=(const WSConfigMessage &) ; // not implemented
private:
	bool _legacyMode;
	std::vector<WSConfigMessageElement> _wsMessages;
};

std::ostream & operator << (std::ostream &, const WSConfigMessage &) ;

_ACS_END_NAMESPACE


#endif // _WSConfigMessage_H_
