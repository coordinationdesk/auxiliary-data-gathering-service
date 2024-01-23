// PRQA S 1050 EOF
////////////////////////////////////////////////////////////////////////////////////////////
// 
// 	Copyright 1995-2013, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
//  $Prod: A.C.S. HWScan class $
// 
// 	$Id$
// 
// 	$Author$
// 
//  $Log$
//  Revision 5.4  2014/02/07 18:42:30  marpas
//  ACS_THROW and ACS_COND_THROW macros adopted
//
//  Revision 5.3  2013/07/04 18:49:27  marpas
//  finishing adoption of libException 5.x standards
//  qa warnings removed
//  compilation warnings removed
//  coding best practices applied
//
//  Revision 5.2  2013/07/03 16:41:25  marpas
//  continuing adoption of libException 5.x standards
//  qa warnings
//
//  Revision 5.1  2013/07/02 17:31:26  marpas
//  adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
//
//  Revision 5.0  2013/06/20 16:43:53  marpas
//  adoption of libException 5.x in progress
//
//  Revision 2.10  2013/03/11 13:04:54  marpas
//  FDGuard is from libException (Guard.hpp)
//  FileGuard completely rewritten
//  interface rationalization, test compilation fiexed
//
//  Revision 2.9  2012/12/17 18:46:55  marpas
//  MemoryGuard changed into CMemoryScopeGuard
//
//  Revision 2.8  2012/02/14 13:43:31  marpas
//  removing deprecated macro
//
//  Revision 2.7  2012/02/07 17:57:44  marpas
//  refactoring in progress
//
//  Revision 2.6  2011/11/04 10:32:54  enrcar
//  EC:: added static method HWScan::getNetworkInterfacesMap
//
//  Revision 2.5  2010/11/16 13:47:46  enrcar
//  EC:: Tag VendorId supported
//
//  Revision 2.4  2010/11/11 16:18:54  enrcar
//  Cpuid (Intel architecture) feature handled
//
//  Revision 2.3  2009/05/14 13:39:57  enrcar
//  EC:: c++ rules
//
//  Revision 2.2  2008/10/24 10:14:03  marpas
//  FDGuard class used
//
//  Revision 2.1  2006/04/28 16:41:51  enrcar
//  EC:: small changes, now code can be also used into plain-C packages
//
//  Revision 2.0  2006/02/28 08:33:12  marpas
//  Exception 2.1 I/F adopted
//
//  Revision 1.5  2005/11/04 17:01:21  enrcar
//  hostname method added
//
//  Revision 1.4  2005/11/03 12:51:52  enrcar
//  Added method: ScanHW_CMD_GETHOSTNAME
//  Parameter GETHOSTNAME supported.
//
//  Revision 1.3  2005/11/03 10:29:40  enrcar
//  Added include for C compatibility
//
//  Revision 1.2  2005/11/02 14:32:58  enrcar
//  GuardFileDesc, MemoryGuard classes added
//  GETMAC command implemented && tested
//
//
////////////////////////////////////////////////////////////////////////////////////////////


#include <HWScan.h>
#include <Guard.hpp>
#include <FileGuard.h>
#include <ACScpuid.h>
#include <Filterables.h>

#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h> // PRQA S 1013 2
#include <sys/types.h>
#include <cerrno>

#include <iostream>
#include <algorithm>
#include <linux/hdreg.h> // PRQA S 1013 4
#include <net/if.h> 
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <ifaddrs.h>
#include <netdb.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using acs::pattern::FDGuard ;
   
ACS_CLASS_DEFINE_DEBUG_LEVEL(HWScan);

//------------------------------------------------------------------------
//                                 HWScan
//------------------------------------------------------------------------
HWScan::HWScan() throw() 
{	
}


//------------------------------------------------------------------------
//                                 ~HWScan
//------------------------------------------------------------------------
HWScan::~HWScan() throw() { }


