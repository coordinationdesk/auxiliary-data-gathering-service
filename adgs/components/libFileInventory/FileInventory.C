// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: 

	$Id$

	$Author$

	$Log$
	Revision 5.45  2018/01/30 13:55:19  lucio.pulvirenti
	After request of S3MPM, if LIST file is empty just issue a warning and remove both LIST and lock files.
	
	Revision 5.44  2017/11/15 10:30:21  chicas
	some message to PRIVINFO
	
	Revision 5.43  2017/09/11 12:41:50  marfav
	Added support to empty backupDir setting, that will not move anything
	
	Revision 5.42  2017/09/04 09:52:26  lucio.pulvirenti
	ECICMF-108: InvBaseline removed.
	
	Revision 5.41  2017/07/13 15:32:50  lucio.pulvirenti
	ECICMF-108: If InventoryBaseline metadata field occurs, dbInventoryObject _invBaseline attr set.
	
	Revision 5.40  2017/05/12 10:13:13  lucio.pulvirenti
	PRS-41: Both Relation and RelationType managed.
	
	Revision 5.39  2016/12/07 09:57:01  marfav
	Releasing the connection as soon as possible to
	limit multiple connection usage
	
	Revision 5.38  2016/10/07 12:59:02  marpas
	Originator moved in OriginatorName
	
	Revision 5.37  2016/05/11 15:19:28  lucio.pulvirenti
	PDSEV-34: DeleteUploadedFilepathsOnSuccess configuration flag turned into DeleteUploadOnSuccess: the shorter the better for configuration.
	
	Revision 5.36  2016/05/10 15:22:52  lucio.pulvirenti
	PDSEV-34: Boolean flag, default false, added to IF to remove path of uploaded files on success. Set by configuration. If set, deletion of file performed
	no more by inventory.
	
	Revision 5.35  2016/04/18 15:25:40  marpas
	some NameIdAssociators adhere to the standards now - they need a db key
	
	Revision 5.34  2016/04/06 09:53:54  marpas
	getting rid of libDbCatalog
	
	Revision 5.33  2016/03/26 21:39:21  marpas
	using new db I/F
	
	Revision 5.32  2016/03/14 12:18:06  marpas
	qa warning fixed
	
	Revision 5.31  2016/02/01 14:55:36  marfav
	Email notification is called only if t_cr_x_filetypes schema is installed
	
	Revision 5.30  2016/01/22 15:40:53  lucio.pulvirenti
	S2PDGS-1402: if option -replace is set, id of t_inventory record to update is passed to dbInventoryObjetcTEC::setId method.
	If moveFileInBackupDir called in catch block, put inside an inner try block, to prevent possibile exception to mask principle error cause.
	
	Revision 5.29  2015/12/15 15:08:59  marfav
	S3MPM-659 replacefile function fixed
	
	Revision 5.28  2015/10/28 13:42:24  damdec
	PDSEV-16: handling of blob in t_inventory removed.
	
	Revision 5.27  2015/10/23 10:30:53  marpas
	moving some info filterables into private info group
	
	Revision 5.26  2015/08/26 09:13:58  marfav
	MC text messages reverted to simple filterables
	
	Revision 5.25  2015/05/26 15:28:00  marpas
	useless warnings emitted only in debug
	
	Revision 5.24  2015/05/25 13:45:05  marpas
	coding best practice applied
	
	Revision 5.23  2015/03/13 17:50:59  marfav
	S2PDGS-1137 Added support to bulk extra parameters and quality value
	
	Revision 5.22  2015/02/26 16:58:22  marfav
	Baseline managed as Id
	
	Revision 5.21  2015/02/20 16:22:25  marfav
	APF-256 introducing baseline metadata
	
	Revision 5.20  2015/02/04 14:58:02  marfav
	S2PDGS-1072 adding support to options noactions and restoreonsync
	
	Revision 5.19  2015/02/02 16:08:42  marfav
	S2PDGS-1072 upload without triggers implemented
	
	Revision 5.18  2015/01/30 17:22:49  marfav
	S2PDGS-976 managing restore on sync storage managers
	
	Revision 5.17  2014/11/14 18:00:30  marpas
	the move to the backup directory is done with an hardlink or a copy (if the path are on different filesystems)
	
	Revision 5.16  2014/05/08 11:00:56  lucio.pulvirenti
	S2PDGS-664: if geographic coords precision set in metadata, set precision in dbGeoObject.
	
	Revision 5.15  2014/02/11 11:03:44  francesco.avanzi
	dbConv moved to db::toSQL
	
	Revision 5.14  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.13  2014/01/08 13:02:37  lucio.pulvirenti
	Bug fixed: parameter in query on t_inventory is the value of metadata File_Name field instead of the filename to inventory.
	In fact they could be different especially in the case where managesngext=true for the current file type.
	
	Revision 5.12  2013/12/17 11:47:04  marfav
	S1PDGS-31336: in case the file is already in database a filterable is issued to notify also in syslog
	
	Revision 5.11  2013/11/29 16:25:48  marfav
	Warning text on log delay and anomalies table aligned
	
	Revision 5.10  2013/10/24 14:18:16  marpas
	query statistics upgraded
	
	Revision 5.9  2013/10/23 15:27:40  marfav
	Added support to delay notification in inventory anomalies
	
	Revision 5.8  2013/10/09 17:05:15  marpas
	some statistics added
	
	Revision 5.7  2013/10/01 17:06:53  chicas
	removed ACS_LOG_NOTIFY_EX if jms resource value not found
	
	Revision 5.6  2013/09/24 15:28:58  marfav
	Known extensions management removed (warns in log when attempt to use it)
	
	Revision 5.5  2013/09/09 14:08:44  marfav
	OCADA periodicity check code replaced by ADC code
	
	Revision 5.4  2013/08/30 15:19:32  marfav
	Replacing OCADA timeliness check with coverage rules
	
	Revision 5.3  2013/07/23 17:21:48  marpas
	compilation warnings removed
	
	Revision 5.2  2013/07/15 16:31:31  marpas
	compilation warnings fixed
	
	Revision 5.1  2013/07/04 16:33:37  marpas
	debug improved: setting FileInventory_DEBUG to any value grater than 0 will leave in tmp a .xml file with the metadata.
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.71  2013/06/07 16:56:33  nicvac
	S2PDGS-308: Inventory notifications fixed.
	
	Revision 2.70  2013/05/29 15:24:40  marpas
	No static variable can invoke Singleton when destroying. Tehy could already have been destroyed
	
	Revision 2.69  2013/05/24 18:28:40  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 2.68  2013/05/09 13:38:24  marpas
	fixing getArraySize calls for points, sites, gaps and comments
	fixing test compilation (almost all)
	Some debug more
	
	Revision 2.67  2013/04/12 12:17:19  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.66  2013/04/05 09:37:21  marpas
	NameIdAssociator partial refactoring
	
	Revision 2.65  2013/03/12 17:50:49  marpas
	using dbConnectionPool
	
	Revision 2.64  2013/02/21 19:25:15  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.63  2012/11/28 12:06:09  chicas
	Added Parent_Id and Group_Id tag
	
	Revision 2.62  2012/08/02 16:37:32  marpas
	refactoring and new test
	
	Revision 2.61  2012/05/14 07:44:57  chicas
	Added Logical_Id field
	
	Revision 2.60  2012/03/28 13:47:38  marpas
	added Point case for geolocalization
	
	Revision 2.59  2012/03/15 11:11:33  marfav
	Query fixed when checking existing files
	
	Revision 2.58  2012/03/12 17:51:11  marpas
	fixing already present blob when --overwrite passed
	
	Revision 2.57  2012/03/12 17:26:00  marpas
	fixed regression on Inventory options
	
	Revision 2.56  2012/03/12 16:21:31  marpas
	fixed regression about already present inventory
	
	Revision 2.55  2012/03/06 15:54:04  marpas
	messages improved and standardized
	
	Revision 2.54  2012/03/06 13:28:05  marpas
	changes in progress
	
	Revision 2.53  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.52  2011/07/15 12:26:10  marpas
	when creating TreeScopeGuard, in case _relaxFnCheck is true, skip files not in metadata
	
	Revision 2.51  2011/07/15 09:49:09  marpas
	when _relaxFnCheck, during metadata validation, files in the disk and not present into metadata are skipped
	
	Revision 2.50  2011/06/20 10:25:26  marpas
	file number check can be relaxed
	
	Revision 2.49  2011/04/19 16:17:40  marpas
	now uses File::hardlink instead of ::link (it is able to hard link trees)
	useless messages removed
	when working dir is the current one, the internal path does not contain "./" or ".//" prefix
	
	Revision 2.48  2011/04/18 13:11:57  chicas
	Dir inventory added
	
	Revision 2.47  2011/04/13 13:09:10  marpas
	work in progress
	
	Revision 2.46  2011/03/21 18:22:09  marpas
	VEN-1193 change.
	-noCriticalOnDuplicates option management. New exception if a duplicate was found
	
	Revision 2.45  2010/10/08 08:11:10  crivig
	log messages added
	
	Revision 2.44  2010/10/04 13:22:57  crivig
	no satellite handled
	
	Revision 2.43  2010/09/22 13:07:54  marpas
	changes reverted to Rev 2.41 - no anomalies from Inventory
	
	Revision 2.41  2010/09/20 15:19:33  marpas
	wong exception notified in case of wrong geo object: fixed
	
	Revision 2.40  2010/05/19 09:27:22  marpas
	geo localization, if present, shall be correct or the inventory will fail
	
	Revision 2.39  2010/04/26 10:29:10  marpas
	preparation to ORACLE integration (needs libDbGeo ORACLE aware)
	
	Revision 2.38  2010/04/23 16:46:47  marpas
	cloud percentage and product specification management added.
	Test (testMetadata) now outputs a <file>.labels with the mandatory flag (to be assumed the tag is mandatory, its value cannot)
	
	Revision 2.37  2010/02/24 11:20:01  marpas
	jms file management work in progress
	
	Revision 2.36  2010/02/22 10:10:39  marpas
	jms file management added
	
	Revision 2.35  2009/12/04 14:22:55  marpas
	diagnostic message fixed
	
	Revision 2.34  2009/12/04 11:35:11  marpas
	compilation error fixed
	
	Revision 2.33  2009/12/03 13:45:16  marpas
	trying to raise an exception when an element in a list goes wrong
	
	Revision 2.32  2009/09/28 17:02:53  marpas
	dbParam interface obsolecence
	
	Revision 2.31  2009/09/01 17:10:40  marpas
	executeInventory function strongl improved:
	1) process all the list not stopping at the very first problem on a file
	2) slightly better performances while uploading files on SM
	3) exception management improved
	4) log improved
	the above are all depending on Jira Issues VEN-1472
	
	Revision 2.30  2009/09/01 16:17:06  marpas
	log improved
	
	Revision 2.29  2009/06/12 09:31:52  marpas
	VEN-1337 - calling new method: dbInventoryObjectTEC::removeAllSitesLink
	
	Revision 2.28  2009/05/18 17:34:51  marpas
	ocada: checkPeriodicity AFTER the succesful inventory so the minimum expected has to be 2
	
	Revision 2.27  2009/04/06 13:27:03  marpas
	mail recipients get fixed
	
	Revision 2.26  2009/04/03 17:31:25  marpas
	check file type periodicity added
	
	Revision 2.25  2009/03/06 11:55:48  marpas
	local headers included with double quotes
	
	Revision 2.24  2009/02/13 15:42:27  manuel.maccaroni
	corrected an error in Staellite_Id contents checkup
	
	Revision 2.23  2009/02/11 10:15:25  marfav
	replacefile sets the qualified flag if it was not set by metadata
	
	Revision 2.22  2009/02/02 17:51:12  ivafam
	Bug Fixed. Now distribution request is emitted
	
	Revision 2.21  2009/01/29 18:51:01  ivafam
	--replacefiles option added
	
	Revision 2.20  2009/01/29 13:59:30  manuel.maccaroni
	Satellite_Id no longer a mandatory field
	
	Revision 2.19  2009/01/29 11:28:43  marpas
	replacefile behaviour fixed
	
	Revision 2.18  2009/01/29 09:42:18  marpas
	ource field no longer mandatory
	
	Revision 2.17  2009/01/28 16:19:26  marpas
	-replaceFile implemented
	
	Revision 2.16  2009/01/28 16:08:29  marpas
	work in progress
	
	Revision 2.15  2009/01/15 10:16:02  manuel.maccaroni
	added handling of Qualified metadata field
	
	Revision 2.14  2009/01/15 10:15:40  manuel.maccaroni
	added handling of Qualified metadata field
	
	Revision 2.13  2008/12/16 11:04:26  marpas
	PROJECTFLAG used in compilation (OCADA)
	
	Revision 2.12  2008/11/18 13:58:50  marpas
	new test added, debug improved
	
	Revision 2.11  2008/11/10 17:43:26  marpas
	Comments added
	
	Revision 2.10  2008/11/07 17:59:45  marpas
	Band, Tdi and Sites introduction. Partially tested...
	
	Revision 2.9  2008/08/28 08:36:14  giucas
	Messaging improved
	
	Revision 2.8  2007/03/22 09:00:58  valspa
	VDS: ready to test pre dry-run including handling of originator and MDSs
	
	Revision 2.7  2007/01/08 13:48:40  giucas
	The main loop has changed: the current dir is checked before and if and only if no lists are found the configured directory is checked for inventory.
	
	Revision 2.6  2006/10/23 10:06:45  marpas
	variables and/or methods related to file size, header size and blob sized now take into account 64 bits
	
	Revision 2.5  2006/07/03 14:51:22  giucas
	Passing string instead of char* to DateTime ctor
	
	Revision 2.4  2006/07/03 14:46:03  giucas
	Removed convTime method. Using DateTime ctor instead for a correct use of mseconds
	
	Revision 2.3  2006/05/04 18:01:36  giucas
	system("rm ... ") changed into File::unlink
	
	Revision 2.2  2006/04/28 14:59:05  giucas
	InventoryGaps management implemented
	
	Revision 2.1  2006/03/09 10:53:53  seralb
	exception adapted
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.53  2006/02/21 13:40:37  nicvac
	Geo_Type handled only by means the get/setGeoType() method. getMetadataValue access deprecated.
	
	Revision 1.52  2006/01/24 18:05:22  enrcar
	Now symbolic link is removed from input dir
	
	Revision 1.51  2006/01/24 16:30:40  enrcar
	Corrected symbolic link detection when moving to backup
	
	Revision 1.50  2005/09/29 16:27:06  marpas
	orders failed string improved
	
	Revision 1.49  2005/09/29 16:19:52  marpas
	failedOrders method implemented
	
	Revision 1.48  2005/09/28 14:17:16  marpas
	message improved
	
	Revision 1.47  2005/09/23 13:54:43  marpas
	still fixing
	
	Revision 1.46  2005/09/23 11:47:11  danalt
	 else if instead of if
	
	Revision 1.45  2005/09/22 15:10:29  paoscu
	Exceptions management enhanced.
	
	Revision 1.44  2005/06/15 07:57:49  integrator
	multiple storage manager imlemented
	
	Revision 1.43  2005/05/31 14:45:09  marpas
	interfacce di file action cambiate
	
	Revision 1.42  2005/03/17 16:06:33  giucas
	Modified to send messages to excmcf
	
	Revision 1.41  2005/01/04 11:09:34  ivafam
	Added dbFileActionScopeGuard
	
	Revision 1.40  2005/01/03 14:34:50  ivafam
	The constructor of classes deriving from dbFileAction becomes protected.
	
	Revision 1.39  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.38  2004/10/27 09:22:36  seralb
	Bug solved in setting of Mission field
	
	Revision 1.37  2004/10/26 08:39:02  seralb
	ComputeGeoPoints method interface changed and new field Mission
	
	Revision 1.36  2004/10/18 10:49:55  seralb
	Validity flag: solved a bug (getFieldValue) and now extracted from db
	
	Revision 1.35  2004/10/15 15:07:26  seralb
	Bug solved in -overwrite option
	
	Revision 1.34  2004/10/14 15:01:20  seralb
	overwrite option added
	
	Revision 1.33  2004/08/26 11:57:32  seralb
	Added bug comment in case of metadata mandatory field not set
	
	Revision 1.32  2004/05/26 15:00:59  seralb
	Uploaded files now removed at the end by a list
	
	Revision 1.31  2004/05/17 15:40:54  seralb
	Gestion auxiliary files for catalog added
	
	Revision 1.30  2004/05/07 12:22:06  seralb
	Catalog file gestion
	
	Revision 1.29  2004/04/14 12:44:14  seralb
	GeographicLocalization field: now it can work on geo object of different shape
	
	Revision 1.28  2004/02/16 09:49:44  giucas
	Added catch exException in executeInventory method
	
	Revision 1.27  2003/10/28 14:34:08  seralb
	Improvement
	
	Revision 1.26  2003/10/28 11:28:36  seralb
	Added msec in convTime method
	
	Revision 1.25  2003/10/14 14:49:45  seralb
	Added -rf in rm command
	
	Revision 1.24  2003/09/11 16:11:28  seralb
	Notify for optional fields removed
	
	Revision 1.23  2003/09/11 10:01:51  seralb
	Envisat upgrade in moveFileInBackupDir method
	
	Revision 1.22  2003/09/09 14:48:14  seralb
	Added convTime method
	
	Revision 1.21  2003/09/04 15:56:41  seralb
	Bug solved: wrong tag name in resource
	
	Revision 1.20  2003/09/04 15:46:07  seralb
	Added list of extension to remove
	
	Revision 1.19  2003/09/03 15:21:41  seralb
	SourceSwVersion field now optional
	
	Revision 1.18  2003/08/28 14:45:54  giucas
	Added messages for files uploaded
	
	Revision 1.17  2003/07/17 15:12:47  seralb
	Improvement
	
	Revision 1.16  2003/07/16 16:24:22  seralb
	Bug Solved
	
	Revision 1.15  2003/07/09 10:42:04  seralb
	Improvement
	
	Revision 1.14  2003/07/09 10:19:48  seralb
	Methods added
	
	Revision 1.13  2003/06/23 14:42:47  giucas
	Corrected bug on wrong longitude limit value
	
	Revision 1.12  2003/06/06 08:34:27  seralb
	Bug solved
	
	Revision 1.11  2003/05/30 08:32:01  giucas
	Added field Order_Id
	
	Revision 1.10  2003/05/20 14:57:44  seralb
	Exception added
	
	Revision 1.9  2003/05/07 10:32:39  seralb
	dbInventoryObject upgrade
	
	Revision 1.8  2003/04/30 16:46:59  marpas
	using namespace std no longer into acs includes
	
	Revision 1.7  2003/03/25 16:02:42  seralb
	Mandatory and optional Inventory fields review
	
	Revision 1.6  2003/02/20 14:53:00  giucas
	Added libSignal handling
	
	Revision 1.5  2003/02/03 09:04:26  davcas
	Modified exit message to avoid exception notification in case of success
	
	Revision 1.4  2003/02/02 11:30:49  davcas
	Modified warning messages if matadata optional fields lacking
	
	Revision 1.3  2003/01/31 14:06:32  seralb
	
	inserted new catch condition
	
	Revision 1.2  2003/01/28 09:50:12  seralb
	Removed version control and related methods
	
	Revision 1.1.1.1  2003/01/14 14:52:23  seralb
	Import libFileInventory
	

