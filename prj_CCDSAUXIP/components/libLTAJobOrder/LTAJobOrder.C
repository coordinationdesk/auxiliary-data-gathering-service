// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA JobOrder Library $

*/


#include <libltajoV.h>
#include <LTAJobOrder.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#include <exStream.h>

#include <sstream>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>
#include <StringUtils.h>
#include <DateTime.h>


using namespace std;
using namespace acs;


// WARNING
// Reading a job from xml file causes:
//
// - all the inputs are stored with origin flag set to DB -> local downloaded files
//
// - the outputs are stored with destination flag set to
//    DB if the File_Name read is a directory
//    PROC if the File_Name read is a complete pathname


// IPF_CONFIG tags
namespace {
    const char* _ipfConfTag = "Ipf_Conf";
    const char* _processorNameTag = "Processor_Name";
    const char* _versionTag = "Version";
    const char* _orderTypeTag = "Order_Type";
    const char* _orderIdTag = "Order_ID";
    const char* _loggingLevelTag = "Logging_Level";
    const char* _testTag = "Test";
    const char* _troubleshootingTag = "Troubleshooting";
    const char* _processingStationTag = "Processing_Station";
    const char* _sensingTimeTag = "Sensing_Time";
    const char* _startTag = "Start";
    const char* _stopTag = "Stop";

    // IPF_PROCS tags
    const char* _ipfProcTag = "Ipf_Proc";
    const char* _taskNameTag = "Task_Name";
    const char* _taskVersionTag = "Task_Version";
    const char* _breakpointTag = "Breakpoint";
    const char* _enableTag = "Enable";
    const char* _brkFileTag = "Brk_File";
    const char* _fileTypeTag = "File_Type";
    const char* _inputTag = "Input";
    const char* _timeIntervalTag = "Time_Interval";
    const char* _outputTag = "Output";
    const char *_fileNameTypeTag     = "File_Name_Type";

    // PROCESSOR_CONF tags
    const char *_processorConfTag = "Processor_Conf";

    // PARAMETER_FILE tags
    const char *_parametersFileTag = "Parameter_File";

    const char *_fileNameTag     = "File_Name";
    const char *_fileNameListTag = "List_of_File_Names";
    const char *_fileNameListCountKey    = "List_of_File_Names.count.value";
    const char *_timeIntervalListTag = "List_of_Time_Intervals";
    const char *_timeIntervalListCountKey    = "List_of_Time_Intervals.count.value";
    const char *_brkFileListTag  = "List_of_Brk_Files";
    const char *_brkFileListCountKey = "List_of_Brk_Files.count.value";
    const char *_inputListTag    = "List_of_Inputs";
    const char *_inputListCountKey ="List_of_Inputs.count.value";
    const char *_outputListTag   = "List_of_Outputs";
    const char *_outputListCountKey = "List_of_Outputs.count.value";
    const char *_procListTag     = "List_of_Ipf_Procs";
    const char *_procListCountKey = "List_of_Ipf_Procs.count.value";
    const char *_rootTag = "Ipf_Job_Order";
    const char *_configFilesTag = "Config_Files";

    const char *_defaultExtension = ".DBL";

	libltajoV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAJobOrder)

LTAJobOrder::LTAJobOrder()
{

}

LTAJobOrder::~LTAJobOrder() ACS_NOEXCEPT {}

string
LTAJobOrder::DateTime2String(DateTime d) const
{
  DateTime::UTCD utcd(d);
  ostringstream os;
  os << setfill('0') <<
	setw(4) << utcd.year	<<
	setw(2) << utcd.month	<<
	setw(2) << utcd.day	<<
	'_' <<
	setw(2) << utcd.hour	<<
	setw(2) << utcd.min	<<
	setw(2) << utcd.sec     <<
	setw(3) << utcd.millisec()    <<
	"000"; //usecs

  return os.str();
}

DateTime
LTAJobOrder::utcs2datetime (const string& s) const
{
    // JobOrder date format is YYYYMMDD_hhmmssmmm000" 
    // changing the '_' into a 'T'
    string st = s.substr(0,8) + 'T' + s.substr(9,6);
	string smsec = s.substr(15,3);
	long msec = atol(smsec.c_str());
    return DateTime(st)+=msec/(86400.0*1000) ;
}