////////////////////////////////////////////////
//
//   DEVICE-SCAN INTERFACE
//
////////////////////////////////////////////////
void *  												    /* OUT: OutBuffer */
		HWScan::ScanHW_Scan(signed int* outlen,			/* OUT: >=0 OutLEN, <0 ERROR */ // PRQA S 4020
						    unsigned short int command, /* IN: ScanHW_Commands */	
						    signed int param, 			/* IN: CMD Params */
						    void *const cmdbuffer,		/* IN: Databuffer */ 
						    unsigned int cmdlen)		/* IN: Databuffer_length */
{   
	// Multiple returns used to keep the code legible

	*outlen = -255 ;	/* Invalid command */ // PRQA S 4400
	
	switch (command) // PRQA S 3000
	{
    case HWScan::GETMAC :
        {
            return ScanHW_CMD_GETMAC(outlen, param, cmdbuffer, cmdlen) ;
        }
	case HWScan::GETHDDSERIAL :
        {
            return ScanHW_CMD_GETHDDSERIAL(outlen, param, cmdbuffer, cmdlen) ;
        }
    case HWScan::GETHOSTNAME :
        {
			return ScanHW_CMD_GETHOSTNAME(outlen, param, cmdbuffer, cmdlen) ;
        }
    case HWScan::GETCPUID :
        {
			return ScanHW_CMD_GETCPUID(outlen, param, cmdbuffer, cmdlen) ;
        }
    default:
		{ /* nothing */ }
	
	} /* switch (command) */ 

	/* IF HERE: ERROR */

	return 0 ;
}	


////////////////////////////////////////////////
//
//   SCAN HDD FOR SERIAL NUMBER
//
////////////////////////////////////////////////	
void*  														    /* OUT: OutBuffer */
		HWScan::ScanHW_CMD_GETHDDSERIAL(signed int* outlen,		/* OUT: >=0 OutLEN, <0 ERROR */ // PRQA S 4020
									    signed int param, 		/* IN: CMD Params */
									    void* const cmdbuffer,	/* IN: Databuffer */ 
									    unsigned int cmdlen)	/* IN: Databuffer_length */
{
	/*
	*	Input buffer must be a (unterminated) char* with device name
    *   Example: cmdbuffer = "/dev/hda" (cmdlen = 8)
	*   Output buffer can be casted as unsigned char* (unterminated)
	*
    *   USAGE: param
    *     0x00: Get model number
    *     0x01: Get firmware version
    *     0x02: Get serial number
    *     0x10: Get unique-ID
    */
    
     
	// Multiple returns used to keep the code legible
	
	
	int open_flags = O_RDONLY|O_NONBLOCK;
	void* outbuf = 0 ;
	

	*outlen = -3 ;	/* Can't open device */ // PRQA S 4400
	
	if (cmdlen > 1023) { // PRQA S 4400
		return 0 ;	/* Mistake */
    }
	
	char opndev[1024] ; // PRQA S 4403
	memset(opndev, 0, sizeof(opndev));
	memcpy(opndev, cmdbuffer, cmdlen) ;	/* force to be 0-terminated */ 
	
    int fd = open (opndev, open_flags) ;
    if ( fd<0 ) {
		return 0 ;
    }
			
	*outlen = -4 ;	/* Can't ioctl device */ // PRQA S 4400
	struct hd_driveid id ; // PRQA S 4102
	if ( 0 != ioctl(fd, HDIO_GET_IDENTITY, &id)) 
	{
		perror("ScanHW_CMD_GETHDDSERIAL:: HDIO_GET_IDENTITY failed");
		if ( 0 != close(fd) ) ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
		return 0 ;
	}
	if ( 0 != close(fd) ) ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));

	*outlen = -5 ;	/* Internal error */ // PRQA S 4400
	switch (param)
	{
    case 0x00 :
		{
			// Get model number ;
			outbuf = malloc(sizeof(id.model)) ;
			if (!outbuf) { return 0 ; }
			memcpy(outbuf, id.model, sizeof(id.model));
			*outlen = sizeof(id.model) ;
		    break;
		}
		
    case 0x01 :
		{	
			// Get firmware version ;
			outbuf = malloc(sizeof(id.fw_rev)) ;
			if (!outbuf) { return 0 ; }
			memcpy(outbuf, id.fw_rev, sizeof(id.fw_rev));
			*outlen = sizeof(id.fw_rev) ;
		    break;
		}

    case 0x02 : // PRQA S 4402
		{
			// Get serial number ;
			outbuf = malloc(sizeof(id.serial_no)) ;
			if (!outbuf) { return 0 ; }
			memcpy(outbuf, id.serial_no, sizeof(id.serial_no));
			*outlen = sizeof(id.serial_no) ;
		    break;
		}	

    case 0x10 : // PRQA S 4402
		{
			// Get unique_id ;
			outbuf = malloc(sizeof(id.serial_no)+sizeof(id.fw_rev)+sizeof(id.model)) ; // PRQA S 3084
			if (!outbuf) { return 0 ; }
			unsigned int i=0;
			memcpy(reinterpret_cast<unsigned char*>(outbuf)+i, id.model, sizeof(id.model) ); // PRQA S 3081, 3084 6
			i += sizeof(id.model) ;
			memcpy(reinterpret_cast<unsigned char*>(outbuf)+i, id.fw_rev, sizeof(id.fw_rev) ); // PRQA S 3081
			i += sizeof(id.fw_rev) ;
			memcpy(reinterpret_cast<unsigned char*>(outbuf)+i, id.serial_no, sizeof(id.serial_no) ); // PRQA S 3081
			i += sizeof(id.serial_no) ;
			
			*outlen = i ; // PRQA S 3000
		    break;
		}

    default:
		{
			*outlen = -6 ; // Parameter not found  // PRQA S 4400
		}
	} /* switch (param) */ 

	
	return outbuf ;
}


