/*

	Copyright 2013-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2014/02/12 12:35:27  marpas
	define INOTIFY_NOT_SUPPORTED for systems where inotify is not supported
	
	Revision 5.3  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/10/01 18:01:15  marpas
	qa warnings fixed
	
	Revision 5.1  2013/10/01 17:22:19  marpas
	Networking class (Network) introduced
	
	
		

*/

#include <Network.h>
#include <Filterables.h>
#include <cstring>
#include <arpa/inet.h> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, net)

using std::string ;
using std::vector ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Network) 

string Network::socktype(int t) // PRQA S 4020 
{
    switch(t & 0777) { // remove flags - NOSONAR
    case SOCK_STREAM: return "SOCK_STREAM" ; 
    case SOCK_DGRAM: return "SOCK_DGRAM" ; 
    case SOCK_RAW: return "SOCK_RAW" ; 
    case SOCK_SEQPACKET: return "SOCK_SEQPACKET" ; 
#ifdef SOCK_DCCP
    case SOCK_DCCP: return "SOCK_DCCP" ; 
#endif
    case SOCK_PACKET: return "SOCK_PACKET" ; 
    default:
        { // NOSONAR - block here
            std::ostringstream os ;
            os << "<unrecognized socktype (oct: " << std::oct << t << ")>" ;
            return os.str() ;
        }
    }
}

string Network::family(int f) // PRQA S 4020 
{
    switch(f) {
    case AF_INET: return "AF_INET" ; 
    case AF_INET6: return "AF_INET6" ; 
    case AF_UNSPEC: return "AF_UNSPEC" ; 
    default:
        { // NOSONAR - block here
            std:: ostringstream os ;
            os << "<unrecognized family(" << f << ")>" ;
            return os.str() ;
        }
    }
}


vector<string> Network::getIPAddresses(const string &hostname, int ai_family) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, hostname);
    ACS_COND_THROW(hostname.empty(),exIllegalValueException("empty hostname")) ; // PRQA S 3081

    struct addrinfo hint ; // PRQA S 4102
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = ai_family ;
    struct addrinfo * res = nullptr ; 
    
    int eno = getaddrinfo(hostname.c_str(), nullptr, &hint, & res) ;
    try {
        ACS_COND_THROW(eno,NetworkException( string(hostname.c_str() ) + " getaddrinfo error: " + string(gai_strerror(eno)), eno)) ; // PRQA S 3081
        vector<string> v ;
        for (struct addrinfo *rp = res; rp != nullptr; rp = rp->ai_next) {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, 
                                   "family: " << family(rp->ai_family) 
                                << " socktype: " << socktype(rp->ai_socktype) 
                                << " protocol: " << rp->ai_protocol 
                                << " addrlen: " << rp->ai_addrlen);
            std::array<char, NI_MAXHOST> in ;
            std::array<char, NI_MAXHOST> srv ;
            eno =  getnameinfo(rp->ai_addr, rp->ai_addrlen,
                               in.data(),  in.size(),
                               srv.data(), srv.size(), 
                               NI_NUMERICHOST);

            ACS_COND_THROW(eno,NetworkException("getnameinfo error: " + string(gai_strerror(eno)), eno)) ; // PRQA S 3081
            v.emplace_back(in.data()) ;
        }
        freeaddrinfo(res) ;
        res = nullptr ;
        return v ;
        
    }
    catch(const std::exception &) { // NOSONAR - any exception
        freeaddrinfo(res) ;
        throw ;
    }   
}

_ACS_END_NESTED_NAMESPACE
