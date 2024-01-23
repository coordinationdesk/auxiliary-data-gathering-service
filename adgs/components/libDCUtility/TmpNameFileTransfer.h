// PRQA S 1050 EOF
/*
 *
 *  Copyright 2013-2021, Exprivia SpA - DFDA
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.exprivia.com
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Exprivia SpA
 *
 *  $Prod: A.C.S. libDCUtility Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 1.3  2013/09/27 10:18:36  marpas
 *  the class does not implements a load conf callback any longer.
 *  The very first time is requested to provide externally its values, the current configuration will be exploited and any warning notification will be notified once for the whole process life.
 *
 *  Revision 1.2  2013/07/05 17:39:14  lucio.pulvirenti
 *  minore changes.
 *
 *  Revision 1.1  2013/07/03 15:59:07  lucio.pulvirenti
 *  First issue.
 *
 *
 *
 */
 
#ifndef _TmpNameFileTransfer_h_
#define _TmpNameFileTransfer_h_
#include <Singleton.hpp>

#include <acs_c++config.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)


using pattern::Singleton ;
//
//
// class TmpNameFileTransfer
//
//

_ACS_BEGIN_NAMESPACE(internal)
    class TmpNameFileTransferConfLoader ;
_ACS_END_NAMESPACE

class TmpNameFileTransfer : public Singleton <TmpNameFileTransfer> // PRQA S 2109, 2153
{
    friend TmpNameFileTransfer* Singleton<TmpNameFileTransfer>::instance(); // PRQA S 2107
    friend class internal::TmpNameFileTransferConfLoader ; // PRQA S 2107
public:
	/*! class Stopped declaration */
	exDECLARE_EXCEPTION(Stopped,exException) ; // PRQA S 2131, 2502

public:
	virtual ~TmpNameFileTransfer() throw() ; 

    enum TmpNameStrategy { Prefix, Suffix, TmpDir, Tstamp } ;
    static std::string toString(TmpNameStrategy) ;  // PRQA S 2502
    static TmpNameStrategy fromString(const std::string &) ;  // PRQA S 2502

	const std::string &getTmpNameValue() const throw() ;
	TmpNameStrategy getTmpNameStrategy() const throw() ;
	static void checkConfigurationConsistency(TmpNameStrategy, const std::string &);

	
    static const TmpNameStrategy defaultTmpNameStrategy = Prefix ; // PRQA S 2100 2
    static const std::string defaultTmpNameValue ; // = "." 
    
protected:
	TmpNameFileTransfer() ; 
	
private:
	TmpNameFileTransfer &operator=(const TmpNameFileTransfer &) ; 	// declared but not implemented
	TmpNameFileTransfer (const TmpNameFileTransfer &) ; 				// declared but not implemented
    void loadConf() const ;
    
	void setTmpNameValue(const std::string &) ;
	void setTmpNameStrategy(TmpNameStrategy) ;
    
private:
    
    std::string tmpNameValue_ ;
    TmpNameStrategy tmpNameStrategy_ ;
    mutable bool loaded_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TmpNameFileTransfer) ;
} ; 

exostream & operator<< (exostream &, TmpNameFileTransfer::TmpNameStrategy) ; // PRQA S 2072
std::ostream & operator<< (std::ostream &, TmpNameFileTransfer::TmpNameStrategy) ;



_ACS_END_NESTED_NAMESPACE


#endif /* _TmpNameFileTransfer_h_ */