////////////////////////////////////////////////
//
//   SCAN CARD FOR MAC ADDRESS
//
////////////////////////////////////////////////
void*  													    /* OUT: OutBuffer */
		HWScan::ScanHW_CMD_GETMAC(	signed int* outlen,		/* OUT: >=0 OutLEN, <0 ERROR */ // PRQA S 4020
                                    signed int param, 		/* IN: CMD Params */
                                    void* const cmdbuffer,	/* IN: Databuffer */ 
                                    unsigned int cmdlen)	/* IN: Databuffer_length */
{
    //
    // param: Counter of ethernet card to scan. Example: param=0 (1st ethernet card)
    // cmdbuffer: UNUSED. Set cmdbuffer = 0
    // cmdlen: UNUSED. Set cmdlen = 0
    //
    // OUTPUT: pointer to unsigned char[6], with mac address
    //
    
	// Multiple returns used to keep the code legible


    unsigned char*      buffer = 0;
    int   size  = 1;
    struct ifconf       ifc; // PRQA S 4102 2
    struct sockaddr_in  sa;
    unsigned int        valid_card = 0;

    *outlen = -4 ;	/* Can't open socket */ // PRQA S 4400


// TESTED: Works on all active interfaces (ie, up and with IP set)
// TODO: work with all interfaces (?)
// TODO: drop interfaces with the same MAC 

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP) ;
    if ( sockfd < 0 ) {
        return 0 ;
    }
    
    FDGuard guardOnSockfd(sockfd) ; // no close(sockfd) will be required
    
    ifc.ifc_len = size*sizeof(struct ifreq); // PRQA S 3000, 3010, 3084
    ifc.ifc_req = 0;

    do 
    {
        size++;
        
        /* realloc buffer size until no overflow occurs  */
        if ( (ifc.ifc_req = reinterpret_cast<struct ifreq*>(realloc(ifc.ifc_req, size*sizeof(struct ifreq))))==0 )  // PRQA S 3000, 3081, 3084
        {
            *outlen = -3 ;	/* Out of memory */ // PRQA S 4400
            return 0 ;
        }
        ifc.ifc_len = size*sizeof(struct ifreq);  // PRQA S 3000, 3010, 3084
        if (0 != ioctl(sockfd, SIOCGIFCONF, &ifc)) 
        {
            *outlen = -2 ;	/* ioctl failed */ // PRQA S 4400
            return 0 ;
        }

    } while  ( int(size*sizeof(struct ifreq)) <= ifc.ifc_len ); // PRQA S 3000, 3081, 3084

    pattern::CMemoryScopeGuard m(ifc.ifc_req) ;


    unsigned char *u = 0 ; 
    unsigned char ip[4] ; // PRQA S 4403
    for (struct ifreq *ifr=ifc.ifc_req;  // PRQA S 4244
         reinterpret_cast<char *>(ifr) < reinterpret_cast<char *>(ifc.ifc_req)+ifc.ifc_len;  // PRQA S 3030, 3081
         ifr++) 
    {

        if (0 != ioctl(sockfd, SIOCGIFFLAGS, ifr)) {
            continue;  /* failed to get flags, skip it */
        }


        u = reinterpret_cast<unsigned char*>( &( ifr->ifr_addr.sa_data[sizeof(sa.sin_port)] )); // PRQA S 3030, 3081
        memcpy(ip, u, 4);   // IP is evaluated and stored, but is not used... // PRQA S 4400
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "IP-ADDRESS: " << int(ip[0]) << '.' << int(ip[1]) << '.' << int(ip[2]) << '.' << int(ip[3]) ); // PRQA S 3081

        if (0 == ioctl(sockfd, SIOCGIFHWADDR, ifr)) 
        {

          /* Select which  hardware types to process.
           *
           *    See list in system include file included from
           *    /usr/include/net/if_arp.h  (For example, on
           *    Linux see file /usr/include/linux/if_arp.h to
           *    get the list.)
           */
            switch (ifr->ifr_hwaddr.sa_family) // PRQA S 3000
            {
                case  ARPHRD_NETROM:  
                case  ARPHRD_ETHER:  
                case  ARPHRD_PPP:
                case  ARPHRD_EETHER:  
                case  ARPHRD_IEEE802:
                    { break; }
                default:
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Not a valid card. SKIPPED");
                    continue;
                }
            } /* switch */

            u = reinterpret_cast<unsigned char *>( &ifr->ifr_addr.sa_data); // PRQA S 3030, 3081

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MAC ADDRESS: "  << hex << int(u[0]) << ' ' << int(u[1]) << ' ' << int(u[2]) << ' ' << int(u[3]) << ' ' << int(u[4]) << ' ' << int(u[5]) ); // PRQA S 3081

            // HERE IF: Found a valid ethernet card.
            // Check if counter matches param
            if ( valid_card++ != static_cast<unsigned int>(param))  // PRQA S 3081
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CARD# " << valid_card-1 << " PARAM=" << param << " SKIPPED") ; // PRQA S 3084
                continue ;
            }
            // IF HERE:
            // Found a valid card, and counter matches with param

            (*outlen) = -1 ;	/* out ot memory */
            buffer = reinterpret_cast<unsigned char *>(malloc(6)) ; // PRQA S 3081, 4400
            if (!buffer) {
                return 0 ;
            }


            memcpy(buffer, u, 6);// PRQA S 4400 2
            *outlen = 6 ;	 
            return buffer ;
 
        } /* if (!ioctl...) */
    } // for
    // no close is required since FDGuard is used 

    // IF STILL HERE: counter did not match with param
    *outlen = -5 ;	/* not found */	 // PRQA S 4400

    return 0;  
} 



