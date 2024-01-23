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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2016/05/11 09:25:09  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.1  2016/03/10 14:14:33  nicvac
	OPMAN-158: satallite, mission, sensor on import rules.
	
	Revision 5.0  2013/07/07 18:34:06  marpas
	adopting libException 5.x standards and coding best practices
	qa and compilation warnings removed
	
	Revision 1.2  2012/11/19 13:55:17  marfav
	Some messages commented
	
	Revision 1.1.1.1  2012/10/17 13:11:09  chicas
	imported sources
	
	
*/

#include <DefaultJobOrder.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#include <exStream.h>
#include <StringUtils.h>
#include <Filterables.h>

#include <sstream>
#include <algorithm>

#include <sys/types.h> // PRQA S 1013
#include <unistd.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


// WARNING
// Reading a job from xml file causes:
//
// - all the inputs are stored with origin flag set to DB -> local downloaded files
//
// - the outputs are stored with destination flag set to
//    DB if the File_Name read is a directory
//    PROC if the File_Name read is a complete pathname

namespace {
    // IPF_CONFIG tags
  const string _ipfConfTag = "Ipf_Conf";
  const string _satelliteIdTag = "SatelliteId";
  const string _missionIdTag = "MissionId";
  const string _sensorIdTag = "SensorId";
  const string _processorNameTag = "Processor_Name";
  const string _versionTag = "Version";
  const string _orderTypeTag = "Order_Type";
  const string _orderIdTag = "Order_ID";
  const string _loggingLevelTag = "Logging_Level";
  const string _testTag = "Test";
  const string _troubleshootingTag = "Troubleshooting";
  const string _processingStationTag = "Processing_Station";
  const string _sensingTimeTag = "Sensing_Time";
  const string _startTag = "Start";
  const string _stopTag = "Stop";

    // IPF_PROCS tags
  const string _ipfProcTag = "Ipf_Proc";
  const string _taskNameTag = "Task_Name";
  const string _taskVersionTag = "Task_Version";
  const string _breakpointTag = "Breakpoint";
  const string _enableTag = "Enable";
  const string _brkFileTag = "Brk_File";
  const string _fileTypeTag = "File_Type";
  const string _inputTag = "Input";
  const string _timeIntervalTag = "Time_Interval";
  const string _outputTag = "Output";
  const string _fileNameTypeTag    = "File_Name_Type";

  // PROCESSOR_CONF tags
  const  string _processorConfTag = "Processor_Conf";

  // PARAMETER_FILE tags
  const  string _parametersFileTag = "Parameter_File";

  const  string _fileNameTag		= "File_Name";
  const  string _fileNameListTag	= "List_of_File_Names";
  const  string _fileNameListCountKey	= "List_of_File_Names.count.value";
  const  string _timeIntervalListTag	= "List_of_Time_Intervals";
  const  string _timeIntervalListCountKey	= "List_of_Time_Intervals.count.value";
  const  string _brkFileListTag	= "List_of_Brk_Files";
  const  string _brkFileListCountKey	= "List_of_Brk_Files.count.value";
  const  string _inputListTag	= "List_of_Inputs";
  const  string _inputListCountKey ="List_of_Inputs.count.value";
  const  string _outputListTag	= "List_of_Outputs";
  const  string _outputListCountKey = "List_of_Outputs.count.value";
  const  string _procListTag		= "List_of_Ipf_Procs";
  const  string _procListCountKey = "List_of_Ipf_Procs.count.value";
  const  string _rootTag = "Ipf_Job_Order";
  const  string _configFilesTag = "Config_Files";

  const  string _defaultExtension = ".DBL";
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultJobOrder)


DefaultJobOrder::DefaultJobOrder() : JobOrder(), _satelliteId(), _sensorId()
{
}

DefaultJobOrder::~DefaultJobOrder() throw() {}


