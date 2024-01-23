//
//	Copyright 1995-2018, Advanced Computer Systems , Inc.
//	Via Della Bufalotta, 378 - 00139 Roma - Italy
//	http://www.acsys.it
//
//	All Rights Reserved.
//
//	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
//	the contents of this file may not be disclosed to third parties, copied or
//	duplicated in any form, in whole or in part, without the prior written
//	permission of Advanced Computer Systems, Inc.
//	
        

#include <ManifestParametersListBuilder.h>

#include <Filterables.h>

#include <UtilXml.h>
#include <algorithm>
#include <sstream>
#include <iterator>


using namespace std;
using namespace acs;
_ACS_BEGIN_NAMESPACE(acs)
		

ACS_CLASS_DEFINE_DEBUG_LEVEL(ManifestParametersListBuilder)

Mutex ManifestParametersListBuilder::_mapMutex;

namespace {
	map< string, pair<string, string> > localParametersMap;
	
}

const int maxVersionNum = 100;

// ****************************************************************************
ManifestParametersListBuilder::ManifestParametersListBuilder():
												_versionNumV(),
												_xmlDoc(),
												_parPaths()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

// ****************************************************************************
ManifestParametersListBuilder::~ManifestParametersListBuilder()  throw() 
{
	
}

// ****************************************************************************
void ManifestParametersListBuilder::readManifestVersions(const string & manifestPath) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);


	std::vector<std::string> versions;

	_xmlDoc.setParameters(XmlDoc::parDefault).initWithFile(manifestPath);

#ifdef MULTIPLE_VERSIONS
    //-----------------------------------------
    // read versions from namespaces
    //-----------------------------------------
	const vector<XmlDoc::XNamespace> & xnss = _xmlDoc.getXnamespaces() ;
	for ( vector<XmlDoc::XNamespace>::const_iterator it = xnss.begin(); it!=xnss.end(); ++it ) {
		const XmlDoc::XNamespace & xns = *it;
		versions.push_back(xns.getHref());
	}
#endif
			
    //-----------------------------------------
    // read version from version attribute
    //-----------------------------------------
	std::vector<XmlDocSptr> xmls = _xmlDoc.xPath("/*/@version");
	if (xmls.empty()==false){
		vector< XmlDocSptr >::iterator vit = xmls.begin();
		versions.push_back((*vit)->str());
	}
			
    //-----------------------------------------
    // convert versions to integers
    //-----------------------------------------
	for ( std::vector<std::string>::const_iterator it = versions.begin(),end = versions.end(); it!=end; ++it ) {
    	int vmajor=0;
    	int vminor=0;
    	string::size_type pos = it->rfind('/');
		int nread=0;
    	if (pos != string::npos){
    		nread = sscanf(it->substr(pos).c_str(),"/%d.%d",&vmajor,&vminor);
    	}
    	if (nread != 0) {_versionNumV.push_back(maxVersionNum*vmajor + vminor);}
	}
			
}

// ****************************************************************************
void ManifestParametersListBuilder::buildParametersList(const rsResourceSet & rs, const string & key )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	ACS_COND_THROW( _versionNumV.empty(), exIllegalValueException("Versions vector not valorized!") );
	
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, rs);
	string rootTag = "ManifestVersionManagement.Version";
    size_t nodeNum = rs.getArraySize( rootTag ); // PRQA S 3000, 3010, 4412
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << rootTag << ", elements " << nodeNum);
	Lock lock(_mapMutex); //protegge static localParametersMap da uso contemporaneo in piu' thread 
    //-----------------------------------------
    // for each version
    //-----------------------------------------
	for( size_t i = 0; i < nodeNum ; i++) {
		string localTag = rootTag;
		if( nodeNum > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			localTag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=\"" << localTag << '\"');
		
    	//-----------------------------------------
    	// if any version greater than manifest version exit loop
    	//-----------------------------------------
		if (rs.hasKey(localTag + ".Versions")) {
			string currentVersions;
			rs.getValue(localTag + ".Versions", currentVersions);
			if ( AnyGreaterThanManifestVersion( currentVersions ) ){
				break;
			}
		}

    	//-----------------------------------------
    	// if parameters for not requested filetype skip them
    	//-----------------------------------------
		vector<string> keysV;
		rs.getArray(localTag + ".ListOfKeys.Key", keysV);

		if ( !keysV.empty() && 
				(key.empty() 
			|| (!key.empty() && find(keysV.begin(), keysV.end(), key) == keysV.end()) ) ) {
			continue;
		}

		vector<Parameter> parameters;
		rs.getArray(localTag + ".ListOfParameters.Parameter", parameters);
		
	}
	_parPaths = localParametersMap;

    //-----------------------------------------
    // Debug if necessary
    //-----------------------------------------
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		for( std::map< std::string, std::pair<std::string, std::string> >::const_iterator it = _parPaths.begin(),end = _parPaths.end(); it !=  end; ++it ) {
			ostringstream os;
			os << "parameter=\"" << it->first << "\"; ";
			if( not it->second.second.empty() ) {
				os << "default=\"" << it->second.second << '\"';
			}
			else {
				os << "xpath=\"" << it->second.first << '\"';
			}
			ACS_LOG_DEBUG( SimpleDebugSignature << os.str() );
		}
	ACS_CLASS_END_DEBUG
	
}

