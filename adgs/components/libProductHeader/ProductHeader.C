// PRQA S 1050 EOF
/*

	Copyright 2002-2019, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.7  2017/10/03 14:17:40  tergem
	format update as for COP V3.4 processors
	
	Revision 2.6  2017/03/08 15:06:22  enrcar
	EC:: Support for NetCDF products (at least, reading the attributes) added
	
	Revision 2.5  2016/05/10 20:46:38  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.4  2014/12/17 11:41:43  marfav
	Fixed management of FD closure
	
	Revision 2.3  2014/02/14 08:46:29  giucas
	New log macros adopted
	

*/

#include <DateTime.h>
#include <Netcdf4Utils.h>
#include <Netcdf4DataFile.h>
#include <ProductHeader.h>
#include <Filterables.h>
#include <prodHeaderV.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
	prodHeaderV version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProductHeader)



// -----------------------------------------------------------------
//
//
// default constructor : no initialization is done
//						 clients MUST call initXXX methods
//
// -----------------------------------------------------------------
ProductHeader::ProductHeader() :
	_mphRead(false),
	_sphRead(false),
	_dsdRead(false),
	_numberOfDSD(0),
	_totSize(0),	
	_mphData(),
	_sphData(),
	_dsdData(),
	_dsdSpareData(),
	_dsdTemplate(),
	_separator(),
	_xmlSchemaServerUrl(),
	_xmlMphData(),
	_xmlSphData(),
	_xmlMdhData(),
	_xmlFixedHeaderData(),
	_xmlDsdData(),	
	_xmlDsdTemplate()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ProductHeader::Id: " << Id )
}

// -----------------------------------------------------------------
//
// constructor used to automatically initialize all 
// the rsResourceSet argument MUST cointain all needed information
//						
//
// -----------------------------------------------------------------
ProductHeader::ProductHeader(const rsResourceSet& resource) :
	_mphRead(false),
	_sphRead(false),
	_dsdRead(false),
	_numberOfDSD(0),
	_totSize(0),
	_mphData(),
	_sphData(),
	_dsdData(),
	_dsdSpareData(),
	_dsdTemplate(),
	_separator(),
	_xmlSchemaServerUrl(),
	_xmlMphData(),
	_xmlSphData(),
	_xmlMdhData(),
	_xmlFixedHeaderData(),
	_xmlDsdData(),	
	_xmlDsdTemplate()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ProductHeader::Id: " << Id )
	initMPH(resource);
	initSPH(resource);
	initDSD(resource);
	initXmlMPH(resource);
	initXmlSPH(resource);
	initXmlMDH(resource);
	initXmlFixedHeader(resource);
	initXmlDSD(resource);
}


ProductHeader::~ProductHeader() throw()
{
}

//
// copy constructor
//
ProductHeader::ProductHeader(const ProductHeader & e) :
	_mphRead(e._mphRead),
	_sphRead(e._sphRead),
	_dsdRead(e._dsdRead),
	_numberOfDSD(e._numberOfDSD),
	_totSize(e._totSize),
	_mphData(e._mphData),
	_sphData(e._sphData),
	_dsdData(e._dsdData),
	_dsdSpareData(e._dsdSpareData),
	_dsdTemplate(e._dsdTemplate),
	_separator(e._separator),
	_xmlSchemaServerUrl(e._xmlSchemaServerUrl),
	_xmlMphData(e._xmlMphData),
	_xmlSphData(e._xmlSphData),
	_xmlMdhData(e._xmlMdhData),
	_xmlFixedHeaderData(e._xmlFixedHeaderData),
	_xmlDsdData(e._xmlDsdData),	
	_xmlDsdTemplate(e._xmlDsdTemplate)
{
}

//
// operator =
//
ProductHeader&  ProductHeader::operator=(const ProductHeader & e)
{
	if (this != &e) {
		_mphRead 			= e._mphRead;
		_sphRead 			= e._sphRead;
		_dsdRead 			= e._dsdRead;
		_numberOfDSD 		= e._numberOfDSD;
		_totSize 			= e._totSize;
		_mphData 			= e._mphData;
		_sphData 			= e._sphData;
		_dsdData 			= e._dsdData;
		_dsdSpareData		= e._dsdSpareData ;
		_dsdTemplate 		= e._dsdTemplate;
		_separator 			= e._separator;
		_xmlSchemaServerUrl	= e._xmlSchemaServerUrl ;
		_xmlMphData 		= e._xmlMphData;
		_xmlSphData 		= e._xmlSphData;
		_xmlMdhData 		= e._xmlMdhData;
		_xmlFixedHeaderData = e._xmlFixedHeaderData;
		_xmlDsdData			= e._xmlDsdData;
		_xmlDsdTemplate		= e._xmlDsdTemplate;
	}
	return *this ;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////// init methods ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

//
// initialize MPH (Main Product Header) part loading infos from a rsResourceSet object
//
void ProductHeader::initMPH(const rsResourceSet& mphResource, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if(_mphData.size())
	{
		ProductHeaderException e("Error: MPH already initialized. ");
		ACS_THROW(e);	
	}
	
	// extract separator
	mphResource.getValue(root+".setOperator",_separator);
	init(mphResource,root,_mphData);
}

//
// initialize SPH (Specific Product Header) part loading infos from a rsResourceSet object
//
void ProductHeader::initSPH(const rsResourceSet& sphResource, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if(_sphData.size())
	{
		ProductHeaderException e("Error: SPH already initialized. ");
		ACS_THROW(e);	
	}	
	init(sphResource,root,_sphData);
}

//
// initialize DSD (Data Set Descriptor) part loading infos from a rsResourceSet object
//
void ProductHeader::initDSD(const rsResourceSet& dsdResource, u_int numberOfDSD, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	for(unsigned int count=1; count <= numberOfDSD; count++)
	{
		vector<PHdata>  tempDSDdata;
		init(dsdResource,root,tempDSDdata);
		_dsdData.push_back(tempDSDdata);
		if (count == 1)
			_dsdTemplate = tempDSDdata;
	}	
}


//
// initialize Spare DSD
//
void ProductHeader::initSpareDSD(u_int numberOfSpare)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	string tmp(279u, ' ');
	tmp = tmp + '\n';

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"defined a spare line of size "<< tmp.size() )
		
	for(unsigned int count=1; count <= numberOfSpare; count++)
	{		
		_dsdSpareData.push_back(tmp);
	}
}

//
// initialize XML Header MPH part loading infos from a rsResourceSet object
//
void ProductHeader::initXmlMPH(const rsResourceSet& xmlMphResource, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if(_xmlMphData.size())
	{
		ProductHeaderException e("Error: XML MPH already initialized. ");
		ACS_THROW(e);	
	}
	unsigned int numberOfFields =0;
	xmlMphResource.getValue(root+".numberOfFields", numberOfFields);
	initXML(xmlMphResource,root,_xmlMphData, numberOfFields);
}

//
// initialize XML Header SPH part loading infos from a rsResourceSet object
//
void ProductHeader::initXmlSPH(const rsResourceSet& xmlSphResource, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if(_xmlSphData.size())
	{
		ProductHeaderException e("Error: XML SPH already initialized. ");
		ACS_THROW(e);	
	}
	unsigned int numberOfFields =0;
	xmlSphResource.getValue(root+".numberOfFields", numberOfFields);	
	initXML(xmlSphResource,root,_xmlSphData, numberOfFields);
}

//
// initialize XML Header MDH part loading infos from a rsResourceSet object
//
void ProductHeader::initXmlMDH(const rsResourceSet& xmlMdhResource, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if(_xmlMdhData.size())
	{
		ProductHeaderException e("Error: XML MDH already initialized. ");
		ACS_THROW(e);	
	}	
	unsigned int numberOfFields =0;
	xmlMdhResource.getValue(root+".numberOfFields", numberOfFields);
	initXML(xmlMdhResource,root,_xmlMdhData, numberOfFields);
}

//
// initialize XML Header Fixed Header part loading infos from a rsResourceSet object
//
void ProductHeader::initXmlFixedHeader(const rsResourceSet& xmlFEResource, string root)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if(_xmlFixedHeaderData.size())
	{
		ProductHeaderException e("Error: XML Fixed Header already initialized. ");
		ACS_THROW(e);	
	}
	unsigned int numberOfFields =0;
	xmlFEResource.getValue(root+".numberOfFields", numberOfFields);	
	initXML(xmlFEResource,root,_xmlFixedHeaderData, numberOfFields);
}

