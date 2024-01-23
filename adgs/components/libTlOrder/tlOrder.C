// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Order Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2015/11/19 13:17:54  marpas
	coding best practices and rules applied
	
	Revision 2.2  2012/06/06 16:53:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.1  2006/10/24 15:56:26  marfav
	Addedd support for string-encapsulated parameters file
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/03/09 09:26:08  marfav
	TlOrderQueue added
	
	Revision 1.11  2004/12/03 18:04:42  marfav
	Header fixed
	
	Revision 1.10  2004/10/19 08:19:31  marfav
	Added satellite and mission isSet flag support
	
	Revision 1.9  2004/08/24 14:52:01  marfav
	Added mission support
	
	Revision 1.8  2004/05/05 10:50:39  marfav
	Added support for SUSPENDED status
	
	Revision 1.7  2004/02/04 11:05:14  marfav
	Recoverable Error misspell fixed
	
	Revision 1.6  2003/09/15 10:22:51  marfav
	processedFileName field added
	
	Revision 1.5  2003/06/05 14:15:31  marfav
	Removed tabs from messages
	
	Revision 1.4  2003/04/18 12:48:23  marfav
	Using namespaces removed from .h
	
	Revision 1.3  2003/03/11 18:37:37  marfav
	Improved logging
	
	Revision 1.2  2003/02/07 08:37:40  fracar
	fixed mispelled PRECONDICTIONCHECK order status
	
	Revision 1.1.1.1  2003/01/30 15:17:07  danalt
	Import libTlOrder
	


*/

#include <tloV.h>
#include <tlOrder.h>
#include <string>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
namespace { // unnamed 
    tloV version;
}

string OrderStatus2String(tlOrderStatus t) // PRQA S 4020
{
  switch (t)
  {
    case OrderStatusPROCESSING: { return "PROCESSING"; }
    case OrderStatusELIGIBLE: { return "ELIGIBLE"; }
    case OrderStatusCHECKING: { return "PRECONDITIONCHECK"; }
    case OrderStatusDONE: { return "COMPLETED"; }
    case OrderStatusRECOVERABLE_FAILURE: { return "RECOVERABLEERROR"; }
    case OrderStatusFATAL_FAILURE: { return "ERROR"; }
    case OrderStatusDOWNLOADING: { return "DOWNLOADING"; }
    case OrderStatusSUSPENDED: { return "SUSPENDED"; }
    default: { return "unknown"; }
  }
}

tlOrderStatus string2OrderStatus(const string& s) // PRQA S 4020
{
    if (s == "DONE") {
      return OrderStatusDONE;
    }
    if (s == "PROCESSING") {
      return OrderStatusPROCESSING;
    }
    if (s == "ELIGIBLE") {
      return OrderStatusELIGIBLE;
    }
    if (s == "PRECONDITIONCHECK") {
      return OrderStatusCHECKING;
    }
    if (s == "COMPLETED") {
      return OrderStatusDONE;
    }
    if (s == "RECOVERABLE_ERROR") {
      return OrderStatusRECOVERABLE_FAILURE;
    }
    if (s == "ERROR") {
      return OrderStatusFATAL_FAILURE;
    }
    if (s == "DOWNLOADING") {
      return OrderStatusDOWNLOADING;
    }
    
    if (s == "SUSPENDED") {
      return OrderStatusSUSPENDED;
    }
    else {
      return OrderStatusUNKNOWN;
    }
}



// constructor
tlOrder::tlOrder() :
	_orderIdOld (),
	_orderId (),
	_creationDate (false),
	_priority (),
	_status (),
	_processorName (),
	_processorNameSet (),
	_processorVersion (),
	_processorVersionSet (),
	_processorConfiguration (),
	_troubleshooting (),
	_startTime (false),
	_stopTime (false),
	_jobResponsible (),
	_productionStartTime (),
	_productionStartTimeSet (),
	_productionStopTime (),
	_productionStopTimeSet (),
	_processingTimeWindow (),
	_processorId (),
	_orderType (),
	_orderTypeSet (),
	_externalOrderId (),
	_processedFileName (),
	_processedFileNameSet (),
	_satelliteId (),
	_satelliteIdSet (),
	_mission (),
	_missionSet (),
	_parametersCLOB (),
	_parametersCLOBSet ()
{
}