////////////////////////////////////////////////
//
//   GET HOST NAME
//
////////////////////////////////////////////////
void*  													        /* OUT: OutBuffer */
		HWScan::ScanHW_CMD_GETHOSTNAME(	signed int* outlen,		/* OUT: >=0 OutLEN, <0 ERROR */ // PRQA S 4020
                                        signed int param, 		/* IN: CMD Params */
                                        void* const cmdbuffer,	/* IN: Databuffer */ 
                                        unsigned int cmdlen)	/* IN: Databuffer_length */
{
    //
    // param: UNUSED. Set param=0
    // cmdbuffer: UNUSED. Set cmdbuffer = 0
    // cmdlen: UNUSED. Set cmdlen = 0
    //
    // OUTPUT: pointer to char*, with hostname. ZERO TERMINATED.
    //
    
	// Multiple returns used to keep the (simple) code legible
	
	
    char temp[1024] ; // PRQA S 4403
    char* buffer = 0;
       
    *outlen = -3 ;	/* Unable to run command */ // PRQA S 4400
    if ( 0 != gethostname(temp, sizeof(temp)) ) {
        return 0 ;
    }
    
    *outlen = -1 ;	/* Out of memory */
    buffer = reinterpret_cast<char*>( malloc( 1 + strlen(temp) ) ) ; // zero terminated // PRQA S 3081, 3084
    if ( ! buffer ) {
        return 0 ;
    }
        
    memcpy(buffer, temp, (1+strlen(temp)) ) ; // zero terminated    // PRQA S 3084
        
    *outlen = 1+strlen(temp) ;    // zero terminated    	 // PRQA S 3000, 3010, 3084
        
    return buffer ;
}