//
// initialize XML Header DSD part loading infos from a rsResourceSet object
//
void ProductHeader::initXmlDSD(const rsResourceSet& xmlDsdResource, unsigned int numberOfDSD, string root)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to initialize " << numberOfDSD << " XML DSDs" )

	for(unsigned int count=1; count <= numberOfDSD; count++)
	{
		vector<XmlPHData>  tempDSDdata;
		unsigned int numberOfFields =0;
		xmlDsdResource.getValue(root+".numberOfFields", numberOfFields);
		initXML(xmlDsdResource,root,tempDSDdata, numberOfFields);
		_xmlDsdData.push_back(tempDSDdata);
		if (count == 1) {
			_xmlDsdTemplate = tempDSDdata;
		}
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- read a Product Header file and load data in private structures
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::read(const string& fileName)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;


	if ( Netcdf4Utils::isHDF5File(fileName) )
	{
		readNetCDFParams (fileName) ; 
		
		_mphRead = (_mphData.size() > 0)  ;
		_sphRead = (_sphData.size() > 0)  ;
		_dsdRead = (_dsdData.size() > 0)  ;
		
	}
	else
	{
		int fd = -1;
		if ((fd = open(fileName.c_str(), O_RDONLY | O_LARGEFILE)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::read -> ERROR:: cannot open input file "<< fileName;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}

		ifstream*  fileStream = new File::ifdstream(fd);

		if(_mphData.size())
		{
			readMPH(*fileStream);
			_mphRead = true;
		}

		if(_sphData.size())
		{
			readSPH(*fileStream);
			_sphRead = true;
		}


		if(_dsdData.size())
		{
			readDSD(*fileStream);
			_dsdRead = true;
		}

		// closing file
		if(fileStream) {
        	// this will close also FD
			delete fileStream;
		}
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- read an XML file, build a resource and load data in private structures
// --------------------------------------------------------------------------------------------------------------------------

void ProductHeader::readXML(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << fileName )
	
	rsResourceSet xmlResource;
	
	try 
	{		
		XMLIstream  xmlIstream(fileName);
		
		xmlIstream >> xmlResource;

		if(isXmlFixedHeaderInitialized()) {
			loadXmlFixedHeaderData("Fixed_Header", xmlResource, _xmlFixedHeaderData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML Fixed Header section is skipped because not initialized" )
		}

		if(isXmlMPHInitialized()) {
			loadXmlMPHData("Variable_Header.MPH", xmlResource, _xmlMphData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML MPH section is skipped because not initialized" )
		}

		if(isXmlSPHInitialized()) {
			loadXmlSPHData("Variable_Header.SPH", xmlResource, _xmlSphData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML SPH section is skipped because not initialized")
		}
		
		if(isXmlMDHInitialized()) {
			loadXmlMDHData("Variable_Header.MDH", xmlResource, _xmlMdhData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML MDH section is skipped because not initialized")
		}
		
		if(isXmlDSDInitialized()) {
			loadXmlDSDData("Variable_Header.SPH.DSDs.List_of_DSDs", xmlResource, _xmlDsdData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML DSD section is skipped because not initialized")
		}
	}
	catch (exception& ex) 
	{
		ACS_THROW(ProductHeaderException("ProductHeader::readXML : reading file " + fileName + "." + ex.what()));
	}
}



void ProductHeader::readXML(const string& fileName, const int& offset)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << fileName << " starting at offset " << offset )
	
	rsResourceSet xmlResource;
	try 
	{	
		XMLIstream  xmlIstream(fileName);
		
		xmlIstream.seekg(offset);
		
		xmlIstream >> xmlResource;

		if(isXmlFixedHeaderInitialized()) {
			loadXmlFixedHeaderData("Fixed_Header", xmlResource, _xmlFixedHeaderData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML Fixed Header section is skipped because not initialized" )
		}

		if(isXmlMPHInitialized()) {
			loadXmlMPHData("Variable_Header.MPH", xmlResource, _xmlMphData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML MPH section is skipped because not initialized" )
		}

		if(isXmlSPHInitialized()) {
			loadXmlSPHData("Variable_Header.SPH", xmlResource, _xmlSphData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML SPH section is skipped because not initialized")
		}
		
		if(isXmlMDHInitialized()) {
			loadXmlMDHData("Variable_Header.MDH", xmlResource, _xmlMdhData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML MDH section is skipped because not initialized")
		}
		
		if(isXmlDSDInitialized()) {
			loadXmlDSDData("Variable_Header.SPH.DSDs.List_of_DSDs", xmlResource, _xmlDsdData, -1);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML DSD section is skipped because not initialized")
		}
	}
	catch (exception& ex) 
	{	
		ACS_THROW(ProductHeaderException("ProductHeader::readXML : reading file " + fileName + "." + ex.what()));
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting MPH Fields
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::setMPHField(const string& fieldName, const string& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with string value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try {
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}	
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setMPHField(const string& fieldName, const float& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with float value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try {	
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setMPHField(const string& fieldName, const int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with int value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setMPHField(const string& fieldName, const long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with long long int value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setMPHField(const string& fieldName, const unsigned long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with unsigned long long int value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setMPHField(const string& fieldName, const long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with long int value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setMPHField(const string& fieldName, const unsigned long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with unsigned long int value " << fieldValue )
	if (_mphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _mphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setMPHField : Error. MPH not initialized.");
		ACS_THROW(e);
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting SPH Fields
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::setSPHField(const string& fieldName, const string& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with string value " << fieldValue )
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setSPHField(const string& fieldName, const float& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with float value " 	<< fieldValue )
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setSPHField(const string& fieldName, const int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with int value " << fieldValue)
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setSPHField(const string& fieldName, const long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with long long int value " << fieldValue)
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setSPHField(const string& fieldName, const unsigned long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with unsigned long long int value " << fieldValue)
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setSPHField(const string& fieldName, const long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with long int value " << fieldValue)
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setSPHField(const string& fieldName, const unsigned long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with unsigned long int value " << fieldValue)
	if (_sphData.size() > 0)
	{
		try{	
			setField(fieldName, fieldValue, _sphData);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("Error in setting SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setSPHField : Error. SPH not initialized.");
		ACS_THROW(e);
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting DSD Fields
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::setDSDField(const string& fieldName, const string& fieldValue, u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with string value " << fieldValue << " for DSD n." << dsdNumber )
	if (_dsdData.size() >= dsdNumber)
	{
		try{	
			setField(fieldName, fieldValue, _dsdData[dsdNumber -1]);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setDSDField : Error in setting DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if (_dsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. No DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setDSDField(const string& fieldName, const int& fieldValue, u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with value " << fieldValue << " for DSD n." << dsdNumber )
	if (_dsdData.size() >= dsdNumber)
	{
		try{	
			setField(fieldName, fieldValue, _dsdData[dsdNumber -1]);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setDSDField : Error in setting DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if (_dsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. No DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setDSDField(const string& fieldName, const long long int& fieldValue, u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with value " << fieldValue << " for DSD n." << dsdNumber )
	if (_dsdData.size() >= dsdNumber)
	{
		try{	
			setField(fieldName, fieldValue, _dsdData[dsdNumber -1]);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setDSDField : Error in setting DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if (_dsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. No DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setDSDField(const string& fieldName, const unsigned long long int& fieldValue, u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with value " << fieldValue << " for DSD n." << dsdNumber )
	if (_dsdData.size() >= dsdNumber)
	{
		try{	
			setField(fieldName, fieldValue, _dsdData[dsdNumber -1]);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setDSDField : Error in setting DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if (_dsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. No DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setDSDField(const string& fieldName, const long int& fieldValue, u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with value " << fieldValue << " for DSD n." << dsdNumber )
	if (_dsdData.size() >= dsdNumber)
	{
		try{	
			setField(fieldName, fieldValue, _dsdData[dsdNumber -1]);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setDSDField : Error in setting DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if (_dsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. No DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setDSDField(const string& fieldName, const unsigned long int& fieldValue, u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName << " with value " << fieldValue << " for DSD n." << dsdNumber )
	if (_dsdData.size() >= dsdNumber)
	{
		try{	
			setField(fieldName, fieldValue, _dsdData[dsdNumber -1]);
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setDSDField : Error in setting DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if (_dsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. No DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setDSDField : Error. DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}



// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting XML MPH Fields
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::setXmlMPHField(const string& fieldName, const string& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{		
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMPHField(const string& fieldName, const int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlMPHField(const string& fieldName, const long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlMPHField(const string& fieldName, const unsigned long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlMPHField(const string& fieldName, const long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMPHField(const string& fieldName, const unsigned long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMPHField(const string& fieldName, const float& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMphData.size() > 0)
	{		
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMPHField : Error in setting XML MPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}		
		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMPHField : XML MPH not initialized.");
		ACS_THROW(e);
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting XML SPH Fields
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::setXmlSPHField(const string& fieldName, const string& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{		
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}		
		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlSPHField(const string& fieldName, const int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlSPHField(const string& fieldName, const long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlSPHField(const string& fieldName, const unsigned long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlSPHField(const string& fieldName, const long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlSPHField(const string& fieldName, const unsigned long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
		
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlSPHField(const string& fieldName, const float& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlSphData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlSphData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlSPHField : Error in setting XML SPH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlSPHField : XML SPH not initialized.");
		ACS_THROW(e);
	}
}


// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting XML MDH Fields
// --------------------------------------------------------------------------------------------------------------------------

void ProductHeader::setXmlMDHField(const string& fieldName, const string& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMDHField(const string& fieldName, const int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMDHField(const string& fieldName, const long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMDHField(const string& fieldName, const unsigned long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlMDHField(const string& fieldName, const long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlMDHField(const string& fieldName, const unsigned long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlMDHField(const string& fieldName, const float& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlMdhData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlMdhData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlMDHField : Error in setting XML MDH tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlMDHField : XML MDH not initialized.");
		ACS_THROW(e);
	}
}



// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting XML Fixed Header Fields
// --------------------------------------------------------------------------------------------------------------------------	
void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const string& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const unsigned long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const unsigned long long int& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlFixedHeaderField(const string& fieldName, const float& fieldValue, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " << fieldValue)
	if (_xmlFixedHeaderData.size() > 0)
	{	
		try {
			if (!setXmlField(fieldName, fieldValue, _xmlFixedHeaderData, fieldNumber))
			{
				ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : Unknown field name");
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : Error in setting XML Fixed Header tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	} 
	else
	{
		ProductHeaderException e("ProductHeader::setXmlFixedHeaderField : XML  Fixed Header not initialized.");
		ACS_THROW(e);
	}
}

// --------------------------------------------------------------------------------------------------------------------------
// ------------- Public methods for setting XML DSD Fields
// --------------------------------------------------------------------------------------------------------------------------
void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const string& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const int& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const float& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}


void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const long long int& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const unsigned long long int& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const long int& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}

void ProductHeader::setXmlDSDField(const string& fieldName, 
								   const unsigned long int& fieldValue, 
								   const unsigned int& fieldNumber, 
								   u_int dsdNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to set field " << fieldName  << " with value " << fieldValue << " for XML DSD n." << dsdNumber )
	if(_xmlDsdData.size() >= dsdNumber)
	{
		try 
		{
			if (!setXmlField(fieldName, fieldValue, _xmlDsdData[dsdNumber -1], fieldNumber))
			{
				ostringstream msg;
				msg << "ProductHeader::setXmlDSDField : Error in setting XML DSD[" << dsdNumber
					<< "] field '" << fieldName << "' with value '" << fieldValue 
					<< "' because of UNKNOWN FIELD NAME" << endl;
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		catch(ProductHeaderException& x)
		{
			ProductHeaderException e("ProductHeader::setXmlDSDField : Error in setting XML DSD tag " + fieldName + " : " + x.description());
			ACS_THROW(e);	
		}
	}
	else if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. No XML DSD initialized.");
		ACS_THROW(e);
	}
	else
	{
		ProductHeaderException e("ProductHeader::setXmlDSDField : Error. XML DSDs not correctly initialized.");
		ACS_THROW(e);
	}
}


/*
 **************************************************************************************************************************
 ************* get methods for retrieving the field values for MPH,SPH,DSD
 **************************************************************************************************************************
 */	

//
// get MPH (Main Product Header) field value
//
void ProductHeader::getMPHField(const string& fieldName, string& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.size() == 0)
			{
				fieldValue = _mphData[i].fieldValue;
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getMPHField : The value format for MPH field " + fieldName + " is not a string.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getMPHField(const string& fieldName, ostringstream& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			fieldValue << _mphData[i].fieldValue ;
			exit = true;
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue.str() )
}


void ProductHeader::getMPHField(const string& fieldName, float& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.find("f") != string::npos 
			    || _mphData[i].fieldValueFormat.find("E") != string::npos
				|| _mphData[i].fieldValueFormat.find("e") != string::npos
			   )
			{
				fieldValue = atof(_mphData[i].fieldValue.c_str());
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getMPHField : The value format for MPH field " + fieldName + " is not a float.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getMPHField(const string& fieldName, int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.find("d") != string::npos)
			{
				fieldValue = atoi(_mphData[i].fieldValue.c_str());
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getMPHField : The value format for MPH field " + fieldName + " is not an integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getMPHField(const string& fieldName, long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.find("d") != string::npos)
			{
				fieldValue = fromString<long int>(_mphData[i].fieldValue);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"returning value '" << fieldValue << "'")
				exit = true;
			}
			else
			{
				ostringstream msg;
				msg << "ProductHeader::getMPHField : The value format [" << _mphData[i].fieldValueFormat << "] for MPH field "
					<< fieldName << " is not recognized as INTEGER";
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getMPHField(const string& fieldName, unsigned long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.find("u") != string::npos)
			{
				fieldValue = fromString<unsigned long int>(_mphData[i].fieldValue);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"returning value '" << fieldValue << "'" )
				exit = true;
			}
			else
			{
				ostringstream msg;
				msg << "ProductHeader::getMPHField : The value format [" << _mphData[i].fieldValueFormat << "] for MPH field "
					<< fieldName << " is not recognized as unsigned long int";
				ProductHeaderException e(msg.str());
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


void ProductHeader::getMPHField(const string& fieldName, long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.find("Ld") != string::npos)
			{
				fieldValue = fromString<long long int>(_mphData[i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getMPHField : The value format for MPH field " + fieldName + " is not a long long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getMPHField(const string& fieldName, unsigned long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHField : MPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_mphData.size()) && !exit)
	{
		if (_mphData[i].fieldName == fieldName)
		{
			if (_mphData[i].fieldValueFormat.find("Lu") != string::npos)
			{
				fieldValue = fromString<unsigned long long int>(_mphData[i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getMPHField : The value format for MPH field " + fieldName + " is not an unsigned long long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getMPHField : Unknown MPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


//
// get SPH field value
//
void ProductHeader::getSPHField(const string& fieldName, string& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.size() == 0)
			{
				fieldValue = _sphData[i].fieldValue;
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not a string.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getSPHField(const string& fieldName, ostringstream& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			fieldValue << _sphData[i].fieldValue;
			exit = true;
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue.str() )
}


void ProductHeader::getSPHField(const string& fieldName, float& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.find("f") != string::npos 
				|| _sphData[i].fieldValueFormat.find("E") != string::npos
				|| _sphData[i].fieldValueFormat.find("e") != string::npos
			   )
			{
				fieldValue = atof(_sphData[i].fieldValue.c_str());
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not a float.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


void ProductHeader::getSPHField(const string& fieldName, int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.find("d") != string::npos)
			{
				fieldValue = atoi(_sphData[i].fieldValue.c_str());
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not an integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getSPHField(const string& fieldName, long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.find("d") != string::npos)
			{
				fieldValue = fromString<long int>(_sphData[i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not an integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getSPHField(const string& fieldName, unsigned long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.find("u") != string::npos)
			{
				fieldValue = fromString<long int>(_sphData[i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not an unsigned long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


void ProductHeader::getSPHField(const string& fieldName, long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.find("Ld") != string::npos)
			{
				fieldValue = fromString<long long int>(_sphData[i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not an integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getSPHField(const string& fieldName, unsigned long long int& fieldValue)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for field " << fieldName )
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHField : SPH not initialized yet");
		ACS_THROW(e);		
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_sphData.size()) && !exit)
	{
		if (_sphData[i].fieldName == fieldName)
		{
			if (_sphData[i].fieldValueFormat.find("Lu") != string::npos)
			{
				fieldValue = fromString<unsigned long long int>(_sphData[i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getSPHField : The value format for SPH field " + fieldName + " is not an unsigned long long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getSPHField : Unknown SPH field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


//
// get DSD field value
//
void ProductHeader::getDSDField(const string& fieldName, string& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			if (_dsdData[dsdNum-1][i].fieldValueFormat.size() == 0)
			{
				fieldValue = _dsdData[dsdNum-1][i].fieldValue;
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getDSDField : The value format for DSD field " + fieldName + " is not a string.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getDSDField(const string& fieldName, ostringstream& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			fieldValue << _dsdData[dsdNum-1][i].fieldValue;
			exit = true;
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue.str() )
}


void ProductHeader::getDSDField(const string& fieldName, int& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			if (_dsdData[dsdNum-1][i].fieldValueFormat.find("d") != string::npos)
			{
				fieldValue = atoi(_dsdData[dsdNum-1][i].fieldValue.c_str());
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getDSDField : The value format for DSD field " + fieldName + " is not an integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


void ProductHeader::getDSDField(const string& fieldName, long int& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			if (_dsdData[dsdNum-1][i].fieldValueFormat.find("d") != string::npos)
			{
				fieldValue = fromString<long int>(_dsdData[dsdNum-1][i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getDSDField : The value format for DSD field " + fieldName + " is not a long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getDSDField(const string& fieldName, unsigned long int& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			if (_dsdData[dsdNum-1][i].fieldValueFormat.find("u") != string::npos)
			{
				fieldValue = fromString<long int>(_dsdData[dsdNum-1][i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getDSDField : The value format for DSD field " + fieldName + " is not an unsigned long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


void ProductHeader::getDSDField(const string& fieldName, long long int& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			if (_dsdData[dsdNum-1][i].fieldValueFormat.find("Ld") != string::npos)
			{
				fieldValue = fromString<long long int>(_dsdData[dsdNum-1][i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getDSDField : The value format for DSD field " + fieldName + " is not a long long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}

void ProductHeader::getDSDField(const string& fieldName, unsigned long long int& fieldValue, u_int dsdNum)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to get value for DSD n." << dsdNum << " field " << fieldName )
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD not initialized yet");
		ACS_THROW(e);		
	}
	else if (_dsdData.size() < dsdNum)
	{
		ProductHeaderException e("ProductHeader::getDSDField : DSD specified does not exist.");
		ACS_THROW(e);
	}
	bool exit = false;
	unsigned int i=0;
	while( (i<_dsdData[dsdNum-1].size()) && !exit)
	{
		if (_dsdData[dsdNum-1][i].fieldName == fieldName)
		{
			if (_dsdData[dsdNum-1][i].fieldValueFormat.find("Lu") != string::npos)
			{
				fieldValue = fromString<unsigned long long int>(_dsdData[dsdNum-1][i].fieldValue);
				exit = true;
			}
			else
			{
				ProductHeaderException e("ProductHeader::getDSDField : The value format for DSD field " + fieldName + " is not an unsigned long long integer.");
				ACS_THROW(e);
			}
		}
		i++;
	}
	if (!exit)
	{
		ProductHeaderException e("ProductHeader::getDSDField : Unknown DSD field " + fieldName + ".");
		ACS_THROW(e);
	}		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got value " << fieldValue )
}


/*
 **************************************************************************************************************************
 ************* get methods for retrieving the field names list for MPH,SPH,DSD, XML MPH, XML SPH, XML MDH, XML Fixed Header
 **************************************************************************************************************************
 */	

//
// get MPH (Main Product Header) fields list
//
vector<string> ProductHeader::getMPHFieldsList()
{
	if(!_mphData.size())
	{
		ProductHeaderException e("ProductHeader::getMPHFieldsList : MPH not initialized yet");
		ACS_THROW(e);		
	}
	vector<string>  temp;
	for(unsigned int count=0; count < _mphData.size(); count++)
	{
		if(_mphData[count].fieldName.size())
			temp.push_back(_mphData[count].fieldName);
	}	
	return temp;
}



//
// get SPH (Specific Product Header) fields list
//
vector<string> ProductHeader::getSPHFieldsList()
{
	if(!_sphData.size())
	{
		ProductHeaderException e("ProductHeader::getSPHFieldsList : SPH not initizialized yet");
		ACS_THROW(e);		
	}
	vector<string>  temp;
	for(unsigned int count=0; count < _sphData.size(); count++)
	{
		if(_sphData[count].fieldName.size())
			temp.push_back(_sphData[count].fieldName);
	}	
	return temp;
}

//
// get DSD (Data Set Descriptor) fields list
//
vector<string> ProductHeader::getDSDFieldsList()
{
	if(!_dsdData.size())
	{
		ProductHeaderException e("ProductHeader::getDSDFieldsList : DSD not initialized yet");
		ACS_THROW(e);		
	}
	vector<string>  temp;
	
	ostringstream num;
	num << _dsdData.size();
	string tempstring(num.str());
	temp.push_back("number of DSD = " +  tempstring);
	
	for(unsigned int count=0; count < _dsdData[0].size(); count++)
	{
		if(_dsdData[0][count].fieldName.size())
			temp.push_back(_dsdData[0][count].fieldName);
	}	
	return temp;
}


void ProductHeader::retrieveFieldNames(vector<XmlPHData>& data, vector<string>& names)
{
	for (u_int i=0; i < data.size(); i++)
	{		
		if (data[i].tagType == "SIMPLE") {
			names.push_back(data[i].data.fieldName);
		}
		else { //COMPOSITE 
			retrieveFieldNames(data[i].children, names);
		}
	}
}


//
// get XML MPH fields list
//
vector<string> ProductHeader::getXmlMPHFieldsList()
{
	if(!_xmlMphData.size())
	{
		ProductHeaderException e("ProductHeader::getXmlMPHFieldsList : XML MPH not initialized yet");
		ACS_THROW(e);		
	}
	vector<string> result;	
	retrieveFieldNames(_xmlMphData, result);
	return result;	
}



//
// get XML SPH fields list
//
vector<string> ProductHeader::getXmlSPHFieldsList()
{
	if(!_xmlSphData.size())
	{
		ProductHeaderException e("ProductHeader::getXmlSPHFieldsList : XML SPH not initialized yet");
		ACS_THROW(e);		
	}
	vector<string> result;	
	retrieveFieldNames(_xmlSphData, result);
	return result;
}


//
// get XML MDH fields list
//
vector<string> ProductHeader::getXmlMDHFieldsList()
{
	if(!_xmlMdhData.size())
	{
		ProductHeaderException e("ProductHeader::getXmlMDHFieldsList : XML MDH not initialized yet");
		ACS_THROW(e);		
	}
	vector<string> result;	
	retrieveFieldNames(_xmlMdhData, result);
	return result;
}

//
// get XML Fixed Header fields list
//
vector<string> ProductHeader::getXmlFixedHeaderFieldsList()
{
	if(!_xmlFixedHeaderData.size())
	{
		ProductHeaderException e("ProductHeader::getXmlFixedHeaderFieldsList : XML Fixed Header not initialized yet");
		ACS_THROW(e);		
	}
	vector<string> result;	
	retrieveFieldNames(_xmlFixedHeaderData, result);
	return result;	
}	

//
// get the number of DSD initialized
//
int ProductHeader::getNumOfDSD()
{
	if(_dsdData.size() > 0) {
		return _dsdData.size();
	}
	else 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"*** WARNING *** : no DSD initialized!!!" )
		return 0;
	}	
}	

//
// This method writes the Product Header on file "asciiFileName" if "binFileName" is not specified in input, 
// otherwise it writes the Product Header on "asciiFileName" and then appends to it the content of "binFileName".
// A ProductHeaderException exception is thrown if "asciiFileName" exists.
//
void ProductHeader::connectBinToPH(const string& asciiFileName, string binFileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << asciiFileName )
	
	try {
		checkDataInizialization();
	}
	catch (ProductHeaderException& ex)
	{
		ProductHeaderException e("ProductHeader::connectBinToPH : Error writing file " + asciiFileName + "." + ex.description());
		ACS_THROW(e);
	}
	
	//if file exists exit	
	if(File::exists(asciiFileName))
	{
		ProductHeaderException e("ProductHeader::connectBinToPH : Error cannot overwrite. File " + asciiFileName + " exists");
		ACS_THROW(e);	
	}
	
	//open a file descriptor with O_LARGEFILE open mode
	int fd = -1;
	if ((fd = open(asciiFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, 0666)) < 0)
	{
		ostringstream msg;
		msg << "ProductHeader::connectBinToPH -> ERROR:: cannot open output file "<< asciiFileName;
		exFileOpenException e(msg.str(), errno);
		ACS_THROW(e);
	}
	
	//build a ofstream from the file descriptor
	ofstream*  asciiFile = new File::ofdstream(fd);
		
	// writing MPH part of ASCII file
	write(_mphData, *asciiFile);
	
	// writing SPH part of ASCII file
	write(_sphData, *asciiFile);
	
	// writing DSD parts of ASCII file in DSD loading order
	for(unsigned int count =0; count < _dsdData.size(); count++)
		write(_dsdData[count], *asciiFile);
	
	// writing Spare DSD if any
	for(unsigned int i =0; i < _dsdSpareData.size(); i++)
		(*asciiFile) << _dsdSpareData[i];	

	// check if in input there is a binary file
	if (binFileName.size())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"it has been specified binary file "  << binFileName )

		if(!File::exists(binFileName))
		{
			ProductHeaderException e("ProductHeader::connectBinToPH : Error Binary File in input " + binFileName + " does not exists");
			ACS_THROW(e);	
		}

		int fdBin = -1;
		if ((fdBin = open(binFileName.c_str(), O_RDONLY | O_LARGEFILE)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::connectBinToPH -> ERROR:: cannot open output file "<< binFileName;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		} 
		
		ifstream*  binFile = new File::ifdstream(fdBin);
				
		// alloc memory
		char* temp;
		temp = new char[1024];
		
		// copying binary file on the ASCII file code
		while(!binFile->fail())
		{
			binFile->read(temp, 1024);
			u_int streamSize = binFile->gcount();
			asciiFile->write(temp, streamSize); 
		}
		
		if (!binFile->eof())
		{
			// free memory
			if (temp)
			{
		 		delete [] temp;
				temp = 0;
			}
			if(binFile)
            {
                // This will close the fdBin file
				delete binFile;
            }

			ProductHeaderException e("ProductHeader::connectBinToPH : Error in reading binary file " + binFileName);
			ACS_THROW(e);		
		}
		
		// free memory
		if (temp)
		{
		 	delete [] temp;
			temp = 0;
		}
		if(binFile) {
            //this will close fdBin file
			delete binFile;
		}
	}
	
	// close file
	if(asciiFile) {
        // this will close fd file
		delete asciiFile;
	}
}

//
// This method writes the Product Header on the head of the binary file "binFileName".
// "binFileName" can be an empty file.
// overwritePH method does not make any check about free space on the head of file.
// If the file does not exists it is created
void ProductHeader::overwritePH(const string& binFileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << binFileName )
	
	try {
		checkDataInizialization();
	}
	catch (ProductHeaderException& ex)
	{
		ProductHeaderException e("ProductHeader::overwritePH : Error writing file " + binFileName + "." + ex.description());
		ACS_THROW(e);
	}

	int fd = -1;
	if(!(File::exists(binFileName.c_str())))
	{
		// File doesn't exist... create it!
		if ((fd = open(binFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, 0666)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::overwritePH -> ERROR:: cannot open output file "<< binFileName;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}
	}
	else //File in input exists
	{
		if ((fd = open(binFileName.c_str(), O_WRONLY | O_LARGEFILE, 0666)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::overwritePH -> ERROR:: cannot open output file "<< binFileName;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}
	}
	
	//build a ofstream from the file descriptor
	ofstream*  asciiAndBinFile = new File::ofdstream(fd);

	// setting position of first byte in stream file
	asciiAndBinFile->seekp(0, ios::beg);
	
	// writing MPH part of ASCII file
	write(_mphData, *asciiAndBinFile);
	
	// writing SPH part of ASCII file
	write(_sphData, *asciiAndBinFile);
	
	// writing DSD parts of ASCII file in DSD loading order
	for(unsigned int count =0; count < _dsdData.size(); count++) {
		write(_dsdData[count], *asciiAndBinFile);
	}
		
	// writing Spare DSD if any
	for(unsigned int i =0; i < _dsdSpareData.size(); i++) {
		(*asciiAndBinFile) << _dsdSpareData[i];
	}

	// close file
	if(asciiAndBinFile) {
        // this will close also fd
		delete asciiAndBinFile;
	}
}

// 
// Write the Product Header on the head of the binary file "binFileName"
// This file must be a not empty binary file.
// This method expects there is enough space left on the head of the binary file for writing the Product Header
// information. If there's not space it is thrown a ProductHeaderException.
// The MPH fields TOT_SIZE and DSD_SIZE value must be set otherwise an exception is thrown.
//
void ProductHeader::writePH(const string& binFileName)
{
	ACS_LOG_ERROR("ProductHeader::writePH -> The use of this method is DEPRECATED. Use ProductHeader::overwritePH instead")
}

//
// This method is used to write the XML Product Header information into xmlFile passed in input.
// If this file exists it is thrown an exception otherwise the file is created and XML generated.
//
void ProductHeader::writeXmlPH(const string& xmlFile, string const& encoding)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << xmlFile )
	
	if(File::exists(xmlFile))
	{
		ProductHeaderException e("ProductHeader::writeXmlPH : Error cannot overwrite. File " + xmlFile + " exists");
		ACS_THROW(e);	
	}

	//open a file descriptor with O_LARGEFILE open mode
	int fd = -1;
	if ((fd = open(xmlFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, 0666)) < 0)
	{
		ostringstream msg;
		msg << "ProductHeader::writeXmlPH -> ERROR:: cannot open output file "<< xmlFile;
		exFileOpenException e(msg.str(), errno);
		ACS_THROW(e);
	}
	
	//build a ofstream from the file descriptor
	ofstream*  asciiFile = new File::ofdstream(fd);	
	
	writeXML(*asciiFile,encoding);
	
	if(asciiFile) {
        // this will close also the fd file 
		delete asciiFile;
	}

}

//if the file DOES NOT EXISTS it is created
void ProductHeader::overwriteXmlPH(const string& xmlFile, int offset, string const& encoding)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << xmlFile << " at offset "<< offset )

	//open a file descriptor with O_LARGEFILE open mode
	int fd = -1;
	
	if(!(File::exists(xmlFile.c_str())))
	{
		// File doesn't exist... create it!
		if ((fd = open(xmlFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, 0666)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::overwriteXmlPH -> ERROR:: cannot open output file "<< xmlFile;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}

	}
	else //File in input exists
	{
		if ((fd = open(xmlFile.c_str(), O_WRONLY | O_LARGEFILE, 0666)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::overwriteXmlPH -> ERROR:: cannot open output file "<< xmlFile;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}
	}

	//build a ofstream from the file descriptor
	ofstream*  asciiFile = new File::ofdstream(fd);	

	if (offset > 0) {
		asciiFile->seekp(offset);
	}
	
	writeXML(*asciiFile,encoding);
	
	if(asciiFile) {
        // this will close the file
		delete asciiFile;
	}
}


//
// This method is used to write the XML Product Header information at the end of binFile passed in input.
// If the file does't exist it is created.
//
void ProductHeader::connectXmlToBin(const string& binFile, string const& encoding)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for file " << binFile )
	
	//open a file descriptor with O_LARGEFILE open mode
	int fd = -1;
	
	if(!(File::exists(binFile.c_str())))
	{
		// File doesn't exist... create it!
		if ((fd = open(binFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, 0666)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::connectXmlToBin -> ERROR:: cannot open file "<< binFile;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}

	}
	else //File in input exists
	{
		if ((fd = open(binFile.c_str(), O_WRONLY | O_LARGEFILE, 0666)) < 0)
		{
			ostringstream msg;
			msg << "ProductHeader::connectXmlToBin -> ERROR:: cannot open file "<< binFile;
			exFileOpenException e(msg.str(), errno);
			ACS_THROW(e);
		}
	}

	//build a ofstream from the file descriptor
	ofstream*  streamFile = new File::ofdstream(fd);	

	// setting position of last byte in stream file
	streamFile->seekp(0, ios::end);
	
	writeXML(*streamFile,encoding);
	
	if(streamFile) {
        // this will close the file
		delete streamFile;
	}

}

//
// copy to object "target" elements specified in toCopy
//
void ProductHeader::copy(ProductHeader & target, const vector<int>& toCopy)
{
	for(unsigned int i=0; i < toCopy.size() ; i++)
	{
		//element to copy is MPH
		if(toCopy[i] == 0)
		{
			target._mphData = _mphData;
			target._separator = _separator;
		}
		//element to copy is SPH
		else if(toCopy[i] == 1)
			target._sphData = _sphData;
		//element to copy is a DSD
		else if ((toCopy[i]-2) < static_cast<int>(_dsdData.size()))
				target._dsdData.push_back(_dsdData[toCopy[i]-2]);
			 else 
			 {
				ostringstream elemStream;
				elemStream << toCopy[i];
				string tempstring(elemStream.str());
				ProductHeaderException e(" ProductHeader::copy: The element " + tempstring + " is not compatible with copy specification.");
				ACS_THROW(e);
			 }
		
	}
	
	target._xmlMphData = _xmlMphData;
	target._xmlSphData = _xmlSphData;
	target._xmlMdhData = _xmlMdhData;
	target._xmlFixedHeaderData = _xmlFixedHeaderData;
}



//
// write part of Product Header ASCII file
//
void ProductHeader::write(const vector<PHdata>& data, ofstream& asciiFile)
{
	string asciiLine = "";
	
	for (u_int count =0; count < data.size(); count++)
	{
		string effectiveFieldValue = "";
		
		// select spare fields
		if (!data[count].fieldName.size())
		{
			effectiveFieldValue = string(data[count].fieldValueSize , ' ');
			asciiLine = effectiveFieldValue;	
		}
		else
		{
			if (data[count].fieldValue.size() == data[count].fieldValueSize)
				effectiveFieldValue = data[count].fieldValue;
			else
				effectiveFieldValue = data[count].fieldValue + string(data[count].fieldValueSize - data[count].fieldValue.size(), ' ');

			asciiLine = data[count].fieldName + _separator 
					+ data[count].quotationMark + effectiveFieldValue
					+ data[count].quotationMark + data[count].unitsFormat;
		}
		
		asciiFile << asciiLine << endl;	
	}
}


void ProductHeader::writeXMLData(vector<XmlPHData>& xmlData, XMLOstream& xmlOstr)
{
	for (u_int i=0; i < xmlData.size(); i++)
	{
		if (xmlData[i].tagType == "SIMPLE")
		{
			string name = xmlData[i].data.fieldName;
			string value = xmlData[i].data.fieldValue;
			string unit = xmlData[i].data.unitsFormat;
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"tagType=" << xmlData[i].tagType << " name = " << name << " value = " << value )
			
			string attr="";
			if (unit != "") {
				attr = " unit=\"" + unit + "\"";
			}
			XMLOstream::Tag unitTag(xmlOstr, name, value, attr, true);
		}
		else // COMPOSITE
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"tagType=" << xmlData[i].tagType )
			
			string attrVal="";			
			if (xmlData[i].tagName.find("List_of_") != string::npos)
			{
				ostringstream num;
				num << xmlData[i].childrenNumber;				
				attrVal = " count=\"" + num.str() + "\"";
			}
			XMLOstream::Tag elem(xmlOstr, xmlData[i].tagName, "", attrVal);
			writeXMLData(xmlData[i].children, xmlOstr);
			elem.close();
		}
	}
}


//
// build XML file
//
void ProductHeader::writeXML(ofstream& headerFile, string const & encoding)
{
	string serverUrlAttribute="";
	if(_xmlSchemaServerUrl != "") {
		serverUrlAttribute = "Schema_Server_Url=\""+_xmlSchemaServerUrl+"\"";
	}
	
	XMLOstream xmlOstr(headerFile,"Earth_Explorer_Header", serverUrlAttribute,"",encoding);
	
	////////////////////
	//add Fixed Header
	////////////////////
	if(isXmlFixedHeaderInitialized())
	{
		XMLOstream::Tag fixedHeader(xmlOstr, "Fixed_Header");
		writeXMLData(_xmlFixedHeaderData, xmlOstr);
		fixedHeader.close();
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML Fixed Header skipped because not initialized")
	}
	
	
	//////////////////////////////////////////////////////////////////
	//add Variable Header section if MPH or SPH or MDH is initialized
	//////////////////////////////////////////////////////////////////
	if(isXmlMPHInitialized() || isXmlSPHInitialized() || isXmlMDHInitialized())
	{
		XMLOstream::Tag variableHeader(xmlOstr, "Variable_Header");
		
		//////////////////////////////////////////////////////
		//write MPH section of XML Header File if initialized
		//////////////////////////////////////////////////////
		if(isXmlMPHInitialized())
		{
			XMLOstream::Tag mph(xmlOstr, "MPH");
			writeXMLData(_xmlMphData, xmlOstr);
			mph.close();
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML MPH skipped because not initialized")
		}
	
		//////////////////////////////////////////////////////	
		//write SPH section of XML Header File if initialized
		//////////////////////////////////////////////////////
		if(isXmlSPHInitialized())
		{
			XMLOstream::Tag sph(xmlOstr, "SPH");
			writeXMLData(_xmlSphData, xmlOstr);			
			//The DSD section is written only if SPH is initialized	
			if(isXmlDSDInitialized())
			{
				XMLOstream::Tag dsds(xmlOstr, "DSDs");
				ostringstream attr;
				attr << "count=\"" << _xmlDsdData.size() << "\"";
				XMLOstream::Tag listdsd(xmlOstr, "List_of_DSDs","",attr.str());
				for(unsigned int i=0; i < _xmlDsdData.size(); i++)
				{
					XMLOstream::Tag datadescriptor(xmlOstr, "Data_Set_Descriptor");
					writeXMLData(_xmlDsdData[i], xmlOstr);
					datadescriptor.close();
				}
				listdsd.close();
				dsds.close();
			}
			else
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML DSD section is skipped because not initialized")
			}
			//close SPH
			sph.close();
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML SPH skipped because not initialized. The DSD section is skipped too.")
		}
		//////////////////////////////////////////////////////
		//write MDH section of XML Header File if initialized
		//////////////////////////////////////////////////////
		if(isXmlMDHInitialized())
		{
			XMLOstream::Tag mdh(xmlOstr, "MDH");
			writeXMLData(_xmlMdhData, xmlOstr);
			mdh.close();
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML MDH section is skipped because not initialized")
		}
	
		//close Variable_Header TAG
		variableHeader.close();
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML Variable Header skipped because MPH or SPH or MDH not initialized")
	}
	
	//Earth_Explorer_Header TAG is automatically closed
}




//
// read NetCDF Params
//
void ProductHeader::readNetCDFParams(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ProductHeader::readNetcdfParams  Method called. Filename: \"" + fileName + " \"")

	if ( !File::exists(fileName) || !Netcdf4Utils::isHDF5File(fileName) )
	{
		ostringstream msg; msg << "ProductHeader::readNetcdfParams  Invalid file format: \"" << fileName << "\"" ;
		ProductHeaderException e(msg.str());
		ACS_THROW(e);
	}
			
	Netcdf4DataFile file(fileName, NcFile::read);

	const multimap<string, NcGroupAtt> atts ( file.getAtts() ) ;


	for (multimap<string, NcGroupAtt>::const_iterator i=atts.begin(); i!=atts.end() ; i++)
	{
		const string attr_name (i->first) ;
		const nc_type type ( (i->second).getType().getId() ) ;
		string dbl_attr_name ;
		bool is_time (false) ;
		bool is_string ( type == NcType::nc_CHAR) ;	// Can be forced to remove or add a string ;

		/**
			A mapping between the NetCDF attributes and the the legacy DBL attributes is required.
			This allows the original code, specific for legacy DBL, to work seamlessly with the new NetCDF attributes
			
			Structure of the dictionary:
			- INPUT: attr_name (the new netcdf attribute)
			- OUTPUT: dbl_attr_name (the name of the legacy DBL attribute)
			
			The rules of the mapping are the following:
			- If an attribute DID NOT CHANGE NAME, it does not need to be inserted.
			(A default-case will turn the attribute UPPERCASE, since all the dbl attributes were uppercase)
			
			- If an attribute CHANGED NAME it needs to be inserted (e.g. NetCDF::abs_orbit_number -> DBL::ABS_ORBIT)
			
			- If an attribute is a time-value it needs to be ALWAYS inserted in the map, in order to set a boolean: is_time = true 
			  (the reason is that times need a conversion between the new format and the legacy one).
			  E.G. NetCDF:: "UTC=2010-11-09T01:34:32.199266" or NetCDF:: "TAI=2010-11-09T01:34:32.199266"
				   ->  DBL:: "09-NOV-2010 01:34:32.199266"
			  Note that TAI times don't need the TAI->UTC conversion at this stage, it will be performed later

			
			Example of the code to implement the dictionary :
			
			... else if ( StringUtils::equalsNoCase(attr_name,  "first_meas_time" ) )  [ NetCDF name ] 
			{ 
				is_time = true ; dbl_attr_name = "START_RECORD_TAI_TIME" ;   [ Legacy DBL name ] 
			}
			...
			else
			{ 
				dbl_attr_name = StringUtils::uppercase(attr_name) ; 
			}
		*/

		/*------------------------------------------------------------------------
		--------------------------------------------------------------------------
		-----------------------  L1-L2 SHARED DICTIONARY  ------------------------
		--------------------------------------------------------------------------
		------------------------------------------------------------------------*/
		if ( StringUtils::equalsNoCase(attr_name,  "abs_orbit_number" ) )
		{ 
			dbl_attr_name = "ABS_ORBIT" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "cycle_number" ) )
		{ 
			dbl_attr_name = "CYCLE" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "delta_ut1" ) )
		{ 
			dbl_attr_name = "DELTA_UT1";  is_string = false ; /* In netcdf the attribute is a string, but in DBL it is not */
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "equator_cross_time" ) )
		{ 
			is_time = true ; dbl_attr_name = "EQUATOR_CROSS_TIME_UTC" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "equator_cross_long" ) )
		{ 
			dbl_attr_name = "EQUATOR_CROSS_LONG" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "ascending_flag" ) )
		{ 
			dbl_attr_name = "ACENDING_FLAG" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "FIRST_RECORD_LAT" ) )
		{ 
			dbl_attr_name = "START_LAT" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "FIRST_RECORD_LON" ) )
		{ 
			dbl_attr_name = "START_LONG" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "SENSING_START" ) )
		{ 
			is_time = true ; dbl_attr_name = "SENSING_START" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "LAST_RECORD_LAT" ) )
		{ 
			dbl_attr_name = "STOP_LAT" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "LAST_RECORD_LON" ) )
		{ 
			dbl_attr_name = "STOP_LONG" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "SENSING_STOP" ) )
		{ 
			is_time = true ; dbl_attr_name = "SENSING_STOP" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "LEAP_UTC" ) )
		{ 
			is_time = true ;  dbl_attr_name = "LEAP_UTC" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "l0_gaps_num" ) )
		{ 
			dbl_attr_name = "L0_GAPS_NUM" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "processing_centre" ) )
		{ 
			dbl_attr_name = "PROC_CENTER";  
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "processing_stage" ) )
		{ 
			dbl_attr_name = "PROC_STAGE" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "acquisition_station" ) )
		{ 
			dbl_attr_name = "ACQUISITION_STATION" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "PROC_TIME" ) )
		{ 
			is_time = true ;  dbl_attr_name = "PROC_TIME" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "product_name" ) )
		{ 
			dbl_attr_name = "PRODUCT" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "REFERENCE_DOCUMENT" ) )
		{ 
			dbl_attr_name = "REF_DOC" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "REL_ORBIT_NUMBER" ) )
		{ 
			dbl_attr_name = "REL_ORBIT" ; 
		}
		else if ( (StringUtils::equalsNoCase(attr_name,  "REL_TIME_ACS_NODE_START" ))||(StringUtils::equalsNoCase(attr_name,  "REL_TIME_ASC_NODE_START" )) )
		{ 
			dbl_attr_name = "REL_TIME_ASC_NODE_START" ; 
		}
		else if ( (StringUtils::equalsNoCase(attr_name,  "REL_TIME_ACS_NODE_STOP" ))||(StringUtils::equalsNoCase(attr_name,  "REL_TIME_ASC_NODE_STOP" )) )
		{ 
			dbl_attr_name = "REL_TIME_ASC_NODE_STOP" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "software_version" ) )
		{ 
			dbl_attr_name = "SOFTWARE_VER" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "first_record_time" ) )
		{ 
			is_time = true ; dbl_attr_name = "START_RECORD_TAI_TIME" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "STATE_VECTOR_TIME" ) )
		{ 
			is_time = true ; dbl_attr_name = "STATE_VECTOR_TIME" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "last_record_time" ) )
		{ 
			is_time = true ; dbl_attr_name = "STOP_RECORD_TAI_TIME" ; 
		}
/* NOTE: NetCDF::creation_time is unhandled */
		/*------------------------------------------------------------------------
		--------------------------------------------------------------------------
		------------------------  L1-SPECIFIC DICTIONARY  ------------------------
		--------------------------------------------------------------------------
		------------------------------------------------------------------------*/
		else if ( StringUtils::equalsNoCase(attr_name,  "l1b_prod_status" ) )
		{ 
			dbl_attr_name = "L1B_PROD_STATUS" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "l1b_proc_flag" ) )
		{ 
			dbl_attr_name = "L1B_PROC_FLAG" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "l1b_processing_quality" ) )
		{ 
			dbl_attr_name = "L1B_PROCESSING_QUALITY" ; 
		}
		/*------------------------------------------------------------------------
		--------------------------------------------------------------------------
		------------------------  L2-SPECIFIC DICTIONARY  ------------------------
		--------------------------------------------------------------------------
		------------------------------------------------------------------------*/
		else if ( StringUtils::equalsNoCase(attr_name,  "l2_prod_status" ) )
		{ 
			dbl_attr_name = "L2_PROD_STATUS" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "l2_proc_flag" ) )
		{ 
			dbl_attr_name = "L2_PROC_FLAG" ; 
		}
		else if ( StringUtils::equalsNoCase(attr_name,  "l2_processing_quality" ) )
		{ 
			dbl_attr_name = "L2_PROCESSING_QUALITY" ; 
		}
		/*------------------------------------------------------------------------
		--------------------------------------------------------------------------
		--------------------------------  DEFAULT  -------------------------------
		--------------------------------------------------------------------------
		------------------------------------------------------------------------*/
		else
		{ 
			dbl_attr_name = StringUtils::uppercase(attr_name) ; 	// Just turn uppercase
		}
	
		
		// Verify if the attribute is MPH, SPH or UNHANDLED
		bool add_to_mph (false), add_to_sph (false) ;
		for (size_t j=0; j<_mphData.size(); j++) { if (_mphData.at(j).fieldName == dbl_attr_name) { add_to_mph = true ; break ; } }
		for (size_t j=0; j<_sphData.size(); j++) { if (_sphData.at(j).fieldName == dbl_attr_name) { add_to_sph = true ; break ; } }

#if(0)
// DUMP UNHANDLED ATTRIBUTES
if ((add_to_mph == false) && (add_to_sph == false)) { excerr << "FIXME UNHANDLED ATTRIBUTE: !" << dbl_attr_name << "!" << endl ; }
//for (size_t j=0; j<_mphData.size(); j++) {excerr <<"M: " <<  _mphData.at(j).fieldName << endl ;} ; for (size_t j=0; j<_sphData.size(); j++) {excerr <<"S: " <<  _sphData.at(j).fieldName << endl ;}
#endif

		// Extract the value of the attribute from the NetCDF
		ostringstream o_v ;
		switch (type)
		{		 
			case (NcType::nc_SHORT) :
			{ short int v ;				(i->second).getValues(&v) ; o_v << v ; break ; }
			case (NcType::nc_USHORT) :
			{ unsigned short int v ;	(i->second).getValues(&v) ; o_v << v ; break ; }
			case (NcType::nc_INT) :
			{ int v ;					(i->second).getValues(&v) ; o_v << v ; break ; }
			case (NcType::nc_UINT) :
			{ unsigned int v ;			(i->second).getValues(&v) ; o_v << v ; break ; }	
			case (NcType::nc_INT64) :
			{ long long int v ;			(i->second).getValues(&v) ; o_v << v ; break ; }
			case (NcType::nc_UINT64) :
			{ unsigned long long int v ;(i->second).getValues(&v) ; o_v << v ; break ; }	
			case (NcType::nc_FLOAT) :
			{ float v ;					(i->second).getValues(&v) ; o_v << v ; break ; }
			case (NcType::nc_DOUBLE) :
			{ double v ;				(i->second).getValues(&v) ; o_v << v ; break ; }
			case (NcType::nc_CHAR) :
			{ string v ;				(i->second).getValues(v)  ; o_v << v ; break ; }
			default:
			{}
		}		

		string attr_value ( o_v.str() ) ;


		// Convert the PROC_STAGE
		if (dbl_attr_name == "PROC_STAGE")
		{
			if ( StringUtils::equalsNoCase(attr_value,  "OFFL" ) )
			{ attr_value = "O" ; }
			else if ( StringUtils::equalsNoCase(attr_value,  "TEST" ) )
			{ attr_value = "T" ; }
			else if ( StringUtils::equalsNoCase(attr_value,  "RPRO" ) )
			{ attr_value = "R" ; }
			else if ( StringUtils::equalsNoCase(attr_value,  "NRT_" ) )
			{ attr_value = "N" ; }
			else
			{ attr_value = "X" ; }
		}

		// Convert the VECTOR_SOURCE
		if (dbl_attr_name == "VECTOR_SOURCE")
		{
			if (StringUtils::equalsNoCase(attr_value, "fos predicted"))
			{ attr_value = "FP";}
			else if (StringUtils::equalsNoCase(attr_value, "doris_navigator"))
                        { attr_value = "DN";}
			else if (StringUtils::equalsNoCase(attr_value, "doris_precise"))
                        { attr_value = "DP";}
			else if (StringUtils::equalsNoCase(attr_value, "doris_preliminary"))
                        { attr_value = "DI";}
			else
			{ attr_value = "XX";}
		}



excerr << "FIXME DEFINITIVE VALUE: \"" << 	attr_value << "\"" << endl ;	
		
		// Convert the time
		if (is_time)
		{
			string time_value = StringUtils::uppercase( StringUtils::trim(attr_value) ) ;
				
			// Remove the UTC=, TAI= label:
			if ( (time_value.substr(0, 4) == "UTC=") || (time_value.substr(0, 4) == "TAI=") ) 
			{ time_value = time_value.substr(4) ; }

			if (false == time_value.empty() )
			{
				// Convert from: "2010-11-09T01:34:32.199266"
				// to: "09-NOV-2010 01:34:32.199266"
				
				DateTime dt (time_value) ;
	        	DateTime::UTCD utcd; dt.toUTCD(utcd) ;

				ostringstream o ; 
				o 	<< setfill('0') << setw(2) << utcd.day << "-" \
					<< StringUtils::uppercase(DateTime::num2Month(utcd.month)) << "-" \
					<< setfill('0') << setw(4) << utcd.year << " " \
					<< setfill('0') << setw(2) << utcd.hour  << ":" \
					<< setfill('0') << setw(2) << utcd.min  << ":" \
					<< setfill('0') << setw(2) << utcd.sec << "." \
					<< setfill('0') << setw(6) << utcd.microsec() ;

				attr_value = o.str() ; 
excerr << "FIXMETIME: \"" << time_value << "\" to: \"" << attr_value << "\"" << endl ;
	
			} else { attr_value = "" ; }	// Empty time
		}	// END: if (is_time)
			
		const string q ( is_string ? "\"" : "" ) ;

		if (!dbl_attr_name.empty())
		{
			const string tempString = ( dbl_attr_name + "=" + q + attr_value + q ) ;

			if (add_to_mph) { load(tempString, _mphData); }
			if (add_to_sph) { load(tempString, _sphData); }
		}

	}	// END: for

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ProductHeader::readNetcdfParams  Method finished")
}


//
// read MPH ASCII file
//
void ProductHeader::readMPH(ifstream& fileStream)
{
	// alloc memory
	char * temp;
	temp = new char[256];
	
	unsigned int counter = 0;
	while(counter < _mphData.size())
	{
		fileStream.getline(temp, 256);
		string tempString(temp);
		if(int(tempString.find('=')) != -1)
		{
			if(tempString.find("NUM_DSD") == 0)
			{
				string numberDSD = tempString.substr(tempString.find('=') +1, tempString.size() -1 - tempString.find('='));
				_numberOfDSD = atoi(numberDSD.c_str());
			}
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading info from string " << tempString )
			load(tempString, _mphData);
		}		
		counter++;	
	}
	
	// free memory
	if (temp)
	{
		delete [] temp;
		temp = 0;
	}
}

//
// read SPH ASCII file
//
void ProductHeader::readSPH(ifstream& fileStream)
{
	// alloc memory
	char * temp;
	temp = new char[256];	
	
	string firstSPHFieldName = _sphData[0].fieldName;
	
	// parsing SPH fields
	unsigned int counter = 0;
	if (!_mphRead)
	{
		bool exit = false;
		while (!exit)
		{
			fileStream.getline(temp, 256);
			string tempStr(temp);
			if(tempStr.find(firstSPHFieldName) != string::npos)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading info from string " << tempStr )
				load(tempStr, _sphData);
				counter++;
				exit = true;	
			}
		}
	}

	while(counter < _sphData.size())
	{
		fileStream.getline(temp, 256);
		string tempString(temp);
		
		// checking first SPH field from input ASCII file
		if(!counter)
		{
			if(tempString.find(firstSPHFieldName) == string::npos)
				counter = _sphData.size();
		}

		if(tempString.find('=') != string::npos){
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading info from string " <<  tempString )
			load(tempString, _sphData);
		}
		counter++;	
	}
	// free memory
	if (temp)
	{
		delete [] temp;
		temp = 0;
	}
}


void ProductHeader::readDSD(ifstream& fileStream)
{
	// size of dsd data strucutre
	int dsdNum = _dsdData.size();
	// alloc memory
	char * temp;
	temp = new char[256];
	
	string firstDSDFieldName = _dsdData[0][0].fieldName;
	
	// parsing DSD fields
	int dsdCount = -1;
	int counter = 0;
	
	if (!_sphRead)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"no SPH read from file. Searching for first DSD field...")
		bool exit = false;
		while (!exit)
		{
			fileStream.getline(temp, 256);
			string tempStr(temp);
			if(tempStr.find(firstDSDFieldName) != string::npos)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading info from string " <<tempStr )
				dsdCount++;
				load(tempStr, _dsdData[dsdCount]);
				counter++;
				exit = true;	
			}
		}
	}

	bool exitDSD = false;
	while(counter < static_cast<int>(_dsdTemplate.size())|| !exitDSD)
	{
		fileStream.getline(temp, 256);
		string tempString(temp);
		
		if(tempString.find(firstDSDFieldName) != string::npos)
		{			
			dsdCount++;
			if (dsdCount >= dsdNum) {
				_dsdData.push_back(_dsdTemplate);
			}
			counter = -1;
			
			load(tempString, _dsdData[dsdCount]);
			counter++;
			continue;			
		}
		else if(counter == static_cast<int>(_dsdTemplate.size())) {
				exitDSD = true; 
		}
		else if(tempString.find('=') != string::npos)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading info from string " << tempString )
			load(tempString, _dsdData[dsdCount]);
		}
		counter++;	
	}	
	// free memory
	if (temp)
	{
		delete [] temp;
		temp = 0;
	}
}

//
// load input ASCII file line
//
void ProductHeader::load(const string& tempLine, vector<PHdata>& data)
{
	// extracting tag name and tag value
	string tagName = tempLine.substr(0, tempLine.find('='));
	string tagValue = tempLine.substr(tempLine.find('=') +1, tempLine.size()-1 - tempLine.find('='));

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"extracting tag name '" << tagName << "' and tag value '" << tagValue << "'")
	
	bool exitLoop = false;
	unsigned int count=0;
	while(!exitLoop && count < data.size())
	{
		if(data[count].fieldName == tagName)
		{	
			// loading value of field type signed long long int
			if(data[count].fieldValueFormat.find("Ld") != string::npos)
			{
				if(tagValue.find('<') != string::npos)
					tagValue = tagValue.substr(0, tagValue.find('<'));

// Comparing the length of a string with the number of bytes of the receiving buffer doesn't have any sense
//				if (tagValue.size() > data[count].fieldValueSize)
//				{
//					ostringstream msg;
//					msg << "ProductHeader::load : Bad value ('" << tagValue << "') found for tag " << tagName 
//						<< ". Incorrect size " << tagValue.size() << "[expected " << data[count].fieldValueSize << "]";
//					ProductHeaderException e(msg.str());
//					ACS_THROW(e);
//				}
				
				long long int value = fromString<long long int>(tagValue);
				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found long long integer field " << tagName << " to set with value " << tagValue )
				setField(tagName, value, data);
			}	
			// loading value of field types unsigned long long int
			else if(data[count].fieldValueFormat.find("Lu") != string::npos)
			{
				if(tagValue.find('<') != string::npos)
					tagValue = tagValue.substr(0, tagValue.find('<'));
				
// Comparing the length of a string with the number of bytes of the receiving buffer doesn't have any sense
//				if (tagValue.size() > data[count].fieldValueSize)
//				{
//					ostringstream msg;
//					msg << "ProductHeader::load : Bad value ('" << tagValue << "') found for tag " << tagName 
//						<< ". Incorrect size " << tagValue.size() << "[expected " << data[count].fieldValueSize << "]";
//					ProductHeaderException e(msg.str());
//					ACS_THROW(e);
//				}

				unsigned long long int value = fromString<unsigned long long int>(tagValue);
				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found unsigned long long int field " << tagName << " to set with value '" << value << "'" )
				setField(tagName, value, data);
			}
			// loading value of field types unsigned long int
			else if(data[count].fieldValueFormat.find('u') != string::npos)
			{
				if(tagValue.find('<') != string::npos)
					tagValue = tagValue.substr(0, tagValue.find('<'));
				
// Comparing the length of a string with the number of bytes of the receiving buffer doesn't have any sense
//				if (tagValue.size() > data[count].fieldValueSize)
//				{
//					ostringstream msg;
//					msg << "ProductHeader::load : Bad value ('" << tagValue << "') found for tag " << tagName 
//						<< ". Incorrect size " << tagValue.size() << "[expected " << data[count].fieldValueSize << "]";
//					ProductHeaderException e(msg.str());
//					ACS_THROW(e);
//				}
				
				unsigned long int value = fromString<unsigned long int>(tagValue);
				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found unsigned long int field " << tagName << " to set with value " << tagValue)
				setField(tagName, value, data);
			}
			// loading value of field types: int, unsigned int, long int
			else if(data[count].fieldValueFormat.find('d') != string::npos)
			{
				if(tagValue.find('<') != string::npos)
					tagValue = tagValue.substr(0, tagValue.find('<'));
				
// Comparing the length of a string with the number of bytes of the receiving buffer doesn't have any sense
//				if (tagValue.size() > data[count].fieldValueSize)
//				{
//					ostringstream msg;
//					msg << "ProductHeader::load : Bad value ('" << tagValue << "') found for tag " << tagName 
//						<< ". Incorrect size " << tagValue.size() << "[expected " << data[count].fieldValueSize << "]";
//					ProductHeaderException e(msg.str());
//					ACS_THROW(e);
//				}				
				//int value = atoi(tagValue.c_str());
				long int value = fromString<long int>(tagValue);
				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found integer field " << tagName << " to set with value " << tagValue)
				setField(tagName, value, data);
			}		
			// loading value of float field
			else if(data[count].fieldValueFormat.find('f') != string::npos
			   		|| data[count].fieldValueFormat.find('e') != string::npos
			   		|| data[count].fieldValueFormat.find('E') != string::npos
			  		)
			{
				if(tagValue.find('<') != string::npos)
					tagValue = tagValue.substr(0, tagValue.find('<'));

// Comparing the length of a string with the number of bytes of the receiving buffer doesn't have any sense
//				if (tagValue.size() > data[count].fieldValueSize)
//				{
//					ProductHeaderException e("ProductHeader::load : Bad value ('" + tagValue + "') found for tag " +tagName+". Incorrect size!");
//					ACS_THROW(e);
//				}

				float value = atof(tagValue.c_str());
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found float field " << tagName << " to set with value " << tagValue)
				setField(tagName, value, data);					
			}
			// loading value of string type field
			else if(!data[count].fieldValueFormat.size())
			{
				if(tagValue.find('"') != string::npos)
					tagValue = tagValue.substr(1, tagValue.size() -2);
					
				if(tagValue.find('<') != string::npos)
					tagValue = tagValue.substr(0, tagValue.find('<'));
					
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found string field " << tagName << " to set with value " << tagValue)
				setField(tagName, tagValue, data);	 				
			}							
			exitLoop = true;
		}
		count++;	
	}
	
	if(!exitLoop)
	{
		ProductHeaderException e("Failure in loading ASCII file line. Unknown input tag name: " + tagName);
		ACS_THROW(e);	
	}
}


//
// check field value format
//
bool ProductHeader::checkFieldValueConstraints(const string& fieldValue , const PHdata& phData)
{
	// checking if field value in input is compatible with field permitted values list
	if (phData.listOfFieldPermittedValues.numberOfFieldValuesPermitted !=0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Number of field permitted values is " << phData.listOfFieldPermittedValues.numberOfFieldValuesPermitted)
		bool valueStatus = false;
		u_int count=0;
		while(count < phData.listOfFieldPermittedValues.numberOfFieldValuesPermitted && !valueStatus)
		{
			if (fieldValue == phData.listOfFieldPermittedValues.permittedValue[count]) {
				valueStatus = true;
			}
			count++;	
		}
		if(!valueStatus)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"field value in input isn't compatible with field permitted values" )
			return false;
		}
	}

	// checking if field value in input is compatible with all field value format constraints
	if (phData.listOfFieldValueConstraints.numberOfFieldValueConstraints !=0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Number of field value constraint is " << phData.listOfFieldValueConstraints.numberOfFieldValueConstraints)
		unsigned int numOfConstraints = phData.listOfFieldValueConstraints.numberOfFieldValueConstraints;
		for(unsigned int i=0; i < numOfConstraints; i++)
		{
			unsigned int counter=0;
			unsigned int numOfConstraintPositions = phData.listOfFieldValueConstraints.valueConstraint[i].numberOfFieldValueConstraintPositions;
			while(counter != numOfConstraintPositions)
			{
				string constraintContent = phData.listOfFieldValueConstraints.valueConstraint[i].fieldValueConstraintContent;
				unsigned int constraintPosition = phData.listOfFieldValueConstraints.valueConstraint[i].infoOnFieldValueConstraintPosition[counter];
				string val(1,fieldValue[constraintPosition]);
				if( val != constraintContent)
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"field value in input isn't compatible with all field value format constraints")
					return false;
				}
				counter++;
			}
		}
	}
	return true;	
}


//
// init part of ASCII file Product Header
//
void ProductHeader::init(const rsResourceSet& resource, const string& root, vector<PHdata>& data)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	// extracting number of resource fields
	unsigned int vectorSize =0;
	resource.getValue(root + ".number" + root + "Fields", vectorSize);
	
	for (u_int count =0; count < vectorSize; count++)
	{
		PHdata  tempData;
		
		char tmp[8];
		sprintf(tmp,"%d",count+1);
		string 	countString(tmp);
		
		string 	tempFieldName = root + ".field" + countString + ".fieldName";
		
		string 	tempFieldValue = root + ".field" + countString + ".fieldValue";
		
		string	tempNumberOfFieldPermittedValues = root + ".field" + countString + ".numberOfFieldPermittedValues";
		
		string	tempFieldPermittedValueContent = root + ".field" + countString + ".fieldPermittedValueContent.";
		
		string 	tempFieldValueFormat = root + ".field" +  countString + ".fieldValueFormat";
		
		string	tempNumberOfFieldValueConstraints = root + ".field" +  countString + ".numberOfFieldValueConstraints";
		
		string	tempFieldValueConstraintContent = root + ".field" +  countString + ".fieldValueConstraintContent.";
		
		string	tempNumberOfFieldValueConstraintPositions = root + ".field" +  countString + ".numberOfFieldValueConstraintPositions.";
		
		string	tempFieldValueConstraintPosition = root + ".field" +  countString + ".fieldValueConstraintPosition.";
		
		string 	tempFieldValueSize = root + ".field" +  countString + ".fieldValueSize";
		
		string 	tempUnitsFormat = root + ".field" +  countString + ".unitsFormat";
		
		string	tempQuotationMark = root + ".field" +  countString + ".quotationMark";

		// exctract fields from ph resource
		try
		{
			resource.getValue(tempFieldName , tempData.fieldName);
			resource.getValue(tempFieldValue , tempData.fieldValue);
			resource.getValue(tempFieldValueSize , tempData.fieldValueSize);
			resource.getValue(tempUnitsFormat , tempData.unitsFormat);
			resource.getValue(tempQuotationMark , tempData.quotationMark);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading element " << tempData.fieldName << " with default value '" << tempData.fieldValue << "'" )
			resource.getValue(tempNumberOfFieldPermittedValues , tempData.listOfFieldPermittedValues.numberOfFieldValuesPermitted);
			if(tempData.listOfFieldPermittedValues.numberOfFieldValuesPermitted)
			{
				u_int counter=1;
				while(counter <= tempData.listOfFieldPermittedValues.numberOfFieldValuesPermitted)
				{
					char tmpCounter[8];
					sprintf(tmpCounter,"%d",counter);
					string 	stringCounter(tmpCounter);
					string tempValue;
					resource.getValue(tempFieldPermittedValueContent + "value" + stringCounter, tempValue);
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Element " << tempData.fieldName 
						  << " permitted value content is '" << tempValue 
						  << "' of size " << tempValue.size() << endl 
						  << "tempData.fieldValueSize = " << tempData.fieldValueSize)
					string newVal = tempValue;
					if( tempValue.size() < tempData.fieldValueSize){						
						ostringstream val;
						int size = tempData.fieldValueSize;
						val << setiosflags(ios::left) << setfill(' ') << setw(size) << tempValue.c_str() << resetiosflags(ios::left);
						newVal = val.str();
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"newVal = " << newVal << " with size " << newVal.size())
					}
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"element " << tempData.fieldName << " HAS PERMITTED VALUE '" << newVal << "'")
					tempData.listOfFieldPermittedValues.permittedValue.push_back(newVal);
					counter++;
				}
			}	
			resource.getValue(tempFieldValueFormat , tempData.fieldValueFormat);
			resource.getValue(tempNumberOfFieldValueConstraints , tempData.listOfFieldValueConstraints.numberOfFieldValueConstraints);
			if(tempData.listOfFieldValueConstraints.numberOfFieldValueConstraints)
			{
				u_int counter=1;
				while(counter <= tempData.listOfFieldValueConstraints.numberOfFieldValueConstraints)
				{
					char tmpCounter[8];
					sprintf(tmpCounter,"%d",counter);
					string 	stringCounter(tmpCounter);
					fieldValueConstraint tmpFldValConstr;
					resource.getValue(tempFieldValueConstraintContent + "constraint" + stringCounter, tmpFldValConstr.fieldValueConstraintContent);
					resource.getValue(tempNumberOfFieldValueConstraintPositions + "numberPosition" + stringCounter, tmpFldValConstr.numberOfFieldValueConstraintPositions);					
					if(tmpFldValConstr.numberOfFieldValueConstraintPositions)
					{
						u_int counter2=1;
						while(counter2 <= tmpFldValConstr.numberOfFieldValueConstraintPositions)
						{
							char tmpCounter2[8];
							sprintf(tmpCounter2,"%d",counter2);
							string 	stringCounter2(tmpCounter2);
							unsigned int tempPosition;
							resource.getValue(tempFieldValueConstraintPosition + "constraintPosition" + stringCounter + ".position" + stringCounter2, tempPosition);
							tmpFldValConstr.infoOnFieldValueConstraintPosition.push_back(tempPosition);
							counter2++;
						}
					}
					tempData.listOfFieldValueConstraints.valueConstraint.push_back(tmpFldValConstr);
					counter++;
				}			
			}
			
			
			data.push_back(tempData);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"stored element " << tempData.fieldName << " with default value " << tempData.fieldValue)
		}
		catch (exception &x) 
		{
			ProductHeaderException e(" Cannot find a specific field in " + root +  " resource: " + x.what());
			ACS_THROW(e);
		
        } 		
	}
}

/*
 * initialize XML part of Product Header
 */
void ProductHeader::initXML(const rsResourceSet& resource, const string& root, vector<XmlPHData>& vecData, unsigned int numOfFields)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	for (u_int count =0; count < numOfFields; count++)
	{		
		try 
		{
			XmlPHData xmlData;

			char tmp[8];
			sprintf(tmp,"%d",count+1);
			string 	countString(tmp);
			string fieldPrefix = root + ".field" + countString;

			//get tagType value
			resource.getValue(fieldPrefix + ".tagType", xmlData.tagType);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"resource element " << fieldPrefix << ".tagType has type "<< xmlData.tagType)

			if ( xmlData.tagType == "SIMPLE" )
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"this element has no sub-elements" )
				PHdata tempData;
				resource.getValue(fieldPrefix + ".fieldNumber", xmlData.fieldNumber);	
				string 	tempFieldName = fieldPrefix + ".fieldName";
				string 	tempFieldValue = fieldPrefix + ".fieldValue";
				string	tempNumberOfFieldPermittedValues = fieldPrefix + ".numberOfFieldPermittedValues";
				string	tempFieldPermittedValueContent = fieldPrefix + ".fieldPermittedValueContent.";
				string 	tempFieldValueFormat = fieldPrefix + ".fieldValueFormat";
				string	tempNumberOfFieldValueConstraints = fieldPrefix + ".numberOfFieldValueConstraints";
				string	tempFieldValueConstraintContent = fieldPrefix + ".fieldValueConstraintContent.";
				string	tempNumberOfFieldValueConstraintPositions = fieldPrefix + ".numberOfFieldValueConstraintPositions.";
				string	tempFieldValueConstraintPosition = fieldPrefix + ".fieldValueConstraintPosition.";
				string 	tempFieldValueSize = fieldPrefix + ".fieldValueSize";
				string 	tempUnitsFormat = fieldPrefix + ".unitsFormat";
				string	tempQuotationMark = fieldPrefix + ".quotationMark";
				// exctract fields from resource		
				resource.getValue(tempFieldName , tempData.fieldName);
				resource.getValue(tempFieldValue , tempData.fieldValue);
				resource.getValue(tempFieldValueSize , tempData.fieldValueSize);
				resource.getValue(tempUnitsFormat , tempData.unitsFormat);
				resource.getValue(tempQuotationMark , tempData.quotationMark);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading element " << tempData.fieldName << " with default value " << tempData.fieldValue )
				resource.getValue(tempNumberOfFieldPermittedValues , tempData.listOfFieldPermittedValues.numberOfFieldValuesPermitted);
				if(tempData.listOfFieldPermittedValues.numberOfFieldValuesPermitted)
				{
					u_int counter=1;
					while(counter <= tempData.listOfFieldPermittedValues.numberOfFieldValuesPermitted)
					{
						char tmpCounter[8];
						sprintf(tmpCounter,"%d",counter);
						string 	stringCounter(tmpCounter);
						string tempValue;
						resource.getValue(tempFieldPermittedValueContent + "value" + stringCounter, tempValue);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Element " << tempData.fieldName 
							   << " permitted value content is '" << tempValue 
							   << "' of size " << tempValue.size() << endl 
							   << "tempData.fieldValueSize = " << tempData.fieldValueSize)
						string newVal = tempValue;
						if( tempValue.size() < tempData.fieldValueSize){						
							ostringstream val;
							int size = tempData.fieldValueSize;
							val << setiosflags(ios::left) << setfill(' ') << setw(size) << tempValue.c_str() << resetiosflags(ios::left);
							newVal = val.str();
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"newVal = " << newVal << " with size " << newVal.size())
						}
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"element " << tempData.fieldName << " HAS PERMITTED VALUE '" << newVal << "'" )
						tempData.listOfFieldPermittedValues.permittedValue.push_back(newVal);
						counter++;
					}
				}	
				resource.getValue(tempFieldValueFormat , tempData.fieldValueFormat);
				resource.getValue(tempNumberOfFieldValueConstraints , tempData.listOfFieldValueConstraints.numberOfFieldValueConstraints);
				if(tempData.listOfFieldValueConstraints.numberOfFieldValueConstraints)
				{
					u_int counter=1;
					while(counter <= tempData.listOfFieldValueConstraints.numberOfFieldValueConstraints)
					{
						char tmpCounter[8];
						sprintf(tmpCounter,"%d",counter);
						string 	stringCounter(tmpCounter);
						fieldValueConstraint tmpFldValConstr;
						resource.getValue(tempFieldValueConstraintContent + "constraint" + stringCounter, tmpFldValConstr.fieldValueConstraintContent);
						resource.getValue(tempNumberOfFieldValueConstraintPositions + "numberPosition" + stringCounter, tmpFldValConstr.numberOfFieldValueConstraintPositions);					
						if(tmpFldValConstr.numberOfFieldValueConstraintPositions)
						{
							u_int counter2=1;
							while(counter2 <= tmpFldValConstr.numberOfFieldValueConstraintPositions)
							{
								char tmpCounter2[8];
								sprintf(tmpCounter2,"%d",counter2);
								string 	stringCounter2(tmpCounter2);
								unsigned int tempPosition;
								resource.getValue(tempFieldValueConstraintPosition + "constraintPosition" + stringCounter + ".position" + stringCounter2, tempPosition);
								tmpFldValConstr.infoOnFieldValueConstraintPosition.push_back(tempPosition);
								counter2++;
							}
						}
						tempData.listOfFieldValueConstraints.valueConstraint.push_back(tmpFldValConstr);
						counter++;
					}			
				}
				

				xmlData.data = tempData;
				vecData.push_back(xmlData);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"stored element " << tempData.fieldName << " with default value " << tempData.fieldValue)
			}
			else //tagType == "COMPOSITE"  
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"this element has sub-elements")
				
				//get the number of sub-elements for this COMPOSITE element and store in XmlPHData structure
				resource.getValue(fieldPrefix + ".numberOfFields", xmlData.childrenNumber);				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"childrenNumber=" << xmlData.childrenNumber)
				
				//get COMPOSITE tagName and store in XmlPHData structure
				resource.getValue(fieldPrefix + ".tagName", xmlData.tagName);				
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"tagName=" << xmlData.tagName )
				//define the vector storing the children for this COMPOSITE tag
				vector<XmlPHData> children;
				//recursive call for children
				initXML(resource, fieldPrefix, children, xmlData.childrenNumber);
				//store children vector in XmlPHData structure
				xmlData.children = children;
				vecData.push_back(xmlData);
			}
		}
		catch (exception &x) 
		{
			ProductHeaderException e(" Cannot find a specific field in XML " + root +  " resource: " + x.what());
			ACS_THROW(e);
        } 		
	}
}

//
// get field value
//
string ProductHeader::getFieldValue(const string& fieldName, const vector<PHdata>& data)
{
	unsigned int i=0;
	bool searchStatus = false;
	string temp = "";
	while(i < data.size())
	{
		if(data[i].fieldName == fieldName)
		{
			searchStatus = true;
			temp = data[i].fieldValue;
			i = data.size();
		}	
		i++;
	}
	
	if(!searchStatus)
	{
		ProductHeaderException e(" Tag: " + fieldName + " not found.");
		ACS_THROW(e);		
	}
	 return temp;
}

//
// check if PH data have been initialized
//
void ProductHeader::checkDataInizialization()
{
	if(!_mphData.size())
	{
		ProductHeaderException e("MPH not initialized.");
		ACS_THROW(e);	
	}
	
	if(!_sphData.size() && _dsdData.size())
	{
		ProductHeaderException e("SPH not initialized while some DSD have been initialized.");
		ACS_THROW(e);	
	}
}

//
// check if XML PH data have been initialized
//
void ProductHeader::checkXMLDataInizialization()
{
	if(!_xmlFixedHeaderData.size())
	{
		ProductHeaderException e("XML Fixed Header not initialized.");
		ACS_THROW(e);	
	}
	
	if(!_xmlMphData.size())
	{
		ProductHeaderException e("XML MPH not initialized.");
		ACS_THROW(e);	
	}
	
	if(!_xmlSphData.size())
	{
		ProductHeaderException e("XML SPH not initialized.");
		ACS_THROW(e);	
	}
	
	if(!_xmlMdhData.size())
	{
		ProductHeaderException e("XML MDH not initialized.");
		ACS_THROW(e);	
	}
	
	if(!_xmlDsdData.size())
	{
		ProductHeaderException e("XML DSDs not initialized.");
		ACS_THROW(e);	
	}
}


//
// set a specific field in PH ASCII file
//
void ProductHeader::setField(const string& fieldName, const string& fieldValue, vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName  << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if (data[count].fieldValueSize > 0)
			{
				if(fieldValue.size() <= data[count].fieldValueSize && checkFieldValueConstraints(fieldValue, data[count]))
				{
					data[count].fieldValue = fieldValue;
					setStatus = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << fieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} else
			  {
			  		if(checkFieldValueConstraints(fieldValue, data[count]))
					{
						data[count].fieldValue = fieldValue;
						setStatus = true;
					} else
					  {
					  		ProductHeaderException e(" Value " + fieldValue + " for tag " + fieldName + " is not compatible with value size");
							ACS_THROW(e);
					  }
			  }	
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}


void ProductHeader::setField(const string& fieldName, const int& fieldValue,vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if (data[count].fieldValueFormat.find('d') == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[64];
			sprintf(effectiveFieldValue , data[count].fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].fieldValueSize)
				{
					data[count].fieldValue = effectiveStringFieldValue;
					setStatus = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].fieldValue = effectiveStringFieldValue;
				setStatus = true;
			}
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}

void ProductHeader::setField(const string& fieldName, const float& fieldValue,vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if (data[count].fieldValueFormat.find('f') != string::npos
			    || data[count].fieldValueFormat.find('e') != string::npos
				|| data[count].fieldValueFormat.find('E') != string::npos
			   )
			{	
				// translate fieldValue in input in fixed format
				char effectiveFieldValue[64];
				sprintf(effectiveFieldValue , data[count].fieldValueFormat.c_str() , fieldValue);
				string effectiveStringFieldValue(effectiveFieldValue);

				// checking a particular case: |float number| in input is < 1
				if(data[count].fieldValueFormat == "%+08.6f")
					effectiveStringFieldValue = effectiveStringFieldValue.substr(0,1) + "." + effectiveStringFieldValue.substr(effectiveStringFieldValue.find(".")+1 ,effectiveStringFieldValue.size()-1-effectiveStringFieldValue.find("."));

				if (data[count].fieldValueSize > 0)
				{
					if(effectiveStringFieldValue.size() <= data[count].fieldValueSize)
					{
						data[count].fieldValue = effectiveStringFieldValue;
						setStatus = true;
					}
					else
					{
						ostringstream msg;
						msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
							<< "' is not compatible with value size " << data[count].fieldValueSize;
						ProductHeaderException e(msg.str());
						ACS_THROW(e);			
					}
				}
				else
				{
					data[count].fieldValue = effectiveStringFieldValue;
					setStatus = true;
				}
			}
			else
			{
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't a float");
				ACS_THROW(e);
			}
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}


void ProductHeader::setField(const string& fieldName, const unsigned long long int& fieldValue,vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if(data[count].fieldValueFormat.find("Lu") == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].fieldValueFormat.c_str() , fieldValue);
			
			if(data[count].fieldValueFormat.find("+") != string::npos)
			{
				//set the first character in output as '+' because the sprintf truncate the + sign for Lu
				effectiveFieldValue[0]='+';
			}
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].fieldValueSize)
				{
					data[count].fieldValue = effectiveStringFieldValue;
					setStatus = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].fieldValue = effectiveStringFieldValue;
				setStatus = true;
			}
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}

void ProductHeader::setField(const string& fieldName, const long long int& fieldValue,vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if (data[count].fieldValueFormat.find("Ld") == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't a long long integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].fieldValueSize)
				{
					data[count].fieldValue = effectiveStringFieldValue;
					setStatus = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].fieldValue = effectiveStringFieldValue;
				setStatus = true;
			}
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}

void ProductHeader::setField(const string& fieldName, const long int& fieldValue,vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if (data[count].fieldValueFormat.find('d') == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].fieldValueSize)
				{
					data[count].fieldValue = effectiveStringFieldValue;
					setStatus = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].fieldValue = effectiveStringFieldValue;
				setStatus = true;
			}
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}

void ProductHeader::setField(const string& fieldName, const unsigned long int& fieldValue,vector<PHdata>& data)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool setStatus = false;
	unsigned int count =0;
	while(count < data.size() && !setStatus)
	{
		if(data[count].fieldName == fieldName)
		{
			if (data[count].fieldValueFormat.find('u') == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an unsigned long integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].fieldValueSize)
				{
					data[count].fieldValue = effectiveStringFieldValue;
					setStatus = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].fieldValue = effectiveStringFieldValue;
				setStatus = true;
			}
		}
		count++;	
	}
	if(!setStatus)
	{
		ProductHeaderException e(" Unknown fieldName: " + fieldName);
		ACS_THROW(e);	
	}
}


//
// set a specific field in XML data structure
//
bool ProductHeader::setXmlField(const string& fieldName, const string& fieldValue, vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;
	while(count < data.size() && !exit)
	{
		if (data[count].fieldNumber == fieldNumber && (data[count].data.fieldName == fieldName))
		{
			
			if (data[count].data.fieldValueSize > 0)
			{
				if(fieldValue.size() <= data[count].data.fieldValueSize && checkFieldValueConstraints(fieldValue, data[count].data))
				{
					data[count].data.fieldValue = trim_right(fieldValue);
					exit = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << fieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].data.fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else
			{
				if(checkFieldValueConstraints(fieldValue, data[count].data))
				{
					data[count].data.fieldValue = trim_right(fieldValue);
					exit = true;
				} 
				else
				{
				  	ProductHeaderException e(" Value for tag: " + fieldName + " is not compatible with value size");
					ACS_THROW(e);
				}
			}		
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}

bool ProductHeader::setXmlField(const string& fieldName, const float& fieldValue,vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;
	while(count < data.size() && !exit)
	{
		if (data[count].fieldNumber == fieldNumber && (data[count].data.fieldName == fieldName))
		{
			if (data[count].data.fieldValueFormat.find('f') != string::npos
			    || data[count].data.fieldValueFormat.find('e') != string::npos
				|| data[count].data.fieldValueFormat.find('E') != string::npos
			   )
			{	
				// translate fieldValue in input in fixed format
				char effectiveFieldValue[64];
				sprintf(effectiveFieldValue , data[count].data.fieldValueFormat.c_str() , fieldValue);
				string effectiveStringFieldValue(effectiveFieldValue);

				// checking a particular case: |float number| in input is < 1
				if(data[count].data.fieldValueFormat == "%+08.6f")
					effectiveStringFieldValue = effectiveStringFieldValue.substr(0,1) + "." + effectiveStringFieldValue.substr(effectiveStringFieldValue.find(".")+1 ,effectiveStringFieldValue.size()-1-effectiveStringFieldValue.find("."));

				if (data[count].data.fieldValueSize > 0)
				{
					if(effectiveStringFieldValue.size() <= data[count].data.fieldValueSize)
					{
						data[count].data.fieldValue = effectiveStringFieldValue;
						exit = true;
					}
					else
					{
						ostringstream msg;
						msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
							<< "' is not compatible with value size " << data[count].data.fieldValueSize;
						ProductHeaderException e(msg.str());
						ACS_THROW(e);			
					}
				}
				else
				{
					data[count].data.fieldValue = effectiveStringFieldValue;
					exit = true;
				}
			}
			else
			{
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name" + fieldName + " isn't a float");
				ACS_THROW(e);
			}
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}

bool ProductHeader::setXmlField(const string& fieldName, const int& fieldValue,vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;

	while(count < data.size() && !exit)
	{
		if ( (data[count].fieldNumber == fieldNumber) && (data[count].data.fieldName == fieldName))
		{
			if (data[count].data.fieldValueFormat.find('d') == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an integer");
				ACS_THROW(e);
			}
				
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[64];
			sprintf(effectiveFieldValue , data[count].data.fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].data.fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].data.fieldValueSize)
				{
					data[count].data.fieldValue = effectiveStringFieldValue;
					exit = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].data.fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].data.fieldValue = effectiveStringFieldValue;
				exit = true;
			}	
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}

bool ProductHeader::setXmlField(const string& fieldName, const long long int& fieldValue,vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;
	while(count < data.size() && !exit)
	{
		if ( (data[count].fieldNumber == fieldNumber) && (data[count].data.fieldName == fieldName))
		{
			if (data[count].data.fieldValueFormat.find("Ld") == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't a long long integer");
				ACS_THROW(e);
			}
						
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].data.fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].data.fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].data.fieldValueSize)
				{
					data[count].data.fieldValue = effectiveStringFieldValue;
					exit = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].data.fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].data.fieldValue = effectiveStringFieldValue;
				exit = true;
			}	
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}

bool ProductHeader::setXmlField(const string& fieldName, const long int& fieldValue,vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;
	while(count < data.size() && !exit)
	{
		if ( (data[count].fieldNumber == fieldNumber) && (data[count].data.fieldName == fieldName))
		{
			if (data[count].data.fieldValueFormat.find('d') == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].data.fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].data.fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].data.fieldValueSize)
				{
					data[count].data.fieldValue = effectiveStringFieldValue;
					exit = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].data.fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].data.fieldValue = effectiveStringFieldValue;
				exit = true;
			}	
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}


bool ProductHeader::setXmlField(const string& fieldName, const unsigned long long int& fieldValue,vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;
	while(count < data.size() && !exit)
	{
		if ( (data[count].fieldNumber == fieldNumber) && (data[count].data.fieldName == fieldName))
		{
			if (data[count].data.fieldValueFormat.find("Lu") == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].data.fieldValueFormat.c_str() , fieldValue);
			
			if(data[count].data.fieldValueFormat.find("+") != string::npos)
			{
				//set the first character in output as '+' because the sprintf truncate the + sign for Lu
				effectiveFieldValue[0]='+';
			}

			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].data.fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].data.fieldValueSize)
				{
					data[count].data.fieldValue = effectiveStringFieldValue;
					exit = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].data.fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].data.fieldValue = effectiveStringFieldValue;
				exit = true;
			}	
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}

bool ProductHeader::setXmlField(const string& fieldName, const unsigned long int& fieldValue,vector<XmlPHData>& data, const unsigned int& fieldNumber)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for  " << fieldName << " to set value " <<  fieldValue )
	bool exit = false;
	unsigned int count =0;
	while(count < data.size() && !exit)
	{
		if ( (data[count].fieldNumber == fieldNumber) && (data[count].data.fieldName == fieldName))
		{
			if (data[count].data.fieldValueFormat.find('u') == string::npos)
			{	
				ProductHeaderException e("Unknown format of field value in input!! Field value format associated to field name " + fieldName + " isn't an unsigned long integer");
				ACS_THROW(e);
			}
			
			// translate fieldValue in input in fixed format
			char effectiveFieldValue[128];
			sprintf(effectiveFieldValue , data[count].data.fieldValueFormat.c_str() , fieldValue);
			string effectiveStringFieldValue(effectiveFieldValue);
			
			if(data[count].data.fieldValueSize > 0)
			{
				if(effectiveStringFieldValue.size() <= data[count].data.fieldValueSize)
				{
					data[count].data.fieldValue = effectiveStringFieldValue;
					exit = true;
				}
				else
				{
					ostringstream msg;
					msg <<  " Value [" << effectiveStringFieldValue << "] for tag '" << fieldName
						<< "' is not compatible with value size " << data[count].data.fieldValueSize;
					ProductHeaderException e(msg.str());
					ACS_THROW(e);			
				}
			} 
			else 
			{
				data[count].data.fieldValue = effectiveStringFieldValue;
				exit = true;
			}	
		}
		else if (data[count].tagType == "COMPOSITE")
		{
			bool result = setXmlField(fieldName, fieldValue, data[count].children, fieldNumber);
			if (result)
				exit = true;
		}
		count++;
	}
	return exit;
}


// -----------------------------------------------------------------------------------------
// ----------------------------- Auxiliary functions for readXML method --------------------
// -----------------------------------------------------------------------------------------

//
// Load XML Fixed Header data stored in reResourceSet into internal data structure
//
void ProductHeader::loadXmlFixedHeaderData(const string& root, const rsResourceSet& resource, vector<XmlPHData>& xmlData, int count)
{
	int counter = -1;
	string resPrefix = "";
	for(unsigned int i=0; i < xmlData.size(); i++)
	{		
		if (xmlData[i].tagType == "SIMPLE")
		{
			resPrefix = root + "." + xmlData[i].data.fieldName;
			
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]";
			}
		
			//check if TAG has attribute and set value
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					//the only possible attributes are unit and count
					if(attributes[k] == "unit")
					{
						string unitValue = "";
						resource.getValue(resPrefix+ "." +attributes[k]+".value", unitValue);
						xmlData[i].data.unitsFormat = unitValue;
					}
				}
			}
			
			
			//string type field
			if( (xmlData[i].data.fieldValueFormat.find('s') != string::npos) || (xmlData[i].data.fieldValueFormat == "") )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				setXmlFixedHeaderField(xmlData[i].data.fieldName , strValue, xmlData[i].fieldNumber);
			}
			//float type field
			else if(xmlData[i].data.fieldValueFormat.find('f') != string::npos
				    || xmlData[i].data.fieldValueFormat.find('e') != string::npos
					|| xmlData[i].data.fieldValueFormat.find('E') != string::npos
				   )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				float floatValue = fromString<float>(strValue);
				setXmlFixedHeaderField(xmlData[i].data.fieldName , floatValue, xmlData[i].fieldNumber);
			}	
			//unsigned long long integer type field
			else if(xmlData[i].data.fieldValueFormat.find("Lu") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long long int value = fromString<unsigned long long int>(strValue);
				setXmlFixedHeaderField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//unsigned long integer type field
			else if(xmlData[i].data.fieldValueFormat.find('u') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long int value = fromString<unsigned long long int>(strValue);
				setXmlFixedHeaderField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//long long integer type field
			else if(xmlData[i].data.fieldValueFormat.find("Ld") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				long long int value = fromString<long long int>(strValue);
				setXmlFixedHeaderField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//integer type field
			else if(xmlData[i].data.fieldValueFormat.find('d') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				int intValue = fromString<int>(strValue);
				setXmlFixedHeaderField(xmlData[i].data.fieldName , intValue, xmlData[i].fieldNumber);
			}
		}
		else //COMPOSITE
		{
			resPrefix = root + "." + xmlData[i].tagName;
			
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
			int cnt = -1;
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					if(attributes[k] == "count")
					{
						u_int childrenNumberValue = 0;
						resource.getValue(resPrefix+"."+attributes[k]+ ".value", childrenNumberValue);
						xmlData[i].childrenNumber = childrenNumberValue;
						cnt = xmlData[i].childrenNumber;
					}
				}
			}
			loadXmlFixedHeaderData(resPrefix, resource, xmlData[i].children, cnt);
		}
	}
}

//
// Load XML MPH data stored in reResourceSet into internal data structure
//
void ProductHeader::loadXmlMPHData(const string& root, const rsResourceSet& resource, vector<XmlPHData>& xmlData, int count)
{
	int counter = -1;
	string resPrefix = "";
	
	for(unsigned int i=0; i < xmlData.size(); i++)
	{		
		if (xmlData[i].tagType == "SIMPLE")
		{
			resPrefix = root + "." + xmlData[i].data.fieldName;
			
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]";
			}
		
			//check if TAG has attribute and set value
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					//the only possible attributes are unit and count
					if(attributes[k] == "unit")
					{
						string unitValue = "";
						resource.getValue(resPrefix+ "." +attributes[k]+".value", unitValue);
						xmlData[i].data.unitsFormat = unitValue;
					}
				}
			}
			
			
			//string type field
			if( (xmlData[i].data.fieldValueFormat.find('s') != string::npos) || (xmlData[i].data.fieldValueFormat == "") )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				setXmlMPHField(xmlData[i].data.fieldName , strValue, xmlData[i].fieldNumber);
			}
			//float type field
			else if(xmlData[i].data.fieldValueFormat.find('f') != string::npos
				    || xmlData[i].data.fieldValueFormat.find('e') != string::npos
					|| xmlData[i].data.fieldValueFormat.find('E') != string::npos
				   )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				float floatValue = fromString<float>(strValue);
				setXmlMPHField(xmlData[i].data.fieldName , floatValue, xmlData[i].fieldNumber);
			}	
			//unsigned long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Lu") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long long int value = fromString<unsigned long long int>(strValue);
				setXmlMPHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//unsigned long int type field
			else if(xmlData[i].data.fieldValueFormat.find('u') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long int value = fromString<unsigned long int>(strValue);
				setXmlMPHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Ld") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				long long int value = fromString<long long int>(strValue);
				setXmlMPHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//integer type field
			else if(xmlData[i].data.fieldValueFormat.find('d') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				int intValue = fromString<int>(strValue);
				setXmlMPHField(xmlData[i].data.fieldName , intValue, xmlData[i].fieldNumber);
			}
		}
		else //COMPOSITE
		{
			resPrefix = root + "." + xmlData[i].tagName;
			
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]";
			}
			int cnt = -1;
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					if(attributes[k] == "count")
					{
						u_int childrenNumberValue = 0;
						resource.getValue(resPrefix+"."+attributes[k]+ ".value", childrenNumberValue);
						xmlData[i].childrenNumber = childrenNumberValue;
						cnt = xmlData[i].childrenNumber;
					}
				}
			}
			loadXmlMPHData(resPrefix, resource, xmlData[i].children, cnt);
		}
	}	
}

//
// Load XML SPH data stored in reResourceSet into internal data structure
//
void ProductHeader::loadXmlSPHData(const string& root, const rsResourceSet& resource, vector<XmlPHData>& xmlData, int count)
{
	int counter = -1;
	string resPrefix = "";
	for(unsigned int i=0; i < xmlData.size(); i++)
	{		
		if (xmlData[i].tagType == "SIMPLE")
		{
			resPrefix = root + "." + xmlData[i].data.fieldName;
			
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
		
			//check if TAG has attribute and set value
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					//the only possible attributes are unit and count
					if(attributes[k] == "unit")
					{
						string unitValue = "";
						resource.getValue(resPrefix+ "." +attributes[k]+".value", unitValue);
						xmlData[i].data.unitsFormat = unitValue;
					}
				}
			}
			
			
			//string type field
			if( (xmlData[i].data.fieldValueFormat.find('s') != string::npos) || (xmlData[i].data.fieldValueFormat == "") )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				setXmlSPHField(xmlData[i].data.fieldName , strValue, xmlData[i].fieldNumber);
			}
			//float type field
			else if(xmlData[i].data.fieldValueFormat.find('f') != string::npos
				    || xmlData[i].data.fieldValueFormat.find('e') != string::npos
					|| xmlData[i].data.fieldValueFormat.find('E') != string::npos
				   )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				float floatValue = fromString<float>(strValue);
				setXmlSPHField(xmlData[i].data.fieldName , floatValue, xmlData[i].fieldNumber);
			}	
			//unsigned long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Lu") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				istringstream str(strValue);
				unsigned long long int value = fromString<unsigned long long int>(strValue);
				setXmlSPHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//unsigned long int type field
			else if(xmlData[i].data.fieldValueFormat.find('u') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long int value = fromString<unsigned long int>(strValue);
				setXmlSPHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Ld") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				long long int value;
				setXmlSPHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//integer type field
			else if(xmlData[i].data.fieldValueFormat.find('d') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				int intValue = fromString<int>(strValue);
				setXmlSPHField(xmlData[i].data.fieldName , intValue, xmlData[i].fieldNumber);
			}
		}
		else //COMPOSITE
		{
			resPrefix = root + "." + xmlData[i].tagName;
			
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
			int cnt = -1;
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					if(attributes[k] == "count")
					{
						u_int childrenNumberValue = 0;
						resource.getValue(resPrefix+"."+attributes[k]+ ".value", childrenNumberValue);
						xmlData[i].childrenNumber = childrenNumberValue;
						cnt = xmlData[i].childrenNumber;
					}
				}
			}
			loadXmlSPHData(resPrefix, resource, xmlData[i].children, cnt);
		}
	}
}


//
// Load XML MDH data stored in reResourceSet into internal data structure
//
void ProductHeader::loadXmlMDHData(const string& root, const rsResourceSet& resource, vector<XmlPHData>& xmlData, int count)
{
	int counter = -1;
	string resPrefix = "";
	
	for(unsigned int i=0; i < xmlData.size(); i++)
	{		
		if (xmlData[i].tagType == "SIMPLE")
		{
			resPrefix = root + "." + xmlData[i].data.fieldName;	
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
		
			//check if TAG has attribute and set value
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					//the only possible attributes are unit and count
					if(attributes[k] == "unit")
					{
						string unitValue = "";
						resource.getValue(resPrefix+ "." +attributes[k]+".value", unitValue);
						xmlData[i].data.unitsFormat = unitValue;
					}
				}
			}
			//string type field
			if( (xmlData[i].data.fieldValueFormat.find('s') != string::npos) || (xmlData[i].data.fieldValueFormat == "") )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				setXmlMDHField(xmlData[i].data.fieldName , strValue, xmlData[i].fieldNumber);
			}
			//unsigned long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Lu") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long long int value = fromString<unsigned long long int>(strValue);
				setXmlMDHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//unsigned long int type field
			else if(xmlData[i].data.fieldValueFormat.find('u') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long int value = fromString<unsigned long int>(strValue);
				setXmlMDHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Ld") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				long long int value = fromString<long long int>(strValue);
				setXmlMDHField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber);
			}
			//integer type field
			else if(xmlData[i].data.fieldValueFormat.find('d') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				int intValue = fromString<int>(strValue);
				setXmlMDHField(xmlData[i].data.fieldName , intValue, xmlData[i].fieldNumber);
			}
			//float type field
			else if(xmlData[i].data.fieldValueFormat.find('f') != string::npos
				    || xmlData[i].data.fieldValueFormat.find('e') != string::npos
					|| xmlData[i].data.fieldValueFormat.find('E') != string::npos
				   )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				float floatValue = fromString<float>(strValue);
				setXmlMDHField(xmlData[i].data.fieldName , floatValue, xmlData[i].fieldNumber);
			}	
		}
		else //COMPOSITE
		{
			resPrefix = root + "." + xmlData[i].tagName;
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
			int cnt = -1;
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					if(attributes[k] == "count")
					{
						u_int childrenNumberValue = 0;
						resource.getValue(resPrefix+"."+attributes[k]+ ".value", childrenNumberValue);
						xmlData[i].childrenNumber = childrenNumberValue;
						cnt = xmlData[i].childrenNumber;
					}
				}
			}
			loadXmlMDHData(resPrefix, resource, xmlData[i].children, cnt);
		}
	}
}

void ProductHeader::loadXmlDSDData(const string& root, 
								   const rsResourceSet& resource, 
								   vector< vector<XmlPHData> >& xmlData, 
								   int count
								  )
{
	//check if XML DSD has been initialized, otherwise throw an exception
	if(_xmlDsdData.size() == 0)
	{
		ProductHeaderException e("ProductHeader::loadXmlDSDData : Unable to load XML DSD data -> no initialization done");
		ACS_THROW(e);
	}

	//calculate the number of XML DSDs to load
	int numberOfDSDtoLoad = -1;
	
	if(resource.hasKey(root+".attributes"))
	{
		vector<string> attributes;
		resource.getArray(root+".attributes", attributes);
		for(unsigned int k=0; k < attributes.size(); k++)
		{
			if(attributes[k] == "count")
				resource.getValue(root+ "." +attributes[k]+".value", numberOfDSDtoLoad);
		}
	}

	if(numberOfDSDtoLoad == -1)
	{
		ProductHeaderException e("ProductHeader::loadXmlDSDData : Unable to load XML DSD data -> count attribute missing");
		ACS_THROW(e);
	}

	unsigned int numConfiguredDSD = _xmlDsdData.size();
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Number of configured XML DSDs is " << numConfiguredDSD )
	
	//if the number of DSDs configured is less than the number of DSDs to load, insert new template elements to fill
	if(static_cast<int>(_xmlDsdData.size()) < numberOfDSDtoLoad)
	{
		for(unsigned int i=0; i < numberOfDSDtoLoad-numConfiguredDSD; i++)
			xmlData.push_back(_xmlDsdTemplate);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"XML DSDs to process is " << _xmlDsdData.size() )
	
	for(int dsdNum=0; dsdNum < numberOfDSDtoLoad; dsdNum++)
	{
		ostringstream singleDsdRoot;
		singleDsdRoot << root+".Data_Set_Descriptor[" << dsdNum << "]";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Loading element " << singleDsdRoot.str() )
		loadXmlDSDSingleData(singleDsdRoot.str(),resource,xmlData[dsdNum],count,dsdNum+1);
	}
}


void ProductHeader::loadXmlDSDSingleData(const string& root, 
										 const rsResourceSet& resource, 
										 vector<XmlPHData>& xmlData, 
										 int count,
										 const unsigned int& numDSDtoSet)
{
	int counter = -1;
	string resPrefix = "";
	
	for(unsigned int i=0; i < xmlData.size(); i++)
	{		
		if (xmlData[i].tagType == "SIMPLE")
		{
			resPrefix = root + "." + xmlData[i].data.fieldName;	
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
		
			//check if TAG has attribute and set value
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					//the only possible attributes are unit and count
					if(attributes[k] == "unit")
					{
						string unitValue = "";
						resource.getValue(resPrefix+ "." +attributes[k]+".value", unitValue);
						xmlData[i].data.unitsFormat = unitValue;
					}
				}
			}
			//string type field
			if( (xmlData[i].data.fieldValueFormat.find('s') != string::npos) || (xmlData[i].data.fieldValueFormat == "") )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				setXmlDSDField(xmlData[i].data.fieldName , strValue, xmlData[i].fieldNumber, numDSDtoSet);
			}
			//unsigned long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Lu") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long long int value = fromString<unsigned long long int>(strValue);
				setXmlDSDField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber, numDSDtoSet);
			}
			//unsigned long int type field
			else if(xmlData[i].data.fieldValueFormat.find('u') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				unsigned long int value = fromString<unsigned long int>(strValue);
				setXmlDSDField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber, numDSDtoSet);
			}
			//long long int type field
			else if(xmlData[i].data.fieldValueFormat.find("Ld") != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				long long int value = fromString<long long int>(strValue);
				setXmlDSDField(xmlData[i].data.fieldName , value, xmlData[i].fieldNumber, numDSDtoSet);
			}
			//integer type field
			else if(xmlData[i].data.fieldValueFormat.find('d') != string::npos)
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				int intValue = fromString<int>(strValue);
				setXmlDSDField(xmlData[i].data.fieldName , intValue, xmlData[i].fieldNumber, numDSDtoSet);
			}
			//float type field
			else if(xmlData[i].data.fieldValueFormat.find('f') != string::npos
				    || xmlData[i].data.fieldValueFormat.find('e') != string::npos
					|| xmlData[i].data.fieldValueFormat.find('E') != string::npos
				   )
			{
				string strValue = "";
				resource.getValue(resPrefix , strValue);
				float floatValue = fromString<float>(strValue);
				setXmlDSDField(xmlData[i].data.fieldName , floatValue, xmlData[i].fieldNumber, numDSDtoSet);
			}	
		}
		else //COMPOSITE
		{
			resPrefix = root + "." + xmlData[i].tagName;
			if ( count != -1)
			{	
				ostringstream num;
				counter++;
				num << counter;
				resPrefix = resPrefix + "[" + num.str() + "]"; 
			}
			int cnt = -1;
			if(resource.hasKey(resPrefix+".attributes"))
			{
				vector<string> attributes;
				resource.getArray(resPrefix+".attributes", attributes);
				for(unsigned int k=0; k < attributes.size(); k++)
				{
					if(attributes[k] == "count")
					{
						u_int childrenNumberValue = 0;
						resource.getValue(resPrefix+"."+attributes[k]+ ".value", childrenNumberValue);
						xmlData[i].childrenNumber = childrenNumberValue;
						cnt = xmlData[i].childrenNumber;
					}
				}
			}
			loadXmlDSDSingleData(resPrefix, resource, xmlData[i].children, cnt, numDSDtoSet);
		}
	}
}



