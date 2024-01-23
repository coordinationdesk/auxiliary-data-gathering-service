// PRQA S 1050 EOF
/*
	Copyright 1995-2021 Exprivia Spa - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.19  2018/07/17 06:52:00  giucas
	writeManifest XMLOstream management fixed.

	Revision 1.18  2018/07/16 14:52:43  giucas
	writeManifest rootTag close added.

	Revision 1.17  2018/03/01 14:57:13  tergem
	S3EVOL-142: productUnitType parameter added for L0 conf files

	Revision 1.16  2016/05/11 09:45:43  danalt
	use of ACS_THROW and ACS_COND_THROW

	Revision 1.15  2014/06/26 15:09:21  tergem
	alignment to format 3.4

	Revision 1.14  2014/04/24 16:43:28  tergem
	Porting to head

	Revision 1.13  2014/01/10 14:33:17  nicvac
	S1PDGS-30085: support xsd added.

	Revision 1.12  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)

	Revision 1.11  2013/11/11 10:47:42  masdal
	dispositionMode handling added.

	Revision 1.10  2013/10/29 19:08:57  nicvac
	compilation warning processed.

	Revision 1.9  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.

	Revision 1.8  2013/02/26 13:02:09  giucas
	PATCH FOR V1 FAT TO DISABLE USAGE OF OLQC.xml : To be managed properly in NEXT DELIVERY

	Revision 1.7  2012/08/03 12:58:10  damdec
	Check on the existence of manifest file introduced.

	Revision 1.6  2012/07/27 10:14:15  damdec
	Exception raised if representation file tag is missing in the configuration file. Manifest file name passed as parameter. dataObject setting changed (some common configuration fields specialized for object.)

	Revision 1.5  2012/06/22 13:02:17  damdec
	Added the possibility to create a dummy representation data file enabling a flag.

	Revision 1.4  2012/05/04 09:25:18  damdec
	Setting of dataObject.textInfo modified.

	Revision 1.3  2012/04/19 13:36:34  damdec
	Member inizialization list of constructor updated.

	Revision 1.2  2012/04/17 15:44:07  damdec
	Loading of the configuration file moved here. getMetadataWrap method added.

	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter


*/
#include <ACSSafeFormatterV.h>
#include <SafeManifest.h>
#include <WrappedMetadata.h>

#include <Filterables.h>
#include <RegExp.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <StringUtils.h>
#include <Md5Sum.h>
#include <File.h>
#include <XMLIstream.h>
#include <XMLOstream.h>
#include <SpawnProc.h>
#include <Timer.h>
#include <XMLHandler.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
const std::string SafeManifest::_defaultSafePrefix = "sentinel-safe:";

ACS_CLASS_DEFINE_DEBUG_LEVEL(SafeManifest)

namespace {
    ACSSafeFormatterV version;
}

SafeManifest::SafeManifest(const string& productType,
                           const std::string& version,
                           const std::string& path,
                           const string & configFilename) :
    _rsConf(),
    _productType(productType),
	_mapDataObject(),
	_mapMetadataWrap(),
	_mapMetadataReference(),
	_mapMetadataObject(),
	_rootContentUnit(),
	_safeNamespace(),
	_safePrefix(_defaultSafePrefix),
	_xfduNamespace(),
	_rootTagOtherAttributes(),
	_version(version),
	_path(path),
	_dispositionMode(),
	_productUnitType()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if( not configFilename.empty() ) {
		XMLIstream  xmlIstream(configFilename);
        xmlIstream >> _rsConf;
	}
	else {
		_rsConf = ConfigurationSingleton::instance()->get() ;
	}

	loadConfiguration();
}

