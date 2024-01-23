/*
	Copyright 1995-2021, Exprivia SpA - DADF-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libLTAProductExtractor$

	$Id$

	$Author$
*/
#include <ConfigurationSingleton.h>
#include <LTAProductExtractor.h>
#include <rsResourceSet.h>
#include <RegExp.h>
#include <Filterables.h>
#include <vector>
#include <map>
#include <File.h>
#include <Dir.h>
#include <iostream>
#include <ostream>
#include <StringUtils.h>
#include <SpawnProc.h>
#include <LTAUtilities.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

_ACS_BEGIN_NAMESPACE(acs)
//using namespace std;
using std::string;
using std::ostringstream;
using std::vector;
using std::map;

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAProductExtractor)

LTAProductExtractor::LTAProductExtractor(string const & productName, const std::vector <Attribute> & attributes, const vector<string> &extractionRule) :
			_productName(productName),
			_metadataFiles()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	//PRQA S 3803
	init(attributes, extractionRule);
}

LTAProductExtractor::LTAProductExtractor(LTAProductExtractor const& e) :
			_productName(e._productName),
			_metadataFiles(e._metadataFiles)
{
	//ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	//PRQA S 3803
}

LTAProductExtractor& LTAProductExtractor::operator=(LTAProductExtractor const& e)
{
	if( this != &e ) {
		_productName = e._productName;
		_metadataFiles = e._metadataFiles;
	}
	return *this ;
}
void LTAProductExtractor::init(const std::vector <Attribute> & attributes, const vector<string> &extractionRule)
{
	map<string, string> metadafile;
	string metadataF("");

	for (size_t t = 0; t < extractionRule.size(); ++t)
	{
		getMetadataFromRule(extractionRule[t], metadataF);
		_metadataFiles.push_back(metadataF);
	}
	
	for (size_t j = 0; j < attributes.size(); ++j)
	{
		if (!attributes[j].metadataFile().empty())
		{
			// insert each metadata file configured into a map in order to have a single occurence. Map value meaningless
			metadafile[attributes[j].metadataFile()]=attributes[j].metadataFile();
		}
	}
	map<string, string>::iterator it = metadafile.begin();
	while(it != metadafile.end())
	{
		_metadataFiles.push_back(it->first);
		it++;
	}
	
ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " number of metadataFiles to extract: " << _metadataFiles.size());
}
void LTAProductExtractor::getMetadataFromRule(const string & rule, string & metadataFile)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	//perform product name substitution
	vector<string> tokens;
	StringUtils::Tokenize(rule, tokens, ",");
	size_t n = tokens.size();
	ACS_COND_THROW(n < 1, LTAProductExtractorException("Unexpected number of parameters describing the extraction rule"));
	string filename = File::removeExt(File::getFileName(_productName));

// for (size_t k=0; k < n; ++k)
// {
// ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " tokens " << tokens[k]);
// }
	size_t t = std::stoi(tokens[0]);
	metadataFile = filename.substr(0, t);
	
	if (n > 1 )
	{
		if ( !metadataFile.empty())
		{
			string tmpS = StringUtils::trim(tokens[1]);
			if (!StringUtils::equals(tmpS, ""))
			{
ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " tokens[1] not empty :" << StringUtils::trim(tokens[1]));		
				metadataFile = File::addExt(metadataFile, StringUtils::trim(tokens[1]));
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Built "<< metadataFile << " tmp metadataFile ");	
			}

			if (4 == n)
			{
				// S2 case
				string newS=StringUtils::trim(tokens[3]);
				metadataFile.replace(std::stoi(tokens[2]), newS.size(), newS);
			}
		}
		else if (metadataFile.empty())
		{
			metadataFile = File::addExt("*", StringUtils::trim(tokens[1]));
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Built "<< metadataFile << " metadataFile name ");

}

void LTAProductExtractor::getMetadataFiles(std::vector<std::string> & metadataFile)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	metadataFile = _metadataFiles;
}
void LTAProductExtractor::setTarArgs(const string & repository, const vector<string> & metadataFile, vector<string> & args)
{
	if (!File::exists(repository))
	{
		File::mkdir(repository);
	}
	args.push_back("xvfz");
	args.push_back(_productName);
	args.push_back("-C");
	args.push_back(repository);
	args.push_back("--xform=s#^.+/##x");
	for (size_t k = 0; k < metadataFile.size(); ++k)
	{
	//std::cout << "metadataFile[k]: " << metadataFile[k]<< std::endl;
		args.push_back( metadataFile[k] );
	}
}
void LTAProductExtractor::setArgs(const string & repository, const vector<string> & metadataFile, vector<string> & args)
{
	args.push_back("e");
	args.push_back(_productName);
	args.push_back("-bd");
	args.push_back("-aoa");
	std::ostringstream os;
	os << "-o" << repository;
	args.push_back(os.str());
	for (size_t k = 0; k < metadataFile.size(); ++k)
	{
	//std::cout << "metadataFile[k]: " << metadataFile[k]<< std::endl;
		args.push_back(metadataFile[k]);
	}
	args.push_back("-r");
}
void LTAProductExtractor::setArgs(vector<string> & args)
{
	args.push_back("-X");
	args.push_back(_productName);

}
void LTAProductExtractor::extract(const string & repository, const vector<string> & metadataFile, const ExtractionStrategy & extractionStrategy)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	string command("7z");
	string outDesc("/dev/null");
	
	vector<string> args;
	switch (extractionStrategy)
	{
		case UNCOMPRESS:
		{
			command = "tar";
			setTarArgs(repository, metadataFile, args);
			break;
		}
		case JP2:
		{
			command="exiftool";
			setArgs(args);
			outDesc = StringUtils::pathJoin(repository, "Inventory_Metadata.xml");
			break;
		}	
		case NONE:
		{
			ostringstream exMsg;
			exMsg << " Extraction Strategy not expected";
			ACS_THROW(exSystemException(exMsg.str()));	// it should not occur
			break;
		}
		default:
		{
			setArgs(repository, metadataFile, args);
			break;
		}
	}
	
	SpawnProc proc(command) ;
	
	for (size_t t = 0; t < args.size(); ++t)
	{
		proc.args()[t] = args[t];
	}

	int fd = open(outDesc.c_str(), O_RDWR | O_CREAT, S_IRWXU);
	int fdErr = open("/dev/null", O_RDWR);

	proc.setOutDescriptor (fd);
	proc.setErrDescriptor (fdErr);
	
	proc.spawn() ;
	close (fd);
	close (fdErr);	
	while (proc.isRunning()) {
		proc.waitExitOrTimeout(1000); // PRQA S 4400 // 1 sec
	}

	int exitCode = 0 ;
	ACS_COND_THROW( not proc.exitCode(exitCode),  exSystemException('\"'+_productName+ " " + command + "\" not terminated") );
	if( exitCode != 0 ) {
		ostringstream exMsg;
		exMsg << '\"' << _productName << " " << command << "\" failed";

		int exitSignal = 0 ;
		if( proc.exitSignal(exitSignal) ) {
			exMsg << " terminated with signal: " << exitSignal ;
		}
		exMsg << " exit code " << exitCode;
		ACS_THROW(exSystemException(exMsg.str()));
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _productName << ": " << command << " successfully executed in \"" << repository << "\" directory");

}	

_ACS_END_NAMESPACE