int ProductHeader::getSize(const vector<PHdata>& data)
{
	int result=0;
	int quot=0;
	
	for(unsigned int i=0; i < data.size(); i++)
	{
		
		if(data[i].fieldName.size() == 0)
		{
			result+=data[i].fieldValueSize+1;
		}
		else
		{
			if(data[i].quotationMark.size() > 0)
				quot = 2;
			result+=data[i].fieldName.size() + 		//the field name
					1 + 							//the = char
					quot + 							//the quotation marks if any
					data[i].fieldValueSize +		//the field value size
					data[i].unitsFormat.size() +	//the unit format if any
					1;								//the newline char
		}
		
		quot=0;
	}
	return result;
}


// trim_right() family.
string ProductHeader::trim_right ( const string& source , const string & t)
{
	string str = source;
	return str.erase ( str.find_last_not_of ( t ) + 1 ) ;
}

// wstring trim_right ( const wstring & source , const wstring & t = L" " )
// {
// 	wstring str = source;
// 	return str.erase ( str.find_last_not_of ( t ) + 1 ) ;
// }

// trim_left() family.
string ProductHeader::trim_left ( const string& source , const string & t)
{
	string str = source;
	return str.erase ( 0 , source.find_first_not_of ( t ) ) ;
}

// wstring trim_left ( const wstring & source , const wstring & t = L" " )
// {
// 	wstring str = source;
// 	return str.erase ( 0 , source.find_first_not_of ( t ) ) ;
// }