// copy constructor
tlOrder::tlOrder(const tlOrder &o) :
	_orderIdOld (o._orderIdOld),
	_orderId (o._orderId),
	_creationDate (o._creationDate),
	_priority (o._priority),
	_status (o._status),
	_processorName (o._processorName),
	_processorNameSet (o._processorNameSet),
	_processorVersion (o._processorVersion),
	_processorVersionSet (o._processorVersionSet),
	_processorConfiguration (o._processorConfiguration),
	_troubleshooting (o._troubleshooting),
	_startTime (o._startTime),
	_stopTime (o._stopTime),
	_jobResponsible (o._jobResponsible),
	_productionStartTime (o._productionStartTime),
	_productionStartTimeSet (o._productionStartTimeSet),
	_productionStopTime (o._productionStopTime),
	_productionStopTimeSet (o._productionStopTimeSet),
	_processingTimeWindow (o._processingTimeWindow),
	_processorId (o._processorId),
	_orderType (o._orderType),
	_orderTypeSet (o._orderTypeSet),
	_externalOrderId (o._externalOrderId),
	_processedFileName (o._processedFileName),
	_processedFileNameSet (o._processedFileNameSet),
	_satelliteId (o._satelliteId),
	_satelliteIdSet (o._satelliteIdSet),
	_mission (o._mission),
	_missionSet (o._missionSet),
	_parametersCLOB (o._parametersCLOB),
	_parametersCLOBSet (o._parametersCLOBSet)
{
	// *this = o;
}


tlOrder::~tlOrder()
{
}


bool tlOrder::compare(const tlOrder &o) const throw()
{
	return ((_orderIdOld==o._orderIdOld) &&
		(_orderId==o._orderId) &&
		(_creationDate==o._creationDate) &&
		(_priority==o._priority) &&
		(_status==o._status) &&
		(_processorName==o._processorName) &&
		(_processorNameSet==o._processorNameSet) &&
		(_processorVersion==o._processorVersion) &&
		(_processorVersionSet==o._processorVersionSet) &&
		(_processorConfiguration==o._processorConfiguration) &&
		(_troubleshooting==o._troubleshooting) &&
		(_startTime==o._startTime) &&
		(_stopTime==o._stopTime) &&
		(_jobResponsible==o._jobResponsible) &&
		(_productionStartTime==o._productionStartTime) &&
		(_productionStartTimeSet==o._productionStartTimeSet) &&
		(_productionStopTime==o._productionStopTime) &&
		(_productionStopTimeSet==o._productionStopTimeSet) &&
		(_processingTimeWindow==o._processingTimeWindow) &&
		(_processorId==o._processorId) &&
		(_orderType==o._orderType) &&
		(_orderTypeSet==o._orderTypeSet) &&
		(_externalOrderId==o._externalOrderId) &&
		(_processedFileName == o._processedFileName) &&
		(_processedFileNameSet == o._processedFileNameSet) &&
		(_satelliteId == o._satelliteId) &&
		(_satelliteIdSet == o._satelliteIdSet) &&
		(_mission == o._mission) &&
		(_missionSet == o._missionSet) &&
		(_parametersCLOB == o._parametersCLOB) &&
		(_parametersCLOBSet == o._parametersCLOBSet) );
}


// operator ==
bool operator==(const tlOrder &a, const tlOrder &b) 
{
	return a.compare(b) ;
}


