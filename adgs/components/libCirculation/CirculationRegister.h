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
 *  $Prod: A.C.S. libCirculation Library $$
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
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
 *  Revision 1.9  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.8  2012/11/29 16:55:05  micmaz
 *  work in progress
 *
 *  Revision 1.7  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.6  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.5  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.4  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.3  2011/04/26 14:42:40  micmaz
 *  updated logs.
 *
 *  Revision 1.2  2011/04/13 14:25:44  micmaz
 *  work in progress...
 *
 *  Revision 1.1  2011/04/05 17:14:24  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#ifndef CIRCULATIONREGISTER_H_
#define CIRCULATIONREGISTER_H_

#include <acs_c++config.hpp>
#include<GenericRegister.h>
#include<CirculationItem.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

using namespace acs::dc;


class CirculationRegister: public GenericRegister<CirculationItem> {

public:

    /**
     * The default ctor.
     * The limit parameter is used to limit the channel capacity (up to now still not used).
     */
    explicit CirculationRegister(StopController& stopper, std::string const& name="", int limit = -1);

    /**
     * The default ctor.
     * The limit parameter is used to limit the channel capacity (up to now still not used).
     */
    explicit CirculationRegister(StopController& stopper, const char*  name, int limit = -1);

    virtual ~CirculationRegister() throw();

protected:
    virtual void log(int errorCode, CirculationItem& item); // PRQA S 2502

    virtual void logDetails(CirculationItem& item);

    virtual void logItemAcquired(acs::pc::SyncPriorityContainer<CirculationItem>& consuming, CirculationItem &item);

    virtual void logItemReleasedWithSuccess(acs::pc::SyncPriorityContainer<CirculationItem>& consuming, CirculationItem &item);

    virtual size_t getElapsedTimeToRetry() const ;

    virtual size_t getMaxAttempts() const ;
private:
    CirculationRegister() ; // not implemented
    CirculationRegister(const CirculationRegister &) ;  // not implmented
    CirculationRegister& operator= (const CirculationRegister &) ;  // not implmented
};


_ACS_END_NESTED_NAMESPACE


#endif /* CIRCULATIONREGISTER_H_ */