SafeManifest::~SafeManifest() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void SafeManifest::loadConfiguration()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	rsPushNSpace pushName(_rsConf, "SafeProduct");
	_rsConf.getValue("dispositionMode", _dispositionMode);
	if (_rsConf.hasKey("productUnitType") ) _rsConf.getValue("productUnitType", _productUnitType);
	_rsConf.getValue("safeNamespace", 			_safeNamespace);
	_rsConf.getValue("xfduNamespace", 			_xfduNamespace);

	if ( _rsConf.hasKey("rootTagOtherAttributes") ) { _rsConf.getValue("rootTagOtherAttributes", _rootTagOtherAttributes); }
	if ( _rsConf.hasKey("safePrefix") ) 			{ _rsConf.getValue("safePrefix", _safePrefix); }
	if (!StringUtils::endsWith(_safePrefix, ":"))
	{
		_safePrefix+=":";
	}
	ACS_COND_THROW(!matchProductType(), exSafeManifestException("Formatter not configured for Product Type " + _productType));  // PRQA S 3081

	loadRootContentUnit();

	unsigned int numObjects(0);
	_rsConf.getValue("ListOfObjects.count.value", numObjects);

	for (unsigned int i=0; i< numObjects; i++)
	{
		ostringstream objectKey; objectKey << "ListOfObjects.Object" << "[" << i << "]";

		string type;
		_rsConf.getValue(objectKey.str() + ".type.value", type);

		rsPushNSpace localNSpace( _rsConf, objectKey.str() );

		if(type == "Data") {
		    loadDataObjectConfiguration();
        }
		else if (type == "Metadata") {
		    loadMetadataConfiguration();
        }
		else if (type == "Schema") {
		    loadSchemaConfiguration();
        }
		else {
		    ACS_THROW(exSafeManifestException("Configuration error: value \"" + type + "\" for tag " + objectKey.str() + ".type NOT VALID" ));	    // PRQA S 3081
	    }
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << "SafeManifest::loadConfiguration() executed:" << '\n';

		msg << "<LIST OF DATAOBJECTS> " << '\n';
		typedef std::map<std::string, DataObjectDescriptor>::iterator doIt;
		for (doIt it =_mapDataObject.begin(); it!=_mapDataObject.end(); ++it) { // PRQA S 4238
			msg << it->first << '\n';
        }
		msg << "</LIST OF DATAOBJECTS> " << '\n';

		msg << "<LIST OF WRAPPED METADATA> " << '\n';
		typedef std::map<std::string, MetadataWrapDescriptor>::iterator mwIt;
		for (mwIt it =_mapMetadataWrap.begin(); it!=_mapMetadataWrap.end(); ++it) { // PRQA S 4238
			msg << it->first << '\n';
        }
		msg << "</LIST OF WRAPPED METADATA> " << '\n';

		msg << "<LIST OF METADATA REFERENCE> " << '\n';
		typedef std::map<std::string, MetadataReferenceDescriptor>::iterator mrIt;
		for (mrIt it =_mapMetadataReference.begin(); it!=_mapMetadataReference.end(); ++it) { // PRQA S 4238
			msg << it->first << '\n';
        }
		msg << "</LIST OF METADATA REFERENCE> " << '\n';

		msg << "<LIST OF METADATA OBJECTS> " << '\n';
		typedef std::map<std::string, MetadataObjectDescriptor>::iterator mIt;
		for (mIt it =_mapMetadataObject.begin(); it!=_mapMetadataObject.end(); ++it) { // PRQA S 4238
			msg << it->first << '\n';
        }
		msg << "</LIST OF METADATA OBJECTS> " << '\n';

		ACS_LOG_DEBUG(msg.str()) ;
	ACS_CLASS_END_DEBUG
}

void SafeManifest::loadRootContentUnit()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream key; key << "InformationPackageMap." ;
	//rsPushNSpace tmpNSpace( _rsConf, key.str());

	_rsConf.getValue(key.str()+"Id", _rootContentUnit.id) ;
	_rsConf.getValue(key.str()+"unitType", _rootContentUnit.unitType) ;
	_rsConf.getValue(key.str()+"textInfo", _rootContentUnit.textInfo) ;
	_rsConf.getValue(key.str()+"pdiId", _rootContentUnit.pdiId) ;
	_rsConf.getValue(key.str()+"dmdId", _rootContentUnit.dmdId) ;

}

