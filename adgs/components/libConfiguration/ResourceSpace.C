// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2017/09/25 15:32:41  lucio.pulvirenti
	S2PDGS-1843: absolute path optional input parm passed to CTOR to set private attribute. get and set methods added.
	
	Revision 5.4  2017/05/02 13:31:08  lucio.pulvirenti
	S2PDGS-1754: incrementVersion static method added.
	
	Revision 5.3  2017/04/27 10:16:02  lucio.pulvirenti
	S2PDGS-1754: get and update new IFs defined to be implemented in derived classes only (not implemented in base class so far).
	
	Revision 5.2  2017/04/26 15:43:44  lucio.pulvirenti
	S2PDGS-1754: WORK IN PROGRESS
	
	Revision 5.1  2016/05/10 21:43:44  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.0  2013/06/18 10:14:18  marpas
	adoption of libException 5.x standards
	
	Revision 2.5  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.4  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.3  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.6  2004/02/03 10:40:37  marpas
	Debug improved
	Check of existence of conf file from ResourceSpace enforced
	
	Revision 1.5  2003/07/11 13:00:00  marpas
	exException::clone implemented
	
	Revision 1.4  2003/04/30 16:31:09  marpas
	using namespace std no longer into acs includes
	
	Revision 1.3  2003/02/03 18:52:00  marpas
	Exception handling improved to distinguish parse errors
	
	Revision 1.2  2003/01/31 14:46:21  marpas
	bug fixed and LIB_INTEGRATION_PATH variable management
	
	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
	libConfiguration import
		

*/


#include <ResourceSpace.h>
#include <ResourceSpaceFactory.h>
#include <rsResourceSet.h>
#include <Filterables.h>
#include <Dir.h>
#include <File.h>
#include <fstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ResourceSpace)

// constructor
ResourceSpace::ResourceSpace(const string &space, const string &version, bool absolutePath) :
    _replacement(),
	_space(space),
	_version(version),
	_absolutePath(absolutePath)
{
}


ResourceSpace::ResourceSpace(const ResourceSpace &o) :
    _replacement(o._replacement),
	_space(o._space),
	_version(o._version),
	_absolutePath(o._absolutePath)
{
}

ResourceSpace &ResourceSpace:: operator=(const ResourceSpace &o) 
{
	if (this != &o) {
        _replacement = o._replacement ;
		_space = o._space ;
		_version = o._version ;
		_absolutePath = o._absolutePath;
	}
	return *this ;
}


ResourceSpace::~ResourceSpace() throw() {}

string ResourceSpace::configurationPath(bool std_path) const // PRQA S 4020
{
    if (std_path) {
		return ResourceSpaceFactory::commonConfigurationPath() ;
    }
    else {
		return ResourceSpaceFactory::optionalConfigurationPath() ; 
    }
}


string ResourceSpace::path(bool std_path) const 
{
	string path = space() ;
	if( not _absolutePath ) {
		path = configurationPath(std_path) + 
	       "/" +
	       buildPathWithoutExtension(space(),version()) + "." + defaultExtension() ;
	}
	return path;
}

string ResourceSpace::buildPathWithoutExtension(const string &s, const string &v) 
{
	return s + ( v != "" ? "_" + v : "") ; // PRQA S 3383, 3385
}


string ResourceSpace::defaultExtension() const
{
	return "conf" ;
}


bool ResourceSpace::mergeIn(rsResourceSet &rs, bool std_path)
{
	_replacement = "" ;
	rsResourceSet myrs ;
	
    const string filepath=path(std_path) ;
	try {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking existence of: " << filepath) ;
		if (!File::exists(filepath)) { // does not exists
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, filepath << " does not exists") ;
			ACS_THROW(exFileOpenException("Unexistent file " + filepath ,errno)) ; // PRQA S 3081
		}
	
		ifstream file(filepath.c_str()) ; // open ... 
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading conf: " << filepath) ;

		if (file.bad()) { // ... check ...
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cannot open conf: " << filepath) ;
			ACS_THROW (exFileOpenException("Cannot open " + filepath ,errno)) ; // PRQA S 3081
		}

		try {
			file >> myrs ; // load
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "conf: " << filepath << " loaded") ;
		}
		catch (exception &e) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cannot load conf: " << filepath) ;
			ACS_THROW(SpaceParseException(e,"Cannot load/parse path:\"" + filepath +"\"")) ;
		}
	}
	catch (exFileOpenException &x) { // was not opened
		// absolute path ?
		ACS_COND_THROW(_absolutePath, SpaceLoadException(x,"Cannot load space " + space())) ;
		
        // version requested ?
		ACS_COND_THROW(version() != "",SpaceLoadException(x,"Cannot load space " + space() + " version " + version())) ;
		
		// try to recover loading the space with the greatest version
		
		// here we count on no version passed as for the previous if 
		string radix = buildPathWithoutExtension(space(),"") + "_";
		
		string candidate ;
		double version = 0 ;
		
		Dir dir(configurationPath(std_path)) ;
        
		for (unsigned long i =0; i < dir.size(); i++) {
		
			if (File::getExt(dir[i]) != defaultExtension()) { // wrong extension
				continue ;
            }
				
			string::size_type start = dir[i].find(radix) ;
			
			if (start == 0) { // the file could be a candidate
				double cver = atof(dir[i].c_str()+radix.length()) ; // PRQA S 3084
				if (cver > version) {
					version = cver ;
					candidate = dir[i] ;					
				}
			}
		}
        // no candidate throw an exception - for optional conf dir keep silent
		ACS_COND_THROW(candidate.empty() && std_path, SpaceLoadException(x,"Cannot load space " + space())) ;
        if ( not candidate.empty() ) {
		    ifstream file((configurationPath(std_path)+"/"+candidate).c_str()) ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading conf: " << (configurationPath(std_path)+"/"+candidate)) ;
		    ACS_COND_THROW(file.bad(),exFileOpenException("Cannot open " + filepath ,errno)) ; // PRQA S 3081

		    file >> myrs ;
            if (std_path) { // no reps for opt conf path
    		    _replacement = candidate ;
	        }
        }
    }
	
    if (not myrs.empty()) {
	    rsResourceSet::MergeMode oldmm = rs.mergeMode() ; // save old merge mode on output rs
	    rs.mergeMode(rsResourceSet::ModeUnion) ;    // set it to last override existing ones
	    rs.merge(myrs) ;                            // merge 
	    rs.mergeMode(oldmm) ;                       // reset merge mode
        return true ;
    }

    return false ;
}

