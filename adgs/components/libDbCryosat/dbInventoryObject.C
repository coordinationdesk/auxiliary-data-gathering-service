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
	Revision 7.3  2017/11/16 09:59:57  marpas
	quality_info is now 72 char long
	deprecated code removed
	
	Revision 7.2  2017/09/04 09:19:36  lucio.pulvirenti
	ECICMF: _invBaseline attribute no more managed since useless.
	
	Revision 7.1  2017/07/13 14:28:06  lucio.pulvirenti
	ECICMF-108: _invBaseline attribute and related methods added to manage baseline associated to Inventory.
	This concept requires a different schema (inventory triggere disabled) so that a compilation flag is used.
	
	Revision 7.0  2017/05/31 17:32:58  nicvac
	OPMAN-260: Need PDS DB schema PDS_12-04.
	
	Revision 6.2  2017/05/12 10:32:43  lucio.pulvirenti
	PRS-41: relation and relationType attributes added along with get and set methods. saveInveRelation: t_inv_relations rel_type_id valorized with relation value
	and t_inv_relations relation with relationType. inv_id2 valorized if CHILD_TO_PARENT only.
	
	Revision 6.1  2016/10/07 11:24:29  marpas
	fixing useless include file
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.15  2016/07/04 14:14:51  damdec
	CSGACQ-82: getFields modified to be compatible with Oracle
	
	Revision 5.14  2016/05/31 09:40:56  lucio.pulvirenti
	CSGACQ-79: useless code removed: p_deleteinventory makes nothing (just return 0). Not implemented in Oracle.
	
	Revision 5.13  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.12  2016/04/29 16:00:43  marpas
	interface slightly changed - now adheres to POSIX
	
	Revision 5.11  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.10  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.9  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.8  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.7  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.6  2015/10/28 11:53:06  damdec
	PDSEV-16: attributes and classes related to blob usage removed.
	
	Revision 5.5  2015/10/27 15:52:40  damdec
	PDSEV-16: blob handling removed.
	
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
	
	Revision 3.4  2013/09/26 19:32:51  marpas
	qa rules
	coding best practices
	compilation warnings removed
	using libException 5.x interface
	
	Revision 3.3  2013/09/24 17:39:54  marpas
	load parentid fixed
	
	Revision 3.2  2013/09/23 18:51:05  marpas
	adopting libException 5.x interface
	coding best practices applied
	qa warnings
	other internal optimizations
	
	Revision 3.1  2013/09/20 13:28:44  marfav
	S2PDGS-465 t_inventorysm references t_inventory via inventory id
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.51  2013/06/03 11:51:39  marpas
	misalignement fixed
	
	Revision 2.50  2013/05/14 19:06:00  marpas
	PostGRES/PostGIS support
	SQLDialectFactory::sqlDialect() now requirs db connection key
	dbSchemaMetadataProvider now requires db connection key
	
	Revision 2.49  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.48  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.47  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.46  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.45  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.44  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.42  2012/05/11 08:27:38  marpas
	logical_id field added to t_inventory
	
	Revision 2.41  2012/03/26 12:46:57  marfav
	Size constraints on string attributes are applied without throwing exceptions
	
	Revision 2.40  2012/03/12 11:23:45  marpas
	aligning to new schema
	
	Revision 2.39  2012/02/15 16:16:34  marpas
	remove fixed
	
	Revision 2.38  2012/02/15 16:13:17  marpas
	bug fixing in load()
	
	Revision 2.37  2012/02/15 13:28:11  chicas
	uniqueid field name correct in unique_id
	
	Revision 2.36  2012/02/14 13:37:11  marpas
	fixing id problems
	
	Revision 2.35  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.34  2011/09/08 12:13:43  marpas
	debug improved, bug fixed on exists
	
	Revision 2.33  2011/09/08 12:00:35  marpas
	exists method added
	
	Revision 2.32  2011/04/20 08:12:00  enrcar
	EC:: Added methods: guessCacheName, guessCacheNameNoInvDate
	
	Revision 2.31  2011/04/05 17:02:23  marpas
	tree management in progreess,
	small bug fixed in update
	
	Revision 2.30  2011/03/30 16:59:31  marpas
	tree management added
	
	Revision 2.29  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.28  2011/01/27 15:16:55  marpas
	fileversion is now enlarged to 32 char
	
	Revision 2.27  2010/08/26 13:23:17  marpas
	ORACLE function call fixed
	
	Revision 2.26  2010/08/26 09:23:28  marpas
	Oracle procedure is now in schema: called
	
	Revision 2.25  2010/08/10 10:08:11  marpas
	Oracle11 patch - no p_deleteinventory proceudre on the schema up to now: skipped
	
	Revision 2.24  2010/08/10 09:55:33  marpas
	Oracle11 support fixed
	
	Revision 2.23  2010/04/23 15:24:28  marpas
	reset bug fixed
	export bugs fixed
	
	Revision 2.22  2010/04/23 14:04:15  marpas
	cloudiness percentage and product specification columns from t_inventory table managed
	
	Revision 2.21  2010/03/09 10:19:08  enrcar
	EC:: Oracle DB handled
	
	Revision 2.20  2010/03/05 15:09:10  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.19  2009/02/18 13:04:00  marpas
	dbstatistics upgrade, major readability of code - uses dbConv 2.5
	
	Revision 2.18  2009/02/18 12:59:30  marpas
	*** empty log message ***
	
	Revision 2.17  2008/11/21 18:03:22  marpas
	p_deleteinventory invocation added
	
	Revision 2.16  2008/11/17 15:30:07  marpas
	currentDateTime used to be db independent
	
	Revision 2.15  2008/11/12 18:13:34  marfav
	Including database BLOB when checking for async availability
	
	Revision 2.14  2008/11/12 11:49:19  marfav
	Using dbConv to know the right type for db booleans
	
	Revision 2.13  2008/11/05 12:40:26  marpas
	Band & Tdi added
	
	Revision 2.12  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.11  2008/09/09 15:37:52  marfav
	Added isSyncDownloadAvailable method
	
	Revision 2.10  2008/07/14 13:17:48  marpas
	getting rid of db_values
	
	Revision 2.9  2007/03/16 14:24:58  marpas
	work in progress
	
	Revision 2.8  2007/03/16 12:28:59  marpas
	work in progress
	
	Revision 2.7  2006/10/23 08:30:34  marpas
	dbInventoryObject interface fixed for sizes (they are now unsigned long long)
	
	Revision 2.6  2006/07/19 16:53:03  marpas
	operator= fixed
	
	Revision 2.5  2006/06/09 12:45:58  giucas
	Fixed syntax error in update query.
	
	Revision 2.4  2006/06/09 11:53:21  giucas
	Update method fixed.
	
	Revision 2.3  2006/04/28 13:22:47  marpas
	transaction added to take care of two-phase blob management
	
	Revision 2.2  2006/03/23 16:22:45  marpas
	sensorMode is now 16 char long
	
	Revision 2.1  2006/03/08 13:30:42  marpas
	modification time management is left to the database during update
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.64  2006/01/20 10:19:55  enrcar
	EC:: define FORCE_DBEXPRESS_QUERY introduced to allow safer tests on dbexpress-ported queries
	
	Revision 1.63  2006/01/13 13:50:04  enrcar
	updated to dbexpress
	
	
	Revision 1.62  2005/11/04 14:17:33  marpas
	new method to load dbGeoObject from dbRow
	
	Revision 1.61  2005/10/24 16:44:01  marpas
	dbexpress compatibility
	
	Revision 1.60  2005/10/21 18:32:13  marpas
	DBEXPRESS compatibility
	
	Revision 1.59  2005/10/11 08:42:16  enrcar
	SQLString compatibility added
	
	Revision 1.58  2005/10/06 08:36:19  marpas
	ifdef on MULTIARF removed (finally)
	Invalidate appears next right to Download (if feature is enabled)
	
	Revision 1.57  2005/10/04 15:52:14  marpas
	compilation error fixed
	
	Revision 1.56  2005/10/04 15:51:33  marpas
	zeroBlob method did set to 0 the database columns, but left unchanged current dbInventoryObject values, fixed
	
	Revision 1.55  2005/10/04 14:17:34  marfav
	Invalidate hyperlink shown in details when feature InventoryObjectHasInvalidateAction enabled
	
	Revision 1.54  2005/09/23 17:45:52  marpas
	dbPersistent::showAllFieldsInList method used to populate result view
	
	Revision 1.53  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.52  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.51  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.50  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.49  2004/09/17 17:07:29  paoscu
	tabular representation improved
	
	Revision 1.48  2004/09/17 15:08:56  paoscu
	no check on source length for dbInventoryObject, too long entries will be unforgivingly truncated
	
	Revision 1.47  2004/09/17 12:49:59  paoscu
	tabular representation improved
	
	Revision 1.46  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.45  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.44  2004/06/01 14:39:56  paoscu
	stObj I/F completed
	
	Revision 1.43  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.42  2004/04/16 12:49:34  paoscu
	Bug fixed in Lock usage
	
	Revision 1.41  2004/04/16 07:58:02  paoscu
	NameIdAssociator no longer used explicitely
	
	Revision 1.40  2004/04/14 15:37:10  paoscu
	NameIdAssociator is a template
	
	Revision 1.39  2004/04/13 16:49:16  paoscu
	SatelliteName.h , SensorName.h and StationName.h included
	clone() method added
	ThreadSafe::Lock used
	
	Revision 1.38  2004/03/03 15:31:37  paoscu
	FileClass lenght = 128
	
	Revision 1.37  2004/02/16 11:21:44  paoscu
	Bug removed in getFields() method.
	
	Revision 1.36  2004/02/13 16:24:34  paoscu
	Mission field added.
	
	Revision 1.35  2004/02/05 16:46:45  paoscu
	segmentation fault removed.
	
	Revision 1.34  2004/02/04 15:00:23  paoscu
	Memory leak removed.
	
	Revision 1.33  2004/02/04 10:12:35  paoscu
	Library generated again. dbPersistent interface changed.
	
	Revision 1.32  2003/11/07 13:01:34  paoscu
	loadRow now uses row::columnSet
	
	Revision 1.31  2003/10/16 13:57:05  paoscu
	Methods className staticClassName f+ count++,e added.
	
	Revision 1.30  2003/09/04 15:39:22  paoscu
	filename field length is now 128.
	
	Revision 1.29  2003/09/03 10:51:37  paoscu
	Library generated again: export methods are now const.
	
	Revision 1.28  2003/07/07 15:58:52  paoscu
	dbPersistent::exportXML added in exportXML method.
	
	Revision 1.27  2003/07/07 09:45:54  paoscu
	Unset methods added.
	New checks on XXXset fields.
	During update unset fields are saved as NULL.
	dbPersistent import and export.
	
	Revision 1.26  2003/06/04 17:39:56  paoscu
	Download command inserted in html.
	
	Revision 1.25  2003/05/26 20:06:25  paoscu
	minor changes
	
	Revision 1.24  2003/05/21 17:03:43  paoscu
	sortableDateString replaced by sortableDateTimeString
	
	Revision 1.23  2003/05/19 15:20:22  paoscu
	Datetime::utcs replaced by Datetime::sortableDateString in getRecordValues method.
	
	Revision 1.22  2003/05/06 11:13:49  paoscu
	Changes in methods getHtmlDescription(HtmlRow &row) and getHtmlTableHeader(HtmlRow &row)
	
	Revision 1.21  2003/04/29 07:22:45  andcol
	Changed
	
	- GetHtmldescription
	- GetRecordFields
	
	in order to fit the HyBro requirements
	
	Revision 1.20  2003/04/16 10:11:52  paoscu
	Fixed a bug for _geographicLocalization
	
	Revision 1.19  2003/03/25 16:32:29  paoscu
	Error in header
	
	Revision 1.18  2003/03/25 14:41:59  paoscu
	inventory date e modification date are not anymore mandatory fields.
	
	Revision 1.17  2003/02/26 18:01:04  paoscu
	Memory leak fixed in setGeographicLocalization
	
	Revision 1.16  2003/02/19 15:34:01  paoscu
	zeroBlob method
	Bugs fixed in update and save
	
	Revision 1.15  2003/02/04 14:16:57  paoscu
	aligned with DB mandatory fields and lengths + loadRow + output null if not set
	Controll after BLOB download.
	
	Revision 1.13  2003/01/27 15:33:15  paoscu
	"ProcessedOrderId" changed in "OrderId"
	
	Revision 1.12  2002/12/16 11:32:13  paoscu
	Load method modified.
	
	Revision 1.11  2002/12/13 18:33:19  paoscu
	SavedGuard became PossessionGuard.
	
	Revision 1.10  2002/11/29 18:02:34  danalt
	added exportXML
	
	Revision 1.9  2002/11/27 11:29:29  paoscu
	Several changes for upload and download functions.
	
	Revision 1.8  2002/11/13 12:59:17  danalt
	ACE Token added
*/

#include <dbInventoryObject.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <Transaction.h>
#include <dbSchemaMetaDataProvider.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ResourceSpace.h>
#include <File.h>
#include <unistd.h>
#include <sstream>
#include <XMLOstream.h>
#include <SQLString.h>
#include <SatelliteName.h>
#include <SensorName.h>
#include <StationName.h>
#include <InvRelationsRules.h>

#include <BandName.h>
#include <Filterables.h>

#include <boost/format.hpp> // PRQA S 1013




_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


//
//
//
// class dbInventoryObject
//
//
//


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObject) 

bool dbInventoryObject::_schemaChecked = false ;
bool dbInventoryObject::_hasSerialId = false ;
bool dbInventoryObject::_hasBaseline = false ;
bool dbInventoryObject::_hasQualityValue = false ;
bool dbInventoryObject::_hasMd5 = false ;


// constructor
dbInventoryObject::dbInventoryObject() :
	dbStObj(),
	_fileName(),
	_fileNameSet(),
	_fileVersion(),
	_fileVersionSet(),
	_source(),
	_sourceSet(),
	_sourceSwVersion(),
	_sourceSwVersionSet(),
	_generationTime(),
	_generationTimeSet(),
	_validityStart(),
	_validityStartSet(),
	_validityStop(),
	_validityStopSet(),
	_startOrbitNumber(),
	_startOrbitNumberSet(),
	_stopOrbitNumber(),
	_stopOrbitNumberSet(),
	_geographicLocalization(0),
	_geographicLocalizationSet(),
	_qualityInfo(),
	_qualityInfoSet(),
	_validityFlag(),
	_validityFlagSet(),
	_validationDate(),
	_validationDateSet(),
	_headerSize(),
	_headerSizeSet(),
	_dataSize(),
	_dataSizeSet(),
	_inventoryDate(),
	_inventoryDateSet(),
	_modificationDate(),
	_modificationDateSet(),
	_fileType(),
	_fileTypeSet(),
	_fileClass(),
	_fileClassSet(),
	_orderId(),
	_orderIdSet(),
	_sensorId(),
	_sensorIdSet(),
	_acquisitionStationId(),
	_acquisitionStationIdSet(),
	_processingStationId(),
	_processingStationIdSet(),
	_sensorMode(),
	_sensorModeSet(),
	_phase(),
	_phaseSet(),
	_satelliteId(),
	_satelliteIdSet(),
	_ascendingFlag(),
	_ascendingFlagSet(),
	_equatorXLongitude(),
	_equatorXLongitudeSet(),
	_equatorXTime(),
	_equatorXTimeSet(),
	_mission(),
	_missionSet(),
	_qualifiedId(),
	_originatorId(),
	_originatorIdSet(),
	_qualifierFile(),
	_qualifierFileSet(),
	_band(),
	_bandSet(),
	_tdi(),
	_tdiSet(),
	_cloudpctg(),
	_cloudpctgSet(),
	_prodspec(),
	_prodspecSet(),
	_RefDirIdSet(false),
    _RefDirId(),
	_DirIdSet(false),
    _DirId(),
	_uniqueId(),
	_uniqueIdSet(),
	_logicalId(),
	_logicalIdSet(),
    _parentId(),
	_parentIdSet(),
	_groupId(),
	_groupIdSet(),
    _qualityValue(),
    _qualityValueSet(),
	_syncDownloadAvailable(),
	_syncDownloadAvailableSet(),
	_relation(),
	_relationSet(),
	_relationType(),
	_relationTypeSet(),
	_md5(),
	_md5Set(false)
{
	dbInventoryObject::reset() ;
}