void SafeManifest::loadDataObjectConfiguration()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string unitType;
	string locatorType;
	string checksumName;

	_rsConf.getValue("Common.unitType", unitType) ;
	_rsConf.getValue("Common.locType", locatorType) ;
	_rsConf.getValue("Common.checksumName", checksumName) ;

	unsigned int numObjs(0);
	_rsConf.getValue("ListOfObjectConfigs.count.value", numObjs);

	for (unsigned int i=0; i< numObjs; i++)
	{
		DataObjectDescriptor dataObject;
		dataObject.isSet = false;

		// common fields
		dataObject.unitType = unitType;
		dataObject.locatorType = locatorType ;
		dataObject.checksumName = checksumName;

		ostringstream key; key << "ListOfObjectConfigs.ObjectConfig" <<  "[" << i << "]";

		_rsConf.getValue(key.str()+".Id", dataObject.id) ;
		_rsConf.getValue(key.str()+".dmdId", dataObject.dmdId) ;
		_rsConf.getValue(key.str()+".textInfo", dataObject.textInfo) ;
		_rsConf.getValue(key.str()+".dataObjectId", dataObject.dataObjectId) ;
		try
		{
			_rsConf.getValue(key.str()+".repId", dataObject.repId) ;
		}
		catch (exception & e)
		{
			ostringstream os;
			os <<  "Mandatory representation id missing " << '\n';
			ACS_THROW(exSafeManifestException(os.str()));
		}
		_rsConf.getValue(key.str()+".mimeType", dataObject.mimeType) ;

		_mapDataObject.insert( make_pair(dataObject.id, dataObject) );
	}

}

void SafeManifest::loadMetadataConfiguration()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string metadataClass("");
	_rsConf.getValue("Common.metadataClass", metadataClass);

	ACS_COND_THROW( metadataClass.empty(), exSafeManifestException("Configuration error: Null metadataClass"));

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SafeManifest: loadMetadataConfiguration for metadataClass " << metadataClass);

	if(metadataClass == "MetadataWrap")
	{
		string mimeType;
		_rsConf.getValue("Common.mimeType", mimeType) ;
		string vocabularyName;
		_rsConf.getValue("Common.vocabularyName", vocabularyName) ;

		unsigned int numObjs(0);
		_rsConf.getValue("ListOfObjectConfigs.count.value", numObjs);

		for (unsigned int i=0; i< numObjs; i++)
		{
			ostringstream key; key << "ListOfObjectConfigs.ObjectConfig" <<  "[" << i << "]";

			MetadataWrapDescriptor metadataWrap;
			// common fields
			metadataWrap.mimeType = mimeType;
			metadataWrap.vocabularyName = vocabularyName;
			// specific fields
			_rsConf.getValue(key.str()+".Id", metadataWrap.id) ;
			_rsConf.getValue(key.str()+".metadataObjectId", metadataWrap.metadataObjectId) ;
			_rsConf.getValue(key.str()+".classification", metadataWrap.classification) ;
			_rsConf.getValue(key.str()+".category", metadataWrap.category) ;
			_rsConf.getValue(key.str()+".textInfo", metadataWrap.textInfo) ;

			// loading of optional specific fields
			try
			{
				_rsConf.getValue(key.str()+".srsName", metadataWrap.srsName) ;
			}
			catch (exception & e)
			{
				//do nothing
			}
			try
			{
				_rsConf.getValue(key.str()+".namespace", metadataWrap.safeNamespace) ;
			}
			catch (exception & e)
			{
				//do nothing
			}

			_mapMetadataWrap.insert( make_pair(metadataWrap.id, metadataWrap) );
		}

	}

	if(metadataClass == "dataObjectPointer")
	{
		//first add an entry into the dataObject map
		string unitType ;
		string locatorType ;
		string checksumName ;
		string mimeType;

		_rsConf.getValue("Common.unitType", unitType) ;
		_rsConf.getValue("Common.locType", locatorType) ;
		_rsConf.getValue("Common.checksumName", checksumName) ;
		_rsConf.getValue("Common.mimeType", mimeType) ;

		unsigned int numObjs(0);
		_rsConf.getValue("ListOfObjectConfigs.count.value", numObjs);
		for (unsigned int i=0; i< numObjs; i++)
		{
			ostringstream key; key << "ListOfObjectConfigs.ObjectConfig" <<  "[" << i << "]";
			DataObjectDescriptor dataObject;
			dataObject.isSet = false;

			dataObject.unitType = unitType;
	     		dataObject.locatorType	= locatorType;
	     		dataObject.checksumName	= checksumName;
	     		dataObject.mimeType	= mimeType;

			_rsConf.getValue(key.str()+".Id", dataObject.id) ;
			_rsConf.getValue(key.str()+".dmdId", dataObject.dmdId) ;
			_rsConf.getValue(key.str()+".textInfo", dataObject.textInfo) ;
			_rsConf.getValue(key.str()+".dataObjectId", dataObject.dataObjectId) ;
			try
			{
				_rsConf.getValue(key.str()+".repId", dataObject.repId) ;
			}
			catch (exception & e)
			{
				ostringstream os;
				os <<  "Mandatory representation id missing " << '\n';
				ACS_THROW(exSafeManifestException(os.str()));
			}

			_mapDataObject.insert( make_pair(dataObject.id, dataObject) );

			//then add an entry into the metadataObject map
			MetadataObjectDescriptor metadataObj;

			_rsConf.getValue(key.str()+".Id", metadataObj.id) ;
			_rsConf.getValue(key.str()+".metadataObjectId", metadataObj.metadataObjectId) ;
			_rsConf.getValue(key.str()+".classification", metadataObj.classification) ;
			_rsConf.getValue(key.str()+".category", metadataObj.category) ;
			_rsConf.getValue(key.str()+".dataObjectId", metadataObj.dataObjectId) ;

			_mapMetadataObject.insert( make_pair(metadataObj.id, metadataObj) );
		}
	}
}