*/

#include <FileInventory.h>
#include <ConfigurationSingleton.h>

#include <FileInvV.h>
#include <Dir.h>
#include <File.h>
#include <Metadata.h>
#include <dbGeoObject.h>
#include <dbGeoString.h>
#include <dbGeoPolygon.h>
#include <dbGeoPnt.h>
#include <dbGeoRing.h>
#include <dbGeoBox.h>
#include <dbGeoLineseg.h>
#include <dbGeoPoint.h>

#include <dbConv.h>
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <dbGenQueryScopeGuard.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryParameters.h>
#include <dbQueryFactoryChain.h>
#include <OriginatorName.h>
#include <NameIdAssociators.h>

#include <InvRelationsRules.h>
#include <dbInventoryObjectTECQuery.h>
#include <dbInventoryObjectTEC.h>
#include <dbFileType.h>
#include <ftmpstream.h>

#include <dbPersistentFactoryChain.h>
#include <Filterables.h>
#include <exFMCFilterables.h>
#include <MCFilterables.h>

#include <DateTime.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <dbUploadFile.h>
#include <InventoryCovRuleChecker.h>

#include <dbSmartPersistent.h>
#include <AnomalyTypes.h>

#include <sys/types.h>  // PRQA S 1013
#include <dirent.h>
#include <sys/types.h>  // PRQA S 1013
#include <sys/stat.h>   // PRQA S 1013
#include <fcntl.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
using namespace mc;

ACS_CLASS_DEFINE_DEBUG_LEVEL(FileInventory)
acs::math::StatNode &FileInventory::statN() { return (*acs::math::Statistics::instance())()["Inventory"] ; }

namespace {
    FileInvV version ;
}

/*
 * Class constructor
 */


FileInventory::FileInventory(dbConnectionPool &pool): 
  _backupDir(),
  _workingDir (),
  _inventoryListFileExt(),
  _mtd(),
  _jms(),
  _rejectedDir(),
  _backupList(),
  _rmFlag(false),
  _testFlag(),
  _allFilesSuccesfullyInventoried(true),
  _isTheListGood(true),
  _numSigInt(0),
  _configuredExt(),
  _filesToBeRemovedList(),
  _option(),
  _failedOrders(),
  _numberOfLists(0),
  _checkFTypePeriodicity(false),
  _duplicateFound(false),
  _relaxFnCheck(false),
  _rejectedDirSet(false),
  _connectionPool(pool), // PRQA S 2528
  _notifications(),
  _manageInventoryAnomalies(false),
  _restoreOnSync (false),
  _noActions (false),
  _deleteUploadedFilepathsOnSuccess(false),
  _setDefaultUniqueId(true),
  _insertOriginatorIfMissing(false)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << version.name()) ;
    statN()["instances"]+=1 ;
} 

/*
 * Class destructor
 */
FileInventory::~FileInventory() throw() {}

//To support Notifications
bool FileInventory_NotifIsEnd( UserNotificationSptr const& userNotificationSptr) {
	//isPerformed set even if Thead received a stop/lazystop
	if ( userNotificationSptr->isPerformed() ) {
		//Nominally will return immediatelly because not running
		userNotificationSptr->waitExitOrTimeout( 10000 ); // PRQA S 4400

		//Check exceptions on performed notification
		exception* ex = 0 ;
		if ( userNotificationSptr->runException(ex) && ex) {
			ACS_LOG_ERROR("UserNotifier: Error occurred sending notification: "<<ex) ;
			delete ex;
		}
	}
    return userNotificationSptr->isPerformed();
}