/**
** HELPER CLASSES IMPLEMENTATION - BEGIN
**/

unsigned int LTAJobOrder::xml_indent::_indentCount = 0;

LTAJobOrder::xml_section_tag::xml_section_tag(ofstream& o, string s, string a) : 
	_o(o),
	_s(s)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }

	if (a != "") {
	    o << "<" << s << " " << a << ">" << '\n';
    }
	else {
		o << "<" << s << ">" << '\n';
    }

	_indentCount++;

}

LTAJobOrder::xml_section_tag::~xml_section_tag()
{
	if (_indentCount>0) {
		_indentCount--;
    }

	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		_o << "\t";
    }

	_o << "</" << _s << ">" << '\n';
}

LTAJobOrder::xml_element_tag::xml_element_tag(ofstream& o, string s, string v)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }
	o << "<" << s << ">" << v << "</" << s << ">" << '\n';
}

LTAJobOrder::xml_element_tag::xml_element_tag(ofstream& o, string s, unsigned int u)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }
	o << "<" << s << ">" << u << "</" << s << ">" << '\n';
}

LTAJobOrder::breakpoint_dumper::breakpoint_dumper(const LTAJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	 _o(oo)
{}

void 
LTAJobOrder::breakpoint_dumper::operator() (const BreakpointDescriptor& b)
{
	xml_section_tag t(_o, "Brk_File");
	xml_element_tag (_o,"File_Type", b.getFileType());

	FileNameType fnt = b.getFileNameType();
	if ( fnt == FileNameTypeUNKNOWN )
	{
		ACS_LOG_ERROR("alter_dumper: ERROR: unknown FileNameType received for alternative " << b.getFileType())
	}
	else
	{
		xml_element_tag (_o, _fileNameTypeTag, FileNameType2String (fnt) );
	}

	xml_element_tag (_o,_fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(b.getFileType(), _defaultExtension));
}


LTAJobOrder::output_dumper::output_dumper(const LTAJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo)
{}

void 
LTAJobOrder::output_dumper::operator() (const OutputDescriptor& d)
{
	xml_section_tag t(_o, "Output");
	xml_element_tag (_o,"File_Type", d.getFileType());

	FileNameType fnt = d.getFileNameType();
	if ( fnt == FileNameTypeUNKNOWN )
	{
		ACS_LOG_ERROR("output_dumper: ERROR: unknown FileNameType received for output " << d.getFileType()) ;
	}
	else
	{
		xml_element_tag (_o, _fileNameTypeTag, FileNameType2String (fnt) );
	}
	
	switch (d.getDestination())
	{
	case OutputDestDB:
        {
		    switch (d.getFileNameType())
		    {
		    case FileNameTypeREGEXP:
                {
			        // in case of regexp outputs
			        // the complete path plus the regular expression 
			        // is set by the ThinLayer
			        xml_element_tag (_o, _fileNameTag, d.getPathName());
                }
                break;
		    default:
                {
			        // if not regexp always use the DIRECTORY specification
			        xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/");
                }
                break;
		    }
        }
        break;
	
	case OutputDestPROC:
        {
/*
		    switch (d.getFileNameType())
		    {
		    // Improvement needed for the PROC naming convention concerning
		    // - FileNameTypePHYSICAL
		    // - FileNameTypeLOGICAL
		    // - FileNameTypeSTEM
		    default:
			    // by default use the INTERIM specification 
			    // that is physical without extension
			    xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(d.getFileType()));
		        break;
		    }
*/
// MP the above switch is useless and increment the complexity 
            xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(d.getFileType()));
        }
        break;
	
	case OutputDestDBPROC:
        {
		    switch (d.getFileNameType())
		    {
		    case FileNameTypeREGEXP:
                {
			        xml_element_tag (_o, _fileNameTag, d.getPathName());
                }
                break;
		    default:
                {
			        // if not regexp always use the DIRECTORY specification
			        xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/");
                }
                break;
		    }
	        break;
	    }
	default:
        {
		    ACS_LOG_ERROR("output_dumper: ERROR: unknown origin received for output " << d.getFileType()) ;
	    }
        break;
	}
}


LTAJobOrder::file_dumper::file_dumper(const LTAJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo)
{}

