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
	Revision 2.4  2018/05/15 14:07:31  nicvac
	S2PDGS-1981: report private working dir conf.
	
	Revision 2.3  2018/03/20 16:02:02  nicvac
	S2PDGS-1960: Internal report renamed when processed. To avoid loop on corrupted internal report.
	
	Revision 2.2  2018/02/06 14:52:45  nicvac
	S2PDGS-1851: Report generation during items rolling.
	
	Revision 2.1  2018/02/05 18:25:00  nicvac
	S2PDGS-1851: Report generation agent.
	
        
*/ 

#include <RollingReportGenerator.h>


#include <ConfigurationSingleton.h>

#include <Filterables.h>

#include <StringUtils.h>
#include <Dir.h>
#include <RegExp.h>

#include <XmlDoc.h>
#include <XMLTransformer.h>
#include <XMLValidator.h>

#include <Timer.h>


#include <boost/algorithm/string.hpp> // PRQA S 1013

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingReportGenerator)



RollingReportGenerator::RollingReportGenerator():
_standbyCondition(),
_reportOutBasket(),
_privateWorkingDir(),
_queuedBinaryRecordsFilenamesAbs()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RollingReportGenerator::~RollingReportGenerator() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void RollingReportGenerator::halt(StopController::HaltLevel l, bool joinAlso) {

	ACS_LOG_PRIVINFO("Rolling report: Received stop request.");
	ACS_LOG_PRIVINFO("Rolling report: Wake up if waiting...");

	{ //Notify the stop event. Wake up if sleeping.
    	ThreadSafe::Lock conditionLock(_standbyCondition);
        // Do not join with the mutex locked just set the halt level at this point
		Thread::halt(l,false);
		_standbyCondition.notify_all();
	}
    // Here the lock is free, can join if needed
    Thread::halt (l,joinAlso);
}


void RollingArchive_reportValidation(string const& absXmlFilename, string const& absSchemaFilename) {

	ACS_LOG_INFO( "Rolling report: Validating ["<<absXmlFilename<<"]" );
	acs::XMLValidator validator( absXmlFilename );
	validator.setSchemaName(absSchemaFilename);
    bool valid = validator.validate(false);

	ACS_COND_THROW( !valid, // PRQA S 3081
			exCriticalException("Error validating ["+absXmlFilename+"] Vs schema ["+validator.getSchemaName(true)+"]") );

	ACS_LOG_INFO( "Rolling report: Validated ["<<absXmlFilename<<"] Vs schema ["<<validator.getSchemaName(true)<<"]");


}


void RollingArchive_reportContentCheck(string const& absFilename, string const& boolXpath) {

	ACS_LOG_PRIVINFO( "Rolling report: Checking content of ["<<absFilename<<"] with ["<<boolXpath<<"]" );

	XmlDocSptr xmlDocSptr( new XmlDoc );
	xmlDocSptr->setParameters(XmlDoc::parDefault).initWithFile( absFilename );

	bool validContent = false ;
	xmlDocSptr->getValue( boolXpath, validContent);

	if ( ! validContent ) {
		ACS_LOG_PRIVINFO( "Rolling report: Content check not passed. Removing ["<<absFilename<<"]" );
		File::unlink( absFilename );
	} else {
		ACS_LOG_PRIVINFO( "Rolling report: Content check passed." );
	}
}


