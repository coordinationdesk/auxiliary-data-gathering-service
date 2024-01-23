// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$

*/


#include <ConfigurationSingleton.h>
#include <ConfigMerger.h>
#include <StringUtils.h>
#include <HWScan.h>
#include <exStream.h>
#include <ftmpstream.h>
#include <algorithm>
#include <Filterables.h>
#include <RegExp.h>
#include <unistd.h>
#include <cmath>
#include <sys/time.h> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigurationSingleton::StopPredicate)

ConfigurationSingleton::StopPredicate::StopPredicate() :
	_simpleExternConditionGiven(false),
	_simpleExternCondition(_simpleExternConditionGiven) 
{
}

ConfigurationSingleton::StopPredicate::StopPredicate(volatile bool &stopCondition) : 
	_simpleExternConditionGiven(true) ,
	_simpleExternCondition(stopCondition) // PRQA S 2528 
{
}


ConfigurationSingleton::StopPredicate::~StopPredicate() throw() {}

ConfigurationSingleton::StopPredicate::StopPredicate(const StopPredicate &rhs) :
	_simpleExternConditionGiven(rhs._simpleExternConditionGiven) ,
	_simpleExternCondition(rhs._simpleExternCondition) // PRQA S 2528
{
}

bool ConfigurationSingleton::StopPredicate::evalCondition() const throw () 
{
	return false ;
}


bool ConfigurationSingleton::StopPredicate::eval() const throw () // PRQA S 4020, 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_simpleExternConditionGiven: " << boolalpha << _simpleExternConditionGiven
		    << " _simpleExternCondition: " << boolalpha << _simpleExternCondition) ;
	
	if (_simpleExternConditionGiven) {
		return _simpleExternCondition ;
    }
	return evalCondition();
}


unsigned long ConfigurationSingleton::StopPredicate::nanosleep(unsigned long nanosec)
{
	struct timespec req ;    // PRQA S 4102
	req.tv_sec = nanosec / 1000000000U ; // PRQA S 3000, 3084, 4400
	req.tv_nsec = nanosec % 1000000000 ; // PRQA S 3000, 3084, 4400
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "tv_sec: " << req.tv_sec
			   << " tv_nsec: " << req.tv_nsec) ;
	struct timespec rem ;    // PRQA S 4102
	::nanosleep(&req,&rem) ;
	return rem.tv_sec*1000000000+rem.tv_nsec ; // PRQA S 3000, 4400
}

bool ConfigurationSingleton::StopPredicate::timeoutOrStop(size_t msec, size_t *remainder, size_t nsecslice) const // PRQA S 4020, 4214
{
	struct timeval _start ; // PRQA S 4102
	struct timezone	_zone ;	// PRQA S 4102
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "msec: " << msec << " nsecslice: " << nsecslice );
	
	if (msec < nsecslice/1000000) { // PRQA S 3084, 4400 
		nsecslice = msec*1000000 ;  // PRQA S 3084, 4400 
    }
	if (remainder) {
		gettimeofday(&_start,&_zone) ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "msec: " << msec << " nsecslice: " << nsecslice );

	float count = msec/(nsecslice/1000000.0) ;  // PRQA S 3011, 3051, 3012, 3084
	size_t firstnsecslice = size_t(nsecslice*(count-int(count))) ; // PRQA S 3011, 3051, 3081, 3084
	size_t top = size_t(ceil(count)) ;                             // PRQA S 3051, 3081
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "count: " << count
			   << " top: " << top
			   << " firstusec: " << firstnsecslice );
	for (size_t j=top; j > 0 ; j--) {
		if (eval()) { // stop requested ?
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "j: " << j << " cancel requested") ;
			if (remainder) { // compute remainder, note that in that case _start has been initialized above
	            struct timeval _stop ; // PRQA S 4102
				gettimeofday(&_stop,&_zone) ;
				struct timeval lapsed ; // PRQA S 4102
				lapsed.tv_usec = _stop.tv_usec - _start.tv_usec ;   // PRQA S 4201
				lapsed.tv_sec = _stop.tv_sec - _start.tv_sec ;      // PRQA S 4201
				int rem = msec - (lapsed.tv_sec*1000 + lapsed.tv_usec/1000) ; // PRQA S 3000, 3010, 3051, 3084
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "j: " << j << " rem: " << rem ) ;
				*remainder = ((rem >=0) ? rem : 0) ; // PRQA S 3000, 3051, 3380
			}
			return true ; // was cancelled
		}
		if (j == top) { // first step - remember the loop is in the standard opposite way
			nanosleep(firstnsecslice) ;
		}
		else {// all other steps
			nanosleep(nsecslice) ;
        }
		pthread_yield() ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "j: " << j  << " no cancel till now") ;
	}
	
	if (remainder) { *remainder = 0 ; }
	return eval() ;
}
		














