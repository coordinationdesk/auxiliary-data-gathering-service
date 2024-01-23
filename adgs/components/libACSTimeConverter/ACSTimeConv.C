// PRQA S 1050 EOF
/* 

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.8  2014/12/12 16:42:30  enrcar
	EC:: Added method: getLeapSecondTimeDifferences
	
	Revision 2.7  2014/04/14 13:59:24  marpas
	coding best practices applied
	performances boost (up to 100x)
	interface rationalization
	coverage test
	
	Revision 2.6  2014/04/10 18:18:50  marpas
	work in progress: do not use
	
	Revision 2.5  2014/04/10 11:14:57  marpas
	refactoring in progress - do not use
	
	Revision 2.4  2014/04/09 16:31:30  marpas
	refactoring in progress - do not use for stable
	
	Revision 2.3  2014/02/03 14:19:38  giucas
	New macros adopted
	
	Revision 2.2  2006/03/21 12:26:14  seralb
	nanoseconds check modified in convert_JD50_StandardFormat method
	
	Revision 2.1  2006/03/21 10:53:11  seralb
	nanoseconds computation modified in convert_JD50_StandardFormat method
	
	Revision 2.0  2006/03/01 16:29:25  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.22  2006/03/01 16:28:43  serfol
	nothing
	
	Revision 1.21  2005/10/12 11:02:02  seralb
	Added ascii cosmo compact
	
	Revision 1.20  2005/09/16 11:41:37  seralb
	Added conversion for Ers Millisec Ascii Format
	
	Revision 1.19  2005/06/14 08:54:46  seralb
	Bug solved in convert_JD50_StandardFormat method
	
	Revision 1.18  2005/06/13 16:12:03  seralb
	Bug solved in convert_JD50_StandardFormat method
	
	Revision 1.17  2005/03/03 14:38:31  seralb
	Cosmo format and long double support
	
	Revision 1.16  2005/01/03 14:36:44  ivafam
	The constructor of classes deriving from dbFileAction becomes protected.
	
	Revision 1.15  2004/12/17 10:29:47  seralb
	Now jd50 instead of jd
	
	Revision 1.14  2004/12/13 17:08:46  seralb
	Configuration file for reference conversion and JD instead of JD2000
	
	Revision 1.13  2004/12/06 16:09:36  seralb
	Upgrade
	
	Revision 1.12  2004/12/06 11:31:57  seralb
	Added UT1 conversion
	
	Revision 1.11  2004/12/03 15:47:39  seralb
	Added method and GPS conversion implemented
	
	Revision 1.10  2004/12/03 13:22:25  seralb
	Header fixed
	
	Revision 1.9  2004/12/02 16:07:18  seralb
	Bug solved
	
	Revision 1.8  2004/12/01 16:36:07  seralb
	Some names changed and a bug solved
	
	Revision 1.7  2004/12/01 15:09:47  seralb
	Added constructor and similar for the structure
	
	Revision 1.6  2004/12/01 14:20:36  seralb
	Now there is just one public method
	
	Revision 1.5  2004/11/30 16:09:26  seralb
	now int are long int
	
	Revision 1.4  2004/11/30 14:43:15  seralb
	Some bug solved
	
	Revision 1.3  2004/11/29 16:57:17  seralb
	Now methods are global
	
	Revision 1.2  2004/11/26 14:14:23  seralb
	Upgrade
	
	Revision 1.1.1.1  2004/11/25 16:27:46  seralb
	Import libACSTimeConverter
	
	
 */


#include <ACSTimeConv.h>
#include <ACSTimeConvV.h>
#include <ConfigurationSingleton.h>
#include <StringUtils.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACSTimeConv)

namespace {
	ACSTimeConvV vers;
    const long double SecondsInADay = 86400.0L ;
    const long double mSecondsInADay = SecondsInADay*1e3L ; // ms
    const long double uSecondsInADay = SecondsInADay*1e6L ; // us
    const long double nSecondsInADay = SecondsInADay*1e9L ; // ns

	const string _UTC("UTC") ;
	const string _TAI("TAI") ;
	const string _UT1("UT1") ;
	const string _GPS("GPS") ;

    inline string toString(ACSTimeConv::TimeReferenceDefines tr, bool throwIfError = true)   // PRQA S 2134, 2502, 4020
    {
        switch(tr) {
        case ACSTimeConv::TAI: { return "TAI" ; } 
        case ACSTimeConv::UTC: { return "UTC" ; } 
        case ACSTimeConv::UT1: { return "UT1" ; } 
        case ACSTimeConv::GPS: { return "GPS" ; } 
        default: 
            { 
                ostringstream os ; os << "Unrecognized TimeReferenceDefines(" << int(tr) << ")" ; // PRQA S 2134, 3081 2
                if (throwIfError) {
                    ACS_THROW(exIllegalValueException(os.str())) ; // PRQA S 3081
                }
                else { return os.str() ; }
            }
        }
    }
    
    inline bool isRefGood(int tr) // PRQA S 2134, 4020
    {
        switch(tr) {
        case ACSTimeConv::TAI: 
        case ACSTimeConv::UTC: 
        case ACSTimeConv::UT1: 
        case ACSTimeConv::GPS: 
            { return true ; }
        default: 
            { return false ; }
        }
    }

    inline string tr2s(int r) { return toString(ACSTimeConv::TimeReferenceDefines(r), false) ; } // PRQA S 2134, 3013, 3081
    