bool RollingReportGenerator::reportXstlProcessing( 	string const& filenameToConvertAbs, string const& workingDir,
											string const& defaultOutputDir,
											string const& defaultSchema, string const& defaultContentCheck,
											vector < pair<string, string> >& reportsToSend ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool isOk(true);
	try {

		const rsResourceSet& conf = ConfigurationSingleton::instance()->get();

		//Here report is written in temporary path.
		//If format conversions is required, Apply the set of configured xslt
		bool xsltEnabled(false);
		conf.getValue("RollingArchive.Report.XsltFormatConverter.enabled.value", xsltEnabled);
		if ( xsltEnabled && File::exists( filenameToConvertAbs ) ) {

			//For each conversion to do
			bool isSingle(false);
			size_t numConv = conf.getArraySize("RollingArchive.Report.XsltFormatConverter.Conversions.Conversion", &isSingle);
			for ( size_t i(0); i<numConv; ++i ) {
				ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

				ostringstream key; key<<"RollingArchive.Report.XsltFormatConverter.Conversions.Conversion";
				if (!isSingle) { key <<"["<<i<<"]"; }

				//Params for conversions
				string xslFileAbs;
				{
					string confXslFileAbs;
					conf.getValue( key.str()+".xslFile", confXslFileAbs );
					xslFileAbs = workingDir +"/"+File::getFileName(confXslFileAbs);
					ACS_LOG_PRIVINFO("Rolling report: copying ["<<confXslFileAbs<<"] to ["<<xslFileAbs<<"]");
					File::unlink(xslFileAbs);
					File::copy(confXslFileAbs, xslFileAbs);
				}

				//Compute the Converted report filename
				string convFilenameAbs ;
                string convFilenameTmpAbs ;
				{
					string nameFrom ;
                    string nameTo ;
					conf.getValue( key.str()+".FileRename.from.value", nameFrom );
					conf.getValue( key.str()+".FileRename.to.value", nameTo );

					string convOutBasket;
					conf.getValue( key.str()+".OutBasket", convOutBasket );
					if (convOutBasket.empty()) { convOutBasket = defaultOutputDir; }

					//Note: convFilename can be the same as rptFilename
					string convFilename = boost::replace_all_copy( File::getFileName(filenameToConvertAbs), nameFrom, nameTo);
					convFilenameAbs = convOutBasket+"/"+convFilename;
					convFilenameTmpAbs = workingDir+"/_CONV_"+convFilename;
				}

				ACS_LOG_PRIVINFO("Rolling report: Generating Rolling report ["<<convFilenameAbs<<"] from ["<<filenameToConvertAbs<<"]. Using ["<<xslFileAbs<<"]");

				//Conversion
				{
					File::unlink( convFilenameTmpAbs );

					{
						//Create the parameter file
						string parameterFile(File::getDirPath(xslFileAbs)+"/rollReportParams.xml");
						std::ofstream ofs (parameterFile.c_str(), std::ofstream::out);

						ofs << "<RollReportParams><File_Name>"<< File::getFileName(convFilenameAbs) <<"</File_Name></RollReportParams>";

						ACS_COND_THROW( ! ofs.good(),
								exCriticalException("Xslt conversion: Error writing Rolling report parameter file ["+parameterFile+"]"));

						ofs.close();
					}

					XMLTransformer tr;
					tr.xslFilePath(xslFileAbs);
					tr.xmlFilePath(filenameToConvertAbs);
					tr.outputFilePath( convFilenameTmpAbs );
					tr.writeOutputFile();
				}

				//Validation
				{
					bool doConvValidation(false);
					string convSchema;
					conf.getValue( key.str()+".Validation.enabled.value", doConvValidation);
					conf.getValue( key.str()+".Validation", convSchema);
					if (convSchema.empty()) { convSchema = defaultSchema; }

					if ( doConvValidation && !convSchema.empty() ) {
						RollingArchive_reportValidation( convFilenameTmpAbs, convSchema );
					} else {
						ACS_LOG_PRIVINFO("Rolling report validation skipped on ["<<convFilenameTmpAbs<<"] by configuration.");
					}
				}

				//Content check
				{
					bool doConvContentCheck(false);
					string convContentCheck;
					conf.getValue(key.str()+".ContentCheck.enabled.value", doConvContentCheck);
					conf.getValue(key.str()+".ContentCheck", convContentCheck);
					if (convContentCheck.empty()) { convContentCheck = defaultContentCheck; }

					if ( doConvContentCheck && !convContentCheck.empty() ) {
						RollingArchive_reportContentCheck( convFilenameTmpAbs, convContentCheck );
					} else {
						ACS_LOG_PRIVINFO("Rolling report content check skipped on ["<<convFilenameTmpAbs<<"] by configuration.");
					}
				}

				//Feed the report basket
				if ( File::exists( convFilenameTmpAbs ) ) {
					reportsToSend.push_back( make_pair( convFilenameTmpAbs, convFilenameAbs ) );
				}
			}

			//Remove the original report (if requested by configuration)
			bool removeOriginal(false);
			conf.getValue("RollingArchive.Report.XsltFormatConverter.removeOriginal.value", removeOriginal);
			if ( removeOriginal ) {
				File::unlink( filenameToConvertAbs );
			}
		}

	} catch (exception& ex) {
		isOk = false;

		ACS_LOG_WARNING("Rolling report: Error occurred converting the Rolling report. The Report to be converted has been left in ["<<filenameToConvertAbs<<"] for investigation purpose. Error was: "<<ex);

	}

	return isOk;
}