//To support Notifications
class FileInventory_NotificationsScopeGuard { // PRQA S 2109
public:
	explicit FileInventory_NotificationsScopeGuard(deque<InventoryEmailNotificationSptr>& notifications):
		_notifications(notifications) {} // PRQA S 2528
	~FileInventory_NotificationsScopeGuard() throw() {
        // PRQA S 4631 L1
		ACS_LOG_PRIVINFO("Flushing Notifications. "<<_notifications.size()<<" Notifications pending ... ") ;
		//Flush the notifications
        try {
		    while ( !_notifications.empty() ) {

			    //Clean the queue: remove finished notifications
			    _notifications.erase(
				    remove_if( _notifications.begin(), _notifications.end(), FileInventory_NotifIsEnd ),
				    _notifications.end());

                if ( !_notifications.empty() ) {
				    ACS_LOG_PRIVINFO("Still "<<_notifications.size()<<" Notifications pending ... ") ;
				    //Ok, hurry up! 5, 4, 3, 2, 1 ...
				    if ( ! _notifications.front()->waitExitOrTimeout(5000) ) { // PRQA S 4400
					    //... Die!
					    _notifications.front()->stop(false);
					    ACS_LOG_ERROR("FileInventory: Notification ["<<_notifications.front()->getDescription()<<"] stopped because notification time elapsed.") ;
				    }
			    }
		    }
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
        // PRQA L:L1
	}
private:
    FileInventory_NotificationsScopeGuard() ; // not implemented
    FileInventory_NotificationsScopeGuard(const FileInventory_NotificationsScopeGuard &) ; // not implemented
    FileInventory_NotificationsScopeGuard & operator= (const FileInventory_NotificationsScopeGuard &) ; // not implemented
private:
	std::deque<InventoryEmailNotificationSptr>& _notifications;
};

/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////

void FileInventory::makeInventory(const string &tgtDir , const string &bcpDir, const string &option) // PRQA S 4020
{	
	ACS_LOG_PRIVINFO("Inventory option is \"" << option << "\"") ;

	ACS_LOG_PRIVINFO("Configuring inventory relation rules") ;
    
    db::configureInvRelationsRules("Inventory") ;

	//Instantiate the scope guard to flush notifications
	FileInventory_NotificationsScopeGuard notificationScopeGuard( _notifications );

	// Load info from the Inventory resource 
	rsResourceSet &_rsConf = ConfigurationSingleton::instance()->get();
	
	_rsConf.getValue("Inventory.WorkingDir" , _workingDir);
	_rsConf.getValue("Inventory.ListFileExtension" , _inventoryListFileExt);
	_rsConf.getValue("Inventory.MetadataFileExtension" , _mtd);
	try {
		_rsConf.getValue("Inventory.JmsFileExtension" , _jms);
	}
	catch(exception &x) {
		_jms = "jms" ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "JMS extension defaulted to \"" << _jms << "\"") ;
	}
	_rsConf.getValue("Inventory.FilteringTestFiles", _testFlag);
	try {
		_rsConf.getValue("Inventory.CheckFTypePeriodicity" , _checkFTypePeriodicity);
	}
	catch(exception &) {
		_checkFTypePeriodicity = false ;
	}
	
	_backupDir = bcpDir;
	_option = option;
	
	_configuredExt.clear() ;
	_rsConf.getArray("Inventory.List_Of_Extensions.Extension",_configuredExt);

	if (!_configuredExt.empty())
	{
		ACS_LOG_WARNING ("Found extensions configured in Inventory conf space. Please remove them from the conf space.");
		ACS_LOG_WARNING ("Extension configuration will be ignored.");
		_configuredExt.clear();
	}
	
	if (_rsConf.hasKey("Inventory.RejectedDir")) {
		_rsConf.getValue ("Inventory.RejectedDir", _rejectedDir);
		_rejectedDirSet = true;
	}
    if (_rsConf.hasKey ("Inventory.PopulateInvAnomalies"))
    {
        _rsConf.getValue ("Inventory.PopulateInvAnomalies", _manageInventoryAnomalies);
    }
    else
    {
        ACS_LOG_PRIVINFO ("Will not populate delay inventory anomalies. Default setting used (feature disabled)");
    }
	
    if (_rsConf.hasKey ("Inventory.DeleteUploadOnSuccess"))
    {
        _rsConf.getValue ("Inventory.DeleteUploadOnSuccess", _deleteUploadedFilepathsOnSuccess);
    }
    else
    {
        ACS_LOG_PRIVINFO ("DeleteUploadOnSuccess flag not occurring in configuration. Default setting used (feature disabled)");
    }
	
	if (_rsConf.hasKey ("Inventory.SetDefaultUniqueId"))
    {
        _rsConf.getValue ("Inventory.SetDefaultUniqueId", _setDefaultUniqueId);
    }
    else
    {
        ACS_LOG_PRIVINFO ("SetDefaultUniqueId flag not occurring in configuration. Default setting used (filename + fileversion)");
    }
	
	if (_rsConf.hasKey ("Inventory.InsertOriginatorIfMissing"))
    {
        _rsConf.getValue ("Inventory.InsertOriginatorIfMissing", _insertOriginatorIfMissing);
    }
    else
    {
        ACS_LOG_PRIVINFO ("InsertOriginatorIfMissing flag not occurring in configuration. Default setting used (feature disabled)");
    }

	string currentDirectory ;	
	try 
	{
		char currdir[1024] ; // PRQA S 4403
		memset(currdir,0,sizeof(currdir)) ;
		getcwd(currdir, sizeof(currdir)-1); // PRQA S 3084
        currentDirectory = currdir ;
        ACS_LOG_PRIVINFO("Looking for LIST files in dir: './' (" << currentDirectory << ")") ;
		loopOnDir("./") ;
	}
	catch(exception& e)
	{
        ACS_LOG_WARNING("Exception got from loopOnDir while looking in \"" << currentDirectory << "\"") ;
		
		if (_numberOfLists > 0 ) { // at least something found
			throw ;
		}
		try
		{
			ACS_LOG_PRIVINFO("Looking for LIST files in dir: \"" << tgtDir << "\"") ;
			loopOnDir(tgtDir) ;
		}
		catch(exception& e1)
		{
			ACS_LOG_ERROR("Exception got from loopOnDir while looking in \"" << tgtDir << "\"") ;
			throw ;
		}
		return ;
	}

	try
	{
		if(_numberOfLists == 0 ) // nothing found on ./
		{
			ACS_LOG_PRIVINFO("Looking for LIST files in dir: \"" << tgtDir << "\"") ;
			loopOnDir(tgtDir) ;
		}
	}
	catch(exception& e1)
	{
		ACS_LOG_ERROR("Exception got from loopOnDir while looking in \"" << tgtDir << "\"") ;
		throw ;
	}
}

void FileInventory::loopOnDir(const string &targetDir) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method called - target dir is \"" << targetDir << "\"") ;
	
	string listName;
	for(;;)
	{
		try
		{	
			listName = findListInDir(targetDir);

			_numberOfLists++;
			
            ACS_LOG_PRIVINFO("Processing inventory list #" <<_numberOfLists << ": " << listName) ;
		}
		catch(exception& et)
		{	
			if(_numberOfLists > 0)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processed "<< _numberOfLists <<" lists")
			}
			else
			{
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ACS_LOG_NOTIFY_EX(et);
				ACS_CLASS_END_DEBUG
			}
			
		
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_allFilesSuccesfullyInventoried is " << boolalpha
					<< boolalpha << _allFilesSuccesfullyInventoried) ;
			
			if (_allFilesSuccesfullyInventoried == false)
			{		
				string exs = "ERROR inventorying at least 1 file in 1 list (tgtdir="+targetDir + ")" ;
				if (_duplicateFound) {
					ACS_THROW(DuplicateInventoryException(et, exs)) ;  // PRQA S 3081
				}
				else {
					ACS_THROW(FileInventoryException(et, exs)) ;  // PRQA S 3081
				}
			}
			else { return ; }
		}
		
		executeInventory(targetDir, listName);
	}
}

string FileInventory::findListInDir(const string &targetDir) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "searching files with extension: \"" << _inventoryListFileExt << "\"")
	
	// Read the Inventory list from the _targetDir 
	Dir dir(targetDir);
	for(unsigned int i=0; i < dir.size(); i++)
	{
		string currentFileName = dir[i];
		
		string currentExt = File::getExt(currentFileName);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "evaluating file " << currentFileName) ;
				
		if (currentExt == _inventoryListFileExt)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current list name is " << currentFileName) ;
		
			ACS_LOG_PRIVINFO("Processing list " << currentFileName << "...") ;
		
			string completeListPath = targetDir + "/" + currentFileName;
			
			// Check lock file existence
			string lockFileName = File::removeExt(completeListPath) + ".lock"; 
			int lockfiledes = open(lockFileName.c_str(), O_CREAT | O_WRONLY, 00666);
			ACS_COND_THROW(lockfiledes == -1,  // PRQA S 3081
									  FileInventoryException("Cannot create lock file: "+lockFileName)) ;
									  
			// If return value is 0 the list is not locked , if -1 the list is already locked 
			int tryLock = lockf(lockfiledes, F_TLOCK, 0);
			// File already locked yet
			if(tryLock == -1)
			{
				if((errno == EACCES) || (errno == EAGAIN))
				{
                    ACS_LOG_WARNING("list file " << currentFileName << " already locked ") ;
                    continue;
				}
				else
				{
					ACS_THROW(FileInventoryException("Error locking file " + lockFileName)) ;  // PRQA S 3081
				}
			}
				
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "available list name " << currentFileName << " proceeding ...") ;
            statN()["list"]+=1 ;

			return currentFileName;
		}
	}	// loop on the working directory
    
	ACS_THROW(FileInventoryException("No available (unlocked) lists found"));  // PRQA S 3081
}

string FileInventory::usage() 
{
    string u = "[--overwrite | --replacefile | --noCriticalOnDuplicates | --restoreonsync] [--noactions]\n" ;
    u+= "\tWhere:\n" ;
    u+= "\t--overwrite   if an old inventory exists and has validity flag true \n" ;
    u+= "\t                 will cause update of invdate and modificationdate \n" ;
    u+= "\t              else\n" ;
    u+= "\t                 a complete erasure and reinventory or just inventory\n" ;
    u+= "\n" ;
    u+= "\t--replacefile if an old inventory exists metadata and data will be replaced\n" ;
    u+= "\t              else just a normal inventory\n" ;
    u+= "\n" ;
    u+= "\t--noCriticalOnDuplicates will not cause a critical exception if the inventory already exists\n" ;
    u+= "\n" ;
    u+= "\t--restoreonsync will upload a file on sync storage managers if the file already exists\n" ;
    u+= "\t                updating only the inventory date in the metadata\n" ;
    u+= "\t                normal Inventory will be performed if the file does not exists\n" ;
    u+= "\n" ;
    u+= "\t--noactions will disable post inventory automatic actions\n";
    u+= "\t            based on the pending inventory table\n" ;
    u+= "\n";

    return u ;
} 	


