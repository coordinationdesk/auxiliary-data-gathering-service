// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:01  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:29  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.12  2013/06/19 18:04:17  marpas
	orderid is a serial so IdType has to be used
	adopting libException 5.x standards
	
	Revision 2.11  2013/03/29 19:51:30  marpas
	removing html related functions
	
	Revision 2.10  2013/02/12 12:38:53  marpas
	interfaces improved over qa rules
	
	Revision 2.9  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.6  2012/06/15 09:54:30  marpas
	removing warning related to virtual method hiding
	
	Revision 2.5  2012/02/14 12:52:49  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.4  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.3  2009/06/03 16:14:51  marpas
	subrequest field added: for CNM
	
	Revision 2.2  2009/02/13 12:28:06  marpas
	constness added when downloading parameter file
	
	Revision 2.1  2006/10/24 16:01:40  marfav
	Now can export parameter files using streams
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.25  2005/06/17 10:31:55  marpas
	std namespaces took into consideration
	
	Revision 1.24  2004/10/14 15:15:39  paoscu
	mission added, conditioned to feature OrderHasStandByState
	
	Revision 1.23  2004/09/03 13:33:03  paoscu
	dbOrder::hasParametersFile removed
	dbOrder::getParameters changed into dbOrder::hasParameters
	bug fixed in updating internal dbOrder::_parameters member
	dbOrder::update can save parameters file too
	
	Revision 1.22  2004/09/02 18:27:27  paoscu
	getParameters method added
	hasParameterFile method I/F changed and reimplemented to call getPameters
	hasParameterFile method is obsolete and it is deprecated
	
	Revision 1.21  2004/08/26 14:40:01  paoscu
	dbOrder::hasParameterFile(...) method added
	
	Revision 1.20  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.19  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.18  2004/04/13 16:57:09  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.17  2004/03/04 18:55:06  paoscu
	SatelliteId and Parameters fields added.
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbOrder_H_
#define _dbOrder_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbPersistent.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class dbCryosatFactory ;

class dbRow ;
class rsResourceSet ;
class XMLOstream ;