void SafeManifest::loadSchemaConfiguration()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string locatorType ;
	string vocabularyName ;
	string mimeType;
	_rsConf.getValue("Common.locatorType", locatorType) ;
	_rsConf.getValue("Common.vocabularyName", vocabularyName) ;
	_rsConf.getValue("Common.mimeType", mimeType) ;

	unsigned int numObjs(0);
	_rsConf.getValue("ListOfObjectConfigs.count.value", numObjs);
	for (unsigned int i=0; i< numObjs; i++)
	{
		ostringstream key; key << "ListOfObjectConfigs.ObjectConfig" <<  "[" << i << "]";
		MetadataReferenceDescriptor metadataRef;

		metadataRef.isSet = false;
		metadataRef.locatorType = locatorType;
		metadataRef.vocabularyName = vocabularyName;
		metadataRef.mimeType = mimeType;

		_rsConf.getValue(key.str()+".Id", metadataRef.id) ;
		_rsConf.getValue(key.str()+".metadataObjectId", metadataRef.metadataObjectId) ;
		_rsConf.getValue(key.str()+".classification", metadataRef.classification) ;
		_rsConf.getValue(key.str()+".category", metadataRef.category) ;
		_rsConf.getValue(key.str()+".filename", metadataRef.href) ;
		//_rsConf.getValue(key.str()+".content", metadataRef.hrefContent) ;

		_mapMetadataReference.insert( make_pair(metadataRef.id, metadataRef) );
	}

}

bool SafeManifest::matchProductType() {

	string prodType("");
	_rsConf.getValue("productType", prodType);

	bool found(false);
	RegExp re;
	re.setReg(prodType);

	if(re.match(_productType) )
		found = true;

	return found;
}

void SafeManifest::setDataObject(const std::string& id, const std::string& filename, const bool & repDummy)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SafeManifest: setDataObject called with parameters: <" << id << "," << filename << ">");

	ACS_COND_THROW( _mapDataObject.find(id) == _mapDataObject.end(), exSafeManifestException("DataObject Id <" + id + "> not configured."));

	_mapDataObject[id].isSet = true;
	_mapDataObject[id].href = filename;

	vector<string> schema;
	StringUtils::Tokenize(_mapDataObject[id].repId, schema, " ");

	typedef map<string, MetadataReferenceDescriptor>::iterator mdRefIt;
	//search for the configured schema
	for(vector<string>::iterator it= schema.begin(); it!=schema.end(); ++it)
	{

		string schemaId = *it;
		mdRefIt refIt = _mapMetadataReference.find(schemaId);
		ACS_COND_THROW(  refIt == _mapMetadataReference.end(), exSafeManifestException("Schema Id <" + schemaId + "> required by Data Object " + id  + " not configured."));

		_mapMetadataReference[schemaId].isSet = true;

		if (repDummy)
		{
			/* Create dummpy representation file */
			//File::touch(_path+"/"+_mapMetadataReference[schemaId].href);

			const string repDummyName( _path+"/"+_mapMetadataReference[schemaId].href );
			ofstream repDummyOfs( repDummyName.c_str() );

			ACS_COND_THROW( ! repDummyOfs.is_open(),
					exSafeManifestException( "Creating support xsd schema, cannot open file "+repDummyName+" in write mode."  ) );

			repDummyOfs << _mapMetadataReference[schemaId].hrefContent;
			repDummyOfs.close();
		}


	}


	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; msg << "SafeManifest::setDataObject() called. "<<'\n';
		msg << "Added dataObject [configured id=" << id << "] with filename " << filename << '\n' ;
		ACS_LOG_DEBUG(msg.str()) ;
	ACS_CLASS_END_DEBUG

}