string
DefaultJobOrder::DateTime2String(const DateTime &d) const
{
  DateTime::UTCD utcd(d); // PRQA S 3050
  ostringstream os;
  os << setfill('0') <<
	setw(4) << utcd.year << // PRQA S 4400 3
	setw(2) << utcd.month <<
	setw(2) << utcd.day	<<
	'_' <<
	setw(2) << utcd.hour << // PRQA S 4400 4
	setw(2) << utcd.min	<<
	setw(2) << utcd.sec <<
	setw(3) << utcd.millisec() <<
	"000"; //usecs

  return os.str();
}

DateTime
DefaultJobOrder::utcs2datetime (const string& s) const
{
	DateTime::UTCD utcd;
	// JobOrder date format is YYYYMMDD_hhmmssmmm000"
	string jobOrderDateFormat = "%04d%02d%02d_%02d%02d%02d%03d000";
	long msec = 0;
	sscanf( s.c_str(), jobOrderDateFormat.c_str(),
						&utcd.year, 
						&utcd.month, 
						&utcd.day, 
						&utcd.hour, 
						&utcd.min, 
						&utcd.sec, 
						&msec);

	//cout << "ris=" << nr << " " << s << "--> MILLISEC=" << msec << "\n";

	string syear = s.substr(0,4); // PRQA S 4400 7
	string smonth = s.substr(4,2);
	string sday = s.substr(6,2);
	string shour = s.substr(9,2);
	string smin = s.substr(11,2);
	string ssec = s.substr(13,2);
	string smsec = s.substr(15,3);
	msec = std::atol(smsec.c_str());
	//cout << s << "-->" << syear << " " << smonth << " " << sday << " " << shour << " " << smin << " " << ssec << " " << smsec << " [" << msec << "]\n" ;

	utcd.millisec(msec); // PRQA S 3000
	return utcd;
}

/**
** HELPER CLASSES IMPLEMENTATION - BEGIN
**/

unsigned int DefaultJobOrder::xml_indent::_indentCount = 0;

DefaultJobOrder::xml_section_tag::xml_section_tag(ofstream& o, const string &s, const string &a) :
    xml_indent(), 
	_o(o), // PRQA S 2528
	_s(s)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }

	if (a != "") {
	    o << "<" << s << " " << a << ">\n" ;
    }
	else {
		o << "<" << s << ">\n" ;
    }

	_indentCount++;

}

DefaultJobOrder::xml_section_tag::~xml_section_tag() throw() 
{
    // PRQA S 4631 L1
	if (_indentCount>0) {
		_indentCount--;
    }

	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		_o << "\t";
    }

	_o << "</" << _s << ">\n" ;
    // PRQA L:L1
}

DefaultJobOrder::xml_element_tag::xml_element_tag(ofstream& o, const string &s, const string &v) :
    xml_indent()
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }
	o << "<" << s << ">" << v << "</" << s << ">\n" ;
}

DefaultJobOrder::xml_element_tag::xml_element_tag(ofstream& o, const string &s, unsigned int u) :
    xml_indent()
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }
	o << "<" << s << ">" << u << "</" << s << ">\n" ;
}

DefaultJobOrder::breakpoint_dumper::breakpoint_dumper(const DefaultJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	 _o(oo) // PRQA S 2528
{}

void 
DefaultJobOrder::breakpoint_dumper::operator() (const BreakpointDescriptor& b)
{
	xml_section_tag t(_o, "Brk_File");
	xml_element_tag (_o,"File_Type", b.getFileType());

	FileNameType fnt = b.getFileNameType();
	if ( fnt == FileNameTypeUNKNOWN )
	{
		ACS_LOG_ERROR("breakpoint_dumper: ERROR: unknown FileNameType received: " << b.getFileType()) ;
	}
	else
	{
		xml_element_tag (_o, _fileNameTypeTag, FileNameType2String (fnt) );
	}

	xml_element_tag (_o,_fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(b.getFileType(), _defaultExtension.c_str()));
}


DefaultJobOrder::output_dumper::output_dumper(const DefaultJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo) // PRQA S 2528
{}

void 
DefaultJobOrder::output_dumper::operator() (const OutputDescriptor& d)
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
		            break;
                }
		    default:
                {
			        // if not regexp always use the DIRECTORY specification
			        xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/");
		            break;
                }
		    }
	        break;
        }
	
	case OutputDestPROC:
        {
		    switch (d.getFileNameType())
		    {
		    // Improvement needed for the PROC naming convention concerning
		    // - FileNameTypePHYSICAL
		    // - FileNameTypeLOGICAL
		    // - FileNameTypeSTEM
		    default:
                {
			        // by default use the INTERIM specification 
			        // that is physical without extension
			        xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(d.getFileType()));
		            break;
                }
		    }
	        break;
        }
	
	case OutputDestDBPROC:
        {
		    switch (d.getFileNameType())
		    {
		    case FileNameTypeREGEXP:
                {
			        xml_element_tag (_o, _fileNameTag, d.getPathName());
		            break;
                }
		    default:
                {
			        // if not regexp always use the DIRECTORY specification
			        xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/");
		            break;
                }
		    }
	        break;
	    }
	default:
        {
		    ACS_LOG_ERROR("output_dumper: ERROR: unknown origin received for output " << d.getFileType());
	        break;
        }
	}
}


