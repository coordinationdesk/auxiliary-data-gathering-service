// PRQA S 1050 EOF
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
	Revision 2.15  2018/03/26 13:21:39  nicvac
	S2PDGS-1966: Report session: report records can be committed only at checkpoints.
	
	Revision 2.14  2018/02/06 14:52:45  nicvac
	S2PDGS-1851: Report generation during items rolling.
	
	Revision 2.13  2018/02/05 18:25:00  nicvac
	S2PDGS-1851: Report generation agent.
	
	Revision 2.12  2018/01/30 18:50:11  marpas
	trying to optimise dbConnection use
	qa warnings partially removed
	coding best practice application in progress
	
	Revision 2.11  2016/07/29 09:36:12  nicvac
	S2PDGS-1501: Fixed reading of internal rolling file in case this one is corrupted.
	
	Revision 2.10  2016/01/28 15:59:29  nicvac
	S2PDGS-1400: Test and fixing loop.
	
	Revision 2.9  2016/01/27 16:20:42  nicvac
	S2PDGS-1400: limit on items in report.
	
	Revision 2.8  2016/01/21 17:05:31  nicvac
	S2PDGS-1394: Report configurable on item id
	
	Revision 2.7  2015/03/12 17:29:29  francesco.avanzi
	now substitute case is correcty handled
	
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

#include <RollingReport.h>

#include <Filterables.h>

#include <File.h>

#include <DateTime.h>

#include <RollingReportGeneratorSingleton.h>

#include <cstring>

#include <boost/algorithm/string/replace.hpp>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingReport)

const std::string RollingReport::mission =        "Mission";
const std::string RollingReport::fileType =       "File_Type";
const std::string RollingReport::fileClass =      "File_Class";
const std::string RollingReport::fileName =       "File_Name";
const std::string RollingReport::valStart =       "Validity_Start";
const std::string RollingReport::valStop =        "Validity_Stop";
const std::string RollingReport::system =         "System";
const std::string RollingReport::creator =        "Creator";
const std::string RollingReport::creatorVersion = "Creator_Version";
const std::string RollingReport::creationDate =   "Creation_Date";
const std::string RollingReport::centre =         "Centre";
const std::string RollingReport::pdiIdTemplate =  "PdiIdTemplate";

const std::string RollingReport::rFilename = 		"Filename";
const std::string RollingReport::rFileversion = 	"Fileversion";


RollingReportAutoSession::RollingReportAutoSession(RollingReport& rollingReport):
	_rollingReport(rollingReport)
{
	_rollingReport.recordSessionOpen();
}

RollingReportAutoSession::~RollingReportAutoSession() {
	_rollingReport.recordSessionClose();
}


RollingReport::RollingReport(string const& workingDir, size_t maxItemsPerReport,  string const& includeItemRe, string const& excludeItemRe) :
	_sessionClosed( true ),
    _records(),
    _info(),
    _privateFile(),
    _nullReportMode( workingDir == "" ),
    _maxItemsPerReport( maxItemsPerReport ),
    _summaryCurrentLoopRecords(),
    _countCurrentInternal(0),
    _reIncludeItem( includeItemRe ),
    _reExcludeItem( excludeItemRe ),
    _recToDump()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631

    _privateFile = workingDir+"/rollingReportRecords.bin";

	if ( ! _nullReportMode ) {
		File::mkdirtree( workingDir );
		ACS_COND_THROW( !File::exists(workingDir), // PRQA S 3081
				exRollingReportCriticalException( "Cannot create private working dir ["+workingDir+"]" ));
	}

}

RollingReport::~RollingReport() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

bool RollingReport::isItemToBeWrittenInReport(const InvUniqueId& invUniqueId) const {

	try {
		bool itemToBeWrittenInReport =
				_reIncludeItem.getReg().empty() || _reIncludeItem.match(invUniqueId);

		itemToBeWrittenInReport = itemToBeWrittenInReport &&
				(_reExcludeItem.getReg().empty() || !_reExcludeItem.match(invUniqueId) )  ;

		if ( !itemToBeWrittenInReport ) {
			ACS_LOG_INFO("Rolling report: Report will not contain ["<<invUniqueId<<"]");
		}

		return itemToBeWrittenInReport;

	} catch( exception& ex ) {

		ACS_LOG_WARNING( "Rolling report: Report item check ignored. Exception: "<<ex.what() );

		return true;
	}

}