    inline ACSTimeConv::TimeReferenceDefines fromString(const string &s) // PRQA S 2134, 4020
    {
 	    if (_TAI == s) { return ACSTimeConv::TAI ; }
 	    if (_UT1 == s) { return ACSTimeConv::UT1 ; }
 	    if (_GPS == s) { return ACSTimeConv::GPS ; }
 	    if (_UTC == s) { return ACSTimeConv::UTC ; }
        ACS_THROW(exIllegalValueException("Unrecognized \"" + s + "\" as TimeReferenceDefines")) ;  // PRQA S 3081
    }
    
    inline void checkInRef(int r, const string &s) // PRQA S 2134
    {
        const string &ref = toString(ACSTimeConv::TimeReferenceDefines(r)) + '=' ; // PRQA S 3013, 3081
        if (s.substr(0,4) != ref) { // PRQA S 4400
            ACS_THROW(exIllegalValueException("\"" + s + "\" date does not start with: \"" + ref + "\"")) ; // PRQA S 3081
        }
    }

    inline string toString(ACSTimeConv::TimeFormatDefines tf, bool throwIfError = true)   // PRQA S 2134, 2502, 4020
    {
        switch(tf) {
	    case ACSTimeConv::PROCESSING: { return "PROCESSING" ; } 

	    case ACSTimeConv::TRANSPORT_DAY_SEC_USEC: { return "TRANSPORT_DAY_SEC_USEC" ; }
	    case ACSTimeConv::TRANSPORT_DAY_MSEC_USEC: { return "TRANSPORT_DAY_MSEC_USEC" ; }

	    case ACSTimeConv::ASCII_STD: { return "ASCII_STD" ; }
	    case ACSTimeConv::ASCII_STD_REF: { return "ASCII_STD_REF" ; }
	    case ACSTimeConv::ASCII_STD_MICROSEC: { return "ASCII_STD_MICROSEC" ; }
	    case ACSTimeConv::ASCII_STD_REF_MICROSEC: { return "ASCII_STD_REF_MICROSEC" ; }

	    case ACSTimeConv::ASCII_COMPACT: { return "ASCII_COMPACT" ; }
	    case ACSTimeConv::ASCII_COMPACT_REF: { return "ASCII_COMPACT_REF" ; }
	    case ACSTimeConv::ASCII_COMPACT_MICROSEC: { return "ASCII_COMPACT_MICROSEC" ; }
	    case ACSTimeConv::ASCII_COMPACT_REF_MICROSEC: { return "ASCII_COMPACT_REF_MICROSEC" ; }

	    case ACSTimeConv::ASCII_ENVI: { return "ASCII_ENVI" ; }
	    case ACSTimeConv::ASCII_ENVI_REF: { return "ASCII_ENVI_REF" ; }
	    case ACSTimeConv::ASCII_ENVI_MICROSEC: { return "ASCII_ENVI_MICROSEC" ; }
	    case ACSTimeConv::ASCII_ENVI_REF_MICROSEC: { return "ASCII_ENVI_REF_MICROSEC" ; }

	    case ACSTimeConv::ASCII_CCSDSA: { return "ASCII_CCSDSA" ; }
	    case ACSTimeConv::ASCII_CCSDSA_REF: { return "ASCII_CCSDSA_REF" ; }
	    case ACSTimeConv::ASCII_CCSDSA_MICROSEC: { return "ASCII_CCSDSA_MICROSEC" ; }
	    case ACSTimeConv::ASCII_CCSDSA_REF_MICROSEC: { return "ASCII_CCSDSA_REF_MICROSEC" ; }
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT: { return "ASCII_CCSDSA_COMPACT" ; }
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_REF: { return "ASCII_CCSDSA_COMPACT_REF" ; }
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_MICROSEC: { return "ASCII_CCSDSA_COMPACT_MICROSEC" ; }
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_REF_MICROSEC: { return "ASCII_CCSDSA_COMPACT_REF_MICROSEC" ; }

	    case ACSTimeConv::ASCII_COSMO_NANOSEC: { return "ASCII_COSMO_NANOSEC" ; }
	    case ACSTimeConv::ASCII_COSMO_MICROSEC: { return "ASCII_COSMO_MICROSEC" ; }
	    case ACSTimeConv::ASCII_COSMO_MILLISEC: { return "ASCII_COSMO_MILLISEC" ; }
	    case ACSTimeConv::ASCII_COSMO_SEC: { return "ASCII_COSMO_SEC" ; }
	    case ACSTimeConv::ASCII_COSMO_COMPACT: { return "ASCII_COSMO_COMPACT" ; }

	    case ACSTimeConv::ASCII_ERS_MILLISEC: { return "ASCII_ERS_MILLISEC" ; }
        default: 
            { 
                ostringstream os ; os << "Unrecognized TimeFormatDefines(" << int(tf) << ")" ; // PRQA S 3081 2
                if (throwIfError) {
                    ACS_THROW(exIllegalValueException(os.str())) ; 
                }
                else { return os.str() ; }
            }
        }
    }
    
