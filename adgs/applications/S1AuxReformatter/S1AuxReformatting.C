/*

    Copyright 1995-2021, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

	$Prod: A.C.S. Sentinel1 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$

*/

#include <sys/resource.h>
#include <S1AuxReformatting.h>
#include <S1L2AuxReformatterSingleton.h>
#include <S1L2AuxReformatterFactory.h>

#include <XMLIstream.h>
#include <rsPushNSpace.h>
#include <StringUtils.h> 
#include <ConfigurationSingleton.h>
#include <DateTime.h>
#include <StandaloneResourceSpaceFactory.h>
#include <S1AuxRefV.h>
#include <JobOrder.h>
#include <Task.h>
#include <OcadaJobOrder.h>
#include <RegExp.h>

#include <Filterables.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

//S1AuxReformatting thisApp;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxReformatting)

static S1AuxRefV vers;

Application::GenericBuilder<S1AuxReformatting> theApplicationBuilder("OCADA_S1_L2_AUX_RFMT", "DSD");

/*
 * STL Helper class
 * Merges all the Input Descritor using a strings array
 */
class accumulate_inputs
{
	vector<string>& _theList;

	public:
	accumulate_inputs (vector<string>& v) : _theList(v) {}

	void operator() (const InputDescriptor& i)
	{
		for_each (i.begin<InputAlternative>(), i.end<InputAlternative>(), accumulate_inputs(_theList));
	}

	void operator() (const InputAlternative& a)
	{
		vector <string> thisList;
		a.getPathNames (thisList);
		for_each (thisList.begin(), thisList.end(), accumulate_inputs (_theList));
	}

	void operator() (const string& path)
	{
		string theFileName = File::getFileName (path);
		//theFileName = File::removeExt(theFileName);
		_theList.push_back (theFileName);
	}

};



//Class constructor
S1AuxReformatting::S1AuxReformatting(const string& appName, const string& subSys) :	
#if   defined (HAS_MISSION_INCLUDES)
PROJECT_STANDALONE_APP(appName, subSys), _privateConf()
#else
StandaloneApp(appName, subSys), _privateConf()
#endif
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: " << Id);
} 

//Class destructor
S1AuxReformatting::~S1AuxReformatting() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


static const char *namespaces[]=
{
  "ProductNaming",
  "S1AuxReformatter"
};

void S1AuxReformatting::printUsage(int argc,  char const * const *argv) const
{
	excerr << "usage: " << argv[0] << " <JobOrder file> \n";
}

S1L2AuxReformatterFactory auxReformatterFactory;

int S1AuxReformatting::main(int argc, char const * const * argv, char const * const * env)		
{																								
	if (argc < 2) 
	{ 
		printUsage(argc, argv) ;
		return 1 ;
	}

	//create the log file
	Application::setRotateLogParametersFromConf(false);
	string logFullPath = "./S1AuxReformatting.log" ;
	// Rotate keeping 5 logs, compressed. 
	// The size has no effect as I don't call the rotate during the execution
	LogRotationPolicy lrp(logFullPath, LogRotationPolicy::FirstApplying, defaultLogAge, (1024*1024*1024), 5, true) ;
	setLRPolicy(lrp) ;
	rotateLog() ; // open the first log

	try
	{
		string joFileName(argv[1]);
		  
		execute(joFileName) ;
	}
	catch(exception& e)
	{
		ACS_LOG_ERROR(SimpleDebugSignature<<"FAILED. Exiting with EXIT_CODE " << dec << 0xff);
		ACS_LOG_NOTIFY_EX(e);
		return(0xff);

	} catch (...)
	{
		ACS_LOG_ERROR(SimpleDebugSignature<<"FAILED. Exiting with EXIT_CODE " << dec << 0xff);
		ACS_LOG_ERROR("Unknown exception caught during execution.");
		return(0xff);
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "completed!");
	
	return 0x00;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////
void S1AuxReformatting::loadConfigResources(const string& path)
{
	rsResourceSet rs;

	for (size_t i=0; i < sizeof(namespaces)/sizeof(namespaces[0]); ++i)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Requesting configuration space " << namespaces[i]);
		
		
		XMLIstream xstr(path + "/" +  namespaces[i] + ".xml") ;
		rsResourceSet myrs ;
		xstr >> myrs ;
		rsResourceSet::MergeMode oldmm = rs.mergeMode() ;
		try {
			rs.mergeMode(rsResourceSet::ModeChange) ;
			rs.merge(myrs) ;
			rs.mergeMode(oldmm) ;
		}
		catch(...) {
			rs.mergeMode(oldmm) ;
			throw ;
		}
	}
	 
	ConfigurationSingleton::instance()->set(rs); 
}

