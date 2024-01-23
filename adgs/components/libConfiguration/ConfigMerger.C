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
	Revision 5.4  2017/09/27 11:19:45  lucio.pulvirenti
	S2PDGS-1843: methods added to manage configuration in input filepath and relative attributes added. mergeResourceSpace and
	mergeResourceSet methods added to be used both in conf space case and in filepath one.
	
	Revision 5.3  2016/09/08 15:28:10  marfav
	Added statistics related to requested ad loaded configuration spaces
	
	Revision 5.2  2016/05/10 21:43:44  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.1  2013/07/18 11:21:40  marpas
	space not found exception are different (in their notifications) from other exceptions
	
	Revision 5.0  2013/06/18 10:14:18  marpas
	adoption of libException 5.x standards
	
	Revision 2.10  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.9  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.8  2012/12/01 18:47:09  marpas
	qa rules
	
	Revision 2.7  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.6  2011/09/12 12:49:55  marpas
	filterables fixed: redundant WARNING removed
	
	Revision 2.5  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.4  2008/12/01 13:53:02  marpas
	messages improved in case of errors
	
	Revision 2.3  2008/11/14 18:00:47  marpas
	bug fixed and debug improved
	
	Revision 2.2  2008/11/13 13:47:04  marpas
	StopPredicate introduced
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
		

*/


#include <ConfigMerger.h>
#include <ResourceSpaceFactory.h>
#include <ResourceSpace.h>
#include <rsResourceSet.h>
#include <Filterables.h>

#include <Statistics.h>

#include <cfv.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    // versioning 
    cfv version ;
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigMerger)

// constructor
ConfigMerger::ConfigMerger(const vector<pair<string, string> > &spaces) :
    _spaces(),
    _replacements(),
	_filepaths()
{
	for (size_t i=0; i < spaces.size(); ++i) {
		_spaces.push_back(ConfigurationSingleton::ConfSpace(spaces[i])) ; // PRQA S 3081
	}
}

ConfigMerger::ConfigMerger() :
    _spaces(),
    _replacements(),
	_filepaths()
{
}


ConfigMerger::~ConfigMerger() throw() {}

void ConfigMerger::getResources(rsResourceSet &rs, const ConfigurationSingleton::StopPredicate &stop) 
{

	math::StatNode &statN = (*math::Statistics::instance())()["ConfigurationSpaces"] ; // PRQA S 4412

	_replacements.clear() ;
	rsResourceSet myrs ;
	
    size_t spacessize = _spaces.size() ;
    bool std_path = true ;
	for (size_t i=0; i < spacessize; ++i) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading space: " << _spaces[i].name()) ;
		ResourceSpaceScopeGuard rsp(ResourceSpaceFactory::newItem(_spaces[i].name(),_spaces[i].version())) ;

		mergeResourceSpace(std_path, rsp.get(), _spaces[i].name(), _spaces[i].mandatory(), 
                           stop, statN["std"], myrs);
		
	}
    
    if (ResourceSpaceFactory::hasOptionalConfDir()) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "There is an optional conf dir: " << ResourceSpaceFactory::optionalConfigurationPath()) ;
        bool std_path = false ;

	    for (size_t i=0; i < spacessize; ++i) {
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading optional space: " << _spaces[i].name()) ;
		    ResourceSpaceScopeGuard rsp(ResourceSpaceFactory::newItem(_spaces[i].name(),_spaces[i].version())) ;
            bool mandatory=false ; 
		    mergeResourceSpace(std_path, rsp.get(), _spaces[i].name(), mandatory, 
                               stop, statN["opt"], myrs);

	    }
    }

	mergeResourceSet(myrs, rs, stop);
}


void ConfigMerger::getOverrideResources(rsResourceSet &rs, bool clearReplacements, const ConfigurationSingleton::StopPredicate &stop) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	math::StatNode &statN = (*math::Statistics::instance())()["OverrideConfigurations"] ; // PRQA S 4412

	if( clearReplacements ) {
		_replacements.clear() ;
	}
	rsResourceSet myrs ;

	size_t filepathsSize = _filepaths.size() ;
	for (size_t i=0; i < filepathsSize; ++i) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading filepath: \"" << _filepaths[i]) ;
		ResourceSpace *rsp = ResourceSpaceFactory::newItem(_filepaths[i], "", true) ;
		ResourceSpaceScopeGuard rspGuard(rsp) ;
        bool std_behaviour = true ;
		mergeResourceSpace(std_behaviour, rsp, _filepaths[i], true, stop, statN, myrs);
	}
	
	mergeResourceSet(myrs, rs, stop);

}


