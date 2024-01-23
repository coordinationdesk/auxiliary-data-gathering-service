// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 7.1  2017/07/13 14:28:06  lucio.pulvirenti
	ECICMF-108: _invBaseline attribute and related methods added to manage baseline associated to Inventory.
	This concept requires a different schema (inventory triggere disabled) so that a compilation flag is used.
	
	Revision 7.0  2017/05/31 17:32:58  nicvac
	OPMAN-260: Need PDS DB schema PDS_12-04.
	
	Revision 6.1  2017/05/12 10:16:37  lucio.pulvirenti
	PRS-41: relation and relationType attributes added along with get and set methods.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.10  2016/07/04 14:16:15  damdec
	CSGACQ-82: getFields modified to be compatible with Oracle
	
	Revision 5.9  2016/04/29 16:00:43  marpas
	interface slightly changed - now adheres to POSIX
	
	Revision 5.8  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.7  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.6  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.5  2015/10/28 11:53:06  damdec
	PDSEV-16: attributes and classes related to blob usage removed.
	
	Revision 5.4  2015/03/13 17:51:19  marfav
	S2PDGS-1137 Added support to bulk extra parameters and quality value
	
	Revision 5.3  2015/02/26 16:50:00  marfav
	APF-256 baseline associated to TEC objects with external relation table
	
	Revision 5.2  2015/02/25 11:29:28  marfav
	APF-256 baseline metadata removed
	
 	Revision 5.1  2015/02/20 16:22:55  marfav
 	APF-256 introducing baseline metadata

	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:58  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.1  2013/09/23 18:50:16  marpas
	adopting libException 5.0 interfaces
	minor changes in method signatures
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.23  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.22  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.21  2013/02/12 12:38:53  marpas
	interfaces improved over qa rules
	
	Revision 2.20  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.18  2012/06/15 09:54:30  marpas
	removing warning related to virtual method hiding
	
	Revision 2.17  2012/05/11 08:27:38  marpas
	logical_id field added to t_inventory
	
	Revision 2.16  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.15  2011/09/08 12:00:35  marpas
	exists method added
	
	Revision 2.14  2011/04/20 08:12:00  enrcar
	EC:: Added methods: guessCacheName, guessCacheNameNoInvDate
	
	Revision 2.13  2011/04/05 17:02:23  marpas
	tree management in progreess,
	small bug fixed in update
	
	Revision 2.12  2011/03/30 16:59:32  marpas
	tree management added
	
	Revision 2.11  2011/03/25 14:11:50  marpas
	tree management in progress
	
	Revision 2.10  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.9  2010/04/23 14:04:15  marpas
	cloudiness percentage and product specification columns from t_inventory table managed
	
	Revision 2.8  2009/02/18 13:04:01  marpas
	dbstatistics upgrade, major readability of code - uses dbConv 2.5
	
	Revision 2.7  2009/02/18 12:59:30  marpas
	*** empty log message ***
	
	Revision 2.6  2008/11/12 18:13:34  marfav
	Including database BLOB when checking for async availability
	
	Revision 2.5  2008/11/05 12:40:26  marpas
	Band & Tdi added
	
	Revision 2.4  2008/09/09 15:37:52  marfav
	Added isSyncDownloadAvailable method
	
	Revision 2.3  2007/03/16 14:24:58  marpas
	work in progress
	
	Revision 2.2  2007/03/16 12:28:59  marpas
	work in progress
	
	Revision 2.1  2006/10/23 08:30:34  marpas
	dbInventoryObject interface fixed for sizes (they are now unsigned long long)
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.32  2005/10/06 08:36:19  marpas
	ifdef on MULTIARF removed (finally)
	Invalidate appears next right to Download (if feature is enabled)
	
	Revision 1.31  2005/09/23 17:45:52  marpas
	dbPersistent::showAllFieldsInList method used to populate result view
	
	Revision 1.30  2005/06/20 09:06:30  enrcar
	Added std::
	
	Revision 1.29  2005/06/17 16:07:42  enrcar
	include<std::string> by mistake - fixed
	
	Revision 1.28  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.27  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.26  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.25  2004/07/14 09:36:32  paoscu
	documentation and some fixes
	
	Revision 1.24  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.23  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.22  2004/06/01 14:39:56  paoscu
	stObj I/F completed
	
	Revision 1.21  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.20  2004/04/13 16:57:08  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.19  2004/02/13 16:24:34  paoscu
	Mission field added.
	
	Revision 1.18  2004/02/04 10:12:36  paoscu
	Library generated again. dbPersistent interface changed.
	
	Revision 1.17  2003/10/16 13:57:05  paoscu
	Methods className staticClassName fieldName added.
	
	Revision 1.16  2003/09/03 10:51:37  paoscu
	Library generated again: export methods are now const.
	
	Revision 1.15  2003/07/07 10:27:13  paoscu
	unset methods added.
	
	Revision 1.14  2003/05/26 20:06:25  paoscu
	minor changes
	
	Revision 1.13  2003/05/02 17:44:49  paoscu
	Constructors are now protected.
	
	Revision 1.12  2003/02/19 15:34:01  paoscu
	zeroBlob method
	Bugs fixed in update and save
	
	Revision 1.11  2003/02/04 14:16:57  paoscu
	aligned with DB mandatory fields and lengths + loadRow + output null if not set
	Controll after BLOB download.
	
	Revision 1.9  2003/01/27 15:33:15  paoscu
	"ProcessedOrderId" changed in "OrderId"
	
	Revision 1.8  2002/12/13 18:33:19  paoscu
	SavedGuard became PossessionGuard.
	
	Revision 1.7  2002/11/29 18:01:46  danalt
	added exportXML
	
	Revision 1.6  2002/11/27 11:29:29  paoscu
	Several changes for upload and download functions.
	
	Revision 1.5  2002/11/13 17:24:08  danalt
	aligned with dbPersistent
	