void FileInventory::sendNotifications( dbInventoryObjectTEC const& invObj ) {

	try {
        const string &dbKey = _connectionPool.getKey() ;    
		//Check if the inventory object is member of a notification group
		//This query is SQL standard
		ostringstream sqlStrm; sqlStrm<<"\
			SELECT crGr.id_crgroup \
			FROM t_filetypes ft \
			INNER JOIN t_cr_x_filetypes crFt \
			   on ft.id_filetype = crFt.filetype_id \
			INNER JOIN t_crgroups crGr \
			   on crFt.crgroup_id = crGr.id_crgroup \
			WHERE ft.filetype = "<<string(db::toSQL(dbKey, invObj.getFileType() )); // PRQA S 3050, 3081

		SQLString querySQL (dbKey, sqlStrm.str() );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing query: " <<querySQL.getSQL()) ;

		dbSet set;

		{
			dbConnectionWrapper pw(_connectionPool) ;
			dbConnection &conn = pw; // PRQA S 3050 

			dbQuery query( conn, querySQL, "SEL not.groups" );
			query.execForSet( set );
		}

		//For each group, send the notification
		dbRow row(dbKey);
		while ( set.getNextRow(row) ) {
			
			string groupId("");
			row.column(0, groupId);

			ostringstream descr; descr<<"Inventory E-mail notification. name: "<<invObj.getFileName();

			InventoryEmailNotificationSptr notification( new InventoryEmailNotification );
			notification->setInfo( invObj.getFileName(), 
								   invObj.getFileType(),
								   groupId ).setDescription( descr.str() );

			if ( notification->isServiceEnabled() ) {
				_notifications.push_back( notification ); //Retain the pointer
				notification->start();
			}
		}

	} catch(exception& ex) {
		ACS_LOG_WARNING("error sending mail notifications for product "<<invObj.getFileName() <<" Occurred Error: " <<ex) ; 
	}

}

namespace {
    class InvStat { // PRQA S 2109
    public:
        explicit InvStat(math::StatNode &n) : timer_(), good_(false), node_(n) // PRQA S 2528
        {
            timer_.start() ;
        }
        ~InvStat() throw() 
        {
            try {
                if (not good_) {
                    node_["error"]+=1 ; // PRQA S 4631
                }
            }
            catch(exception &x) {
                ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
            }      
        }
        void good() {
            if (not good_) {
                good_ = true ;
                node_["ok"]+=1 ;
                timer_.stop() ;
                node_["elapsed [ms]"] += double(timer_.elapsed()) ; // PRQA S 3081
            }
        }
    private:
        InvStat() ; // not implemented
        InvStat(const InvStat &) ; // not implemented
        InvStat& operator=(const InvStat &) ; // not implemented
    private:
        Timer timer_ ;
        bool good_ ;
        math::StatNode &node_ ;
    } ;
}

