// PRQA S 1050 EOF
////////////////////////////////////////////////////////////////////////////////////////////
// 
// 	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
//  $Prod: A.C.S. HWScan class header $
// 
// 	$Id$
// 
// 	$Author$
// 
//  $Log$
//  Revision 2.8  2012/12/17 18:46:55  marpas
//  MemoryGuard changed into CMemoryScopeGuard
//
//  Revision 2.7  2012/11/29 19:31:29  marpas
//  implementing qa rules
//  optimizing code and coding best practices
//  coverage framework initiated
//
//  Revision 2.6  2012/02/07 17:57:44  marpas
//  refactoring in progress
//
//  Revision 2.5  2011/11/04 10:32:54  enrcar
//  EC:: added static method HWScan::getNetworkInterfacesMap
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
//  Revision 2.1  2006/09/19 15:11:18  danalt
//  fixed for gcc 4.1.0
//
//  Revision 2.0  2006/02/28 08:33:12  marpas
//  Exception 2.1 I/F adopted
//
//  Revision 1.4  2005/11/04 17:01:21  enrcar
//  hostname method added
//
//  Revision 1.3  2005/11/03 12:51:52  enrcar
//  Added method: ScanHW_CMD_GETHOSTNAME
//  Parameter GETHOSTNAME supported.
//
//  Revision 1.2  2005/11/02 14:32:58  enrcar
//  GuardFileDesc, MemoryGuard classes added
//  GETMAC command implemented && tested
//
//
////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _HWScan_H_
#define _HWScan_H_


#include <acs_c++config.hpp>

#include <exException.h>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)


/*!
	class HWScan
 */
class HWScan // PRQA S 2109
{

/*
	USAGE:
	=============================

	INPUT must be a structure* or an unterminated char*
	INPUT pointers: outlen and cmdbuffer must be allocated by user.
	OUTPUT will be a structure* or (more likely) an unterminated char*
	OUTPUT pointer is ALLOCATED by function, using MALLOC


	CHECK RETURN VALUE:
	=============================

	RETURN IS VALID IF:
	(*outlen) >= 0
	return pointer is >0

	IN CASE OF PROBLEMS:
	(*outlen) is errorcode<0
	return pointer should be 0
	(No memory will be allocated)


	R E M E M B E R :
	=============================
	Always check (*outlen)>=0 and deallocate the returned pointer with free()

*/

public:  
    enum HWScanCommands	{	
        GETMAC=0, 
        GETHDDSERIAL=1,
        GETHOSTNAME=2,
        GETCPUID=3
	} ;

	struct HWScanNetworkInterfaceStruct { // PRQA S 2173, 2175
        HWScanNetworkInterfaceStruct() throw() : 
            ifname(), 
            ipv4(), 
            ipv6(), 
            hostname() 
        {}
        // PRQA S 2100 5
		std::string ifname ; 
		std::string ipv4 ; 
		std::string ipv6 ; 
		std::string hostname ; 
	} ;

	/*! class ThreadException declaration */
	exDECLARE_EXCEPTION(HWScanInternalError,exException) ; // PRQA S 2131, 2502

    HWScan() throw() ;
    ~HWScan() throw() ;

public:

static void* ScanHW_Scan(signed int *outlen, unsigned short int command, 
    signed int param, void *const cmdbuffer, unsigned int cmdlen) ;

static void* ScanHW_CMD_GETMAC(signed int* outlen, 
    signed int param, void* const cmdbuffer, unsigned int cmdlen) ;

static void* ScanHW_CMD_GETHDDSERIAL(signed int* outlen, 
	signed int param, void* const cmdbuffer, unsigned int cmdlen) ;

static void* ScanHW_CMD_GETHOSTNAME(signed int* outlen, 
	signed int param, void* const cmdbuffer, unsigned int cmdlen) ;

static void* ScanHW_CMD_GETCPUID(signed int* outlen, 
	signed int param, void* const cmdbuffer, unsigned int cmdlen) ;

/**
 * returns the local hostname
 */
static std::string hostname() ;

/**
 * returns a map of all the interfaces in the host
 */
static std::map<std::string, HWScanNetworkInterfaceStruct> getNetworkInterfacesMap(bool omit_domain, bool omit_loopback=false) ;

/**
 * memory dump
 */
static void dump(unsigned char* buffer, size_t n) ; 

private: 
	HWScan(const HWScan&) ;
	HWScan &operator=(const HWScan&) ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(HWScan) ;

} ;

_ACS_END_NAMESPACE

#endif	/* _HWScan_H_ */