// copy constructor
dbInventoryObject::dbInventoryObject(const dbInventoryObject &o):
	dbStObj(o),
	_fileName(o._fileName),
	_fileNameSet(o._fileNameSet),
	_fileVersion(o._fileVersion),
	_fileVersionSet(o._fileVersionSet),
	_source(o._source),
	_sourceSet(o._sourceSet),
	_sourceSwVersion(o._sourceSwVersion),
	_sourceSwVersionSet(o._sourceSwVersionSet),
	_generationTime(o._generationTime),
	_generationTimeSet(o._generationTimeSet),
	_validityStart(o._validityStart),
	_validityStartSet(o._validityStartSet),
	_validityStop(o._validityStop),
	_validityStopSet(o._validityStopSet),
	_startOrbitNumber(o._startOrbitNumber),
	_startOrbitNumberSet(o._startOrbitNumberSet),
	_stopOrbitNumber(o._stopOrbitNumber),
	_stopOrbitNumberSet(o._stopOrbitNumberSet),
	_geographicLocalization((o._geographicLocalization ? o._geographicLocalization->clone() : 0)), // PRQA S 3380
	_geographicLocalizationSet(o._geographicLocalizationSet),
	_qualityInfo(o._qualityInfo),
	_qualityInfoSet(o._qualityInfoSet),
	_validityFlag(o._validityFlag),
	_validityFlagSet(o._validityFlagSet),
	_validationDate(o._validationDate),
	_validationDateSet(o._validationDateSet),
	_headerSize(o._headerSize),
	_headerSizeSet(o._headerSizeSet),
	_dataSize(o._dataSize),
	_dataSizeSet(o._dataSizeSet),
	_inventoryDate(o._inventoryDate),
	_inventoryDateSet(o._inventoryDateSet),
	_modificationDate(o._modificationDate),
	_modificationDateSet(o._modificationDateSet),
	_fileType(o._fileType),
	_fileTypeSet(o._fileTypeSet),
	_fileClass(o._fileClass),
	_fileClassSet(o._fileClassSet),
	_orderId(o._orderId),
	_orderIdSet(o._orderIdSet),
	_sensorId(o._sensorId),
	_sensorIdSet(o._sensorIdSet),
	_acquisitionStationId(o._acquisitionStationId),
	_acquisitionStationIdSet(o._acquisitionStationIdSet),
	_processingStationId(o._processingStationId),
	_processingStationIdSet(o._processingStationIdSet),
	_sensorMode(o._sensorMode),
	_sensorModeSet(o._sensorModeSet),
	_phase(o._phase),
	_phaseSet(o._phaseSet),
	_satelliteId(o._satelliteId),
	_satelliteIdSet(o._satelliteIdSet),
	_ascendingFlag(o._ascendingFlag),
	_ascendingFlagSet(o._ascendingFlagSet),
	_equatorXLongitude(o._equatorXLongitude),
	_equatorXLongitudeSet(o._equatorXLongitudeSet),
	_equatorXTime(o._equatorXTime),
	_equatorXTimeSet(o._equatorXTimeSet),
	_mission(o._mission),
	_missionSet(o._missionSet),
	_qualifiedId(o._qualifiedId),
	_originatorId(o._originatorId),
	_originatorIdSet(o._originatorIdSet),
	_qualifierFile(o._qualifierFile),
	_qualifierFileSet(o._qualifierFileSet),
	_band(o._band),
	_bandSet(o._bandSet),
	_tdi(o._tdi),
	_tdiSet(o._tdiSet),
	_cloudpctg(o._cloudpctg),
	_cloudpctgSet(o._cloudpctgSet),
	_prodspec(o._prodspec),
	_prodspecSet(o._prodspecSet),
	_RefDirIdSet(o._RefDirIdSet),
    _RefDirId(o._RefDirId),
	_DirIdSet(o._DirIdSet),
    _DirId(o._DirId),
	_uniqueId(o._uniqueId),
	_uniqueIdSet(o._uniqueIdSet),
	_logicalId(o._logicalId),
	_logicalIdSet(o._logicalIdSet),	
    _parentId(o._parentId),
	_parentIdSet(o._parentIdSet),
	_groupId(o._groupId),
	_groupIdSet(o._groupIdSet),
    _qualityValue(o._qualityValue),
    _qualityValueSet(o._qualityValueSet),
	_syncDownloadAvailable(o._syncDownloadAvailable),
	_syncDownloadAvailableSet(o._syncDownloadAvailableSet),
	_relation(o._relation),
	_relationSet(o._relationSet),
	_relationType(o._relationType),
	_relationTypeSet(o._relationTypeSet),
	_md5(o._md5),
	_md5Set(o._md5Set)
{
}


dbInventoryObject::~dbInventoryObject() throw() 
{
	delete _geographicLocalization ;
}




dbPersistent *dbInventoryObject::clone() const
{
	return new dbInventoryObject(*this) ;
}



// operator =
dbInventoryObject &dbInventoryObject::operator=(const dbInventoryObject &o)
{
	Lock lock(*this) ; // PRQA S 3050
	Lock lockr(o) ; // PRQA S 3050

	if(this != &o)
	{
		dbStObj::operator=(o) ;
		_fileName=o._fileName;
		_fileNameSet=o._fileNameSet;
		_fileVersion=o._fileVersion;
		_fileVersionSet=o._fileVersionSet;
		_source=o._source;
		_sourceSet=o._sourceSet;
		_sourceSwVersion=o._sourceSwVersion;
		_sourceSwVersionSet=o._sourceSwVersionSet;
		_generationTime=o._generationTime;
		_generationTimeSet=o._generationTimeSet;
		_validityStart=o._validityStart;
		_validityStartSet=o._validityStartSet;
		_validityStop=o._validityStop;
		_validityStopSet=o._validityStopSet;
		_startOrbitNumber=o._startOrbitNumber;
		_startOrbitNumberSet=o._startOrbitNumberSet;
		_stopOrbitNumber=o._stopOrbitNumber;
		_stopOrbitNumberSet=o._stopOrbitNumberSet;
		delete _geographicLocalization ;
		_geographicLocalization=(o._geographicLocalization ? o._geographicLocalization->clone() : 0); // PRQA S 3380
		_geographicLocalizationSet=o._geographicLocalizationSet;
		_qualityInfo=o._qualityInfo;
		_qualityInfoSet=o._qualityInfoSet;
		_validityFlag=o._validityFlag;
		_validityFlagSet=o._validityFlagSet;
		_validationDate=o._validationDate;
		_validationDateSet=o._validationDateSet;
		_headerSize=o._headerSize;
		_headerSizeSet=o._headerSizeSet;
		_dataSize=o._dataSize;
		_dataSizeSet=o._dataSizeSet;
		_inventoryDate=o._inventoryDate;
		_inventoryDateSet=o._inventoryDateSet;
		_modificationDate=o._modificationDate;
		_modificationDateSet=o._modificationDateSet;
		_fileType=o._fileType;
		_fileTypeSet=o._fileTypeSet;
		_fileClass=o._fileClass;
		_fileClassSet=o._fileClassSet;
		_orderId=o._orderId;
		_orderIdSet=o._orderIdSet;
		_sensorId=o._sensorId;
		_sensorIdSet=o._sensorIdSet;
		_acquisitionStationId=o._acquisitionStationId;
		_acquisitionStationIdSet=o._acquisitionStationIdSet;
		_processingStationId=o._processingStationId;
		_processingStationIdSet=o._processingStationIdSet;
		_sensorMode=o._sensorMode;
		_sensorModeSet=o._sensorModeSet;
		_phase=o._phase;
		_phaseSet=o._phaseSet;
		_satelliteId=o._satelliteId;
		_satelliteIdSet=o._satelliteIdSet;
		_ascendingFlag=o._ascendingFlag;
		_ascendingFlagSet=o._ascendingFlagSet;
		_equatorXLongitude=o._equatorXLongitude;
		_equatorXLongitudeSet=o._equatorXLongitudeSet;
		_equatorXTime=o._equatorXTime;
		_equatorXTimeSet=o._equatorXTimeSet;
		_mission=o._mission;
		_missionSet=o._missionSet;
		_qualifiedId=o._qualifiedId ;
		_originatorId=o._originatorId ;
		_originatorIdSet=o._originatorIdSet ;
		_qualifierFile=o._qualifierFile ;
		_qualifierFileSet=o._qualifierFileSet ;
		_band=o._band ;
		_bandSet=o._bandSet ;
		_tdi=o._tdi ;
		_tdiSet=o._tdiSet ;
		_cloudpctg=o._cloudpctg ;
		_cloudpctgSet=o._cloudpctgSet ;
		_prodspec=o._prodspec ;
		_prodspecSet=o._prodspecSet ;
 		_DirIdSet=o._DirIdSet;
		_DirId=o._DirId ;
 		_RefDirIdSet=o._RefDirIdSet;
		_RefDirId=o._RefDirId ;
	    _uniqueId = o._uniqueId ;
	    _uniqueIdSet = o._uniqueIdSet ; 
	    _logicalId = o._logicalId ;
	    _logicalIdSet = o._logicalIdSet ; 
        _parentId = o._parentId;
	    _parentIdSet = o._parentIdSet;
	    _groupId = o._groupId;
	    _groupIdSet = o._groupIdSet;
        _qualityValue = o._qualityValue;
        _qualityValueSet = o._qualityValueSet;
		_syncDownloadAvailable=o._syncDownloadAvailable;
		_syncDownloadAvailableSet=o._syncDownloadAvailableSet;
		_relation=o._relation;
		_relationSet=o._relationSet;
		_relationType=o._relationType;
		_relationTypeSet=o._relationTypeSet;
		_md5=o._md5;
		_md5Set=o._md5Set;
		// _possessionFlag is not altered
	}
	return *this ;
}


// initialize the class
void dbInventoryObject::reset()
{
	Lock lock(*this) ; // PRQA S 3050

	// reset dbPersistent
	dbPersistent::reset() ;
	_fileName="" ;
	_fileNameSet=false ;
	_fileVersion="" ;
	_fileVersionSet=false ;
	_source="" ;
	_sourceSet=false ;
	_sourceSwVersion="" ;
	_sourceSwVersionSet=false ;
	_generationTime.reset() ;
	_generationTimeSet=false ;
	_validityStart.reset() ;
	_validityStartSet=false ;
	_validityStop.reset() ;
	_validityStopSet=false ;
	_startOrbitNumber=0 ;
	_startOrbitNumberSet=false ;
	_stopOrbitNumber=0 ;
	_stopOrbitNumberSet=false ;
	delete _geographicLocalization ;
	_geographicLocalization=0 ;
	_geographicLocalizationSet=false ;
	_qualityInfo="" ;
	_qualityInfoSet=false ;
	_validityFlag=false ;
	_validityFlagSet=false ;
	_validationDate.reset() ;
	_validationDateSet=false ;
	_headerSize=0 ;
	_headerSizeSet=false ;
	_dataSize=0 ;
	_dataSizeSet=false ;
	_inventoryDate.reset() ;
	_inventoryDateSet=false ;
	_modificationDate.reset() ;
	_modificationDateSet=false ;
	_fileType="" ;
	_fileTypeSet=false ;
	_fileClass="" ;
	_fileClassSet=false ;
	_orderId=0 ;
	_orderIdSet=false ;
	_sensorId=0 ;
	_sensorIdSet=false ;
	_acquisitionStationId=0 ;
	_acquisitionStationIdSet=false ;
	_processingStationId=0 ;
	_processingStationIdSet=false ;
	_sensorMode="" ;
	_sensorModeSet=false ;
	_phase="" ;
	_phaseSet=false ;
	_satelliteId=0 ;
	_satelliteIdSet=false ;
	_ascendingFlag=false ;
	_ascendingFlagSet=false ;
	_equatorXLongitude=0.0 ;
	_equatorXLongitudeSet=false ;
	_equatorXTime.reset() ;
	_equatorXTimeSet=false ;
	_mission=0 ;
	_missionSet=false ;
	_qualifiedId=1 ;
	_originatorId=0 ;
	_originatorIdSet=false ;
	_qualifierFile="" ;
	_qualifierFileSet=false ;
	_band=0 ;
	_bandSet=false ;
	_tdi=0 ;
	_tdiSet=false ;
	_cloudpctg=0 ;
	_cloudpctgSet=false ;
	_prodspec="" ;
	_prodspecSet=false;
	_DirIdSet=false;
	_RefDirIdSet=false;
    _uniqueId = "";
    _uniqueIdSet =false;
    _logicalId = "";
    _logicalIdSet =false;  
    _parentId = "";
    _parentIdSet =false;
    _groupId = "";
    _groupIdSet =false;
    _qualityValue=0.0;
    _qualityValueSet=false;
	_syncDownloadAvailable=false;
	_syncDownloadAvailableSet=false;
	_relation = "";
	_relationSet = false;
	_relationType = "";
	_relationTypeSet = false;
	_md5 = "";
	_md5Set = false;
	// _possessionFlag is not altered
}



string dbInventoryObject::className() const
{
	return staticClassName() ;
}


string dbInventoryObject::staticClassName()
{
	return "dbInventoryObject" ;
}