ConfigurationSingleton::ConfSpace::ConfSpace(const string &name, const string &version, bool mandatory) :
	_name(name),
	_version(version),
	_mandatory(mandatory)
{
}



ConfigurationSingleton::ConfSpace::ConfSpace(const pair<string, string> & p, bool mandatory) :
	_name(p.first),
	_version(p.second),
	_mandatory(mandatory)
{
}

ConfigurationSingleton::ConfSpace::~ConfSpace() throw() {}

ConfigurationSingleton::ConfSpace::ConfSpace(const ConfSpace &r) :
	_name(r._name),
	_version(r._version),
	_mandatory(r._mandatory)
{
}

ConfigurationSingleton::ConfSpace &ConfigurationSingleton::ConfSpace::operator=(const ConfSpace &r) 
{
	if (this != &r) {
		_name = r._name ;
		_version = r._version ;
		_mandatory = r._mandatory ;
	}
	return *this ;
}

const string &ConfigurationSingleton::ConfSpace::name() const throw() // PRQA S 4120 
{
	return _name ;
}

const string &ConfigurationSingleton::ConfSpace::version() const throw() // PRQA S 4120 
{
	return _version ;
}

bool ConfigurationSingleton::ConfSpace::mandatory() const throw() // PRQA S 4120 
{
	return _mandatory ;
}




ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigurationSingleton)


ConfigurationSingleton::ConfigurationSingleton() : 
    Singleton <ConfigurationSingleton>(),
    _confSet(),
    _privateConfSet(),
    _confSpaces(),
    _callbacks(),
	_hostnameSpaceMandatory(false),
    _replacements(),
	_spacesLoadedOnce(false),
    _staticConfSet(),
	_confFilepaths()
{
}

ConfigurationSingleton::~ConfigurationSingleton() throw() {}


// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning on LoadStealth
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wswitch"

bool ConfigurationSingleton::addSpace(const ConfSpace &c, SpaceLoadMode rmode, const StopPredicate &stop) 
{
	bool added = addOrReplaceSpace(c, _confSpaces) ;
	bool forceLoad =false ;
	bool invokeCallbacks = true ;
	
	// the whole switch could be rewritten with:
	// forceLoad = (mode == LoadImmediate || ((mode == LoadLazy) && added && _spacesLoadedOnce))
	switch(rmode) {
	case LoadLazy:
        {
		    forceLoad = added && _spacesLoadedOnce ; // only if the space was just added
		    break ;
        }
	case LoadImmediate:
        {
		    // force load of the latter space
		    forceLoad = true ;
		    break ;
        }
	case LoadStealth:
        {
		    // force load of the latter space
		    forceLoad = true ;
            invokeCallbacks = false ;
		    break ;
        }
	case NoLoad:
    default:
        {
		    // nothing to do, regardless if the space was added or replaced
		    break ;
	    }
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding space \"" << c.name() << "\" mandatory is " << boolalpha << c.mandatory() << " force load is " << boolalpha << forceLoad) ;
	if (forceLoad) {
        bool old_spacesLoadedOnce = _spacesLoadedOnce ;
		loadSingleSpace(c, stop, invokeCallbacks) ;
        // reset _spacesLoadedOnce to old value
        _spacesLoadedOnce = old_spacesLoadedOnce ;
    }
	return _spacesLoadedOnce ;
}

