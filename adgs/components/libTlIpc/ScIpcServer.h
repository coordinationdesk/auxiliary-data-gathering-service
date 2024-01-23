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
	
	Revision 1.1.1.1  2004/04/07 13:33:14  marfav
	Import libTlIpc
	


*/



#ifndef _ScIpcServer_H_
#define _ScIpcServer_H_

#include <acs_c++config.hpp>
#include <TcpIpIpcServer.h>
#include <TlMessage.h>

_ACS_BEGIN_NAMESPACE(acs)

class ScIpcServer : public TcpIpIpcServer<acs::TlMessage> // PRQA S 2109
{
public:
	ScIpcServer ();
	virtual ~ScIpcServer() throw() ;
	virtual void unmarshal (const std::string&);

private:
	ScIpcServer (ScIpcServer const &);
	ScIpcServer& operator=(ScIpcServer const &);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ScIpcServer) ;
};

_ACS_END_NAMESPACE



#endif//_ScIpcServer_H_