    inline bool isFmtGood(int tf) // PRQA S 2134, 4020
    {
        switch(tf) {
	    case ACSTimeConv::PROCESSING:  

	    case ACSTimeConv::TRANSPORT_DAY_SEC_USEC: 
	    case ACSTimeConv::TRANSPORT_DAY_MSEC_USEC: 

	    case ACSTimeConv::ASCII_STD: 
	    case ACSTimeConv::ASCII_STD_REF: 
	    case ACSTimeConv::ASCII_STD_MICROSEC: 
	    case ACSTimeConv::ASCII_STD_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_COMPACT: 
	    case ACSTimeConv::ASCII_COMPACT_REF: 
	    case ACSTimeConv::ASCII_COMPACT_MICROSEC: 
	    case ACSTimeConv::ASCII_COMPACT_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_ENVI: 
	    case ACSTimeConv::ASCII_ENVI_REF: 
	    case ACSTimeConv::ASCII_ENVI_MICROSEC: 
	    case ACSTimeConv::ASCII_ENVI_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_CCSDSA: 
	    case ACSTimeConv::ASCII_CCSDSA_REF: 
	    case ACSTimeConv::ASCII_CCSDSA_MICROSEC: 
	    case ACSTimeConv::ASCII_CCSDSA_REF_MICROSEC: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_REF: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_MICROSEC: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_COSMO_NANOSEC: 
	    case ACSTimeConv::ASCII_COSMO_MICROSEC: 
	    case ACSTimeConv::ASCII_COSMO_MILLISEC: 
	    case ACSTimeConv::ASCII_COSMO_SEC: 
	    case ACSTimeConv::ASCII_COSMO_COMPACT: 

	    case ACSTimeConv::ASCII_ERS_MILLISEC: 
            { return true ; }
        default: 
            { return false ; }
        }
    }

    inline string tf2s(int f) { return toString(ACSTimeConv::TimeFormatDefines(f), false) ; } // PRQA S 2134, 3013, 3081
    
}

/*
 * Class constructor
 */
ACSTimeConv::ACSTimeConv() :
	_tD(),
	_timeDiff()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"ACSTimeConv::Id: " << Id )
	
	loadConfiguration();
}



long double ACSTimeConv::toJD50(const ACSTimeConv::TimeConv& in) const // PRQA S 4020
{
	/* Control the input format and call the right method to convert it to standard format */
	switch(in.format) {
    case PROCESSING:
        {
		    return in.timeD ;
        }
	case TRANSPORT_DAY_SEC_USEC:
        {
		    return fromTransportTime(in.timeI, true); 
        }
	case TRANSPORT_DAY_MSEC_USEC:
        {
		    return fromTransportTime(in.timeI, false); 
        }
	case ASCII_STD:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_STD_REF:
        {
		    return fromAscii( (checkInRef(in.ref, in.timeS), in.timeS.substr(4)) ) ; // PRQA S 4400 L1
        }
	case ASCII_STD_MICROSEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_STD_REF_MICROSEC:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), in.timeS.substr(4) ) ) ;
        }
	case ASCII_COMPACT:
        {
		    return fromAscii(StringUtils::transform(in.timeS,'_','T')) ;
        }
	case ASCII_COMPACT_REF:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), StringUtils::transform(in.timeS.substr(4),'_','T') ) ) ;
        }
	case ASCII_COMPACT_MICROSEC:
        {
		    return fromAscii(StringUtils::transform(in.timeS.substr(0,15), '_', 'T') + '.' + in.timeS.substr(15)) ;
        }
	case ASCII_COMPACT_REF_MICROSEC:
        {
		    return fromAscii( 
                             (  checkInRef(in.ref, in.timeS),
                                StringUtils::transform(in.timeS.substr(4,15), '_', 'T') + '.' + in.timeS.substr(19)
                             )
                            ) ;
        }
	case ASCII_ENVI:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_ENVI_REF:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), in.timeS.substr(4) ) ) ;
        }
	case ASCII_ENVI_MICROSEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_ENVI_REF_MICROSEC:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), in.timeS.substr(4) ) ) ;
        }
	case ASCII_CCSDSA:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_CCSDSA_REF:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), in.timeS.substr(4) ) ) ;
        }
	case ASCII_CCSDSA_MICROSEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_CCSDSA_REF_MICROSEC:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), in.timeS.substr(4) ) ) ;
        }
	case ASCII_CCSDSA_COMPACT:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_CCSDSA_COMPACT_REF:
        {
		    return fromAscii( ( checkInRef(in.ref, in.timeS), in.timeS.substr(4) ) ) ;
        }
	case ASCII_CCSDSA_COMPACT_MICROSEC:
        {
		    return fromAscii( in.timeS.substr(0,15) + '.' + in.timeS.substr(15)) ;
        }
	case ASCII_CCSDSA_COMPACT_REF_MICROSEC:
        {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"in.timeS: " << in.timeS )
		    return fromAscii( ( checkInRef(in.ref, in.timeS), 
                                in.timeS.substr(4,15) + '.' + in.timeS.substr(19)) ) ;
        }
	case ASCII_COSMO_NANOSEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_COSMO_MICROSEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_COSMO_MILLISEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_COSMO_SEC:
        {
		    return fromAscii(in.timeS);
        }
	case ASCII_COSMO_COMPACT:
        {
		    return fromAscii( in.timeS.substr(0,8) + 'T' + in.timeS.substr(8)) ; // PRQA L:L1
        }
	case ASCII_ERS_MILLISEC:
        {
		    return fromAscii(in.timeS);
        }
	default: 
	    {
		    ACS_THROW(ACSTimeConvException("Unrecognized input format: " + tf2s(in.format))) ; // PRQA S 3081
	    }
    }
}