bool ConfigurationSingleton::addOverrideFilepath(const string &filepath, SpaceLoadMode rmode, const StopPredicate &stop) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	bool added = addOrReplaceFilepath(filepath, _confFilepaths) ;
	bool forceLoad =false ;
	bool invokeCallbacks = true ;
	
	// the whole switch could be rewritten with:
	// forceLoad = (mode == LoadImmediate || ((mode == LoadLazy) && added && _spacesLoadedOnce))
	switch(rmode) {
	case LoadLazy:
        {
		    forceLoad = added && _spacesLoadedOnce ; // only if the space was just added
		    break ;
        }
	case LoadImmediate:
        {
		    // force load of the latter space
		    forceLoad = true ;
		    break ;
        }
	case LoadStealth:
        {
		    // force load of the latter space
		    forceLoad = true ;
            invokeCallbacks = false ;
		    break ;
        }
	case NoLoad:
    default:
        {
		    // nothing to do, regardless if the space was added or replaced
		    break ;
	    }
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding file path \"" << filepath << "\" force load is " << boolalpha << forceLoad) ;
	if (forceLoad) {
        bool old_spacesLoadedOnce = _spacesLoadedOnce ;
		loadSingleOverrideFilepath(filepath, stop, invokeCallbacks) ;
        // reset _spacesLoadedOnce to old value
        _spacesLoadedOnce = old_spacesLoadedOnce ;
    }
	return _spacesLoadedOnce ;
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#pragma GCC diagnostic warning "-Wswitch"

void ConfigurationSingleton::set(const rsResourceSet &s) // PRQA S 4121
{
	_confSet = s ;
}

rsResourceSet &ConfigurationSingleton::get() // PRQA S 4120
{
	return _confSet ;                        // PRQA S 4024
}

rsResourceSet &ConfigurationSingleton::getPrivate()  // PRQA S 4120
{
	return _privateConfSet ;                 // PRQA S 4024
}

void ConfigurationSingleton::patchResourceSet()
{
	Lock l1(_confSet) ; // PRQA S 3050 3
	Lock l2(_privateConfSet) ;
	Lock l3(_staticConfSet) ;
    mergePrivate();
    mergeStaticSettings();
}


void ConfigurationSingleton::mergePrivate() 
{
	Lock l1(_confSet) ; // PRQA S 3050 2
	Lock l2(_privateConfSet) ;
	_confSet.mergeMode(rsResourceSet::ModeUnion) ;
	_confSet.merge(_privateConfSet) ;
}


bool ConfigurationSingleton::setHostnameSpaceMandatory(bool v) 
{
	bool old = _hostnameSpaceMandatory ;
	_hostnameSpaceMandatory = v;
	return old ;
}


void ConfigurationSingleton::loadConfiguration(const StopPredicate &stop)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "creating merging class") ;
	ConfigMerger mergeConf ;
	mergeConf.addSpaces(_confSpaces) ;
	
	string hostname=HWScan::hostname() ; 
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding space for hostname: \"" << hostname << "\" as " << (_hostnameSpaceMandatory ? "" : "not ") << "mandatory") ; // PRQA S 3380
	mergeConf.addSpace(hostname, _hostnameSpaceMandatory) ;
	
    {
	    // when it goes out of scope ... _confSet is unlocked 
	    // note that it should be locked until the end of the current 
	    // method .. 
	    Lock l1(_confSet) ;  // PRQA S 3050 
	    _confSet.clear() ;
	    mergeConf.getResources(_confSet, stop) ;
	    _replacements = mergeConf.getReplacements() ;

        // detect and collect static settings
        detectStaticSettings(_confSet);

        // apply the private and static settings
        patchResourceSet();
        
		if( not _confFilepaths.empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to add filepaths to override configuration");
			mergeConf.addFilepaths(_confFilepaths) ;
			bool clearReplacements = false; 
 	    	mergeConf.getOverrideResources(_confSet, clearReplacements, stop) ;
		}
       // Merge done!
       
	    _spacesLoadedOnce = true ;

	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
		    oftmpstream cnf("/tmp/confSngtn_conf",".rs") ;
		    ACS_LOG_DEBUG(SimpleDebugSignature << "writing conf in " << cnf.path()) ;
		    cnf.setAutoRemove(false) ;
		    cnf() << _confSet ; 

		    oftmpstream pvt("/tmp/confSngtn_pvt",".rs") ;
		    ACS_LOG_DEBUG(SimpleDebugSignature << "writing pvt in " << pvt.path()) ;
		    pvt.setAutoRemove(false) ;
		    pvt() << _privateConfSet ; 
	    ACS_CLASS_END_DEBUG
    }
    
    // invoke callbacks
    confLoaded() ;

}

