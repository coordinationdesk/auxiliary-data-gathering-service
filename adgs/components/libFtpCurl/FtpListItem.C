// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
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
 *  Revision 5.7  2013/12/09 14:25:33  marpas
 *  qa warnings
 *
 *  Revision 5.6  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.5  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/23 17:10:11  marpas
 *  classes rationalization and robustness improved
 *
 *  Revision 5.1  2013/06/18 12:54:14  marpas
 *  debug macros rationalized
 *
 *  Revision 5.0  2013/06/06 18:08:21  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.7  2013/05/29 10:54:25  marpas
 *  no implicit cast/ctor
 *
 *  Revision 2.6  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.5  2012/11/29 14:38:56  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 2.4  2012/11/28 18:36:19  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 2.3  2012/03/27 10:52:22  micmaz
 *  work in progress
 *
 *  Revision 2.2  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.1  2012/01/20 16:06:40  micmaz
 *  modified logs
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.8  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.7  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.6  2011/07/15 09:24:55  micmaz
 *  work in progress..
 *
 *  Revision 1.5  2011/05/09 15:58:14  micmaz
 *  removed TODO
 *
 *  Revision 1.4  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/01/21 18:04:48  micmaz
 *  corrected the date/time parser.
 *
 *  Revision 1.2  2011/01/19 16:42:58  micmaz
 *  added the openConnection() and closeConnection() to cURL operations.
 *
 *  Revision 1.1  2011/01/13 10:57:28  micmaz
 *  added mput() and mget() method in order to open only one connection if you must trasfer more than one file to/from the same host.
 *
 *
 *
 */

#include <FtpListItem.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <climits>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

using namespace std;
using namespace boost;
using namespace posix_time;

ACS_CLASS_DEFINE_DEBUG_LEVEL(FtpListItem);

FtpListItem::FtpListItem() :
        _permissions(),
        _numberOfFiles(),
        _owner(),
        _group(),
        _size(),
        _year(),
        _month(),
        _day(),
        _hour(),
        _min(),
        _name(),
        _date(false) // invalid
{
    // empty
}

FtpListItem::~FtpListItem() throw(){
    // empty
}

FtpListItem::FtpListItem(FtpListItem const& other) :
                _permissions(other._permissions),
                _numberOfFiles(other._numberOfFiles),
                _owner(other._owner),
                _group(other._group),
                _size(other._size),
                _year(other._year),
                _month(other._month),
                _day(other._day),
                _hour(other._hour),
                _min(other._min),
                _name(other._name),
                _date(other._date) 
{
    //empty.
}

FtpListItem& FtpListItem::operator=(FtpListItem const& other) {
    if (&other != this) {
        _permissions = other._permissions;
        _numberOfFiles = other._numberOfFiles;
        _owner = other._owner;
        _group = other._group;
        _size = other._size;
        _month = other._month;
        _year = other._year;
        _day = other._day;
        _hour = other._hour;
        _min = other._min;
        _name = other._name;
        _date = other._date ;
    }
    return *this;
}


void FtpListItem::reset() 
{
    *this = FtpListItem() ;
}

void FtpListItem::setField(Token token, string const &strRead) {
    switch (token) {
    case permissions: 
        {
            setPermissions(FilePermissions(strRead)); // PRQA S 3081
            break;
        }
    case numberOfFiles:
        {
            setNumberOfFiles(atol(strRead.c_str()));
            break;
        }
    case owner:
        {
            setOwner(strRead);
            break;
        }
    case group:
        {
            setGroup(strRead);
            break;
        }
    case size:
        {
            setSize(atol(strRead.c_str()));
            break;
        }
    case month:
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "month: " << strRead << " -> " << DateTime::month2Num(strRead));
            _month = static_cast<unsigned short>(DateTime::month2Num(strRead)) ; // PRQA S 3000, 3081
            break;
        }
    case day:
        {
            _day = short(atoi(strRead.c_str())) ; // PRQA S 3000, 3081
            break;
        }
    case hour:
        {
            _hour = short(atoi(strRead.c_str())) ; // PRQA S 3000, 3081
            break;
        }
    case minutes:
        {
            _min = short(atoi(strRead.c_str())); // PRQA S 3000, 3081
            break;
        }
    case name:
        {
            setName(strRead);
            break;
        }
    case year:
        {
            _year = short(atoi(strRead.c_str())); // PRQA S 3000, 3081
            break;
        }
    default:
        {
            break;
        }
    }
}

string FtpListItem::str() const {
    ostringstream os;
    os << "FtpListItem[";
    os << "permissions=" << _permissions.str() << ",files=" << _numberOfFiles << ",owner=" << _owner << ",group="
            << _group << ",size=" << _size << ",date=" << getDateTime().getSqlString() << ",hour=" << _hour << ",mins="
            << _min << ",name=" << _name;
    os << "]";

    return os.str();

}

//Getters and Setters