// save the data in database
dbPersistent::IdType dbInventoryObject::save(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050
	
	if (!isModified()) {
		return 0 ; 
    }
	
	//Check Mandatory fields 
    // PRQA S 3081 L1
	ACS_COND_THROW(isFromDatabase(),InvalidUse("Error: dbInventoryObject::save(). Use the method update() instead of save().")) ; 

	ACS_COND_THROW(!_fileNameSet,InvalidUse("Mandatory field \"File Name\" must be set.")) ;
	ACS_COND_THROW(!_fileVersionSet,InvalidUse("Mandatory field \"File Version\" must be set.")) ;
	ACS_COND_THROW(!_generationTimeSet,InvalidUse("Mandatory field \"Generation Time\" must be set.")) ;
	ACS_COND_THROW(!_validityStartSet,InvalidUse("Mandatory field \"Validity Start\" must be set.")) ;
	ACS_COND_THROW(!_validityStopSet,InvalidUse("Mandatory field \"Validity Stop\" must be set.")) ;
	ACS_COND_THROW(!_validityFlagSet,InvalidUse("Mandatory field \"Validity Flag\" must be set.")) ;
	ACS_COND_THROW(!_validationDateSet,InvalidUse("Mandatory field \"Validation Date\" must be set.")) ;
	ACS_COND_THROW(!_fileTypeSet,InvalidUse("Mandatory field \"File Type\" must be set.")) ;
	ACS_COND_THROW(!_fileClassSet,InvalidUse("Mandatory field \"File Class\" must be set.")) ;
    // PRQA L:L1
    
    const string &dbKey = conn.getKey() ;

	SQLString queryString(dbKey, SQLString::INSERT) ;

	dbQuery query(conn) ;

    queryString.addTableList(getTable()) ;
	queryString.addValuesList("FileName", db::toSQL(dbKey, _fileName)) ;
	queryString.addValuesList("FileVersion", db::toSQL(dbKey, _fileVersion)) ;
	if(_sourceSet) 			{ queryString.addValuesList("Source", db::toSQL(dbKey, _source)) ; }
	if(_sourceSwVersionSet)	{ queryString.addValuesList("SourceSwVersion", db::toSQL(dbKey, _sourceSwVersion)) ; }
	queryString.addValuesList("GenerationTime", db::toSQL(dbKey, _generationTime,dbConv::YearToFraction)) ;
	queryString.addValuesList("ValidityStart", db::toSQL(dbKey, _validityStart,dbConv::YearToFraction)) ;
	queryString.addValuesList("ValidityStop", db::toSQL(dbKey, _validityStop,dbConv::YearToFraction)) ;
	if(_startOrbitNumberSet) { queryString.addValuesList("StartOrbitNumber", db::toSQL(dbKey, _startOrbitNumber)) ; }
	if(_stopOrbitNumberSet)  { queryString.addValuesList("StopOrbitNumber", db::toSQL(dbKey, _stopOrbitNumber)) ; }
	if(_geographicLocalizationSet) { queryString.addValuesList("GeographicLocalization", db::toSQL(dbKey, *_geographicLocalization)) ; }
	if(_qualityInfoSet)	{ queryString.addValuesList("QualityInfo", db::toSQL(dbKey, _qualityInfo)) ; }
	queryString.addValuesList("ValidityFlag", db::toSQL(dbKey, _validityFlag)) ;
	queryString.addValuesList("ValidationDate", db::toSQL(dbKey, _validationDate,dbConv::YearToFraction)) ;
    if(_headerSizeSet) { queryString.addValuesList("HeaderSize", db::toSQL(dbKey, _headerSize)) ; }
	if(_dataSizeSet) { queryString.addValuesList("DataSize", db::toSQL(dbKey, _dataSize)) ; }
	if(_inventoryDateSet) { queryString.addValuesList("InventoryDate", db::toSQL(dbKey, _inventoryDate,dbConv::YearToFraction)) ; }
	if(_modificationDateSet) { queryString.addValuesList("ModificationDate", db::toSQL(dbKey, _modificationDate,dbConv::YearToFraction)) ; }
	queryString.addValuesList("FileType", db::toSQL(dbKey, _fileType)) ;
	queryString.addValuesList("FileClass", db::toSQL(dbKey, _fileClass)) ;
    
	if(_orderIdSet)	{ queryString.addValuesList("OrderId", db::toSQL(dbKey, _orderId)) ; }
	if(_sensorIdSet) { queryString.addValuesList("SensorId", db::toSQL(dbKey, _sensorId)) ; }
	if(_acquisitionStationIdSet) { queryString.addValuesList("AcquisitionStationId", db::toSQL(dbKey, _acquisitionStationId)) ; }
	if(_processingStationIdSet)	{ queryString.addValuesList("ProcessingStationId", db::toSQL(dbKey, _processingStationId)) ; }
	if(_sensorModeSet) { queryString.addValuesList("SensorMode", db::toSQL(dbKey, _sensorMode)) ; }
	if(_phaseSet) {queryString.addValuesList("Phase", db::toSQL(dbKey, _phase)) ; }
	if(_satelliteIdSet) { queryString.addValuesList("SatelliteId", db::toSQL(dbKey, _satelliteId)) ; }
	if(_ascendingFlagSet) {	queryString.addValuesList("AscendingFlag", db::toSQL(dbKey, _ascendingFlag)) ; }
	if(_equatorXLongitudeSet) { queryString.addValuesList("EquatorXLongitude", db::toSQL(dbKey, _equatorXLongitude)) ; }
	if(_equatorXTimeSet) { queryString.addValuesList("EquatorXTime", db::toSQL(dbKey, _equatorXTime,dbConv::YearToFraction)) ; }
	if(_missionSet) { queryString.addValuesList("Mission", db::toSQL(dbKey, _mission)) ; }
	queryString.addValuesList("Qualified", db::toSQL(dbKey, _qualifiedId)) ;
	if(_originatorIdSet) { queryString.addValuesList("Originator", db::toSQL(dbKey, _originatorId)) ; }
	if(_qualifierFileSet) { queryString.addValuesList("QualifierFile", db::toSQL(dbKey, _qualifierFile)) ; }
	if(_bandSet) { queryString.addValuesList("Band", db::toSQL(dbKey, _band)) ; }
	if(_tdiSet) { queryString.addValuesList("Tdi", db::toSQL(dbKey, _tdi)) ; }
	if(_cloudpctgSet) { queryString.addValuesList("cloud_pctg", db::toSQL(dbKey, _cloudpctg)) ; }
	if(_prodspecSet) { queryString.addValuesList("prodspec", db::toSQL(dbKey, _prodspec)) ; }
	if(_RefDirIdSet) { queryString.addValuesList("ref_dir_id", db::toSQL(dbKey, _RefDirId)) ; }
    if (hasSerialId(conn)) {
	    if(_uniqueIdSet) { queryString.addValuesList("unique_id", db::toSQL(dbKey, _uniqueId)) ; }
	    if(_logicalIdSet) { queryString.addValuesList("logical_id", db::toSQL(dbKey, _logicalId)) ; }
    }
    if (hasQualityValue(conn) )
    {
        if (_qualityValueSet) { queryString.addValuesList("qualityvalue", db::toSQL(dbKey, _qualityValue)) ; }
    }
	if( hasMd5(conn) ) {
		if( _md5Set ) {
			queryString.addValuesList("md5", db::toSQL(dbKey, _md5)) ; 
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString.getSQL()) ;

	query.sqlString(queryString,"InventorySave") ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInventoryObject_SAVE") ;

	if (!query.execForStatus())
	{
		ACS_THROW(dbQuery::QueryExecuteException( // PRQA S 3081
					   "Query Execution error: Command : " + 
					   queryString.getSQL() + 
					   " Error : " + 
					   query.errorString())) ;
	}

    // obtain the last serial just inserted
	setId( hasSerialId(conn) ? getLastSerial(conn, getTable()) : 0 ) ; // PRQA S 3380

	// dir_id always set when saving only if it has been set to any value
	if (_DirIdSet) {
		SQLString maxFun(dbKey, SQLString::FUNCTION ) ; // max(dir_id)
        maxFun.setFunctionName("MAX");
       	maxFun.addFunctionParameter("dir_id");

		SQLString maxQueryString(dbKey, SQLString::SELECT) ; // select max(dir_id) from t_inventory 
 		maxQueryString.addTableList( getTable() ) ;
		maxQueryString.addSelectList(maxFun) ;
		dbQuery queryDir(conn) ;
		queryDir.sqlString(maxQueryString,"InventorySaveMaxDir") ;
		dbSet set ;
        queryDir.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
		dbRow row(dbKey) ;
        set.getNextRow(row) ; // first and only 
		IdType maxValue = 0 ;
		bool maxValueSet = false ;	
		row.columnSet(0, maxValue, maxValueSet) ; 
		if (!maxValueSet) {
			maxValue = 0 ;
        }

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "actual max dir_id value: " << maxValue) ;
		// increment it 
		maxValue ++ ;
		_DirId = maxValue ;
		_DirIdSet = true ;
        SQLString updateDirIdString(dbKey, SQLString::UPDATE ) ;
        updateDirIdString.addSetList("dir_id", db::toSQL(dbKey, _DirId) ) ;
        updateDirIdString.addTableList( getTable() );

        // Build Where clause
        if (hasSerialId(conn) && (0 != getId())) {  
            dbQueryParameters pars(dbKey) ;
            pars.addParameter(dbParam(dbKey, getTable(),getIdColumnName(),dbParam::EQUAL_TO,db::toSQL(dbKey, getId()))) ;
            updateDirIdString.addANDWhereParameter(pars) ;
        }
        else
        {
            dbQueryParameters pp(dbKey) ;
	        pp.addParameter(dbParam(dbKey, getTable(),"filename" , dbParam::EQUAL_TO , db::toSQL(dbKey, _fileName) )) ;
	        pp.addParameter(dbParam(dbKey, getTable(),"fileversion" , dbParam::EQUAL_TO , db::toSQL(dbKey, _fileVersion) )) ;
	        updateDirIdString.addANDWhereParameter(pp) ;
        }

        dbQuery updateDirQuery(conn, updateDirIdString, "InventoryUpdateMaxDir") ;	
		updateDirQuery.execForStatus() ;
	}  

    saveInveRelation(conn) ;
    transaction.commit() ; 

	// record that the values come from the database
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "calling setFromDatabase") ;

	setFromDatabase() ; 
	load(conn, getId()) ;
    return getId() ;
}



void dbInventoryObject::saveInveRelation(dbConnection &conn) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_parentIdSet: " << boolalpha << _parentIdSet << " _groupIdSet: " << _groupIdSet << " _relationSet: " << _relationSet << " _relationTypeSet: " << _relationTypeSet) ;
    
    const std::string table = "t_inv_relations" ;
    const std::string inv_id1_col = "inv_id1" ;
    const std::string inv_id2_col = "inv_id2" ;
    const std::string rel_col = "relation" ;
    const std::string dbKey = conn.getKey() ; 
	
	string inRelationType;
	string inRelation;
	bool relationIsUniqueId = true;
	if( _parentIdSet && not _relationSet && not _relationTypeSet ) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "parent of: " << getId() << " is " << _parentId) ;
		inRelationType = "CHILD_TO_PARENT";
		inRelation = _parentId;
		relationIsUniqueId = false;
	}
	else if( _relationSet && _relationTypeSet ) {
		inRelationType = _relationType;
		inRelation = _relation;
	}
	
	if( not inRelationType.empty() ) {
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Relation type: \"" << inRelationType << '\"') ;
	    SQLString rel(dbKey, SQLString::INSERT) ;
		rel.addTableList(table);
        rel.addValuesList(inv_id1_col, db::toSQL(dbKey, getId())) ;
		if( StringUtils::equalsNoCase(inRelationType, "CHILD_TO_PARENT") ) { 
    		SQLString psearch(dbKey, SQLString::SELECT ) ;
			psearch.addTableList(getTable());
		    psearch.addSelectList(getIdColumnName());
			{
    			dbQueryParameters pp(dbKey) ;
				string field = "unique_id";
				if( not relationIsUniqueId ) {
					field = "filename";
				}
				pp.addParameter(dbParam(dbKey, getTable(),field , dbParam::EQUAL_TO , db::toSQL(dbKey,inRelation) )) ;
				psearch.addANDWhereParameter(pp) ;
  			}

			rel.addValuesList(inv_id2_col, psearch);
		}

		{ 
    		SQLString reltype(dbKey, SQLString::SELECT ) ;
			reltype.addTableList(dbKey, "t_inv_relation_types trel");
		    reltype.addSelectList("trel.id");
			{
    			dbQueryParameters pp(dbKey) ;
				pp.addParameter(dbParam(dbKey, "trel","relation_type" , dbParam::EQUAL_TO , db::toSQL(dbKey,StringUtils::uppercase(inRelationType)) )) ;
				reltype.addANDWhereParameter(pp) ;
  			}

			rel.addValuesList("rel_type_id", reltype);
		}
        rel.addValuesList(rel_col, db::toSQL(dbKey,inRelation));
        dbQuery query(conn) ;
	    query.sqlString(rel,"InventorySaveChildRelation") ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Relation qry: " << rel.getSQL()) ;

		if (!query.execForStatus())
		{
			ACS_THROW(dbQuery::QueryExecuteException( // PRQA S 3081
						   "Query Execution error: Command : " + 
						   rel.getSQL() + 
						   " Error : " + 
						   query.errorString())) ;
		}

    }
	
    if (db::InvRelationsRules::instance()->update_required(_fileType, "CHILD_TO_PARENT")) {
        if( StringUtils::equalsNoCase(inRelationType, "CHILD_TO_PARENT") ) {
            SQLString rel(dbKey, SQLString::UPDATE ) ;
            rel.addSetList(inv_id2_col, db::toSQL(dbKey, getId()) ) ;


            // Add Table list (From can be used to creat join-UPDATE)
            rel.addTableList(table);


            // Build Where clause
            {
                dbQueryParameters pp(dbKey) ;
                pp.addParameter(dbParam(dbKey, table, inv_id2_col, dbParam::ISNULL)) ;
			    string parameter = _uniqueId;
			    if( not relationIsUniqueId ) {
				    parameter = _fileName;
			    }
                pp.addParameter(dbParam(dbKey, table, rel_col , dbParam::EQUAL_TO , db::toSQL(dbKey,parameter))) ;
                rel.addANDWhereParameter(pp) ;
            }
            { 
    	        SQLString reltype(dbKey, SQLString::SELECT ) ;
		        reltype.addTableList(dbKey, "t_inv_relation_types trel");
		        reltype.addSelectList("trel.id");
		        {
    		        dbQueryParameters pp(dbKey) ;
			        pp.addParameter(dbParam(dbKey, "trel","relation_type" , dbParam::EQUAL_TO , db::toSQL(dbKey,StringUtils::uppercase("CHILD_TO_PARENT")) )) ;
			        reltype.addANDWhereParameter(pp) ;
  		        }

			    dbQueryParameters pp(dbKey) ;
                pp.addParameter( dbParam( dbKey, table, "rel_type_id", dbParam::EQUAL_TO, reltype.getSQL(true) ) );
			    rel.addANDWhereParameter(pp);
            }
            dbQuery query(conn) ;
	        query.sqlString(rel,"InventorySaveParentRelation") ;
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "parent qry: " << rel.getSQL()) ;

		    if (!query.execForStatus())
		    {
			    ACS_THROW(dbQuery::QueryExecuteException( // PRQA S 3081
						       "Query Execution error: Command : " + 
						       rel.getSQL() + 
						       " Error : " + 
						       query.errorString())) ;
		    }
        }
    }                     
}

bool dbInventoryObject::exists(dbConnection &conn) const 
{
    const string dbKey=conn.getKey() ;
    SQLString queryString(dbKey, SQLString::SELECT) ;
    SQLString count(dbKey, SQLString::FUNCTION ) ;
   	count.setFunctionName("COUNT"); count.addFunctionParameter("*");
    queryString.addSelectList(count);
    queryString.addTableList( getTable() ) ;
    dbQueryParameters pars(dbKey) ;
    pars.addParameter(dbParam(dbKey, getTable(),"FileName",dbParam::EQUAL_TO,db::toSQL(dbKey, _fileName))) ;
 	pars.addParameter(dbParam(dbKey, getTable(),"FileVersion",dbParam::EQUAL_TO,db::toSQL(dbKey, _fileVersion))) ;
    queryString.addANDWhereParameter(pars) ;

	dbQuery query(conn, queryString, "InventoryCheckExistence") ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;
    dbSet set ;
	query.execForSet(set) ;
    ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
    ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
	dbRow row(dbKey) ;
    set.getNextRow(row) ; // first and only 
	int records = 0 ;
	row.column(0, records) ; 
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "(" << _fileName << "," << _fileVersion << "): nrecords:" << records) ;
    return records != 0 ;
}

// load the class from the database. Return true if object exist
bool dbInventoryObject::load(dbConnection &conn , IdType id) // PRQA S 4020
{
	Lock lock(*this) ;  // PRQA S 3050
    const string &dbKey=conn.getKey();

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called") ;

    IdType Id = id ;
    if (hasSerialId(conn)) {
	    if(Id == 0) {
		    Id=getId() ;
        }
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called with id=" << id << " getId():" << getId() << " Id:" << Id) ;
    }

	dbInventoryObject tempRecord(*this); //store current record.

	string oldFileName = _fileName ;
	string oldFileVersion = _fileVersion ;

	reset() ; 


    SQLString queryString(dbKey, SQLString::SELECT) ;
	deque<string> fields = getFieldArray(conn );
	for (uint i = 0 ; i < fields.size(); ++i)
	{
    	queryString.addSelectList( fields[i] ) ;
	}
    queryString.addTableList( getTable() ) ;
    if (hasSerialId(conn) && (0 != Id)) {  
        dbQueryParameters pars(dbKey) ;
        pars.addParameter(dbParam(dbKey, getTable(),getIdColumnName(),dbParam::EQUAL_TO,db::toSQL(dbKey, Id))) ;
        queryString.addANDWhereParameter(pars) ;
    }
    else { 
        dbQueryParameters pars(dbKey) ;
        pars.addParameter(dbParam(dbKey, getTable(),"FileName",dbParam::EQUAL_TO,db::toSQL(dbKey, oldFileName))) ;
 	    if (oldFileVersion != "") {
            pars.addParameter(dbParam(dbKey, getTable(),"FileVersion",dbParam::EQUAL_TO,db::toSQL(dbKey, oldFileVersion))) ;
        }
        queryString.addANDWhereParameter(pars) ;
    }
    queryString.setOrderByString(getOrderBy()) ;


	dbQuery query(conn, queryString, "InventoryLoad") ;
	dbRow row(dbKey) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

	try
	{
        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
	}
	catch (dbQuery::QueryNoDataFound &e)
	{
		*this = tempRecord;  //Nothing found: restore previuos situation.
		return false ;
	}

	loadRow(conn, row, 0) ;

	setFromDatabase() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	resetModifiedFlag() ;

	return true ;
}