void S1AuxReformatting::createInventoryMetadata( const JobOrder& jo, const std::string& outFiletype, const string& safeFilename ) const {

	 XmlDocSptr xmlDocSptr( new XmlDoc );
	 xmlDocSptr->initWithFile( jo.getConfig() );

	 if ( xmlDocSptr->xPath("//GenerateMetdata//@enabled").at(0)->str() == "true" ) {

		ACS_LOG_INFO("Generating Inventory interface files: Metadata and List file");

		//Load Manifest to get metadata
		XmlDocSptr manifestSptr( new XmlDoc );
		manifestSptr->initWithFile( safeFilename + "/manifest.safe" ).setOn( XmlDoc::parSetRootNs );

		//Set needed fields
		string prodName = File::getFileName( safeFilename );
		string generationTime = "UTC="+ manifestSptr->xPath("//s1auxsar:generation/text()").at(0)->str();
		string validity = "UTC="+ manifestSptr->xPath("//s1auxsar:validity/text()").at(0)->str();
		ostringstream size; size<< File::size(safeFilename);


		//Load template
		string mtdTemplate = xmlDocSptr->xPath("//MetadataTemplate/text()").at(0)->str();

		boost::replace_all(mtdTemplate, "__filename__", prodName);
		boost::replace_all(mtdTemplate, "__source__", jo.getProcName());
		boost::replace_all(mtdTemplate, "__sourceSwVersion__", jo.getProcVersion());
		boost::replace_all(mtdTemplate, "__generationTime__", generationTime);
		boost::replace_all(mtdTemplate, "__validityStart__", validity);
		boost::replace_all(mtdTemplate, "__validityStop__", validity);
		boost::replace_all(mtdTemplate, "__validationDate__", generationTime);
		boost::replace_all(mtdTemplate, "__dataSize__", size.str());
		boost::replace_all(mtdTemplate, "__fileType__", outFiletype);


		string mtdFilename = "./"+prodName+".MTD";
		{
			ofstream mtdfile;
			mtdfile.open(mtdFilename.c_str());
			mtdfile << mtdTemplate;
		}
		ACS_LOG_INFO("Generated Inventory Metadata file: "<<mtdFilename);

		string listFilename = "./"+prodName+".LIST";
		{
			ofstream listfile;
			listfile.open(listFilename.c_str());
			listfile << prodName <<endl;
		}
		ACS_LOG_INFO("Generated Inventory List file: "<<listFilename);

	 }



}