// operator =
tlOrder &tlOrder::operator=(const tlOrder &o)
{
	if(this != &o)
	{
		_orderIdOld=o._orderIdOld;
		_orderId=o._orderId;
		_creationDate=o._creationDate;
		_priority=o._priority;
		_status=o._status;
		_processorName=o._processorName;
		_processorNameSet=o._processorNameSet;
		_processorVersion=o._processorVersion;
		_processorVersionSet=o._processorVersionSet;
		_processorConfiguration=o._processorConfiguration;
		_troubleshooting=o._troubleshooting;
		_startTime=o._startTime;
		_stopTime=o._stopTime;
		_jobResponsible=o._jobResponsible;
		_productionStartTime=o._productionStartTime;
		_productionStartTimeSet=o._productionStartTimeSet;
		_productionStopTime=o._productionStopTime;
		_productionStopTimeSet=o._productionStopTimeSet;
		_processingTimeWindow=o._processingTimeWindow;
		_processorId=o._processorId;
		_orderType=o._orderType;
		_orderTypeSet=o._orderTypeSet;
		_externalOrderId=o._externalOrderId;
		_processedFileName = o._processedFileName;
		_processedFileNameSet = o._processedFileNameSet;
		_satelliteId = o._satelliteId;
		_satelliteIdSet = o._satelliteIdSet;
		_mission = o._mission;
		_missionSet = o._missionSet;
		_parametersCLOB = o._parametersCLOB;
		_parametersCLOBSet = o._parametersCLOBSet;
	}
	return *this ;
}


// initialize the class
void tlOrder::reset()
{
	_orderIdOld=0 ;
	_orderId=0 ;
	_creationDate.reset() ;
	_priority=0 ;
	_status="" ;
	_processorName="" ;
	_processorNameSet=false ;
	_processorVersion="" ;
	_processorVersionSet=false ;
	_processorConfiguration="" ;
	_troubleshooting=false ;
	_startTime.reset() ;
	_stopTime.reset() ;
	_jobResponsible="" ;
	_productionStartTime.reset() ;
	_productionStartTimeSet=false ;
	_productionStopTime.reset() ;
	_productionStopTimeSet=false ;
	_processingTimeWindow=0 ;
	_processorId="" ;
	_orderType="" ;
	_orderTypeSet=false ;
	_externalOrderId="" ;
	_processedFileName = "";
	_processedFileNameSet = false;
	_satelliteId = 0;
	_satelliteIdSet = false;
	_mission = 0;
	_missionSet = false;
	_parametersCLOB = "";
	_parametersCLOBSet = false;
}


void
tlOrder::setSatelliteId(int id)
{
	_satelliteId = id;
	_satelliteIdSet = true;
}

int
tlOrder::getSatelliteId(bool* set) const throw() 
{
	if (set) 
	{
		*set = _satelliteIdSet;
	}
	return _satelliteId;
}

void tlOrder::setOrderId(unsigned long v)
{
	if (_orderId != v ) 
	{
		_orderId = v ;
	}
}


void tlOrder::setCreationDate(const DateTime &v)
{
	if (_creationDate != v ) 
	{
		_creationDate = v ;
	}
}



void tlOrder::setPriority(int v)
{
	if (_priority != v ) 
	{
		_priority = v ;
	}
}

void tlOrder::setStatus(const string &v)
{
	if (_status != v ) 
	{
		_status = v ;
	}
}

void tlOrder::setProcessorName(const string &v)
{
	if (!_processorNameSet || _processorName != v ) 
	{
		_processorName = v ;
		_processorNameSet = true ;
	}
}


const string &tlOrder::getProcessorName(bool *set) const throw() 
{
	if (0 != set) {
		*set = _processorNameSet ;
    }
	return _processorName ;
}


void tlOrder::setProcessorVersion(const string &v)
{
	if (!_processorVersionSet || _processorVersion != v ) 
	{
		_processorVersion = v ;
		_processorVersionSet = true ;
	}
}


