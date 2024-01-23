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

	$Prod:  A.C.S. WS Config Builder $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/06/21 13:44:08  marfav
	Warning removed. Aligned to libException V5
	
	Revision 2.3  2012/02/14 15:19:08  marpas
	fixing dtor
	
	Revision 2.2  2010/08/03 14:41:38  nicvac
	Task Table plugins management
	
	Revision 2.1  2010/06/15 11:31:13  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 2.0  2006/02/28 17:57:01  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/07/18 17:14:07  marfav
	Fixed order type strings for OFFL and RPRO in WSConfigElement attributes
	
	Revision 1.2  2005/07/18 17:02:42  marfav
	WSConfigElement interface changed. Using the new version
	
	Revision 1.1.1.1  2005/06/10 13:13:53  fracar
	Import WSConfigBuilder
	

*/


#include <wscbV.h>

#include <WSConfigBuilder.h>

#include <TaskTable.h>
#include <TaskTableFactoryChain.h>

#include <DirPluginSet.h>

#include <XMLIstream.h>

#include <exException.h>
#include <ChDir.h>
#include <Dir.h>

#include <RegExp.h>

#include <Filterables.h>

#include <boost/tokenizer.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iterator>

#include <Functional.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace po = boost::program_options;

using namespace std;

namespace { 
    wscbV version;
}


const char *manual[] =
{
"    Usage:",
"      [--help] {-d <DirName> | -l <ListFile>} [-o logfilename] [-f <WSConfigFile>]",
"",
"    Loads the TaskTables found in the specified directory or listed in the specified file",
"    and generates the corresponding Workstation Configuration File",
"    with default value assigned to the WS Config fields.",
"",
"",
"    --help extended_help",
"	    prints this manual",
"",
"    -o WSConfigFile",
"	    the next argument is the name of the output WS Config file to generate",
"	    if this option is not specified, default filename WSConfig.xml is used",
"",
"    -l ListFile",
"	    the next argument points to a file containing a list of task table files,",
"	    one for each row;",
"	    all legal task table files found in the listfile are loaded and;",
"	    inserted in the generated WS Config file;",
"	    this option is mandatory if no -d option is specified",
"",
"    -d DirName",
"	    the next argument points to a directory containing task table files,",
"	    possibly with other files;",
"	    all legal task table files found in the directory are loaded and;",
"	    inserted in the generated WS Config file;",
"	    this option is mandatory if no -l option is specified",
"",
"    --ttPluginsPath <Path for Task Tables Plugins>",
"	    some TaskTable must be read by specialized plugins.",
"",
"    --ttKeyFile <KeyFile>",
"	    some TaskTable must be read by specialized plugins.",
"	    <KeyFile> is an xml file containing keys for TaskTables.",
"	    Eg.:",
"	    <KeyFile>",
"		    <ListOfTTKey count=\"\">",
"		       <TTKey ttRegExp='TT.Specific.*xml'      key='satellite,mission,procName,procVersion,id'/>",
"		       <TTKey ttRegExp='TT.CS2.*xml'           key='cryosat,2'/>",
"		       <TTKey ttRegExp='TT.CS[[:digit:]].*xml' key='cryosat'/>",
"		       ...",
"		    </ListOfTTKey>",
"	    </KeyFile>",
"	    Key ['','','','',''] is used for TaskTables that don't match any ttRegExp,",
"	    (that is the key returning the base TaskTable).",
};

Application::GenericBuilder<WSConfigBuilder> theApplicationBuilder("WSConfigBuilder", "ThinLayer") ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(WSConfigBuilder)

WSConfigBuilder::WSConfigBuilder(const string &appName, const string &subSystem) :
	BASE_APP(appName, subSystem),
	_allParDef(string("Usage: \n")+appName+" [--help] {-d <DirName> | -l <ListFile>} [-o <WSConfigFile>] [--ttPluginsPath <TTpluginsPath>] [--ttKeyFile <KeyFile>]"),
    _argvParameters(),
	_wscFile(""),
	_taskTableListFile(""),
	_dirName(""),
    _wsConfiguration(),
	_ttPluginsPath(""),
    _keyMap(),
    _pluginLoader()

{
}


WSConfigBuilder::~WSConfigBuilder() throw() 
{
}


int WSConfigBuilder::usage(const string&) const
{
	ostringstream msg; msg<<_allParDef;
    ACS_LOG_INFO (msg.str());
    return 0;
}


void WSConfigBuilder::print_manual()
{
	excout << endl;
	for (size_t i=0; i<sizeof(manual)/sizeof(manual[0]); ++i)
		excout << manual[i] << endl;
	excout << endl;
}

