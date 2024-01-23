/*
	Copyright 1995-2021, Exprivia SpA - DADF
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
	Revision 1.9  2018/03/01 14:57:13  tergem
	S3EVOL-142: productUnitType parameter added for L0 conf files

	Revision 1.8  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)

	Revision 1.7  2013/11/11 10:47:51  masdal
	dispositionMode handling added.

	Revision 1.6  2013/10/29 19:08:57  nicvac
	compilation warning processed.

	Revision 1.5  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.

	Revision 1.4  2012/07/27 10:14:15  damdec
	Exception raised if representation file tag is missing in the configuration file. Manifest file name passed as parameter. dataObject setting changed (some common configuration fields specialized for object.)

	Revision 1.3  2012/06/22 13:02:17  damdec
	Added the possibility to create a dummy representation data file enabling a flag.

	Revision 1.2  2012/04/17 15:44:07  damdec
	Loading of the configuration file moved here. getMetadataWrap method added.

	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter


*/
#ifndef _SafeManifest_H_
#define _SafeManifest_H_

#include <exException.h>
#include <rsResourceSet.h>
#include <SafeObjectDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)

class XMLOstream ;
class WrappedMetadata ;
/**
* \brief
* Main class for the handling of the SAFE manifest
**/

class SafeManifest  {  // PRQA S 2109
public:

	/** SafeManifest Exceptions */
	exDECLARE_EXCEPTION(exSafeManifestException, exException) ; // Base SafeManifest Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exSafeManifestCriticalException, exSafeManifestException) ; // Critical Exception. // PRQA S 2153


	/** Class constructor */
	SafeManifest(const std::string& productType, const std::string& version,  const std::string& path, const std::string & configFilename);
	/** Destructor */
	virtual ~SafeManifest() throw() ;

	/** return the XML schemas referenced by the data components */
	std::vector<std::string> getXmlSchemas() const;

	void setDataObject(const std::string& id, const std::string& filename, const bool & repDummy=false); // create dummy representation data files if repDummy==true
	void setMetadataWrap(const std::string& id, const WrappedMetadata& );

	void resetPath(const std::string& path) {_path = path;};
	/** 
	 * Allow to overwrite the configured dataObjectId with a new value
	 * \param id: _mapDataObject map key
	 * \param newValue: new value to be used to overwrite the dataObjectId
	 */
	void resetDataObject(const std::string& id, const std::string& newValue);
	const std::string &dispositionMode() const throw() { return _dispositionMode ; }
	const std::string &productUnitType() const throw() { return _productUnitType ; }

	MetadataWrapDescriptor & getMetadataWrap(const std::string& id);

	/* write the manifest file with the following sections :
	   - information package map (list of content units);
	   - metadataObjectSection (wrapped metadata + metadata referemce ) --> writing this section an existence check on the XML schemas is performed;
	   - dataObjectSection --> writing this section an existence check on the data files is performed: for each file the size and the md5sum are computed */
	virtual void writeManifest(const std::string & name="manifest.safe") ;

	/**
	 * \brief  Manifest validation method.
	 * \param Manifest file name
	 * \param Schema file name
	 * \param Raise Exception if validation fails FLAG.
	 * \retval TRUE if validation is successful, FALSE otherwise.
	 */
	static bool validateManifest(const std::string&, const std::string&, bool=false) ;

protected:
	virtual void writeInformationPackageMap(XMLOstream&);
	virtual void writeContentUnit(XMLOstream& xst, const DataObjectDescriptor& dataObj);
	virtual void writeMetadataSection(XMLOstream&);
	virtual void writeDataObjectSection(XMLOstream&);

private:
	SafeManifest(); // not implemented
	SafeManifest(const SafeManifest & ); // not implemented
	SafeManifest &operator=(const SafeManifest &); // not implemented

	void loadConfiguration();

	bool matchProductType() ;
	void loadRootContentUnit();
	void loadDataObjectConfiguration();
	void loadMetadataConfiguration();
	void loadSchemaConfiguration();

private:
	rsResourceSet _rsConf;
	std::string _productType;

	std::map<std::string, DataObjectDescriptor> 		_mapDataObject;
	std::map<std::string, MetadataWrapDescriptor> 		_mapMetadataWrap;
	std::map<std::string, MetadataReferenceDescriptor> 	_mapMetadataReference;
	std::map<std::string, MetadataObjectDescriptor> 	_mapMetadataObject;

	RootContentUnitDescriptor _rootContentUnit;

	std::string _safeNamespace;
	std::string _safePrefix;
	std::string _xfduNamespace;
	std::string _rootTagOtherAttributes;
	std::string _version;
	std::string _path;
	std::string _dispositionMode;
	std::string _productUnitType;
	
	static const std::string _defaultSafePrefix;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SafeManifest)

};



_ACS_END_NAMESPACE

#endif //_SafeManifest_H_
