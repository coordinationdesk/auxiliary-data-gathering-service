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

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Network) 

string Network::socktype(int t) // PRQA S 4020 
{
    switch(t & 0777) { // remove flags // PRQA S 3003, 4400
    case SOCK_STREAM: { return "SOCK_STREAM" ; } 
    case SOCK_DGRAM: { return "SOCK_DGRAM" ; } 
    case SOCK_RAW: { return "SOCK_RAW" ; } 
    case SOCK_SEQPACKET: { return "SOCK_SEQPACKET" ; } 
#ifdef SOCK_DCCP
    case SOCK_DCCP: { return "SOCK_DCCP" ; } 
#endif
    case SOCK_PACKET: { return "SOCK_PACKET" ; } 
    default:
        {
            ostringstream os ;
            os << "<unrecognized socktype (oct: " << oct << t << ")>" ;
            return os.str() ;
        }
    }
}

string Network::family(int f) // PRQA S 4020 
{
    switch(f) {
    case AF_INET: { return "AF_INET" ; } 
    case AF_INET6: { return "AF_INET6" ; } 
    case AF_UNSPEC: { return "AF_UNSPEC" ; } 
    default:
        {
            ostringstream os ;
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
    struct addrinfo * res = 0 ; 
    
    int eno = getaddrinfo(hostname.c_str(), 0, &hint, & res) ;
    try {
        ACS_COND_THROW(0 != eno,NetworkException( string(hostname.c_str() ) + " getaddrinfo error: " + string(gai_strerror(eno)))) ; // PRQA S 3081
        vector<string> v ;
        for (struct addrinfo *rp = res; rp != 0; rp = rp->ai_next) {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, 
                                   "family: " << family(rp->ai_family) 
                                << " socktype: " << socktype(rp->ai_socktype) 
                                << " protocol: " << rp->ai_protocol 
                                << " addrlen: " << rp->ai_addrlen);
            char in[NI_MAXHOST] ;
            char srv[NI_MAXHOST] ;
            eno =  getnameinfo(rp->ai_addr, rp->ai_addrlen,
                               in, sizeof(in),
                               srv, sizeof(srv), NI_NUMERICHOST);

            ACS_COND_THROW(0 != eno,NetworkException("getnameinfo error: " + string(gai_strerror(eno)))) ; // PRQA S 3081
            v.push_back(in) ;
        }
        freeaddrinfo(res) ;
        res = 0 ;
        return v ;
        
    }
    catch(...) {
        freeaddrinfo(res) ;
        throw ;
    }   
}

_ACS_END_NESTED_NAMESPACE
