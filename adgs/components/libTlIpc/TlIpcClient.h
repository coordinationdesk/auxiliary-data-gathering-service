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


#ifndef _TLIPCCLIENT_H_
#define _TLIPCCLIENT_H_

#include <acs_c++config.hpp>
#include <TcpIpIpcClient.h>
#include <TlMessage.h>

_ACS_BEGIN_NAMESPACE(acs)

class TlIpcClient : public TcpIpIpcClient<TlMessage> // PRQA S 2109
{
public:
	TlIpcClient();
	virtual ~TlIpcClient() throw() ;
	virtual void marshal (const TlMessage&);

private:
	TlIpcClient (TlIpcClient const &);
	TlIpcClient& operator=(TlIpcClient const &);

private:
	std::string packString(std::string&);
	std::string tlMessage2XMLString (const TlMessage&);

};

_ACS_END_NAMESPACE

#endif // _TLIPCCLIENT_H_

