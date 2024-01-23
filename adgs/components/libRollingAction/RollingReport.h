// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libRollingAction$

	$Id$

	$Author$

	$Log$
	Revision 2.14  2018/03/26 13:21:39  nicvac
	S2PDGS-1966: Report session: report records can be committed only at checkpoints.
	
	Revision 2.13  2018/02/06 14:52:45  nicvac
	S2PDGS-1851: Report generation during items rolling.
	
	Revision 2.12  2018/02/05 18:25:00  nicvac
	S2PDGS-1851: Report generation agent.
	
	Revision 2.11  2018/01/30 18:50:11  marpas
	trying to optimise dbConnection use
	qa warnings partially removed
	coding best practice application in progress
	
	Revision 2.10  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
	Revision 2.9  2016/01/28 15:59:29  nicvac
	S2PDGS-1400: Test and fixing loop.
	
	Revision 2.8  2016/01/27 16:20:42  nicvac
	S2PDGS-1400: limit on items in report.
	
	Revision 2.7  2016/01/21 17:05:31  nicvac
	S2PDGS-1394: Report configurable on item id
	
	Revision 2.6  2015/03/10 10:54:23  francesco.avanzi
		New action "Substitute" added
	
	Revision 2.5  2015/01/21 18:31:53  nicvac
	S2PDGS-1047
	
	Revision 2.4  2014/12/17 15:56:21  nicvac
	S2PDGS-978
	
	Revision 2.3  2014/12/03 14:48:36  nicvac
	S2PDGS-897
	
	Revision 2.2  2014/11/28 16:18:28  nicvac
	S2PDGS-897
	
	Revision 2.1  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
        
*/

#ifndef _RollingReport_H_
#define _RollingReport_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <set>

#include <RegExp.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Report about the actions performed on the inventory items.
* Each rolled item is kept in report
* After rolling the report is created.
* Note: This could happen:
* - During rolling operation, each rolled item is kept in report.
*   If crash during rolling, the report of current operation is lost.
*   To prevent this, each record request append the record in a private file.
*   At report creation time, the records are loaded from private file,
*   the report is created, the private file is erased. In this way items recorded during crash
*   will be reported in report at next report creation.
**/

class RollingReport  { // PRQA S 2109
public:

	/** RollingReport Exceptions */
	exDECLARE_EXCEPTION(exRollingReportCriticalException, exCriticalException); // RollingReport Critical Exception. // PRQA S 2131, 2153, 2502

	/** Class constructor
	 * - workingDir: a path where I can put my private files. If set to "" ==> null report mode: no report is used
	 * - includeItemRe: include only items matching the reg exp. if set to "" ==> do not apply this filter
	 * - excludeItemRe: ignore all items matching the reg exp. if set to "" ==> do not apply this filter
	 * */
	explicit RollingReport(std::string const& workingDir, size_t maxItemsPerReport=35000, std::string const& includeItemRe="", std::string const& excludeItemRe="");
	/** Destructor */
	virtual ~RollingReport();

    /** types **/
    
	/** Removed: item deleted */
	enum Action { unset=-1, removed, invalidated, offline, online, substitute };

	/** The report additional Infos */
	typedef std::map< std::string, std::string > Info;

	/** Each record contains: Inventory Unique id (from t_inventory.unique_id) - Action, Other info */
	typedef std::string InvUniqueId; 

	/** The record additional Infos */
	typedef std::map< std::string, std::string > InfoRecord; // PRQA S 2100

	struct RecordContent { // PRQA S 2173, 2175
		RecordContent(): action(unset), infoRecord() {}
		RecordContent(const RecordContent &r): action(r.action), infoRecord(r.infoRecord) {}
	    ~RecordContent() throw() {}
		RecordContent& operator=(const RecordContent &r) {
            if (this != &r) {
                action = r.action ;
                infoRecord = r.infoRecord ;
            }
            return *this ;
        }
		Action action; // PRQA S 2100 2
		InfoRecord infoRecord;
	};


	/** Getters and setters */
	Info& getInfo() { return _info;	} // PRQA S 4024
	void setInfo(const Info& info) {_info = info; }

	/** Disk mode. Open a record session */
	void recordSessionOpen();
	void recordSessionClose();

	/** Disk mode. Append a new record in the private file */
	void record( InvUniqueId const& invUniqueId, Action action);
	void record( InvUniqueId const& invUniqueId, Action action, InfoRecord const& infoRecord );

	/** Disk mode. Set the private file as 'to be processed' */
	void recordCommit();

	/** Disk mode. Load records from private file and return them */
	std::map<InvUniqueId, RecordContent> const& loadAndGetRecords( const std::string& binaryRecordsFilename );