void 
LTAJobOrder::file_dumper::operator() (const string& s)
{ 
	xml_element_tag (_o, _fileNameTag, s); 
}


LTAJobOrder::altern_dumper::altern_dumper(const LTAJobOrder* jo, ofstream& oo) :
	_jo(jo),
	_o(oo)
{}

void 
LTAJobOrder::altern_dumper::operator() (const InputAlternative& i)
{
	xml_section_tag t(_o, "Input");
	xml_element_tag (_o,"File_Type", i.getFileType());
	FileNameType fnt = i.getFileNameType();
	if ( fnt == FileNameTypeUNKNOWN )
	{
		ACS_LOG_ERROR("alter_dumper: ERROR: unknown FileNameType received for alternative " << i.getFileType()) ;
	}
	else
	{
		xml_element_tag (_o, _fileNameTypeTag, FileNameType2String (fnt) );
	}

	switch (i.getOrigin())
	{
	case InputOriginDB:
	    {
		    vector<string> files;
		    i.getPathNames(files);
		    if (files.size()) // check if not empty
		    {
			    {
				    ostringstream os;
				    os << " count=\"" << files.size() << "\"";
				    xml_section_tag t(_o, _fileNameListTag, os.str());

				    for_each(files.begin(), files.end(), file_dumper(_jo, _o));
			    }
			    {
				    // dump intervals
				    vector< pair <int, TimeRange> > intervals;
				    i.getIntervals(intervals);

				    ostringstream os;
				    os << " count=\"" << intervals.size() << "\"";
				    xml_section_tag t(_o, _timeIntervalListTag, os.str());

				    for (size_t j=0; j<intervals.size(); ++j)
				    {
					    xml_section_tag t(_o, "Time_Interval");

					    xml_element_tag (_o,"Start", _jo->DateTime2String(intervals[j].second.start()));
					    xml_element_tag (_o,"Stop", _jo->DateTime2String(intervals[j].second.stop()));

					    // index -1 signals an interval covering a data gap
					    xml_element_tag (_o, _fileNameTag, (intervals[j].first == -1 ? string("") : files[intervals[j].first]));
				    }
			    }
		    }
		    else
		    {
			    ACS_LOG_WARNING("altern_dumper: WARNING!! files.size is empty!") ;
		    }
	    }
        break;
	
	case InputOriginPROC:
	    {
		    string _filename;
		    switch (i.getFileNameType())
		    {
		    case FileNameTypeREGEXP:
			    // The regexp inputs have only one filename 
			    // that is applicable for the complete processing interval
			    {
				    vector<string> files;
				    i.getPathNames(files);
				    _filename = files.at(0);
			    }
		        break;
		    default:
                {
			        // the default behaviour is to use the interim approach for
			        // - Physical
			        // - Logical
			        // - Stem
			        _filename = _jo->getWorkingDir() + "/" + _jo->buildTempFilename(i.getFileType());
                }
		        break;
		    }

		    {
			    xml_section_tag t(_o, _fileNameListTag, " count=\"1\"");
			    xml_element_tag (_o, _fileNameTag, _filename);
		    }

		    {
			    xml_section_tag t(_o, _timeIntervalListTag, " count=\"1\"");
			    {
				    xml_section_tag t(_o, "Time_Interval");
				    DateTime s1, s2;
				    _jo->getTimeInterval(s1, s2);

				    xml_element_tag (_o,"Start", _jo->DateTime2String(s1));
				    xml_element_tag (_o,"Stop", _jo->DateTime2String(s2));
				    xml_element_tag (_o, _fileNameTag, _filename);
			    }
		    }
	    }
        break;
	
	case InputOriginLOG:
	    {

		    // expect the logfilename to be set by the daemon and passed along with the input
		    vector<string> v;
		    i.getPathNames(v);

		    string file_name;
		    if (v.size()) {
			    file_name = v[0];
            }
		    else {
			    ACS_LOG_WARNING("LTAJobOrder: WARNING:" <<
				    " no logfile name received in input; generating temporary one") ;
			    file_name = _jo->getWorkingDir()+"/"+_jo->buildTempFilename(i.getFileType());
		    }

		    {
			    xml_section_tag t(_o, _fileNameListTag, " count=\"1\"");
			    xml_element_tag (_o, _fileNameTag, file_name);
		    }
		    {
			    xml_section_tag t(_o, _timeIntervalListTag, " count=\"1\"");
			    {
				    xml_section_tag t(_o, "Time_Interval");
				    DateTime s1, s2;
				    _jo->getTimeInterval(s1, s2);

				    xml_element_tag (_o,"Start", _jo->DateTime2String(s1));
				    xml_element_tag (_o,"Stop", _jo->DateTime2String(s2));
				    xml_element_tag (_o, _fileNameTag, file_name);
			    }
		    }
	    }
	    break;
	
	default:
        {
		    ACS_LOG_ERROR("altern_dumper: ERROR: unknown origin received for alternative " << i.getFileType()) ;
	    }
        break;
	}
}