void ACSTimeConv::convertTimeInternal(long double date, TimeConv& out) const
{	
	/*	Control the output format and call the right conversion method	*/
	switch(out.format) {
    case PROCESSING: {
		out.timeD = date ;
        break ;
    }
	case TRANSPORT_DAY_SEC_USEC: {
		toTransportTimeDSU(date,out.timeI); // PRQA S 4400 L2
        break ;
    }
	case TRANSPORT_DAY_MSEC_USEC: {
		toTransportTimeDMU(date,out.timeI); 
        break ;
    }
	case ASCII_STD: {
		out.timeS = toAsciiStd(date, TimeReferenceDefines(out.ref), false, 0); // PRQA S 3013, 3081 L2
        break ;
    }
	case ASCII_STD_REF: {
		out.timeS = toAsciiStd(date, TimeReferenceDefines(out.ref), true, 0);
        break ;
    }
	case ASCII_STD_MICROSEC: {
		out.timeS = toAsciiStd(date, TimeReferenceDefines(out.ref), false, 6);
        break ;
    }
	case ASCII_STD_REF_MICROSEC: {
		out.timeS = toAsciiStd(date, TimeReferenceDefines(out.ref), true, 6);
        break ;
    }
	case ASCII_COMPACT: {
		out.timeS = toAsciiCompact(date, TimeReferenceDefines(out.ref), false, 0);
        break ;
    }
	case ASCII_COMPACT_REF: {
		out.timeS = toAsciiCompact(date, TimeReferenceDefines(out.ref), true, 0);
        break ;
    }
	case ASCII_COMPACT_MICROSEC: {
		out.timeS = toAsciiCompact(date, TimeReferenceDefines(out.ref), false, 6);
        break ;
    }
	case ASCII_COMPACT_REF_MICROSEC: {
		out.timeS = toAsciiCompact(date, TimeReferenceDefines(out.ref), true, 6);
        break ;
    }
	case ASCII_ENVI: {
		out.timeS = toAsciiEnvi(date, TimeReferenceDefines(out.ref), false, 0) ;
        break ;
    }
	case ASCII_ENVI_REF: {
		out.timeS = toAsciiEnvi(date, TimeReferenceDefines(out.ref), true, 0);
        break ;
    }
	case ASCII_ENVI_MICROSEC: {
		out.timeS = toAsciiEnvi(date, TimeReferenceDefines(out.ref), false, 6);
        break ;
    }
	case ASCII_ENVI_REF_MICROSEC: {
		out.timeS = toAsciiEnvi(date, TimeReferenceDefines(out.ref), true, 6);
        break ;
    }
	case ASCII_CCSDSA: {
		out.timeS = toCcsdsa(date, TimeReferenceDefines(out.ref), false, 0);
        break ;
    }
	case ASCII_CCSDSA_REF: {
		out.timeS = toCcsdsa(date, TimeReferenceDefines(out.ref), true, 0);
        break ;
    }
	case ASCII_CCSDSA_MICROSEC: {
		out.timeS = toCcsdsa(date, TimeReferenceDefines(out.ref), false, 6);
        break ;
    }
	case ASCII_CCSDSA_REF_MICROSEC: {
		out.timeS = toCcsdsa(date, TimeReferenceDefines(out.ref), true, 6);
        break ;
    }
	case ASCII_CCSDSA_COMPACT: {
		out.timeS = toCcsdsaCompact(date, TimeReferenceDefines(out.ref), false, 0);
        break ;
    }
	case ASCII_CCSDSA_COMPACT_REF: {
		out.timeS = toCcsdsaCompact(date, TimeReferenceDefines(out.ref), true, 0);
        break ;
    }
	case ASCII_CCSDSA_COMPACT_MICROSEC: {
		out.timeS = toCcsdsaCompact(date, TimeReferenceDefines(out.ref), false, 6);
        break ;
    }
	case ASCII_CCSDSA_COMPACT_REF_MICROSEC: {
		out.timeS = toCcsdsaCompact(date, TimeReferenceDefines(out.ref), true, 6);
        break ;
    }
	case ASCII_COSMO_NANOSEC: {
		out.timeS = toCosmo(date, false, 9) ;
        break ;
    }
	case ASCII_COSMO_MICROSEC: {
		out.timeS = toCosmo(date, false, 6) ;
        break ;
    }
	case ASCII_COSMO_MILLISEC: {
		out.timeS = toCosmo(date, false, 3) ;
        break ;
    }
	case ASCII_COSMO_SEC: {
		out.timeS = toCosmo(date, false, 0) ;
        break ;
    }
	case ASCII_COSMO_COMPACT: {
		out.timeS = toCosmo(date, true, 0) ; // PRQA L:L2
        break ;
    }
	case ASCII_ERS_MILLISEC: {
		out.timeS = toAsciiErsMillisec(date);
        break ;
    }
	default: 
	    {
		    ACS_THROW(ACSTimeConvException("Unrecognized output format: " + tf2s(out.format))) ; // PRQA S 3081
	    }
    }
}