	/** Serialize the whole report */
	void serialize(std::ostringstream& report) ;

	/** Serialize part of the report
	 * In:
	 *  - it: start iterator
	 *  - HowMany: how many elements to serialize. If limit exceeds, serialization is up to last element.
	 * Out:
	 *  - it: next iterator of the serialized one.
	 *  */
	void serialize(std::ostringstream& report, size_t howMany, std::map< InvUniqueId, RecordContent >::iterator& it );

	/** Init for report dump */
	void dumpInit();
	/** Dump the report.
	 * In:
	 * - filename: filename to write.
	 * - howMany: how many items to dump.
	 * return: true if some records have been dumped
	 * */
	bool dump(const std::string& filename, size_t howMany);
	/** End of dump:  Delete the private file */
	bool dumpCommit( const std::string& binaryRecordsFilename );

	/** Get the total number of records in report */
	size_t getNumberOfRecords() const { return _records.size(); }

	/** Summary Info: number of rolled items during this rolling loop */
	size_t getSummaryCurrentLoopNumRecords() const { return _summaryCurrentLoopRecords.size(); }

	/** Summary Info: Reset number of rolled items during this rolling loop. */
	void resetSummaryCurrentLoopNumRecords() { _summaryCurrentLoopRecords.clear(); }



public:

	/** Ram mode: do not use this with rolling. */
	void recordUnsafe( InvUniqueId const& invUniqueId, Action action  );
	void recordUnsafe( InvUniqueId const& invUniqueId, Action action, InfoRecord const& infoRecord );

private: 
	RollingReport(); // not implemented 
	RollingReport(const RollingReport & ); // not implemented 
	RollingReport &operator=(const RollingReport &); // not implemented 

public:

	/** The Info keys */
	static const std::string mission; // PRQA S 2100 12
	static const std::string fileType;
	static const std::string fileClass;
	static const std::string fileName;
	static const std::string valStart;
	static const std::string valStop;
    static const std::string system;
    static const std::string creator;
    static const std::string creatorVersion;
    static const std::string creationDate;
    static const std::string centre;
    // Define how the pdi id is formatted in report. You can use the inventory fields: ${uniqueid}, ${filename}, ${fileversion}.
    // If not set, used ${uniqueid}
    //Some Examples:
    // - "${uniqueid}"
    // - "<File_Name>${filename}</File_Name><File_version>${fileversion}</File_version>"
    // - "${filename}"
    static const std::string pdiIdTemplate; // PRQA S 2100
    /** End Info keys*/

	/** The InfoRecord keys */
	static const std::string rFilename; // PRQA S 2100 2 
	static const std::string rFileversion;

    static const int buffer_size = 400 ;

private:

    /** Current internal records' file can be committed only if current session is closed
     * Note: this is used because the same inventory Item can be recorded twice in a session: first time as deleted, second time as off-file (cf. Actions).
     * The last action on the same item wins. With session policy, all the actions on the same item will be recored in the same internal records' file. */
    bool _sessionClosed;

	/** Records */
	std::map< InvUniqueId, RecordContent > _records;

	/** Main report Info */
	Info _info;

	/** Private file keeping records */
	std::string _privateFile;

	/** Null report mode */
	bool _nullReportMode;

	/** Maximum number of rolled items that a single report file can contain */
	size_t _maxItemsPerReport;

	/** Summary Info: number of rolled items during this rolling loop
	 * Used a set to keep distinct items called in 'record'. record can be called more than one time for the same item (cf. Actions) */
	std::set< InvUniqueId > _summaryCurrentLoopRecords;

	/** Counter of items stored in the current internal binary report */
	size_t _countCurrentInternal;

	/** Cf. constructor */
	RegExp _reIncludeItem;
	RegExp _reExcludeItem;

	/** Remember the last dumped record */
	std::map< InvUniqueId, RecordContent >::iterator _recToDump;


private:
	/** true if the item has to be written in the report */
	bool isItemToBeWrittenInReport(const InvUniqueId& invUniqueId) const;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingReport)

};


/** Automatically open and close a record session
 * Report note: Report's current internal records' file can be committed only if current record session is closed.
 * Note: session is important because the same inventory Item can be recorded more than once  in a session.
 * E.g. first time as deleted, second time as off-file (cf. Actions).
 * The last action on the same item wins. With session policy, all the actions on the same item will be recorded in the same internal records' file. */

class RollingReportAutoSession {
public:
	RollingReportAutoSession(RollingReport& rollingReport);
	~RollingReportAutoSession();
private:
	RollingReport& _rollingReport;
};


_ACS_END_NAMESPACE

#endif //_RollingReport_H_