string RollingReportGenerator::generateReportFilename( const string & rptFilenameTemplate, const string & centreid, const DateTime & nowTime, const DateTime & rollingTime ) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	string rptFilename = rptFilenameTemplate;
	boost::replace_all( rptFilename, "${centreId}", centreid );
	boost::replace_all( rptFilename, "${now-compact}", nowTime.YYYYMMDDThhmmss() );
	boost::replace_all( rptFilename, "${rollingTime-compact}", rollingTime.YYYYMMDDThhmmss() );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "report filename =\"" << rptFilename << '\"');
	return rptFilename;
}

void RollingReportGenerator::waitCurrentTimeReportNewName( const string & rptFilenameTemplate, const string & centreId, const DateTime & rollingTime, const string & reportName ) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	bool namesAreEqual = false;
	do {
		DateTime currentTime;
		string currentReportName = generateReportFilename( rptFilenameTemplate, centreId, currentTime, rollingTime ) ;
		namesAreEqual = StringUtils::equals( reportName, currentReportName ) ;
		if( not namesAreEqual ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << reportName << "\" reference name and \"" << currentReportName << "\" current name differ");
			break;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << reportName << "\" reference name and current name are equal");
		Timer::delay(200);
	} while( namesAreEqual ) ;

}


bool RollingReportGenerator::lockReportName( const string & reportName, ACS_SMARTPTR<File::Lock> & lockPtr )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	string lockFilepath = StringUtils::pathJoin( "/tmp" , reportName);
	lockPtr = ACS_SMARTPTR<File::Lock>(new  File::Lock(lockFilepath)  );

	bool locked = true;
	if( not lockPtr->lock(0) ) {
	// Cannot lock lockFilepath 
		ACS_LOG_PRIVINFO('\"' << reportName << "\" already generated by concurrent process");
		locked = false;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << lockPtr->filename() << "\" file " << ( locked ? "" : "NOT " ) << "locked" );
	return locked;
}

void RollingReportGenerator::waitNextSecond( unsigned long int threshold )
{
	DateTime::UTCD utcd;
	DateTime now;
	now.toUTCD(utcd) ;
	unsigned long millisec = utcd.millisec();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "millisec: " << millisec << "; threshold: " << threshold );
	if( millisec > threshold ) {
		Timer::delay( (1000 - millisec) + 1 );
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Ready" );
}