LTAJobOrder::input_dumper::input_dumper(const LTAJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo)
	{}

void 
LTAJobOrder::input_dumper::operator() (const InputDescriptor& i)
{
	vector<InputAlternative> v;
	i.getList(v, always_true_pred(v));
	for_each(v.begin(), v.end(), altern_dumper(_jo, _o));
}


LTAJobOrder::confSpaces_dumper::confSpaces_dumper (ofstream& oo) : 
	_o(oo) 
{ }

void 
LTAJobOrder::confSpaces_dumper::operator() (const ConfigSpaceDescriptor& c)
{
	xml_element_tag (_o, c.getSpaceName(), c.getPath() );
}

LTAJobOrder::task_dumper::task_dumper(const LTAJobOrder* jo, ofstream& oo, bool b) : 
	_jo(jo), 
	_o(oo), 
	_brkEnabled(b) 
{}

void
LTAJobOrder::task_dumper::operator() (const Task& task)
{
	xml_section_tag t(_o, "Ipf_Proc");
	xml_element_tag (_o,"Task_Name", task.getName());
	
	// add task version information
	xml_element_tag ( _o, "Task_Version", task.getVersion() );
	
	{
	    // dump breakpoints
	    xml_section_tag t(_o, "Breakpoint");
	    xml_element_tag (_o,"Enable", _brkEnabled ? "ON" : "OFF");

	    vector<BreakpointDescriptor> l;
	    task.getBreakpointList(l);
	
		{
			ostringstream os;
			os << " count=\"" << l.size() << "\"";
			xml_section_tag t(_o, _brkFileListTag, os.str());
		
			for_each(l.begin(), l.end(), breakpoint_dumper(_jo, _o));
		}
	}

	{
		// dump inputs
		vector<InputDescriptor> l;
		task.getInputList(l);
		
		ostringstream os;
		os << " count=\"" << l.size() << "\"";
		xml_section_tag t(_o, _inputListTag, os.str());
		
		for_each(l.begin(), l.end(), input_dumper(_jo, _o));
	}

	{
		// dump outputs
		vector<OutputDescriptor> l;
		task.getOutputList(l);
		
		ostringstream os;
		os << " count=\"" << l.size() << "\"";
		xml_section_tag t(_o, _outputListTag, os.str());
		
		for_each(l.begin(), l.end(), output_dumper(_jo, _o));
	}
}


/**
** HELPER CLASSES IMPLEMENTATION - END
**/