void ACSTimeConv::convertTime(const TimeConv& in, TimeConv& out) const	
{
    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Reference (in-out):"<< in.ref << "-" << out.ref 
                                        << " Format (in-out):"<< in.format << "-" << out.format)  
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Date in (string|double|long): " << setprecision(30)<< '|' << in.timeS << '|' << in.timeD <<'|'<< in.timeI[0] << "." <<in.timeI[1] << "." << in.timeI[2] << '|') 
    ACS_CLASS_END_DEBUG 
    
    ACS_COND_THROW(not isRefGood(in.ref), ACSTimeConvException("Unsupported input reference: " + tr2s(in.ref) )) ; // PRQA S 3081 4
    ACS_COND_THROW(not isRefGood(out.ref), ACSTimeConvException("Unsupported output reference: " + tr2s(out.ref) )) 
    ACS_COND_THROW(not isFmtGood(in.format), ACSTimeConvException("Unsupported input format: " + tf2s(in.format) )) 
    ACS_COND_THROW(not isFmtGood(out.format), ACSTimeConvException("Unsupported output format: " + tf2s(out.format) )) 
	// Fill the output structure
	out.timeS     = "";
	out.timeD     = 0.0;
	out.timeI[0]  = 0;
	out.timeI[1]  = 0;
	out.timeI[2]  = 0; // PRQA S 4400
	
	long double tmpDate = toJD50(in) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Temporary date is " << setprecision(30) << tmpDate )	

    long double convDate = convert(TimeReferenceDefines(in.ref), TimeReferenceDefines(out.ref), tmpDate) ; // PRQA S 3013, 3081
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Conversion date is " << setprecision(30) << convDate )	

	//	Control the output format and call the right conversion method	
    convertTimeInternal(convDate, out) ;

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Reference: "<< out.ref << " Format: "<< out.format)	
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Date out (string|double|long):"<< setprecision(30)<< '|' << out.timeS <<'|'<< out.timeD <<'|'<< out.timeI[0] << "." <<out.timeI[1] << "." << out.timeI[2]<< '|')	
    ACS_CLASS_END_DEBUG 
}


long double ACSTimeConv::convert(TimeReferenceDefines in, TimeReferenceDefines out, long double v) const // PRQA S 4020
{
    if (in != out) {
        return (v * SecondsInADay + convert_s(in,out,v)) / SecondsInADay ;
    }
    else { return v ; } // no conversion needed
}



long double ACSTimeConv::fromAscii(const string& inName) const
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "inName: \"" << inName << "\" -> " << setprecision(20) << DateTime(inName).jd50()) ; // PRQA S 3081
    
    return DateTime(inName).jd50() ; // PRQA S 3081
}





/*	Convert standard format (RRRyyyymmddhhnnssuuuuuuuuu) to all ascii format  	*/


string ACSTimeConv::toAsciiStd(
    long double jd50, 
    TimeReferenceDefines r,
    bool hasR, 
    unsigned short fraction) const
{
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    if (hasR) {
        os << toString(r) << '=' ;
    }
    os << setw(4) << setfill('0') << y2ns.Y << '-' // PRQA S 4400 9
       << setw(2) << setfill('0') << y2ns.M << '-'
       << setw(2) << setfill('0') << y2ns.D << '_'
       << setw(2) << setfill('0') << y2ns.h << ':'
       << setw(2) << setfill('0') << y2ns.m << ':'
       << setw(2) << setfill('0') << y2ns.s ;
    if (fraction > 0) { // PRQA S 3000 
        os << '.' << setw(6) << setfill('0') << y2ns.ns/1000 ; // PRQA S 3084
    }
    return os.str() ;
}

string ACSTimeConv::toAsciiEnvi(
    long double jd50, 
    TimeReferenceDefines r,
    bool hasR, 
    unsigned short fraction) const
{
//RRR=dd-mmm-yyyy hh:nn:ss.uuuuuu
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    if (hasR) {
        os << toString(r) << '=' ;
    }
    os << setw(2) << setfill('0') << y2ns.D << '-'  // PRQA S 4400 8
       << StringUtils::uppercase(DateTime::num2Month(y2ns.M)) << '-' // PRQA S 3000
       << setw(4) << setfill('0') << y2ns.Y << ' '
       << setw(2) << setfill('0') << y2ns.h << ':'
       << setw(2) << setfill('0') << y2ns.m << ':'
       << setw(2) << setfill('0') << y2ns.s ;
    if (fraction > 0) { // PRQA S 3000
        os << '.' << setw(6) << setfill('0') << y2ns.ns/1000 ; // PRQA S 3084
    }
    return os.str() ;
}

string ACSTimeConv::toCcsdsa(
    long double jd50, 
    TimeReferenceDefines r,
    bool hasR, 
    unsigned short fraction) const
{
// RRR=yyyy-mm-ddThh:nn:ss.uuuuuu
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    if (hasR) {
        os << toString(r) << '=' ;
    }
    os << setw(4) << setfill('0') << y2ns.Y << '-' // PRQA S 4400 8
       << setw(2) << setfill('0') << y2ns.M << '-'
       << setw(2) << setfill('0') << y2ns.D << 'T'
       << setw(2) << setfill('0') << y2ns.h << ':'
       << setw(2) << setfill('0') << y2ns.m << ':'
       << setw(2) << setfill('0') << y2ns.s ;
    if (fraction > 0) { // PRQA S 3000
        os << '.' << setw(6) << setfill('0') << y2ns.ns/1000 ; // PRQA S 3084
    }
    return os.str() ;
}