void RollingReport::recordSessionOpen() {
	_sessionClosed = false;
}

void RollingReport::recordSessionClose() {
	_sessionClosed = true;

	//If the binary file report reached the max items ==> Set the current private file to be processed and create a new binary file report (on next loop).
	if ( _countCurrentInternal >= _maxItemsPerReport ) {
		this->recordCommit();
	}
}

void RollingReport::record(const InvUniqueId& invUniqueId, Action action) {

	InfoRecord infoRecord;
	return record( invUniqueId, action, infoRecord );
}

void RollingReport::record(const InvUniqueId& invUniqueId, Action action, InfoRecord const& infoRecord) {

	//This would be more robust using transactions on a sqlite db (on private file)

	++ _countCurrentInternal;

	_summaryCurrentLoopRecords.insert( invUniqueId ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Called on ["<<invUniqueId<<"]; current loop num records: ["<<_summaryCurrentLoopRecords.size()<<"]");

	if ( _nullReportMode ) return;
	if ( ! this->isItemToBeWrittenInReport(invUniqueId) ) return;

	ofstream ofs;
	ofs.open (_privateFile.c_str(), ofstream::out | ofstream::app | ofstream::binary);

	ACS_COND_THROW( !ofs.good(), // PRQA S 3081
			exRollingReportCriticalException( "Error recording items into private report file ["+_privateFile+"]" ));

	char itemIdBuffer[buffer_size]; // PRQA S 4403
	memset( itemIdBuffer, 0, sizeof(itemIdBuffer) );
	memcpy( itemIdBuffer, invUniqueId.c_str(), invUniqueId.size() );
	ofs.write(itemIdBuffer, sizeof(itemIdBuffer));

	size_t infoRecordSize( infoRecord.size() );
	ofs.write( reinterpret_cast<char*>( &infoRecordSize ), sizeof(infoRecordSize));

	for ( InfoRecord::const_iterator it = infoRecord.begin(); it != infoRecord.end(); ++it ) {
		memset( itemIdBuffer, 0, sizeof(itemIdBuffer) );
		memcpy( itemIdBuffer, it->first.c_str(), it->first.size() );
		ofs.write(itemIdBuffer, sizeof(itemIdBuffer));

		memset( itemIdBuffer, 0, sizeof(itemIdBuffer) );
		memcpy( itemIdBuffer, it->second.c_str(), it->second.size() );
		ofs.write(itemIdBuffer, sizeof(itemIdBuffer));
	}

	ofs.write( reinterpret_cast<char*>( &action ), sizeof(action));

	ACS_COND_THROW( !ofs.good(), // PRQA S 3081
			exRollingReportCriticalException( "Error recording items into private report file ["+_privateFile+"]" ));

	ofs.close();

	ACS_COND_THROW( !ofs.good(), // PRQA S 3081
			exRollingReportCriticalException( "Error recording items into private report file ["+_privateFile+"]" ));

}

void RollingReport::recordCommit() {

	if ( _sessionClosed && File::exists( _privateFile ) ) {
		DateTime dtNow;
		File::rename( _privateFile, _privateFile + "_TOPROCESS_" + dtNow.asISO8601(6, "T", false) );

		_countCurrentInternal = 0;

		RollingReportGeneratorSingleton::instance()->getAgent().checkAndProcessReports();
	}
}

map<RollingReport::InvUniqueId, RollingReport::RecordContent> const& RollingReport::loadAndGetRecords(const string& binaryRecordsFilename) {

	//This would be more robust using transactions on a sqlite db (on private file)

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( _nullReportMode || !File::exists( binaryRecordsFilename ) ) return _records;

	_records.clear();

	ifstream ifs;
	ifs.open (binaryRecordsFilename.c_str(), ofstream::in | ofstream::binary);

	string errorMessage = "Error loading items from temporary private report file ["+binaryRecordsFilename+"]. Corrupted records in file?";
	ACS_COND_THROW( !ifs.good(), exRollingReportCriticalException( errorMessage )); // PRQA S 3081

	size_t countDiscardedRecords(0);
	char itemIdBuffer[buffer_size]; // PRQA S 4403

	//Record buffer
	while (	ifs.read( itemIdBuffer, sizeof(itemIdBuffer) ) ) {

		//Record elements
		string invUniqueId;
		RecordContent recordContent;


		invUniqueId = itemIdBuffer; //Up to the end-of-string char

		size_t infoRecordSize(0);
		ifs.read( reinterpret_cast<char*>( &infoRecordSize ), sizeof(infoRecordSize) );
		ACS_COND_THROW( !ifs.good(), exRollingReportCriticalException( errorMessage )); // PRQA S 3081

		for ( size_t i(0); i<infoRecordSize; ++i ) {
			string key ; 
            string value ;

			ifs.read( itemIdBuffer, sizeof(itemIdBuffer) );
			ACS_COND_THROW( !ifs.good(), exRollingReportCriticalException( errorMessage )); // PRQA S 3081
			key = itemIdBuffer;

			ifs.read( itemIdBuffer, sizeof(itemIdBuffer) );
			ACS_COND_THROW( !ifs.good(), exRollingReportCriticalException( errorMessage )); // PRQA S 3081
			value = itemIdBuffer;

			recordContent.infoRecord[key] = value;
		}

		ifs.read( reinterpret_cast<char*>( &(recordContent.action) ), sizeof(recordContent.action) );
		ACS_COND_THROW( !ifs.good(), exRollingReportCriticalException( errorMessage )); // PRQA S 3081

		//Record validation
		if ( ! invUniqueId.empty() &&
			( RollingReport::removed <= recordContent.action && recordContent.action <= RollingReport::substitute )
		) {

			_records[ invUniqueId ] = recordContent;
		} else {
			++countDiscardedRecords;
		}

	}

	if ( !ifs.eof() ) {
		ACS_LOG_WARNING("Rolling report: Error loading items from private report file ["<<binaryRecordsFilename<<"]. Report may be incomplete");
	}

	if ( countDiscardedRecords > 0 ) {
		ostringstream c; c<<countDiscardedRecords;
		ACS_LOG_WARNING("Rolling report: Some items ["+c.str()+"] from private report file ["<<binaryRecordsFilename<<"] has been discarded (corrupted records in private report file ["+binaryRecordsFilename+"]?). Report may be incomplete");
	}

	ifs.close();

	return _records;

}

void RollingReport::serialize(ostringstream& report)  {

	std::map< InvUniqueId, RecordContent >::iterator it = _records.begin();

	this->serialize( report, _records.size(), it );

}


void RollingReport::serialize(ostringstream& report, size_t howMany, std::map< InvUniqueId, RecordContent >::iterator& it  ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	report.str("");

	string const& pdiIdTemplateValue =  _info[ pdiIdTemplate ];

	report << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<Earth_Explorer_File xsi:noNamespaceSchemaLocation=\"AI_PDISetUpdateReport.xsd\" xmlns:archive=\"http://gs2.esa.int/DICO/1.0/PDGS/archive/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \n\
  <Earth_Explorer_Header>\n\
	<Fixed_Header>\n\
	  <File_Name>"<<_info[fileName]<<"</File_Name>\n\
	  <File_Description/>\n\
	  <Notes/>\n\
	  <Mission>"<<_info[mission]<<"</Mission>\n\
	  <File_Class>"<<_info[fileClass]<<"</File_Class>\n\
	  <File_Type>"<<_info[fileType]<<"</File_Type>\n\
	  <Validity_Period>\n\
		<Validity_Start>"<<_info[valStart]<<"</Validity_Start>\n\
		<Validity_Stop>"<<_info[valStop]<<"</Validity_Stop>\n\
	  </Validity_Period>\n\
	  <File_Version>0001</File_Version>\n\
	  <Source>\n\
		<System>"<<_info[system]<<"</System>\n\
		<Creator>"<<_info[creator]<<"</Creator>\n\
		<Creator_Version>"<<_info[creatorVersion]<<"</Creator_Version>\n\
		<Creation_Date>"<<_info[creationDate]<<"</Creation_Date>\n\
	  </Source>\n\
	</Fixed_Header>\n\
	<Variable_Header/>\n\
  </Earth_Explorer_Header>\n\
  <Data_Block xsi:type=\"PDISetUpdateReport\">\n\
	<Centre>"<<_info[centre]<<"</Centre>\n\
	<List_of_ItemsMetadataUpdate>\n";

	for ( size_t count(0); it!=_records.end() && count<howMany; ++count, ++it ) {

		string const& uniqueId = it->first;
		RecordContent& recordContent = it->second;

		string pdiIdContent( uniqueId );
		if ( ! pdiIdTemplateValue.empty() ) {
			pdiIdContent = boost::replace_all_copy( pdiIdTemplateValue, "${uniqueid}", uniqueId );
			string const& filename = recordContent.infoRecord[rFilename];
			boost::replace_all( pdiIdContent, "${filename}", filename );
			string const& filevesion = recordContent.infoRecord[rFileversion];
			boost::replace_all( pdiIdContent, "${fileversion}", filevesion );
		}

		if (recordContent.action == offline) {
			report<<
"		<ItemMetadataUpdate>\n\
			<archive:Pdi-id>"<< pdiIdContent <<"</archive:Pdi-id>\n\
			<Operation>Update</Operation>\n\
			<archive:Latency>Off-line</archive:Latency>\n\
		</ItemMetadataUpdate>\n";
		} else if (recordContent.action == online) {
			report<<
"		<ItemMetadataUpdate>\n\
			<archive:Pdi-id>"<< pdiIdContent <<"</archive:Pdi-id>\n\
			<Operation>Update</Operation>\n\
			<archive:Latency>On-line</archive:Latency>\n\
		</ItemMetadataUpdate>\n";
		} else if (recordContent.action == substitute) {
			report<<
"		<ItemMetadataUpdate>\n\
			<archive:Pdi-id>"<< pdiIdContent <<"</archive:Pdi-id>\n\
			<Operation>Substitute</Operation>\n\
		</ItemMetadataUpdate>\n";
		} else {
			report<<
"		<ItemMetadataUpdate>\n\
			<archive:Pdi-id>"<< pdiIdContent <<"</archive:Pdi-id>\n\
			<Operation>Delete</Operation>\n\
		</ItemMetadataUpdate>\n";
		}

	}

	report <<
"	</List_of_ItemsMetadataUpdate>\n\
  </Data_Block>\n\
</Earth_Explorer_File>\n\
";

}

void RollingReport::dumpInit() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	_recToDump = _records.begin();
}

