// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Configuration Library $

*/


#include <StandaloneResourceSpace.h>
#include <ResourceSpaceFactory.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <Filterables.h>

#include <Dir.h>
#include <File.h>
#include <fstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(StandaloneResourceSpace)

// constructor
StandaloneResourceSpace::StandaloneResourceSpace(const string &space, const string &version, bool absPath) :
	ResourceSpace(space,version, absPath)
{
}


StandaloneResourceSpace::StandaloneResourceSpace(const StandaloneResourceSpace &o) : 
    ResourceSpace(o)
{
}

StandaloneResourceSpace &StandaloneResourceSpace:: operator=(const StandaloneResourceSpace &o) 
{
	if (this != &o) {
		ResourceSpace::operator=(o) ;
	}
	return *this ;
}


StandaloneResourceSpace::~StandaloneResourceSpace() ACS_NOEXCEPT {}
string StandaloneResourceSpace::path(bool std_path) const 
{
	string p = space();
	if( not absolutePath() ) {
		p = path(version(), std_path);
	}
	return p;
}

string StandaloneResourceSpace::path(const std::string & version, bool std_path) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path= \"" 
			   << (configurationPath(std_path) + 
		   		   "/" +
		   		   buildPathWithoutExtension(space(),version) + 
				   "." + defaultExtension())) ;
	// build the path as 
	// <common|opt>/<name_version>.<defaultExtension>
	// see base or derived class for implementation
	return configurationPath(std_path) + 
		   "/" +
		   buildPathWithoutExtension(space(),version) + "." + defaultExtension() ;
}

void StandaloneResourceSpace::fillResources(const string &path, rsResourceSet &myrs)
{
    if (!File::exists(path)) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path: " << path << " does not exists") ;
        ACS_THROW(StandaloneSpaceNotFoundException("file: " + path + " does not exists")) ; // PRQA S 3081
    }
    rsResourceSet::MergeMode oldmode = myrs.mergeMode() ;
	try {
		// the conf space is an XML
		XMLIstream  file(path);			
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading xml: " << path) ;

        // union mode - see rsResourceSet 
 		myrs.mergeMode(rsResourceSet::ModeUnion) ;
 		// and then load the content from the file
		file >> myrs ;
        // reset mode 
        myrs.mergeMode(oldmode) ;
	}
	catch (exception &e) {
        // reset mode 
        myrs.mergeMode(oldmode) ;
		ACS_THROW(SpaceParseException(e,"Cannot load/parse path:\"" + path +"\"")) ; // PRQA S 3081
	}
}


bool StandaloneResourceSpace::mergeIn(rsResourceSet &rs, bool std_path)
{
	replacement("") ;
	rsResourceSet myrs ;
	
	const string filepath=path(std_path) ;
	
	try
	{
		fillResources(filepath, myrs) ;
	}
	catch (StandaloneResourceSpaceException &x) {
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_NOTIFY_EX(x) ;
        ACS_CLASS_END_DEBUG ;
		throw ;
	}
	catch (SpaceParseException &x) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "got: SpaceParseException") ;
		// specific parse exception just rethrow it
		throw ;
	}
	catch(exception &x) {
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_NOTIFY_EX(x) ;
        ACS_CLASS_END_DEBUG ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading dbConfigurationSpace: " << space() << " versions: " << version()) ;
		// check if the file exists anyway
		// version requested ?
		if (version() == "" && not absolutePath() ) {
			// try to recover loading the space with the greatest version

			string filetl = getTheGreatestVersionResourcePath(std_path);
			fillResources(filetl,myrs) ;

			// store the whole path in the _replacement to signal 
			// the loading of that file instead of the one on the DB
			replacement(filetl) ;

		}
		else { // the path is assumed complete: with version
			if (File::exists(filepath)) {

				fillResources(filepath,myrs) ;

				// store the whole path in the _replacement to signal 
				// the loading of that file instead of the one on the DB
				replacement(filepath) ; 

			}
			else { // the file with the requested version doesn't exists
				StandaloneSpaceNotFoundException ex(x,"Cannot load space " + space() + (version().empty() ? string(" with no version") : string(" version ") + version())) ; // PRQA S 3081, 3380, 3385
				ACS_THROW(ex) ;
			}
		}				
	}
    
    if (not myrs.empty()) {
	    rsResourceSet::MergeMode oldmm = rs.mergeMode() ;

	    rs.mergeMode(rsResourceSet::ModeUnion) ;

	    rs.merge(myrs) ;

	    rs.mergeMode(oldmm) ;
        return true ;
    }

    return false ;
}