string ACSTimeConv::toCcsdsaCompact(
    long double jd50, 
    TimeReferenceDefines r,
    bool hasR, 
    unsigned short fraction) const
{
// RRR=yyyy-mm-ddThh:nn:ss.uuuuuu
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    if (hasR) {
        os << toString(r) << '=' ;
    }
    os << setw(4) << setfill('0') << y2ns.Y  // PRQA S 4400 8
       << setw(2) << setfill('0') << y2ns.M 
       << setw(2) << setfill('0') << y2ns.D << 'T'
       << setw(2) << setfill('0') << y2ns.h 
       << setw(2) << setfill('0') << y2ns.m 
       << setw(2) << setfill('0') << y2ns.s ;
    if (fraction > 0) { // PRQA S 3000
        os << setw(6) << setfill('0') << y2ns.ns/1000 ; // PRQA S 3084
    }
    return os.str() ;
}


string ACSTimeConv::toCosmo(
    long double jd50, 
    bool compact, 
    unsigned short fraction) const
{
// yyyy-mm-ddThh:nn:ss.uuuuuu
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    if (compact) {
        os << setw(4) << setfill('0') << y2ns.Y // PRQA S 4400 6
           << setw(2) << setfill('0') << y2ns.M 
           << setw(2) << setfill('0') << y2ns.D 
           << setw(2) << setfill('0') << y2ns.h 
           << setw(2) << setfill('0') << y2ns.m 
           << setw(2) << setfill('0') << y2ns.s ;
    }
    else {
        os << setw(4) << setfill('0') << y2ns.Y << '-' // PRQA S 4400 6
           << setw(2) << setfill('0') << y2ns.M << '-'
           << setw(2) << setfill('0') << y2ns.D << ' '
           << setw(2) << setfill('0') << y2ns.h << ':'
           << setw(2) << setfill('0') << y2ns.m << ':'
           << setw(2) << setfill('0') << y2ns.s ;
    }
    if (fraction > 0) { // PRQA S 3000 2
        os << '.' << setw(fraction) << setfill('0') << y2ns.ns/long(exp10(9-fraction)) ; // PRQA S 3011, 3081, 3084, 4400
    }
    return os.str() ;
}

string ACSTimeConv::toAsciiErsMillisec( long double jd50) const
{
// dd-mmm-yyyy hh:nn:ss.uuu
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    os << setw(2) << setfill('0') << y2ns.D << '-' // PRQA S 4400 8
       << StringUtils::uppercase(DateTime::num2Month(y2ns.M)) << '-' // PRQA S 3000
       << setw(4) << setfill('0') << y2ns.Y << '_'
       << setw(2) << setfill('0') << y2ns.h << ':'
       << setw(2) << setfill('0') << y2ns.m << ':'
       << setw(2) << setfill('0') << y2ns.s << '.'
       << setw(3) << setfill('0') << y2ns.ns/1000000 ; // PRQA S 3084
    return os.str() ;
}



string ACSTimeConv::toAsciiCompact(
    long double jd50, 
    TimeReferenceDefines r,
    bool hasR, 
    unsigned short fraction) const
{
    const DateTime::Y2ns_type &y2ns = DateTime::jd50_to_Y2ns(jd50) ; 
    ostringstream os ; 
    if (hasR) {
        os << toString(r) << '=' ;
    }
    os << setw(4) << setfill('0') << y2ns.Y  // PRQA S 4400 8 
       << setw(2) << setfill('0') << y2ns.M 
       << setw(2) << setfill('0') << y2ns.D << '_'
       << setw(2) << setfill('0') << y2ns.h 
       << setw(2) << setfill('0') << y2ns.m 
       << setw(2) << setfill('0') << y2ns.s ;
    if (fraction > 0) { // PRQA S 3000
        os << setw(6) << setfill('0') << y2ns.ns/1000 ; // PRQA S 3084
    }
    return os.str() ;
}



/* 	Convert transport time to standard format and viceversa	*/
long double ACSTimeConv::fromTransportTime(const vector<long>& v, bool secs) const // PRQA S 4020
{
    if (secs) {
        return v[0] + v[1]/SecondsInADay + v[2]/ uSecondsInADay ; // PRQA S 3011, 4400 3
    }
    else {
        return v[0] + v[1]/mSecondsInADay + v[2] / uSecondsInADay ;
    }
}



void  ACSTimeConv::toTransportTimeDSU(long double d, vector<long> &v) const 
{
    v[0] = long (d) ; // days // PRQA S 3081
    long double secs = (d - v[0])*SecondsInADay ; // PRQA S 3011 3
    v[1] = secs ; // sec 
    v[2] = (secs - v[1])*1000000.0L ; // usec // PRQA S 4400
}

void  ACSTimeConv::toTransportTimeDMU(long double d, vector<long> &v) const 
{
    toTransportTimeDSU(d, v) ;
    v[1] *= 1000 ; // PRQA S 4400
}