// trim() family.
string ProductHeader::trim ( const string& source , const string & t)
{
	string str = source;
	return trim_left ( trim_right ( str , t ) , t ) ;
}

// wstring trim ( const wstring & source , const wstring & t = L" " )
// {
// 	wstring str = source;
// 	return trim_left ( trim_right ( str , t ) , t ) ;
// }



/*
 *******************************************************************************************
 ******************************* Private Data Structures ***********************************
 *******************************************************************************************
 */

//
// PHdata struct
// -----------------------------------------------------------------
//
//
// constructor
//
// -----------------------------------------------------------------
ProductHeader::PHdata::PHdata() :
	fieldName(),
	fieldValue(),
	listOfFieldPermittedValues(),
	fieldValueFormat(),
	listOfFieldValueConstraints(),
	fieldValueSize(0),
	unitsFormat(),
	quotationMark()
{
}

ProductHeader::PHdata::~PHdata()
{
}

//
// copy contructor
//
ProductHeader::PHdata::PHdata(const PHdata &e):
	fieldName(e.fieldName),
	fieldValue(e.fieldValue),
	listOfFieldPermittedValues(e.listOfFieldPermittedValues),
	fieldValueFormat(e.fieldValueFormat),
	listOfFieldValueConstraints(e.listOfFieldValueConstraints),
	fieldValueSize(e.fieldValueSize),
	unitsFormat(e.unitsFormat),
	quotationMark(e.quotationMark)
{
}