bool WSConfigBuilder::parseArgv(int argc, char const * const * argv, char const * const * env)
{
	//Create the Program Options
	_allParDef.add_options()
		("help,h", 									"extended help")
		("dirName,d",	  po::value< string >(),	"arg: task tables' path" )
		("listFile,l",	  po::value< string >(),	"arg: task table list file" )
		("outWSfile,o",	  po::value< string >(),	"arg: output WS config file" )
		("ttPluginsPath", po::value< string >(),	"arg: <Path for Task Tables' Plugins>")
		("ttKeyFile",	  po::value< string >(),	"arg: <File of Task Tables' Keys>");

	try {

		po::command_line_parser parser(argc, const_cast<char**>(argv)) ;
		parser.options(_allParDef).allow_unregistered();
		po::parsed_options parsed = parser.run();

		po::store(parsed, _argvParameters);
		po::notify(_argvParameters);

	} catch(exception& ex) {
        ACS_LOG_ERROR ("Error Parsing Command line: ("<<ex.what()<<")");
        ostringstream os;
        os << _allParDef;
        ACS_LOG_ERROR (os.str());
		return false;

	}

	return true;

}

void
WSConfigBuilder::loadPlugIns()
{
	try 
	{
        ACS_LOG_INFO ("WSConfigBuilder::loadPlugIns: loading plugins from: "<<_ttPluginsPath);

		vector<string> pluginsPaths(1, _ttPluginsPath);

		DirPluginSet myPluginSet ( pluginsPaths, "\\.so$");
		myPluginSet.load (_pluginLoader);
		if (!_pluginLoader.load()) 
		{
            ACS_LOG_WARNING ("WSConfigBuilder::loadPlugIns: WARNING Plugin loader failed. Some plugins have not been installed.");
		}

	} 
	catch (exception& e) 
	{
		string err("Error while loading plugins");
		exIOException ee(e, err);
		throwException(ee);
	}
}

int WSConfigBuilder::main(int argc, char const * const * argv, char const * const * env)
{

	if ( !(parseArgv(argc, argv, env)) )
	{
		exit(1) ;
	}

	if ( _argvParameters.count("dirName") )
		_dirName = _argvParameters["dirName"].as<string>();

	if ( _argvParameters.count("listFile") )
		_taskTableListFile = _argvParameters["listFile"].as<string>();

	if ( _argvParameters.count("outWSfile") )
		_wscFile = _argvParameters["outWSfile"].as<string>();

	if ( _argvParameters.count("ttPluginsPath") )
		_ttPluginsPath = _argvParameters["ttPluginsPath"].as< string >();

	if ( _argvParameters.count("help") )
	{
		print_manual();
		exit(0) ;
	}

	if ((_dirName == string("")) && (_taskTableListFile == string("")))
	{
		usage("") ;
		exit(1) ;
	}

	if ( (_dirName != "") && (_taskTableListFile != "") )
	{
        ACS_LOG_INFO ("WSConfigBuilder: cannot specify both -d and -l options");
		usage("") ;
		exit(1) ;
	}

	if (_wscFile == string(""))
	{
		_wscFile = "WSConfig.xml";
	}

	try 
	{
		if ( _ttPluginsPath != "" )
			loadPlugIns();
	}
	catch (exception& toCatch)
	{
		exIOException e(toCatch, "Cannot load plugins from "+_ttPluginsPath) ;
		throwException (e) ;
	}


	if ( _argvParameters.count("ttKeyFile") )
	{
		initKeyMap( _argvParameters["ttKeyFile"].as< string >() );
	}


	return execute (argc, argv, env);
}


void 
WSConfigBuilder::initKeyMap(const string& ttKeyFile)
{
	XMLIstream xmlIstream(ttKeyFile);
	rsResourceSet rs; xmlIstream >> rs;

	ostringstream rsBaseKey; rsBaseKey<<"ListOfTTKey.TTKey";
	unsigned short numKeys = rs.getArraySize(rsBaseKey.str()); 
	for ( unsigned short i=0; i<numKeys; ++i )
	{
		ostringstream rsRegKey; rsRegKey<<rsBaseKey.str()<<"["<<i<<"]"<<".ttRegExp.value";
		ostringstream rsTtkKey; rsTtkKey<<rsBaseKey.str()<<"["<<i<<"]"<<".key.value";

		string reg; rs.getValue(rsRegKey.str(), reg);
		string ttk; rs.getValue(rsTtkKey.str(), ttk);

		_keyMap[reg] = ttk;
	}
}