long double ACSTimeConv::convert_s( // PRQA S 4020
    TimeReferenceDefines refIn, 
    TimeReferenceDefines refOut, 
    long double secs) const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"refIn:"<<toString(refIn)<< " refOut:"<<toString(refOut) <<" secs:" <<secs);
    if (refIn == refOut) { return secs ; } // no conversion needed
	// Control input days		
	ACS_COND_THROW(secs<0.0 , ACSTimeConvException("Date is out of range") ) ; // PRQA S 3081

	ACS_COND_THROW(_timeDiff.size()<2 , ACSTimeConvException("not enough references") ); // PRQA S 3081
	
	long double minTai=0 ;
    long double tmpTai=0 ;
	long double minUt1=0 ;
    long double tmpUt1=0 ;

	long double tai=0 ;
    long double ut1=0 ;
	long double utc=0 ;
    long double gps=0 ; 
	
    size_t td_size = _timeDiff.size() ;
	for(unsigned int i=0; i < td_size; ++i)
	{
		const long double &start = _timeDiff[i].start ;
	
		if(	secs>=start)
		{
			if(	_timeDiff[i].ref == TAI) {
				tai=_timeDiff[i].value;
            }
			else if( _timeDiff[i].ref == UT1) {
				ut1=_timeDiff[i].value;
            }
            else { /* nothing */ }
		}

		if(_timeDiff[i].ref == TAI) 
		{
			tmpTai=start;
			if ( (i==0) || (tmpTai<minTai) ) {
				minTai = tmpTai;
            }
		}
		else if(_timeDiff[i].ref == UT1)
		{
			tmpUt1=start;
			if ( (i==0) || (tmpUt1<minUt1) ) {
				minUt1 = tmpUt1;
            }
		}
        else { /* nothing */ }
	}

	gps = tai - utc - 19;  // PRQA S 4400

	// Control input secs		
	if(((refIn==TAI) || (refOut==TAI) || (refIn==GPS) || (refOut==GPS)) && (secs<minTai))
	{
		ACS_THROW(ACSTimeConvException("Date is out of range")) ; // PRQA S 3081
	}
	else if(((refIn==UT1) || (refOut==UT1)) && (secs<minUt1))
	{
		ACS_THROW(ACSTimeConvException("Date is out of range")) ; // PRQA S 3081
	}
    else { /* nothing */ }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"_timeDiff.size():"<<_timeDiff.size()<< " utc:"<<utc <<" tai:" <<tai<< " gps:"<<gps <<" ut1:" <<ut1);

	// Return offset from UTC to all
    switch(refIn) {
	case UTC:
        {
            switch(refOut) {
            case TAI: { return (tai-utc); }
            case GPS: { return (gps-utc); }
            case UT1: {	return (ut1-utc); }
            default: { ACS_THROW(ACSTimeConvException("out reference: " + toString(refOut) + " not allowed")) ; } // PRQA S 3081
            }
        }
	// Return offset from TAI to all
	case TAI:
        {
            switch(refOut) {
            case GPS: { return (gps-tai); }
            case UT1: { return (ut1-tai); }
            case UTC: { return (utc-tai); }
            default: { ACS_THROW(ACSTimeConvException("out reference: " + toString(refOut) + " not allowed")) ; } // PRQA S 3081
            }
        }
    case GPS:
        {
            switch(refOut) {
            case UT1: {	return (ut1-gps); }
            case UTC: {	return (utc-gps); }
            case TAI: {	return (tai-gps); }
            default: { ACS_THROW(ACSTimeConvException("out reference: " + toString(refOut) + " not allowed")) ; } // PRQA S 3081
            }
        }
	case UT1:
        {
            switch(refOut) {
            case TAI: { return (tai-ut1); }
	        case GPS: { return (gps-ut1); }
	        case UTC: { return (utc-ut1); }
            default: { ACS_THROW(ACSTimeConvException("out reference: " + toString(refOut) + " not allowed")) ; } // PRQA S 3081
            }
        }
	default: 
	    {
		    ACS_THROW(ACSTimeConvException("input reference not allowed")) ; // PRQA S 3081
	    }
    }	
}

void ACSTimeConv::loadConfiguration()
{
	unsigned int n=0;
    const rsResourceSet &rsConf = ConfigurationSingleton::instance()->get() ;
#warning MP: dangerous - use getArray instead
	rsConf.getValue("ACSTimeConverter.List_of_References.count.value",n);
	if(n == 0)
	{
		ACSTimeConvException e("ACSTimeConv::loadConfiguration: no reference available!");
		ACS_THROW(e);
	}
	for(unsigned int i=0; i<n; i++)
	{
		ostringstream num;
		num << i;
        string ref ;
		rsConf.getValue("ACSTimeConverter.List_of_References.Reference["+num.str()+"].Name",ref);
		_tD.ref = fromString(ref) ;
		unsigned int m=0;
		rsConf.getValue("ACSTimeConverter.List_of_References.Reference["+num.str()+"].List_of_Leap_Seconds.count.value",m);
		for(unsigned int j=0; j<m; j++)
		{
			ostringstream numb;
			numb << j;
            string start_s ;
			rsConf.getValue("ACSTimeConverter.List_of_References.Reference["+num.str()+"].List_of_Leap_Seconds.Leap_Seconds["+numb.str()+"].Start_Time",start_s);
			_tD.start = fromAscii(StringUtils::transform(start_s,'_','T')) ;
            rsConf.getValue("ACSTimeConverter.List_of_References.Reference["+num.str()+"].List_of_Leap_Seconds.Leap_Seconds["+numb.str()+"].Value",_tD.value);
			_timeDiff.push_back(_tD);
		}
	}
}


vector <pair <string, ssize_t> > ACSTimeConv::getLeapSecondTimeDifferences() const 
{
	/* AS: JD50, total leap seconds to date */
	vector <pair <string, ssize_t> > timeVector ;
	for (size_t i=0; i<_timeDiff.size(); i++)
	{
		// Only check the TAI / UTC differences (these are used to compute the leap second)
		if (_timeDiff[i].ref == TAI)
		{
			const string asciiTime( toAsciiCompact(_timeDiff[i].start, UTC, false, 0 ) ) ;
			pair <string, ssize_t> p(asciiTime, _timeDiff[i].value) ;
			timeVector.push_back( p ) ; 		
		}
	}
	return timeVector ;
}   