void ResourceSpace::get(std::string &, bool)
{
	ACS_THROW(SpaceException("Method not implemented!"));
}

void ResourceSpace::update(const std::string &, bool, bool)
{
	ACS_THROW(SpaceException("Method not implemented!"));
}

const string &ResourceSpace::space() const throw() // PRQA S 4120
{
	return _space ;
}

const string &ResourceSpace::version() const throw()  // PRQA S 4120
{
	return _version ;
}

const bool &ResourceSpace::absolutePath() const throw()  // PRQA S 4120
{
	return _absolutePath ;
}

void ResourceSpace::space(const string &v) // PRQA S 4121
{
	_space = v ;
}

void ResourceSpace::version(const string &v)  // PRQA S 4121
{
	_version = v ;
}

void ResourceSpace::absolutePath(const bool &v)  // PRQA S 4121
{
	_absolutePath = v ;
}

const string &ResourceSpace::replacement() const   // PRQA S 4120
{
	return _replacement ;
}

void ResourceSpace::replacement(const string &r)    // PRQA S 4121
{
	_replacement = r ;
}
	

void ResourceSpace::incrementVersion( const string & oldVers, string & newVers )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	size_t found = oldVers.find('.');
	ACS_COND_THROW( found == string::npos, SpaceLoadException("Unknown version format, \".\" is missing: \""+oldVers+'\"') );
	ACS_COND_THROW( (found + 1) == oldVers.length(), SpaceLoadException("Unknown version format, no character after dot: \""+oldVers+'\"') );
	
	string oldMajorVersionStr = oldVers.substr(0, found);
	string oldMinorVersionStr = oldVers.substr(found+1);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "major version=\"" << oldMajorVersionStr << "\", minor version=\"" << oldMinorVersionStr << '\"');
	
	size_t minorVersionLen = oldMinorVersionStr.length();
	size_t majorVersionLen = oldMajorVersionStr.length();
	int minorVersion;
	{
		istringstream iss(oldMinorVersionStr);
		iss >> minorVersion;
	}
	
	bool upgradeMajorVersion = false;
	{
		ostringstream os; 
    	os << ++minorVersion;
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Incremented minor version=\"" << os.str() << '\"' );
		if( os.str().length() > minorVersionLen ) {
			minorVersion = 0;
			upgradeMajorVersion = true;
		}
	}
	
	string minorVersionString;
    {
		ostringstream os; 
    	os << setw(minorVersionLen) << setfill('0') << minorVersion;
		minorVersionString = os.str();
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "New minor version=\"" << minorVersionString << '\"' );
	
	string majorVersionString = oldMajorVersionStr;
	if( upgradeMajorVersion ) {
		int majorVersion ;
		{		
			istringstream iss(majorVersionString);
			iss >> majorVersion;
			
			ostringstream os;
			os << ++majorVersion;
			if( os.str().length() > majorVersionLen ) {
#ifdef FORCE_VERSION_INCREMENT
				ACS_LOG_WARNING("Major version field increased by increment: \"" << oldMajorVersionStr << "\" -> \"" << os.str() << '\"');
#else
				ACS_THROW( SpaceLoadException("Cannot increment major version from \""+oldMajorVersionStr+"\" to \""+os.str()+"\": field too long!") );
#endif
			}
		}
		
    	{
			ostringstream os; 
    		os << setw(majorVersionLen) << setfill('0') << majorVersion;
			majorVersionString = os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "New major version=\"" << majorVersionString << '\"' );
	
	}
	
	newVers = majorVersionString + '.' + minorVersionString;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "old version=\"" << oldVers << "\", new version=\"" << newVers << '\"');


}
_ACS_END_NAMESPACE