////////////////////////////////////////////////
//
//   GET CPUID 
//
////////////////////////////////////////////////	
void*  														    /* OUT: OutBuffer */
		HWScan::ScanHW_CMD_GETCPUID(signed int* outlen,		/* OUT: >=0 OutLEN, <0 ERROR */
									signed int param, 		/* IN: CMD Params */
									void* const cmdbuffer,	/* IN: Databuffer */ 
									unsigned int cmdlen)	/* IN: Databuffer_length */
{
	/*
	*	Input buffer must be NULL
	*   Output buffer can be casted as unsigned char* (unterminated)
	*
    *   USAGE: param
    *     0x00: Get CPU vendor string
    *     0x01: Get signature flag
    *     0x11: Get feature flag
    *     0x03: Get Processor Serial Number (PSN)
    */
    
     
	// Multiple returns used to keep the code legible
		
                          
    *outlen = -(0xff) ;	/* Generic error */ // PRQA S 4400
    unsigned char* outbuf = 0 ;

	switch (param)
	{	

    case 0x00 :
		{	
            // Get CPU vendor string
            unsigned char vendorId[12] ; // 12 bytes string // PRQA S 4403
            if ( 0 == ACScpuid_is_supported(vendorId) )
            {
                outbuf = reinterpret_cast<unsigned char*>(calloc(sizeof(vendorId), 1)) ; // PRQA S 3081
                *outlen = sizeof(vendorId) ;
                memcpy(outbuf, vendorId, (*outlen)) ; // PRQA S 3000
            }
            else
            {
                *outlen = -(0xfe) ; // PRQA S 4400
            }
		    break ;
		}
        
    case 0x01 :
		{
			// Get signature flag ;
            unsigned int sign = 0 ;
            if ( 0 == ACScpuid_sign_feat(&sign, 0) )
            {
                unsigned char* ptr = reinterpret_cast<unsigned char*>(&sign) ;  // PRQA S 3030, 3081
                outbuf = reinterpret_cast<unsigned char*>(calloc(sizeof(sign), 1)) ; // PRQA S 3081
                (*outlen) = sizeof(sign) ;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
                memcpy(outbuf, ptr, (*outlen)) ;
#else
                outbuf[0] = ptr[3] ; // PRQA S 4400 4
                outbuf[1] = ptr[2] ; 
                outbuf[2] = ptr[1] ;  
                outbuf[3] = ptr[0] ; 
#endif
            }
            else
            {
                (*outlen) = -(0x01) ;
            }
		    break;
		}
        
    case 0x11 : // PRQA S 4402
		{
			// Get feature flag ;
            unsigned int feat = 0 ;
            if ( 0 == ACScpuid_sign_feat(0, &feat) )
            {
                unsigned char* ptr = reinterpret_cast<unsigned char*>(&feat) ; // PRQA S 3030, 3081
                outbuf = reinterpret_cast<unsigned char*>(calloc(sizeof(feat), 1)) ; // PRQA S 3081
                *outlen = sizeof(feat) ;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
                memcpy(outbuf, ptr, (*outlen)) ;
#else
                outbuf[0] = ptr[3] ; // PRQA S 4400 4
                outbuf[1] = ptr[2] ; 
                outbuf[2] = ptr[1] ;  
                outbuf[3] = ptr[0] ; 
#endif
            }
            else
            {
                *outlen = -(0x11) ; // PRQA S 4400
            }
		    break;
  		}
		
    case 0x03 : // PRQA S 4402
		{	
            // Get Processor Serial Number (PSN)
            unsigned char psn[12] ; // 96 bit PSN // PRQA S 4403
            if ( 0 == ACScpuid_get_psn(psn) )
            {
                outbuf = reinterpret_cast<unsigned char*>(calloc(sizeof(psn), 1)) ; // PRQA S 3081
                *outlen = sizeof(psn) ;
                memcpy(outbuf, psn, (*outlen)) ; // PRQA S 3000
            }
            else
            {
                *outlen = -(0x03) ; // PRQA S 4400
            }
		    break;
		}
		
    default:
		{
            *outlen = -6 ; // Parameter not found  // PRQA S 4400
		}
	} /* switch (param) */ 

  
	return outbuf ;
}


string HWScan::hostname() 
{   
 	char hname[1024] ; // PRQA S 4403
	// gethostname unspecifies if the returned value is zero terminated
	memset(hname,0,sizeof(hname)) ; 
	if (0 == gethostname(hname, sizeof(hname)-1)) { // PRQA S 3084
		return hname ;
    }
	else {
		ACS_THROW(exIOException("Cannot get local hostname", errno)) ; // PRQA S 3081
	}
}


void HWScan::dump(unsigned char* buffer, size_t n) 
{
	for (size_t i=0; i<n; i++) { 
        ACS_LOG_DEBUG(i << " " << hex << int(buffer[i])) ; // PRQA S 3081
    } ;
	return ;
}