DefaultJobOrder::file_dumper::file_dumper(const DefaultJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo) // PRQA S 2528
{}

void 
DefaultJobOrder::file_dumper::operator() (const string& s)
{ 
	xml_element_tag (_o, _fileNameTag, s); 
}


DefaultJobOrder::altern_dumper::altern_dumper(const DefaultJobOrder* jo, ofstream& oo) :
	_jo(jo),
	_o(oo) // PRQA S 2528
{}

void 
DefaultJobOrder::altern_dumper::operator() (const InputAlternative& i)
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
		if (not files.empty()) // check if not empty
		{
			{
				ostringstream os;
				os << " count=\"" << files.size() << "\"";
				xml_section_tag t1(_o, _fileNameListTag, os.str());
			
				for_each(files.begin(), files.end(), file_dumper(_jo, _o));
			}
			{
				// dump intervals
				vector< pair <int, TimeRange> > intervals;
				i.getIntervals(intervals);
			
				ostringstream os;
				os << " count=\"" << intervals.size() << "\"";
				xml_section_tag t1(_o, _timeIntervalListTag, os.str());
			
                size_t intsize = intervals.size() ;
				for (size_t j=0; j<intsize; ++j)
				{
					xml_section_tag t2(_o, "Time_Interval");
			
					xml_element_tag (_o,"Start", _jo->DateTime2String(intervals[j].second.start()));
					xml_element_tag (_o,"Stop", _jo->DateTime2String(intervals[j].second.stop()));
			
					// index -1 signals an interval covering a data gap
					xml_element_tag (_o, _fileNameTag, ((intervals[j].first == -1) ? string("") : files[intervals[j].first])); // PRQA S 3000, 3081, 3380, 3385
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
		    break;
			}
		default:
            {
			    // the default behaviour is to use the interim approach for
			    // - Physical
			    // - Logical
			    // - Stem
			    _filename = _jo->getWorkingDir() + "/" + _jo->buildTempFilename(i.getFileType());
		        break;
            }
		}

		{
			xml_section_tag t1(_o, _fileNameListTag, " count=\"1\"");
			xml_element_tag (_o, _fileNameTag, _filename);
		}

		{
			xml_section_tag t1(_o, _timeIntervalListTag, " count=\"1\"");
			{
				xml_section_tag t2(_o, "Time_Interval");
				DateTime s1;
                DateTime s2;
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
		if (not v.empty()) {
			file_name = v[0];
        }
		else
		{
			ACS_LOG_WARNING("DefaultJobOrder: WARNING:" <<
				" no logfile name received in input; generating temporary one") ;
			file_name = _jo->getWorkingDir()+"/"+_jo->buildTempFilename(i.getFileType());
		}
	
		{
			xml_section_tag t1(_o, _fileNameListTag, " count=\"1\"");
			xml_element_tag (_o, _fileNameTag, file_name);
		}
		{
			xml_section_tag t1(_o, _timeIntervalListTag, " count=\"1\"");
			{
				xml_section_tag t2(_o, "Time_Interval");
				DateTime s1 ;
                DateTime s2;
				_jo->getTimeInterval(s1, s2);
	
				xml_element_tag (_o,"Start", _jo->DateTime2String(s1));
				xml_element_tag (_o,"Stop", _jo->DateTime2String(s2));
				xml_element_tag (_o, _fileNameTag, file_name);
			}
		}
	}
	break;
	
	default:
		ACS_LOG_ERROR("altern_dumper: ERROR: unknown origin received for alternative " << i.getFileType());
	break;
	}
}