// update an existing instance in database
dbPersistent::IdType dbInventoryObject::update(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	ACS_COND_THROW(!isFromDatabase(), ObjectNotLoaded("dbInventoryObject::update() error. Use the method save() instead of update().")) ;  // PRQA S 3081

	if (!isModified()) {
		return 0 ; 
    }

	string queryString ;
    const string &dbKey=conn.getKey();
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "UPDATE " + getTable() + " SET " ; 

		queryString += db::nullConv(dbKey, "Source = ",_sourceSet,_source) ;
		queryString += db::nullConv(dbKey, ", SourceSwVersion = ",_sourceSwVersionSet,_sourceSwVersion) ;

		queryString += ", GenerationTime= " + db::toSQL(dbKey, _generationTime,dbConv::YearToFraction) ;
		queryString += ", ValidityStart= " + db::toSQL(dbKey, _validityStart,dbConv::YearToFraction) ;
		queryString += ", ValidityStop= " + db::toSQL(dbKey, _validityStop,dbConv::YearToFraction) ;


		queryString += db::nullConv(dbKey, ", StartOrbitNumber= ",_startOrbitNumberSet,_startOrbitNumber) ;
		queryString += db::nullConv(dbKey, ", StopOrbitNumber= ",_stopOrbitNumberSet,_stopOrbitNumber) ;
		queryString += db::nullConv(dbKey, ", GeographicLocalization= ",_geographicLocalizationSet,*_geographicLocalization) ;
		queryString += db::nullConv(dbKey, ", QualityInfo= " ,_qualityInfoSet,_qualityInfo) ;

		queryString += ", ValidityFlag= " + db::toSQL(dbKey, _validityFlag) ;
		queryString += ", ValidationDate= " + db::toSQL(dbKey, _validationDate,dbConv::YearToFraction) ;
		queryString += db::nullConv(dbKey, ", HeaderSize= ",_headerSizeSet,_headerSize) ;
		queryString += db::nullConv(dbKey, ", DataSize= ",_dataSizeSet,_dataSize) ;
		queryString += db::nullConv(dbKey, ", InventoryDate= ",_inventoryDateSet,_inventoryDate) ;

		queryString += ", ModificationDate= " + SQLDialectFactory::sqlDialect(conn.getKey()).currentDateTime();

		queryString += ", FileType= " + db::toSQL(dbKey, _fileType) ;
		queryString += ", FileClass= " + db::toSQL(dbKey, _fileClass) ;

		queryString += db::nullConv(dbKey, ", OrderId= ",_orderIdSet,_orderId) ;
		queryString += db::nullConv(dbKey, ", SensorId= ",_sensorIdSet,_sensorId) ;
		queryString += db::nullConv(dbKey, ", AcquisitionStationId= ",_acquisitionStationIdSet,_acquisitionStationId) ;
		queryString += db::nullConv(dbKey, ", ProcessingStationId= ",_processingStationIdSet,_processingStationId) ;
		queryString += db::nullConv(dbKey, ", SensorMode= ",_sensorModeSet,_sensorMode) ;
		queryString += db::nullConv(dbKey, ", Phase= ",_phaseSet,_phase) ;
		queryString += db::nullConv(dbKey, ", SatelliteId= ",_satelliteIdSet,_satelliteId) ;
		queryString += db::nullConv(dbKey, ", AscendingFlag= ",_ascendingFlagSet,_ascendingFlag) ;
		queryString += db::nullConv(dbKey, ", EquatorXLongitude= ",_equatorXLongitudeSet,_equatorXLongitude) ;
		queryString += db::nullConv(dbKey, ", EquatorXTime= ",_equatorXTimeSet,_equatorXTime) ;
		queryString += db::nullConv(dbKey, ", Mission= ",_missionSet,_mission) ;
		queryString += ", Qualified= " + db::toSQL(dbKey, _qualifiedId) ;
		queryString += db::nullConv(dbKey, ", Originator= ",_originatorIdSet,_originatorId) ;
		queryString += db::nullConv(dbKey, ", QualifierFile= ",_qualifierFileSet,_qualifierFile) ;
		queryString += db::nullConv(dbKey, ", Band= ",_bandSet,_band) ;
		queryString += db::nullConv(dbKey, ", Tdi= ",_tdiSet,_tdi) ;
		queryString += db::nullConv(dbKey, ", cloud_pctg= ",_cloudpctgSet,_cloudpctg) ;
		queryString += db::nullConv(dbKey, ", prodspec= ",_prodspecSet,_prodspec) ;
		queryString += db::nullConv(dbKey, ", dir_id= ",_DirIdSet,_DirId) ;
		queryString += db::nullConv(dbKey, ", ref_dir_id= ",_RefDirIdSet,_RefDirId) ;
        if (hasSerialId(conn)) {
		    queryString += db::nullConv(dbKey, ", unique_id= ",_uniqueIdSet,_uniqueId) ;
		    queryString += db::nullConv(dbKey, ", logical_id= ",_logicalIdSet,_logicalId) ;
        }

        if (hasQualityValue(conn) )
        {
           queryString += db::nullConv(dbKey, ", qualityvalue= ",_qualityValueSet,_qualityValue) ;
        }
        if (hasMd5(conn) )
        {
           queryString += db::nullConv(dbKey, ", md5= ",_md5Set,_md5) ;
        }

		queryString += " WHERE " ;

	    if (hasSerialId(conn) && (0 != getId()))
	    {
		    queryString += getIdColumnName() +" = " + db::toSQL(dbKey, getId()) ;
	    }
        else {
		    queryString += "FileName = " + db::toSQL(dbKey, _fileName) + " AND " ;
		    queryString += "FileVersion = " + db::toSQL(dbKey, _fileVersion) ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;


	dbQuery query(conn, sqlString, "InventoryUpdate") ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInventoryObject_UPDATE") ;

    query.execForStatus() ;

	transaction.commit() ;
	resetModifiedFlag() ;
	return getId() ;

}


// remove the class from the database
dbPersistent::IdType dbInventoryObject::remove(dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050
    const string &dbKey = conn.getKey() ;
	load(conn) ;
	string queryString ;
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE (" ;

	    if (hasSerialId(conn) && (0 != getId())) {
		    queryString += getIdColumnName() +"=" + db::toSQL(dbKey, getId()) ;
        }
        else {
		    queryString += "FileName=" + db::toSQL(dbKey, _fileName) + " AND " ;
		    queryString += "FileVersion=" + db::toSQL(dbKey, _fileVersion) ;
        }
		queryString += ") " ;
		if (_DirIdSet) {
			queryString += "OR ref_dir_id = " + db::toSQL(dbKey, _DirId);
		}
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;


	dbQuery query(conn, sqlString,"InventoryDelete") ;

	query.execForStatus() ;

	// here the code to signal an inventory has been deleted
	
	resetFromDatabase() ;
	setModifiedFlag() ;
	
	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbInventoryObject::validate() const
{
	Lock lock(*this) ; // PRQA S 3050

	return true;
}


// export the class to stream
void dbInventoryObject::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryObject") ;
	
	dbPersistent::exportObj(s);
    
    bool ser = hasSerialId(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
    if (ser) {
    	s.setValue("_inventoryId", getId()) ;
    }
	
	if(_fileNameSet) { s.setValue("_fileName",_fileName) ; }
	s.setValue("_fileNameSet",_fileNameSet) ;

	if(_fileVersionSet) { s.setValue("_fileVersion",_fileVersion) ; }
	s.setValue("_fileVersionSet",_fileVersionSet) ;

    if (ser) {
	    if(_uniqueIdSet) { s.setValue("_uniqueId",_uniqueId) ; }
	    s.setValue("_uniqueIdSet",_uniqueIdSet) ;
	    if(_logicalIdSet) { s.setValue("_logicalId",_logicalId) ; }
	    s.setValue("_logicalIdSet",_logicalIdSet) ;  
 
        if(_parentIdSet) { s.setValue("_parentId",_parentId) ; }
	    s.setValue("_parentIdSet",_parentIdSet) ;
        if(_groupIdSet) { s.setValue("_groupId",_groupId) ; }
	    s.setValue("_groupIdSet",_groupIdSet) ;

    }

	if(_sourceSet) { s.setValue("_source",_source) ; }
	s.setValue("_sourceSet",_sourceSet) ;

	if(_sourceSwVersionSet) { s.setValue("_sourceSwVersion",_sourceSwVersion) ; }
	s.setValue("_sourceSwVersionSet",_sourceSwVersionSet) ;

	if(_generationTimeSet)
	{
		rsPushNSpace ns(s,"_generationTime") ;
		_generationTime.exportObj(s) ;
	}
	s.setValue("_generationTimeSet",_generationTimeSet) ;
	
	if(_validityStartSet)
	{
		rsPushNSpace ns(s,"_validityStart") ;
		_validityStart.exportObj(s) ;
	}
	
	s.setValue("_validityStartSet",_validityStartSet) ;

	if(_validityStopSet)
	{
		rsPushNSpace ns(s,"_validityStop") ;
		_validityStop.exportObj(s) ;
	}
	s.setValue("_validityStopSet",_validityStopSet) ;


	if(_startOrbitNumberSet) { s.setValue("_startOrbitNumber",_startOrbitNumber) ; }
	s.setValue("_startOrbitNumberSet",_startOrbitNumberSet) ;

	if(_stopOrbitNumberSet) { s.setValue("_stopOrbitNumber",_stopOrbitNumber) ; }
	s.setValue("_stopOrbitNumberSet",_stopOrbitNumberSet) ;

	if(_geographicLocalizationSet)
	{
		rsPushNSpace ns(s,"_geographicLocalization") ;
		_geographicLocalization->exportObj(s) ;
	}
	s.setValue("_geographicLocalizationSet",_geographicLocalizationSet) ;
	
	if(_qualityInfoSet) { s.setValue("_qualityInfo",_qualityInfo) ; }
	s.setValue("_qualityInfoSet",_qualityInfoSet) ;

	if(_validityFlagSet) { s.setValue("_validityFlag",_validityFlag) ; }
	s.setValue("_validityFlagSet",_validityFlagSet) ;

	if(_validationDateSet)
	{
		rsPushNSpace ns(s,"_validationDate") ;
		_validationDate.exportObj(s) ;
	}
	s.setValue("_validationDateSet",_validationDateSet) ;

	if(_headerSizeSet) { s.setValue("_headerSize",_headerSize) ; }
	s.setValue("_headerSizeSet",_headerSizeSet) ;

	if(_dataSizeSet) { s.setValue("_dataSize",_dataSize) ; }
	s.setValue("_dataSizeSet",_dataSizeSet) ;

	if(_inventoryDateSet)
	{
		rsPushNSpace ns(s,"_inventoryDate") ;
		_inventoryDate.exportObj(s) ;
	}
	s.setValue("_inventoryDateSet",_inventoryDateSet) ;

	if(_modificationDateSet)
	{
		rsPushNSpace ns(s,"_modificationDate") ;
		_modificationDate.exportObj(s) ;
	}
	s.setValue("_modificationDateSet",_modificationDateSet) ;

	if(_fileTypeSet) { s.setValue("_fileType",_fileType) ; }
	s.setValue("_fileTypeSet",_fileTypeSet) ;

	if(_fileClassSet) { s.setValue("_fileClass",_fileClass) ; }
	s.setValue("_fileClassSet",_fileClassSet) ;

	if(_orderIdSet) { s.setValue("_orderId",_orderId) ; }
	s.setValue("_orderIdSet",_orderIdSet) ;

	if(_sensorIdSet) { s.setValue("_sensorId",_sensorId) ; }
	s.setValue("_sensorIdSet",_sensorIdSet) ;

	if(_acquisitionStationIdSet) { s.setValue("_acquisitionStationId",_acquisitionStationId) ; }
	s.setValue("_acquisitionStationIdSet",_acquisitionStationIdSet) ;

	if(_processingStationIdSet) { s.setValue("_processingStationId",_processingStationId) ; }
	s.setValue("_processingStationIdSet",_processingStationIdSet) ;

	if(_sensorModeSet) { s.setValue("_sensorMode",_sensorMode) ; }
	s.setValue("_sensorModeSet",_sensorModeSet) ;

	if(_phaseSet) { s.setValue("_phase",_phase) ; }
	s.setValue("_phaseSet",_phaseSet) ;

	if(_satelliteIdSet) { s.setValue("_satelliteId",_satelliteId) ; }
	s.setValue("_satelliteIdSet",_satelliteIdSet) ;

	if(_ascendingFlagSet) { s.setValue("_ascendingFlag",_ascendingFlag) ; }
	s.setValue("_ascendingFlagSet",_ascendingFlagSet) ;

	if(_equatorXLongitudeSet) { s.setValue("_equatorXLongitude",_equatorXLongitude) ; }
	s.setValue("_equatorXLongitudeSet",_equatorXLongitudeSet) ;

	if(_equatorXTimeSet)
	{
		rsPushNSpace ns(s,"_equatorXTime") ;
		_equatorXTime.exportObj(s) ;
	}
	s.setValue("_equatorXTimeSet",_equatorXTimeSet) ;

	if(_missionSet) { s.setValue("_mission",_mission) ; }
	s.setValue("_missionSet",_missionSet) ;

	s.setValue("_qualifiedId",_qualifiedId) ;

	if(_originatorIdSet) { s.setValue("_originatorId",_originatorId) ; }
	s.setValue("_originatorIdSet",_originatorIdSet) ;

	if(_qualifierFileSet) { s.setValue("_qualifierFile",_qualifierFile) ; }
	s.setValue("_qualifierFileSet",_qualifierFileSet) ;

	if(_bandSet) { s.setValue("_band",_band) ; }
	s.setValue("_bandSet",_bandSet) ;

	if(_tdiSet) { s.setValue("_tdi",_tdi) ; }
	s.setValue("_tdiSet",_tdiSet) ;

	if(_cloudpctgSet) { s.setValue("_cloudpctg",_cloudpctg) ; }
	s.setValue("_cloudpctgSet",_cloudpctgSet) ;

	if(_prodspecSet) { s.setValue("_prodspec",_prodspec) ; }
	s.setValue("_prodspecSet",_prodspecSet) ;

	if(_DirIdSet)
	{
		s.setValue("_DirId",_DirId) ;
	}
	s.setValue("_DirIdSet",_DirIdSet) ;
	
	if(_RefDirIdSet)
	{
		s.setValue("_RefDirId",_RefDirId) ;
	}
	s.setValue("_RefDirIdSet",_RefDirIdSet) ;
	
	if( hasMd5(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) {
		if(_md5Set) {
			s.setValue("_md5",_md5) ;
		}
		s.setValue("_md5Set",_md5Set) ;
	}
}


// import the class from a stream
void dbInventoryObject::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryObject") ;
    bool ser = hasSerialId(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
	dbPersistent::importObj(s) ;
    if (ser) {
        IdType id = 0 ;
	    s.getValue("_inventoryId", id) ;
	    setId(id) ;
    }

	s.getValue("_fileNameSet",_fileNameSet) ;
	if(_fileNameSet) { s.getValue("_fileName",_fileName) ; }
	else { _fileName = "" ; }

	s.getValue("_fileVersionSet",_fileVersionSet) ;
	if(_fileVersionSet) { s.getValue("_fileVersion",_fileVersion) ; }
	else {  _fileVersion = "" ; }


    if (ser) {
	    s.getValue("_uniqueIdSet",_uniqueIdSet) ;
	    if(_uniqueIdSet) { s.getValue("_uniqueId",_uniqueId) ; }
        else { _uniqueId = "" ; }
	    s.getValue("_logicalIdSet",_logicalIdSet) ;
	    if(_logicalIdSet) { s.getValue("_logicalId",_logicalId) ; }
        else { _logicalId = "" ; }
    
        s.getValue("_parentIdSet",_parentIdSet) ;
	    if(_parentIdSet) { s.getValue("_parentId",_parentId) ; }
        else { _parentId = "" ; }
        s.getValue("_groupIdSet",_groupIdSet) ;
	    if(_groupIdSet) { s.getValue("_groupId",_groupId) ; }
        else { _groupId = "" ; }

    }


	s.getValue("_sourceSet",_sourceSet) ;
	if(_sourceSet) { s.getValue("_source",_source) ; }
	else  { _source = "" ; }

	s.getValue("_sourceSwVersionSet",_sourceSwVersionSet) ;
	if(_sourceSwVersionSet) { s.getValue("_sourceSwVersion",_sourceSwVersion) ; }
	else { _sourceSwVersion = "" ; }
		
	s.getValue("_generationTimeSet",_generationTimeSet) ;
	if(_generationTimeSet) {
		rsPushNSpace ns(s,"_generationTime") ;
		_generationTime.importObj(s) ;
	}
	else { _generationTime.reset() ; }
	
	s.getValue("_validityStartSet",_validityStartSet) ;
	if(_validityStartSet) {
		rsPushNSpace ns(s,"_validityStart") ;
		_validityStart.importObj(s) ;
	}
	else { _validityStart.reset() ; }
		
	s.getValue("_validityStopSet",_validityStopSet) ;
	if(_validityStopSet) {
		rsPushNSpace ns(s,"_validityStop") ;
		_validityStop.importObj(s) ;
	}
	else { _validityStop.reset() ; }
		
	s.getValue("_startOrbitNumberSet",_startOrbitNumberSet) ;
	if(_startOrbitNumberSet) { s.getValue("_startOrbitNumber",_startOrbitNumber) ; }
	else { _startOrbitNumber = 0 ; }
	
	s.getValue("_stopOrbitNumberSet",_stopOrbitNumberSet) ;
	if(_stopOrbitNumberSet) { s.getValue("_stopOrbitNumber",_stopOrbitNumber) ; }
	else { _stopOrbitNumber = 0 ; }
	
	s.getValue("_geographicLocalizationSet",_geographicLocalizationSet) ;
	if(_geographicLocalizationSet) {
		rsPushNSpace ns(s,"_geographicLocalization") ;
		_geographicLocalization = dbGeoObject::importObject(s) ;
	}
	else { _geographicLocalization = 0 ; }
		
	s.getValue("_qualityInfoSet",_qualityInfoSet) ;
	if(_qualityInfoSet) { s.getValue("_qualityInfo",_qualityInfo) ; }
	else { _qualityInfo = "" ; }
	
	s.getValue("_validityFlagSet",_validityFlagSet) ;
	if(_validityFlagSet) { s.getValue("_validityFlag",_validityFlag) ; }
	else { _validityFlag = false ; }
		
	s.getValue("_validationDateSet",_validationDateSet) ;
	if(_validationDateSet) {
		rsPushNSpace ns(s,"_validationDate") ;
		_validationDate.importObj(s) ;
	}
	else { _validationDate.reset() ; }
	
	s.getValue("_headerSizeSet",_headerSizeSet) ;
	if(_headerSizeSet) { s.getValue("_headerSize",_headerSize) ; }
	else { _headerSize = 0 ; }
		
	s.getValue("_dataSizeSet",_dataSizeSet) ;
	if(_dataSizeSet) { s.getValue("_dataSize",_dataSize) ; }
	else { _dataSize = 0 ; }
	
	s.getValue("_inventoryDateSet",_inventoryDateSet) ;
	if(_inventoryDateSet) {
		rsPushNSpace ns(s,"_inventoryDate") ;
		_inventoryDate.importObj(s) ;
	}
	else { _inventoryDate.reset() ; }
		
	s.getValue("_modificationDateSet",_modificationDateSet) ;
	if(_modificationDateSet) {
		rsPushNSpace ns(s,"_modificationDate") ;
		_modificationDate.importObj(s) ;
	}
	else { _modificationDate.reset() ; }

	s.getValue("_fileTypeSet",_fileTypeSet) ;
	if(_fileTypeSet) { s.getValue("_fileType",_fileType) ; }
	else { _fileType = "" ; }

	s.getValue("_fileClassSet",_fileClassSet) ;
	if(_fileClassSet) { s.getValue("_fileClass",_fileClass) ; }
	else { s.getValue("_orderIdSet",_orderIdSet) ; }
	if(_orderIdSet) { s.getValue("_orderId",_orderId) ; }
	else { _orderId = 0 ; }
	
	s.getValue("_sensorIdSet",_sensorIdSet) ;
	if(_sensorIdSet) { s.getValue("_sensorId",_sensorId) ; }
	else { _sensorId = 0 ; }
	
	s.getValue("_acquisitionStationIdSet",_acquisitionStationIdSet) ;
	if(_acquisitionStationIdSet) { s.getValue("_acquisitionStationId",_acquisitionStationId) ; }
	else { _acquisitionStationId = 0 ; }
	
	s.getValue("_processingStationIdSet",_processingStationIdSet) ;
	if(_processingStationIdSet) { s.getValue("_processingStationId",_processingStationId) ; }
	else { _processingStationId = 0 ; }

	s.getValue("_sensorModeSet",_sensorModeSet) ;
	if(_sensorModeSet) { s.getValue("_sensorMode",_sensorMode) ; }
	else { _sensorMode = "" ; }
	
	s.getValue("_phaseSet",_phaseSet) ;
	if(_phaseSet) { s.getValue("_phase",_phase) ; }
	else { _phase = "" ; }
	
	s.getValue("_satelliteIdSet",_satelliteIdSet) ;
	if(_satelliteIdSet) { s.getValue("_satelliteId",_satelliteId) ; }
	else { _satelliteId = 0 ; }

	s.getValue("_ascendingFlagSet",_ascendingFlagSet) ;
	if(_ascendingFlagSet) { s.getValue("_ascendingFlag",_ascendingFlag) ; }
	else { _ascendingFlag = false ; }
	
	s.getValue("_equatorXLongitudeSet",_equatorXLongitudeSet) ;
	if(_equatorXLongitudeSet) { s.getValue("_equatorXLongitude",_equatorXLongitude) ; }
	else { _equatorXLongitude = 0.0 ; }
		
	s.getValue("_equatorXTimeSet",_equatorXTimeSet) ;
	if(_equatorXTimeSet) {
		rsPushNSpace ns(s,"_equatorXTime") ;
		_equatorXTime.importObj(s) ;
	}
	else { _equatorXTime.reset() ; }


	s.getValue("_missionSet",_missionSet) ;
	if(_missionSet) { s.getValue("_mission",_mission) ; }
	else { _mission = 0 ; }

	s.getValue("_qualifiedId",_qualifiedId) ;

	s.getValue("_originatorIdSet",_originatorIdSet) ;
	if(_originatorIdSet) { s.getValue("_originatorId",_originatorId) ; }
	else { _originatorId = 0 ; }

	s.getValue("_qualifierFileSet",_qualifierFileSet) ;
	if(_qualifierFileSet) { s.getValue("_qualifierFile",_qualifierFile) ; }
	else { _qualifierFile = "" ; }

	s.getValue("_bandSet",_bandSet) ;
	if(_bandSet) { s.getValue("_band",_band) ; }
	else { _band = 0 ; }

	s.getValue("_tdiSet",_tdiSet) ;
	if(_tdiSet) { s.getValue("_tdi",_tdi) ; }
	else { _tdi = 0 ; }

	s.getValue("_cloudpctgSet",_cloudpctgSet) ;
	if(_cloudpctgSet) { s.getValue("_cloudpctg",_cloudpctg) ; }
	else { _cloudpctg = 0 ; }

	s.getValue("_prodspecSet",_prodspecSet) ;
	if(_prodspecSet) { s.getValue("_prodspec",_prodspec) ; }
	else { _prodspec = "" ; }

	s.getValue("_DirIdSet",_DirIdSet) ;
	if(_DirIdSet)
	{
		s.getValue("_DirId",_DirId) ;
	}
	
	s.getValue("_RefDirIdSet",_RefDirIdSet) ;
	if(_RefDirIdSet)
	{
		s.getValue("_RefDirId",_RefDirId) ;
	}
	
	if( hasMd5(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) {
		s.getValue("_md5Set",_md5Set) ;
		if(_md5Set) {
			s.getValue("_md5",_md5) ;
		}
	}



}


// export the class to XML stream
void dbInventoryObject::exportXML(XMLOstream &x) const
{
	Lock lock(*this) ; // PRQA S 3050

    using db::toSQL ;
    const string K = "" ;

	XMLOstream::Tag txml(x, "dbInventoryObject");

	dbPersistent::exportXML(x);
    bool ser = hasSerialId(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412

	if (ser) {
		ostringstream os;
		os << getId();
		XMLOstream::Tag txml0(x, "_inventoryId", os.str(), "label=\"Inventory Id\"");
	}
     // PRQA S 3380, 3385 L2
	{ XMLOstream::Tag t(x, "_fileName", _fileNameSet ? _fileName : "<null>", "label=\"File Name\""); }
	{ XMLOstream::Tag t(x, "_fileNameSet", (_fileNameSet ? "true" : "false")) ; }
	if (ser) {
	    { XMLOstream::Tag t(x, "_uniqueIdSet", (_uniqueIdSet ? "true" : "false")) ; }
	    { XMLOstream::Tag t(x, "_uniqueId", _uniqueIdSet? toSQL(K, _uniqueId) : "<null>", "label=\"Unique Id\""); }
	    { XMLOstream::Tag t(x, "_logicalIdSet", (_logicalIdSet ? "true" : "false")) ; }
	    { XMLOstream::Tag t(x, "_logicalId", _logicalIdSet? toSQL(K, _logicalId) : "<null>", "label=\"Unique Id\""); }
        
        { XMLOstream::Tag t(x, "_parentIdSet", (_parentIdSet ? "true" : "false")) ; }
	    { XMLOstream::Tag t(x, "_parentId", _parentIdSet? toSQL(K, _parentId) : "<null>", "label=\"Parent Id\""); }
        { XMLOstream::Tag t(x, "_groupIdSet", (_groupIdSet ? "true" : "false")) ; }
	    { XMLOstream::Tag t(x, "_groupId", _groupIdSet? toSQL(K, _groupId) : "<null>", "label=\"Group Id\""); }
   }

	{ XMLOstream::Tag t(x, "_fileVersion", _fileVersionSet ? _fileVersion : "<null>", "label=\"File Version\""); }
	{ XMLOstream::Tag t(x, "_fileVersionSet", (_fileVersionSet ? "true" : "false")) ; }
	{ XMLOstream::Tag t(x, "_source",_sourceSet?_source:"<null>", "label=\"Source\""); }
	{ XMLOstream::Tag t(x, "_sourceSet", (_sourceSet ? "true" : "false")) ; }
	{ XMLOstream::Tag t(x, "_sourceSwVersion", _sourceSwVersionSet? _sourceSwVersion:"<null>", "label=\"Source Sw Version\""); }
	{ XMLOstream::Tag txml3Set(x, "_sourceSwVersionSet", (_sourceSwVersionSet ? "true" : "false")) ; }

	if(_generationTimeSet) {
		XMLOstream::Tag t(x, "_generationTime", "", "label=\"Generation Time\"");
		_generationTime.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_generationTime", "<null>", "label=\"Generation Time\"");
	}
	{ XMLOstream::Tag t(x, "_generationTimeSet", (_generationTimeSet ? "true" : "false")) ; }
	
	if(_validityStartSet) {
		XMLOstream::Tag t(x, "_validityStart", "", "label=\"Validity Start\"");
		_validityStart.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_validityStart", "<null>", "label=\"Validity Start\"");
	}
	{ XMLOstream::Tag t(x, "_validityStartSet", (_validityStartSet ? "true" : "false")) ; }
	
	if(_validityStopSet) {
		XMLOstream::Tag t(x, "_validityStop", "", "label=\"Validity Stop\"");
		_validityStop.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_validityStop", "<null>", "label=\"Validity Stop\"");
	}
	{ XMLOstream::Tag t(x, "_validityStopSet", (_validityStopSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_startOrbitNumber", _startOrbitNumberSet? toSQL(K, _startOrbitNumber) : "<null>", "label=\"Start Orbit Number\""); }
	{ XMLOstream::Tag t(x, "_startOrbitNumberSet", (_startOrbitNumberSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_stopOrbitNumber", _stopOrbitNumberSet ? toSQL(K, _stopOrbitNumber) : "<null>", "label=\"Stop Orbit Number\""); }
	{ XMLOstream::Tag t(x, "_stopOrbitNumberSet", (_stopOrbitNumberSet ? "true" : "false")) ; }
	
	if(_geographicLocalizationSet) {
		XMLOstream::Tag t(x, "_geographicLocalization", "", "label=\"Geographic Localization\"");
		_geographicLocalization->exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_geographicLocalization", "<null>", "label=\"Geographic Localization\"");
	}
	{ XMLOstream::Tag t(x, "_geographicLocalizationSet", (_geographicLocalizationSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_qualityInfo", _qualityInfoSet ? _qualityInfo : "<null>", "label=\"Quality Info\""); }
	{ XMLOstream::Tag t(x, "_qualityInfoSet", (_qualityInfoSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_validityFlag", _validityFlagSet ? (_validityFlag ? "true" : "false") : "<null>", "label=\"Validity Flag\""); }
	{ XMLOstream::Tag t(x, "_validityFlagSet", (_validityFlagSet ? "true" : "false")) ; }
	
	if(_validationDateSet) {
		XMLOstream::Tag txml13(x, "_validationDate", "", "label=\"Validation Date\"");
		_validationDate.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_validationDate", "<null>", "label=\"Validation Date\"");
	}
	{ XMLOstream::Tag t(x, "_validationDateSet", (_validationDateSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_headerSize", _headerSizeSet ? toSQL(K, _headerSize) : "<null>", "label=\"Header Size\""); }
	{ XMLOstream::Tag t(x, "_headerSizeSet", (_headerSizeSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_dataSize", _dataSizeSet ? toSQL(K, _dataSize) : "<null>", "label=\"Data Size\""); }
	{ XMLOstream::Tag t(x, "_dataSizeSet", (_dataSizeSet ? "true" : "false")) ; }
	
	if(_inventoryDateSet) 	{
		XMLOstream::Tag t(x, "_inventoryDate", "", "label=\"Inventory Date\"");
		_inventoryDate.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_inventoryDate", "<null>", "label=\"Inventory Date\"");
	}
	{ XMLOstream::Tag t(x, "_inventoryDateSet", (_inventoryDateSet ? "true" : "false")) ; }
	
	if(_modificationDateSet) {
		XMLOstream::Tag t(x, "_modificationDate", "", "label=\"Modification Date\"");
		_modificationDate.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_modificationDate", "<null>", "label=\"Modification Date\"");
	}
	{ XMLOstream::Tag t(x, "_modificationDateSet", (_modificationDateSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_fileType", _fileTypeSet ? _fileType : "<null>", "label=\"File Type\""); }
	{ XMLOstream::Tag t(x, "_fileTypeSet", (_fileTypeSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_fileClass", _fileClassSet ? _fileClass : "<null>", "label=\"File Class\""); }
	{ XMLOstream::Tag t(x, "_fileClassSet", (_fileClassSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_orderId", _orderIdSet ? toSQL(K, _orderId) : "<null>", "label=\"Order Id\""); }
	{ XMLOstream::Tag t(x, "_orderIdSet", (_orderIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_sensorId", _sensorIdSet ? toSQL(K, _sensorId) : "<null>", "label=\"Sensor Id\""); }
	{ XMLOstream::Tag t(x, "_sensorIdSet", (_sensorIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_acquisitionStationId", _acquisitionStationIdSet ? toSQL(K, _acquisitionStationId) : "<null>", "label=\"Acquisition Station Id\""); }
	{ XMLOstream::Tag t(x, "_acquisitionStationIdSet", (_acquisitionStationIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_processingStationId", _processingStationIdSet ? toSQL(K, _processingStationId) : "<null>", "label=\"Processing Station Id\""); }
	{ XMLOstream::Tag t(x, "_processingStationIdSet", (_processingStationIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_sensorMode", _sensorModeSet ? _sensorMode: "<null>", "label=\"Sensor Mode\"");}
	{ XMLOstream::Tag t(x, "_sensorModeSet", (_sensorModeSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_phase", _phaseSet ? _phase : "<null>", "label=\"Phase\""); }
	{ XMLOstream::Tag t(x, "_phaseSet", (_phaseSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_satelliteId", _satelliteIdSet ? toSQL(K, _satelliteId) : "<null>", "label=\"Satellite Id\""); }
	{ XMLOstream::Tag t(x, "_satelliteIdSet", (_satelliteIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_ascendingFlag", _ascendingFlagSet ? (_ascendingFlag ? "true" : "false") : "<null>", "label=\"Ascending Flag\""); }
	{ XMLOstream::Tag t(x, "_ascendingFlagSet", (_ascendingFlagSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_equatorXLongitude", _equatorXLongitudeSet?toSQL(K, _equatorXLongitude):"<null>", "label=\"Equator X Longitude\""); }
	{ XMLOstream::Tag t(x, "_equatorXLongitudeSet", (_equatorXLongitudeSet ? "true" : "false")) ; }

	if(_equatorXTimeSet) {
		XMLOstream::Tag t(x, "_equatorXTime", "", "label=\"Equator X Time\"");
		_equatorXTime.exportXML(x) ;
	}
	else {
		XMLOstream::Tag t(x, "_equatorXTime", "<null>", "label=\"Equator X Time\"");
	}
	{ XMLOstream::Tag t(x, "_equatorXTimeSet", (_equatorXTimeSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_mission", _missionSet ? toSQL(K, _mission):"<null>", "label=\"Mission Id\""); }
	{ XMLOstream::Tag t(x, "_missionSet", (_missionSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_qualifiedId", toSQL(K, _qualifiedId), "label=\"Qualified\""); }
	{ XMLOstream::Tag t(x, "_qualifiedIdSet", ("true")) ; }

	{ XMLOstream::Tag t(x, "_originatorId", _originatorIdSet?toSQL(K, _originatorId):"<null>", "label=\"Originator\""); }
	{ XMLOstream::Tag t(x, "_originatorIdSet", (_originatorIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_qualifierFile", _qualifierFileSet? toSQL(K, _originatorId) : "<null>", "label=\"Qualifier File\""); }
	{ XMLOstream::Tag t(x, "_qualifierFileSet", (_qualifierFileSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_band", _bandSet ? toSQL(K, _band):"<null>", "label=\"Band\""); }
	{ XMLOstream::Tag t(x, "_bandSet", (_bandSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_tdi", _tdiSet ?  toSQL(K, _tdi): "<null>", "label=\"Tdi\""); }
	{ XMLOstream::Tag t(x, "_tdiSet", (_tdiSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_cloudpctg", _cloudpctgSet ?  toSQL(K, _cloudpctg): "<null>", "label=\"Cloud Percentage\""); }
	{ XMLOstream::Tag t(x, "_cloudpctgSet", (_cloudpctgSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_prodspec", _prodspecSet ?  toSQL(K, _prodspec): "<null>", "label=\"Product Specification\""); }
	{ XMLOstream::Tag t(x, "_prodspecSet", (_prodspecSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_DirId", _DirIdSet? toSQL(K, _DirId) : "<null>", "label=\"Dir Id\""); }
	{ XMLOstream::Tag t(x, "_DirIdSet", (_DirIdSet ? "true" : "false")) ; }

	{ XMLOstream::Tag t(x, "_RefDirId", _RefDirIdSet? toSQL(K, _RefDirId) : "<null>", "label=\"Ref Dir Id\""); }
	{ XMLOstream::Tag t(x, "_RefDirIdSet", (_RefDirIdSet ? "true" : "false")) ; }
	
	if( hasMd5(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) {
		{ XMLOstream::Tag t(x, "_md5", _md5Set? toSQL(K, _md5) : "<null>", "label=\"Md5\""); }
		{ XMLOstream::Tag t(x, "_md5Set", (_md5Set ? "true" : "false")) ; }
	}
	
    // PRQA L:L2
}



// load the object from a row
int dbInventoryObject::loadRow(dbConnection &conn, const dbRow &row, int offset)
{
	Lock lock(*this) ; // PRQA S 3050

    uint16_t loff = offset ; // PRQA S 3000, 3010
    if (hasSerialId(conn ) ) {
        IdType id = 0 ;
        bool set = false ;
	    row.columnSet(loff++, id, 			set) ; 
        setId(id) ;
    }
	row.columnSet(loff++, _fileName, 			_fileNameSet) ; 
	row.columnSet(loff++, _fileVersion, 		_fileVersionSet) ;
    if (hasSerialId(conn) ) {
	    row.columnSet(loff++, _uniqueId, 		_uniqueIdSet) ; 
	    row.columnSet(loff++, _logicalId, 	_logicalIdSet) ; 
    }
    if (hasQualityValue(conn))
    {
	    row.columnSet(loff++, _qualityValue, 		_qualityValueSet) ; 
    }
	row.columnSet(loff++, _source, 			_sourceSet) ;
	row.columnSet(loff++, _sourceSwVersion, 	_sourceSwVersionSet) ;
	row.columnSet(loff++, _generationTime, 	_generationTimeSet) ;
	row.columnSet(loff++, _validityStart, 	_validityStartSet) ;
	row.columnSet(loff++, _validityStop,		_validityStopSet) ;
	row.columnSet(loff++, _startOrbitNumber, 	_startOrbitNumberSet) ;
	row.columnSet(loff++, _stopOrbitNumber, 	_stopOrbitNumberSet) ;

	
    row.columnSet(loff++, _geographicLocalization, _geographicLocalizationSet) ;
    if (!_geographicLocalizationSet) {
		delete _geographicLocalization;
		_geographicLocalization=0;
	}

	row.columnSet(loff++, _qualityInfo, 		_qualityInfoSet) ;
	row.columnSet(loff++, _validityFlag, 		_validityFlagSet) ;
	row.columnSet(loff++, _validationDate, 	_validationDateSet) ;
	row.columnSet(loff++, _headerSize, 		_headerSizeSet) ;
	row.columnSet(loff++, _dataSize, 			_dataSizeSet) ;
	row.columnSet(loff++, _inventoryDate, 		_inventoryDateSet) ;
	row.columnSet(loff++, _modificationDate, 	_modificationDateSet) ;
	row.columnSet(loff++, _fileType, 			_fileTypeSet) ;
	row.columnSet(loff++, _fileClass, 			_fileClassSet) ;
	row.columnSet(loff++, _orderId, 			_orderIdSet) ;
	row.columnSet(loff++, _sensorId, 			_sensorIdSet) ;
	row.columnSet(loff++, _acquisitionStationId, _acquisitionStationIdSet) ;
	row.columnSet(loff++, _processingStationId, _processingStationIdSet) ;
	row.columnSet(loff++, _sensorMode, 		_sensorModeSet) ;
	row.columnSet(loff++, _phase, 				_phaseSet) ;
	row.columnSet(loff++, _satelliteId, _satelliteIdSet) ;
	row.columnSet(loff++, _ascendingFlag, _ascendingFlagSet) ;
	row.columnSet(loff++, _equatorXLongitude, _equatorXLongitudeSet) ;
	row.columnSet(loff++, _equatorXTime, _equatorXTimeSet) ;
	row.columnSet(loff++, _mission, _missionSet) ;
	bool qualifiedIdSet = false ;
	row.columnSet(loff++, _qualifiedId, qualifiedIdSet) ;
	if (!qualifiedIdSet) { _qualifiedId = defaultQualifiedId ; }
	
	row.columnSet(loff++, _originatorId, _originatorIdSet) ;
	row.columnSet(loff++, _qualifierFile, _qualifierFileSet) ;
	row.columnSet(loff++, _band, _bandSet) ;
	row.columnSet(loff++, _tdi, _tdiSet) ;
	row.columnSet(loff++, _cloudpctg, _cloudpctgSet) ;
	row.columnSet(loff++, _prodspec, _prodspecSet) ;
	row.columnSet(loff++, _DirId, _DirIdSet) ;
	row.columnSet(loff++, _RefDirId, _RefDirIdSet) ;
	if( hasMd5(conn) ) {
		row.columnSet(loff++, _md5, _md5Set) ;
	}
	row.columnSet(loff++, _parentId, _parentIdSet) ;

	setFromDatabase() ;

	return loff ; // PRQA S 3000
}




string dbInventoryObject::guessCacheName(bool* set) const 
{
	bool set1 = false ;
	string fileName			= getFileName(&set1) ;
    bool set2 = false ; 
	string fileVersion		= getFileVersion(&set2) ;
    bool set3 = false ;
	DateTime inventoryDate	= getInventoryDate(&set3) ;

	string r ;
	if (set1 && set2)	/* Regardless set3 */
	{
		if (set) { *set = true ; }
	
		time_t t = 0  ;
		if (!set3) { t = 0 ; }
		else { t = inventoryDate.Datetime2time_t() ; }
			
		r = File::getCacheFileName(fileName, fileVersion, t) ;
	}
	else
	{
        r = "" ; 
        if (set) { *set = false ; }
    }


	return r ;
}


string dbInventoryObject::guessCacheNameNoInvDate(bool* set) const 
{
	bool set1 = false ;
	string fileName		= getFileName(&set1) ;
    bool set2 = false ;
	string fileVersion	= getFileVersion(&set2) ;
	
	string r ;
	if (set1 && set2)	
	{
		if (set) { *set = true ; }
		const time_t t = 0 ;
		r = File::getCacheFileName(fileName, fileVersion, t) ;
	}
	else
	{	
        r = "" ; 
        if (set) { *set = false ; }
    }


	return r ;
}


void dbInventoryObject::setFileName(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbInventoryObject::setFileName(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 128) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 128); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setFileName(). String too long. Truncated to 128 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_fileNameSet || _fileName != v ) 
	{
		_fileName = v ;
		_fileNameSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getFileName(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {	*set = _fileNameSet ; }
	return _fileName ;
}


void dbInventoryObject::unsetFileName()
{
	Lock lock(*this) ; // PRQA S 3050

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbInventoryObject::unsetFileName(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if(_fileNameSet) {
		setModifiedFlag();
    }

	_fileNameSet = false;

}




void dbInventoryObject::setFileVersion(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbInventoryObject::setFileVersion(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 32) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 32); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setFileVersion(). String too long. Truncated to 32 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_fileVersionSet || (_fileVersion != v) ) 
	{
		_fileVersion = v ;
		_fileVersionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getFileVersion(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _fileVersionSet ; }
	return _fileVersion ;
}


void dbInventoryObject::unsetFileVersion()
{

	Lock lock(*this) ; // PRQA S 3050
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbInventoryObject::unsetFileVersion(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if(_fileVersionSet) {
		setModifiedFlag();
    }


	_fileVersionSet = false;

}



void dbInventoryObject::setSource(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 128) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 128); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setSource(). String too long. Truncated to 128 chars." ) ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}
	if (!_sourceSet || (_source != v) ) 
	{
		_source = v ;
		_sourceSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getSource(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _sourceSet ; }
	return _source ;
}

void dbInventoryObject::unsetSource()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_sourceSet) {
		setModifiedFlag();
    }

	_sourceSet = false;

}


void dbInventoryObject::setSourceSwVersion(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 8) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 8); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setSourceSwVersion(). String too long. Truncated to 8 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_sourceSwVersionSet || (_sourceSwVersion != v) ) 
	{
		_sourceSwVersion = v ;
		_sourceSwVersionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getSourceSwVersion(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _sourceSwVersionSet ; }
	return _sourceSwVersion ;
}


void dbInventoryObject::unsetSourceSwVersion()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_sourceSwVersionSet) {
		setModifiedFlag();
    }
	_sourceSwVersionSet = false;

}


void dbInventoryObject::setGenerationTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_generationTimeSet || (_generationTime != v) ) 
	{
		_generationTime = v ;
		_generationTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryObject::getGenerationTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _generationTimeSet ; }
	return _generationTime ;
}


void dbInventoryObject::unsetGenerationTime()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_generationTimeSet) {
		setModifiedFlag();
    }
	_generationTimeSet = false;

}


void dbInventoryObject::setValidityStart(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_validityStartSet || (_validityStart != v) ) 
	{
		_validityStart = v ;
		_validityStartSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryObject::getValidityStart(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _validityStartSet ; }
	return _validityStart ;
}


void dbInventoryObject::unsetValidityStart()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_validityStartSet) {
		setModifiedFlag();
    }
	_validityStartSet = false;

}



void dbInventoryObject::setValidityStop(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_validityStopSet || (_validityStop != v) ) 
	{
		_validityStop = v ;
		_validityStopSet = true ;
		setModifiedFlag() ;
	}
}


void dbInventoryObject::unsetValidityStop()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_validityStopSet) {
		setModifiedFlag();
    }
	_validityStopSet = false;

}



const DateTime &dbInventoryObject::getValidityStop(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _validityStopSet ; }
	return _validityStop ;
}

const dbGeoObject *dbInventoryObject::shape() const 
{
	return getGeographicLocalization() ;
}

void dbInventoryObject::setStartOrbitNumber(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_startOrbitNumberSet || (_startOrbitNumber != v) ) 
	{
		_startOrbitNumber = v ;
		_startOrbitNumberSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getStartOrbitNumber(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _startOrbitNumberSet ; }
	return _startOrbitNumber ;
}


void dbInventoryObject::unsetStartOrbitNumber()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_startOrbitNumberSet) {
		setModifiedFlag();
    }

	_startOrbitNumberSet = false;

}


void dbInventoryObject::setStopOrbitNumber(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_stopOrbitNumberSet || (_stopOrbitNumber != v) ) 
	{
		_stopOrbitNumber = v ;
		_stopOrbitNumberSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getStopOrbitNumber(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _stopOrbitNumberSet ; }
	return _stopOrbitNumber ;
}


void dbInventoryObject::unsetStopOrbitNumber()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_stopOrbitNumberSet) {
		setModifiedFlag();
    }

	_stopOrbitNumberSet = false;

}


void dbInventoryObject::setGeographicLocalization(const dbGeoObject &v)
{
	Lock lock(*this) ; // PRQA S 3050
	
	delete _geographicLocalization;
	_geographicLocalization=0;
	
	_geographicLocalization = v.clone() ;
	_geographicLocalizationSet = true ;
	setModifiedFlag() ;
}


const dbGeoObject *dbInventoryObject::getGeographicLocalization(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _geographicLocalizationSet ; }
	return _geographicLocalization ;
}


void dbInventoryObject::unsetGeographicLocalization()
{
	Lock lock(*this) ; // PRQA S 3050

	delete _geographicLocalization;
	_geographicLocalization=0;

	if(_geographicLocalizationSet) {
		setModifiedFlag();
    }

	_geographicLocalizationSet = false;

}



void dbInventoryObject::setQualityInfo(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050
    
    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 72) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 72); // PRQA S 4400
		ACS_LOG_TRYCATCHNOTIFY_EX(InvalidUse("dbInventoryObject::setQualityInfo() too long. Truncated to 72 chars.")) ;
	}

	if (!_qualityInfoSet || (_qualityInfo != v) ) 
	{
		_qualityInfo = v ;
		_qualityInfoSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getQualityInfo(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _qualityInfoSet ; }
	return _qualityInfo ;
}


void dbInventoryObject::unsetQualityInfo()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_qualityInfoSet) {
		setModifiedFlag();
    }

	_qualityInfoSet = false;

}


