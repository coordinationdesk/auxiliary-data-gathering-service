// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 2.2  2013/12/09 14:01:10  giucas
	Library completely revisited.
	
	Revision 2.1  2012/03/12 10:19:18  giucas
	Some compile warnings fixed
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.1  2004/05/06 17:13:33  envisat
	Added new class BWSProductTypeDescriptor (Factory Implemented) - micepi
	
	Revision 1.4  2003/09/29 14:00:50  marfav
	aligned with L0ProductTypeDescriptor.C
	removed method definitions not requested for BWS processing (micepi)
	


*/
	/**modi by micepi: 
	 * aligned with L0ProductTypeDescriptor.C
	 * removed method definitions not requested for BWS processing
	 */

#include <BWSProductTypeDescriptor.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>
#include <StringUtils.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(BWSProductTypeDescriptor)

/*
 * Class constructors
 */
BWSProductTypeDescriptor::BWSProductTypeDescriptor() :
    ProductTypeDescriptor(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_refDocName(),
	_satelliteName(),
	_productInfo(),	
	_bwsProductIds(),
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
BWSProductTypeDescriptor::~BWSProductTypeDescriptor() throw()
{
    // PRQA S 4631 L1
	//free pointers
	map<string, BWSProductInfo*>::iterator it = _productInfo.begin();
    size_t pi_size = _productInfo.size() ;
	for(unsigned int i=0; i<pi_size; i++){
		delete it->second;
		++it;
	}
	//clean map
	_productInfo.clear() ;
	delete _fileNamesHandler;

    // PRQA L:L1
}


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////
//base class methods implementation
////////////////////////////////////

void BWSProductTypeDescriptor::getNamesById(const string& prodId,
										   vector<string>& names)
{
	//check for satellite setting
	if(!_satFlag)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
	}

	//check if prodId is a valid Product Id
	if( _productInfo.find(prodId) == _productInfo.end() )
	{
		ACS_THROW(BWSProductTypeDescriptorException("Incorrect ProductId " + prodId)) ; // PRQA S 3081
	}
		
	_fileNamesHandler->getNamesById(prodId,names);	
}

void BWSProductTypeDescriptor::getNamesById(const string& prodId,
								           const string& startTime,
								           const string& stopTime,
								  		   vector<string>& names)
{
	//check for satellite setting
	if(!_satFlag)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
	}

	//check if prodId is a valid Product Id
	if( _productInfo.find(prodId) == _productInfo.end() )
	{
		ACS_THROW(BWSProductTypeDescriptorException("Incorrect ProductId " + prodId)) ; // PRQA S 3081
	}
	
	_fileNamesHandler->getNamesById(prodId,startTime,stopTime,names);
}



void BWSProductTypeDescriptor::getNamesByMission(const string& missionId,
												vector<string>& names)
{
	//check for satellite setting
	if(!_satFlag)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
	}

	map<string, BWSProductInfo*>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByMission(missionId,it->first,names);
		it++;	
	}
}

void BWSProductTypeDescriptor::getNamesByMission(const string& missionId,
									            const string& startTime,
								  	   			const string& stopTime,
									   			vector<string>& names)
{
	//check for satellite setting
	if(!_satFlag)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
	}

	map<string, BWSProductInfo*>::iterator it = _productInfo.begin();
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

void BWSProductTypeDescriptor::getNamesByExtension(const string& ext,
												  vector<string>& names)
{
	//check for satellite setting
	if(!_satFlag)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
	}
	
	map<string, BWSProductInfo*>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByExtension(ext,it->first,names);
		it++;
	}	
}

void BWSProductTypeDescriptor::getNamesByExtension(const string& ext,
										 		  const string& startTime,
								  	     		  const string& stopTime,
										 		  vector<string>& names)
{
	//check for satellite setting
	if(!_satFlag)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
	}
	map<string, BWSProductInfo*>::iterator it = _productInfo.begin();
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
//LeveBWS Specific Methods implementation
/////////////////////////////////////////

string BWSProductTypeDescriptor::setSatelliteAcsName() const
{
    return StringUtils::capitalize(_satelliteName) ;
}


void BWSProductTypeDescriptor::setSatellite(const string& satName)
{
	if(_satFlag)
	{
		delete _fileNamesHandler;
		_fileNamesHandler = 0;
	}
	_fileNamesHandler = _fileNameFactoryObject.newObject(satName);
	if(!_fileNamesHandler)
	{
		ACS_THROW(BWSProductTypeDescriptorException("Error in instantiating ProductFileName for satellite "+satName)); // PRQA S 3081
	}
	_satFlag = true;
}

string BWSProductTypeDescriptor::getSensorName(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, BWSProductInfo*>::iterator it = _productInfo.find(prodId);
	if( it == _productInfo.end() ){
		ACS_THROW(BWSProductTypeDescriptorException("Incorrect ProductId " + prodId)); // PRQA S 3081
	}
	return (*it).second->getSensorName();
}

string BWSProductTypeDescriptor::getSensorMode(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, BWSProductInfo*>::iterator it = _productInfo.find(prodId);
	if( it == _productInfo.end() ){
		ACS_THROW(BWSProductTypeDescriptorException("Incorrect ProductId " + prodId)); // PRQA S 3081
	}
	return (*it).second->getSensorMode();
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * read data from rsResourceSet and set private attributes
 */
void BWSProductTypeDescriptor::init()

{
	//storing ProductIds and ProductInfo objects
	unsigned int numberOfFiles = 0 ;
	_rsConf.getValue("BWSProductDescriptor.NumberOfProducts", numberOfFiles);

	for (unsigned int i=1; i <= numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "BWSProductDescriptor.Product" + temp + ".Id";
		_rsConf.getValue(key,productID);
		_bwsProductIds.push_back(productID);
		_productInfo[productID] = new BWSProductInfo("BWSProductDescriptor.Product" + temp);
	}

	//storing other general informations
	_rsConf.getValue("BWSProductDescriptor.RefDocName", _refDocName);
	_rsConf.getValue("BWSProductDescriptor.SatelliteName", _satelliteName);
}


_ACS_END_NAMESPACE