void RollingReportGenerator::generateReportUniqueFilename( const string & rptFilenameTemplate, const string & centreid, const DateTime & rollingTime,
															 const string & outBasket, const string & tmpDir, string & rptFilename, string & rptFilenameAbs, string & rptFilenameTmpAbs, RollingReport::Info & info) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	bool fileLocked = false;
	do {
		waitNextSecond(800UL);
		DateTime dtNow ;
		ACS_CLASS_WRITE_DEBUG( ACS_MID_VERB, "Rolling time: " << rollingTime.YYYYMMDDThhmmss() << " now time: " << dtNow.YYYYMMDDThhmmss()) ;
		rptFilename = generateReportFilename( rptFilenameTemplate, centreid, dtNow, rollingTime ) ; 
		
		ACS_SMARTPTR<File::Lock> lock ;
		fileLocked = lockReportName( rptFilename, lock );
		if( fileLocked ) {

			ACS_LOG_PRIVINFO('\"' << rptFilename << "\" report name booked");
			lock->autoRemoveLockFile(true);
			

			info[ RollingReport::fileName ] = File::removeExt( rptFilename );
			
			{
				ostringstream ossNow; 
				dtNow.asISO8601(ossNow, 0);
				info[ RollingReport::creationDate ] = "UTC="+ossNow.str();
			}
			
			// wait the name changes...
			waitCurrentTimeReportNewName( rptFilenameTemplate, centreid, rollingTime, rptFilename ) ;
			
			ACS_LOG_PRIVINFO("Released "<<lock->filename());
		}
		else {
			waitCurrentTimeReportNewName( rptFilenameTemplate, centreid, rollingTime, rptFilename ) ;
		}
	} while ( not fileLocked );
	
	rptFilenameAbs = StringUtils::pathJoin( outBasket, rptFilename );
	rptFilenameTmpAbs = StringUtils::pathJoin( tmpDir, rptFilename );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Report filename=\"" << rptFilename << "\" absolute filepath=\"" << rptFilenameAbs << "\" temporary filepath=\"" << rptFilenameTmpAbs << '\"'); 

}