void dbInventoryObject::setValidityFlag(bool v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_validityFlagSet || (_validityFlag != v) ) 
	{
		_validityFlag = v ;
		_validityFlagSet = true ;
		setModifiedFlag() ;
	}
}


bool dbInventoryObject::getValidityFlag(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _validityFlagSet ; }
	return _validityFlag ;
}


void dbInventoryObject::unsetValidityFlag()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_validityFlagSet) {
		setModifiedFlag();
    }

	_validityFlagSet = false;

}



void dbInventoryObject::setValidationDate(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_validationDateSet || (_validationDate != v) ) 
	{
		_validationDate = v ;
		_validationDateSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryObject::getValidationDate(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _validationDateSet ; }
	return _validationDate ;
}


void dbInventoryObject::unsetValidationDate()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_validationDateSet) {
		setModifiedFlag();
    }

	_validationDateSet = false;

}


void dbInventoryObject::setHeaderSize(off_t v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_headerSizeSet || (_headerSize != v) ) 
	{
		_headerSize = v ;
		_headerSizeSet = true ;
		setModifiedFlag() ;
	}
}


off_t dbInventoryObject::getHeaderSize(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _headerSizeSet ; }
	return _headerSize ;
}


void dbInventoryObject::unsetHeaderSize()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_headerSizeSet) {
		setModifiedFlag();
    }

	_headerSizeSet = false;

}