int
WSConfigBuilder::execute (int argc, char const * const * argv, char const * const * env)
{

	vector<string> taskTableFilenames;

	if (_dirName != string(""))
	{
		Dir d(_dirName);
		ChDir chd(_dirName);
		for (size_t i=0; i<d.size(); ++i)
		{
			// prefix the name with the directory path, otherwise opening of files will fail
			taskTableFilenames.push_back(chd.getCwd()+"/"+d[i]);
		}
	}

	if (_taskTableListFile != string(""))
	{
		ifstream ifs(_taskTableListFile.c_str());
		copy(istream_iterator<string>(ifs), istream_iterator<string>(), back_inserter(taskTableFilenames));
	}

	build(taskTableFilenames);


	ofstream of(_wscFile.c_str());
	_wsConfiguration.setLegacy(true);
	of << _wsConfiguration ;

	return 0;
}


//Just say if filename matches with RegExp in the pair.first
struct MatchTT: public acs::func::binary_function< pair<string,string>, string, bool >
{
	bool operator() (const pair<string,string>& keyMapElem, const string& ttFilename ) const
	{
		RegExp re( keyMapElem.first );
		return re.match( File::getFileName(ttFilename) ) ;
	}
};

//Just build the key for the taskTable named ttFilename
struct KeyElementBuilder: public acs::func::binary_function<string, map<string,string>, StringKey>
{
	StringKey operator() (const string& ttFilename, const map<string,string>& keyMap) const
	{

		vector<string> ttKey(5, "");

		//If this taskTable match some Re in keyMap
		map<string, string>::const_iterator keyMapElem = find_if( keyMap.begin(), keyMap.end(), bind2nd(MatchTT(), ttFilename) );
		if ( keyMapElem != keyMap.end() )
		{
			ttKey.clear();

			//Return the found key
			typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;

			Tokenizer tok( keyMapElem->second );
			for(Tokenizer::iterator it=tok.begin(); it!=tok.end();++it) {
				ttKey.push_back( *it );
			}

			//Fill the key with empty strings
			short toIns( 5-ttKey.size() );
			if ( toIns > 0 )
				ttKey.insert( ttKey.end(), toIns, "");
		}

		
		return StringKeyUtils::makeKey( ttKey.at(0), ttKey.at(1), ttKey.at(2), ttKey.at(3), ttKey.at(4) );
	}

};

struct WSConfigElementBuilder
{
	WSConfigElement operator() (const string& filename, const StringKey& key) const
	{
		WSConfigElement e;

		try
		{
			//Get task table
            ACS_LOG_INFO ("WSConfigBuilder: TT:"<<filename<<"; Querying TT Factory with key: "<<key);

			ACS_SMARTPTR<TaskTable> tt( TaskTableFactoryChain::instance()->newObject( key ) );
			tt->readFromFile( filename );
			//*/

			e.name       = tt->getProcName();
			e.version    = tt->getProcVersion();
			e.logLevel   = "INFO";
			e.taskTable  = tt->getPath();
			e.orderTypeStatus["OFFL"] = true;
			e.orderTypeStatus["NRT"] = true;
			e.orderTypeStatus["RPRO"] = true;
			e.orderTypeStatus["Trblshoot"] = true;
			e.orderTypeStatus["SUBS"] = false;
		}
		catch (const exException&)
		{
            ACS_LOG_WARNING ("Filename " << filename << " does not reference a valid task table... skipping!");

			e.name       = "";
			e.version    = "";
			e.logLevel   = "";
			e.taskTable  = "";
		}

		return e;
	}
};


void
WSConfigBuilder::build (const vector<string>& filenames)
{

	//Build a key for each task table
	vector<StringKey> keys;
	transform(filenames.begin(), filenames.end(), back_inserter(keys), bind2nd(KeyElementBuilder(),_keyMap) );

	//Build a WS Entry for each (taskTable,key)
	std::vector<WSConfigElement> wsElements;
	transform(filenames.begin(), filenames.end(), keys.begin(), back_inserter(wsElements), WSConfigElementBuilder() );
	for (size_t i = 0; i < wsElements.size(); ++i)
	{
		_wsConfiguration.push_back(wsElements[i]);
	}

	// now trim away elements in the vector not mapping to valid task tables...
	WSConfigElement empty;

	empty.name       = "";
	empty.version    = "";
	empty.logLevel   = "";
	empty.taskTable  = "";

	_wsConfiguration.erase(remove(_wsConfiguration.begin(), _wsConfiguration.end(), empty), _wsConfiguration.end());
}


_ACS_END_NAMESPACE