const string &tlOrder::getProcessorVersion(bool *set) const throw() 
{
	if (0 != set) {
		*set = _processorVersionSet ;
    }
	return _processorVersion ;
}


void tlOrder::setProcessorConfiguration(const string &v)
{
	if (_processorConfiguration != v ) 
	{
		_processorConfiguration = v ;
	}
}


void tlOrder::setTroubleshooting(bool v) throw() 
{
	if (_troubleshooting != v ) 
	{
		_troubleshooting = v ;
	}
}



void tlOrder::setStartTime(const DateTime &v)
{
	if (_startTime != v ) 
	{
		_startTime = v ;
	}
}


void tlOrder::setStopTime(const DateTime &v)
{
	if (_stopTime != v ) 
	{
		_stopTime = v ;
	}
}

void tlOrder::setJobResponsible(const string &v)
{
	if (_jobResponsible != v ) 
	{
		_jobResponsible = v ;
	}
}



void tlOrder::setProductionStartTime(const DateTime &v)
{
	if (!_productionStartTimeSet || _productionStartTime != v ) 
	{
		_productionStartTime = v ;
		_productionStartTimeSet = true ;
	}
}


const DateTime &tlOrder::getProductionStartTime(bool *set) const throw()
{
	if (set) {
		*set = _productionStartTimeSet ;
    }
	return _productionStartTime ;
}

void tlOrder::setProcessedFileName (const string &s)
{
	if (!_processedFileNameSet || _processedFileName != s )
	{
		_processedFileName = s ;
		_processedFileNameSet = true ;
	}
}

const string &tlOrder::getProcessedFileName (bool *set) const throw() 
{
	if (set) {
		*set = _processedFileNameSet ;
    }
	return _processedFileName ;
}


void tlOrder::setProductionStopTime(const DateTime &v)
{
	if (!_productionStopTimeSet || _productionStopTime != v ) 
	{
		_productionStopTime = v ;
		_productionStopTimeSet = true ;
	}
}


const DateTime &tlOrder::getProductionStopTime(bool *set) const throw() 
{
	if (set) {
		*set = _productionStopTimeSet ;
    }
	return _productionStopTime ;
}


void tlOrder::setProcessingTimeWindow(int v)
{
	if (_processingTimeWindow != v ) 
	{
		_processingTimeWindow = v ;
	}
}


void tlOrder::setProcessorId(const string &v)
{
	if (_processorId != v ) 
	{
		_processorId = v ;
	}
}


void tlOrder::setOrderType(const string &v)
{
	if (!_orderTypeSet || _orderType != v ) 
	{
		_orderType = v ;
		_orderTypeSet = true ;
	}
}


const string &tlOrder::getOrderType(bool *set) const throw() 
{
	if (set) {
		*set = _orderTypeSet ;
    }
	return _orderType ;
}

void tlOrder::setMission(int m)
{
	_mission = m;
	_missionSet = true;
}

int tlOrder::getMission(bool* set) const throw() 
{
	if (set) 
	{
		*set = _missionSet;
	}
	return _mission;
}


void tlOrder::setExternalOrderId(const string &v)
{
	if (_externalOrderId != v ) 
	{
		_externalOrderId = v ;
	}
}



string tlOrder::getOrderSignature() const
{
	string s1 ;
    string s2 ;
	ostringstream os;
	os << "ID#" << getOrderId() << " [" <<
	      getStartTime().utcs(s1) << "  --  " << getStopTime().utcs(s2) << "]";
	return os.str();
}

void tlOrder::setParametersCLOB (const string& p)
{
	_parametersCLOB = p;
	_parametersCLOBSet = true;
}

const std::string& tlOrder::getParametersCLOB (bool* set) const throw() 
{
	if (set)
	{
		*set = _parametersCLOBSet;
	}
	return _parametersCLOB;
}
_ACS_END_NAMESPACE
