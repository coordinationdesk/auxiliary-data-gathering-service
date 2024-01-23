// PRQA S 1050 EOF
/*

    Copyright 2020, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    SRV Attribute Extractor main

*/ 

#include <SRVAttributeExtractor.h>
#include <ApplicationLock.h>
#include <SRVExtractorV.h>
#include <DefaultJobOrder.h>
#include <LTAProductExtractor.h>
#include <LTAJsonProductAttributes.h>
#include <Dir.h>
#include <StringUtils.h>
#include <RegExp.h>
#include <rsPushNSpace.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace 
{ 
	// version management
	SRVExtractorV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVAttributeExtractor) 
const std::string SRVAttributeExtractor::_defaultExtension = "ATTRIBUTES";

namespace { // unnamed
	PROJECT_STANDALONE_APP::GenericBuilder<SRVAttributeExtractor> theApplicationBuilder("SRVAttributeExtractor", "SRVQ");
}


SRVAttributeExtractor::SRVAttributeExtractor(const string& appname, const string& appsubs) :
	PROJECT_STANDALONE_APP (appname, appsubs), // PRQA S 1021
	_extension(_defaultExtension),
	_cleanTempDir(false),
	_optionalAttributes(false)
{
}

SRVAttributeExtractor::~SRVAttributeExtractor() throw() 
{
}

/* void SRVAttributeExtractor::customSigIntHandler()
{
	ACS_LOG_INFO ("SIGINT handler called in user space");
}

void SRVAttributeExtractor::customSigTermHandler()
{
	ACS_LOG_INFO ("SIGTERM handler called in user space");
}
 */
int SRVAttributeExtractor::usage(const std::string &) const
{
	excerr << "Use: \n" ;
	excerr << "\t" << applicationName() << " JobOrderFileName \n" ;
    
// 	excerr << "\n\tApplication options:\n" ;
// 	excerr << "\t__________________________________\n" ;

    return 1 ;
}


int SRVAttributeExtractor::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{

	// Is it OK, until now
	Application::setStatus (Application::Ok);


	//******************** Load Configuration and rotate log **************
	bool confOk = loadConf();
	Application::rotateLog();

	if ( ! confOk )
	{
		ACS_LOG_ERROR ("Unable to load the configuration. Exiting.");
  		Application::setStatus (Application::Critical, "Unable to start, configuration not available.");
  		return 1;
	}
        if (argc < 2) {
                usage(File::getFileName(Application::executableName()));
                Application::setStatus(Application::Exiting);
                return 1;
        }

	try 
	{
        	string joFileName(argv[1]);
		execute(joFileName) ; 
	}
	catch(exception &e)
	{
		ACS_LOG_ERROR("Exiting due to following exception: " << e) ;
  		Application::setStatus (Application::Critical, "Error due to exception");
		return 1 ;
	}

	Application::setStatus (Application::Exiting);
	ACS_LOG_PRIVINFO("Processing successfully executed." );
	return 0;
}



