/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Ocada JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 1.6  2015/06/22 09:04:23  damdec
	Compilation warnings removed.
	
	Revision 1.5  2012/05/18 15:19:25  marpas
	getting rid of some compiler warnings
	
	Revision 1.4  2012/03/28 16:14:04  damdec
	Destructor updated.
	
	Revision 1.3  2009/09/11 16:56:01  marpas
	global symbol made private to the compilation unit
	
	Revision 1.2  2009/03/09 16:10:22  chicas
	added parameter files
	
	Revision 1.1.1.1  2009/03/02 14:40:14  chicas
	Just imported
	
*/


#include <OcadaJobOrder.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <AsciiTime.h>
#include <TimeFormat.h>
#include <File.h>
#include <exStream.h>
#include <Filterables.h>

#include <sstream>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

// WARNING
// Reading a job from xml file causes:
//
// - all the inputs are stored with origin flag set to DB -> local downloaded files
//
// - the outputs are stored with destination flag set to
//    DB if the File_Name read is a directory
//    PROC if the File_Name read is a complete pathname


// IPF_CONFIG tags
static const char* _ipfConfTag = "Ipf_Conf";
static const char* _processorNameTag = "Processor_Name";
static const char* _versionTag = "Version";
static const char* _orderTypeTag = "Order_Type";
static const char* _orderIdTag = "Order_ID"; /**/
static const char* _loggingLevelTag = "Logging_Level";
static const char* _testTag = "Test";
static const char* _troubleshootingTag = "Troubleshooting";
static const char* _processingStationTag = "Processing_Station";
static const char* _sensingTimeTag = "Sensing_Time";
static const char* _startTag = "Start";
static const char* _stopTag = "Stop";

// IPF_PROCS tags
static const char* _ipfProcTag = "Ipf_Proc";
static const char* _taskNameTag = "Task_Name";
static const char* _taskVersionTag = "Task_Version";
static const char* _breakpointTag = "Breakpoint";
static const char* _enableTag = "Enable";
static const char* _brkFileTag = "Brk_File";
static const char* _fileTypeTag = "File_Type";
static const char* _inputTag = "Input";
static const char* _timeIntervalTag = "Time_Interval";
static const char* _outputTag = "Output";
static const char *_fileNameTypeTag 	= "File_Name_Type";

// PROCESSOR_CONF tags
static const char *_processorConfTag = "Processor_Conf";

// PARAMITER_FILE tags
static const char *_parametersFileTag = "Parameter_File"; /**/



static const char *_fileNameTag		= "File_Name";
static const char *_fileNameListTag	= "List_of_File_Names";
static const char *_fileNameListCountKey	= "List_of_File_Names.count.value";
static const char *_timeIntervalListTag	= "List_of_Time_Intervals";
static const char *_timeIntervalListCountKey	= "List_of_Time_Intervals.count.value";
static const char *_brkFileListTag	= "List_of_Brk_Files";
static const char *_brkFileListCountKey	= "List_of_Brk_Files.count.value";
static const char *_inputListTag	= "List_of_Inputs";
static const char *_inputListCountKey ="List_of_Inputs.count.value";
static const char *_outputListTag	= "List_of_Outputs";
static const char *_outputListCountKey = "List_of_Outputs.count.value";
static const char *_procListTag		= "List_of_Ipf_Procs";
static const char *_procListCountKey = "List_of_Ipf_Procs.count.value";
static const char *_rootTag = "Ipf_Job_Order";
static const char *_configFilesTag = "Config_Files"; /**/
// static const char *_configFilesTag = "Config_Files";
#ifdef NOT_USE_SUBCLASSES
static const char *_constantsTag = "Config_Files.Geophysical_Constants";
#endif

static const char *_defaultExtension = ".DBL";

ACS_CLASS_DEFINE_DEBUG_LEVEL(OcadaJobOrder)


OcadaJobOrder::OcadaJobOrder()
{
  
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}

OcadaJobOrder::~OcadaJobOrder() throw()
{}