void StandaloneResourceSpace::get(string & resSpace, bool std_path)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Going to get \"" << space() << '\"' << ( (version().empty() || absolutePath() ) ? "" : " version="+version()) ); // PRQA S 3370, 3385
	
	string resourcePath = space();
	if( not absolutePath() ) {
		resourcePath = getResourcePath(std_path);
	}
	
	// This exception could occur in strong concurrency only
	ACS_COND_THROW( not File::exists(resourcePath), StandaloneSpaceNotFoundException("Resource space file \"" + resourcePath + "\" does not exist any more!") ); // PRQA S 3081

	std::ifstream ifs( resourcePath.c_str() );
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	resSpace = buffer.str();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "space:\n" << resSpace) ;

}

string StandaloneResourceSpace::defaultExtension() const 
{
	return "xml" ;
}

string StandaloneResourceSpace::getTheGreatestVersionResourcePath(bool std_path) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string radix = buildPathWithoutExtension(space(),"") + "_";
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "radix: " << radix) ;

	string candidate ;
	double version = 0 ;

	Dir dir(configurationPath(std_path)) ;
	for (unsigned long i =0; i < dir.size(); i++) {

		if (File::getExt(dir[i]) != defaultExtension()) { // wrong extension
			continue ;
    	}

		string::size_type start = dir[i].find(radix) ;

		if (start == 0) { // the file could be a candidate
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying: " << dir[i]) ;
			double cver = atof(dir[i].c_str()+radix.length()) ; // PRQA S 3084
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "current version: " << cver) ;
			if (cver > version) {
				version = cver ;
				candidate = dir[i] ;
			}
		}
	}
	
	ACS_COND_THROW(candidate.empty(), StandaloneSpaceNotFoundException("Cannot load space " + space()) ); // PRQA S 3081

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying candidate: " << candidate) ;
	return (configurationPath(std_path)+"/"+candidate) ;
}

string StandaloneResourceSpace::getResourcePath(bool std_path)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string resourcePath = path(std_path);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Resource file path=\"" << resourcePath << '\"' );
	if( not File::exists(resourcePath) ) {
		ACS_COND_THROW( not version().empty(), StandaloneSpaceNotFoundException("Resource space file \"" + resourcePath + "\" does not exist!") ); // PRQA S 3081
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << resourcePath << "\" resource path does not exist, check if occurs with version");
	// try to see if file with version occurs...
		resourcePath = getTheGreatestVersionResourcePath(std_path);
	}
	
	return resourcePath;
}
	
string StandaloneResourceSpace::getUpgradedVersionResourcePath( const string & resourcePath, bool std_path ) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string originalVersion = getResourceVersion(resourcePath);
	if( originalVersion.empty() ) {
		ACS_LOG_WARNING("Version does not occur in \"" << resourcePath << "\" resource path: no upgrade is possible");
		return resourcePath;
	}

	string newVersion;
	incrementVersion(originalVersion, newVersion);
	string newVersionResourcePath = path(newVersion, std_path);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "New version resource path \"" << newVersionResourcePath << '\"') ;
	if( File::exists(newVersionResourcePath) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << newVersionResourcePath << "\" already occurs") ;

		string theGreatestVersionResourcePath = getTheGreatestVersionResourcePath(std_path);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The greatest version resource path is \"" << theGreatestVersionResourcePath << '\"') ;
		newVersionResourcePath = getUpgradedVersionResourcePath(theGreatestVersionResourcePath, std_path);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Original resource path=\"" << resourcePath << "\", upgraded version resource path=\"" << newVersionResourcePath << '\"');
	return newVersionResourcePath;
}

string StandaloneResourceSpace::getResourceVersion( const string & resourcePath ) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string radix = buildPathWithoutExtension(space(),"") + "_";
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "radix: " << radix) ;

	string resourcePathNoExt = File::removeExt(File::getFileName(resourcePath));
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Filename with no ext: \"" << resourcePathNoExt << '\"') ;
	
	string version;
	if( resourcePathNoExt.find(radix) != string::npos ) {
		version = resourcePathNoExt.substr(radix.length());
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << resourcePath << "\" resource path version=\"" << version << '\"'); 
	return version;

}

_ACS_END_NAMESPACE