void FileInventory::executeInventory(const string& currDir, const string &curFilNam) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method called for target dir " << currDir) ;
	string invFileListName = (currDir == "./" ? curFilNam : currDir + "/" + curFilNam); // PRQA S 3384, 3385 
	const string &dbKey = _connectionPool.getKey() ;
	off_t fileSize = File::size(invFileListName);
	if(fileSize == 0)
	{
		/* If the file.LIST has size 0, issue a warning, remove the LIST, remove the lock file and continue searching for other lists AAA*/
		ACS_LOG_WARNING("Warning: the list " << curFilNam <<" is empty") ;
		removeListAndLockFile(invFileListName);
		return;
	}
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "list file size " << fileSize) ;
	
	char * tempBuf = new char[1024]; // PRQA S 4403
	vector<string> invStrList;
	ifstream fileStream;
	fileStream.open(invFileListName.c_str());
	if(!fileStream.is_open())
	{
		/* If I can't open the file.LIST I notify the exception, move the LIST, remove the lock files and continue searching for other lists AAA*/
		ACS_LOG_ERROR("Error cannot open the list file " << curFilNam) ;
		moveListAndLockFile(invFileListName);
		return;
	   
	}
    
	while(!fileStream.eof())
	{
		/* If I can't read the file.LIST I notify the exception, move the LIST, remove the lock file and continue searching for other lists AAA*/
		fileStream.getline(tempBuf, fileSize+1);
		if(fileStream.bad())
		{
			ACS_LOG_ERROR("Error reading list file " << curFilNam) ;
			moveListAndLockFile(invFileListName);
			return ;
		}
        
		string element(tempBuf);
		if (not element.empty()) {
			invStrList.push_back(element);
        }
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "read line '" << element << "'") ;
	}
	
	delete [] tempBuf;
	tempBuf = 0;
	
	// This is the list of files to be moved in backup dir in case of at least a wrong file in the list
	vector<string> goodFilesList;;
	
	// Check if all the files of the list are good
	size_t lsize = invStrList.size() ;
    for (size_t iterat = 0; iterat < lsize; ++iterat)
	{
		string element = invStrList[iterat];
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "element " << element << " of " << invStrList.size() << " elements...") ;
		
		checkFiles(currDir, element, goodFilesList);
	}	
	
	if(!_isTheListGood)
	{
	    size_t glistsize = goodFilesList.size() ;
		for (size_t iterat = 0 ; iterat < glistsize; ++iterat)
		{
			string elem = goodFilesList[iterat] ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "moving " << elem << " to backup dir...") ;
			wrongList(currDir,elem); /* I put the good files in the backup dir and let the wrong files in the backup dir */
		}		

		moveListAndLockFile(invFileListName); /* I put the complete (wrong and good files) list in the backup dir */
		_isTheListGood = true;
		return;
		
	}
	
	// Start a cycle on all the file found in the Inventory List
	lsize = invStrList.size() ;
    
    for (size_t it = 0 ; it < lsize; ++it) 
		 
	{
        statN()["files"]+=1 ;
        InvStat inventory_stat(statN()["files"]) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "starting inventory cycle...") ;
		
		string elem ;
		string fileNoPoint ;
		string fileNewExt ;
		string fileWithPath ;
		string fileWithPathDO ;
		string fileJms ;
		bool   isProductDir = false ; // default 
		try {
			elem = invStrList[it]; 
			
			// Notify to MCF that process is started
			ACS_LOG_EVENT("INVENTORY_LIST",elem) ;

			_rmFlag = false;			

			string ext 	= File::getExt(elem); 	

			{
				// Define files name and extensions (e.g. pippo.n1)
				fileNoPoint		= elem;               				// pippo.n1
				fileNewExt		= elem + "." + _mtd;				// pippo.n1.MTD
				fileJms			= elem + "." + _jms;				// pippo.n1.jms
				fileWithPath 	= (currDir == "./" ? fileNewExt : (currDir + "/" + fileNewExt));		// ./currDir/pippo.n1.MTD  // PRQA S 3380, 3385 2
				fileWithPathDO	= (currDir == "./" ? elem : (currDir + "/" + elem));			// ./currDir/pippo.n1.DBL or ./currDir/pippo.n1.OBJ
			}


			isProductDir = File::isDir(fileWithPathDO) ;
			Metadata  toFillTheInventoryObj;
			string fiCl;

			// Read the file from disk
			try
			{
				toFillTheInventoryObj.readMetadataFile(fileWithPath);
				fiCl = toFillTheInventoryObj.getMetadataValue("File_Class");
			}
			/* If I can't read the requested file from the file.LIST I notify the exception AAA*/
			catch(exception& et)
			{
                statN()["files"]["error"]+=1 ;
				ACS_THROW(FileInventoryException(et, "read error on " + fileWithPath)) ;  // PRQA S 3081
			}

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FilteringTestFiles is set "
			    	<< boolalpha << _testFlag << " and the file_class is "
					<< fiCl) ;

			if(!_testFlag && fiCl=="TEST")
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "removing TEST files...") ;

				// Remove datablock from working directory
				string fileToRemove1 = currDir + "/" + elem; 
				_filesToBeRemovedList.push_back(fileToRemove1);

				// Remove metadata from working directory
				string fileToRemove2 = currDir + "/" + fileNewExt; 
				_filesToBeRemovedList.push_back(fileToRemove2);

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Files " << elem << " and " << fileNewExt << " cadidate to final remotion.") ;
				_rmFlag = true;

			}
			// Execute the inventory
			else
			{			
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ext is " << ext
					    << ", fileNoPoint is " << fileNoPoint
					    << ", fileNewExt is " << fileNewExt
					    << " and elem is " << elem ) ;

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing file " << fileWithPath) ;
				dbInventoryObjectTEC* theInventoryObject =
				dynamic_cast<dbInventoryObjectTEC*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObjectTEC", dbKey)); // PRQA S 3081, 4412
	            dbPersistentScopeGuard queryGuard(theInventoryObject);		

				if(!theInventoryObject)
				{
                    statN()["files"]["error"]+=1 ;

					//Locally try this exception because I want to notify it here
					try{
						ACS_THROW(FileInventoryException("Cannot obtain a dbInventoryObjectTEC instance"));  // PRQA S 3081
	   				}
					catch(exception& newEx)
					{
						ACS_LOG_NOTIFY_EX(newEx);
						throw ;
					}

				}

                string prodVersion ;
				try
				{	
					prodVersion = fillObj(toFillTheInventoryObj, *theInventoryObject);  // *theInventoryObject

				}	
				catch(exception& et)
				{
                    statN()["files"]["error"]+=1 ;
					//Locally try this exception because I want to notify it here
					try{
						ACS_THROW(FileInventoryException(et, "Error in setting one mandatory field of db_inventory_object (remember to clean manually the working dir before to start again the Inventory) "));  // PRQA S 3081
					}
					catch(exception& newEx) {
						ACS_LOG_NOTIFY_EX(newEx);
						throw ;
					}
				}

				//******* UPLOAD TO DB AND Archive *******
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Upload to Archive and DB...") ;

				//Verify if file is already in the database

				dbInventoryObjectTECQuery* dbInvObjectQuery=dynamic_cast<dbInventoryObjectTECQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectTECQuery", dbKey)); // PRQA S 3081, 4412
				if(!dbInvObjectQuery)
				{
                    statN()["files"]["error"]+=1 ;
					//Locally try this excpetion because I want to notify it here
					try{
						ACS_THROW(FileInventoryException("Cannot obtain a dbInventoryObjectTECQuery instance"));  // PRQA S 3081
					}
					catch(exception& newEx){
						ACS_LOG_NOTIFY_EX(newEx);
						throw ;
					}
				}
				
				string inventoryFilename = toFillTheInventoryObj.getGlobalFileName();
				dbGenQueryScopeGuard queryGuard2(dbInvObjectQuery);
				dbQueryParameters parameters(dbKey) ; 
				parameters.addParameter(dbParam(dbKey, "T_Inventory","FileName", dbParam::EQUAL_TO, db::toSQL(dbKey, inventoryFilename))) ; // PRQA S 3050, 3081 2
				parameters.addParameter(dbParam(dbKey, "T_Inventory","FileVersion", dbParam::EQUAL_TO, db::toSQL(dbKey, prodVersion))) ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to query T_Inventory where " << parameters.getSqlString() ) ;
				dbInvObjectQuery->addParameter(parameters) ;

				// Init num variable and compute it with a scoped connection
				size_t num = 0;

				{
		            dbConnectionWrapper pw(_connectionPool) ;
		            dbConnection &conn = pw ; // PRQA S 3050
					num = dbInvObjectQuery->doQuery(conn);
				}

				if (num>0)	/* There are files in db */
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, num << " t_inventory records occur in DB with filename=\"" << inventoryFilename << "\" and version=\"" << prodVersion << "\"") ;
					if(_option == "-overwrite" || _option == "--overwrite" )
					{	
					
 						ACS_COND_THROW(isProductDir, FileInventoryException("Overwrite option not implemented on product tree") ) ;  // PRQA S 3081
						bool valFlag = (*dbInvObjectQuery)[0]->getValidityFlag();

						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting Inventory with option " << _option 
                                << " and Validity Flag " << boolalpha << valFlag << "...") ;

						if(valFlag)// if validity flag is true I update the inventory and modification date
						{
						    dbConnectionWrapper pw(_connectionPool) ;
						    dbConnection &conn = pw ; // PRQA S 3050
							DateTime today;
							(*dbInvObjectQuery)[0]->setInventoryDate(today);
							(*dbInvObjectQuery)[0]->setModificationDate(today);
							dbPersistent::IdType id = (*dbInvObjectQuery)[0]->update(conn);
							ACS_LOG_INFO("inventory and modification date updated on inventory id: " << id << " filename is " << (*dbInvObjectQuery)[0]->getFileName()) ;
						}
						else 	// if validity flag is false I delete the file and redo the inventory
						{
							dbFileAction* eraser = dbFileActionFactoryChain::instance()->newEraseAction(_connectionPool, *(*dbInvObjectQuery)[0]);
							dbFileActionScopeGuard guard(eraser);
							// This is not a thread and the start is a synchronous operation
							eraser->start();

							{
								dbConnectionWrapper pw(_connectionPool) ;
								dbConnection &conn = pw ; // PRQA S 3050
								(*dbInvObjectQuery)[0]->remove(conn) ;
							}

							num = 0;	/* reset num 'cause I have to do the inventory again*/
							ACS_LOG_INFO(fileNoPoint << "(" << prodVersion << ") removed from Archive and DB") ;
						}
					}
					else if(_option == "-replacefile" || _option == "--replacefile")
					{	

#warning THIS WILL USE ANOTHER CONNECTION TO PERFORM THE ERASE

						ACS_COND_THROW(isProductDir, FileInventoryException("Replacefile option not implemented on product tree") ) ;  // PRQA S 3081
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting Inventory with option " << _option << "...") ;

						dbConnectionWrapper pw(_connectionPool) ;
						dbConnection &conn = pw ; // PRQA S 3050   

						db::Transaction transaction(conn) ;	

						transaction.begin_work("") ;

						try
						{
							DateTime today;
							{	
								dbPersistent ::IdType id = (*dbInvObjectQuery)[0]->getId();
								ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Inventory id to update = " << id);
								theInventoryObject->setId(id);
							}
							theInventoryObject->setFromDatabase() ; // to allow update on it
							theInventoryObject->setValidityFlag(false);
							theInventoryObject->setInventoryDate(today);
							theInventoryObject->setModificationDate(today);
							bool set = false ;
							theInventoryObject->getQualified(&set) ;
							if (!set) { theInventoryObject->setQualified(1) ; }
							// note: up to now, the link already existing on the DB are unknown, while 
							// the theInventoryObject contains all the sites links read from metadata.
							// the removeAllSitesLink method removes those eventually on the DB, when updating
							// it will save again them if any
							theInventoryObject->removeAllSitesLink(conn) ;
							theInventoryObject->update(conn);

                            if (!_noActions)
                            {
							    SQLString pendingString(dbKey, SQLString::INSERT, "INS t_pendinginventory") ;
        						pendingString.addTableList("t_pendinginventory") ;
        						pendingString.addValuesList("filename" , db::toSQL(dbKey, theInventoryObject->getFileName())) ; // PRQA S 3050, 3081 2
							    pendingString.addValuesList("fileversion" , db::toSQL(dbKey, theInventoryObject->getFileVersion())) ;
							    dbQuery pendingquery(conn) ;

							    pendingquery.sqlString(pendingString, "INS t_pendinginventory") ;


							    if (!pendingquery.execForStatus())
							    {
								    ACS_THROW(dbQuery::QueryExecuteException("Query Execution error: Command : " +  // PRQA S 3081
																     pendingString.getSQL() + " Error: " + 
																     pendingquery.errorString())) ;
							    }
                            }




							dbFileAction* eraser = dbFileActionFactoryChain::instance()->newEraseAction(_connectionPool, *theInventoryObject);
							dbFileActionScopeGuard guard(eraser);
							eraser->start();

							num = 0;	/* reset num 'cause I have to do the inventory again*/
  							transaction.commit() ;
							ACS_LOG_INFO("file " << fileNoPoint << "(" << prodVersion << ") removed from Archive and DB ") ;
						}
						catch (...)
						{
							transaction.roll_back() ;
							throw ;
						}

 					}
                    else if(_option == "-restoreonsync" || _option == "--restoreonsync")
                    {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting Inventory with option " << _option << "...") ;

						dbConnectionWrapper pw(_connectionPool) ;
						dbConnection &conn = pw ; // PRQA S 3050 

                        // Update the inventory date and nothing else
                        DateTime today;
                        theInventoryObject->load(conn); // Restore metadata from the database
                        theInventoryObject->setInventoryDate(today);
                        theInventoryObject->update(conn);

                        // This will start the upload on the configured Storage Managers
                        num=0;
                    }
					else /* option is not -overwrite or there is no option */
					{	
						bool raiseError = true;
						_duplicateFound = true ;
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "option not recognized (" << _option << ")") ;
						// ECICMF-48: noCriticalOnDuplicates is used to identify the Importer scenario (noCriticalOnDuplicates = true) from the 
						// ThinLayer scenario (noCriticalOnDuplicates = false)
      					if (_option == "-noCriticalOnDuplicates" || _option == "--noCriticalOnDuplicates") {
							ACS_LOG_WARNING("A duplicate file was found: " 
								<< fileNoPoint << " (" << prodVersion << "), but the option is -noCriticalOnDuplicates. "
								<< "Probably the monitor and control will ignore this error.") ;
							
							// Check if rejectedDir has been configured
							if (_rejectedDirSet) {
								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rejected dir configured (" << _rejectedDir << ")");
								std::string filePath = StringUtils::pathJoin(currDir, elem);
								std::string mtdPath = filePath + "." + _mtd;
								std::string jmsPath = filePath + "." + _jms;
								
								// Move file into rejected dir
								if (File::exists(filePath)) {
									ACS_LOG_INFO("Moving file " << filePath << " into rejected directory \"" << _rejectedDir << "\"");
									File::rename(filePath, StringUtils::pathJoin(_rejectedDir, File::getFileName(filePath)));
								} else {
									ACS_LOG_WARNING("File " << filePath << " not found. It cannot be moved into rejected directory.");
								}
								
								// Move MTD into rejected dir
								if (File::exists(mtdPath)) {
									ACS_LOG_INFO("Moving file " << mtdPath << " into rejected directory \"" << _rejectedDir << "\"");
									File::rename(mtdPath, StringUtils::pathJoin(_rejectedDir, File::getFileName(mtdPath)));
								} else { 
									ACS_LOG_WARNING("File " << mtdPath << " not found. It cannot be moved into rejected directory");
								}
								
								// Move JMS into rejected dir
								if (File::exists(jmsPath)) {
									ACS_LOG_INFO("Moving file " << jmsPath << " into rejected directory \"" << _rejectedDir << "\"");
									File::rename(jmsPath, StringUtils::pathJoin(_rejectedDir, File::getFileName(jmsPath)));
								} else { 
									ACS_LOG_WARNING("File " << jmsPath << " not found. It cannot be moved into rejected directory.");
								}
								
								ACS_LOG_MC_INFO(MCFunction("Archive") << MCCode("ARC") << MCMetric(Metric_WARN) << mc_param("jobid", File::getFileName(currDir)) 
										<< mc_param("slotid", getpid()) << mc_param("message", "Product " + elem + " moved into rejected dir"));
								
								raiseError = false;
							}
						}

						if (raiseError) {
							try {
								// S1PDGS-31336: reporting the message also in syslog via a filterable error
								ostringstream os;
								os << "File " << fileNoPoint << " is already in database";
								ACS_LOG_ERROR (os.str());
								ACS_THROW(FileInventoryException(os.str()));  // PRQA S 3081
							}
							catch(exception &x) {
								ACS_LOG_NOTIFY_EX(x);
							}
							moveFileInBackupDir(currDir,elem) ;
						}
						_rmFlag = true;

					}
				}
				if (num==0)
				{	
					bool isTheFileCorrectlyUploaded = true;

					string failedOrder ; 
					{
						ostringstream orderMessage ;
						bool orderidset = false;
						int failedOrderId = theInventoryObject->getOrderId(&orderidset) ;
						if (orderidset) {

							orderMessage << failedOrderId ;
							if (_failedOrders.empty()) {
								_failedOrders = "Failed order(s): " ;
                            }
							else {
								_failedOrders += ", " ;
                            }
							_failedOrders += orderMessage.str() ;
							failedOrder = "Failed order: " + orderMessage.str() ;
						}
					}

					try
					{
						dbUploadFile* uploader = dynamic_cast<dbUploadFile*>(dbFileActionFactoryChain::instance()->newUploadAction(_connectionPool, *theInventoryObject,fileWithPathDO)); // PRQA S 3081

						if(!uploader)
						{
							ACS_THROW(exIllegalValueException("Cannot obtain a dbUploadFile instance")) ; // PRQA S 3081
						}
						dbFileActionScopeGuard guard(uploader);
                        InvStat upload_stat(statN()["upload"]) ;

                        // set the special options
                        uploader->restoreOnSync (_restoreOnSync);
                        uploader->noActions (_noActions);
						
						if( _deleteUploadedFilepathsOnSuccess ) {
							uploader->setDeleteUploadedFilepathOnSuccess(true);
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << fileWithPathDO << "\" file path must be deleted after successful upload") ;
						}

						uploader->start() ;

						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "starting upload...") ;

						while(uploader->uploading() )
						{
							Timer::delay(100) ; // PRQA S 4400
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Still uploading ....") ;
						}
                        upload_stat.good() ;				
					}
					catch(dbUploadFile::PreconditionException &e)
					{
						// If there's an error in the dbInventoryObject it move the file.DBL/.MTD in the 
						// BackupDir and write the file.DBL name in the backupList
						ACS_LOG_NOTIFY_EX(e);
                        ACS_LOG_ERROR("There's an error in dbInventoryObjectTEC (PreconditionException). " << failedOrder) ;
						try {
							moveFileInBackupDir(currDir,elem);
						}
						catch( exception &ee) 
						{
							ACS_LOG_NOTIFY_EX(ee);
							ACS_LOG_WARNING("Cannot even move file in backup dir." );
						}
						_rmFlag = true;
						isTheFileCorrectlyUploaded = false;
					}	
					catch(dbUploadFile::UploadException &e)
					{
						// If there's an error while uploading in Arf it move the file.DBL/.MTD in the 
						// BackupDir and write the file.DBL name in the backupList
						ACS_LOG_NOTIFY_EX(e);
						ACS_LOG_ERROR("There's an error while uploading file (UploadException). "  << failedOrder) ;
						try {
 							moveFileInBackupDir(currDir,elem);
						}
						catch( exception &ee) 
						{
							ACS_LOG_NOTIFY_EX(ee);
							ACS_LOG_WARNING("Cannot even move file in backup dir." );
						}
						_rmFlag = true;
						isTheFileCorrectlyUploaded = false;
					}	
					catch(exception &e) 
					{
						// If there's a generic error it move the file.DBL/.MTD in the 
						// BackupDir and write the file.DBL name in the backupList
						ACS_LOG_NOTIFY_EX(e); 
						ACS_LOG_ERROR("Generic error. "  << failedOrder) ;
						try {
							moveFileInBackupDir(currDir, elem);
						}
						catch( exception &ee) 
						{
							ACS_LOG_NOTIFY_EX(ee);
							ACS_LOG_WARNING("Cannot even move file in backup dir." );
						}
						_rmFlag = true;
						isTheFileCorrectlyUploaded = false;
					}

					if(isTheFileCorrectlyUploaded == true)
					{
                        inventory_stat.good() ; // update stats

						ACS_LOG_INFO("Inventory of " << fileWithPathDO << " succesfull (option " << _option << ").") ;
 
 						//Send Notification - if allowed by configuration
						//Only if the coordinated release DB schema is installed
						bool notificationsAllowed = false;
						{
							// Release the connection from the pool after the check and before issuing notifications
							dbConnectionWrapper pw(_connectionPool) ;
							dbConnection &conn = pw ; // PRQA S 3050
							notificationsAllowed = dbSchemaMetaDataProvider::existTable("t_cr_x_filetypes",conn);
						}
						
						if (notificationsAllowed)  // PRQA S 4412
						{
							this->sendNotifications( *theInventoryObject );
						}
	// MP *=*=*=*=*
	// this born with OCADA and improved in ADC .. see Inventory config file for parameters 
						if (_checkFTypePeriodicity) { // file type periodicity to be checked ?
                            try
                            {
                                dbPersistent::IdType id = theInventoryObject->getId();

								// Perform the coverage check if requested in a scoped database connection
								InventoryCovRuleChecker::CoverageResult result;
								{								
									dbConnectionWrapper pw(_connectionPool) ;
									dbConnection &conn = pw ; // PRQA S 3050
		                            result = InventoryCovRuleChecker::checkRules (conn, id);
								}

                                bool emitWarning = (result.warningDelay || result.warningCoverage);

                                if (emitWarning)
                                {
                                    ostringstream os;
                                    os << "The file [" << theInventoryObject->getFileName() << "] Version [" << theInventoryObject->getFileVersion() << "] ";
                                    if (result.warningDelay && result.warningCoverage) {
					                    os << "does not meet the arrival timeliness and expected time coverage";
                                    }
                                    else if (result.warningDelay) {
                                        os << "does not meet the arrival timeliness";
                                    }
                                    else {
                                        os << "does not meet the expected time coverage";
                                    }

                                    // Emit the message on the log file
                                    ACS_LOG_WARNING (os.str());

                                    // If configured emit also an event in the inventory anomalies table
                                    if (_manageInventoryAnomalies)
                                    {
										dbConnectionWrapper pw(_connectionPool) ;
						                dbConnection &conn = pw ; // PRQA S 3050
                                        dbSmartPersistent anomaly("t_inventoryanomalies", conn);
                                        anomaly.set( "filename", theInventoryObject->getFileName(), conn );
                                        anomaly.set( "fileversion", theInventoryObject->getFileVersion(), conn );
                                        anomaly.set( "message", os.str(), conn );
                                        anomaly.set( "anomaly_type_id", AnomalyTypes::instance(conn.getKey())->id("DELAY"), conn );
                                        anomaly.save(conn);
                                    }
                                }

                            }
                            catch (exception& ex)
                            {
                                // No problem. Notify the exception and go ahead.
                                ACS_LOG_NOTIFY_EX(ex);
                            }
						}
	// MP *=*=*=*=*
	
	// MP *=*= EMSA upgrade
						moveJmsFile(currDir == "./" ? fileJms : currDir + "/" + fileJms) ; // PRQA S 3380, 3385
	// MP *=*=*=
					}
					else
					{
						ACS_LOG_ERROR("Inventory of " << fileWithPathDO << " failed.") ;
					}	

					isTheFileCorrectlyUploaded = true;	

				}

			}	// end if able to execute inventory

			if(_rmFlag == false)
			{
							
				string fileToRemove1 ; 
				if( not _deleteUploadedFilepathsOnSuccess ) {
					fileToRemove1 = currDir + "/" + elem; 
					_filesToBeRemovedList.push_back(fileToRemove1);
				}

				string fileToRemove2 = currDir + "/" + fileNewExt; 
				_filesToBeRemovedList.push_back(fileToRemove2);
				
				if (File::exists(fileJms)) {
					_filesToBeRemovedList.push_back(currDir + "/" + fileJms);
                }
				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "list of file to remove: " 
                        << ( fileToRemove1.empty() ? "" : fileToRemove1 + ", " ) << fileToRemove2 << ", " 
						<< (currDir + "/" + fileJms)) ;
			}
		}
		catch(exception & itex)
		{
			try {
				moveFileInBackupDir(currDir,elem);
			}
			catch( exception &ee) 
			{
				ACS_LOG_NOTIFY_EX(ee);
				ACS_LOG_WARNING("Cannot even move file in backup dir." );
			}
			
			_rmFlag = true;
			ACS_LOG_NOTIFY_EX(itex);
            ACS_LOG_ERROR("an error occurred on an element in the LIST. Continuing on following elements") ;
		}
		
	}	// end of while loop on the inventory list
	

	// Remove from working dir the files succesfully uploaded
	if( currDir != "./")
	{
        size_t ftrsize = _filesToBeRemovedList.size() ;
		for (size_t itr = 0 ; itr < ftrsize; ++itr)
		{
			string el = _filesToBeRemovedList[itr];
			try {
				try {
					File::unlink(el) ;
				} 
				catch(exception &x) {
					ACS_THROW(FileInventoryException(x,"cannot remove file " + el + " from working dir (permission ?)"));  // PRQA S 3081
				}
			}
			catch(exception &e) {
				ACS_LOG_NOTIFY_EX(e) ;
			}

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"file " << el << " removed from working dir") ;
		}	
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"files not removed because app is working on local dir") ;
	}
			
	// Write on disk the backup list (if not empty)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"writing on disk the backup list if size<>0... size is " << _backupList.size()) ;
	
	if(_backupList.size()!=0 && (!_backupDir.empty()) )
	{
		_allFilesSuccesfullyInventoried = false;
		
		ofstream outfile;
		string bckDir = _backupDir +"/" + curFilNam; 

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"writing the backup list on " << bckDir) ;
		
		outfile.open(bckDir.c_str(),ofstream::out);
        size_t bcklsize = _backupList.size() ;
		for(size_t itera = 0 ; itera < bcklsize; ++itera)
		{
			string el = _backupList[itera] ;
			outfile << el << "\n";
		}	
		outfile.close();
		
		_backupList.clear() ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"backup list written on " << bckDir) ;
	}
	
	// Removing list and related lock file from working directory 
	removeListAndLockFile(invFileListName);
		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"method executed ") ;
		
}

