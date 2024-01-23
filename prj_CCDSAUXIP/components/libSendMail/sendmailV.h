// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2013, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: libSendMail  -- version file header $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 */

 
#ifndef _sendmailV_H_
#define _sendmailV_H_

namespace acs {

// support class for versioning

class sendmailV { // PRQA S 2109
public:
	sendmailV() {}
	~sendmailV() {}
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
    sendmailV(const sendmailV &) ; // not implemented
    sendmailV& operator= (const sendmailV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _sendmailV_H_