ostream & operator << (ostream &os, const ACSTimeConv::TimeConv &t) 
{
    os << tr2s(t.ref) << ' ' 
       << tf2s(t.format) << ' ' 
       << '"' << t.timeS << "\" "
       << setprecision(21) << t.timeD // PRQA S 4400 2
       << " (" << t.timeI[0] << ',' <<  t.timeI[1] << ',' <<  t.timeI[2] << ')' ;
    return os ; // PRQA S 4028
}

exostream & operator << (exostream &os, const ACSTimeConv::TimeConv &t) // PRQA S 4222
{
    os << tr2s(t.ref) << ' ' 
       << tf2s(t.format) << ' ' 
       << '"' << t.timeS << "\" "
       << setprecision(21) << t.timeD // PRQA S 4400 2
       << " (" << t.timeI[0] << ',' <<  t.timeI[1] << ',' <<  t.timeI[2] << ')' ;
    return os ; // PRQA S 4028
}

istream & ACSTimeConv::read(std::istream &is, TimeConv &t) 
{
    is.exceptions(ios_base::failbit) ;
    try {
        int r = 0 ; 
        is >> r ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Ref: " << r) ;
        if (not isRefGood(r)) {
            ACS_THROW(exIllegalValueException("TimeConv extraction: " +tr2s(r))) ; // PRQA S 3081
        }
        t.ref = r ;
        
        int fmt = 0; 
        is >> fmt ;
        t.format = fmt ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Fmt: " << fmt) ;
        switch(t.format) {
	    case ACSTimeConv::PROCESSING: // 1
            {
                long double d = 0 ;
                is >> d ;
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "d: " << setprecision(21) << d) ;

                t.timeD = d ;
                t.timeS = "" ;
                t.timeI = vector<long>(3) ; // PRQA S 3081, 4400
                
                
                break ;
            }

	    case ACSTimeConv::TRANSPORT_DAY_SEC_USEC: 
	    case ACSTimeConv::TRANSPORT_DAY_MSEC_USEC: 
            {
                vector<long> td(3) ; // PRQA S 4400
                is >> td[0] ;
                is >> td[1] ;
                is >> td[2] ; // PRQA S 4400
                t.timeD = 0 ;
                t.timeS = "" ;
                t.timeI = td ;
                
                break ;
            }
            
	    case ACSTimeConv::ASCII_ENVI: // 39
	    case ACSTimeConv::ASCII_ENVI_REF: 
	    case ACSTimeConv::ASCII_ENVI_MICROSEC: 
	    case ACSTimeConv::ASCII_ENVI_REF_MICROSEC: //42
	    case ACSTimeConv::ASCII_COSMO_NANOSEC: // 51
	    case ACSTimeConv::ASCII_COSMO_MICROSEC: 
	    case ACSTimeConv::ASCII_COSMO_MILLISEC: 
	    case ACSTimeConv::ASCII_COSMO_SEC: // 54
            {
                string a ;
                string b ;
                is >> a ;
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "a: \"" << a << "\"") ;
                is >> b ;
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "b: \"" << b << "\"") ;
                t.timeD = 0 ;
                t.timeS = StringUtils::trim( a+" "+b );
                t.timeI = vector<long>(3) ; // PRQA S 3081, 4400
        
                break ;
            }

	    case ACSTimeConv::ASCII_STD: 
	    case ACSTimeConv::ASCII_STD_REF: 
	    case ACSTimeConv::ASCII_STD_MICROSEC: 
	    case ACSTimeConv::ASCII_STD_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_COMPACT: 
	    case ACSTimeConv::ASCII_COMPACT_REF: 
	    case ACSTimeConv::ASCII_COMPACT_MICROSEC: 
	    case ACSTimeConv::ASCII_COMPACT_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_CCSDSA: 
	    case ACSTimeConv::ASCII_CCSDSA_REF: 
	    case ACSTimeConv::ASCII_CCSDSA_MICROSEC: 
	    case ACSTimeConv::ASCII_CCSDSA_REF_MICROSEC: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_REF: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_MICROSEC: 
	    case ACSTimeConv::ASCII_CCSDSA_COMPACT_REF_MICROSEC: 

	    case ACSTimeConv::ASCII_COSMO_COMPACT: // 55

	    case ACSTimeConv::ASCII_ERS_MILLISEC: 
            {
                string s ;
                is >> s ;
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "s: \"" << s << "\"") ;
                t.timeD = 0 ;
                t.timeS = s ;
                t.timeI = vector<long>(3) ; // PRQA S 3081, 4400
        
                break ;
            }
        default: 
            { 
                ACS_THROW(exIllegalValueException("TimeConv extraction: " +tf2s(t.format))) ; // PRQA S 3081
            }
        }
        
        return is ; // PRQA S 4028
    }
    catch(exception &x) {
        ACS_THROW(exIOException(x,"TimeConv extraction failure") ) ; // PRQA S 3081
    }
}


std::istream & operator >> (std::istream &is, ACSTimeConv::TimeConv &t) 
{
    return ACSTimeConv::read(is, t) ;
}

_ACS_END_NAMESPACE