void
LTAJobOrder::write(ofstream& o) const
{
  o << "<" << _rootTag << " xmlns:a=\"http://www.acsys.it/schemas/IPF\" xmlns:xsi=\"http://www.w3.org/2000/10/XMLSchema-instance\" xsi:schemaLocation=\"http://www.acsys.it/schemas/IPF JobOrder.xsd\">" << endl;
   

  // Ipf_Conf section
  {
    xml_section_tag t(o, "Ipf_Conf");
    xml_element_tag (o, "Processor_Name", getProcName());
    xml_element_tag (o, "Version", getProcVersion());
    xml_element_tag (o, "Order_Type", getOrderType());
    xml_element_tag (o, _orderIdTag, getOrderId());


    xml_element_tag (o,"Logging_Level", LogLevel2String(getLogLevel()));
    xml_element_tag (o,"Test", (isTest() ? "true" : "false" ));
    xml_element_tag (o,"Troubleshooting", (isTroubleshooting() ? "true" : "false" ));
    xml_element_tag (o,"Processing_Station", getProcStation() );

    {
	    xml_section_tag t(o, _configFilesTag);

	    for_each(_configSpaces.begin(), _configSpaces.end(), confSpaces_dumper(o));
    }

    {
      xml_section_tag t(o, "Sensing_Time");

      DateTime s1, s2;
      getTimeInterval(s1, s2);

      xml_element_tag (o,"Start", DateTime2String(s1));
      xml_element_tag (o,"Stop", DateTime2String(s2));
    }
  }

  // List_of_Ipf_Proc section
  {
    ostringstream os;
    os << " count=\"" << size() << "\"";
    xml_section_tag t(o, _procListTag, os.str());


    for_each(begin(), end(), task_dumper(this, o, brkEnabled()));
  }

  // Processor_Conf section
  {
    xml_section_tag t(o, _processorConfTag);
    xml_element_tag (o,_fileNameTag, getConfig());
  }
  
  // Parameter_File section
  {
    xml_section_tag t(o, _parametersFileTag);
	xml_element_tag (o, _fileNameTag, getParametersFileName());
  }

  // close the main tag
  o << "</" << _rootTag <<">" << endl;
}


void LTAJobOrder::readConfigSpaces (acs::rsResourceSet&)
{
	// Do nothing... just there are no configuration spaces to download
}

void LTAJobOrder::read (ifstream& is)
{
   ACS_THROW(LTAJobOrderException ("Cannot use plain ASCII stream for reading.") );
}


