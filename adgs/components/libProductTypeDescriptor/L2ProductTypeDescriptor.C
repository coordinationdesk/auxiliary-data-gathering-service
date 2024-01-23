// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.1  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.1  2004/02/05 16:23:04  marfav
	Import L2 support classes
	

*/

#include <L2ProductTypeDescriptor.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <TimeConverter.h>
#include <TimeConverterSingleton.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(L2ProductTypeDescriptor)

/*
 * Class constructors
 */
L2ProductTypeDescriptor::L2ProductTypeDescriptor() :
    ProductTypeDescriptor(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_refDocName(),
	_satelliteName(),
	_productInfo(),
	_l2ProductIds(),	
	_fileNamesHandler(0),
	_fileNameFactoryObject(),
	_satFlag(false)
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Id :" << Id);

	init();
}

/*
 * Class destructor
 */
L2ProductTypeDescriptor::~L2ProductTypeDescriptor() throw()
{
    // PRQA S 4631 L1
	//free pointers
	map<string, L2ProductInfo*>::iterator it = _productInfo.begin();
    size_t pi_size = _productInfo.size() ;
	for(unsigned int i=0; i<pi_size; i++){
		delete it->second;
		it++;
	}
	//clean map
	_productInfo.clear();
	delete _fileNamesHandler;
    // PRQA L:L1
}


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////
//base class methods implementation
////////////////////////////////////

void L2ProductTypeDescriptor::getNamesById(const string& prodId,
										   vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag, L2ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	//check if prodId is a valid Product Id
	ACS_COND_THROW( _productInfo.find(prodId) == _productInfo.end() , L2ProductTypeDescriptorException("Incorrect ProductId " + prodId) ); // PRQA S 3081
		
	_fileNamesHandler->getNamesById(prodId,names);	

}

void L2ProductTypeDescriptor::getNamesById(const string& prodId,
								           const string& startTime,
								           const string& stopTime,
								  		   vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag, L2ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	//check if prodId is a valid Product Id
	ACS_COND_THROW( _productInfo.find(prodId) == _productInfo.end() , L2ProductTypeDescriptorException("Incorrect ProductId " + prodId) ); // PRQA S 3081
	
	_fileNamesHandler->getNamesById(prodId,startTime,stopTime,names);
}



void L2ProductTypeDescriptor::getNamesByMission(const string& missionId,
												vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag, L2ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081

	map<string, L2ProductInfo*>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByMission(missionId,it->first,names);
		it++;	
	}
}

void L2ProductTypeDescriptor::getNamesByMission(const string& missionId,
									            const string& startTime,
								  	   			const string& stopTime,
									   			vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag, L2ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081

	map<string, L2ProductInfo*>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByMission(missionId,
											 it->first,
											 startTime,
											 stopTime,
											 names);		
		it++;
	}
}

void L2ProductTypeDescriptor::getNamesByExtension(const string& ext,
												  vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag, L2ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	
	map<string, L2ProductInfo*>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByExtension(ext,it->first,names);
		it++;
	}	
}

void L2ProductTypeDescriptor::getNamesByExtension(const string& ext,
										 		  const string& startTime,
								  	     		  const string& stopTime,
										 		  vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag, L2ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	map<string, L2ProductInfo*>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByExtension(ext,
											   it->first,
											   startTime,
											   stopTime,
											   names);		
		it++;
	}
}


/////////////////////////////////////////
//LeveL2 Specific Methods implementation
/////////////////////////////////////////

string L2ProductTypeDescriptor::setSatelliteAcsName() const
{
    return StringUtils::capitalize(_satelliteName) ;
}


void L2ProductTypeDescriptor::setSatellite(const string& satName)
{
	if(_satFlag)
	{
		delete _fileNamesHandler;
		_fileNamesHandler = 0;
	}
	_fileNamesHandler = _fileNameFactoryObject.newObject(satName);
	ACS_COND_THROW(!_fileNamesHandler, L2ProductTypeDescriptorException("Error in instantiating ProductFileName for satellite "+satName) ); // PRQA S 3081
	_satFlag = true;
}

string L2ProductTypeDescriptor::getSensorName(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L2ProductInfo*>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L2ProductTypeDescriptorException("Incorrect ProductId " + prodId) ); // PRQA S 3081
	return (*it).second->getSensorName();
}

string L2ProductTypeDescriptor::getSensorMode(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L2ProductInfo*>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L2ProductTypeDescriptorException("Incorrect ProductId " + prodId) ); // PRQA S 3081
	return (*it).second->getSensorMode();
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * read data from rsResourceSet and set private attributes
 */
void L2ProductTypeDescriptor::init()

{
	//storing ProductIds and ProductInfo objects
	unsigned int numberOfFiles = 0;
	_rsConf.getValue("L2ProductDescriptor.NumberOfProducts", numberOfFiles);

	for (unsigned int i=1; i <= numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "L2ProductDescriptor.Product" + temp + ".Id";
		_rsConf.getValue(key,productID);
		_l2ProductIds.push_back(productID);
		_productInfo[productID] = new L2ProductInfo("L2ProductDescriptor.Product" + temp);
	}

	//storing other general informations
	_rsConf.getValue("L2ProductDescriptor.RefDocName", _refDocName);
	_rsConf.getValue("L2ProductDescriptor.SatelliteName", _satelliteName);
}

_ACS_END_NAMESPACE


