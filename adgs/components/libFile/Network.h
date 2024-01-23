// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2013/10/01 18:01:15  marpas
	qa warnings fixed
	
	Revision 5.1  2013/10/01 17:22:19  marpas
	Networking class (Network) introduced
	
		

*/

#ifndef _net_Network_H_
#define _net_Network_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>
#include <string>
#include <sys/types.h> // PRQA S 1013 2 
#include <sys/socket.h>
#include <netdb.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, net)

/*!	\mainpage
	This library is intended to support network convenience functions.
	Among them there are access ways to get the ip given the hostname
*/

/*! \class Network
	
	Utility class to support network convenience functions.
*/
class Network                                  // PRQA S 2109
{
public:

    exDECLARE_EXCEPTION(NetworkException,exException); // PRQA S 2131, 2502

    
	/*! This method returns IPV4 or IPV6 ip address for a given hostname.
	*/
	static std::vector<std::string> getIPAddresses(const std::string &hostname, int ai_family = AF_UNSPEC) ;

    static std::string family(int) ;
    static std::string socktype(int) ;
private:
	Network() ;
	~Network() ;
	Network(const Network &) ;
	Network &operator=(const Network &) ;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Network) ;
} ;

_ACS_END_NESTED_NAMESPACE

#endif	/* _net_Network_H_ */
