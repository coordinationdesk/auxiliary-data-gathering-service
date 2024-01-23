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
 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.2  2013/10/28 17:33:53  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:49:02  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.6  2013/04/05 09:51:07  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.5  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.4  2013/02/13 17:06:30  micmaz
 *  executed regression tests.
 *
 *  Revision 1.3  2012/11/29 16:55:05  micmaz
 *  work in progress
 *
 *  Revision 1.2  2012/04/04 09:34:08  micmaz
 *  work in progress
 *
 *  Revision 1.1  2012/04/03 10:11:06  micmaz
 *  work in progress
 *
 *
 */

#ifndef _MediaUpdater_H_
#define _MediaUpdater_H_

#include <acs_c++config.hpp>
#include"MediaSigner.h"
#include<Circulation.h>
#include <CirculationDBService.h>

#include<boost/shared_ptr.hpp> // PRQA S 1013
#include<map>
#include<vector>


_ACS_BEGIN_NAMESPACE(acs) ;
class dbConnection ;
_ACS_END_NAMESPACE;

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;


/*
 *
 */
class MediaUpdater { // PRQA S 2109
public:
    MediaUpdater();
    ~MediaUpdater() throw();
private:
    MediaUpdater(MediaUpdater const& other);
    MediaUpdater& operator=(MediaUpdater const& other);
public:
    bool update(dbConnection &, std::vector<SignedFile> const& files);
private:
    Circulation* findCirculation(dbConnection &, long circulationId);
    bool checkFile(SignedFile const& file) ;
private:
    std::map<long, boost::shared_ptr<circulation::Circulation> > _circs;
    CirculationDBService _service;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(MediaUpdater) ;
};

exDECLARE_EXCEPTION(MediaUpdaterException, exException); // PRQA S 2131, 2502


_ACS_END_NESTED_NAMESPACE;

#endif /* _MediaUpdater_H_ */