void FileInventory::lockDbTable(const std::string& table, const std::string& mode, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();
	
	std::string lockCommand = "LOCK TABLE " + table + " IN " + mode + " MODE";
	
	SQLString lockQueryString(dbKey, SQLString::COMMAND);
	lockQueryString.forceSQLString(lockCommand);
	
	dbQuery query(conn);
	query.sqlString(lockQueryString, "LockTable_" + table);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << query.sqlString());
	
	if (!query.execForStatus()) {
		ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + lockQueryString.getSQL() + "\"Error was: " + std::string(query.errorString())));
	}
}

dbPersistent::IdType FileInventory::insertOriginator(const std::string& originatorName, dbConnection& conn) {
	// t_originators ids are not serials, in transaction search for the highest id and insert the new one.
	
	const std::string& dbKey = conn.getKey();
	
	db::Transaction tr(conn);
	tr.begin_work("InsertOriginatorTransaction");
	
	// Acquire the lock on t_originators in order to avoid concurrency when retrieving the maximum inserted id and inserting the new record
	// The locks is released when the transaction ends.
	lockDbTable("t_originators", "EXCLUSIVE", conn);
	
	dbPersistent::IdType newId = 0;
	
	SQLString sql(dbKey, SQLString::SELECT);
	sql.addTableList("t_originators");
	sql.addSelectList("id");
	sql.addOrderByList("id", SQLString::DESC);
	sql.setLimit(1);
	
	dbQuery query(conn);
	query.sqlString(sql, "GetOriginatorMaxId");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: " << query.sqlString());
	
	dbSet set;
	query.execForSet(set);
	
	if (!set.empty()) {
		dbRow row(dbKey);
		set.getNextRow(row);
		row.column(0, newId);		
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Table t_originators is empty.");
	}
	
	newId += 1;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inserting new id " << newId << " in table t_originators");
	
	dbSmartPersistent sp("t_originators", conn);
	sp.setId(newId);
	sp.set("name", originatorName, conn);
	sp.save(conn);
	
	tr.commit();
	
	return newId;
}