//
// operator =
//
ProductHeader::PHdata& 
ProductHeader::PHdata::operator=(const PHdata &e) 
{
	if (this != &e) {

		fieldName = e.fieldName ;
		fieldValue = e.fieldValue ;
		listOfFieldPermittedValues = e.listOfFieldPermittedValues ;
		fieldValueFormat = e.fieldValueFormat ;
		listOfFieldValueConstraints = e.listOfFieldValueConstraints ;
		fieldValueSize = e.fieldValueSize ;
		unitsFormat = e.unitsFormat ;
		quotationMark = e.quotationMark ;
	}
	return *this ;
}

//
// XmlPHData struct
// -----------------------------------------------------------------
//
//
// constructor
//
// -----------------------------------------------------------------
ProductHeader::XmlPHData::XmlPHData():
	tagType(),
	fieldNumber(0),
	data(),	
	tagName(),
	childrenNumber(0),
	children()
{
}

ProductHeader::XmlPHData::~XmlPHData()
{
}

//
// XmlPHData copy contructor
//
ProductHeader::XmlPHData::XmlPHData(const XmlPHData &e):
	tagType(e.tagType),
	fieldNumber(e.fieldNumber),
	data(e.data),	
	tagName(e.tagName),
	childrenNumber(e.childrenNumber),
	children(e.children)
{
}