void ConfigurationSingleton::detectStaticSettings(const rsResourceSet& theRs)
{
    // Avoid interferences... the theRs should be already locked 
    // but better safe than sorry
	Lock l1(theRs) ;  // PRQA S 3050 2
    Lock l2(_staticConfSet) ;

    // Get all the keys stored in the theRs resource set
    size_t confSetSize = theRs.size();
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Size of theRs is " << confSetSize) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Size of _staticConfSet is " << _staticConfSet.size()) ;

    string keys[confSetSize];
    string values[confSetSize];
    vector<string> comments[confSetSize];

    // Keys extraction call
    theRs.keys (keys, values, comments);

    // Regular expression and key detection tools
    string regexp = "(.*)\\.attributes\\[[0-9]+\\]$";        
    RegExp::KeyMatches keyMatches;
    RegExp myRegExp (regexp);
    bool staticValue = false ;
    string fieldValue;
    string candidateStaticKey;
    string staticValueAbsoluteKey;
    string absoluteKey;

    // Now cycle all the resources trying to find the static keys and maintaining the _staticConfSet resources
    for (size_t i=0; i<confSetSize; ++i)
    {
        if (values[i] == "static")
        {
            if ( myRegExp.match( keys[i], keyMatches) )
            {
                candidateStaticKey = keyMatches[1].getMatched();
                // Use root access (//) to the resource set (some namespace could be pushed)
                staticValueAbsoluteKey = "//" + candidateStaticKey + ".static.value"; 
                absoluteKey = "//" + candidateStaticKey;
                
                if ( theRs.hasKey (staticValueAbsoluteKey) )
                {
                    theRs.getValue (staticValueAbsoluteKey, staticValue);
                    
                    // Static attribute found. Evaluate it.
                    if (staticValue)
                    {
                        // It is a static value. Retrieve it and update the static resource set if needed.
                        theRs.getValue (absoluteKey, fieldValue);
                        
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "STATIC VALUE FOUND: [" << candidateStaticKey << "] = [" << fieldValue << "]") ;
                        
                        if (_staticConfSet.hasKey (absoluteKey))
                        {
                            // Nothing to do here. Just a debug message if enabled.
                            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value already in STATIC area. Not updated") ;
                         }
                        else
                        {
                            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value NOT in STATIC area. Updating...") ;
                            
                            _staticConfSet.setValue (absoluteKey, fieldValue);
                        }
                    }
                }
            }          
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Size of theRs is " << confSetSize) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Size of _staticConfSet is " << _staticConfSet.size()) ;
}

void ConfigurationSingleton::mergeStaticSettings()
{
    // Avoid interferences... the _confSet should be already locked 
    // but better safe than sorry
	Lock l1(_confSet) ;  // PRQA S 3050 2
    Lock l2(_staticConfSet) ;
	_confSet.mergeMode(rsResourceSet::ModeUnion) ;
	_confSet.merge(_staticConfSet) ;
}

void ConfigurationSingleton::loadSingleSpace(const ConfSpace &s, const StopPredicate &stop, bool invokeCallbacks) 
{
	ConfigMerger mergeConf ;
	mergeConf.addSpace(s) ;

    {
	    // when it goes out of scope ... _confSet is unlocked 
	    // note that it should be locked until the end of the current 
	    // method .. 
	    Lock l1(_confSet) ;  // PRQA S 3050 

	    rsResourceSet rs ;
	    mergeConf.getResources(rs, stop) ;
	    addReplacements(mergeConf.getReplacements()) ;
	    _confSet.mergeMode(rsResourceSet::ModeUnion) ;
	    _confSet.merge(rs) ;

        // detect and collect static settings
        detectStaticSettings(rs);

        // apply private and static settings
        patchResourceSet();

		if( not _confFilepaths.empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to add filepaths to override configuration");
			mergeConf.addFilepaths(_confFilepaths) ;
			bool clearReplacements = false;
 	    	mergeConf.getOverrideResources(_confSet, clearReplacements, stop) ;
		}

	    _spacesLoadedOnce = true ;
	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
		    oftmpstream cnf("/tmp/confSngtn_conf",".rs") ;
		    ACS_LOG_DEBUG(SimpleDebugSignature << "writing conf in " << cnf.path()) ;
		    cnf.setAutoRemove(false) ;
		    cnf() << _confSet ; 

		    oftmpstream pvt("/tmp/confSngtn_pvt",".rs") ;
            ACS_LOG_DEBUG(SimpleDebugSignature << "writing pvt in " << pvt.path()) ;
		    pvt.setAutoRemove(false) ;
		    pvt() << _privateConfSet ; 
	    ACS_CLASS_END_DEBUG
    }
    
    if (invokeCallbacks) {
        // invoke callbacks
        confLoaded() ;
    }
}


