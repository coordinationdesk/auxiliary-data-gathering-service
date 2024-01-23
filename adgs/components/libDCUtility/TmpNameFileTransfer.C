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
 *  Revision 1.7  2016/05/11 09:27:29  danalt
 *  use of ACS_THROW and ACS_COND_THROW
 *
 *  Revision 1.6  2013/10/02 15:22:15  lucio.pulvirenti
 *  Bug fixed: loadConf method was called in loop.
 *
 *  Revision 1.5  2013/09/27 10:18:36  marpas
 *  the class does not implements a load conf callback any longer.
 *  The very first time is requested to provide externally its values, the current configuration will be exploited and any warning notification will be notified once for the whole process life.
 *
 *  Revision 1.4  2013/07/19 12:24:45  lucio.pulvirenti
 *  TmpNameFileTransferConfLoader::confLoadedCallback: rsPushNSpace class used to set TmpNameFileTransfer node.
 *  Minor changes.
 *
 *  Revision 1.3  2013/07/15 17:20:10  marpas
 *  checks improved
 *
 *  Revision 1.2  2013/07/05 17:39:41  lucio.pulvirenti
 *  Debug added.
 *
 *  Revision 1.1  2013/07/03 15:59:07  lucio.pulvirenti
 *  First issue.
 *
 *
 *
 *
 *
 */

#include <TmpNameFileTransfer.h>

#include <Filterables.h>
#include <ConfigurationSingleton.h>
#include <StringUtils.h>
#include <rsPushNSpace.h>



_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)

using namespace std;

const string TmpNameFileTransfer::defaultTmpNameValue = "." ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TmpNameFileTransfer) ;


TmpNameFileTransfer::TmpNameFileTransfer() :
    Singleton<TmpNameFileTransfer>(),
    tmpNameValue_(defaultTmpNameValue),
    tmpNameStrategy_(defaultTmpNameStrategy),
    loaded_(false)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

TmpNameFileTransfer::~TmpNameFileTransfer() throw() {}

const string &TmpNameFileTransfer::getTmpNameValue() const throw()  // PRQA S 4120
{ 
    loadConf() ;
    return tmpNameValue_  ; 
}

TmpNameFileTransfer::TmpNameStrategy TmpNameFileTransfer::getTmpNameStrategy() const throw()  // PRQA S 4214
{ 
    loadConf() ;
    return tmpNameStrategy_ ; 
}

TmpNameFileTransfer::TmpNameStrategy TmpNameFileTransfer::fromString(const std::string &s) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Converting \"" << s << "\" into TmpNameStrategy") ;
    if (StringUtils::equalsNoCase("Prefix", s)) { return Prefix ; }
    else if (StringUtils::equalsNoCase("Suffix", s)) { return Suffix ; }
    else if (StringUtils::equalsNoCase("TmpDir", s)) { return TmpDir ; }
    else if (StringUtils::equalsNoCase("Tstamp", s)) { return Tstamp ; }
    else {
        ACS_THROW(exIllegalValueException("Cannot convert \"" + s + "\" into TmpNameStrategy")) ; // PRQA S 3081
    }
}

string TmpNameFileTransfer::toString(TmpNameStrategy s) // PRQA S 4020
{
    switch (s) {
    case Prefix: { return "Prefix" ; }
	case Suffix: { return "Suffix" ; }
    case TmpDir: { return "TmpDir" ; }
    case Tstamp: { return "Tstamp" ; }
    default:
        {
            ostringstream os ;
            os << "<unrecognized TmpNameFileTransfer::TmpNameStrategy: "<<int(s) << ">" ; // PRQA S 3081
            return os.str() ;
        }
    
    }
}

exostream & operator<< (exostream &os, TmpNameFileTransfer::TmpNameStrategy s) // PRQA S 4222
{
    os << TmpNameFileTransfer::toString(s) ;
    return os ; // PRQA S 4028
}

ostream & operator<< (ostream &os, TmpNameFileTransfer::TmpNameStrategy s) 
{
    os << TmpNameFileTransfer::toString(s) ;
    return os ; // PRQA S 4028
}


