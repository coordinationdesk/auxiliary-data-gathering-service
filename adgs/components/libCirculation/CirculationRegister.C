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
 *  Revision 5.5  2014/08/06 16:08:24  lucio.pulvirenti
 *  S2PDGS-835: Exceptions notificated by proper macro not to send stack on syslog.
 *
 *  Revision 5.4  2014/04/15 15:40:48  marpas
 *  getting rid of bad log macros
 *
 *  Revision 5.3  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/18 12:29:50  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.18  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.17  2012/11/29 16:55:05  micmaz
 *  work in progress
 *
 *  Revision 1.16  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.15  2012/03/29 10:51:52  micmaz
 *  work in progress
 *
 *  Revision 1.14  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.13  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.12  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.11  2011/05/12 17:22:32  micmaz
 *  work in progress
 *
 *  Revision 1.10  2011/05/09 16:25:03  micmaz
 *  work in progress...
 *
 *  Revision 1.9  2011/04/27 12:46:17  micmaz
 *  corrected the stop procedure
 *
 *  Revision 1.8  2011/04/27 09:50:52  micmaz
 *  errored ftp transfers must be logged.
 *
 *  Revision 1.7  2011/04/26 14:42:40  micmaz
 *  updated logs.
 *
 *  Revision 1.6  2011/04/15 18:02:54  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/04/15 15:33:37  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/13 14:25:44  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/04/12 18:00:29  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/11 12:25:16  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:14:24  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#include <CirculationRegister.h>
#include <RemoteSiteFactory.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;
using namespace std;
using namespace acs::curl;
using namespace acs::dc;


CirculationRegister::CirculationRegister(StopController& stopper, string const& name, int limit) :
        GenericRegister<CirculationItem>(stopper, name, limit) {
}
CirculationRegister::CirculationRegister(StopController& stopper, const char*  name, int limit) :
        GenericRegister<CirculationItem>(stopper, name, limit) {
}
CirculationRegister::~CirculationRegister() throw() {
    //no-op
}

void CirculationRegister::log(int errorCode, CirculationItem& item) {
    if (errorCode != conf::SUCCESS_TRANSFER) { // PRQA S 1021
		ACS_LOG_WARNING(item.getLastException() << " error " << errorCode << ". Error in transferring " << item.id());
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Item successfully transferred.  \n\t" << item.pItem()->str() << ")");
    }
}

void CirculationRegister::logDetails(CirculationItem& item) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "***        " << item.id());
}

void CirculationRegister::logItemReleasedWithSuccess(acs::pc::SyncPriorityContainer<CirculationItem>& consuming, CirculationItem &item) {
    log(item.lastErrorCode(), item);
}

void CirculationRegister::logItemAcquired(acs::pc::SyncPriorityContainer<CirculationItem>& consuming, CirculationItem &item) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transferring:        " << item.id());
}

size_t CirculationRegister::getElapsedTimeToRetry() const {
    return conf::dcConfigurerSingleton::instance()->getElapsedTimeToRetry() ;
}

size_t CirculationRegister::getMaxAttempts() const {
    return 1;
}


_ACS_END_NESTED_NAMESPACE;

