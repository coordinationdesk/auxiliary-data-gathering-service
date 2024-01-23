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
 *  $Prod: A.C.S.  $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.4  2014/02/14 13:54:32  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.3  2013/10/24 18:28:00  marpas
 *  work in progress
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
 *  Revision 2.2  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.5  2011/06/16 18:27:12  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/05/05 17:52:33  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/05/03 14:01:34  micmaz
 *  corrected some circulation bugs and add CurlStatistics class
 *
 *  Revision 1.2  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.1  2011/02/24 11:41:10  micmaz
 *  Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.
 *
 *
 *
 *
 */

#include <RemoteDirVisitor.h>

#include <FtpCurl.h>
#include <FtpListItem.h>
#include <FtpListReader.h>

#include <StringUtils.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL( RemoteDirVisitor);


RemoteDirVisitor::RemoteDirVisitor(VisitFileCallback fileCallback, VisitDirCallback dirCallback, void* visitor) :  // NOSONAR - callback data
    _fileCallback(fileCallback),
    _dirCallback(dirCallback), 
    _visitor(visitor) 
{
    // empty.
}

void RemoteDirVisitor::setVisitor(VisitFileCallback fileCallback, VisitDirCallback dirCallback, void* visitor)  // NOSONAR - callback data
{
    _fileCallback = fileCallback;
    _dirCallback = dirCallback;
    _visitor = visitor;
}

void RemoteDirVisitor::nextLineCallback(const std::string &line, void* data) { // NOSONAR - callback 
    vector<FtpListItem> *lines = reinterpret_cast<vector<FtpListItem>*> (data); // NOSONAR - callback 
    FtpListItem item;
    item.setLine(line) ;
    if (!StringUtils::equalsNoCase("..", item.getName()) && !StringUtils::equalsNoCase(".", item.getName())) {
        lines->push_back(item);
    }
}

void RemoteDirVisitor::visit(RemoteSite const& site, const string &remoteDirectory) // PRQA S 4211
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[visitWithSource: " << remoteDirectory << "]");

    deque < string > subDirs;
    subDirs.push_back(remoteDirectory);
    try {
        while (!subDirs.empty()) {
            string remotePath = subDirs.front();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current path: " << remotePath);
            subDirs.pop_front();
            vector<FtpListItem> lines;
            curl::BandPriority no_priority ;
            FtpCurl ftpCurl(no_priority, site);
            ftpCurl.setRempath(remotePath);

            ftpCurl.list(RemoteDirVisitor::nextLineCallback, &lines);

            size_t l_size = lines.size() ;
            for (size_t counter = 0; counter < l_size; ++counter) {
                string remoteFile = lines[counter].getName();
                if (StringUtils::equalsNoCase("..", remoteFile) || StringUtils::equalsNoCase(".", remoteFile)) {
                    continue;
                }
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "remotePath :  " << remotePath);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "remoteFile :  " << remoteFile);
                string relativeRemoteName = remotePath.substr(remoteDirectory.length(), std::string::npos - remoteDirectory.length()); // PRQA S 3084
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relativeRemoteName  :  " << relativeRemoteName);
                if (!lines[counter].getPermissions().isDirectory()) {
                    _fileCallback(remotePath + "/" + remoteFile, relativeRemoteName + "/" + remoteFile, _visitor);
                } 
                else {
                    _dirCallback(remotePath + "/" + remoteFile, relativeRemoteName + "/" + remoteFile, _visitor);
                    subDirs.push_back(remotePath + "/" + remoteFile);
                }
            }
        }
    } 
    catch (std::exception const& ex) { // NOSONAR - any exception 
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex.what());
        throw ;
    }
}

_ACS_END_NESTED_NAMESPACE