void FileInventory::fillObjOptional(const Metadata &metadata, dbInventoryObjectTEC &obj)
{
	string fname ;
	// they are here after the copy ... 
	obj.setFileName(checkMandatoryField(fname, metadata, "File_Name"));
    string fvers ;
	obj.setFileVersion(checkMandatoryField(fvers, metadata, "File_Version"));
	
    string tmp ;
    //Unique_Id  : optional name for identifing inventoried file for subsequent operation
    if (checkOptionalField(tmp,metadata,"Unique_Id")) {
        obj.setUniqueId(tmp); 
    }
	else if ( _setDefaultUniqueId ) {
		// S3MPM-659: Set the default uniqueid in case it is not set in the metadata
		obj.setUniqueId (fname+fvers);
	}
	else {
		// do nothing: unique_id not set
	}

    //Logical_Id  : optional name 
    if (checkOptionalField(tmp,metadata,"Logical_Id")) {
        obj.setLogicalId(tmp); 
    }

    //Parent_Id  : optional name for product parent name if any
    if (checkOptionalField(tmp,metadata,"Parent_Id")) {
        obj.setParentId(tmp); 
    }

    //Group_Id  : optional name for logical grouping of more products 
    if (checkOptionalField(tmp,metadata,"Group_Id")) {
        obj.setGroupId(tmp); 
    }

    //Inventory_Extra  : optional extra parameters field 
    if (checkOptionalField(tmp,metadata,"Inventory_Extra")) {
        obj.setExtraInfo(tmp); 
    }
	

	// Handle Inventory Additional Parameters	
	if (	(true == checkOptionalField(tmp,metadata,Metadata::_mtdkey_Session_Id)) \
		|| 	(true == checkOptionalField(tmp,metadata,Metadata::_mtdkey_Granule_Number)) \
		|| 	(true == checkOptionalField(tmp,metadata,Metadata::_mtdkey_Granule_Position)) \
		|| 	(true == checkOptionalField(tmp,metadata,Metadata::_mtdkey_Dump_Start)) \
		|| 	(true == checkOptionalField(tmp,metadata,Metadata::_mtdkey_Mission_Data_Take_Id)) \
		|| 	(true == checkOptionalField(tmp,metadata,Metadata::_mtdkey_Next_Equator_X_Time)) \
	)
	{
		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw; // PRQA S 3
		map <string, string> addParams_M ; 

		string key = Metadata::_mtdkey_Session_Id ; if ( true == checkOptionalField(tmp,metadata,key) )
		{ addParams_M.insert (pair<string, string>(key, metadata.getMetadataValue(key))) ; }

		key = Metadata::_mtdkey_Granule_Number ; if ( true == checkOptionalField(tmp,metadata,key) )
		{ addParams_M.insert (pair<string, string>(key, metadata.getMetadataValue(key))) ; }

		key = Metadata::_mtdkey_Granule_Position; if ( true == checkOptionalField(tmp,metadata,key) )
		{ addParams_M.insert (pair<string, string>(key, metadata.getMetadataValue(key))) ; }

		key = Metadata::_mtdkey_Dump_Start ; if ( true == checkOptionalField(tmp,metadata,key) )
		{ addParams_M.insert (pair<string, string>(key, metadata.getMetadataValue(key))) ; }

		key = Metadata::_mtdkey_Mission_Data_Take_Id ; if ( true == checkOptionalField(tmp,metadata,key) )
		{ addParams_M.insert (pair<string, string>(key, metadata.getMetadataValue(key))) ; }

		key = Metadata::_mtdkey_Next_Equator_X_Time ; if ( true == checkOptionalField(tmp,metadata,key) )
		{ addParams_M.insert (pair<string, string>(key, metadata.getMetadataValue(key))) ; }

		obj.setAddParams(addParams_M, conn) ; 
	}

    //Quality_Value: a floating point added for S2 purposes 
	float qvalue = 0.0 ;
	if (checkOptionalField(qvalue,metadata,"Quality_Value")) {
		obj.setQualityValue(qvalue);
    }



    //OPTIONAL FIELDS:	
	if (checkOptionalField(tmp,metadata,"Source")) {
		obj.setSource(tmp);
    }


	if (checkOptionalField(tmp,metadata,"Ascending_Flag")) {
		bool ascendingFlag = false;		
		switch(tmp[0]) {
			case 'A': 
			case 'a': 
                {
				    ascendingFlag = true;
				    break ;
                }
			case 'D':
			case 'd':
                {
				    ascendingFlag = false;
				    break ;
                }
			default: // neither A nor D
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: The optional field Ascending_Flag was set by default to false") ;
                    break ;
                }	
		}

		obj.setAscendingFlag(ascendingFlag);
	}

	float equatorXLongitude = 0 ;
	if (checkOptionalField(equatorXLongitude,metadata,"Equator_X_Longitude")) {
		obj.setEquatorXLongitude(equatorXLongitude);
    }

	int orbit = 0 ;
	if (checkOptionalField(orbit,metadata,"Start_Orbit_Number")) {
		obj.setStartOrbitNumber(orbit);
    }
	DateTime equatorXTime ;
	if (checkOptionalField(equatorXTime,metadata,"Equator_X_Time")) {
		obj.setEquatorXTime(equatorXTime);
    }

	if (checkOptionalField(orbit,metadata,"Stop_Orbit_Number")) {
		obj.setStopOrbitNumber(orbit);
    }


	int station = 0 ;
	if (checkOptionalField(station,metadata,"Acquisition_Station_Id")) {
		obj.setAcquisitionStationId(station);
    }

	if (checkOptionalField(station,metadata,"Processing_Station_Id")) {
		obj.setProcessingStationId(station);
    }

	if (checkOptionalField(tmp,metadata,"Sensor_Mode")) {
		obj.setSensorMode(tmp);
    }

	if (checkOptionalField(tmp,metadata,"Source_Sw_Version")) {
		obj.setSourceSwVersion(tmp);
    }

	if (checkOptionalField(tmp,metadata,"Phase")) {
		obj.setPhase(tmp);
    }

	if (checkOptionalField(tmp,metadata,"Quality_Info")) {
		obj.setQualityInfo(tmp);
    }

	float cloudpctg = 100 ;
	if (checkOptionalField(cloudpctg,metadata,"CloudPercentage")) {
		obj.setCloudPctg(cloudpctg);
    }

	if (checkOptionalField(tmp,metadata,"ProductSpecification")) {
		obj.setProdSpec(tmp);
    }

	int OrderId = 0 ;
	if (checkOptionalField(OrderId,metadata,"Order_Id")) {
		obj.setOrderId(OrderId);
    }

	short qualified = 0 ;
	if (checkOptionalField(qualified,metadata,"Qualified")) {
		obj.setQualified(qualified);
    }

	int band = 0 ;
	if (checkOptionalField(band,metadata,"Band")) {
		obj.setBand(band);
    }

	short tdi = 0 ;
	if (checkOptionalField(tdi,metadata,"Tdi")) {
		obj.setTdi(tdi);
    }

    if (checkOptionalField(tmp,metadata,"Baseline")) {
       obj.addBaselineId ( Baselines::instance(_connectionPool.getKey())->id (tmp) );
    }
	
    if (checkOptionalField(tmp,metadata,"Relation")) {
       obj.setRelation(tmp) ;
    }
	
    if (checkOptionalField(tmp,metadata,"RelationType")) {
       obj.setRelationType(tmp) ;
    }
 
    if (checkOptionalField(tmp,metadata,"Md5")) {
       obj.setMd5(tmp) ;
    }
 
	// inventory gaps
	vector < Metadata::InventoryGap > invGap;
	metadata.getGaps(invGap) ;
	if(invGap.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field: List_Of_Gaps") ;
	}
    else {
		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw; // PRQA S 3050 
        size_t gsize = invGap.size() ;
		for(size_t i=0; i<gsize; ++i)
		{
			obj.addGap(DateTime((invGap[i].startTime)), DateTime((invGap[i].stopTime)), conn); // PRQA S 3081
		}
    }

	// inventory comments
	vector<Metadata::InventoryComment> invComments;
	metadata.getComments(invComments);
	if(invComments.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field: List_Of_Comments") ;
	}
    else {
		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw; // PRQA S 3
        size_t csize = invComments.size() ;
		for(size_t i=0; i<csize; ++i)
		{
			obj.addComment(invComments[i].text, DateTime(invComments[i].date), invComments[i].author, conn); // PRQA S 3081
		}
    }

	if (checkOptionalField(tmp,metadata,"System")) {
		dbPersistent::IdType origId = 0;
		try {
			origId = OriginatorName::instance(_connectionPool.getKey())->id(tmp);
		} catch (exIllegalValueException& e) { // tmp not found in t_originators
			if (_insertOriginatorIfMissing) {
				dbConnectionWrapper pw(_connectionPool) ;
				dbConnection &conn = pw; // PRQA S 3
				origId = insertOriginator(tmp, conn);
				
				// Reload originators map after insert
				OriginatorName::instance(_connectionPool.getKey())->load();
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Insert originator functionality disabled. Rethrowing exception");
				// rethrow exception
				throw;
			}
		}
				
		obj.setOriginator(origId); // PRQA S 3000
    }


	// inventory measurement data sets
	vector<Metadata::MeasDataSetDescriptor> invMDSs ;
	metadata.getMDSs(invMDSs);
	if(invMDSs.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field: List_Of_MDSs") ;
	}
	else {
		NameIdAssociator<int> measdataset("t_measdataset","Id", "Name", _connectionPool.getKey()) ;
		measdataset.load() ;
        size_t msize = invMDSs.size() ;

		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw; // PRQA S 3
		for(size_t i=0; i<msize; ++i)
		{
			obj.addMDS(measdataset.id(invMDSs[i].Ds_Name,false),atoi(invMDSs[i].Num_Dsr.c_str()), conn) ; 
		}
	}

	if (checkOptionalField(band,metadata,"Band")) {
		obj.setBand(band);
    }
	vector<int> invSites;
	metadata.getSites(invSites);
	if(invSites.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field: List_Of_Sites") ;
	}
	else {
		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw; // PRQA S 3
        size_t sitsize = invSites.size() ;
		for(size_t i=0; i<sitsize; ++i)
		{
			obj.addSite(invSites[i], conn) ; 
		}
	}

	vector<Metadata::GeoPointInfo> metadataGeoPointStruct;

	//Get from metadata the GeoPoint vector
	metadata.getGeoPoints(metadataGeoPointStruct);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"metadataGeoPointStruct size: " << metadataGeoPointStruct.size()) ;

	if (metadataGeoPointStruct.size() > 0) {


		// Get object type from metadata and verify it isn't null
		string objType = metadata.getGeoType();

		ACS_COND_THROW(objType.empty(),   // PRQA S 3081
								  FileInventoryException(
									"Missing metadata optional field: Geo_Type")
								  );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"the metadata must have enough points ... points are " << metadataGeoPointStruct.size()) ;
		dbGeoPntList geoPointListObject;

		// Start a cycle and set a dbGeoList object with all the points coordinates 
		Metadata::GeoPointInfo first ;
        Metadata::GeoPointInfo last ;
		first = metadataGeoPointStruct[0] ;
		last = metadataGeoPointStruct[metadataGeoPointStruct.size()-1] ; // PRQA S 3084

        size_t mgpsize = metadataGeoPointStruct.size() ;
		for (size_t i =0; i < mgpsize; ++i)
		{
			Metadata::GeoPointInfo &el = metadataGeoPointStruct[i] ;
			geoPointListObject.append(dbGeoPnt(atof(el.latitude.c_str()),atof(el.longitude.c_str()))) ;
		}



		ACS_SMARTPTR<dbGeoObject> geoObj ;
		try {
			// Define an appropriate dbGeoObject
			if (objType == "String") {
				geoObj = ACS_SMARTPTR<dbGeoObject>(new dbGeoString(geoPointListObject)) ; // PRQA S 3050, 3081
            }
			else if (objType == "Polygon") {
				geoObj = ACS_SMARTPTR<dbGeoObject>(new dbGeoPolygon(geoPointListObject)) ; // PRQA S 3050, 3081
            }
			else if	(objType == "Ring") {
				geoObj = ACS_SMARTPTR<dbGeoObject>(new dbGeoRing(geoPointListObject)) ; // PRQA S 3050, 3081
            }
			else if	(objType == "Box") 
			{
				ACS_COND_THROW(metadataGeoPointStruct.size() > 2,  // PRQA S 3081
					GeoInventoryException("too many points for Box") ) ;
				geoObj = ACS_SMARTPTR<dbGeoObject>(  // PRQA S 3050, 3081
                         new dbGeoBox(dbGeoPnt(atof(first.latitude.c_str()),atof(first.longitude.c_str())), 
									  dbGeoPnt(atof(last.latitude.c_str()),atof(last.longitude.c_str()))))  ;

			}	
			else if	(objType == "Lineseg")
			{
				ACS_COND_THROW(metadataGeoPointStruct.size() > 2,  // PRQA S 3081
					GeoInventoryException("too many points for Lineseg") ) ;
				geoObj = ACS_SMARTPTR<dbGeoObject>(  // PRQA S 3050, 3081
                     new dbGeoLineseg(dbGeoPnt(atof(first.latitude.c_str()),atof(first.longitude.c_str())), 
									  dbGeoPnt(atof(last.latitude.c_str()),atof(last.longitude.c_str())))) ;
			}	
			else if	(objType == "Point")
			{
				geoObj = ACS_SMARTPTR<dbGeoObject>(new dbGeoPoint(dbGeoPnt(atof(first.latitude.c_str()),atof(first.longitude.c_str())))) ;  // PRQA S 3050, 3081
			}
            else
			{
				ACS_THROW(GeoInventoryException("geo type field \"" + objType + "\" is not allowed"));  // PRQA S 3081
			}
		}
		catch(exception &x) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,x) ;
			ACS_THROW(GeoInventoryException(x, "geographic localization error")) ;  // PRQA S 3081
		}

		int precision = metadata.getGeoCoordsPrecision();
		if( precision > 0 ) {
			geoObj->precision(precision);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"geo object coordinates precision set to " << precision);
		}
		
		try
		{
			DateTime startT(false) ; // ANY
			checkOptionalField(startT,metadata,"Start_Time") ;
			DateTime stopT(false) ; // ANY
			checkOptionalField(stopT,metadata,"Stop_Time") ;

			geoObj->timeRange() = dbGeoTimeRange(startT,stopT) ;

			// Set the dbInventory object Geographic_Localization field
			obj.setGeographicLocalization(*geoObj);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"geo object sql: \"" << geoObj->getSqlString(SQLDialectFactory::dialectName(_connectionPool.getKey())) << "\"") ;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"geo object set with the requested shape ("<< objType	<< ")");
		}
		catch(exception& et)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, et) ;
			ACS_THROW(FileInventoryException(et, "Cannot set time in geo object")) ;  // PRQA S 3081
		}			
	}
	else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field: Geographic_Localization") ;
	}
}