void S1AuxReformatting::execute(const string & jobOrderFileName)
{
	time_t stopProcessing= time(0), startProcessing= time(0);
	
	// Retrieve output filetypes list from the jobOrder
	vector<string> outputList;
	vector<string> inputList;
	
	ACS_SMARTPTR<JobOrder> jo(new OcadaJobOrder);

	try
	{
		XMLIstream is (jobOrderFileName);
		is >> *jo ;
	}
	catch (exException& e)
	{
		ostringstream os;
		os <<  "Unable to load JobOrder from file  \"" << jobOrderFileName << "\"";
		throwException (S1AuxReformattingException (e, os.str() ));
	}
	catch (...)
	{
		ostringstream os;
		os <<  "Unknown exception caught while loading JobOrder \"" << jobOrderFileName << "\"";
		throwException (S1AuxReformattingException (os.str() ));
	}

	//read config file
	string configFilename = jo->getConfig();
	XMLIstream  xmlIstream(configFilename);
	xmlIstream >> _privateConf;

	string confDir("");
	_privateConf.getValue("configurationPath", confDir);

	loadConfigResources(confDir);
	getFileList (jobOrderFileName, inputList, outputList);
	
	unsigned int numConfigs(0);
	_privateConf.getValue("List_of_SAFE_Configurations.count.value", numConfigs);
	
	bool found(false);
	string outProductType;
	string safeVersion;
	string confSafe("");
	
	for (unsigned int i=0; i< numConfigs; i++)
	{
		ostringstream confKey; confKey << "List_of_SAFE_Configurations.SAFE_Configuration" << "[" << i << "]";
		
		string type;
		_privateConf.getValue(confKey.str() + ".InputProduct", type);
		
		RegExp re;
		re.setReg(type);

		if (re.match(inputList.at(0))) 
		{
			string path;
			_privateConf.getValue(confKey.str() + ".ConfPath", path);
			_privateConf.getValue(confKey.str() + ".version", safeVersion);

			confSafe = path+"/SafeFormatter.xml";
			found = true;
			break;
		}
	}
	throwConditionedException(!found, exIllegalValueException("No match found from input file name "+inputList.at(0) + " and input name type "));
	
	string key = outputList.at(0) ;

	S1L2AuxReformatter * auxRef = S1L2AuxReformatterSingleton::instance()->newReformatter(key);
	auxRef->init(inputList, jo->getConfig(), outputList.at(0), safeVersion);
	string outFilename = auxRef->createSafePackage(jo->getProcName(), jo->getProcVersion(), confSafe);

	//If enabled by configuration, create the Metadata file and .LIST for Inventory.
	this->createInventoryMetadata( *jo, outputList.at(0), outFilename );
	
	stopProcessing = time(0);
	
	unsigned int minExecution = (stopProcessing - startProcessing)/60;
	ACS_LOG_INFO("S1AuxReformatting executed in "
		<< minExecution
		<< " minutes and "
		<< ((stopProcessing - startProcessing) - minExecution*60)
		<< " seconds\n");
	
	//free mem
	if (auxRef!=NULL)
		delete auxRef;
	auxRef=NULL;	
}

void S1AuxReformatting::getFileList(const string &joFilename, vector<string>& in, vector<string>& out)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_SMARTPTR<JobOrder> jo(new OcadaJobOrder);

	XMLIstream is (joFilename);
	is >> *jo ;
	
	vector<OutputDescriptor> outputs;
	vector<InputDescriptor> inputsDescr;
	// Retrieve output descriptors from the tasks
	for (vector<Task>::iterator it=jo->begin(); it!=jo->end(); ++it)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting output descriptors from task [" << it->getName() << "]");
		it->getOutputList (outputs);
		it->getInputList (inputsDescr);
	}

	vector <string> outputFiletypes;
	// Transform the OutputDescrptor vector in vector of strings.
	// And do not insert duplicated filetypes
	for (vector<OutputDescriptor>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding filetype [" << it->getFileType() << "]");
		if ( find (outputFiletypes.begin(), outputFiletypes.end(), it->getFileType()) != outputFiletypes.end() )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "filetype [" << it->getFileType() << "] exists - not inserted");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "filetype [" << it->getFileType() << "] inserted");
			outputFiletypes.push_back (it->getFileType());
		}
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, outputFiletypes.size() << " unique filetypes found");


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method executed...");

	out = outputFiletypes;

	vector<string> inputsName;
	// Extract the input list from the descriptors
	for_each (inputsDescr.begin(), inputsDescr.end(), accumulate_inputs (inputsName)); 	
	in = inputsName;
}

_ACS_END_NAMESPACE