DefaultJobOrder::input_dumper::input_dumper(const DefaultJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo) // PRQA S 2528
	{}

void 
DefaultJobOrder::input_dumper::operator() (const InputDescriptor& i)
{
	vector<InputAlternative> v;
	i.getList(v, always_true_pred(v));
	for_each(v.begin(), v.end(), altern_dumper(_jo, _o));
}


DefaultJobOrder::confSpaces_dumper::confSpaces_dumper (ofstream& oo) : 
	_o(oo) // PRQA S 2528
{ }

void 
DefaultJobOrder::confSpaces_dumper::operator() (const ConfigSpaceDescriptor& c)
{
	xml_element_tag (_o, c.getSpaceName(), c.getPath() );
}

DefaultJobOrder::task_dumper::task_dumper(const DefaultJobOrder* jo, ofstream& oo, bool b) : 
	_jo(jo), 
	_o(oo), // PRQA S 2528
	_brkEnabled(b) 
{}

void
DefaultJobOrder::task_dumper::operator() (const Task& task)
{
	xml_section_tag t(_o, "Ipf_Proc");
	xml_element_tag (_o,"Task_Name", task.getName());
	
	// add task version information
	xml_element_tag ( _o, "Task_Version", task.getVersion() );
	
	{
	    // dump breakpoints
	    xml_section_tag t1(_o, "Breakpoint");
	    xml_element_tag (_o,"Enable", _brkEnabled ? "ON" : "OFF"); // PRQA S 3380

	    vector<BreakpointDescriptor> l;
	    task.getBreakpointList(l);
	
		{
			ostringstream os;
			os << " count=\"" << l.size() << "\"";
			xml_section_tag t2(_o, _brkFileListTag, os.str());
		
			for_each(l.begin(), l.end(), breakpoint_dumper(_jo, _o));
		}
	}

	{
		// dump inputs
		vector<InputDescriptor> l;
		task.getInputList(l);
		
		ostringstream os;
		os << " count=\"" << l.size() << "\"";
		xml_section_tag t1(_o, _inputListTag, os.str());
		
		for_each(l.begin(), l.end(), input_dumper(_jo, _o));
	}

	{
		// dump outputs
		vector<OutputDescriptor> l;
		task.getOutputList(l);
		
		ostringstream os;
		os << " count=\"" << l.size() << "\"";
		xml_section_tag t1(_o, _outputListTag, os.str());
		
		for_each(l.begin(), l.end(), output_dumper(_jo, _o));
	}
}


/**
** HELPER CLASSES IMPLEMENTATION - END
**/