//
// XmlPHData operator =
//
ProductHeader::XmlPHData& 
ProductHeader::XmlPHData::operator=(const XmlPHData &e) 
{
	if (this != &e) {

		tagType = e.tagType ;
		fieldNumber = e.fieldNumber ;
		data = e.data ;
		tagName = e.tagName ;
		childrenNumber = e.childrenNumber ;
		children = e.children ;
	}
	return *this ;
}


//
// fieldValueConstraintsList struct
// -----------------------------------------------------------------
//
//
// constructor
//
// -----------------------------------------------------------------
ProductHeader::fieldValueConstraintsList::fieldValueConstraintsList():
		numberOfFieldValueConstraints(0),
		valueConstraint()
{
}

ProductHeader::fieldValueConstraintsList::~fieldValueConstraintsList()
{
}

//
// copy contructor
//
ProductHeader::fieldValueConstraintsList::fieldValueConstraintsList(const fieldValueConstraintsList &e):
		numberOfFieldValueConstraints(e.numberOfFieldValueConstraints),
		valueConstraint(e.valueConstraint)
{
}

//
// operator =
//
ProductHeader::fieldValueConstraintsList& 
ProductHeader::fieldValueConstraintsList::operator=(const fieldValueConstraintsList &e) 
{
	if (this != &e) {

		numberOfFieldValueConstraints = e.numberOfFieldValueConstraints ;
		valueConstraint = e.valueConstraint ;
	}
	return *this ;
}