//
//
//
// class dbOrder
//
//
//
class dbOrder :
	public dbPersistent
{
public:
	friend class dbCryosatFactory ; // PRQA S 2107

	virtual ~dbOrder() throw() ;
	dbOrder &operator=(const dbOrder &) ; // operator =

	static  std::string staticClassName() ;
	virtual dbPersistent *clone() const ;   // PRQA S 2502

	// inherited from dbPersistent
	virtual std::string className() const ;
	virtual void reset() ; // initialize the class
	virtual IdType save(dbConnection &)  ; // save the data in database
	virtual bool load(dbConnection & , IdType id=0)  ; // load the class from the database. Return true if obect exist
	virtual IdType update(dbConnection &)  ; // update an existing instance in database
	virtual IdType remove(dbConnection &)  ; // remove the class from the database   // PRQA S 2502
	virtual bool validate() const  ; // True if the class have valid parameters, false otherwise
	virtual void exportObj(rsResourceSet &s) const ; // export the class to rsResourceSet
	virtual void importObj(rsResourceSet &s)  ; // import the class from a rsResourceSet
	virtual void exportXML(XMLOstream &xst) const ; // export the class to XML stream

	int loadRow(const dbRow &, int offset=0) ;

	void setOrderId(IdType) ;
	IdType getOrderId() const ;

	void setOrderType(const std::string &) ;
	const std::string &getOrderType(bool *set=0) const ;
	void unsetOrderType();

	void setStatus(const std::string &) ;
	const std::string &getStatus(bool *set=0) const ;
	void unsetStatus();

	void setProcessorName(const std::string &) ;
	const std::string &getProcessorName(bool *set=0) const ;
	void unsetProcessorName();

	void setSatelliteId(unsigned int) ;
	unsigned int getSatelliteId(bool *set=0) const ;
	void unsetSatelliteId();

	void setMission(int) ;
	int getMission(bool *set=0) const ;
	void unsetMission();

	void setStartTime(const DateTime &) ;
	const DateTime &getStartTime(bool *set=0) const ;
	void unsetStartTime();

	void setStopTime(const DateTime &) ;
	const DateTime &getStopTime(bool *set=0) const ;
	void unsetStopTime();

	void setJobResponsible(const std::string &) ;
	const std::string &getJobResponsible(bool *set=0) const ;
	void unsetJobResponsible();

	void setProductionStartTime(const DateTime &) ;
	const DateTime &getProductionStartTime(bool *set=0) const ;
	void unsetProductionStartTime();

	void setProductionStopTime(const DateTime &) ;
	const DateTime &getProductionStopTime(bool *set=0) const ;
	void unsetProductionStopTime();

	void setExternalOrderId(const std::string &) ;
	const std::string &getExternalOrderId(bool *set=0) const ;
	void unsetExternalOrderId();

	void setCreationDate(const DateTime &) ;
	const DateTime &getCreationDate(bool *set=0) const ;
	void unsetCreationDate();

	void setPriority(int) ;
	int getPriority(bool *set=0) const ;
	void unsetPriority();

	void setProcessorVersion(const std::string &) ;
	const std::string &getProcessorVersion(bool *set=0) const ;
	void unsetProcessorVersion();

	void setProcessorConfiguration(const std::string &) ;
	const std::string &getProcessorConfiguration(bool *set=0) const ;
	void unsetProcessorConfiguration();

	// replace the file of an existing record
	void replaceParametersFile(const std::string &path, dbConnection &) ;
	// save the file on disk
	void saveParametersFile(const std::string &path, const std::string &filename, dbConnection &) const ;
	// save the file on stream
	void saveParametersFile(std::ostream &, dbConnection &) const ;


	// This method return a hint for the filename
	std::string getParametersFileName() const ;
	// set/get the original file name
	void setParametersUploadFileName(const std::string &) ;
	std::string getParametersUploadFileName(bool *set=0) const;

	void setTroubleshooting(bool) ;
	bool getTroubleshooting(bool *set=0) const ;
	void unsetTroubleshooting();

	void setProcessingTimeWindow(int) ;
	int getProcessingTimeWindow(bool *set=0) const ;
	void unsetProcessingTimeWindow();

	void setProcessorId(const std::string &) ;
	const std::string &getProcessorId(bool *set=0) const ;
	void unsetProcessorId();

	void setSubReq(int) ;
	int getSubReq(bool *set=0) const ;
	void unsetSubReq();

	bool hasParameters() const ;
	
	static std::string getFields() ;
	static std::string getTable() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;


protected:
	dbOrder() ;
	dbOrder(const dbOrder &) ;


private:
	std::string _orderType ;
	bool _orderTypeSet ;

	std::string _status ;
	bool _statusSet ;

	std::string _processorName ;
	bool _processorNameSet ;

	unsigned int _satelliteId ;
	bool _satelliteIdSet ;

	int _mission ;
	bool _missionSet ;

	DateTime _startTime ;
	bool _startTimeSet ;

	DateTime _stopTime ;
	bool _stopTimeSet ;

	std::string _jobResponsible ;
	bool _jobResponsibleSet ;

	DateTime _productionStartTime ;
	bool _productionStartTimeSet ;

	DateTime _productionStopTime ;
	bool _productionStopTimeSet ;

	std::string _externalOrderId ;
	bool _externalOrderIdSet ;

	DateTime _creationDate ;
	bool _creationDateSet ;

	int _priority ;
	bool _prioritySet ;

	std::string _processorVersion ;
	bool _processorVersionSet ;

	std::string _processorConfiguration ;
	bool _processorConfigurationSet ;

	std::string _parametersUploadFileName ;
	bool _parametersUploadFileNameSet ;

	bool _troubleshooting ;
	bool _troubleshootingSet ;

	int _processingTimeWindow ;
	bool _processingTimeWindowSet ;

	std::string _processorId ;
	bool _processorIdSet ;

	int _subreq ;
	bool _subreqSet ;

	bool _parameters ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbOrder) ;
} ;

_ACS_END_NAMESPACE


#endif // _dbOrder_H_
