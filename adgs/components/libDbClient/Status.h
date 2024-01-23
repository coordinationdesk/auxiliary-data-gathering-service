// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2018, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. libDbClient Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 1.9  2018/07/24 09:05:49  lucio.pulvirenti
 *  S3EVOL-159: enabled, disabled, blocked status added.
 *
 *  Revision 1.8  2017/05/29 17:53:30  nicvac
 *  OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
 *
 *  Revision 1.7  2015/09/01 13:32:33  lucio.pulvirenti
 *  S2PDGS-1247: processed status added (already occurring in db).
 *
 *  Revision 1.6  2014/10/24 08:21:09  lucio.pulvirenti
 *  LTA_PROCESSING status removed.
 *
 *  Revision 1.5  2014/10/23 16:22:20  lucio.pulvirenti
 *  S2PDGS-952 LTA_PROCESSING status added. Method added to get the new status since it could be not configured.
 *
 *  Revision 1.4  2013/10/31 11:22:37  marpas
 *  adding Status::waitingack
 *
 *  Revision 1.3  2013/10/31 10:58:26  marpas
 *  added suspended status
 *
 *  Revision 1.2  2013/07/12 07:06:30  marpas
 *  merging DSStatus static data member
 *
 *  Revision 1.1  2013/07/12 06:59:05  marpas
 *  NameIdAssociator derived class, moved here because t_stati is a lower level table in PDS
 *
 *
 *
 */

#ifndef _Status_H_
#define _Status_H_

#include <acs_c++config.hpp>
#include <NameIdAssociator.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs) 


using pattern::ParamConstructibleSingletonMap ;

class Status: // PRQA S 2109, 2153
    public NameIdAssociator<short>,
    public ParamConstructibleSingletonMap<Status, std::string> {
    
    friend Status* ParamConstructibleSingletonMap<Status, std::string>::instance(const std::string &); // PRQA S 2107
public:
    // PRQA S 2100 L1
    std::string const static aborted;
    std::string const static completed;
    std::string const static deleting;
    std::string const static distribution;   
    std::string const static downloading;
    std::string const static error;
    std::string const static eligible;
    std::string const static preconditioncheck;
    std::string const static ltadownloading;
    std::string const static mediaAcknowledge;
    std::string const static mediaWait;
    std::string const static notFoundError;
    std::string const static processing;
    std::string const static recoverableError;
    std::string const static standby;
    std::string const static suspended;
    std::string const static transferring;
    std::string const static uploaded;
    std::string const static uploading;
    std::string const static waitingack;
    std::string const static processed;
    std::string const static enabled;
    std::string const static disabled;
    std::string const static blocked;
    std::string const static blocking;
    std::string const static booked;
    std::string const static acknowledged;
    std::string const static downloaditem;
    std::string const static preparing;
    std::string const static progress;
    std::string const static submitted;
    std::string const static transient;
    std::string const static uploaditem;
    std::string const static deleted;
    std::string const static invalid;
    std::string const static requested;
    std::string const static planned;
    std::string const static replanned;
    std::string const static executed;
    std::string const static abandoned;
    std::string const static ltaprocessing;
    std::string const static partiallytransferred;
    std::string const static todelete;
    std::string const static online;
    std::string const static ongoing;
    std::string const static evicted;

    // PRQA L:L1
public:
	virtual ~Status() throw() ;
	 
protected:
	explicit Status(const std::string &key) ;
private:
	Status &operator=(const Status &) ; // declared but not implemented
	Status (const Status &) ;          // declared but not implemented
	Status () ;                       // declared but not implemented

};


_ACS_END_NAMESPACE


#endif // _Status_H_ 