map <string, HWScan::HWScanNetworkInterfaceStruct> HWScan::getNetworkInterfacesMap(bool omit_domain, bool omit_loopback) // PRQA S 4020
{
	map <string, HWScanNetworkInterfaceStruct> m ; 

	const unsigned char null_ipv4[] = { 0,0,0,0 }  ;	// Empty (undefined) IPv4 address 
	const unsigned char null_ipv6[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } ;	// Empty (undefined) IPv6 address 

	struct ifaddrs *ifaddr = 0 ;
	if (getifaddrs(&ifaddr) == -1) { return m ; }

	for (struct ifaddrs *ifa = ifaddr; ifa != 0; ifa = ifa->ifa_next) {

		// ifa->if_addr is the ptr to one of these structures, depending from the value of ifa->ifa_addr->sa_family 
		// (that is ALWAYS the first short in the structure) :
		// - Family: AF_INET. Ptr: sockaddr_in*
		// - Family: AF_INET6. Ptr: sockaddr_in6*  (THIS STRUCT IS LARGER THAN THE "BASE" ONE!!!)
		// - Family: Other(TBC). Ptr: sockaddr (Original structure, obsolete)

		string addr ;
        string host ; 

		const int family = ifa->ifa_addr->sa_family; // PRQA S 3000
		const string ifname = ifa->ifa_name ;

		if (omit_loopback && (0 == strncmp(ifname.c_str(), "lo", 2)) ) { // PRQA S 4400
              
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Skipping loopback interface: \"" << ifname << "\"") ;
				
				continue ;
		}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing interface: \"" << ifname << "\"") ;

		switch(family) {

        case AF_INET: 
			{
				const sockaddr_in *s = reinterpret_cast<const sockaddr_in*> (ifa->ifa_addr) ; // PRQA S 3030, 3081 2
				const unsigned char* p = reinterpret_cast<const unsigned char*> (&s->sin_addr) ;
				char caddr[64] ; // PRQA S 4403
                memset(caddr, 0, sizeof(caddr)) ; 

				if (0 != memcmp(p, null_ipv4, sizeof(null_ipv4))) 
				{
					sprintf(caddr, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]) ; // PRQA S 4400
					addr = caddr ; 
				} else {
					// IPv4 is undefined 
					addr = "" ; 
				}

				break ;
			}

        case AF_INET6 :
			{
				const sockaddr_in6 *s = reinterpret_cast<const sockaddr_in6*>( ifa->ifa_addr ); // PRQA S 3030, 3081 2
				const unsigned char* p = reinterpret_cast<const unsigned char*> (&s->sin6_addr) ;
				char caddr[64] ; // PRQA S 4403
                memset(caddr, 0, sizeof(caddr)) ; 

				if (0 != memcmp(p, null_ipv6, sizeof(null_ipv6))) 
				{
					// IPv6 is defined (not all-zeros)
					sprintf(caddr, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x", 
						p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], // PRQA S 4400 2
						p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15] ) ;
					addr = caddr ; 
				} else {
					// IPv6 is undefined 
					addr = "" ; 
				}

				break ;
			}

        default :
            {
			
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Skipping interface: \"" << ifname << "\"  (family not supported)") ;

				continue ; // Unsupported family: Just ignore the entry and check the next one
            }
		} // END: switch


		// IF HERE: family is either AF_INET or AF_INET6


		if ((ifname != "") || (addr != ""))
		{
			char chost[NI_MAXHOST];
			memset(chost, 0, sizeof(chost)) ;

			if (0 == getnameinfo(ifa->ifa_addr, 
                              ((family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6)),  // PRQA S 3380
			                  chost, 
                              sizeof(chost), 
                              0, 
                              0, 
                              NI_NAMEREQD|(omit_domain? NI_NOFQDN:0) ) ) { // PRQA S 3000, 3003, 3380
				host = chost ; 
			} else {
				host = "" ; 
			}

			HWScanNetworkInterfaceStruct s ; 

			if (m.find(ifname) != m.end()) { s = m[ifname] ; } // Retrieve current value for HWScanNetworkInterfaceStruct

			// Fill HWScanNetworkInterfaceStruct with new values 
			s.ifname = ifname ;	

			if (addr!="")
			{
				if ( strchr(addr.c_str(), '.') && !strchr(addr.c_str(), ':') ) {
					s.ipv4 = addr ; 
				} else if ( strchr(addr.c_str(), ':') && !strchr(addr.c_str(), '.') ) {
					s.ipv6 = addr ; 
				} 
                else { /* nothing */ }  
			}

			if (host != "") { s.hostname = host ; }

			m[ifname] = s ; // Store current value to HWScanNetworkInterfaceStruct
		
		}	// END: if ((ifname != "") && (addr != ""))

	}	// END: for
	
	freeifaddrs(ifaddr);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. Returning: " << m.size() << " devices") ;

	return m ; 
}

_ACS_END_NAMESPACE
