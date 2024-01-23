// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S. libFtpCurl Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.11  2017/12/13 09:34:04  davide.tiriticco
 *  Added timestamp strategy for temporary files
 *
 *  Revision 5.10  2015/05/04 13:58:48  marfav
 *  Use of default strategy values fixed
 *
 *  Revision 5.9  2015/04/23 17:39:45  marpas
 *  isChangeStrategy method changed into hasTemporaryTargetName
 *  debug improved
 *
 *  Revision 5.8  2015/04/23 11:56:27  marpas
 *  removing useless boost pointers
 *  coding best practices applied
 *
 *  Revision 5.7  2014/06/19 18:08:08  francesco.avanzi
 *  switch-case logic enhanced
 *
 *  Revision 5.6  2014/06/04 12:21:16  marpas
 *  getting rid of confusing Assert macros
 *
 *  Revision 5.5  2014/02/13 19:14:24  marpas
 *  potential crash when referring static const members fixed
 *
 *  Revision 5.4  2013/10/24 18:28:00  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/06/18 12:53:35  marpas
 *  minor changes
 *
 *  Revision 5.1  2013/06/11 14:31:35  lucio.pulvirenti
 *  Debug macros fixed and debug msgs added.
 *  Static const strings names with strategy value changed according to the new content of t_circulationtempnames table.
 *  New IF member tempnamevalue_ properly set in CTORs and =op.
 *  init():tempnamevalue_ passed to AbstractTempStrategy derived class CTORs
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2013/04/15 09:12:34  marpas
 *  coding best practices applied
 *  interfaces rationalization and simplification
 *
 *  Revision 2.2  2012/03/28 15:22:25  micmaz
 *  work in progress
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.5  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.4  2011/08/29 10:24:29  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *
 *  Revision 1.3  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.2  2011/07/08 17:49:47  micmaz
 *  removed commented lines.
 *
 *  Revision 1.1  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *
 *
 *
 */

#include<TemporaryFileService.h>

#include<TempStrategy.h>
#include<RemoteSite.h>
#include<StringUtils.h>
#include<Filterables.h>

#include<iostream>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 
_ACS_BEGIN_NAMESPACE(temp) 
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL (TemporaryFileService);

const std::string TemporaryFileService::PREFIX = "PREFIX" ;
const std::string TemporaryFileService::SUFFIX = "SUFFIX" ;
const std::string TemporaryFileService::TEMPDIR = "TMPDIR" ;
const std::string TemporaryFileService::TSTAMP = "TSTAMP" ;

const std::string TemporaryFileService::PREFIX_NAME_STRATEGY = PREFIX;
const std::string TemporaryFileService::SUFFIX_NAME_STRATEGY = SUFFIX;
const std::string TemporaryFileService::TEMP_DIR_NAME_STRATEGY = TEMPDIR;
const std::string TemporaryFileService::TIMESTAMP_NAME_STRATEGY = TSTAMP;
const std::string TemporaryFileService::NO_NAME_STRATEGY = "NO_TEMP";

const std::string &TemporaryFileService::get_DEFAULT_TEMP_NAME_STRATEGY()
{ 
    static std::string s = "PREFIX" ; 
    return s ; 
}

TemporaryFileService::TemporaryFileService(const string &strategyName, const string & tempnamevalue) :
    handle_(), 
    strategy_(),
	tempnamevalue_(tempnamevalue)	
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
    strategy_ = makeStrategy(strategyName);
    if (strategy_ == unknownStrategy) {
        strategy_ = makeDefaultStrategy();
		tempnamevalue_="";
    }
    init();
}

TemporaryFileService::TemporaryFileService(TempRenameStrategy strategy, const string & tempnamevalue) :
    handle_(),
	strategy_(strategy),
	tempnamevalue_(tempnamevalue)	
{
    init();
}

TemporaryFileService::~TemporaryFileService() throw () 
{
}

TempRenameStrategy TemporaryFileService::makeDefaultStrategy() 
{
    return makeStrategy(get_DEFAULT_TEMP_NAME_STRATEGY());
}

TempRenameStrategy TemporaryFileService::makeStrategy(std::string const& strategyName) 
{
    TempRenameStrategy strategy = unknownStrategy;
    if (StringUtils::equalsNoCase(strategyName, PREFIX_NAME_STRATEGY)) {
        strategy = prefix;
    } 
    else {
        if (StringUtils::equalsNoCase(strategyName, SUFFIX_NAME_STRATEGY)) {
            strategy = postfix;
        } 
        else {
            if (StringUtils::equalsNoCase(strategyName, TEMP_DIR_NAME_STRATEGY)) {
                strategy = tempDir;
            }
	    else {
	    	if (StringUtils::equalsNoCase(strategyName, TIMESTAMP_NAME_STRATEGY)) {
                    strategy = tstamp;
		}
		else {
                     if (StringUtils::equalsNoCase(strategyName, NO_NAME_STRATEGY)) {
                         strategy = noTemp;
                     }
                }
	    }
        }
    }
    return strategy;
}

TemporaryFileService& TemporaryFileService::operator=(TemporaryFileService const& other) 
{
    if (this != &other) {
        strategy_ = other.strategy_;
		tempnamevalue_ = other.tempnamevalue_;
        handle_.reset(other.handle_->clone());
    }
    return *this;
}

