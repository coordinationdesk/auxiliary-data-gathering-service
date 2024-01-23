// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S. libFtpCurl Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
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
 *  Revision 2.5  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.4  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.3  2012/11/29 14:38:56  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 2.2  2012/11/28 18:36:19  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.8  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.7  2011/07/15 09:24:55  micmaz
 *  work in progress..
 *
 *  Revision 1.6  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.4  2011/02/23 11:21:27  micmaz
 *  removed include boost/date_time.hpp
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

#ifndef _FtpListItem_H_
#define _FtpListItem_H_

#include <FilePermissions.h>

#include <exException.h>
#include <DateTime.h>
#include <deque>
#include <string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

class FtpListItem { // PRQA S 2109
public:

    std::string str() const;

    const DateTime &getDateTime() const noexcept ;

    const std::string &getGroup() const noexcept ;

    const std::string & getName() const noexcept ;

    long getNumberOfFiles() const noexcept ;

    const std::string &getOwner() const noexcept ;

    const FilePermissions &getPermissions() const noexcept ;

    long long getSize() const noexcept ;

    enum Token { // NOSONAR - old enum
        permissions = 0, 
        numberOfFiles, 
        owner, 
        group, 
        size, 
        month, 
        day, 
        year, 
        hour, 
        minutes, 
        name
    } ;

    void setField(Token token, std::string const & strRead);
    void setSize(long long) noexcept ;
    void setName(const std::string &);
    void setDateTime(DateTime const&) ;

    void setLine(const std::string &) ;
    
private:
    void reset() ;
    void setGroup(const std::string &);

    void setNumberOfFiles(long) noexcept ;

    void setOwner(const std::string &);

    void setPermissions(const FilePermissions &);

    void computeDateTime() ;
 
    using Tokens = std::deque<Token> ;

    static void initMask(Tokens &mask) ; 

private: // NOSONAR - for clarity 

    FilePermissions _permissions = {} ;
    long _numberOfFiles = 0 ;
    std::string _owner = "" ;
    std::string _group = "" ;
    long long _size = 0 ;

	unsigned short _year = 0 ;
	unsigned short _month = 0 ;
	unsigned short _day = 0 ;
	unsigned short _hour = 0 ;
	unsigned short _min = 0 ;

    std::string _name = "" ;
    DateTime _date = DateTime(false) ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FtpListItem) ;
};

std::ostream& operator<<(std::ostream& stream, const FtpListItem &obj) ;
acs::exostream& operator<<(acs::exostream& stream, const FtpListItem &obj) ; // PRQA S 2072


_ACS_END_NESTED_NAMESPACE

#endif /* _FtpListItem_H_ */
