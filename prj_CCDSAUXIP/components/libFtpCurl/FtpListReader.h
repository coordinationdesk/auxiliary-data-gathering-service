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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.5  2014/04/14 15:08:43  marfav
 *  Do not convert time_t in DateTime when filtering by date local URLs
 *
 *  Revision 5.4  2014/03/11 17:41:55  marfav
 *  Added support to date based filtering
 *
 *  Revision 5.3  2014/01/31 11:56:28  marpas
 *  interface rationalization, robustness improved
 *
 *  Revision 5.2  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.1  2013/07/23 17:10:12  marpas
 *  classes rationalization and robustness improved
 *
 *  Revision 5.0  2013/06/06 18:08:21  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.6  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.5  2013/01/18 15:45:32  micmaz
 *  work in progress
 *
 *  Revision 2.4  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.3  2012/11/29 14:38:56  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 2.2  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.1  2012/01/20 19:31:46  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.8  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.7  2011/06/21 15:33:58  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/06/16 18:27:12  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/05/04 09:52:10  micmaz
 *  added a method to decide if a  remote resource exits and if it is a file or directory.
 *
 *  Revision 1.4  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.3  2011/01/21 18:04:48  micmaz
 *  corrected the date/time parser.
 *
 *  Revision 1.2  2011/01/19 16:52:29  micmaz
 *  added comments.
 *
 *  Revision 1.1  2011/01/13 10:57:28  micmaz
 *  added mput() and mget() method in order to open only one connection if you must trasfer more than one file to/from the same host.
 *
 *
 *
 */

#ifndef _FtpListReader_H_
#define _FtpListReader_H_

#include<FtpListItem.h>

#include<exMacros.h>
#include <FSTree.h>
#include <DateTime.h>

#include<vector>
#include <set>
#include <StopController.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

/**
 * Parses the output of the "ls -'" command creating a acs::FtpListItem object.
 */
class FtpListReader { // PRQA S 2109
public:
    explicit FtpListReader(std::vector<FtpListItem>& items);

    FtpListReader(const FtpListReader &) = delete ;
    FtpListReader & operator= (const FtpListReader &) = delete ;
	
    /**
     * Empty dtor.
     */
    ~FtpListReader() = default ;

    /**
     * Create the FtpListItem from the line read.
     * @param line the current line read from the "ls -l" command.
     */
    static void createLocalFtpListItems(std::string const& sourcePath , 
                                        std::vector<FtpListItem> & sourceFiles, 
                                        bool filterByDate, 
                                        const time_t& lastScan, 
                                        StopController const& stopController = StopController(), 
										const std::set<std::string> & = std::set<std::string>() ) ; // NOSONAR - set w/o transparent comp

    void addListItem(FtpListItem const& item);

    void clearListItems();
private:
	static void buildFtpListItem(const std::pair<std::string, acs::os::FSTree::Entry> &, const std::string & sourcePath, FtpListItem & );

    std::vector<FtpListItem>& _items ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(FtpListReader) ;
};

_ACS_END_NESTED_NAMESPACE


#endif /* _FtpListReader_H_ */