/*?*/
string
OcadaJobOrder::DateTime2FileNamePart(DateTime d) const
{
  DateTime::UTCD utcd(d);
  ostringstream os;
  os << setfill('0') <<
	setw(4) << utcd.year	<<
	setw(2) << utcd.month	<<
	setw(2) << utcd.day	<<
	'T' <<
	setw(2) << utcd.hour	<<
	setw(2) << utcd.min	<<
	setw(2) << utcd.sec;
  return os.str();
}

string
OcadaJobOrder::DateTime2String(DateTime d) const
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
OcadaJobOrder::utcs2datetime (const string& s) const
{
	DateTime::UTCD utcd ;
	// JobOrder date format is YYYYMMDD_hhmmssmmm000"
	string jobOrderDateFormat = "%04d%02d%02d_%02d%02d%02d%06lu";
	unsigned long usecs;
	sscanf( s.c_str(), jobOrderDateFormat.c_str(),
						&utcd.year,
						&utcd.month,
						&utcd.day,
						&utcd.hour,
						&utcd.min,
						&utcd.sec,
						&usecs);
	utcd.microsec (usecs);
	return utcd;
}


unsigned int OcadaJobOrder::xml_indent::_indentCount = 0;

OcadaJobOrder::xml_section_tag::xml_section_tag(ofstream& o, string s, string a)
      : _o(o), _s(s)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++)
		o << "\t";

	if (a != "")
		o << "<" << s << " " << a << ">\n";
	else
		o << "<" << s << ">\n";

	_indentCount++;

}

OcadaJobOrder::xml_section_tag::~xml_section_tag() throw() 
{
	if (_indentCount>0)
		_indentCount--;

	// indentation
	for (unsigned int i=0; i<_indentCount; i++)
		_o << "\t";

	_o << "</" << _s << ">\n" ;
}


OcadaJobOrder::xml_element_tag::xml_element_tag(ofstream& o, string s, string v)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++)
		o << "\t";
	o << "<" << s << ">" << v << "</" << s << ">\n" ;
}

OcadaJobOrder::xml_element_tag::xml_element_tag(ofstream& o, string s, unsigned int u)
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++)
		o << "\t";
	o << "<" << s << ">" << u << "</" << s << ">\n" ;
}

OcadaJobOrder::breakpoint_dumper::breakpoint_dumper(const OcadaJobOrder* jo, ofstream& oo) : 
	_jo(jo), 
	_o(oo) 
{}

void 
OcadaJobOrder::breakpoint_dumper::operator() (const BreakpointDescriptor& b)
{
	xml_section_tag t(_o, "Brk_File");
	xml_element_tag (_o,"File_Type", b.getFileType());

	FileNameType fnt = b.getFileNameType();
	if ( fnt == FileNameTypeUNKNOWN )
	{
	    excerr << "alter_dumper: ERROR: unknown FileNameType received for alternative " << b.getFileType() << endl;
	}
	else
	{
	    xml_element_tag (_o, _fileNameTypeTag, FileNameType2String (fnt) );
	}

	xml_element_tag (_o,_fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(b.getFileType(), _defaultExtension));
}


OcadaJobOrder::output_dumper::output_dumper(const OcadaJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo) 
{}

void OcadaJobOrder::output_dumper::operator() (const OutputDescriptor& d)
{
	  xml_section_tag t(_o, "Output");
	  xml_element_tag (_o,"File_Type", d.getFileType());

	  FileNameType fnt = d.getFileNameType();
	  if ( fnt == FileNameTypeUNKNOWN )
	  {
	      excerr << "output_dumper: ERROR: unknown FileNameType received for output " << d.getFileType() << endl;
	  }
	  else
	  {
	      xml_element_tag (_o, _fileNameTypeTag, FileNameType2String (fnt) );
	  }

	  switch (d.getDestination())
	  {
	      case OutputDestDB:
		  xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/");
	      break;

	      case OutputDestPROC:
		  xml_element_tag (_o, _fileNameTag, _jo->getWorkingDir()+"/"+_jo->buildTempFilename(d.getFileType()));
	      break;

	      default:
		  excerr << "output_dumper: ERROR: unknown origin received for output " << d.getFileType() << endl;
	      break;
	  }
}