const std::string &FtpListItem::getGroup() const throw() { return _group; } // PRQA S 4120

const std::string &FtpListItem::getName() const throw() { return _name; } // PRQA S 4120

long FtpListItem::getNumberOfFiles() const throw() { return _numberOfFiles; } // PRQA S 4120

const std::string &FtpListItem::getOwner() const throw() { return _owner; } // PRQA S 4120

const FilePermissions &FtpListItem::getPermissions() const throw() { return _permissions; } // PRQA S 4120

long long FtpListItem::getSize() const throw() { return _size; } // PRQA S 4120

void FtpListItem::setGroup(const std::string &group)  // PRQA S 2502, 4121
{
    _group = group;
}

void FtpListItem::setName(const std::string &name)  // PRQA S 2502, 4121
{
    _name = name;
}

void FtpListItem::setNumberOfFiles(long n) throw()  // PRQA S 4121
{
    _numberOfFiles = n;
}

void FtpListItem::setOwner(const std::string &owner)  // PRQA S 2502, 4121
{
    _owner = owner;
}

void FtpListItem::setPermissions(const FilePermissions &p)  // PRQA S 4121
{
    _permissions.init(p.getPermissions());
}

void FtpListItem::setSize(long long s) throw() // PRQA S 4121
{
    _size = s;
}


void FtpListItem::computeDateTime()  
{
    try {
        unsigned short yr = ( _year != 0 ) ? _year : DateTime::UTCD(DateTime()).year ; // PRQA S 3000, 3050, 3081, 3384
        DateTime::UTCD utcd(yr, _month, _day, _hour, _min) ;
        // correct aroung 31st Dec.
        if (DateTime() < utcd) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is in the future" << DateTime(utcd)); // PRQA S 3081
            utcd.year-- ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Final" << DateTime(utcd)); // PRQA S 3081
        _date = utcd ;
    }
    catch(std::exception &) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Invalid");
        _date = DateTime(false) ; // invalid date time // PRQA S 3081
    }
}

const DateTime &FtpListItem::getDateTime() const throw() // PRQA S 4120
{
    return _date ;
}

void FtpListItem::setDateTime(DateTime const& date) // PRQA S 4121
{
    _date = date ;
}

void FtpListItem::initMask(Tokens &mask) 
{
    mask.clear() ;
    //fills in all items that must be read.
    mask.push_back(FtpListItem::permissions) ;
    mask.push_back(FtpListItem::numberOfFiles) ;
    mask.push_back(FtpListItem::owner) ;
    mask.push_back(FtpListItem::group) ;
    mask.push_back(FtpListItem::size) ;
    mask.push_back(FtpListItem::month) ;
    mask.push_back(FtpListItem::day) ;
    mask.push_back(FtpListItem::hour) ;
    mask.push_back(FtpListItem::minutes) ;
    mask.push_back(FtpListItem::name) ;
}

void FtpListItem::setLine(const string &in_line) 
{
    ACS_COND_THROW(in_line.empty(), exIllegalValueException("The line read must not be empty.")) ; // PRQA S 3081
    reset() ;
    string line = in_line ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,  "line to read: " << line);
    Tokens currentMask ;
    initMask(currentMask) ;
    while (not currentMask.empty()) {
        FtpListItem::Token currentField = *currentMask.begin();
        size_t spacePosition = line.find(' ');
        if (spacePosition != string::npos) {
            string tokenFound = StringUtils::substringBefore(line, " ");
            setField(currentField, tokenFound);
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Current Field " << tokenFound);
            currentMask.pop_front();
        }
        line = StringUtils::substringAfter(line, " ");
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "line remained : " << line);
        line = StringUtils::trim(line);
        if (currentField == day) {
            string hourAndMinutes = StringUtils::substringBefore(line, " ");
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "hourAndMinutes  " << hourAndMinutes);
            size_t colonPosition = hourAndMinutes.find(':');
            if (colonPosition != string::npos) {
                string hr = StringUtils::substringBefore(hourAndMinutes, ":");
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "hour " << hr);
                setField(hour, hr);
                string min = StringUtils::substringAfter(hourAndMinutes, ":");
                setField(minutes, min);
                line = StringUtils::trim(line);
                line = StringUtils::substringAfter(line, " ");
                line = StringUtils::trim(line);
                setField(FtpListItem::name, line);
                computeDateTime() ;
                break;
            } 
            else {
                setField(year, hourAndMinutes);
                line = StringUtils::substringAfter(line, " ");
                line = StringUtils::trim(line);
                setField(name, line);
                computeDateTime() ;
                break;
            }
        }
    }
}


std::ostream& operator<<(std::ostream& stream, const FtpListItem &obj) 
{
    return stream << obj.str();
}

acs::exostream& operator<<(acs::exostream& stream, const FtpListItem &obj) // PRQA S 4222
{
    stream << obj.str(); return stream ; // PRQA S 4028
}

_ACS_END_NESTED_NAMESPACE