void RollingReportGenerator::generateReportsFrom( const string& binaryRecordsFilename ) const {


	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Processing Report ["<<binaryRecordsFilename<<"]" );

	bool reportGenerationRequested(false);
	bool reportGenerationCommitted(false);

	try {

		RollingReport rollingReport( _privateWorkingDir );

		size_t numRecords = rollingReport.loadAndGetRecords( binaryRecordsFilename ).size();
		if ( numRecords == 0 ) return;

		const rsResourceSet& conf = ConfigurationSingleton::instance()->get();

		string outBasket = _reportOutBasket;

		string rptFilenameTemplate;
		conf.getValue("RollingArchive.Report.FileName", rptFilenameTemplate);

		//Filename computation
		string centreid;
		conf.getValue("Station.CentreID", centreid) ;

		const DateTime dtRollingTime;

		if ( !outBasket.empty() ) {

			ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

			reportGenerationRequested = true;

			string tmpDir = _privateWorkingDir;
			File::mkdirtree(tmpDir);

			RollingReport::Info repInfo;
			{
				string val;
				ostringstream osTime;

				conf.getValue("RollingArchive.Report.Params.Mission", val);		repInfo [ RollingReport::mission  ] = val;
				conf.getValue("RollingArchive.Report.Params.File_Type", val);	repInfo [ RollingReport::fileType ] = val;
				conf.getValue("RollingArchive.Report.Params.File_Class", val);	repInfo [ RollingReport::fileClass] = val;

				osTime.str(""); dtRollingTime.asISO8601(osTime, 0);
				repInfo [ RollingReport::valStart       ] = "UTC="+osTime.str();
				repInfo [ RollingReport::valStop        ] = "UTC="+osTime.str();
				repInfo [ RollingReport::system         ] = centreid;

				conf.getValue("RollingArchive.Report.Params.Creator", val);		repInfo [ RollingReport::creator ] = val;
				conf.getValue("RollingArchive.Report.Params.CreatorVer", val);	repInfo [ RollingReport::creatorVersion ] = val;

				repInfo [ RollingReport::centre         ] = centreid;

				string confPdiIdTemplate;
				conf.getValue("RollingArchive.Report.Params.PdiIdTemplate", confPdiIdTemplate, false);
				repInfo [ RollingReport::pdiIdTemplate ] = confPdiIdTemplate;

				rollingReport.setInfo( repInfo );
			}

			//Dump reports (max n items per report)
			size_t maxItemsPerReport(35000); //S2PDGS-1399
			conf.getValue("RollingArchive.Report.MaxItemsPerReport", maxItemsPerReport, false);

			//Report naming
			string rptFilename;
			string rptFilenameAbs ;
			string rptFilenameTmpAbs;
			generateReportUniqueFilename( rptFilenameTemplate, centreid, dtRollingTime, outBasket, tmpDir, rptFilename, rptFilenameAbs, rptFilenameTmpAbs, rollingReport.getInfo()  ) ;
			
			//Collection of reports to be sent
			vector < pair<string, string> > reportsToSend;

			size_t reportCount(0);
			size_t const totRecords ( rollingReport.getNumberOfRecords() );
			rollingReport.dumpInit();
			while ( rollingReport.dump( rptFilenameTmpAbs, maxItemsPerReport ) ) {
				ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

				++reportCount;
				ACS_LOG_INFO("Generating Rolling report #"<<reportCount<<"/"<<static_cast<int>( ceil( static_cast<float>(totRecords)/static_cast<float>(maxItemsPerReport) ) )<<". Max items per report: "<<maxItemsPerReport);

				//Validation
				string confSchema;
				{
					bool doValidation(false);
					conf.getValue("RollingArchive.Report.Validation.enabled.value", doValidation);
					conf.getValue("RollingArchive.Report.Validation.Schema", confSchema);

					if ( doValidation && !confSchema.empty() ) {
						RollingArchive_reportValidation( rptFilenameTmpAbs, confSchema );
					} else {
						ACS_LOG_PRIVINFO("Rolling report validation skipped by configuration.");
					}
				}

				//Content check
				string confContentCheck;
				{
					bool doContentCheck(false);
					conf.getValue("RollingArchive.Report.ContentCheck.enabled.value", doContentCheck);
					conf.getValue("RollingArchive.Report.ContentCheck", confContentCheck);

					if ( doContentCheck && !confContentCheck.empty() ) {
						RollingArchive_reportContentCheck( rptFilenameTmpAbs, confContentCheck );
					} else {
						ACS_LOG_PRIVINFO("Rolling report content check skipped by configuration.");
					}
				}

				//Process configured Xslt on current generated report
				bool conversionOk = this->reportXstlProcessing(
						rptFilenameTmpAbs, tmpDir, outBasket,
						confSchema, confContentCheck,
						reportsToSend);

				ACS_COND_THROW( !conversionOk, // PRQA S 3081
						exCriticalException("Error on xslt processing on ["+rptFilenameTmpAbs+"]") );

				//Feed the report basket. Current original report can be removed by conversion
				if ( conversionOk && File::exists( rptFilenameTmpAbs ) ) {
					reportsToSend.push_back( make_pair( rptFilenameTmpAbs , rptFilenameAbs ) );
				}

				//Updates for the next cycle
				generateReportUniqueFilename( rptFilenameTemplate, centreid, dtRollingTime, outBasket, tmpDir, rptFilename, rptFilenameAbs, rptFilenameTmpAbs, rollingReport.getInfo() ) ;
			} //while ( rollingReport.dump( rptFilenameTmpAbs, maxItemsPerReport ) )

			//Feed the collected output baskets
			for ( vector< pair<string, string> >::iterator it( reportsToSend.begin()); it!=reportsToSend.end(); ++it ) {

				pair<string, string>& old_new = *it;
				File::rename( old_new.first, old_new.second );

				ACS_LOG_INFO("Dumped Rolling report ["<<old_new.second<<"]");
			}

			//Reports generated. Delete report internal file. If error occurred, report internal file will be used
			// to restore report at next call of this method (cf. rollingReport.loadAndGetRecords())
			reportGenerationCommitted = rollingReport.dumpCommit( binaryRecordsFilename );
		}

	} catch(exception& ex) {
		ACS_LOG_WARNING( "Cannot Write Rolling Report! Error was: "<<ex );

		if (reportGenerationRequested && !reportGenerationCommitted ) {
			ACS_LOG_WARNING( "Report generation was requested, but error occurred. Internal file ["<<binaryRecordsFilename<<"] contains records of rolled items but not included in report (because failure). This internal file has been left on disk for analysis purpose." );
		}
	}



}