void dbInventoryObject::setDataSize(off_t v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_dataSizeSet || (_dataSize != v) ) 
	{
		_dataSize = v ;
		_dataSizeSet = true ;
		setModifiedFlag() ;
	}
}


off_t dbInventoryObject::getDataSize(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _dataSizeSet ; }
	return _dataSize ;
}


void dbInventoryObject::unsetDataSize()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_dataSizeSet) {
		setModifiedFlag();
    }

	_dataSizeSet = false;

}


void dbInventoryObject::setInventoryDate(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_inventoryDateSet || (_inventoryDate != v) ) 
	{
		_inventoryDate = v ;
		_inventoryDateSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryObject::getInventoryDate(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _inventoryDateSet ; }
	return _inventoryDate ;
}

void dbInventoryObject::unsetInventoryDate()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_inventoryDateSet) { 
        setModifiedFlag(); 
    }

	_inventoryDateSet = false;

}


void dbInventoryObject::setModificationDate(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_modificationDateSet || (_modificationDate != v) ) 
	{
		_modificationDate = v ;
		_modificationDateSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryObject::getModificationDate(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _modificationDateSet ; }
	return _modificationDate ;
}


void dbInventoryObject::unsetModificationDate()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_modificationDateSet) {
		setModifiedFlag();
    }

	_modificationDateSet = false;

}


void dbInventoryObject::setFileType(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 16) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 16); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setFileType(). String too long. Truncated to 16 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_fileTypeSet || (_fileType != v) ) 
	{
		_fileType = v ;
		_fileTypeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getFileType(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _fileTypeSet ; }
	return _fileType ;
}

void dbInventoryObject::unsetFileType()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_fileTypeSet) {
		setModifiedFlag();
    }

	_fileTypeSet = false;

}

void dbInventoryObject::setFileClass(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 128) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 128); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setFileClass(). String too long. Truncated to 128 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_fileClassSet || (_fileClass != v) ) 
	{
		_fileClass = v ;
		_fileClassSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getFileClass(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _fileClassSet ; }
	return _fileClass ;
}

void dbInventoryObject::unsetFileClass()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_fileClassSet) {
		setModifiedFlag();
    }

	_fileClassSet = false;

}

void dbInventoryObject::setOrderId(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_orderIdSet || (_orderId != v) ) 
	{
		_orderId = v ;
		_orderIdSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getOrderId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _orderIdSet ; }
	return _orderId ;
}

void dbInventoryObject::unsetOrderId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_orderIdSet) { 
        setModifiedFlag(); 
    }

	_orderIdSet = false;

}

void dbInventoryObject::setSensorId(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_sensorIdSet || (_sensorId != v) ) 
	{
		_sensorId = v ;
		_sensorIdSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getSensorId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _sensorIdSet ; }
	return _sensorId ;
}


void dbInventoryObject::unsetSensorId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_sensorIdSet) {
		setModifiedFlag();
    }

	_sensorIdSet = false;

}


void dbInventoryObject::setAcquisitionStationId(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_acquisitionStationIdSet || (_acquisitionStationId != v) ) 
	{
		_acquisitionStationId = v ;
		_acquisitionStationIdSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getAcquisitionStationId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _acquisitionStationIdSet ; }
	return _acquisitionStationId ;
}


void dbInventoryObject::unsetAcquisitionStationId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_acquisitionStationIdSet) {
		setModifiedFlag();
    }

	_acquisitionStationIdSet = false;

}


void dbInventoryObject::setProcessingStationId(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_processingStationIdSet || (_processingStationId != v) ) 
	{
		_processingStationId = v ;
		_processingStationIdSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getProcessingStationId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _processingStationIdSet ; }
	return _processingStationId ;
}


void dbInventoryObject::unsetProcessingStationId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_processingStationIdSet) {
		setModifiedFlag();
    }

	_processingStationIdSet = false;

}


void dbInventoryObject::setSensorMode(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050
    
    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 16) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 16); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setSensorMode(). String too long. Truncated to 16 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_sensorModeSet || (_sensorMode != v) ) 
	{
		_sensorMode = v ;
		_sensorModeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getSensorMode(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _sensorModeSet ; }
	return _sensorMode ;
}


