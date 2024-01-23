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

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.19  2017/04/07 10:39:39  marfav
	APF-412
	SymbolTable modified to allow a list of linked filetypes for each query
	
	Revision 1.18  2017/02/02 16:09:02  marfav
	CSGACQ-110
	Default expiration date is not filled
	
	Revision 1.17  2017/01/16 13:25:32  marfav
	Enforcing cpp coding rules
	
	Revision 1.16  2016/04/11 13:30:51  marpas
	adopting new db interfaces
	
	Revision 1.15  2016/03/26 21:40:46  marpas
	using new db I/F
	
	Revision 1.14  2016/03/08 10:41:13  nicvac
	ALSAT1B-72: Record Inputs for Order
	
	Revision 1.13  2016/03/07 11:39:25  nicvac
	ALSAT1B-72: Record Input of Order
	
	Revision 1.12  2015/11/20 11:57:18  marpas
	coding best practices application in progress
	
	Revision 1.11  2015/06/16 13:09:25  damdec
	Compilation warnings solved.
	
	Revision 1.10  2013/02/21 13:43:56  marfav
	Compiler warnings removed
	
	Revision 1.9  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.8  2011/09/16 14:02:35  marfav
	Adding Stream tag support to alternatives
	
	Revision 1.7  2010/03/23 17:52:42  marfav
	Added expiration date support to queries and symbol table
	
	Revision 1.6  2009/09/10 13:51:12  marfav
	Added support for query results and time line coverage in the SymTable definition
	
	Revision 1.5  2008/11/07 15:40:53  marfav
	Added _deltaT in parameters
	
	Revision 1.4  2008/09/09 15:42:11  marfav
	Added support to mandatory flag in descriptors
	
	Revision 1.3  2008/04/30 15:57:51  marfav
	Added temporary symbol table to manage alternatives
	
	Revision 1.2  2008/04/29 12:45:40  marfav
	Added support for PreconditionCheckFactoryChain
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#ifndef _PCC_BASE_H_
#define _PCC_BASE_H_

#include <exException.h>

#include <TaskTableVisitor.h>
#include <tlOrder.h>
#include <InputAlternative.h>

#include <dbGeoTimeRange.h>

#include <StringKey.h>

