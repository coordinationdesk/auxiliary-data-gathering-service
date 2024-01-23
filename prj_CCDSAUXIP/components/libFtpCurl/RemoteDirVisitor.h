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
 *  Revision 5.1  2013/07/23 17:10:12  marpas
 *  classes rationalization and robustness improved
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.1  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.3  2011/09/29 16:46:53  marpas
 *  rules fixing
 *
 *  Revision 1.2  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.1  2011/02/24 11:41:10  micmaz
 *  Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.
 *
 *
 *
 *
 */

#ifndef _RemoteDirVisitor_H_
#define _RemoteDirVisitor_H_

#include <RemoteSite.h>

#include <exMacros.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

class RemoteDirVisitor { // PRQA S 2109
public:
    using VisitFileCallback = void (*)(const std::string &remoteFile, const std::string &relativeRemoteFile, void* observer);
    using VisitDirCallback = void (*)(const std::string &remoteFile, const std::string &relativeRemoteFile, void* observer);

    RemoteDirVisitor() = default ;
    RemoteDirVisitor(VisitFileCallback fileCallback, VisitDirCallback dirCallback, void* visitor);

    RemoteDirVisitor(RemoteDirVisitor const& ) = delete ;
    RemoteDirVisitor& operator=(RemoteDirVisitor const& ) = delete ;

    ~RemoteDirVisitor() = default ;
    void setVisitor(VisitFileCallback fileCallback, VisitDirCallback dirCallback, void* visitor);

    static void nextLineCallback(const std::string &line, void* data);

    void visit(RemoteSite const& site, const std::string &remoteDirectory);

private:
    VisitFileCallback _fileCallback = nullptr ;
    VisitDirCallback _dirCallback = nullptr ;
    void* _visitor = nullptr ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(RemoteDirVisitor) ;

};

_ACS_END_NESTED_NAMESPACE

#endif /* _RemoteDirVisitor_H_ */
