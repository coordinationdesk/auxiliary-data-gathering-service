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
	Revision 2.2  2011/09/29 19:16:39  marpas
	wrapper name fixed
	
	Revision 2.1  2006/10/24 15:56:26  marfav
	Addedd support for string-encapsulated parameters file
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/03/09 09:26:08  marfav
	TlOrderQueue added
	
	Revision 1.8  2004/12/03 18:04:42  marfav
	Header fixed
	
	Revision 1.7  2004/10/19 08:19:31  marfav
	Added satellite and mission isSet flag support
	
	Revision 1.6  2004/08/24 14:52:01  marfav
	Added mission support
	
	Revision 1.5  2004/05/05 10:50:39  marfav
	Added support for SUSPENDED status
	
	Revision 1.4  2003/09/15 10:22:51  marfav
	processedFileName field added
	
	Revision 1.3  2003/04/18 12:48:23  marfav
	Using namespaces removed from .h
	
	Revision 1.2  2003/03/11 18:37:37  marfav
	Improved logging
	
	Revision 1.1.1.1  2003/01/30 15:17:07  danalt
	Import libTlOrder
	

*/


#ifndef _tlOrder_H_
#define _tlOrder_H_

#include <DateTime.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

enum tlOrderStatus
{
  OrderStatusUNKNOWN = -1,
  OrderStatusELIGIBLE,
  OrderStatusCHECKING,
  OrderStatusPROCESSING,
  OrderStatusDONE,
  OrderStatusRECOVERABLE_FAILURE,
  OrderStatusFATAL_FAILURE,
  OrderStatusDOWNLOADING,
  OrderStatusSUSPENDED
};
std::string OrderStatus2String(tlOrderStatus t);
tlOrderStatus string2OrderStatus(const std::string& s);

//
//
//
// class tlOrder
//
//
//
class tlOrder
{
public:

	tlOrder() ;
	tlOrder(const tlOrder &) ;
	virtual ~tlOrder();
	tlOrder &operator=(const tlOrder &) ; // operator =
    bool compare (const tlOrder &) const throw() ;

	void reset();

	void setOrderId(unsigned long) ;
	unsigned long getOrderId() const throw() { return _orderId ; }

	void setCreationDate(const acs::DateTime &) ;
	const acs::DateTime &getCreationDate() const throw() { return _creationDate ; }

	void setPriority(int) ;
	int getPriority() const throw() { return _priority ; }

	void setStatus(const std::string &) ;
	const std::string &getStatus() const throw() { return _status ; }

	void setProcessorName(const std::string &) ;
	const std::string &getProcessorName(bool *set=0) const throw() ;

	void setProcessorVersion(const std::string &) ;
	const std::string &getProcessorVersion(bool *set=0) const throw()  ;

	void setProcessorConfiguration(const std::string &) ;
	const std::string &getProcessorConfiguration() const throw() { return _processorConfiguration ; }

	void setTroubleshooting(bool) throw() ;
	bool getTroubleshooting() const throw() { return _troubleshooting ; }

	void setStartTime(const acs::DateTime &) ;
	const acs::DateTime &getStartTime() const throw() { return _startTime ; }

	void setStopTime(const acs::DateTime &) ;
	const acs::DateTime &getStopTime() const throw() { return _stopTime ; }

	void setJobResponsible(const std::string &) ;
	const std::string &getJobResponsible() const throw() { return _jobResponsible ; }


	void setProductionStartTime(const acs::DateTime &) ;
	const acs::DateTime &getProductionStartTime(bool *set=0) const throw() ;

	void setProductionStopTime(const acs::DateTime &) ;
	const acs::DateTime &getProductionStopTime(bool *set=0) const throw() ;

	void setProcessingTimeWindow(int) ;
	int getProcessingTimeWindow() const throw() { return _processingTimeWindow ; }


	void setProcessorId(const std::string &) ;
	const std::string &getProcessorId() const throw() { return _processorId ; }

	void setOrderType(const std::string &) ;
	const std::string &getOrderType(bool *set=0) const throw() ;

	void setExternalOrderId(const std::string &) ;
	const std::string &getExternalOrderId() const throw() { return _externalOrderId ; }

	std::string getOrderSignature() const ;

	void setProcessedFileName (const std::string &);
	const std::string& getProcessedFileName (bool *set=0) const throw() ;

	void setSatelliteId(int);
	int getSatelliteId(bool* set=0) const throw() ;
	
	void setMission (int) ;
	int getMission (bool* set=0) const throw() ;

	void setParametersCLOB (const std::string&);
	const std::string&  getParametersCLOB (bool* set=0) const throw() ;
   
    unsigned long getOldOrderId() const throw() { return _orderIdOld ; }
private:
	unsigned long _orderIdOld ;
	unsigned long _orderId ;

	acs::DateTime _creationDate ;

	int _priority ;

	std::string _status ;

	std::string _processorName ;
	bool _processorNameSet ;

	std::string _processorVersion ;
	bool _processorVersionSet ;

	std::string _processorConfiguration ;

	bool _troubleshooting ;

	acs::DateTime _startTime ;

	acs::DateTime _stopTime ;

	std::string _jobResponsible ;

	acs::DateTime _productionStartTime ;
	bool _productionStartTimeSet ;

	acs::DateTime _productionStopTime ;
	bool _productionStopTimeSet ;

	int _processingTimeWindow ;

	std::string _processorId ;

	std::string _orderType ;
	bool _orderTypeSet ;

	std::string _externalOrderId ;

	std::string _processedFileName;
	bool _processedFileNameSet;

	int _satelliteId;
	bool _satelliteIdSet;
	
	int _mission;
	bool _missionSet;

	std::string _parametersCLOB;
	bool _parametersCLOBSet;
} ;

bool operator== (const tlOrder &, const tlOrder &) ;

_ACS_END_NAMESPACE

#endif // _tlOrder_H_
