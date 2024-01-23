// PRQA S 1050 EOF
/*

	Copyright 1995-2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DBXpress for Postgres $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2012/12/10 17:02:29  marpas
	qa rules
	
	Revision 1.6  2012/08/07 12:51:16  enrcar
	EC:: Bug fixed. An invalid representation of negative numbers (2's complement instead of mod/sign) was provided. Fixed.
	
	Revision 1.5  2012/05/17 09:17:00  lucio.pulvirenti
	Methods: jd_to_YMD and jd2000secs_to_YMDhms added
	Unused, commented code removed
	
	Revision 1.4  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.3  2010/10/27 08:33:33  enrcar
	EC:: number of digits in fraction always set to "9", nsec (even if PG actually has a lower resolution, usec)
	
	Revision 1.2  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.1  2008/03/19 15:34:37  enrcar
	EC:: Imported source
	
	

*/

#include "pglerror.hpp"
#include "pgldefs.hpp"

#include <math.h>
#include <memory.h>

extern "C" const unsigned short ctstmp_fraction_digits = 9 ;


/////////////////////////////////////////////////////////////////////////////
// INT8 to BCD Conversion function

void int8_to_bcd(signed long long int *pint8, FMTBcd *pbcd)
{
	memset(pbcd, 0, sizeof(FMTBcd)) ;

	BYTE digits[INT8_AS_DECIMAL_PRECISION];
	int precision = 0;
	
	if ((*pint8)<0) {
		pbcd->iSignSpecialPlaces = 0x80 ;
		*pint8 = -(*pint8) ;
	}

    unsigned char* plow = reinterpret_cast<unsigned char*>(pint8) ;
    unsigned char* phigh = 4+reinterpret_cast<unsigned char*>(pint8) ;
    uint4 low = *reinterpret_cast<uint4*>(plow) ;
	uint4 high = *reinterpret_cast<uint4*>(phigh) ;

	pBYTE digit = &digits[INT8_AS_DECIMAL_PRECISION];
	while (high != 0) {
		uint4 high_remainder = high % 10;
		uint4 full_remainder = high_remainder*6 + low % 10;
		high /= 10;
		(low /= 10) += (high_remainder*429496729) + (full_remainder/10);
		*(--digit) = static_cast<BYTE>(full_remainder % 10);
		precision++;
	}
	while (low != 0) {
		*(--digit) = static_cast<BYTE>(low % 10);
		low /= 10;
		precision++;
	}
	pbcd->iPrecision = precision;
	pBYTE nibble = pbcd->iFraction;
	while (precision-- > 0)
	{
		*nibble = *digit++ << 4;
		if (precision-- == 0)
			break;
		*nibble++ |= *digit++;
	}

    #if(0)
    {
    	/* LOW LEVEL DEBUG -- TO BE REMOVED FURTHER*/
        int i;
        unsigned char* p = (unsigned char*) pbcd ;
        printf("method int8_to_bcd: input %ld\n", *pint8) ;
        for (unsigned int i=0; i<sizeof(FMTBcd); i++) 
        { printf("\tDUMPING: %d 0x%02x\n", i, p[i]); }
    }
    #endif

	return ;
}


/////////////////////////////////////////////////////////////////////////////
// Julian date conversion utilities :

//void jd_to_YMD(size_t jd, size_t& year, size_t& month, size_t& day) FIXME CUT
void jd_to_YMD(size_t jd, INT16& year, UINT16& month, UINT16& day) 
{
	size_t Z = jd /* +0.5 */ ;  /* Fraction omitted since an integer value is used as input */ 
	size_t W = static_cast<size_t> ((Z - 1867216.25)/36524.25) ;
	size_t X = W/4 ;
	size_t A = Z+1+W-X ;
	size_t B = A+1524 ;
	size_t C = static_cast<size_t> ((B-122.1)/365.25) ;
	size_t D = static_cast<size_t> (365.25 * C) ;
	size_t E = static_cast<size_t> ((B-D)/30.6001) ;
	size_t F = static_cast<size_t> (30.6001 * E) ;

	day = static_cast<UINT16>( B-D-F ) ;

	if (E<=13) { month = static_cast<UINT16>( E-1 ) ; year -- ;}
	else
	{ month = static_cast<UINT16>( E-13 ) ; }

	year = static_cast<INT16>( C-4715 ) ; if (month>2) year -- ;
}


// FIXME CUT void jd2000secs_to_YMDhms(signed long long int jd2000sec, size_t& year, size_t& month, size_t& day, size_t& h, size_t& m, size_t& s)
void jd2000secs_to_YMDhms(signed long long int jd2000sec, INT16& year, UINT16& month, UINT16& day, UINT16& h, UINT16& m, UINT16& s) 
{
	// Below, the DOY is computed (without the seconds, evaluated below), as INTEGER NUMBER OF WHOLE DAYS elapsed prior the date to convert
	// E.G. : 2000-01-01 12:00, the number of whole days is zero
	// If the date is negative, as INTEGER NUMBER OF WHOLE DAYS elapsed ALSO INCLUDING the current date to convert
	// E.G. : 1999-12-31 12:00, the number of whole days is -1
	signed long long int jd2000 = ( (jd2000sec<0)? (((jd2000sec+1)/86400)-1) : (jd2000sec/86400) ) ;	/* Rounded as CEIL (insted of usual FLOOR) if negative*/

	// Below, the number of seconds (ALWAYS POSITIVE, since size_t is used) elapsed from hour 00:00 of the converted DOY (computed above)
	// This figure will be added to the DOY
	size_t total_secs = ( (jd2000sec+(2451545LL * 86400LL)) % (86400LL) ) ;

	h = static_cast<UINT16> ( total_secs / 3600 ) ;
	m = static_cast<UINT16> ( (total_secs % 3600) / 60 ) ;
	s = static_cast<UINT16> ( total_secs % 60 ) ;

    jd_to_YMD( static_cast<ssize_t>(jd2000 + ssize_t(2451545)), year, month, day) ;
}


#if(0)
static void jd2000_to_YMD(double jd2000, size_t& year, size_t& month, size_t& day)
{
        jd_to_YMD( (jd2000 + 2451545.0), year, month, day) ;
}
#endif