void LTAJobOrder::read (XMLIstream& is)
{
  // delete all the existing data on this jobOrder

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "LTAJobOrder::read start... ") 


  this -> clear();   // remove all items in the vector of tasks
  _configSpaces.clear(); // remove all pushed configuration spaces

  rsResourceSet rs;

  try
  {
    is >> rs;
  }
  catch (const exception& toCatch)
  {
    ACS_THROW(LTAJobOrderException( toCatch, "Cannot load job order from xml stream")) ;
  }

  try
  {
    // read the Ipf_Conf section
    {
	  rsPushNSpace nspace(rs,_ipfConfTag);

	  string s;
	  bool b;
	  unsigned int ui;

	  // processor name
	  rs.getValue (_processorNameTag, s);
	  setProcName (s);
	  // processor version
	  rs.getValue (_versionTag, s);
	  setProcVersion (s);
     // order type
	  rs.getValue (_orderTypeTag, s);
	  setOrderType (s);
     // order id
	  rs.getValue (_orderIdTag, ui);
	  setOrderId(ui);
	  // processor version
	  rs.getValue (_versionTag, s);
	  setProcVersion (s);
	  // log level
	  rs.getValue (_loggingLevelTag,s);
	  setLogLevel ( string2LogLevel (s) );
	  // test flag
	  rs.getValue (_testTag, b);
	  isTest (b);
	  // troubleshooting flag
	  rs.getValue (_troubleshootingTag, b);
	  isTroubleshooting (b);
	  // processing station
	  rs.getValue (_processingStationTag, s);
	  setProcStation (s);

	  readConfigSpaces (rs);


	  // sensing time
	  DateTime tStart, tStop;

	  // Sensing time start
	  {
	    ostringstream os;
	    os  << _sensingTimeTag << "." << _startTag;
	    rs.getValue ( os.str(), s );
	    tStart = utcs2datetime (s);
	  }
	  // Sensing time stop
	  {
	    ostringstream os;
	    os  << _sensingTimeTag << "." << _stopTag;
	    rs.getValue ( os.str(), s );
	    tStop = utcs2datetime (s);
	  }

	  setTimeInterval ( tStart, tStop);
    }

    // processor configuration file
    {
	  string s;
	  ostringstream os;
	  os << _processorConfTag << "." << _fileNameTag;
	  rs.getValue ( os.str(), s);
	  this -> setConfig (s);
    }

    // parameters file
    {
      string s;
      ostringstream os;
      os << _parametersFileTag << "." << _fileNameTag;
      rs.getValue ( os.str(), s);
      this -> setParametersFileName (s);
    }

      // read the Ipf_Proc sections
    {
	  if(! rs.hasKey(_procListTag) )    //nothing to load
	  {
		ACS_THROW(LTAJobOrderException("No list of Ipf Processors key found")) ;
	  }

	  size_t procCount = 0, procs;

	  try
	  {
	    rs.getValue(_procListCountKey, procCount);
	  }
	  catch(rsResourceSet::NotFoundException &toCatch)
	  {
	    ACS_THROW(LTAJobOrderException(toCatch, "No processors count key found" )) ;
	  }

	  procs = procCount;

	  // load procs: next block {} necessary to handle rs namespaces
	  {
		rsPushNSpace nspace(rs,_procListTag);

		for(size_t i=0; i<procs ; i++)
		{

          ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the proc n. " << i+1 << " out of " << procs) ;

		  Task t;

		  ostringstream os;
		  os << _ipfProcTag << "["<<i<<"]";
		  string prefix=os.str();
		  rsPushNSpace nspace(rs,prefix);


		  // start filling the task
		  string s;

		  rs.getValue(_taskNameTag , s);
		  t.setName(s);

		  // use the procname to produce warning messages
		  string procName = s;

		  rs.getValue(_taskVersionTag, s);
		  t.setVersion(s);

		  // load inputs: next block {} necessary to handle rs namespaces
		  {
		    size_t inputs, inputCount;

		    rs.getValue(_inputListCountKey, inputCount);

		    inputs = inputCount;

		    rsPushNSpace nspace(rs, _inputListTag);


		    // now let's load the array of inputs
		    for(size_t j=0; j<inputs; j++)
		    {

              ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the input n. " << j+1 << " out of " << inputs << " (task = " << i << ")") ;

              // there is only one alternative for each descriptor
			  InputDescriptor d;
			  InputAlternative a;

			  ostringstream os;
			  os << _inputTag << "[" << j <<"]";
			  string prefix=os.str();
			  rsPushNSpace nspace(rs,prefix);

			  // let's load the alternative for this input
			  {
			    string type;
			    rs.getValue(_fileTypeTag, type);
			    a.setFileType(type);

			    // no need to handle the unknown file name type case
			    string nameType;
			    rs.getValue(_fileNameTypeTag, nameType);
			    a.setFileNameType (String2FileNameType (nameType) );

			    a.setOrigin (InputOriginDB);

			    // read the list of filenames
			    size_t filenames, filenamesCount;

			    rs.getValue(_fileNameListCountKey, filenamesCount);

			    filenames = filenamesCount;

			    if (filenames == 0)
			    {
				  // cannot exists an input descriptor without any listed filename
				  ostringstream os;
				  os << "Input alternative without any filename in  [" << prefix << "], task [" << t.getName() << "]";
				  ACS_THROW(LTAJobOrderException(os.str()));
			    }

				vector <string> readedFileNames;
				for (size_t fn=0; fn<filenames; fn++)
				{
				  ostringstream os;
				  os << _fileNameListTag << "." << _fileNameTag << "[" << fn << "]";
				  string s;
				  rs.getValue (os.str(), s);
				  readedFileNames.push_back(s);
				}

				// add the fileNames vector to the InputAlternative object
				a.addPathNames(readedFileNames);

				// now have to read the time intervals
				size_t timeintervals, timeintervalsCount;

				rs.getValue(_timeIntervalListCountKey, timeintervalsCount);

				timeintervals = timeintervalsCount;

				for (size_t ti=0; ti<timeintervals; ti++)
				{
				  vector<string> fileNames;
				  ostringstream os;
				  os << _timeIntervalListTag << "." << _timeIntervalTag << "[" << ti << "]";
				  rsPushNSpace nspace( rs, os.str() );
				  // now look into the List_of_Time_Intervals.TimeInterval[ti]

				  // time interval
				  DateTime tStart, tStop;
				  // Time start
				  {
					rs.getValue ( _startTag, s );
					tStart = utcs2datetime (s);
				  }
				  // time stop
				  {
					rs.getValue ( _stopTag, s );
					tStop = utcs2datetime (s);
				  }
				  // have to read the filename
				  rs.getValue (_fileNameTag, s);
				  // now have to search this filename in the fileNames vector
				  vector<string>::iterator iter = find (readedFileNames.begin(), readedFileNames.end(), s);
			      int idx = -1;
				  if (s != "")
				  {
					if (iter == readedFileNames.end())
					{
                      ACS_THROW(LTAJobOrderException("No match in the input list for the filename " + s)) ;
					}
					idx = iter-readedFileNames.begin();
				  }
				  a.addInterval (std::pair<int, TimeRange>(idx, TimeRange(tStart,tStop)));


				} // for loop on time intervals

				// ok, we got the alternative: now add it to the input
				d.push_back(a);
			  }

			  // hey! we loaded all alternatives: we got the input! let's add it to the task...
			  t.addInput(d);

            } // for(size_t j=0; j<inputs; j++)

          } // close nspace of inputs

	  // Now check for the outputs

		  // load outputs
		  {
			size_t outputs, outputCount;

			rs.getValue(_outputListCountKey, outputCount);

			outputs = outputCount;

			// for each output
			for(size_t j=0; j<outputs; j++)
			{


              ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the output n. " << j+1 << " out of " << outputs << " (task = " << i << ")") ;


			  OutputDescriptor od;
			  ostringstream os;
			  os << _outputListTag << "." << _outputTag << "[" << j << "]";
			  rsPushNSpace nspace (rs, os.str() );
			  // read the file name
			  rs.getValue ( _fileNameTag, s);
			  od.setPathName (s);

			  if ( (File::getDirPath(s) + "/") == s)
			  {
				// it is a directory => output dest is DB
				od.setDestination (OutputDestDB);
			  }
			  else
			  {
				// it is a filename => output dest is PROC
				od.setDestination (OutputDestPROC);
			  }

              ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Readed filename " << s)

			  // read the File Type
			  rs.getValue (_fileTypeTag,s);
			  od.setFileType(s);

			  // no need to handle the unknown file name type case
			  string nameType;
			  rs.getValue(_fileNameTypeTag, nameType);
			  od.setFileNameType (String2FileNameType (nameType) );

			  // Have an output descriptor to add
			  t.addOutput (od);
			}
          } // Outputs

          // now it's time to check breakpoints

          {
		    bool brkEn;
		    rsPushNSpace nspace (rs, _breakpointTag);
		    rs.getValue (_enableTag, brkEn);
		    // set the breakpoint enable flag
		    this -> brkEnabled (brkEn);

		    size_t brks, brkCount;

		    rs.getValue(_brkFileListCountKey, brkCount);

		    brks = brkCount;

		    // for each breakpoint file
		    for (size_t b=0; b<brks; b++)
		    {

              ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the breakpoint n. " << b+1 << " out of " << brks << " (task = " << i << ")") ;

			  BreakpointDescriptor bd;
			  ostringstream os;
			  os << _brkFileListTag << "." << _brkFileTag << "[" << b << "]";
			  rsPushNSpace nspace (rs, os.str() );

			  // read the file name
			  rs.getValue ( _fileNameTag, s);
			  bd.setPathName (s);
			  // read the File Type
			  rs.getValue (_fileTypeTag,s);
			  bd.setFileType(s);

			  // no need to handle the unknown file name type case
			  string nameType;
			  rs.getValue(_fileNameTypeTag, nameType);
			  bd.setFileNameType (String2FileNameType (nameType) );

			  // Have an output descriptor to add
			  t.addBreakpoint (bd);
		    }
		  }  // breakpoints

		  // if we get here, we have a complete task: WOW!!! let's add it
		  this->push_back(t);
        }//for(size_t i=0; i<N; i++)
	  } // ok... all procs have been loaded for this job order
	}
  }
  catch (const exception& toCatch)
  {
    ACS_THROW(LTAJobOrderException(toCatch, "Error while reading job order")) ;
  }
}


const string mission = "CS";
const string version = "0001";
string
LTAJobOrder::buildTempFilename(const string& Filetype, const char *ext) const
{
    string fileclass(isTest() ? "TEST" : "OPER" );

    DateTime s1, s2;
    getTimeInterval(s1, s2);

    ostringstream name;
    name << mission   << "_" <<
	    fileclass << "_" << 
	    Filetype  <<
	    DateTime2String(s1) << "_" <<
	    DateTime2String(s2) << "_" <<
	    version <<
	    (ext ? ext : "");

    return name.str();
}