void dbInventoryObject::unsetSensorMode()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_sensorModeSet) {
		setModifiedFlag();
    }

	_sensorModeSet = false;

}


void dbInventoryObject::setPhase(const string &a)
{
	Lock lock(*this) ; // PRQA S 3050
    
    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 8) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 8); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setPhase(). String too long. Truncated to 8 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ; 
	}

	if (!_phaseSet || (_phase != v) ) 
	{
		_phase = v ;
		_phaseSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryObject::getPhase(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _phaseSet ; }
	return _phase ;
}


void dbInventoryObject::unsetPhase()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_phaseSet) {
		setModifiedFlag();
    }

	_phaseSet = false;

}


void dbInventoryObject::setSatelliteId(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_satelliteIdSet || (_satelliteId != v) ) 
	{
		_satelliteId = v ;
		_satelliteIdSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getSatelliteId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _satelliteIdSet ; }
	return _satelliteId ;
}

void dbInventoryObject::unsetSatelliteId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_satelliteIdSet) {
		setModifiedFlag();
    }

	_satelliteIdSet = false;

}


void dbInventoryObject::setAscendingFlag(bool v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_ascendingFlagSet || (_ascendingFlag != v) ) 
	{
		_ascendingFlag = v ;
		_ascendingFlagSet = true ;
		setModifiedFlag() ;
	}
}


bool dbInventoryObject::getAscendingFlag(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _ascendingFlagSet ; }
	return _ascendingFlag ;
}

void dbInventoryObject::unsetAscendingFlag()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_ascendingFlagSet) {
		setModifiedFlag();
    }

	_ascendingFlagSet = false;

}


void dbInventoryObject::setEquatorXLongitude(float v)
{
	Lock lock(*this) ; // PRQA S 3050

	_equatorXLongitude = v ;
	_equatorXLongitudeSet = true ;
	setModifiedFlag() ;
}


float dbInventoryObject::getEquatorXLongitude(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _equatorXLongitudeSet ; }
	return _equatorXLongitude ;
}

void dbInventoryObject::unsetEquatorXLongitude()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_equatorXLongitudeSet) {
		setModifiedFlag();
    }

	_equatorXLongitudeSet = false;

}

void dbInventoryObject::setEquatorXTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_equatorXTimeSet || (_equatorXTime != v) ) 
	{
		_equatorXTime = v ;
		_equatorXTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryObject::getEquatorXTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _equatorXTimeSet ; }
	return _equatorXTime ;
}

void dbInventoryObject::unsetEquatorXTime()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_equatorXTimeSet) {
		setModifiedFlag();
    }

	_equatorXTimeSet = false;

}


void dbInventoryObject::setMission(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_missionSet || (_mission != v) ) 
	{
		_mission = v ;
		_missionSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getMission(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _missionSet ; }
	return _mission ;
}


void dbInventoryObject::unsetMission()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_missionSet == true) {
		setModifiedFlag();
    }
	_missionSet = false;

}


void dbInventoryObject::setQualified(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (_qualifiedId != v ) 
	{
		_qualifiedId = v ;
		setModifiedFlag() ;
	}
}

int dbInventoryObject::getQualified(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = true ; }
	return _qualifiedId ;
}



void dbInventoryObject::setOriginator(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_originatorIdSet || (_originatorId != v) ) 
	{
		_originatorId = v ;
		_originatorIdSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getOriginator(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _originatorIdSet ; }
	return _originatorId ;
}

void dbInventoryObject::unsetOriginator()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_originatorIdSet == true) {
		setModifiedFlag();
    }
	_originatorIdSet = false;

}

void dbInventoryObject::setQualifierFile(const std::string & v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_qualifierFileSet || (_qualifierFile != v) ) 
	{
		_qualifierFile = v ;
		_qualifierFileSet = true ;
		setModifiedFlag() ;
	}
}

const string & dbInventoryObject::getQualifierFile(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _qualifierFileSet ;
    }
	return _qualifierFile ;
}

void dbInventoryObject::unsetQualifierFile()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_qualifierFileSet == true) {
		setModifiedFlag();
    }
	_qualifierFileSet = false;

}

void dbInventoryObject::setBand(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_bandSet || (_band != v) ) 
	{
		_band = v ;
		_bandSet = true ;
		setModifiedFlag() ;
	}
}


int dbInventoryObject::getBand(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _bandSet ;
    }
	return _band ;
}

void dbInventoryObject::unsetBand()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_bandSet == true) {
		setModifiedFlag();
    }
	_bandSet = false;

}



void dbInventoryObject::setTdi(short v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_tdiSet || (_tdi != v) ) 
	{
		_tdi = v ;
		_tdiSet = true ;
		setModifiedFlag() ;
	}
}


short dbInventoryObject::getTdi(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _tdiSet ;
    }
	return _tdi ;
}

void dbInventoryObject::unsetTdi()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_tdiSet == true) {
		setModifiedFlag();
    }
	_tdiSet = false;

}


void dbInventoryObject::setCloudPctg(float v)
{
	Lock lock(*this) ; // PRQA S 3050

	_cloudpctg = v ;
	_cloudpctgSet = true ;
	setModifiedFlag() ;
}


float dbInventoryObject::getCloudPctg(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _cloudpctgSet ;
    }
	return _cloudpctg ;
}

void dbInventoryObject::unsetCloudPctg()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_cloudpctgSet == true) {
		setModifiedFlag();
    }
	_cloudpctgSet = false;

}


void dbInventoryObject::setProdSpec(const string & a)
{

	Lock lock(*this) ; // PRQA S 3050

    // allow the parameter changes with a non const local variable
    string v = a;

	if (v.size() > 128) // PRQA S 4400
	{
        // Truncate and notify the issue
        v = v.substr (0, 128); // PRQA S 4400
		InvalidUse e("Error: dbInventoryObject::setProdSpec(). String too long. Truncated to 128 chars.") ;
		ACS_LOG_TRYCATCHNOTIFY_EX(e) ;
	}

	if (!_prodspecSet || (_prodspec != v) ) 
	{
		_prodspec = v ;
		_prodspecSet = true ;
		setModifiedFlag() ;
	}
}


const string & dbInventoryObject::getProdSpec(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _prodspecSet ;
    }
	return _prodspec ;
}

void dbInventoryObject::unsetProdSpec()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_prodspecSet == true) {
		setModifiedFlag();
    }
	_prodspecSet = false;

}

void dbInventoryObject::setDirId(IdType v) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_DirIdSet || (_DirId != v) ) 
	{
		_DirId = v ;
		_DirIdSet = true ;
		setModifiedFlag() ;
	}

}

dbPersistent::IdType dbInventoryObject::getDirId(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _DirIdSet ;
    }
	return _DirId ;
}

void dbInventoryObject::unsetDirId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_DirIdSet) {
		setModifiedFlag();
    }

	_DirIdSet = false;
}




void dbInventoryObject::setRefDirId(IdType v) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_RefDirIdSet || (_RefDirId != v) ) 
	{
		_RefDirId = v ;
		_RefDirIdSet = true ;
		setModifiedFlag() ;
	}

}

dbPersistent::IdType dbInventoryObject::getRefDirId(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _RefDirIdSet ;
    }
	return _RefDirId ;
}

void dbInventoryObject::unsetRefDirId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_RefDirIdSet) {
		setModifiedFlag();
    }

	_RefDirIdSet = false;
}


void dbInventoryObject::setUniqueId(const std::string &v) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_uniqueIdSet || (_uniqueId != v) ) 
	{
		_uniqueId = v ;
		_uniqueIdSet = true ;
		setModifiedFlag() ;
	}

}


const std::string &dbInventoryObject::getUniqueId(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _uniqueIdSet ;
    }
	return _uniqueId ;
}


void dbInventoryObject::unsetUniqueId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_uniqueIdSet) {
		setModifiedFlag();
    }

	_uniqueIdSet = false;
}


void dbInventoryObject::setLogicalId(const std::string &v) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_logicalIdSet || (_logicalId != v) ) 
	{
		_logicalId = v ;
		_logicalIdSet = true ;
		setModifiedFlag() ;
	}
}


const std::string &dbInventoryObject::getLogicalId(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _logicalIdSet ;
    }
	return _logicalId ;
}


void dbInventoryObject::unsetLogicalId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_logicalIdSet) {
		setModifiedFlag();
    }

	_logicalIdSet = false;
}


void dbInventoryObject::setQualityValue (float q) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_qualityValueSet || (fabsf(_qualityValue - q) > 1e-7) ) // PRQA S 4400
	{
		_qualityValue = q ;
		_qualityValueSet = true ;
		setModifiedFlag() ;
	}
}


float dbInventoryObject::getQualityValue(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _qualityValueSet ;
    }
	return _qualityValue ;
}


void dbInventoryObject::unsetQualityValue()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_qualityValueSet) {
		setModifiedFlag();
    }

	_qualityValueSet = false;
}

void dbInventoryObject::setParentId(const std::string &v) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_parentIdSet || (_parentId != v) ) 
	{
		_parentId = v ;
		_parentIdSet = true ;
		setModifiedFlag() ;
	}
}


const std::string &dbInventoryObject::getParentId(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _parentIdSet ;
    }
	return _parentId ;
}


void dbInventoryObject::unsetParentId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_parentIdSet) {
		setModifiedFlag();
    }

	_parentIdSet = false;
}

//*/
void dbInventoryObject::setGroupId(const std::string &v) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_groupIdSet || (_groupId != v) ) 
	{
		_groupId = v ;
		_groupIdSet = true ;
		setModifiedFlag() ;
	}
}


const std::string &dbInventoryObject::getGroupId(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _groupIdSet ;
    }
	return _groupId ;
}


void dbInventoryObject::unsetGroupId()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_groupIdSet) {
		setModifiedFlag();
    }

	_groupIdSet = false;
}


void dbInventoryObject::setRelation(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_relationSet || (_relation != v) ) 
	{
		_relation = v ;
		_relationSet = true ;
		setModifiedFlag() ;
	}
	
}

const string & dbInventoryObject::getRelation(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _relationSet ;
    }
	return _relation ;
}

void dbInventoryObject::unsetRelation()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_relationSet) {
		setModifiedFlag();
    }

	_relationSet = false;
}


void dbInventoryObject::setRelationType(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_relationTypeSet || (_relationType != v) ) 
	{
		_relationType = v ;
		_relationTypeSet = true ;
		setModifiedFlag() ;
	}
	
}

const string & dbInventoryObject::getRelationType(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _relationTypeSet ;
    }
	return _relationType ;
}

void dbInventoryObject::unsetRelationType()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_relationTypeSet) {
		setModifiedFlag();
    }

	_relationTypeSet = false;
}

void dbInventoryObject::setMd5(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_md5Set || (_md5 != v) ) 
	{
		_md5 = v ;
		_md5Set = true ;
		setModifiedFlag() ;
	}
	
}

const string & dbInventoryObject::getMd5(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _md5Set ;
    }
	return _md5 ;
}

void dbInventoryObject::unsetMd5()
{
	Lock lock(*this) ; // PRQA S 3050

	if(_md5Set) {
		setModifiedFlag();
    }

	_md5Set = false;
}


string dbInventoryObject::getFields()
{
	return getFields(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
}

string dbInventoryObject::getFields(dbConnection& conn)
{
	deque <string> stringArray = getFieldArray(conn);
	return dbInventoryObject::convertFieldList (stringArray);
}

string dbInventoryObject::convertFieldList (deque <string> const & stringArray)
{
	string retString(stringArray.at(0));
	for (uint i = 1; i < stringArray.size(); ++i)
	{
		retString+=", " + stringArray.at(i);
	}
	return retString;
}

// deque <string> dbInventoryObject::getFieldArray()
// {
//     return getFieldArray(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
// }

deque<string> dbInventoryObject::getFieldArray(dbConnection& conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	deque<string> s ;
    const string  dbKey = conn.getKey() ;
    if (hasSerialId(conn) ) {
	    s.push_back(getTable() + "." + getIdColumnName() +" ");
    }
    
	s.push_back(getTable() + "." + "FileName");
	s.push_back(getTable() + "." + "FileVersion");
    if (hasSerialId(conn) ) {
	    s.push_back(getTable() + "." + "unique_id");
	    s.push_back(getTable() + "." + "logical_id");
    }
    if (hasQualityValue(conn) )
    {
	    s.push_back(getTable() + "." + "qualityvalue");
    }
	s.push_back(getTable() + "." + "Source");
	s.push_back(getTable() + "." + "SourceSwVersion");
	s.push_back(getTable() + "." + "GenerationTime");
	s.push_back(getTable() + "." + "ValidityStart");
	s.push_back(getTable() + "." + "ValidityStop");
	s.push_back(getTable() + "." + "StartOrbitNumber");
	s.push_back(getTable() + "." + "StopOrbitNumber");
	s.push_back(SQLDialect::convertColumnToString(dbKey, getTable() + "." + "GeographicLocalization", dbSchemaMetaDataProvider::instance(dbKey)->getFieldType (getTable(), "GeographicLocalization", conn) ) );
	s.push_back(getTable() + "." + "QualityInfo");
	s.push_back(getTable() + "." + "ValidityFlag");
	s.push_back(getTable() + "." + "ValidationDate");
	s.push_back(getTable() + "." + "HeaderSize");
	s.push_back(getTable() + "." + "DataSize");
	s.push_back(getTable() + "." + "InventoryDate");
	s.push_back(getTable() + "." + "ModificationDate");
	s.push_back(getTable() + "." + "FileType");
	s.push_back(getTable() + "." + "FileClass");
	s.push_back(getTable() + "." + "OrderId");
	s.push_back(getTable() + "." + "SensorId");
	s.push_back(getTable() + "." + "AcquisitionStationId");
	s.push_back(getTable() + "." + "ProcessingStationId");
	s.push_back(getTable() + "." + "SensorMode");
	s.push_back(getTable() + "." + "Phase");
	s.push_back(getTable() + "." + "SatelliteId");
	s.push_back(getTable() + "." + "AscendingFlag");
	s.push_back(getTable() + "." + "EquatorXLongitude");
	s.push_back(getTable() + "." + "EquatorXTime");
	s.push_back(getTable() + "." + "Mission") ;
	s.push_back(getTable() + "." + "Qualified") ;
	s.push_back(getTable() + "." + "Originator") ;
	s.push_back(getTable() + "." + "QualifierFile") ;
	s.push_back(getTable() + "." + "Band") ;
	s.push_back(getTable() + "." + "Tdi") ;
	s.push_back(getTable() + "." + "cloud_pctg") ;
	s.push_back(getTable() + "." + "prodspec");
	s.push_back(getTable() + "." + "dir_id"); 
	s.push_back(getTable() + "." + "ref_dir_id");
    if (hasMd5(conn) )
    {
	    s.push_back(getTable() + "." + "md5");
    }

	SQLString sqlString ( dbKey, SQLString::SELECT ) ;
	sqlString.addTableList("t_inv_relations") ;
    sqlString.addSelectList( "inv_id2" );
    {
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, getTable(), "id_inv", dbParam::EQUAL_TO, "t_inv_relations.inv_id1") ); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
		
		{
			SQLString inStr ( dbKey, SQLString::SELECT ) ;
			inStr.addTableList("t_inv_relation_types") ;
    		inStr.addSelectList( "id" );
    		{
	    		dbQueryParameters pi(dbKey);
	    		pi.addParameter( dbParam(dbKey, "t_inv_relation_types", "relation_type", dbParam::EQUAL_TO, db::toSQL(dbKey,"CHILD_TO_PARENT"))); // PRQA S 3050, 3081
	    		inStr.addANDWhereParameter(pi);
    		}  
	    	dbQueryParameters pi(dbKey);
	    	pi.addParameter( dbParam(dbKey, "t_inv_relations", "rel_type_id", dbParam::EQUAL_TO, inStr.getSQL(true)) ); // PRQA S 3050, 3081
	    	sqlString.addANDWhereParameter(pi);
		
		}
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parent query: " << sqlString.getSQL() );
	s.push_back(sqlString.getSQL(true)+ "AS parent");
	
	return s ;
}
string dbInventoryObject::getIdColumnName() 
{
    return "id_inv" ;
}


// get the table name
string dbInventoryObject::getTable()
{
	return "T_Inventory" ;
}


// get the order by clause
string dbInventoryObject::getOrderBy()
{
#warning MP: using empty string as db key

    const string dbKey = "" ;
  return SQLString::getOrderByString(dbKey, "FileName") 
        + SQLString::getSeparator(dbKey) 
        + SQLString::getOrderByString(dbKey, "FileVersion", SQLString::DESC) ;
}