void TmpNameFileTransfer::checkConfigurationConsistency(TmpNameFileTransfer::TmpNameStrategy strategy, const string &value)
{
	if( value.empty() 							||
		(value.find(" ") != string::npos)		||
		((strategy != Suffix || strategy != Tstamp) && (value == "..")) 	||
		((strategy == TmpDir) && (value == ".")) ) {
		ACS_THROW(exIllegalValueException("Unallowed strategy (" + toString(strategy) + ") and value (" + value + ") combination")) ; // PRQA S 3081
    }
    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Strategy (" + toString(strategy) + ") and value (" + value + ") is allowed") ;
}


void TmpNameFileTransfer::loadConf() const // PRQA S 4020, 4214
{
    if (loaded_) { return ; }
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	rsResourceSet &rs = ConfigurationSingleton::instance()->get();
	rsPushNSpace rsPusher (rs, "TmpNameFileTransfer");

    const string k1 = "TmpNameFileTransferStrategy" ;
	TmpNameStrategy strategy = defaultTmpNameStrategy ;
	try
	{
        string tmp ;
		rs.getValue(k1, tmp);
		try {
			strategy = TmpNameFileTransfer::fromString(tmp);
		}
		catch(exception &ee)
		{
            ACS_LOG_WARNING("Cannot convert \"" << tmp << "\" as " << k1 << ". Default set to " << strategy) ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, k1 << "=" << strategy ) ;

	}
	catch(exception &e)
	{
        ACS_LOG_WARNING("Cannot find " << k1 << ". Default set to " << strategy) ;
	}

	const string k2 = "TmpNameFileTransferValue" ;
	string strategyValue = defaultTmpNameValue ;
	try {
		rs.getValue(k2, strategyValue);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, k2 << "=\"" << strategyValue << "\"") ;
	}
	catch(exception &e)
	{
        ACS_LOG_WARNING("Cannot find " << k2 << ". Default set to \"" << strategyValue << "\"") ;
	}

    loaded_ = true ;
	try {
		TmpNameFileTransfer::checkConfigurationConsistency(strategy, strategyValue);
		TmpNameFileTransfer::instance()->setTmpNameStrategy(strategy);
		TmpNameFileTransfer::instance()->setTmpNameValue(strategyValue) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Strategy set to \"" << strategy << "\" and value to \"" << strategyValue << "\"") ;
	}
	catch(exception &e)
	{
        ACS_LOG_WARNING(e.what() << ". Default strategy set to " << getTmpNameStrategy() << " and value to \""
						<< getTmpNameValue() << "\"" );
	}

}


void TmpNameFileTransfer::setTmpNameValue(const std::string &v) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Setting \"" << v << "\" as temp name value") ;
    tmpNameValue_ = v ;
}

void TmpNameFileTransfer::setTmpNameStrategy(TmpNameStrategy v) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Setting \"" << v << "\" as temp name strategy") ;
	tmpNameStrategy_ = v;
}

_ACS_BEGIN_NAMESPACE(internal)

    class TmpNameFileTransferConfLoader {
    public:
        TmpNameFileTransferConfLoader() throw() ;
        static void confLoadedCallback(void *) ;
    private:
	    ACS_CLASS_DECLARE_DEBUG_LEVEL(TmpNameFileTransferConfLoader) ;

    } ;

    ACS_CLASS_DEFINE_DEBUG_LEVEL(TmpNameFileTransferConfLoader) ;
    namespace { // unnamed
        TmpNameFileTransferConfLoader TmpNameFileTransferConfLoader_ ;
    }
    
    TmpNameFileTransferConfLoader::TmpNameFileTransferConfLoader() throw()
    {   
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding TmpNameFileTransfer conf space load request") ;
        try {
			
          	ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("TmpNameFileTransfer", "", false), ConfigurationSingleton::NoLoad);
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
    }


_ACS_END_NAMESPACE
_ACS_END_NESTED_NAMESPACE
