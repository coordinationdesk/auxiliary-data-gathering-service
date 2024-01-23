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

	$Prod: A.C.S. ThinLayer IPC Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:13  marfav
	Import libTlIpc

*/



#ifndef _TLMESSAGE_H_
#define _TLMESSAGE_H_


#include <acs_c++config.hpp>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class TlMessage // PRQA S 2109
{
public:
	explicit TlMessage (const std::string &cmdType= "", 
                      const std::string &cmdValue= "", 
                      const std::string &paramValue= "", 
                      unsigned int orderid= 0);

	~TlMessage() throw() {} 
    
	void cmdType(const std::string & );
	void cmdValue(const std::string & );
	void paramValue(const std::string & );
	void orderId(unsigned int);

	const std::string &cmdType() const throw() { return _cmdType ; } 
	const std::string &cmdValue() const throw() { return _cmdValue ; } 
	const std::string &paramValue() const throw() { return _paramValue ; } 
	unsigned int orderId() const throw() { return _orderId ; }

	// copy ctor and operator =
	TlMessage(TlMessage const &);
	TlMessage & operator=(TlMessage const &);

public: //cmdType values default set
	static const std::string suspendType; // PRQA S 2100 6
	static const std::string resumeType;
	static const std::string stopType;
	static const std::string abortType;
	static const std::string killType;
	static const std::string userType;


private:
	std::string _cmdType;
	std::string _cmdValue;
	std::string _paramValue;
	unsigned int _orderId;
};

_ACS_END_NAMESPACE


#endif //_TLMESSAGE_H_