void RollingReportGenerator::scanForBinaryRecordsFilenames() {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Dir reportDir( _privateWorkingDir );

	RegExp reToProc(".*_TOPROCESS_.*");

	for ( size_t i(0); i<reportDir.size(); ++i) {

		if ( reToProc.match( reportDir[i] ) ) {
			_queuedBinaryRecordsFilenamesAbs.push_back( _privateWorkingDir + "/" + reportDir[i] );
		}
	}
}


void RollingReportGenerator::checkAndProcessReports() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	{   //Wake up if sleeping.
    	ThreadSafe::Lock conditionLock(_standbyCondition);
        // Do not join with the mutex locked just set the halt level at this point
		_standbyCondition.notify_all();
	}

}

void RollingReportGenerator::ifErrorThrowException() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	exception* te(NULL);
	this->runException(te);
	if (te) {
		exCriticalException ex(*te, "Received exception from RollingReportGenerator");
		delete te;
		ACS_THROW(ex);
	}

}


void RollingReportGenerator::run() {

	ACS_LOG_PRIVINFO("started.");

	if ( _privateWorkingDir.empty() ) {
		ACS_LOG_PRIVINFO( "Report generation disabled." );
		return;
	}

	ACS_LOG_PRIVINFO( "Report generation agent started." );
	File::mkdirtree( _reportOutBasket );
	File::mkdirtree( _privateWorkingDir );

	//Main loop.
	while( ! this->requestedCancel() ) {

		/*/Pattern is:
		lock the condition
		while (!(boolean_predicate)) {
			wait on condition
		}
		do protected work
		(optionally, signal or broadcast the condition again or change a predicate value)
		unlock the condition
		//*/

		{
			//--lock the condition
	        ThreadSafe::Lock lock ( _standbyCondition) ;

			//Update my status -- change the boolean_predicate value
			this->scanForBinaryRecordsFilenames();

			//You can rest if there's nothing to do -- while (!(boolean_predicate)) {
			while ( _queuedBinaryRecordsFilenamesAbs.size()==0 && !this->requestedCancel() ) {

				try {
					//Sleep and wake me up on _standbyCondition notification.
					//release mutex and block on condition

					_standbyCondition.wait(); //-- wait on condition

					//The mutex is locked by me here (as per wait implementation)

					ACS_LOG_PRIVINFO("Rolling report: Woken up.");

				} catch (exception& ex) {
					ACS_LOG_WARNING("RollingReportGenerator::run: Got exception in the wakeup condition. Try on next loop. Exception:" << ex.what() );
				}

				//The mutex is locked by me here (as per pthread_cond_wait ans timedwait)
				//Notification received. Refresh the status
				this->scanForBinaryRecordsFilenames();
			}

			//-- do protected work. The mutex is locked by me here
			//Something to do at last...

			//No protected work needed here.

			//-- unlock the condition automatically
		}

		//Consumer (Report generation) does not need mutual exclusion with the Producer (binary records producer).
		//So here the both producer and consumer (this) are running.
		while ( _queuedBinaryRecordsFilenamesAbs.size() > 0 && ! this->requestedCancel() ) {

			//Consume the queued file conaining records

			const string binaryRecordsFilename = _queuedBinaryRecordsFilenamesAbs.front();
			_queuedBinaryRecordsFilenamesAbs.pop_front();

			const string binaryRecordsFilenameProc = boost::replace_all_copy( binaryRecordsFilename, "_TOPROCESS_", "_PROCESSING_" );

			File::rename( binaryRecordsFilename, binaryRecordsFilenameProc );

			ACS_LOG_INFO("Rolling report: generating reports from file records ["<<binaryRecordsFilenameProc<<"].");

			this->generateReportsFrom( binaryRecordsFilenameProc );

		}

	}

	ACS_LOG_INFO("Rolling report: Agent quit.");
}

_ACS_END_NAMESPACE

