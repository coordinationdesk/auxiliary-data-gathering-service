// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2019, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.11  2017/02/08 14:41:14  marfav
 *  ACSMAINT-88
 *  Nanosecond precision is needed to estimate mtime and ctime
 *
 *  Revision 5.10  2016/03/07 15:33:56  marfav
 *  S2PDGS-1428, PDSEV-28
 *  Stop controllers managed during FTP file listing and collection
 *
 *  Revision 5.9  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.8  2014/04/14 15:08:43  marfav
 *  Do not convert time_t in DateTime when filtering by date local URLs
 *
 *  Revision 5.7  2014/03/12 14:00:58  marfav
 *  Using the max of ctime and mtime to discover new files
 *
 *  Revision 5.6  2014/03/12 12:20:48  marpas
 *  qa warnings fixed
 *
 *  Revision 5.5  2014/03/11 17:41:55  marfav
 *  Added support to date based filtering
 *
 *  Revision 5.4  2014/02/07 18:45:10  marpas
 *  minor changes
 *
 *  Revision 5.3  2014/01/31 11:56:28  marpas
 *  interface rationalization, robustness improved
 *
 *  Revision 5.2  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/23 17:10:12  marpas
 *  classes rationalization and robustness improved
 *
 *  Revision 5.0  2013/06/06 18:08:21  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.10  2013/01/18 16:38:03  micmaz
 *  work in progress
 *
 *  Revision 2.9  2013/01/18 15:45:32  micmaz
 *  work in progress
 *
 *  Revision 2.8  2012/12/07 13:53:45  micmaz
 *  correct a core dump on put rollback
 *
 *  Revision 2.7  2012/12/03 16:56:48  marpas
 *  using new StringUtils interface
 *
 *  Revision 2.6  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.5  2012/11/29 14:38:56  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 2.4  2012/03/27 10:52:22  micmaz
 *  work in progress
 *
 *  Revision 2.3  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/01/20 19:31:46  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 2.1  2012/01/20 16:06:40  micmaz
 *  modified logs
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.10  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.9  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.8  2011/06/21 15:33:58  micmaz
 *  work in progress
 *
 *  Revision 1.7  2011/06/16 18:27:12  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/05/04 09:52:10  micmaz
 *  added a method to decide if a  remote resource exits and if it is a file or directory.
 *
 *  Revision 1.5  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.4  2011/02/23 13:01:15  micmaz
 *  removed boost dependency
 *
 *  Revision 1.3  2011/02/23 11:37:11  micmaz
 *  removed include boost/date_time.hpp
 *
 *  Revision 1.2  2011/01/21 18:04:48  micmaz
 *  corrected the date/time parser.
 *
 *  Revision 1.1  2011/01/13 10:57:28  micmaz
 *  added mput() and mget() method in order to open only one connection if you must trasfer more than one file to/from the same host.
 *
 *
 *
 */

#include "FtpListReader.h"


#include <StringUtils.h>
#include <File.h>
#include <Filterables.h>

using namespace std;
using namespace boost;

using acs::os::FSTree;

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(FtpListReader);

FtpListReader::FtpListReader(std::vector<FtpListItem>& items) :
    _items(items) // PRQA S 2528
{
}

FtpListReader::~FtpListReader() throw(){
    // empty
}

namespace {
    class DateTimeFilter // PRQA S 2109
    {
    public:
        DateTimeFilter (const string& basePath, bool filter, const time_t& date) :
            _basePath (basePath),
            _filter (filter),
            _datetime (date) // PRQA S 2528
        {}

        bool operator() (std::string& path, acs::os::FSTree::Entry& entry) const 
        {
            // filter the first . in the tree
            bool allow = (path != _basePath);

            // Check if need to filter also on the modification date
            if (_filter)
            {
                // Keep only elements with the modification time 
                // in the future wrt the given _date
                allow = ( std::max (entry.stat_r.st_mtime, entry.stat_r.st_ctime) >= _datetime );
            }
            return allow;
        }
    private:  
        DateTimeFilter() ; // not implemented 
    private:
        string _basePath;
        bool _filter;
        time_t _datetime;

    };
	
	string getFilepathFromEntryPair(  const pair<string, FSTree::Entry>  &p) 
	{
    	return p.first;
	}

	
}

void FtpListReader::createLocalFtpListItems(string const& sourcePath, std::vector<FtpListItem> & sourceFiles, bool filterByDate, const time_t& lastScan, StopController const& stopController, const set<string> & filepathsToSkip) {
    FSTree path (sourcePath);
    path.maxDepth (1);
    path.hidden (false);
    path.fatalFailures (true);
    
    DateTimeFilter myFilter (sourcePath, filterByDate, lastScan);
    path.load(myFilter);
	
	FSTree::EntriesMap entriesMap = path.getMap();
	if( filepathsToSkip.empty() ) {
    
    	for (FSTree::EntriesMap::const_iterator it=entriesMap.begin(); it != entriesMap.end() && !stopController.requestedCancel(); ++it)  {
			FtpListItem item ;
			buildFtpListItem(*it, sourcePath, item);
			sourceFiles.push_back(item);
    	}
		
	}
	else {
		set<string> filepaths;
   		transform(entriesMap.begin(), entriesMap.end(), inserter(filepaths, filepaths.begin()), getFilepathFromEntryPair);
		
		// subtract filenamesToSkip to filepaths
		set<string> filepathsToKeep;
		set_difference(
			filepaths.begin(), filepaths.end(),
			filepathsToSkip.begin(), filepathsToSkip.end(),
			inserter(filepathsToKeep, filepathsToKeep.begin())
			);

		
		// loop on new set and choose map elements
		for( set<string>::const_iterator it = filepathsToKeep.begin(); it != filepathsToKeep.end(); it++ ) {
			FSTree::EntriesMap::const_iterator itMap = entriesMap.find(*it);
			ACS_COND_THROW( itMap == entriesMap.end(), exIllegalValueException( '\"' + *it + "\" element does not occur in map!") ); // it should not occur
			FtpListItem item ;
			buildFtpListItem(*itMap, sourcePath, item);
			sourceFiles.push_back(item);
		}

	}
}

void FtpListReader::addListItem(FtpListItem const& item) {
    _items.push_back(item);
}

void FtpListReader::clearListItems() {
    _items.clear();
}

void FtpListReader::buildFtpListItem(const pair<string, FSTree::Entry> &p, const string & sourcePath, FtpListItem & item)
{
    string fullPath = p.first;
    if (fullPath != sourcePath) // do not take into account the "." in the sourcePath
    {
        string name = File::getFileName (fullPath);
        item.setName(name);
        item.setSize ((p.second).stat_r.st_size);

		// Take into account also nanoseconds
		DateTime ctime ( (p.second).stat_r.st_ctime );
		ctime += (p.second).stat_r.st_ctim.tv_nsec / 86400.0L / 1000000000.0L; // Days, long double // PRQA S 3011, 4400

		DateTime mtime ( (p.second).stat_r.st_mtime );
		mtime += (p.second).stat_r.st_mtim.tv_nsec / 86400.0L / 1000000000.0L; // Days, long double // PRQA S 3011, 4400

        DateTime modificationTime ( std::max ( ctime, mtime) );

        item.setDateTime(modificationTime); 
    }


}
_ACS_END_NESTED_NAMESPACE