*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbInventoryObject_H_
#define _dbInventoryObject_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbStObj.h>
#include <DateTime.h>
#include <dbStObj.h>
#include <dbGeoObject.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class dbCryosatFactory;

class dbRow ;
class rsResourceSet ;
class XMLOstream ;

//
//
//
// class dbInventoryObject
//
//
//
class dbInventoryObject : // PRQA S 2109
	public dbStObj
{
public:
	
	friend class dbCryosatFactory; // PRQA S 2107
	
	/*! class BlobError declaration */
	exDECLARE_EXCEPTION(BlobError,exDbPersistent) ; // PRQA S 2131, 2153, 2502

	/*! class SyncDetectionError declaration */
	exDECLARE_EXCEPTION(SyncDetectionError,exDbPersistent) ; // PRQA S 2131, 2153, 2502
	//
	// Methods
	//

	virtual ~dbInventoryObject() throw() ;
	dbInventoryObject &operator=(const dbInventoryObject &) ; // operator =

	// inherited from dbPersistent
	virtual dbPersistent *clone() const ;   // PRQA S 2502
	virtual std::string className() const ;
	static  std::string staticClassName() ;
	virtual void reset() ; // initialize the class
    /*! returns true if the filename, fileversion exists on db */
    bool exists(dbConnection &) const ;
	virtual IdType save(dbConnection &)  ; // save the data in database
	virtual bool load(dbConnection & , IdType id=0)  ; // load the class from the database. Return true if obect exist
	virtual IdType update(dbConnection &)  ; // update an existing instance in database
	virtual IdType remove(dbConnection &)  ; // remove the class from the database // PRQA S 2502
	virtual bool validate() const  ; // True if the class have valid parameters, false otherwise
	virtual void exportObj(rsResourceSet &s) const ; // export the class to stream
	virtual void importObj(rsResourceSet &s)  ; // import the class from a stream
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
	virtual void exportXML(XMLOstream &) const ; // export the class to XML stream
#pragma GCC diagnostic pop

	int loadRow(dbConnection &conn, const dbRow &, int offset=0) ;

	void setFileName(const std::string &) ;
	const std::string &getFileName(bool *set=0) const ;
	void unsetFileName();

	void setFileVersion(const std::string &) ;
	const std::string &getFileVersion(bool *set=0) const ;
	void unsetFileVersion();

	void setSource(const std::string &) ;
	const std::string &getSource(bool *set=0) const ;
	void unsetSource();

	void setSourceSwVersion(const std::string &) ;
	const std::string &getSourceSwVersion(bool *set=0) const ;
	void unsetSourceSwVersion();

	void setGenerationTime(const DateTime &) ;
	const DateTime &getGenerationTime(bool *set=0) const ;
	void unsetGenerationTime();

	void setValidityStart(const DateTime &) ;
	const DateTime &getValidityStart(bool *set=0) const ;
	void unsetValidityStart();

	void setValidityStop(const DateTime &) ;
	const DateTime &getValidityStop(bool *set=0) const ;
	void unsetValidityStop();

	// inherited from dbStObj
	virtual const dbGeoObject *shape() const ;

protected:	
	dbInventoryObject() ;
	dbInventoryObject(const dbInventoryObject &) ;

private:
	unsigned int executeCountQuery (const std::string&, dbConnection &, const std::string &what="");	
    void saveInveRelation(dbConnection &conn) ;
public:
	
	void setStartOrbitNumber(int) ;
	int getStartOrbitNumber(bool *set=0) const ;
	void unsetStartOrbitNumber();

	void setStopOrbitNumber(int) ;
	int getStopOrbitNumber(bool *set=0) const ;
	void unsetStopOrbitNumber();

	void setGeographicLocalization(const dbGeoObject &) ;
	const dbGeoObject *getGeographicLocalization(bool *set=0) const ;
	void unsetGeographicLocalization();

	void setQualityInfo(const std::string &) ;
	const std::string &getQualityInfo(bool *set=0) const ;
	void unsetQualityInfo();

	void setValidityFlag(bool) ;
	bool getValidityFlag(bool *set=0) const ;
	void unsetValidityFlag();

	void setValidationDate(const DateTime &) ;
	const DateTime &getValidationDate(bool *set=0) const ;
	void unsetValidationDate();

	void setHeaderSize(off_t) ;
	off_t getHeaderSize(bool *set=0) const ;
	void unsetHeaderSize();

	void setDataSize(off_t) ;
	off_t  getDataSize(bool *set=0) const ;
	void unsetDataSize();

	void setInventoryDate(const DateTime &) ;
	const DateTime &getInventoryDate(bool *set=0) const ;
	void unsetInventoryDate();

	void setModificationDate(const DateTime &) ;
	const DateTime &getModificationDate(bool *set=0) const ;
	void unsetModificationDate();

	void setFileType(const std::string &) ;
	const std::string &getFileType(bool *set=0) const ;
	void unsetFileType();

	void setFileClass(const std::string &) ;
	const std::string &getFileClass(bool *set=0) const ;
	void unsetFileClass();

	void setOrderId(int) ;
	int getOrderId(bool *set=0) const ;
	void unsetOrderId();

	void setSensorId(int) ;
	int getSensorId(bool *set=0) const ;
	void unsetSensorId();

	void setAcquisitionStationId(int) ;
	int getAcquisitionStationId(bool *set=0) const ;
	void unsetAcquisitionStationId();

	void setProcessingStationId(int) ;
	int getProcessingStationId(bool *set=0) const ;
	void unsetProcessingStationId();

	void setSensorMode(const std::string &) ;
	const std::string &getSensorMode(bool *set=0) const ;
	void unsetSensorMode();

	void setPhase(const std::string &) ;
	const std::string &getPhase(bool *set=0) const ;
	void unsetPhase();

	void setSatelliteId(int) ;
	int getSatelliteId(bool *set=0) const ;
	void unsetSatelliteId();

	void setAscendingFlag(bool) ;
	bool getAscendingFlag(bool *set=0) const ;
	void unsetAscendingFlag();

	void setEquatorXLongitude(float) ;
	float getEquatorXLongitude(bool *set=0) const ;
	void unsetEquatorXLongitude();

	void setEquatorXTime(const DateTime &) ;
	const DateTime &getEquatorXTime(bool *set=0) const ;
	void unsetEquatorXTime();

	void setMission(int) ;
	int getMission(bool *set=0) const ;
	void unsetMission();

	void setQualified(int) ;
	int getQualified(bool *set=0) const ;

	void setOriginator(int) ;
	int getOriginator(bool *set=0) const ;
	void unsetOriginator();

	void setQualifierFile(const std::string &) ;
	const std::string & getQualifierFile(bool *set=0) const ;
	void unsetQualifierFile();

	void setBand(int) ;
	int getBand(bool *set=0) const ;
	void unsetBand();

	void setTdi(short) ;
	short getTdi(bool *set=0) const ;
	void unsetTdi();

	void setCloudPctg(float) ;
	float getCloudPctg(bool *set=0) const ;
	void unsetCloudPctg();

	void setProdSpec(const std::string &) ;
	const std::string & getProdSpec(bool *set=0) const ;
	void unsetProdSpec();

	void setRefDirId(IdType) ;
	IdType getRefDirId(bool *set=0) const ;
	void unsetRefDirId();

	void setDirId(IdType) ;
	IdType getDirId(bool *set=0) const ;
	void unsetDirId();

	void setUniqueId(const std::string &) ;
	const std::string &getUniqueId(bool *set=0) const ;
	void unsetUniqueId();

	void setLogicalId(const std::string &) ;
	const std::string &getLogicalId(bool *set=0) const ;
	void unsetLogicalId();

    void setParentId(const std::string &) ;
    const std::string &getParentId(bool *set=0) const ;
	void unsetParentId();
	
    void setGroupId(const std::string &) ;
	const std::string &getGroupId(bool *set=0) const ;
	void unsetGroupId();

    void setQualityValue(float) ;
	float getQualityValue(bool *set=0) const ;
	void unsetQualityValue();
	
	void setRelation(const std::string &);
	const std::string & getRelation(bool *set=0) const ;
	void unsetRelation();
	
	void setRelationType(const std::string &);
	const std::string & getRelationType(bool *set=0) const ;
	void unsetRelationType();

	void setMd5(const std::string &);
	const std::string & getMd5(bool *set=0) const ;
	void unsetMd5();

	std::string guessCacheName(bool* set) const ;
	std::string guessCacheNameNoInvDate(bool* set) const ;


	// checks if the file is available in a synchronous storage manager
	bool isSyncDownloadAvailable ();
	bool isSyncDownloadAvailable (dbConnection&);
    static bool hasSerialId(dbConnection&) ;
    static bool checkSchema() ;
    static bool checkSchema(dbConnection&) ;
    static bool hasBaseline(dbConnection&) ;
    static bool hasQualityValue (dbConnection&) ;
    static bool hasMd5 (dbConnection&) ;

	static std::string getFields() ;
	static std::string getFields(dbConnection&) ;
	//static std::deque<std::string> getFieldArray() ;
	static std::deque<std::string> getFieldArray(dbConnection&) ;

	static std::string convertFieldList (std::deque<std::string> const&);

	static std::string getTable() ;
	static std::string getIdColumnName() ;
	static std::string getOrderBy() ;

	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;
public:
	const static int defaultQualifiedId = 1 ;
private:
    static bool _schemaChecked ;
    static bool _hasSerialId ;
    static bool _hasBaseline ;
    static bool _hasQualityValue ;
	static bool _hasMd5;

	std::string _fileName ;
	bool _fileNameSet ;

	std::string _fileVersion ;
	bool _fileVersionSet ;

	std::string _source ;
	bool _sourceSet ;

	std::string _sourceSwVersion ;
	bool _sourceSwVersionSet ;

	DateTime _generationTime ;
	bool _generationTimeSet ;

	DateTime _validityStart ;
	bool _validityStartSet ;

	DateTime _validityStop ;
	bool _validityStopSet ;

	int _startOrbitNumber ;
	bool _startOrbitNumberSet ;

	int _stopOrbitNumber ;
	bool _stopOrbitNumberSet ;

	dbGeoObject *_geographicLocalization ;
	bool _geographicLocalizationSet ;

	std::string _qualityInfo ;
	bool _qualityInfoSet ;

	bool _validityFlag ;
	bool _validityFlagSet ;

	DateTime _validationDate ;
	bool _validationDateSet ;

	off_t _headerSize ;
	bool _headerSizeSet ;

	off_t _dataSize ;
	bool _dataSizeSet ;

	DateTime _inventoryDate ;
	bool _inventoryDateSet ;

	DateTime _modificationDate ;
	bool _modificationDateSet ;

	std::string _fileType ;
	bool _fileTypeSet ;

	std::string _fileClass ;
	bool _fileClassSet ;

	int _orderId ;
	bool _orderIdSet ;

	int _sensorId ;
	bool _sensorIdSet ;

	int _acquisitionStationId ;
	bool _acquisitionStationIdSet ;

	int _processingStationId ;
	bool _processingStationIdSet ;

	std::string _sensorMode ;
	bool _sensorModeSet ;

	std::string _phase ;
	bool _phaseSet ;

	int _satelliteId ;
	bool _satelliteIdSet ;

	bool _ascendingFlag ;
	bool _ascendingFlagSet ;

	float _equatorXLongitude ;
	bool _equatorXLongitudeSet ;

	DateTime _equatorXTime ;
	bool _equatorXTimeSet ;

	int _mission ;
	bool _missionSet ;

	int _qualifiedId ;

	int _originatorId ;
	bool _originatorIdSet ;

	std::string _qualifierFile ;
	bool _qualifierFileSet ;

	int _band ;
	bool _bandSet ;

	short _tdi ;
	bool _tdiSet ;

	float _cloudpctg ;
	bool _cloudpctgSet ;

	std::string _prodspec ;
	bool _prodspecSet ;

	bool _RefDirIdSet ; 
	IdType _RefDirId ; 

	bool _DirIdSet ; 
	IdType _DirId ;	

	std::string _uniqueId ;
	bool _uniqueIdSet ;

	std::string _logicalId ;
	bool _logicalIdSet ;
	
    std::string _parentId ;
	bool _parentIdSet ;

	std::string _groupId ;
	bool _groupIdSet ;

	float _qualityValue ;
	bool _qualityValueSet ;


	bool _syncDownloadAvailable;
	bool _syncDownloadAvailableSet;
	
	std::string _relation;
	bool _relationSet;
	
	std::string _relationType;
	bool _relationTypeSet;

	std::string _md5;
	bool _md5Set;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObject);
} ;


_ACS_END_NAMESPACE


#endif // _dbInventoryObject_H_