void SafeManifest::resetDataObject(const std::string& id, const std::string& newValue)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	ACS_COND_THROW( _mapDataObject.find(id) == _mapDataObject.end(), exSafeManifestException("DataObject Id <" + id + "> not configured."));

	_mapDataObject[id].dataObjectId = newValue;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reset dataObject for id "<< id <<" with " << newValue);
}

void SafeManifest::setMetadataWrap(const std::string& id, const WrappedMetadata& metadata)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SafeManifest: setMetadataWrap called for id: <" << id << ">");

	ACS_COND_THROW( _mapMetadataWrap.find(id) == _mapMetadataWrap.end(), exSafeManifestException("WrappedMetadata Id <" + id + "> not configured."));

	_mapMetadataWrap[id].xmlData = metadata.getXMLStream() ;
}

void SafeManifest::writeManifest(const std::string & name)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string filename( _path + "/" + name );
	ACS_COND_THROW(File::exists(filename), exIOException("Manifest file \"" + filename + "\" already exists. Cannot overwrite it!") );
	ofstream ostr( filename.c_str(), ios::out);
	ACS_COND_THROW(!ostr.is_open(), exIOException("Can't open manifest file \"" + filename + "\"") );
	string spfx = StringUtils::removeEnd(_safePrefix, 1) ;

	ostringstream rootTagValue;
	if ( !_version.empty() ) 				{ rootTagValue << "version=\"" << _version << "\"";	}
	if ( !_safeNamespace.empty() ) 			{ rootTagValue << " xmlns:"<< spfx << "=\"" << _safeNamespace << "\"";	}
	if ( !_xfduNamespace.empty() ) 			{ rootTagValue << " xmlns:xfdu=\"" << _xfduNamespace << "\"";	}
	if ( !_rootTagOtherAttributes.empty() ) { rootTagValue << " " << _rootTagOtherAttributes; }

	XMLOstream  xst(ostr, "xfdu:XFDU", rootTagValue.str());

	writeInformationPackageMap(xst);

	writeMetadataSection(xst);

	writeDataObjectSection(xst);
	xst.close();
	ostr.close() ;
}

void SafeManifest::writeContentUnit(XMLOstream& xst, const DataObjectDescriptor& dataObj)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream attribute;
	attribute << "unitType=\"" << dataObj.unitType << "\"";
	attribute << " textInfo=\"" << dataObj.textInfo << "\"";
	if(!dataObj.repId.empty()) {
		attribute << " repID=\"" << dataObj.repId << "\"";
	}
	if(!dataObj.dmdId.empty()) {
		attribute << " dmdID=\"" << dataObj.dmdId << "\"";
	}
	XMLOstream::Tag ContentUnit(xst, "xfdu:contentUnit", "", attribute.str());

	attribute.str(""); //reset
	attribute << "dataObjectID=\"" << dataObj.dataObjectId	 << "\"";
	XMLOstream::Tag DataObjectPointer(xst, "dataObjectPointer", "", attribute.str(), true);
}

void SafeManifest::writeInformationPackageMap(XMLOstream& xst)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	XMLOstream::Tag InformationPackageMap(xst, "informationPackageMap", "", "");

	ostringstream mainContentUnitAttribute;
	mainContentUnitAttribute << "unitType=\""  << _rootContentUnit.unitType << "\"";
	if(!_rootContentUnit.textInfo.empty()) {
		mainContentUnitAttribute << " textInfo=\"" << _rootContentUnit.textInfo << "\"";
	}
	if(!_rootContentUnit.dmdId.empty()) {
		mainContentUnitAttribute << " dmdID=\""    << _rootContentUnit.dmdId    << "\"";
	}
	if(!_rootContentUnit.pdiId.empty()) {
		mainContentUnitAttribute << " pdiID=\""    << _rootContentUnit.pdiId    << "\"";
	}

	XMLOstream::Tag MainContentUnit(xst, "xfdu:contentUnit", "", mainContentUnitAttribute.str());

	typedef map<string, DataObjectDescriptor>::iterator dataObjIt;
	for(dataObjIt it=_mapDataObject.begin(); it!=_mapDataObject.end(); ++it)
	{
		if(it->second.isSet) {
			writeContentUnit(xst, it->second);
		}
	}
}