bool RollingReport::dump(const std::string& filename, size_t howMany)  {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( _nullReportMode ) return false; //No records to dump

	if ( _recToDump == _records.end() ) return false; //No records dumped

	std::ofstream ofs (filename.c_str(), std::ofstream::out);

	ACS_COND_THROW( ! ofs.good(), // PRQA S 3081
		  exIOException("Error writing Rolling Report on file ["+filename+"]"));

	ostringstream report;
	this->serialize(report, howMany, _recToDump);
	ofs << report.str();

	ACS_COND_THROW( ! ofs.good(), // PRQA S 3081
		  exIOException("Error writing Rolling Report on file ["+filename+"]"));

	ofs.close();

	//true: some records dumped
	return true;

}

bool RollingReport::dumpCommit( const string& binaryRecordsFilename ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Reports created. Can clean loaded records and remove related binary file

	_records.clear();

	File::unlink( binaryRecordsFilename );

	return true;
}


void RollingReport::recordUnsafe(const InvUniqueId& invUniqueId, Action action) {

	InfoRecord infoRecord;
	this->recordUnsafe( invUniqueId, action, infoRecord );
}

void RollingReport::recordUnsafe(const InvUniqueId& invUniqueId, Action action, InfoRecord const& infoRecord ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( this->isItemToBeWrittenInReport(invUniqueId) ) {

		RecordContent recordContent;
		recordContent.action = action;
		recordContent.infoRecord = infoRecord;

		_records[ invUniqueId ] = recordContent;
	}

}


_ACS_END_NAMESPACE