void ConfigMerger::addSpace(const ConfigurationSingleton::ConfSpace& s) 
{
	addOrReplaceSpace(s) ;
}

void ConfigMerger::addSpace(const string &s, bool mandatory, const string & v) 
{
	addSpace(ConfigurationSingleton::ConfSpace(s,v,mandatory)) ;
}

void ConfigMerger::addSpace(const string &s, const string&v) 
{
	addSpace(ConfigurationSingleton::ConfSpace(s,v)) ;
}

void ConfigMerger::addSpaces(const vector< ConfigurationSingleton::ConfSpace >& s ) 
{
	for (size_t i=0; i < s.size(); i++) {
		addSpace(s[i]) ;
	}
}

void ConfigMerger::addOrReplaceSpace(const ConfigurationSingleton::ConfSpace &c)  
{
	ConfigurationSingleton::addOrReplaceSpace(c, _spaces) ;
}

void ConfigMerger::clear()
{
	_spaces.clear() ;
}

void ConfigMerger::getReplacements(vector< pair <string, string> > &r) const 
{
	r = _replacements ;
}

const vector<pair<string,string> > &ConfigMerger::getReplacements() const throw() // PRQA S 4120
{
	return _replacements ;
}

void ConfigMerger::addFilepath(const string & fp) 
{
	ConfigurationSingleton::addOrReplaceFilepath(fp, _filepaths) ;
}

void ConfigMerger::addFilepaths(const vector< std::string >& s ) 
{
	size_t fpSize = s.size();
	for (size_t i=0; i < fpSize; i++) {
		addFilepath(s[i]) ;
	}
}

void ConfigMerger::mergeResourceSpace(
    bool std_path, 
    ResourceSpace *rsp, 
    const string &spaceName, 
    bool mandatory, 
    const ConfigurationSingleton::StopPredicate &stop, 
    math::StatNode &statN, 
    rsResourceSet &rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	try {
		ACS_COND_THROW(stop.eval(),ConfigurationSingleton::Stopped("Stopped loading " + spaceName)) ;  // PRQA S 3081


		if (rsp->mergeIn(rs,std_path)) { // something loaded
		   if ("" == rsp->replacement() )
		   {
			   statN[spaceName]["loaded"] += 1.0;
		   }
		   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loaded space: " << spaceName) ;
        }
		ACS_COND_THROW(stop.eval(),ConfigurationSingleton::Stopped("Stopped loading " + spaceName)) ;  // PRQA S 3081

	}
	catch(ResourceSpace::SpaceNotFoundException &x) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "exception caught: " << x) ;
		if (mandatory) {
			statN[spaceName]["critical fail"] += 1.0;
			throw ; 
		} // rethrow
		else {
			ACS_LOG_WARNING("Optional space \"" << spaceName << "\" not found.") ;
		}
        if (std_path) { // optional path are not in the stats 
        	statN[spaceName]["not found"] += 1.0;
		    _replacements.push_back(pair<string, string>(spaceName, "")) ;
        }
	}
	catch(exception &x) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "exception caught: " << x) ;
		if (mandatory) {
			statN[spaceName]["critical fail"] += 1.0;
			throw ; 
		} // rethrow
		else {
			ACS_LOG_WARNING("Loading of space \"" << spaceName << "\" raised: " << x) ;
		}
        statN[spaceName]["with errors"] += 1.0;
        if (std_path) { // optional path are not in the reps 
		    _replacements.push_back(pair<string, string>(spaceName, "")) ;
        }
	}
	// record the replacement if any
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "replacement for " << spaceName <<":\"" << rsp->replacement() << "\"") ;
	if ((rsp->replacement() != "") && std_path) {
		_replacements.push_back(pair<string, string>(spaceName, rsp->replacement())) ;
		statN[spaceName]["replaced"] += 1.0;
    }
}

void ConfigMerger::mergeResourceSet(const rsResourceSet &inRs, rsResourceSet &outRs, const ConfigurationSingleton::StopPredicate &stop)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	rsResourceSet::MergeMode oldmm = outRs.mergeMode() ;
	try {
		outRs.mergeMode(rsResourceSet::ModeUnion) ;

		ACS_COND_THROW(stop.eval(),ConfigurationSingleton::Stopped("Stopped just before merge ")) ;  // PRQA S 3081
		outRs.merge(inRs) ;
	
		outRs.mergeMode(oldmm) ;
	}
	catch(...) {
		outRs.mergeMode(oldmm) ;
		throw ;
	}
}

_ACS_END_NAMESPACE