// ****************************************************************************
std::string ManifestParametersListBuilder::getParameter(const std::string & parName) const
{

	std::string retval("");
	try{
		std::map< std::string, std::pair<std::string, std::string> >::const_iterator 
			it = _parPaths.find(parName);

    	//-----------------------------------------
    	// if found parName
    	//-----------------------------------------
		if (it != _parPaths.end()) {

			std::pair<std::string, std::string> val = (*it).second;

			if (val.first.empty()) {
				// use default value
				retval = val.second;
			} else {
				// use xpath 
				std::vector<XmlDocSptr> xmls = _xmlDoc.xPath(val.first);
                if (xmls.empty()==false){
				    retval = xmls[0]->str();
                    if (xmls.size() > 1) {
                        ACS_LOG_WARNING("More instances of xPath=\"" << val.first << "\"");
                    }
				}else{
                        ACS_LOG_WARNING("No instances of xPath=\"" << val.first << "\"");
                }
			}
		}else{
            ACS_LOG_WARNING("Not configured parName=\"" << parName << "\"");
        }
	}catch(const exception& toCatch){
		ACS_LOG_WARNING("error reading parName=\"" << parName << "\": " << toCatch.what());
    }
	return(retval);	
}


// ****************************************************************************
string ManifestParametersListBuilder::getParameter(map< string, pair<string, string> >::const_iterator it,  string & parName) const
{
	parName = it->first;
	return getParameter(parName);
}


// ****************************************************************************
void ManifestParametersListBuilder::getVersionNum(vector<int> & VersionNumV, const string & versions)
{
    std::stringstream ss(versions);
    string s;
    while (getline(ss, s, ' ')) {
		int vmajor=0;
		int vminor=0;
		sscanf(s.c_str(),"%d.%d",&vmajor,&vminor);
		int versionNum = maxVersionNum*vmajor + vminor;
		VersionNumV.push_back(versionNum);
    }
}
    
// ****************************************************************************
bool ManifestParametersListBuilder::AnyGreaterThanManifestVersion(const string & currentVersions)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	vector<int> VersionNumV;
	getVersionNum(VersionNumV, currentVersions);
	size_t nVersions = VersionNumV.size();
	

	ACS_COND_THROW( nVersions != _versionNumV.size(), exIllegalValueException("Versions vector must have the same number of elements!") );
	
	bool retval = false;
	for(size_t i=0;i<nVersions;++i){
		if (VersionNumV[i] > _versionNumV[i]) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, VersionNumV[i] << " configured version greater than corresponding manifest version " << _versionNumV[i] );
			retval = true;
			break;
		}
	}

	return retval;
}



// ****************************************************************************
ManifestParametersListBuilder::Parameter::Parameter():
		_name(),
		_xpath(),
		_defaultVal()
{
}								

// ****************************************************************************
ManifestParametersListBuilder::Parameter::Parameter(ManifestParametersListBuilder::Parameter const &o):
		_name(o._name),
		_xpath(o._xpath),
		_defaultVal(o._defaultVal)
{
}								

// ****************************************************************************
ManifestParametersListBuilder::Parameter & ManifestParametersListBuilder::Parameter::operator = (ManifestParametersListBuilder::Parameter const &o)
{
	if( this != &o ) {
		_name = o._name;
		_xpath = o._xpath;
		_defaultVal = o._defaultVal;
	}
	return (*this);
}								

// ****************************************************************************
template<>
void rsResourceSet::getValue(const std::string &res, ManifestParametersListBuilder::Parameter &v) const 
{
    Lock lock(_mutex) ;
	std::string name("");
	std::string xpath("");
	std::string defaultVal("");
	getValue(res+".name.value")->get(name) ;
	v.setname(name);
	
	if( hasKey(res+".default.value") ) {
		getValue(res+".default.value")->get(defaultVal);
		v.setdefaultVal(defaultVal);
	}else{
		getValue(res)->get(xpath);
		v.setxpath(xpath);
	}
	
	localParametersMap[name] = make_pair(xpath, defaultVal);
	
} 

_ACS_END_NAMESPACE

