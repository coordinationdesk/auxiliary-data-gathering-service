/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libCirculation$

	$Id$

	$Author$

	$Log$
	Revision 7.3  2018/06/14 14:42:24  damdec
	PDSEV-24: implementation finalized.
	
	Revision 7.2  2017/10/19 16:18:03  marpas
	getting rid of EntityBean
	
	Revision 7.1  2017/03/03 09:30:23  marfav
	S3MPM-874
	Aligned to libException Rev_5_93 new Condition Interface
	
	Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
	Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
	
	Revision 1.2  2016/08/31 15:54:08  marfav
	Added explicit DTOR with throw()
	
	Revision 1.1  2016/08/31 08:56:30  damdec
	Added.
	
        
*/

#ifndef _CirculationCallbackSingleton_H_
#define _CirculationCallbackSingleton_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>

#include <queue>

#include <dbPersistent.h>
#include <DateTime.h>
#include <Status.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

struct CirculationInfo // PRQA S 2171, 2173, 2175
{
public:
	CirculationInfo(): status_id(), timestamp(), extId(), url(), hostname(), circId() {};
	CirculationInfo(const CirculationInfo &r) : 
        status_id(r.status_id), 
        timestamp(r.timestamp), 
        extId(r.extId), 
        url(r.url),
		hostname(r.hostname),
		circId(r.circId)
    {}
	CirculationInfo &operator=(const CirculationInfo &r) 
    {
        if (&r != this) {
            status_id = r.status_id ;
            timestamp = r.timestamp ;
            extId = r.extId ;
            url = r.url ;
			hostname = r.hostname;
			circId = r.circId;
        }
        return *this;
    }
    ~CirculationInfo() throw() {}

    // PRQA S 2100 5    	
	std::string status_id;			// circulation status 
	DateTime timestamp;			// circulation status modification date
	dbPersistent::IdType extId; // cross table id (t_dsqueue_x_cirqueue table)
	std::string url;			// callback address 
	std::string hostname;
	dbPersistent::IdType circId; // circulation id
};

/**
* \brief
* Singleton used to handle info exchange between DS and DC 
**/

class CirculationCallbackSingleton : public Singleton <CirculationCallbackSingleton> // PRQA S 2109, 2153
{ 
public:
	//DTOR
	virtual ~CirculationCallbackSingleton() throw();	

	size_t size();
	bool empty();
	// Returns a flag indicating if the queue is not empty
	bool getCallbackEventsFromQueue (CirculationInfo& item);
	// insert pending circulation info in a queue
	void insertCallbackEventsInQueue (dbConnection& conn, const dbPersistent::IdType & circ_id, const short & status_id, const DateTime & updatetime);

	Condition & getStandbyCondition();

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	CirculationCallbackSingleton();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	CirculationCallbackSingleton(const CirculationCallbackSingleton & );
	/** Operator = */
	CirculationCallbackSingleton &operator=(const CirculationCallbackSingleton &);
	friend CirculationCallbackSingleton* Singleton<CirculationCallbackSingleton>::instance(); // PRQA S 2107

#ifdef ACS_TEST
public:
	void clear() { _pendingCirculations = std::queue<CirculationInfo>() ; }
	CirculationInfo front() { return _pendingCirculations.front() ; }
#else
private:
#endif
	void push(const CirculationInfo & info);	// push back

protected:

private:
	std::queue<CirculationInfo> _pendingCirculations;		// Pending Circulations
	/** Synch with consumer */
	Condition _standbyCondition;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationCallbackSingleton)
};

_ACS_END_NAMESPACE

#endif //_CirculationCallbackSingleton_H_