#include <map>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class PCC_Base : public TaskTableVisitor // PRQA S 2109, 2153
{
public:

	exDECLARE_EXCEPTION (PCC_Base_Exception, exException) ; // PRQA S 2131, 2502

	struct InputFileName // PRQA S 2173, 2175
	{
		std::string fileName; // PRQA S 2100 2 
		std::string fileVersion;
        
        InputFileName() :
            fileName(),
            fileVersion()
        {}
        InputFileName(const InputFileName &r) :
            fileName(r.fileName),
            fileVersion(r.fileVersion)
        {}
        InputFileName& operator=(const InputFileName &r) 
        {
            if (this != &r) {
                fileName = r.fileName ;
                fileVersion = r.fileVersion ;
            }
            return *this ;
        }
        bool operator<(const InputFileName &r) const //This to supports sort/unique stl functions
        {
        	bool isLess(false);
        	if ( fileName < r.fileName ) {
        		isLess = true;
        	} else if ( fileName == r.fileName ) {
        		isLess = ( fileVersion < r.fileVersion );
        	}
        	return isLess;
        }
        ~InputFileName () throw() {}
        
	};

	struct TimelineDetails // PRQA S 2173, 2175
	{
		int fileNameIdx; // PRQA S 2100 2 
		acs::dbGeoTimeRange timeRange;
        
        TimelineDetails() :
            fileNameIdx(),
            timeRange()
        {}
        TimelineDetails(const TimelineDetails &r) :
            fileNameIdx(r.fileNameIdx),
            timeRange(r.timeRange)
        {}
        TimelineDetails& operator=(const TimelineDetails &r) 
        {
            if (this != &r) {
                fileNameIdx = r.fileNameIdx ;
                timeRange = r.timeRange ;
            }
            return *this ;
        }
        ~TimelineDetails () throw() {}
	};

	struct SymTableParams // PRQA S 2173, 2175
	{
		bool checkPassed; // PRQA S 2100 L1
		std::vector< InputFileName > fileList;
		std::vector< TimelineDetails > fileTimeline;
		std::string queryTrigger;
		DateTime expirationDate;
		bool isActive; 
		DateTime activationDate;
		bool toBeDownloaded;
		bool isMandatory;
		bool isStream;
		std::vector < std::string > linkedFileTypes;
		// Added in ECICMF is used to place the files in the JobOrder
		// without actually downloading it in the WD
		bool virtualDownload;
		// Added since CRYO2PDS-130
		DateTime queryStartTime;
		DateTime queryStopTime;
        // PRQA L:L1
        
        SymTableParams() :
			checkPassed(),
			fileList(),
			fileTimeline(),
			queryTrigger(),
			expirationDate( DateTime(false) ),
			isActive( true ),
			activationDate ( DateTime(false) ),
			toBeDownloaded(),
			isMandatory(),
			isStream(),
			linkedFileTypes(),
			virtualDownload( false ),
			queryStartTime( false ),
			queryStopTime( false )
        {}
        SymTableParams(const SymTableParams &r) :
			checkPassed(r.checkPassed),
			fileList(r.fileList),
			fileTimeline(r.fileTimeline),
			queryTrigger(r.queryTrigger),
			expirationDate(r.expirationDate),
			isActive(r.isActive),
			activationDate(r.activationDate),
			toBeDownloaded(r.toBeDownloaded),
			isMandatory(r.isMandatory),
			isStream(r.isStream),
			linkedFileTypes(r.linkedFileTypes),
			virtualDownload(r.virtualDownload),
			queryStartTime( r.queryStartTime ),
			queryStopTime( r.queryStopTime )
        {}
        SymTableParams& operator= (const SymTableParams &r) 
        {
            if (this != &r) {
				checkPassed = r.checkPassed ;
				fileList = r.fileList ;
				fileTimeline = r.fileTimeline ;
				queryTrigger = r.queryTrigger ;
				expirationDate = r.expirationDate ;
				isActive = r.isActive;
				activationDate = r.activationDate;
				toBeDownloaded = r.toBeDownloaded ;
				isMandatory = r.isMandatory ;
				isStream = r.isStream ;
				linkedFileTypes = r.linkedFileTypes;
				virtualDownload = r.virtualDownload;
				queryStartTime = r.queryStartTime;
				queryStopTime = r.queryStopTime;
            }
            return *this ;
        }
        ~SymTableParams () throw() {}
       
	};

	typedef std::map<InputAlternative, SymTableParams> SymTable;

	struct PCC_Params // PRQA S 2173, 2175
	{
		tlOrder theOrder; // PRQA S 2100 L2 
		SymTable theSymbolTable;
		SymTable tempSymbolTable;
		double deltaT;
		std::string dbKey ;
		StringKey _factoryKey;
        // PRQA L:L2
		PCC_Params () : 
            theOrder(),
            theSymbolTable(),
            tempSymbolTable(),
            deltaT(),
		    dbKey(),
            _factoryKey()
		{}
        
        PCC_Params (const PCC_Params& p) :
            theOrder(p.theOrder),
            theSymbolTable(p.theSymbolTable),
            tempSymbolTable(p.tempSymbolTable),
            deltaT(p.deltaT),
		    dbKey(p.dbKey),
            _factoryKey(p._factoryKey)
        {}
        
        PCC_Params& operator= (const PCC_Params& p)
        {
            if (&p != this)
            {
                theOrder=p.theOrder;
                theSymbolTable=p.theSymbolTable;
                tempSymbolTable=p.tempSymbolTable;
                deltaT=p.deltaT;
		        dbKey=p.dbKey;
                _factoryKey=p._factoryKey;
            }
            return *this;
        }
        ~PCC_Params () throw() {}
	};
	typedef PCC_Params* PCC_ParamsPtr;

    PCC_Base (const PCC_Base& b);
    PCC_Base& operator= (const PCC_Base& b);

	explicit PCC_Base (const PCC_ParamsPtr& theParameters);
	virtual ~PCC_Base () throw() ;

	bool isCheckPassed() const;
    void setCheckPassed (bool cp) { _checkPassed = cp; }

	bool stopOnFailure() const;
	void stopOnFailure(bool);

	const std::string& getStatus() const throw() ;
    void setStatus ( const std::string& s) { _status = s; }

    PCC_ParamsPtr const & getParameters() const { return _parameters; };


protected:
	virtual void reset();

	// Symbol table management
	bool isAlternativeInSymTable(InputAlternative const& alt, bool active) const;
	SymTableParams getAlternativeValFromSymTable(InputAlternative const& alt, bool active) const;
	void addAlternativeToSymTable(InputAlternative const& alt, SymTableParams const& val, bool active);

	// Temporary symbol table management
	bool isAlternativeInTempSymTable(InputAlternative const& alt, bool active) const;
	SymTableParams getAlternativeValFromTempSymTable(InputAlternative const& alt, bool active) const;
	void addAlternativeToTempSymTable(InputAlternative const& alt, SymTableParams const& val, bool active);

	// Puts the temp symble table in the final one updating the "toBeDownloaded" field
	// and cleans the temporary
	void promoteTempSymbolTable(bool mandatoryFlag, bool inputCheckStatus);
	// update expiration date taking into account of AlternativeActivationTime
	virtual void updateExpirationDate();
private:
    PCC_Base() ; // not implemented


protected:

private:
	// This is the final boolean stating if the check is sucessful or not
    bool _checkPassed;
	bool _stopOnFailure;
	PCC_ParamsPtr _parameters;
	std::string _status;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PCC_Base)
};
	
_ACS_END_NAMESPACE
		       
#endif //_PCC_BASE_H_