void dbInventoryObject::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

    using db::toSQL ;
	string tmp ; 
    const string K = "" ;

    bool ser = hasSerialId(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
    if (ser ) {
        o << "id: " << getId() << '\n' ;
    }
    // PRQA S 3380, 3385 L4
	o
         << "_fileName = " << (_fileNameSet ? _fileName : "<null>") << '\n' 
		 << "_fileVersion = " << (_fileVersionSet ? _fileVersion : "<null>") << '\n' ;
    if (ser )
    {
        o << "_uniqueId = " << (_uniqueIdSet ? _uniqueId : "<null>") << '\n' 
         << "_logicalId = " << (_logicalIdSet ? _logicalId : "<null>") << '\n' 
         << "_parentId = " << (_parentIdSet ? _parentId : "<null>") << '\n' 
         << "_groupId = " << (_groupIdSet ? _groupId : "<null>") << '\n' ;
    }
    if (hasQualityValue(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) // PRQA S 3050, 3081, 4412
    {
        o << "_qualityValue = " << ( _qualityValueSet ? toSQL(K, _qualityValue) : "<null>") << '\n';
    }
	if (hasMd5(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) // PRQA S 3050, 3081, 4412
    {
        o << "_md5 = " << ( _md5Set ? toSQL(K, _md5) : "<null>") << '\n';
    }

    
    o     
		 << "_source = " << (_sourceSet ? _source : "<null>") << '\n' 
		 << "_sourceSwVersion = " << (_sourceSwVersionSet ? _sourceSwVersion : "<null>") << '\n' 
		 << "_generationTime = " << (_generationTimeSet ? _generationTime.utcs(tmp) : "<null>") << '\n' 
		 << "_validityStart = " << (_validityStartSet ? _validityStart.utcs(tmp) : "<null>") << '\n' 
		 << "_validityStop = " << (_validityStopSet ? _validityStop.utcs(tmp) : "<null>") << '\n' 
		 << "_startOrbitNumber = " << (_startOrbitNumberSet ? toSQL(K, _startOrbitNumber) : "<null>") << '\n' 
		 << "_stopOrbitNumber = " << (_stopOrbitNumberSet ? toSQL(K, _stopOrbitNumber) : "<null>") << '\n' 
		 << "_geographicLocalization = " << (_geographicLocalization ? "Geo Object " + dbGeoObject::type2String(_geographicLocalization->type()) : "<null>") << '\n' 
		 << "_qualityInfo = " << (_qualityInfoSet ? _qualityInfo : "<null>") << '\n' 
		 << "_validityFlag = " << (_validityFlagSet ? (_validityFlag ? "True" : "False") : "<null>") << '\n' 
		 << "_validationDate = " << (_validationDateSet ? _validationDate.utcs(tmp) : "<null>") << '\n' 
		 << "_headerSize = " << (_headerSizeSet ? toSQL(K, _headerSize) : "<null>") << '\n' 
		 << "_dataSize = " << (_dataSizeSet ? toSQL(K, _dataSize) : "<null>") << '\n' 
		 << "_inventoryDate = " << (_inventoryDateSet ? _inventoryDate.utcs(tmp) : "<null>") << '\n' 
		 << "_modificationDate = " << (_modificationDateSet ? _modificationDate.utcs(tmp) : "<null>") << '\n' 
		 << "_fileType = " << (_fileTypeSet ? _fileType : "<null>") << '\n' 
		 << "_fileClass = " << (_fileClassSet ? _fileClass : "<null>") << '\n' 
		 << "_orderId = " << (_orderIdSet ? toSQL(K, _orderId) : "<null>") << '\n' 
		 << "_sensorId = " << (_sensorIdSet ? toSQL(K, _sensorId) : "<null>") << '\n' 
		 << "_acquisitionStationId = " << (_acquisitionStationIdSet ? toSQL(K, _acquisitionStationId) : "<null>") << '\n' 
		 << "_processingStationId = " << (_processingStationIdSet ? toSQL(K, _processingStationId) : "<null>") << '\n' 
		 << "_sensorMode = " << (_sensorModeSet ? _sensorMode : "<null>") << '\n' 
		 << "_phase = " << (_phaseSet ? _phase : "<null>") << '\n' 
		 << "_satelliteId = " << (_satelliteIdSet ? toSQL(K, _satelliteId) : "<null>") << '\n' 
		 << "_ascendingFlag = " << (_ascendingFlagSet ? (_ascendingFlag ? "True" : "False") : "<null>") << '\n' 
		 << "_equatorXLongitude = " << (_equatorXLongitudeSet ? toSQL(K, _equatorXLongitude) : "<null>") << '\n' 
		 << "_equatorXTime = " << (_equatorXTimeSet ? _equatorXTime.utcs(tmp) : "<null>") << '\n' 
		 << "_mission = " << (_missionSet ? toSQL(K, _mission) : "<null>") << '\n' 
		 << "_qualifiedId = " << toSQL(K, _qualifiedId) << '\n'
		 << "_originatorId = " << (_originatorIdSet ? toSQL(K, _originatorId) : "<null>") << '\n'
		 << "_qualifierFile = " << (_qualifierFileSet ? toSQL(K, _qualifierFile) : "<null>") << '\n'
		 << "_band = " << (_bandSet ? toSQL(K, _band) : "<null>") << '\n'
		 << "_tdi = " << (_tdiSet ? toSQL(K, _tdi) : "<null>") << '\n' 
		 << "_cloudpctg = " << (_cloudpctgSet ? toSQL(K, _cloudpctg) : "<null>") << '\n' 
		 << "_prodspec = " << (_prodspecSet ? toSQL(K, _prodspec) : "<null>") << '\n' 
		 << "_DirId = " << (_DirIdSet ? toSQL(K, _DirId) : "<null>") << '\n'  
		 << "_RefDirId = " << (_RefDirIdSet ? toSQL(K, _RefDirId) : "<null>") << '\n' 
		 << "_parentId = " << (_parentIdSet ? toSQL(K, _parentId) : "<null>") << '\n' ; 
		 bool s = false ; 
         string r; 
		 r=guessCacheNameNoInvDate(&s); o << "guessCacheNameNoInvDate = " << (s? r: "<null>") << '\n' ; 
		 r=guessCacheName(&s); o << "guessCacheName = " << (s? r: "<null>") << '\n' ; 
    // PRQA L:L4
}



void dbInventoryObject::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050
    using db::toSQL ;
	string tmp ; 
    const string K = "" ;
    bool ser = hasSerialId(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412

    if (ser ) {
        o << "id: " << getId() << '\n' ;
    }
    // PRQA S 3380, 3385 L5
	o
         << "_fileName = " << (_fileNameSet ? _fileName : "<null>") << '\n' 
		 << "_fileVersion = " << (_fileVersionSet ? _fileVersion : "<null>") << '\n' ;
    if (ser )
    {
        o << "_uniqueId = " << (_uniqueIdSet ? _uniqueId : "<null>") << '\n' 
         << "_logicalId = " << (_logicalIdSet ? _logicalId : "<null>") << '\n' 
         << "_parentId = " << (_parentIdSet ? _parentId : "<null>") << '\n' 
         << "_groupId = " << (_groupIdSet ? _groupId : "<null>") << '\n' ;
    }
    if (hasQualityValue(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) // PRQA S 3050, 3081, 4412
    {
        o << "_qualityValue = " << ( _qualityValueSet ? toSQL(K, _qualityValue) : "<null>") << '\n';
    }
	if (hasMd5(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ) // PRQA S 3050, 3081, 4412
    {
        o << "_md5 = " << ( _md5Set ? toSQL(K, _md5) : "<null>") << '\n';
    }
    
    o     
		 << "_source = " << (_sourceSet ? _source : "<null>") << '\n' 
		 << "_sourceSwVersion = " << (_sourceSwVersionSet ? _sourceSwVersion : "<null>") << '\n' 
		 << "_generationTime = " << (_generationTimeSet ? _generationTime.utcs(tmp) : "<null>") << '\n' 
		 << "_validityStart = " << (_validityStartSet ? _validityStart.utcs(tmp) : "<null>") << '\n' 
		 << "_validityStop = " << (_validityStopSet ? _validityStop.utcs(tmp) : "<null>") << '\n' 
		 << "_startOrbitNumber = " << (_startOrbitNumberSet ? toSQL(K, _startOrbitNumber) : "<null>") << '\n' 
		 << "_stopOrbitNumber = " << (_stopOrbitNumberSet ? toSQL(K, _stopOrbitNumber) : "<null>") << '\n' 
		 << "_geographicLocalization = " << (_geographicLocalization ? "Geo Object " + dbGeoObject::type2String(_geographicLocalization->type()) : "<null>") << '\n' 
		 << "_qualityInfo = " << (_qualityInfoSet ? _qualityInfo : "<null>") << '\n' 
		 << "_validityFlag = " << (_validityFlagSet ? (_validityFlag ? "True" : "False") : "<null>") << '\n' 
		 << "_validationDate = " << (_validationDateSet ? _validationDate.utcs(tmp) : "<null>") << '\n' 
		 << "_headerSize = " << (_headerSizeSet ? toSQL(K, _headerSize) : "<null>") << '\n' 
		 << "_dataSize = " << (_dataSizeSet ? toSQL(K, _dataSize) : "<null>") << '\n' 
		 << "_inventoryDate = " << (_inventoryDateSet ? _inventoryDate.utcs(tmp) : "<null>") << '\n' 
		 << "_modificationDate = " << (_modificationDateSet ? _modificationDate.utcs(tmp) : "<null>") << '\n' 
		 << "_fileType = " << (_fileTypeSet ? _fileType : "<null>") << '\n' 
		 << "_fileClass = " << (_fileClassSet ? _fileClass : "<null>") << '\n' 
		 << "_orderId = " << (_orderIdSet ? toSQL(K, _orderId) : "<null>") << '\n' 
		 << "_sensorId = " << (_sensorIdSet ? toSQL(K, _sensorId) : "<null>") << '\n' 
		 << "_acquisitionStationId = " << (_acquisitionStationIdSet ? toSQL(K, _acquisitionStationId) : "<null>") << '\n' 
		 << "_processingStationId = " << (_processingStationIdSet ? toSQL(K, _processingStationId) : "<null>") << '\n' 
		 << "_sensorMode = " << (_sensorModeSet ? _sensorMode : "<null>") << '\n' 
		 << "_phase = " << (_phaseSet ? _phase : "<null>") << '\n' 
		 << "_satelliteId = " << (_satelliteIdSet ? toSQL(K, _satelliteId) : "<null>") << '\n' 
		 << "_ascendingFlag = " << (_ascendingFlagSet ? (_ascendingFlag ? "True" : "False") : "<null>") << '\n' 
		 << "_equatorXLongitude = " << (_equatorXLongitudeSet ? toSQL(K, _equatorXLongitude) : "<null>") << '\n' 
		 << "_equatorXTime = " << (_equatorXTimeSet ? _equatorXTime.utcs(tmp) : "<null>") << '\n' 
		 << "_mission = " << (_missionSet ? toSQL(K, _mission) : "<null>") << '\n' 
		 << "_qualifiedId = " << toSQL(K, _qualifiedId) << '\n'
		 << "_originatorId = " << (_originatorIdSet ? toSQL(K, _originatorId) : "<null>") << '\n'
		 << "_qualifierFile = " << (_qualifierFileSet ? toSQL(K, _qualifierFile) : "<null>") << '\n'
		 << "_band = " << (_bandSet ? toSQL(K, _band) : "<null>") << '\n'
		 << "_tdi = " << (_tdiSet ? toSQL(K, _tdi) : "<null>") << '\n' 
		 << "_cloudpctg = " << (_cloudpctgSet ? toSQL(K, _cloudpctg) : "<null>") << '\n' 
		 << "_prodspec = " << (_prodspecSet ? toSQL(K, _prodspec) : "<null>") << '\n' 
		 << "_DirId = " << (_DirIdSet ? toSQL(K, _DirId) : "<null>") << '\n'  
		 << "_RefDirId = " << (_RefDirIdSet ? toSQL(K, _RefDirId) : "<null>") << '\n' 
		 << "_parentId = " << (_parentIdSet ? toSQL(K, _parentId) : "<null>") << '\n' ;
		 bool s = false ; 
         string r; 
		 r=guessCacheNameNoInvDate(&s); o << "guessCacheNameNoInvDate = " << (s? r: "<null>") << '\n' ; 
		 r=guessCacheName(&s); o << "guessCacheName = " << (s? r: "<null>") << '\n' ; 
    // PRQA L:L5
}


unsigned int 
dbInventoryObject::executeCountQuery (const std::string& theQuery, dbConnection& conn, const string &what) // PRQA S 4211
{
	Lock lock(*this) ; // PRQA S 3050


	// and execute it
	// Prepare the SQLstring
	SQLString queryString (conn.getKey(), SQLString::COMMAND);
	queryString.forceSQLString (theQuery);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing the query [" << theQuery << ']' ) ;
		
	dbQuery query(conn, queryString,"InventoryCount"+what) ;
	dbRow row(conn.getKey()) ;

	try
	{
        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
	}
	catch (exception &ex)
	{
		ACS_THROW(SyncDetectionError(ex, "Query execution error")); // PRQA S 3081
	}
	
	int count = 0 ;
	bool isSet = false ;
	row.columnSet (0, count, isSet);

	if (!isSet)
	{
		// Why should it happen ?
		ACS_THROW(SyncDetectionError("Unable to retrieve the count value from the query ["+theQuery+"]")); // PRQA S 3081
	}
	
	return count; // PRQA S 3000
}

bool dbInventoryObject::isSyncDownloadAvailable () { // PRQA S 4020
    return isSyncDownloadAvailable(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
}

bool dbInventoryObject::isSyncDownloadAvailable (dbConnection& conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking the file[" << _fileName << ", "  << _fileVersion << ']') ;

	ACS_COND_THROW(!_fileNameSet,InvalidUse("Mandatory field \"File Name\" must be set.")) ; // PRQA S 3081 2
	ACS_COND_THROW(!_fileVersionSet,InvalidUse("Mandatory field \"File Version\" must be set.")) ;

	if (_syncDownloadAvailableSet)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_isSyncDownloadAvailable already set") ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning " << boolalpha << _syncDownloadAvailable) ;

		return _syncDownloadAvailable ;
	}

    const string &dbKey = conn.getKey() ;

	// Synchronous sources query
	boost::format syncQuery (
		"SELECT COUNT(*) "
		"FROM "
		"t_inventorysm INNER JOIN t_storagemanager "
		"ON "
		"(t_inventorysm.storagemanid = t_storagemanager.id) "
		"WHERE "
		"t_storagemanager.syncDownload = %s AND "
		"t_inventorysm.inv_id = %s");


	// Insert the TRUE boolean
	syncQuery % db::toSQL(dbKey, true);

	// Replace the filename and the fileversion 
	syncQuery % db::toSQL(dbKey, getId()) ;

	unsigned int syncCount = 0;

	try
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking storages") ;

		syncCount = executeCountQuery (syncQuery.str(), conn, "SyncSM");
	}
	catch (exception& ex)
	{
		// Unable to retrieve sync storages information
		// assume no sync storage available
		ACS_LOG_NOTIFY_EX(ex);
		syncCount = 0;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[BLOB, Storages] counters are [" << syncCount << "]") ;

	_syncDownloadAvailable = (syncCount>0);
	_syncDownloadAvailableSet = true;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning " << boolalpha << _syncDownloadAvailable ) ;

	return _syncDownloadAvailable;
}

bool dbInventoryObject::hasSerialId(dbConnection &conn) 
{
    if (!_schemaChecked) { checkSchema(conn) ; }
    return _hasSerialId ;
}

bool dbInventoryObject::hasBaseline (dbConnection &conn)
{
    if (!_schemaChecked) { checkSchema(conn) ; }
    return _hasBaseline ;
}

bool dbInventoryObject::hasQualityValue (dbConnection &conn)
{
    if (!_schemaChecked) { checkSchema(conn) ; }
    return _hasQualityValue ;
}

bool dbInventoryObject::hasMd5 (dbConnection &conn)
{
    if (!_schemaChecked) { checkSchema(conn) ; }
    return _hasMd5 ;
}


bool dbInventoryObject::checkSchema(dbConnection &conn) 
{
    const string &dbKey = conn.getKey() ;
    _hasSerialId = dbSchemaMetaDataProvider::instance(dbKey)->hasFieldWithName(getTable(),getIdColumnName(),conn) ;
    _hasBaseline = dbSchemaMetaDataProvider::existTable("t_baselines_x_inventories", conn) ;
    _hasQualityValue = dbSchemaMetaDataProvider::instance(dbKey)->hasFieldWithName(getTable(),"QualityValue",conn) ;
	_hasMd5 = dbSchemaMetaDataProvider::instance(dbKey)->hasFieldWithName(getTable(),"md5",conn) ;
    _schemaChecked = true ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_hasSerialId: " << boolalpha << _hasSerialId << " | _hasBaseline: " << boolalpha << _hasBaseline
            << " | _hasQualityValue: " << boolalpha << _hasQualityValue << " | _hasMd5: " << boolalpha << _hasMd5 << " | _schemaChecked: " << boolalpha << _schemaChecked) ;
    return _schemaChecked ; 
}



_ACS_END_NAMESPACE