OcadaJobOrder::file_dumper::file_dumper(const OcadaJobOrder* jo, ofstream& oo) : 
	_jo(jo), 
	_o(oo) 
{}

void 
OcadaJobOrder::file_dumper::operator() (const string& s)
{ 
	xml_element_tag (_o, _fileNameTag, s); 
}


OcadaJobOrder::altern_dumper::altern_dumper(const OcadaJobOrder* jo, ofstream& oo) : 
	_jo(jo), 
	_o(oo) 
{}

void OcadaJobOrder::altern_dumper::operator() (const InputAlternative& i)
{
	    xml_section_tag t(_o, "Input");
	    xml_element_tag (_o,"File_Type", i.getFileType());
	FileNameType fnt = i.getFileNameType();
	if ( fnt == FileNameTypeUNKNOWN )
	{
		  excerr << "alter_dumper: ERROR: unknown FileNameType received for alternative " << i.getFileType() << endl;
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
	    excerr << "altern_dumper: WARNING!! files.size is empty!" << endl;
	  }
	}
	break;

	case InputOriginPROC:
	{
	  string _filename = _jo->getWorkingDir() + "/" + _jo->buildTempFilename(i.getFileType());


	  {
	    xml_section_tag t(_o, _fileNameListTag, " count=\"1\"");
	    xml_element_tag (_o, _fileNameTag, _filename);
	  }
	    {
	      // dump intervals
	      vector< pair <int, TimeRange> > intervals;
	      i.getIntervals(intervals);

	      vector<string> files;
	      files.push_back(_filename);

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
	break;

	case InputOriginLOG:
	{

	  // expect the logfilename to be set by the daemon and passed along with the input
	  vector<string> v;
	  i.getPathNames(v);

	  string file_name;
	  if (v.size())
	    file_name = v[0];
	  else
	  {
	    excerr << "OcadaJobOrder: WARNING:" <<
		      " no logfile name received in input; generating temporary one" << endl;
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
	  excerr << "altern_dumper: ERROR: unknown origin received for alternative " << i.getFileType() << endl;
	break;
      }
}


OcadaJobOrder::input_dumper::input_dumper(const OcadaJobOrder* jo, ofstream& oo) : 
	_jo(jo),
	_o(oo)
{}

void 
OcadaJobOrder::input_dumper::operator() (const InputDescriptor& i)
{
      for_each(i.begin<InputAlternative>(), i.end<InputAlternative>(), altern_dumper(_jo, _o));
}


OcadaJobOrder::confSpaces_dumper::confSpaces_dumper (ofstream& oo): 
	_o(oo) 
{ }

void 
OcadaJobOrder::confSpaces_dumper::operator() (const ConfigSpaceDescriptor& c)
{
           xml_element_tag (_o, c.getSpaceName(), c.getPath() );
}

OcadaJobOrder::task_dumper::task_dumper(const OcadaJobOrder* jo, ofstream& oo, bool b) : 
	_jo(jo), 
	_o(oo), 
	_brkEnabled(b) 
{}

void 
OcadaJobOrder::task_dumper::operator() (const Task& task)
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


void
OcadaJobOrder::write(ofstream& o) const
{
  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "write start... ") ;

  o << "<" << _rootTag << " xmlns:a=\"http://www.acsys.it/schemas/IPF\" xmlns:xsi=\"http://www.w3.org/2000/10/XMLSchema-instance\" xsi:schemaLocation=\"http://www.acsys.it/schemas/IPF JobOrder.xsd\">" << endl;
   


  // Ipf_Conf section
  {
    xml_section_tag t(o, "Ipf_Conf");
    xml_element_tag (o,_processorNameTag, getProcName());
    xml_element_tag (o,_versionTag, getProcVersion());
    xml_element_tag (o,_orderTypeTag, getOrderType());
    xml_element_tag (o, _orderIdTag, getOrderId()); /**/


    xml_element_tag (o,_loggingLevelTag, LogLevel2String(getLogLevel()));
    xml_element_tag (o,_testTag, (isTest() ? "true" : "false" ));
    xml_element_tag (o,_troubleshootingTag, (isTroubleshooting() ? "true" : "false" ));
    xml_element_tag (o,_processingStationTag, getProcStation() );

    {
	{
		xml_section_tag t(o, _configFilesTag);

		for_each(_configSpaces.begin(), _configSpaces.end(), confSpaces_dumper(o));
	}
    }

    {
      xml_section_tag t(o, _sensingTimeTag);

      DateTime s1, s2;
      getTimeInterval(s1, s2);

      xml_element_tag (o, _startTag, DateTime2String(s1));
      xml_element_tag (o, _stopTag, DateTime2String(s2));
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




void OcadaJobOrder::readConfigSpaces (acs::rsResourceSet&)
{
	// Do nothing... just there are no configuration spaces to download
	//throw (OcadaJobOrderException ("Cannot read the configuration spaces tag. Try to use one of the specialized subclass of OcadaJobOrder.") );
}

void OcadaJobOrder::read (ifstream& is)
{
   ACS_THROW (OcadaJobOrderException ("Cannot use plain ASCII stream for reading.") );
}


void OcadaJobOrder::read (XMLIstream& is)
{
    // delete all the existing data on this jobOrder

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "read start... ") ;


    this -> clear();   // remove all items in the vector of tasks
    _configSpaces.clear(); // remove all pushed configuration spaces

    rsResourceSet rs;

    try
    {
	is >> rs;
    }

    catch (const exException& toCatch)
    {
	OcadaJobOrderException e( toCatch, "Cannot load job order from xml stream") ;
	ACS_THROW (e) ;
    }
    catch (...)
    {
	OcadaJobOrderException e(string("Unexpected exception while loading job order from xml stream")) ;
	ACS_THROW (e) ;
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
	    rs.getValue (_orderIdTag, ui);/**/
	    setOrderId(ui);/**/
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

#ifdef NOT_USE_SUBCLASSES
	    // constants
	    rs.getValue (_constantsTag,s);
	    ConfigSpaceDescriptor csd ("Geophysical_Constants", s);
	    addConfigSpace (csd);
#else //NOT_USE_SUBCLASSES
	    readConfigSpaces (rs);
#endif //NOT_USE_SUBCLASSES


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
		 OcadaJobOrderException e(string("No list of Ipf Processors key found")) ;
		    ACS_THROW (e) ;
	    }

	    size_t procCount = 0, procs;

	    try
	    {
	      rs.getValue(_procListCountKey, procCount);
	    }
	    catch(rsResourceSet::NotFoundException &toCatch)
	    {
	      OcadaJobOrderException e(toCatch, "No processors count key found" ) ;
	      ACS_THROW (e) ;
	    }

#ifdef RESOURCE_SET_BUG_FIXED
	    try
	    {
	      procs=rs.getArraySize(_procListTag);
	    }
	    catch(rsResourceSet::NotFoundException &toCatch)
	    {
		OcadaJobOrderException e(toCatch,  "No processors found " ) ;
		ACS_THROW (e) ;
	    }

	    if (procCount != procs)
	    {
		ostringstream os;
		os << "Ipf processors count mismatch [count=" << procCount << ", size=" << procs << "]" ;
		OcadaJobOrderException e(os.str()) ;
		ACS_THROW (e) ;
	    }
#else
	    procs = procCount;
#endif // RESOURCE_SET_BUG_FIXED

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

#ifdef RESOURCE_SET_BUG_FIXED
			try
			{	
			    inputs=rs.getArraySize(_inputListTag);
			}
			catch(rsResourceSet::NotFoundException &)
			{
			    inputs=0;
			}

			if (inputCount != inputs)
			{
				ostringstream os;
				os << "Input count mismatch [count=" << inputCount << ", size=" << inputs << "], task [" << t.getName() << "]";
			    OcadaJobOrderException e(os.str());
			    ACS_THROW (e) ;
			}
#else
			inputs = inputCount;
#endif // RESOURCE_SET_BUG_FIXED


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

#ifdef RESOURCE_SET_BUG_FIXED
				try
				{	
				    filenames=rs.getArraySize(_fileNameListTag);
				}
				catch(rsResourceSet::NotFoundException &)
				{
				    filenames=0;
				}

				if (filenamesCount != filenames)
				{
					ostringstream os;
					os << "Input file name count mismatch [count=" << filenamesCount << ", size=" << filenames << "]";
				    OcadaJobOrderException e(os.str());
				    ACS_THROW (e) ;
				}
#else
				filenames = filenamesCount;
#endif // RESOURCE_SET_BUG_FIXED

				if (filenames == 0)
				{
					// cannot exists an input descriptor without any listed filename
					ostringstream os;
					os << "Input alternative without any filename in  [" << prefix << "], task [" << t.getName() << "]";
					OcadaJobOrderException e(os.str());
					ACS_THROW (e) ;
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

#ifdef RESOURCE_SET_BUG_FIXED
				try
				{	
				    timeintervals=rs.getArraySize(_timeIntervalListTag);
				}
				catch(rsResourceSet::NotFoundException &)
				{
				    timeintervals=0;
				}

				if (timeintervalsCount != timeintervals)
				{
					ostringstream os;
					os << "Time intevals count mismatch [count=" << timeintervalsCount << ", size=" << timeintervals << "], task [" << t.getName() << "]";
					OcadaJobOrderException e(os.str());
					ACS_THROW (e) ;
				}
#else
				timeintervals = timeintervalsCount;
#endif // RESOURCE_SET_BUG_FIXED

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
						OcadaJobOrderException e(string("No match in the input list for the filename " + s));
						ACS_THROW (e) ;
					    }
					    idx = iter-readedFileNames.begin();
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
			size_t outputs, outputCount;

			rs.getValue(_outputListCountKey, outputCount);

#ifdef RESOURCE_SET_BUG_FIXED
			try
			{	
			   outputs=rs.getArraySize(_outputListTag);
			}
			catch(rsResourceSet::NotFoundException &)
			{
			    outputs=0;
			}

			if (outputCount != outputs)
			{
				ostringstream os;
				os << "Outputs count mismatch [count=" << outputCount << ", size=" << outputs << "], task [" << t.getName() << "]";
				OcadaJobOrderException e(os.str());
				ACS_THROW (e) ;
			}
#else
			outputs = outputCount;
#endif // RESOURCE_SET_BUG_FIXED

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


				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Readed filename " << s ) ;

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

#ifdef RESOURCE_SET_BUG_FIXED
				try
				{	
				    brks=rs.getArraySize(_brkFileListTag);
				}
				catch(rsResourceSet::NotFoundException &)
				{
				    brks=0;
				}

				if (brkCount != brks)
				{
					ostringstream os;
					os << "Breakpoints count mismatch [count=" << brkCount << ", size=" << brks << "], task [" << t.getName() << "]";
					OcadaJobOrderException e(os.str());
					ACS_THROW (e) ;
				}
#else
				brks = brkCount;
#endif // RESOURCE_SET_BUG_FIXED

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
    catch (const exException& toCatch)
    {
	OcadaJobOrderException e(toCatch, "Error while reading job order") ;
	ACS_THROW (e) ;
    }
    catch (...)
    {
	OcadaJobOrderException e(string("Unexpected exception while reading job order")) ;
	ACS_THROW (e) ;
    }
}


const string mission = "CS";
const string version = "0001";
string
OcadaJobOrder::buildTempFilename(const string& Filetype, const char *ext) const
{
    string fileclass(isTest() ? "TEST" : "OPER" );

    DateTime s1, s2;
    getTimeInterval(s1, s2);

    ostringstream name;
    name << mission   << "_" <<
	    fileclass << "_" << 
	    Filetype  << "_" <<
	    DateTime2FileNamePart(s1) << "_" <<
	    DateTime2FileNamePart(s2) << "_" <<
	    version <<
	    (ext ? ext : "");

    return name.str();
}

_ACS_END_NAMESPACE