void SafeManifest::writeMetadataSection(XMLOstream& xst)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	XMLOstream::Tag MetadataSection(xst, "metadataSection", "", "");

	//Wrapped Metadata Objects
	typedef map<string, MetadataWrapDescriptor>::iterator mdWrapIt;
	for(mdWrapIt it=_mapMetadataWrap.begin(); it!=_mapMetadataWrap.end(); ++it)
	{
		ostringstream attribute;
		attribute << "ID=\"" << it->second.metadataObjectId << "\"";
		attribute << " classification=\"" << it->second.classification << "\"";
		attribute << " category=\"" << it->second.category << "\"";

		XMLOstream::Tag MetadataObject(xst, "metadataObject", "", attribute.str());

		attribute.str(""); //reset
		attribute << "mimeType=\"" << it->second.mimeType << "\"" ;
		attribute << " vocabularyName=\"" << it->second.vocabularyName << "\"" ;
		attribute << " textInfo=\"" << it->second.textInfo << "\"";

		XMLOstream::Tag MetadataWrap(xst, "metadataWrap", "", attribute.str());
		stringstream ss; ss << it->second.xmlData;
		xst.insertXML( ss );


	}
	//Metadata Reference
	typedef map<string, MetadataReferenceDescriptor>::iterator mdRefIt;
	for(mdRefIt it=_mapMetadataReference.begin(); it!=_mapMetadataReference.end(); ++it)
	{
		if(it->second.isSet)
		{
		    ostringstream metadataObjAttribute;
		    metadataObjAttribute << "ID=\"" << it->second.metadataObjectId << "\"";
		    metadataObjAttribute << " classification=\"" << it->second.classification << "\"";
		    metadataObjAttribute << " category=\"" << it->second.category << "\"";

		    ostringstream metadataReferenceAttribute;
		    metadataReferenceAttribute << "mimeType=\"" << it->second.mimeType << "\"";
		    metadataReferenceAttribute << " vocabularyName=\"" << it->second.vocabularyName << "\"";
		    metadataReferenceAttribute << " locatorType=\"" << it->second.locatorType << "\"";

		    string file = _path + "/" + it->second.href;
		    ACS_COND_THROW(!File::exists(file), exSafeManifestException("File " + file +  " doesn't exist!"));

		    metadataReferenceAttribute << " href=\"" << it->second.href << "\"";

		    XMLOstream::Tag Schema(xst, "metadataObject", "", metadataObjAttribute.str());
		    XMLOstream::Tag MetadataReference(xst, "metadataReference", "", metadataReferenceAttribute.str(), true);

		}


	}

	//Metadata Object
	typedef map<string, MetadataObjectDescriptor>::iterator mdIt;
	for(mdIt it=_mapMetadataObject.begin(); it!=_mapMetadataObject.end(); ++it)
	{
		if( _mapDataObject.find(it->second.id)->second.isSet)
		{
			ostringstream attribute;
			attribute << "ID=\"" << it->second.metadataObjectId << "\"";
			attribute << " classification=\"" << it->second.classification << "\"";
			attribute << " category=\"" << it->second.category << "\"";

			XMLOstream::Tag MetadataObject(xst, "metadataObject", "", attribute.str());

			attribute.str(""); //reset
			attribute << "dataObjectID=\"" << it->second.dataObjectId << "\"";

			XMLOstream::Tag DataObjectPointer(xst,"dataObjectPointer","", attribute.str(), true);
		}

	}
}

MetadataWrapDescriptor & SafeManifest::getMetadataWrap(const std::string& id)
{
	ACS_COND_THROW( _mapMetadataWrap.find(id) == _mapMetadataWrap.end(), exSafeManifestException("WrappedMetadata Id <" + id + "> not configured."));

	return _mapMetadataWrap[id];
}

vector<string> SafeManifest::getXmlSchemas() const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	vector<string> schemaFilenames;

	typedef map<string, MetadataReferenceDescriptor>::const_iterator mdRefIt;
	for(mdRefIt it=_mapMetadataReference.begin(); it!=_mapMetadataReference.end(); ++it)
	{
		if(it->second.isSet)
		{
			schemaFilenames.push_back( File::getFileName(it->second.href));
		}
	}
	return schemaFilenames;
}