void
DefaultJobOrder::write(ofstream& o) const
{
  o << "<" << _rootTag << " xmlns:a=\"http://www.acsys.it/schemas/IPF\" xmlns:xsi=\"http://www.w3.org/2000/10/XMLSchema-instance\" xsi:schemaLocation=\"http://www.acsys.it/schemas/IPF JobOrder.xsd\">\n" ;
   

  // Ipf_Conf section
  {
    xml_section_tag t(o, "Ipf_Conf");
    xml_element_tag (o, _satelliteIdTag, getSatelliteId());
    xml_element_tag (o, _missionIdTag, getMission());
    xml_element_tag (o, _sensorIdTag, getSensorId());
    xml_element_tag (o, "Processor_Name", getProcName());
    xml_element_tag (o, "Version", getProcVersion());
    xml_element_tag (o, "Order_Type", getOrderType());
    xml_element_tag (o, _orderIdTag, getOrderId());


    xml_element_tag (o,"Logging_Level", LogLevel2String(getLogLevel()));
    xml_element_tag (o,"Test", (isTest() ? "true" : "false" )); // PRQA S 3380
    xml_element_tag (o,"Troubleshooting", (isTroubleshooting() ? "true" : "false" )); // PRQA S 3380
    xml_element_tag (o,"Processing_Station", getProcStation() );

    {
	    xml_section_tag t1(o, _configFilesTag);

	    for_each(_configSpaces.begin(), _configSpaces.end(), confSpaces_dumper(o)); // PRQA S 3081
    }

    {
      xml_section_tag t1(o, "Sensing_Time");

      DateTime s1; 
      DateTime s2;
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
  o << "</" << _rootTag <<">\n" ;
}


void DefaultJobOrder::readConfigSpaces (acs::rsResourceSet&)
{
	// Do nothing... just there are no configuration spaces to download
}

void DefaultJobOrder::read (ifstream& is)
{
   ACS_THROW (DefaultJobOrderException ("Cannot use plain ASCII stream for reading.") ); // PRQA S 3081
}


void DefaultJobOrder::read (XMLIstream& is)
{
    // delete all the existing data on this jobOrder

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"start... ") ;


  this -> clear();   // remove all items in the vector of tasks
  _configSpaces.clear(); // remove all pushed configuration spaces

  rsResourceSet rs;

  try
  {
	is >> rs;
  }
  catch (const exception& toCatch)
  {
    ACS_THROW(DefaultJobOrderException( toCatch, "Cannot load job order from xml stream")) ;
  }


  try
  {
	// read the Ipf_Conf section
	{
	    rsPushNSpace nspace(rs,_ipfConfTag);

	    string s;
	    bool b = false ;
	    unsigned int ui = 0 ;

	    //Satellite id
	    rs.getValue( _satelliteIdTag, s );
	    setSatelliteId(s);

	    //Mission id
	    rs.getValue( _missionIdTag, s );
	    setMission(s);

	    //Sensor id
	    rs.getValue( _sensorIdTag, s );
	    setSensorId( s );

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
	    DateTime tStart; 

	    // Sensing time start
	    {
		    ostringstream os;
		    os  << _sensingTimeTag << "." << _startTag;
		    rs.getValue ( os.str(), s );
		    tStart = utcs2datetime (s);
	    }

        DateTime tStop;
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
		    ACS_THROW(DefaultJobOrderException("No list of Ipf Processors key found")) ; // PRQA S 3081
	    }

	    size_t procCount = 0; 
	    try
	    {
	      rs.getValue(_procListCountKey, procCount);
	    }
	    catch(rsResourceSet::NotFoundException &toCatch)
	    {
	      ACS_THROW(DefaultJobOrderException(toCatch, "No processors count key found" )) ;
	    }

	    size_t procs = procCount;

	    // load procs: next block {} necessary to handle rs namespaces
	    {
		    rsPushNSpace nspace(rs,_procListTag);

		    for(size_t i=0; i<procs ; i++)
		    {

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the proc n. " << i+1 << " out of " << procs) ; // PRQA S 3084

		        Task t;

		        ostringstream os;
		        os << _ipfProcTag << "["<<i<<"]";
		        string prefix=os.str();
		        rsPushNSpace nspace1(rs,prefix);


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
                    size_t inputCount = 0 ;

			        rs.getValue(_inputListCountKey, inputCount);

			        size_t inputs = inputCount;

			        rsPushNSpace nspace2(rs, _inputListTag);


			        // now let's load the array of inputs
			        for(size_t j=0; j<inputs; j++)
			        {

                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the input n. " << j+1 << " out of " << inputs << " (task = " << i << ")" ); // PRQA S 3084

                                    // there is only one alternative for each descriptor
			            InputDescriptor d;
			            InputAlternative a;

			            ostringstream os1;
			            os1 << _inputTag << "[" << j <<"]";
			            string prefix1=os1.str();
			            rsPushNSpace nspace3(rs,prefix1);

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
				            size_t filenamesCount = 0 ;

				            rs.getValue(_fileNameListCountKey, filenamesCount);

				            size_t filenames = filenamesCount;

				            if (filenames == 0)
				            {
					            // cannot exists an input descriptor without any listed filename
					            ACS_THROW(DefaultJobOrderException( // PRQA S 3081
                                    "Input alternative without any filename in  [" + prefix1 + "], task [" + t.getName() + "]"));
				            }

				            vector <string> readedFileNames;
				            for (size_t fn=0; fn<filenames; fn++)
				            {
				                ostringstream os2;
				                os2 << _fileNameListTag << "." << _fileNameTag << "[" << fn << "]";
				                string s1;
				                rs.getValue (os2.str(), s1);
				                readedFileNames.push_back(s1);
				            }

				            // add the fileNames vector to the InputAlternative object
				            a.addPathNames(readedFileNames);

				            // now have to read the time intervals
				            size_t timeintervalsCount = 0 ;

				            rs.getValue(_timeIntervalListCountKey, timeintervalsCount);

				            size_t timeintervals = timeintervalsCount;

				            for (size_t ti=0; ti<timeintervals; ti++)
				            {
				                vector<string> fileNames;
				                ostringstream os3;
				                os3 << _timeIntervalListTag << "." << _timeIntervalTag << "[" << ti << "]";
				                rsPushNSpace nspace4( rs, os3.str() );
				                // now look into the List_of_Time_Intervals.TimeInterval[ti]

				                // time interval
				                DateTime tStart ;
				                // Time start
				                {
					                rs.getValue ( _startTag, s );
					                tStart = utcs2datetime (s);
				                }
				                // time stop
                                DateTime tStop;
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
						                ACS_THROW(DefaultJobOrderException("No match in the input list for the filename " + s)) ; // PRQA S 3081
					                }
					                idx = iter-readedFileNames.begin(); // PRQA S 3010
				                }
				                a.addInterval (make_pair ( idx, TimeRange(tStart,tStop) ));


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
			        size_t outputCount = 0 ;

			        rs.getValue(_outputListCountKey, outputCount);

			        size_t outputs = outputCount;

			        // for each output
			        for(size_t j=0; j<outputs; j++)
			        {


                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the output n. " << j+1 << " out of " << outputs << " (task = " << i << ")" ) ; // PRQA S 3084

				        OutputDescriptor od;
				        ostringstream os4;
				        os4 << _outputListTag << "." << _outputTag << "[" << j << "]";
				        rsPushNSpace nspace5 (rs, os4.str() );
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


                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Read filename " << s) ;


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
				    rsPushNSpace nspace6 (rs, _breakpointTag);
				    bool brkEn = false ;
				    rs.getValue (_enableTag, brkEn);
				    // set the breakpoint enable flag
				    this -> brkEnabled (brkEn);

				    size_t brkCount = 0 ;

				    rs.getValue(_brkFileListCountKey, brkCount);

				    size_t brks = brkCount;

				    // for each breakpoint file
				    for (size_t b=0; b<brks; b++)
				    {

                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading the breakpoint n. " << b+1 << " out of " << brks << " (task = " << i << ")" ); // PRQA S 3084

					    BreakpointDescriptor bd;
					    ostringstream os5;
					    os5 << _brkFileListTag << "." << _brkFileTag << "[" << b << "]";
					    rsPushNSpace nspace7 (rs, os5.str() );

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
	ACS_THROW(DefaultJobOrderException(toCatch, "Error while reading job order")) ;
  }
}

namespace {
    const string mission = "CS";
    const string version = "0001";
}

string
DefaultJobOrder::buildTempFilename(const string& Filetype, const char *ext) const
{
    string fileclass(isTest() ? "TEST" : "OPER" ); // PRQA S 3380

    DateTime s1 ;
    DateTime s2;
    getTimeInterval(s1, s2);

    ostringstream name;
    name << mission   << "_" <<
	    fileclass << "_" << 
	    Filetype  <<
	    DateTime2String(s1) << "_" <<
	    DateTime2String(s2) << "_" <<
	    version <<
	    (ext ? ext : ""); // PRQA S 3380

    return name.str();
}



_ACS_END_NAMESPACE

