/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
	Revision 2.3  2018/05/15 14:07:31  nicvac
	S2PDGS-1981: report private working dir conf.
	
	Revision 2.2  2018/02/06 14:52:45  nicvac
	S2PDGS-1851: Report generation during items rolling.
	
	Revision 2.1  2018/02/05 18:25:00  nicvac
	S2PDGS-1851: Report generation agent.
	
        
*/

#ifndef _RollingReportGenerator_H_
#define _RollingReportGenerator_H_

#include <RollingReport.h>

#include <DateTime.h>
#include <File.h>
#include <exException.h>

#include <Thread.h>

#include <deque>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Thread generating rolling reports.
* Get files containing binary records about rolled items.
* For each record file, generate Reports.
**/

class RollingReportGenerator: public Thread  {
public:

	/** RollingReportGenerator Exceptions */
	exDECLARE_EXCEPTION(exRollingReportGeneratorException, exException); // RollingReportGenerator Exception.
	exDECLARE_EXCEPTION(exRollingReportGeneratorCriticalException, exCriticalException); // RollingReportGenerator Critical Exception.

	/** Default Class constructor */
	RollingReportGenerator();
	/** Destructor */
	virtual ~RollingReportGenerator() throw();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	RollingReportGenerator(const RollingReportGenerator & );
	/** Operator = */
	RollingReportGenerator &operator=(const RollingReportGenerator &);
	
public:

	/** Setter and Getter */
	void setPaths(const std::string& reportOutBasket, const std::string& reportPrivateWorkingDir ) {
		_reportOutBasket = reportOutBasket;
		_privateWorkingDir = reportPrivateWorkingDir;
         if ( _privateWorkingDir.empty() ) _privateWorkingDir = reportOutBasket+"/.RollingArchive/";
	}
	const std::string& getPrivateWorkingDir() const { return _privateWorkingDir; }


	/** Check for new report to process */
	void checkAndProcessReports();

	/** If error occurred, throw exception */
	void ifErrorThrowException() const;
	
	static bool lockReportName( const std::string & reportName, ACS_SMARTPTR<File::Lock> & )  ;

	/** wait the beginning of next seconds if current number of ms > threshold: not to compute filename in the following second w.r.t. current time */
	static void waitNextSecond(unsigned long int threshold);

protected:
	/** Thread interface implementation */
	virtual void run();
	virtual void halt(StopController::HaltLevel l, bool joinAlso=true);

private:
	/** Condition for the standby.
	 * Note: here Thread::waitExitOrTimeout is not enought because this condition can be also notified from other Threads (e.g. for package addition request)
	 *  */
	Condition _standbyCondition;

	/** Path to write final reports */
	std::string _reportOutBasket;

	/** Working dir for Report binary file scanning */
	std::string _privateWorkingDir;

	/** Set of Binary Record Filenames to be processed */
	std::deque<std::string> _queuedBinaryRecordsFilenamesAbs;


private:
	/** Scan Report working dir to detect new report to be processed */
	void scanForBinaryRecordsFilenames();

	/** Generate report from file records */
	void generateReportsFrom( const std::string& binaryRecordsFilename ) const;

	//Used by generateReport. xslt processing on report.
	bool reportXstlProcessing( 	std::string const& filenameToConvertAbs, std::string const& workingDir,
								std::string const& defaultOutputDir,
								std::string const& defaultSchema, std::string const& defaultContentCheck,
								std::vector < std::pair <std::string, std::string> >& reportsToSend ) const;
								
	void generateReportUniqueFilename( const std::string & rptFilenameTemplate, const std::string & centreid, const DateTime & rollingTime,
										const std::string & outBasket, const std::string & tmpDir, std::string & rptFilename, std::string & rptFilenameAbs, std::string & rptFilenameTmpAbs, RollingReport::Info &) const ;
	std::string generateReportFilename( const std::string & rptFilenameTemplate, const std::string & centreid, const DateTime & nowTime, const DateTime & rollingTime ) const ;
	void waitCurrentTimeReportNewName( const std::string & rptFilenameTemplate, const std::string & centreId, const DateTime & rollingTime, const std::string & reportName ) const;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingReportGenerator)

};

_ACS_END_NAMESPACE

#endif //_RollingReportGenerator_H_