//
// fieldPermittedValuesList struct
// -----------------------------------------------------------------
//
//
// constructor
//
// -----------------------------------------------------------------
ProductHeader::fieldPermittedValuesList::fieldPermittedValuesList():
	numberOfFieldValuesPermitted(0),
	permittedValue()
{
}

ProductHeader::fieldPermittedValuesList::~fieldPermittedValuesList()
{
}

//
// copy contructor
//
ProductHeader::fieldPermittedValuesList::fieldPermittedValuesList(const fieldPermittedValuesList &e):
	numberOfFieldValuesPermitted(e.numberOfFieldValuesPermitted),
	permittedValue(e.permittedValue)
{
}

//
// operator =
//
ProductHeader::fieldPermittedValuesList& 
ProductHeader::fieldPermittedValuesList::operator=(const fieldPermittedValuesList &e) 
{
	if (this != &e) {

		numberOfFieldValuesPermitted = e.numberOfFieldValuesPermitted ;
		permittedValue = e.permittedValue ;
	}
	return *this ;
}


//
// fieldValueConstraint struct
// -----------------------------------------------------------------
//
//
// constructor
//
// -----------------------------------------------------------------
ProductHeader::fieldValueConstraint::fieldValueConstraint():
	fieldValueConstraintContent(),
	numberOfFieldValueConstraintPositions(0),
	infoOnFieldValueConstraintPosition()
{
}

ProductHeader::fieldValueConstraint::~fieldValueConstraint()
{
}

//
// copy contructor
//
ProductHeader::fieldValueConstraint::fieldValueConstraint(const fieldValueConstraint &e):
	fieldValueConstraintContent(e.fieldValueConstraintContent),
	numberOfFieldValueConstraintPositions(e.numberOfFieldValueConstraintPositions),
	infoOnFieldValueConstraintPosition(e.infoOnFieldValueConstraintPosition)
{
}

//
// operator =
//
ProductHeader::fieldValueConstraint& 
ProductHeader::fieldValueConstraint::operator=(const fieldValueConstraint &e) 
{
	if (this != &e) {

		numberOfFieldValueConstraintPositions = e.numberOfFieldValueConstraintPositions ;
		fieldValueConstraintContent = e.fieldValueConstraintContent ;
		infoOnFieldValueConstraintPosition = e.infoOnFieldValueConstraintPosition;
		
	}
	return *this ;
}
	
_ACS_END_NAMESPACE
