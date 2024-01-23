// PRQA S 1050 EOF
/*
 *
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 *  $Prod: A.C.S. libDbClient Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 1.10  2018/07/24 09:05:49  lucio.pulvirenti
 *  S3EVOL-159: enabled, disabled, blocked status added.
 *
 *  Revision 1.9  2017/05/29 17:53:30  nicvac
 *  OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
 *
 *  Revision 1.8  2015/09/01 13:32:33  lucio.pulvirenti
 *  S2PDGS-1247: processed status added (already occurring in db).
 *
 *  Revision 1.7  2015/02/16 12:06:54  chicas
 *  changed DB status MEDIA_ACKNOLEDGE in MEDIA_ACKNOWLEDGE
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
 */

#include <Status.h>


_ACS_BEGIN_NAMESPACE(acs) 

std::string const Status::aborted("ABORTED");
std::string const Status::completed("COMPLETED");
std::string const Status::deleting("DELETING");
std::string const Status::distribution("DISTRIBUTION");
std::string const Status::downloading("DOWNLOADING");
std::string const Status::error("ERROR");
std::string const Status::eligible("ELIGIBLE");
std::string const Status::preconditioncheck("PRECONDITIONCHECK");
std::string const Status::ltadownloading("LTA_DOWNLOADING");
std::string const Status::mediaAcknowledge("MEDIA_ACKNOWLEDGE");
std::string const Status::mediaWait("MEDIA_WAIT");
std::string const Status::notFoundError("NOTFOUNDERROR");
std::string const Status::processing("PROCESSING");
std::string const Status::recoverableError("RECOVERABLEERROR");
std::string const Status::standby("STANDBY");
std::string const Status::suspended("SUSPENDED");
std::string const Status::transferring("TRANSFERRING");
std::string const Status::uploaded("UPLOADED");
std::string const Status::uploading("UPLOADING");
std::string const Status::waitingack("WAITINGACK");
std::string const Status::processed("PROCESSED");
std::string const Status::enabled("ENABLED");
std::string const Status::disabled("DISABLED");
std::string const Status::blocked("BLOCKED");
std::string const Status::blocking("BLOCKING");
std::string const Status::booked("BOOKED");
std::string const Status::acknowledged("ACKNOWLEDGED");
std::string const Status::downloaditem("DOWNLOADITEM");
std::string const Status::preparing("PREPARING");
std::string const Status::progress("PROGRESS");
std::string const Status::submitted("SUBMITTED");
std::string const Status::transient("TRANSIENT");
std::string const Status::uploaditem("UPLOADITEM");
std::string const Status::deleted("DELETED");
std::string const Status::invalid("INVALID");
std::string const Status::requested("REQUESTED");
std::string const Status::planned("PLANNED");
std::string const Status::replanned("RE-PLANNED");
std::string const Status::executed("EXECUTED");
std::string const Status::abandoned("ABANDONED");
std::string const Status::ltaprocessing("LTA_PROCESSING");
std::string const Status::partiallytransferred("PARTIALLY_TRANSFERRED");
std::string const Status::todelete("TO_DELETE");
std::string const Status::online("ONLINE");
std::string const Status::ongoing("ONGOING");
std::string const Status::evicted("EVICTED");


Status::Status(const std::string &key) :
    NameIdAssociator<short>("t_stati", "id", "status", key),
    ParamConstructibleSingletonMap<Status, std::string>()
{
    NameIdAssociator<short>::load() ;
}

_ACS_END_NAMESPACE