TemporaryFileService::TemporaryFileService(TemporaryFileService const& other) :
        handle_(), 
		strategy_(),
		tempnamevalue_()		
{
    operator=(other);
}

TempRenameStrategy TemporaryFileService::getStrategy() const // PRQA S 4120 
{
    return strategy_;
}

void TemporaryFileService::init() { // PRQA S 4020
    switch(strategy_) {
    case tempDir: 
        {
	        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "tempDir tempnamevalue_="<<tempnamevalue_);
			if (tempnamevalue_.empty())
			{
	            handle_.reset(new TempDirStrategy()) ;
			}
			else
			{
	            handle_.reset(new TempDirStrategy(tempnamevalue_)) ;
			}
            return ;
        } 
    case prefix: 
        {
	        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "prefix tempnamevalue_="<<tempnamevalue_);
			if (tempnamevalue_.empty())
			{
	            handle_.reset(new PrefixTempStrategy()) ;
			}
			else
			{
	            handle_.reset(new PrefixTempStrategy(tempnamevalue_)) ;
			}
            return ;
        } 
    case postfix: 
        {
	        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "postfix tempnamevalue_="<<tempnamevalue_);
			if (tempnamevalue_.empty())
			{
	            handle_.reset(new PostfixTempStrategy()) ;
			}
			else
			{
	            handle_.reset(new PostfixTempStrategy(tempnamevalue_)) ;
			}
            return ;
        } 
    case tstamp:
	{
	        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "tstamp tempnamevalue_="<<tempnamevalue_);
		if (tempnamevalue_.empty())
		{
			handle_.reset(new TimestampTempStrategy());
		}
		else
		{
			handle_.reset(new TimestampTempStrategy(tempnamevalue_));
		}
		return ;
	}		
    case noTemp: 
        {
	        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "noTemp");
            handle_.reset(new NoTempStrategy()) ;
            return ;
        } 
    default: 
        {
            ostringstream os ;
            os << strategy_ ;
            ACS_THROW(exIllegalValueException("Temp file startegy: \""+os.str()+"\" unknown")) ; // PRQA S 3081
        }
    }
}

string TemporaryFileService::getTemporaryDir(string const& path) const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\"" << handle_->getTemporaryDir(path) << "\"");
    return handle_->getTemporaryDir(path);
}
string TemporaryFileService::getTemporaryFile(string const& path) const {
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\"" << handle_->getTemporaryFile(path) << "\"");
    return handle_->getTemporaryFile(path);
}
string TemporaryFileService::getTemporaryPath(string const& path) const {
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\"" << handle_->getTemporaryPath(path) << "\"");
    return handle_->getTemporaryPath(path);
}

void TemporaryFileService::resetStrategyState() {
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Resetting strategy state.");
	handle_.get()->reset();
}

bool TemporaryFileService::hasTemporaryTargetName() const {
    return handle_->isDirNameChanged() || handle_->isFileNameChanged();
}

void TemporaryFileService::setRemotePath(bool isTemporaryDir, bool isTemporaryFile, RemoteSite* site) const {
    std::string dir;
    std::string file;
    if (isTemporaryDir) {
        dir = handle_->getTemporaryDir(site->getPath());
    } 
    else {
        dir = site->getRemoteDir();
    }
    if (isTemporaryFile) {
        file = handle_->getTemporaryFile(site->getPath());
    } 
    else {
        file = site->getRemoteFileName();
    }
    std::ostringstream os;
    os << dir << "/" << file;

    site->setPath(os.str());
}

exostream & operator<< (exostream &os, TempRenameStrategy s) // PRQA S 4222
{
    switch (s) {
    case noTemp:
    {
        os << "noTemp" ;
        break ;
    }
	case prefix: 
    {
        os << "prefix" ;
        break ;
    }
    case postfix:
    {
        os << "postfix" ;
        break ;
    }
	case tempDir:
    {
        os << "tempDir" ;
        break ;
    }
	case tstamp:
    {
        os << "tstamp" ;
	break;
    }
	case unknownStrategy:
    {
        os << "unknownStrategy" ;
        break ;
    }
    default:
    {
        os << "<unrecognized TempRenameStrategy: "<<int(s) << ">" ; // PRQA S 3081
        break ;
    }
    
    }
    return os ; // PRQA S 4028
}

ostream & operator<< (ostream &os, TempRenameStrategy s) 
{
     switch (s) {
    case noTemp:
    {
        os << "noTemp" ;
        break ;
    }
	case prefix: 
    {
        os << "prefix" ;
        break ;
    }
    case postfix:
    {
        os << "postfix" ;
        break ;
    }
	case tempDir:
    {
        os << "tempDir" ;
        break ;
    }
    case tstamp:
    {
        os << "tstamp" ;
	break;
    }
	case unknownStrategy:
    {
        os << "unknownStrategy" ;
        break ;
    }
    default:
    {
        os << "<unrecognized TempRenameStrategy: "<<int(s) << ">" ; // PRQA S 3081
        break ;
    }
    
    }
    return os ; // PRQA S 4028
}


_ACS_END_NAMESPACE
_ACS_END_NESTED_NAMESPACE