void FileInventory::fillObjMandatory(const Metadata &metadata, dbInventoryObjectTEC &obj)
{
	//MANDATORY FIELDS:

	string tmp;

	if(checkField(tmp, metadata, "File_Type", true)) { 
        obj.setFileType(tmp); 
    }
	if(checkField(tmp, metadata, "File_Class", true)) {
        obj.setFileClass(tmp); 
    }
	unsigned long long  headerSize = 0 ;
	if(checkField(headerSize,metadata,"Header_Size", true)) {
		obj.setHeaderSize(headerSize);
    }
	unsigned long long dataSize = 0 ;
	if(checkField(dataSize,metadata,"Data_Size", true)) {
		obj.setDataSize(dataSize);
    }
	DateTime t ;
	if(checkField(t,metadata,"Validity_Start", true)) {
		obj.setValidityStart(t);
    }
	if(checkField(t,metadata,"Validity_Stop", true)) {
		obj.setValidityStop(t);
    }
	if(checkField(t,metadata,"Generation_Time", true)) {
		obj.setGenerationTime(t);
    }
	if(checkField(t,metadata,"Validation_Date", true)) {
		obj.setValidationDate(t);
    }

	int satelliteId = -1;
	if (checkOptionalField(satelliteId,metadata,"Satellite_Id")) {// != -1) {
		if (true) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Satellite set to value " << satelliteId) ;
        }
		obj.setSatelliteId(satelliteId);
	} else if (true) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Satellite undefined.") ;
    }
    else {
        // nothing 
    }

	if(satelliteId != -1)		
	{
		int sensorId = -1 ;
		if (checkOptionalField(sensorId,metadata,"Sensor_Id")) {
			obj.setSensorId(sensorId);
        }

		int mission = -1 ;
		if (checkOptionalField(mission,metadata,"Mission")) {
			obj.setMission(mission);
        }
	}
}

// Fill the dbInventoryObjectWithGaps
string FileInventory::fillObj(Metadata &metadata, dbInventoryObjectTEC & obj)
{	
    // fill mandatory fields
    fillObjMandatory(metadata, obj ) ;

    // fill optional fields
    fillObjOptional(metadata, obj ) ;

    string productversion = obj.getFileVersion() ;

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        try {
		    oftmpstream xml("/tmp/InvObj",".xml") ;
		    ACS_LOG_DEBUG(SimpleDebugSignature << "writing InventoryObj in " << xml.path()) ;
		    xml.setAutoRemove(false) ;
            ofstream xmlfile(xml.path().c_str());
	        XMLOstream xst(xmlfile, "InvObj");
	        obj.exportXML(xst);
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
	ACS_CLASS_END_DEBUG
        
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method executed") ;
    return productversion ;
}


int FileInventory::localMoveInDir(const string &inFile,const string &outFile) // PRQA S 4211
{

	// Patch to disable the backup directory management
	// in such cases outFile is the _backupDir and if it is set to empty string
	// just return zero to simulate that the file has been moved 
	if (outFile.empty() )
	{
		ACS_LOG_INFO ("Move destination path not set. No move will be performed.");
		// Move to empty path.. return zero
		return 0;
	}

#warning da controllare
// MP occhio
	int status=0;
	string currdir = File::getDirPath(inFile) ;
	if( currdir == _workingDir )
	{
		bool isLink = File::isSymbolicLink(inFile); // symbolic link ?
		string tmp = File::readlink(inFile);
		string command = "mv " + tmp + " " + outFile; 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing \"" << command << "\"") ;
		status = ::system(command.c_str());
		if(isLink) {// was a link ?
			::unlink(inFile.c_str());	// remove it
        }
	}
	else
	{
		string tmp = outFile + "/" + inFile ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing hard link (or copy) [" << inFile << "] -> [" << tmp << "]") ;
		try {
			File::hardlink(inFile, tmp, true);
		}
		catch(exception &x) {
			ACS_LOG_NOTIFY_EX(x) ; 
			status = 1;
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "status is " << status << " errno " << errno) ;
	}
	return(status);
}


void FileInventory::moveFileInBackupDir(const string &currdir, const string &elem)
{

    _allFilesSuccesfullyInventoried = false ;
    
    if ( ! _backupDir.empty() )
    {
            string tmpFile = currdir + "/" + elem;
            // Move the file (if there is a link the linked file will be moved) in the backup directory
            ACS_LOG_INFO("Moving " << tmpFile << " to " << _backupDir) ;
            int returnVal = localMoveInDir(tmpFile,_backupDir); 
            
            _backupList.push_back(elem);
            if(returnVal != 0)
            {
                try{
                    ACS_THROW(FileInventoryException("Cannot move file " + tmpFile + " in backup dir (permissions ?)"));  // PRQA S 3081
                }
                catch(exception& ex){
                    ACS_LOG_NOTIFY_EX(ex) ;
                    throw ;
                }
            }

            // Move the metadata in the backup directory
            string fileNewExt ;
            string fileJms ;
            {
                fileNewExt  = elem + "." + _mtd;
                fileJms		= elem + "." + _jms;
            }
                
            ACS_LOG_INFO("Moving " << currdir << "/" << fileNewExt << " to " << _backupDir) ;
            returnVal = localMoveInDir(currdir + "/" + fileNewExt,_backupDir); 

            
            if(returnVal != 0)
            {
                try{
                    ACS_THROW(FileInventoryException("File: "+fileNewExt+ " not moved in backup dir."));  // PRQA S 3081
                }
                catch(exception& ex){
                    ACS_LOG_NOTIFY_EX(ex);
                    throw ;
                }
            }

            // Move the jms in the backup directory if any
            if (File::exists(currdir + "/" + fileJms)) {
                ACS_LOG_INFO("Moving " << currdir << "/" << fileJms << " to " << _backupDir) ;
                returnVal = localMoveInDir(currdir + "/" + (fileJms),_backupDir); 

                
                if(returnVal != 0)
                {
                    try{
                        ACS_THROW(FileInventoryException("File: " +fileJms+ " not moved in backup dir."));  // PRQA S 3081
                    }
                    catch(exception& ex){
                        ACS_LOG_NOTIFY_EX(ex);
                        throw ;
                    }
                }
            }
    }
}

// Rename and move the file .LIST in the backup dir and remove the file .lock  (INPUT: LIST FILE NAME with path) AAA
void FileInventory::moveListAndLockFile(const string &elem)
{
	_allFilesSuccesfullyInventoried = false;
	
	string wrongListName = File::removeExt(elem) + ".ERRLIST"; 
	string lockFileName = File::removeExt(elem) + ".lock"; 
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "moving in backup dir " << elem 
		    << " list ("	<< wrongListName << ") and delete the lock file " << lockFileName 
		    << "..." ) ;
	
	int returnVal = localMoveInDir(elem,wrongListName); 
	if(returnVal != 0)
	{
		ACS_THROW(FileInventoryException("File " + elem + " not renamed in " + wrongListName)); // PRQA S 3081
	}
	
	returnVal = localMoveInDir(wrongListName,_backupDir); 
	if(returnVal != 0)
	{
		ACS_THROW(FileInventoryException("Cannot move file " + elem +" (" + wrongListName + ") in backup dir")); // PRQA S 3081
	}

	// remove the lock file of the .LIST file from the working directory 
	try {
		File::unlink(lockFileName) ;
	} 
	catch(exception &x) {
		ACS_THROW(FileInventoryException(x, // PRQA S 3081
					   "Cannot remove file " + lockFileName + " from working dir"));
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method executed") ;
}


void FileInventory::moveJmsFile(const string &f) throw ()  // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file: \"" << f << "\"") ;

	if (!File::exists(f)) {	return ; }
		
	rsResourceSet &_rsConf = ConfigurationSingleton::instance()->get();

	string jmsDestination ;
	
	try {
		_rsConf.getValue("Inventory.JmsDestination" , jmsDestination);
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
		ACS_LOG_ERROR("The file: " << f << " was not moved in final destination because the above exception.")
		return ;
	}

	string command = "mv " + f + " " + jmsDestination; 
	try {
		ACS_COND_THROW(::system(command.c_str()) != 0, // PRQA S 3081
								  FileInventoryException("File " + f + " not moved."));
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
		ACS_LOG_ERROR("The file: " << f << " was not moved in final destination because the above exception.") ;
	}
}

// Remove the file .LIST and the file .lock  (INPUT: LIST FILE NAME with path) 
void FileInventory::removeListAndLockFile(const string &elem)
{
	string lockFileName = File::removeExt(elem) + ".lock"; 
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing " << elem << " list and delete the lock file " << lockFileName << "...") ;
	
	try {
		File::unlink(elem) ;
	} 
	catch(exception &x) {
		ACS_THROW(FileInventoryException(x,"Cannot remove file " + elem + " from working dir (permission ?).")); // PRQA S 3081
	}
	
	try {
		File::unlink(lockFileName) ;
	} 
	catch(exception &x) {
		ACS_THROW(FileInventoryException(x,"Cannot remove file " + lockFileName + " from working dir (permission ?).")); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method executed") ;
}


// This method check if the files belonging to the list are right, if not so it sets a flag... it returns also a list of good files
void FileInventory::checkFiles(const string &currdir, const string &elem, vector<string>& goodFilesList)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking the files of the list... in " << currdir) ;

	string ext 				= File::getExt(elem); 	
	string fileWithPathDO   = (currdir == "./" ? elem : (currdir + "/" + elem)) ; // PRQA S 3384, 3385
		
	string fileNewExt ;
	{
		fileNewExt = elem + "." + _mtd;
	}

	string fileWithPath = (currdir == "./" ? fileNewExt : (currdir + "/" + fileNewExt)); // PRQA S 3384, 3385				

	if (File::exists(fileWithPath) && File::exists(fileWithPathDO)) 
	{
		goodFilesList.push_back(elem);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Files " << fileWithPath << " and " << fileWithPathDO << " found.") ;
	}
	else
	{
		ACS_LOG_TRYCATCHNOTIFY_EX(FileInventoryException("Files " + fileWithPath + " and " + fileWithPathDO + " not found")) ; // PRQA S 3081
		_isTheListGood = false;
	}	
}

// This method try to move all the file belonging to the wrong list to the backup dir
void FileInventory::wrongList(const string &currdir, const string &elem)
{
	string fileWithPathDO   = (currdir + "/" + elem);			// ./_targetDir/pippo.DBL or ./_targetDir/pippo.OBJ
	string ext 				= File::getExt(elem);
	string fileNewExt="";
	
	fileNewExt = elem + "." + _mtd;
		
	string fileWithPath = (currdir + "/" + fileNewExt);		// ./_targetDir/pippo.MTD
			
	// Move the right file in the backup directory
	int returnVal = localMoveInDir(fileWithPathDO,_backupDir); 
	if(returnVal != 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File " << fileWithPathDO << " not moved to the backup dir...") ;
	}

	returnVal = localMoveInDir(fileWithPath,_backupDir); 
	if(returnVal != 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File " << fileWithPath << " not moved to the backup dir...") ;
	}
}


const std::string &FileInventory::failedOrders() const throw() // PRQA S 4120
{
	return _failedOrders ;
}

void FileInventory::relaxFNumberCheck(bool v) throw() // PRQA S 4121
{
	_relaxFnCheck = v ;
}

void FileInventory::restoreOnSync(bool v) throw() // PRQA S 4121
{
	_restoreOnSync = v ;
}

void FileInventory::noActions(bool v) throw() // PRQA S 4121
{
	_noActions = v ;
}

_ACS_END_NAMESPACE