bool SRVAttributeExtractor::loadConf() // PRQA S 4020
{
	try 
	{
		addSpace(ConfigurationSingleton::ConfSpace("SRVJsonProductAttributes"), ConfigurationSingleton::NoLoad) ; // PRQA S 3081 
		loadConfiguration(false) ;
	}
	catch(exception &e){
		// In case of exceptions return false
		ACS_LOG_NOTIFY_EX(e) ;
		return false ;
	}
	loadConfFiles();

	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	rsPushNSpace ns(rs, "SRVJsonProductAttributes");
	if (rs.hasKey("OutputExt")) 
	{
		rs.getValue("OutputExt", _extension);
	}
	else
	{
		ACS_LOG_WARNING("Using default value for OutputExt: " << _extension) ;
	}
	if (rs.hasKey("CleanTempDir")) 
	{
		rs.getValue("CleanTempDir", _cleanTempDir);
	}
	else
	{
		ACS_LOG_WARNING("Using default value for CleanTempDir: " << _cleanTempDir) ;
	}
	if (rs.hasKey("OptionalAttributes")) 
	{
		rs.getValue("OptionalAttributes", _optionalAttributes);
	}
	else
	{
		ACS_LOG_WARNING("Using default value for OptionalAttributes: " << _optionalAttributes) ;
	}

	return true ;
}
string SRVAttributeExtractor::getDefaultLogName() const
{
	return "./"+applicationName()+".log";
}
void SRVAttributeExtractor::loadConfFiles()
{
	rsResourceSet rset = ConfigurationSingleton::instance()->get();

	vector<string> confFile;
	rsPushNSpace ns(rset, "SRVJsonProductAttributes");
	rset.getArray("List_Of_Conf_Spaces.Conf_Space", confFile);

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Conf_Space found: "<< confFile.size());
	for (size_t i = 0; i < confFile.size(); ++i)
	{
		ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace(confFile[i]), ConfigurationSingleton::NoLoad) ; // PRQA S 3081
	}
	loadConfiguration(false) ;
}
 
 void SRVAttributeExtractor::execute(const string & jobOrderFileName)
{
	// Retrieve input list from the jobOrder
	
	ACS_SMARTPTR<JobOrder> jo(new DefaultJobOrder);

	try
	{
		XMLIstream is (jobOrderFileName);
		is >> *jo;
	}
	catch (exception& e)
	{
		ACS_THROW(SRVAttributeExtractorException (e, "Unable to load JobOrder from file  \"" + jobOrderFileName + "\"" ));
	}

	vector<InputDescriptor> inputDescr;
	Task task = jo->getTask( "INPUT_TASK");
	task.getInputList( inputDescr);	
	
	for( vector<InputDescriptor>::iterator it=inputDescr.begin(); it!=inputDescr.end(); ++it )  // PRQA S 4238
	{
		vector<TaskTableVisitable*> vTTVisitable = it->const_getElements();

        	InputAlternative* theAlternative = 0;
		vector<string> thePathNames(0);

		if( 0== vTTVisitable.size())
		{
			ostringstream os;
			os << "No input file found in the jobOrder "<< jobOrderFileName;
			ACS_THROW(SRVAttributeExtractorException (os.str()));
		}
		else if (vTTVisitable.size() > 1)
		{
			ACS_LOG_WARNING("Unexpected number of input filetypes found in the jobOrder " << jobOrderFileName << ". Processing only the first.");
		}

		theAlternative = dynamic_cast<InputAlternative*>( vTTVisitable[0]); // PRQA S 3081
        	theAlternative->getPathNames( thePathNames);

		if( thePathNames.size() != 1)
		{
			ACS_LOG_WARNING("Unexpected number of input file found. Only the first one shall be processed.");
		}

		if( thePathNames[0] == "")
		{
			// empty filename
	        	string theFileType = theAlternative->getFileType();
			SRVAttributeExtractorException e( "File_Name is EMPTY for fileType \"" + theFileType + "\" in the joborder \"" + jobOrderFileName + "\"");
            		ACS_THROW( e);
		}

		ACS_LOG_PRIVINFO("Extracting attributes for file " << thePathNames[0]);

		// check input existence
		if (!File::exists(thePathNames[0]))
		{
			SRVAttributeExtractorException e("File "+ thePathNames[0] + " does not exist!");
            		ACS_THROW( e);
		}
		string satellite;
		string extension;
		string sectionName;
		vector<string> extractionRule;
		// get attribute mapping from configuration
		std::vector <Attribute> attributes;
		bool createEmpty(false);
		LTAExtractedConf conf;
		try
		{
			LTAUtilities::loadConf(thePathNames[0], conf);
			LTAUtilities::getConfAttributes(conf.satellite(), conf.sectionName(), attributes);
		}
		catch (LTAUtilities::LTAUtilitiesException & e)
		{
			ACS_LOG_NOTIFY_EX(e) ;
			ostringstream os;
			os << "No attribute configuration found for " << thePathNames[0]; //<< ". Empty json file shall be created.";
			ACS_LOG_WARNING(os.str());
			createEmpty = true;
		}
		catch (exception & e)
		{
			ACS_LOG_NOTIFY_EX(e) ;
			ostringstream os;
			os << "Error in the attribute retrieval from " << thePathNames[0];
			ACS_THROW(SRVAttributeExtractorException(os.str()));
			createEmpty = true;
		}
		std::vector <AttributeFromFilename> attrFromFilename;
		try
		{
			// get attribute mapping from configuration for fields from product filename
			LTAUtilities::getConfAttributes(conf.satellite(), conf.sectionName(), attrFromFilename);
			createEmpty = false;
		}
		catch (const LTAUtilities::LTAUtilitiesException & e)
		{
			if (createEmpty)
			{
				ACS_LOG_NOTIFY_EX(e) ;
				ostringstream os;
				os << "No attribute configuration from filename found for " << thePathNames[0]<< ". Empty json file shall be created.";
				ACS_LOG_WARNING(os.str());
			}
		}
		
		std::vector<json> json_attr;
		if (!createEmpty) // attribute and/or attribute from filename found
		{
			// create a temporary dir 
			FileGuard fg = File::tempdir("TMP_DIR"); 
			string tmpDir = fg;

			if (!_cleanTempDir)
			{
				fg.detach();
			}
			if (!attributes.empty()) // attribute configuration found
			{
				//ACS_LOG_INFO("Strategy name " << conf.extractionStrategy());
				if (conf.extractionStrategy() != ExtractionStrategy::NONE)
				{
					LTAProductExtractor pe(thePathNames[0], attributes, conf.extractionRule());
					// get the list of metadata file used to extract the attributes
					std::vector<std::string> metadataFiles;
					pe.getMetadataFiles(metadataFiles);
					// extract metadatafiles
					try {
						pe.extract(tmpDir, metadataFiles, conf.extractionStrategy());
					}
					catch (exception & e)
					{
						if (!_optionalAttributes) // conf flag setting indicates that metadata files are mandatory
						{
							ACS_LOG_NOTIFY_EX(e) ;
							ostringstream os;
							os << "Error uncompressing input file " << thePathNames[0] ;
							ACS_THROW(SRVAttributeExtractorException(os.str()));
						}
					}
				}
				else
				{
					// create soft link
					File::link(File::getAbsolutePath(thePathNames[0]), StringUtils::pathJoin(tmpDir, File::getFileName(thePathNames[0])), false);
				}
			}
			else // only attribute from filename found
			{
					// create soft link
					File::link(File::getAbsolutePath(thePathNames[0]), StringUtils::pathJoin(tmpDir, File::getFileName(thePathNames[0])), false);
			}
			Dir dir(tmpDir);

			try
			{
				LTAJsonProductAttributes attr(tmpDir, File::getFileName(thePathNames[0]));
				attr.setAllowOptional(_optionalAttributes);

				attr.setConfAttributes(attributes);
				attr.setConfAttributesFromFilename(attrFromFilename);
				json_attr = attr.readAttributes();
				if (json_attr.empty())
				{
					ostringstream os;
					os << "No attributes loaded associated to section name " << sectionName;
					ACS_THROW(SRVAttributeExtractorException(os.str()));
				}
			}
			catch (LTAUtilities::LTAUtilitiesException & e)
			{
				ACS_LOG_NOTIFY_EX(e) ;
				ostringstream os;
				os << "No attribute configuration found for " << thePathNames[0]<< " Empty json file shall be created.";
				ACS_LOG_WARNING(os.str());
			}
			catch (SRVAttributeExtractorException & e)
			{
				ACS_LOG_NOTIFY_EX(e) ;
				ostringstream os;
				os << "Error in the attribute retrieval from " << thePathNames[0] << " Empty json file shall be created.";
				ACS_LOG_WARNING(os.str());
			}
			catch (exception & e)
			{
				ACS_LOG_NOTIFY_EX(e) ;
				ostringstream os;
				os << "Error in the attribute retrieval from " << thePathNames[0];
				ACS_THROW(SRVAttributeExtractorException(os.str()));
			}
		}
		std::string jsonFile(File::addExt(thePathNames[0], _extension));
		try
		{
			ACS_LOG_PRIVINFO("Writing attributes on file " << jsonFile);
			std::ofstream o(jsonFile);
			o << std::setw(4) << json_attr << std::endl;
		}
		catch (exception & e)
		{
			ACS_LOG_NOTIFY_EX(e) ;
			ostringstream os;
			os << "Error writing output file " << jsonFile;
			ACS_THROW(SRVAttributeExtractorException(os.str()));
		}
	} //new input filetype
}

_ACS_END_NAMESPACE