void ConfigurationSingleton::loadSingleOverrideFilepath(const std::string &fp, const StopPredicate &stop, bool invokeCallbacks) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	ConfigMerger mergeConf ;

    {
	    // when it goes out of scope ... _confSet is unlocked 
	    // note that it should be locked until the end of the current 
	    // method .. 
	    Lock l1(_confSet) ;  // PRQA S 3050 

	    rsResourceSet rs ;
		mergeConf.addFilepath (fp);
		bool clearReplacements = true;
		mergeConf.getOverrideResources(rs, clearReplacements, stop) ;
	    _confSet.mergeMode(rsResourceSet::ModeUnion) ;
	    _confSet.merge(rs) ;
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		    oftmpstream cnf("/tmp/confSngtn_conf",".rs") ;
		    ACS_LOG_DEBUG(SimpleDebugSignature << "writing conf in " << cnf.path()) ;
		    cnf.setAutoRemove(false) ;
		    cnf() << _confSet ; 
			
		    oftmpstream pvt("/tmp/confSngtn_pvt",".rs") ;
            ACS_LOG_DEBUG(SimpleDebugSignature << "writing pvt in " << pvt.path()) ;
		    pvt.setAutoRemove(false) ;
		    pvt() << _privateConfSet ; 
	    ACS_CLASS_END_DEBUG

    }
    
    if (invokeCallbacks) {
        // invoke callbacks
        confLoaded() ;
    }
}

void ConfigurationSingleton::confLoaded() const 
{
    std::for_each(_callbacks.begin(), _callbacks.end(), InvokeCallback()) ;
}


void ConfigurationSingleton::addReplacements(const std::vector< std::pair <std::string, std::string> > &v) 
{
	for (size_t i=0; i < v.size(); ++i) {
		addOrReplaceReplacements(v[i], _replacements) ;
    }
}


void ConfigurationSingleton::addReplacements(const std::pair <std::string, std::string> &r) 
{
	addOrReplaceReplacements(r, _replacements) ;
}


const vector<pair<string,string> > &ConfigurationSingleton::getReplacements() const // PRQA S 4120
{
	return _replacements ;
}


void ConfigurationSingleton::addCallback(ConfSpaceLoadCallback f, void *data) // PRQA S 4020
{
    if (not f) { return ; }
    
    Callback cb(f,data) ;
    // anyway add the callback
    if (std::find(_callbacks.begin(), _callbacks.end(), cb) == _callbacks.end()) {
        _callbacks.push_back(cb) ;  
    }
}

bool ConfigurationSingleton::addOrReplaceSpace(
    const ConfSpace & c, std::vector<ConfSpace> &v) 
{
	bool replaced = false ;
    size_t vsize = v.size() ;
	for (size_t i=0; i < vsize; ++i) {
		if (v[i].name() == c.name()) {
			v[i] = c ;
			replaced = true ;
			break ;		
		}
	}
	if (!replaced) {
		v.push_back(c) ;
    }
	return !replaced ;
}

bool ConfigurationSingleton::addOrReplaceFilepath(
	const std::string & fp, std::vector<std::string> &v) 
{
	bool replaced = false ;
    size_t vsize = v.size() ;
	string fpAbs = StringUtils::pathNormalize(File::getAbsolutePath(fp));
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Normalized absolute path is \"" << fpAbs << '\"');
	for (size_t i=0; i < vsize; ++i) {
		if (v[i] == fpAbs ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << fpAbs << "\" file path already exists in vector");
			v[i] = fpAbs ;
			replaced = true ;
			break ;		
		}
	}
	if (!replaced) {
		v.push_back(fpAbs) ;
    }
	return !replaced ;
	
	
	
}


bool ConfigurationSingleton::addOrReplaceReplacements(const std::pair <std::string, std::string> &r, std::vector< std::pair <std::string, std::string> >&v) 
{
	bool replaced = false ;
    size_t vsize = v.size() ;
	for (size_t i=0; i < vsize; ++i) {
		if (v[i].first == r.first) {
			v[i] = r ;
			replaced = true ;
			break ;		
		}
	}
	
	if (!replaced) {
		v.push_back(r) ;
    }
	return !replaced ;
}



void ConfigurationSingleton::InvokeCallback::operator()( const Callback &c)  throw() // PRQA S 4211
{
    try {
        c.first(c.second) ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}


_ACS_END_NAMESPACE