void SafeManifest::writeDataObjectSection(XMLOstream& xst)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	XMLOstream::Tag DataObjectSection(xst, "dataObjectSection", "", "");

	typedef map<string, DataObjectDescriptor>::iterator dataObjIt;
	for(dataObjIt it=_mapDataObject.begin(); it!=_mapDataObject.end(); ++it)
	{
		if(it->second.isSet && it->second.href != "OLQC.xml") //!!! PATCH FOR V1 FAT TO DISABLE USAGE OF OLQC.xml
		//if(it->second.isSet)
		{
			ostringstream dataObjectAttribute;
			dataObjectAttribute << "ID=\"" << it->second.dataObjectId << "\"";
			if(!it->second.repId.empty()) dataObjectAttribute << " repID=\"" << it->second.repId << "\"";

			ACS_COND_THROW(!File::exists( _path + "/" + it->second.href), exSafeManifestException("File " + it->second.href +  " doesn't exist!"));

			XMLOstream::Tag DataObject(xst, "dataObject", "", dataObjectAttribute.str());

			size_t dataObjectSize = File::size(_path + "/" + it->second.href);

			ostringstream byteStreamAttribute;
			byteStreamAttribute << "mimeType=\"" << it->second.mimeType << "\"";
			byteStreamAttribute << " size=\"" << dataObjectSize << "\"";

			XMLOstream::Tag ByteStream(xst, "byteStream", "", byteStreamAttribute.str());


			stringstream fileLocationAttribute;
			fileLocationAttribute << "locatorType=\"" << it->second.locatorType << "\"" ;
			fileLocationAttribute << " textInfo=\"" << it->second.textInfo << "\"";
			fileLocationAttribute << " href=\"" << it->second.href << "\"";
			XMLOstream::Tag FileLocation(xst, "fileLocation", "", fileLocationAttribute.str(), true);


			ostringstream checksumNameAttribute;
			checksumNameAttribute << "checksumName=\"" << it->second.checksumName << "\"" ;

			Md5Sum md5Checksum(_path + "/" + it->second.href);
			XMLOstream::Tag Checksum(xst, "checksum", md5Checksum.computeSignature(), checksumNameAttribute.str(), true);
		}
	}
}

bool SafeManifest::validateManifest(const string& manifestFilename, const string& xfduSchemaFilename, bool raiseException) {
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  ACS_COND_THROW(not File::exists(manifestFilename) , exSafeManifestException("Input manifestFile "+manifestFilename+" does not exists") )
  ACS_COND_THROW(not File::exists(xfduSchemaFilename) , exSafeManifestException("Input XFDU Schema "+xfduSchemaFilename+" does not exists") )
  SpawnProc proc("xmllint") ;
	unsigned short argcount = 0 ;
	proc.args()[argcount++] = WaitPid::ProcArg("--noout") ; // PRQA S 3081 4
	proc.args()[argcount++] = WaitPid::ProcArg("--schema") ;
	proc.args()[argcount++] = WaitPid::ProcArg(xfduSchemaFilename) ;
	proc.args()[argcount++] = WaitPid::ProcArg(manifestFilename) ;
  int outfiledes[2] ; // PRQA S 4403
  ACS_COND_THROW(pipe(outfiledes) != 0 ,exIOException("XMLValidator: Cannot open pipe")) ; // PRQA S 3081
  proc.setOutDescriptor(outfiledes[1]) ;
  proc.setErrDescriptor(outfiledes[1]) ;
	// outfiledes[0] will be closed by the ifdstream dtor
	File::ifdstream input(outfiledes[0]) ;
	proc.spawn() ;
	close(outfiledes[1]) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "process running: "<< boolalpha << proc.isRunning() << " pid is : " << proc.pid() ) ;
	do {
	   Timer::delay(31) ; // PRQA S 4400

	} while (proc.isRunning()) ;

  int ret = 0 ;
	bool ex = proc.exitCode(ret) ;
	int sig = 0 ;
	bool sg = proc.exitSignal(sig)  ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Process " << (ex ? "exited" : "running") << " exit code: " << ret // PRQA S 3380
				                << " Process " << (sg ? "exited" : "running") << " signal: " << sig) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "input good: " << boolalpha << input.good() << " "
				                << " eof: " << input.eof() << " "
				                << " bad: " << input.bad()) ;
  ostringstream out ;
  char c = 0;
  while(input.get(c)) { // PRQA S 3050, 4244
    out.put(c) ;
  }

  ACS_COND_THROW((ret != 0) && raiseException, XMLHandler::XMLParserError(out.str())) ; // PRQA S 3081
  return ret == 0 ;
}
_ACS_END_NAMESPACE
