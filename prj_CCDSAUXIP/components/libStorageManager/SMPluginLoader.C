// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/12/11 11:53:49  marfav
	Issuing major release 6.0
	
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.4  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.3  2013/02/06 16:16:57  marpas
	some coding best prctice applied
	some qa rule fixed
	
	Revision 2.2  2012/03/01 16:40:33  marpas
	default path warning emitted
	
	Revision 2.1  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/01/17 10:50:00  marpas
	init signature slightly changed behaviour is the same
	
	Revision 1.2  2005/07/28 14:26:43  marpas
	inline SMPluginLoader::init(void) method added. The previous
	init method has been changed in order to pass a boolean to avoid throwing
	exceptions if the plugin file was not found. In the latter case a Warning is issued
	
	Revision 1.1  2005/06/15 08:07:43  integrator
	multiple storage manager implemented
	
	

*/


#include <SMPluginLoader.h>
#include <File.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

SMPluginLoader *SMPluginLoader::_instance = 0 ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SMPluginLoader) 

SMPluginLoader::SMPluginLoader() :
    PluginLoader(),
    _pluginSet() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


void SMPluginLoader::load(const string &path, bool ignoreIfNotExists) // PRQA S 4020
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	if (!File::exists(path)) {
		if (ignoreIfNotExists) {
			ACS_LOG_WARNING("the file \"" << path << "\" does not exixts or is not readable") ;
			return ;
		}
		ACS_THROW(exFileOpenException("file \"" + path +"\" does not exists")) ; // PRQA S 3081
	}
	
	try {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path: " << path) ;
		rsResourceSet rs ;
		ifstream in(path.c_str()) ;
		in >> rs ;
		size_t loaded = _pluginSet.load(rs, *this) ;
		PluginLoader::load() ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loaded: " << loaded << " plugins definitions") ;
		if (0 == loaded) {
			ACS_THROW(PluginSet::BadResourceFile("No plugin configured in " + path)) ; // PRQA S 3081
		}
	}
	catch(exException &e) {
		ACS_THROW(PluginSet::BadResourceFile(e,"Error loading SM plugins")) ; // PRQA S 3081
	}

}

void SMPluginLoader::init(const std::string &path, bool ignoreIfNotExists) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_instance: " << _instance) ;
	if (_instance) {
		ACS_THROW(exIllegalValueException("duplicate initialization of SMPluginLoader")) ; // PRQA S 3081
	}
	
	_instance = new SMPluginLoader() ;
	try {
		_instance->load(path, ignoreIfNotExists) ;
	}
	catch(...) {
		delete _instance ;
		_instance = 0 ;
		throw ;
	}
	
}

void SMPluginLoader::init(const string & path)
{
	init(not path.empty() ? path : base::defaultSMPluginsPath() + std::string("/") + base::defaultSMPluginsFileName(),true) ; // PRQA S 3081, 3380, 3385
}


_ACS_END_NAMESPACE
