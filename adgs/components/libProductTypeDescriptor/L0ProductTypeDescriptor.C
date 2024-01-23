// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.7  2014/03/26 09:57:36  marpas
	coding best practices applied
	warnings fixed (qa & g++)
	
	Revision 2.6  2013/12/09 14:01:12  giucas
	Library completely revisited.
	

*/

#include <L0ProductTypeDescriptor.h>
#include <Filterables.h>
#include <vector>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(L0ProductTypeDescriptor)

//default class constructor
L0ProductTypeDescriptor::L0ProductTypeDescriptor() :
    ProductTypeDescriptor(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_refDocName(),
	_satelliteName(),
	_acqRepFileType(),
	_sourcePacketHeaderSize(0),
	_annotationSize(0),
	_productInfo(),
	_l0ProductIds(),
	_fileNamesHandler(0),
	_fileNameFactoryObject(),
	_satFlag(false),
	_annotationIsInUTC(true)
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Id :" << Id);
}


//class constructor
L0ProductTypeDescriptor::L0ProductTypeDescriptor(const string& satName) :
    ProductTypeDescriptor(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_refDocName(),
	_satelliteName(),
	_acqRepFileType(),
	_sourcePacketHeaderSize(0),
	_annotationSize(0),
	_productInfo(),
	_l0ProductIds(),
	_fileNamesHandler(0),
	_fileNameFactoryObject(),
	_satFlag(false),
	_annotationIsInUTC(true)
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Id :" << Id);
	
	L0ProductTypeDescriptor::setSatellite(satName);
	
	init();
}


//Class destructor
L0ProductTypeDescriptor::~L0ProductTypeDescriptor() throw()
{
	if(_fileNamesHandler!=0) {
		delete _fileNamesHandler;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////
//base class methods implementation
////////////////////////////////////

void L0ProductTypeDescriptor::getNamesByApidVcid(unsigned int apid, 
			 						             unsigned int vcid,
			 						             vector<string>& result)
{
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	bool exit=false;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end())
		  && (((*it).second).getVCID() == vcid) )
		{
			_fileNamesHandler->getNamesById(it->first,result);		
		    exit=true;
		}
		it++;
	}	
}

void L0ProductTypeDescriptor::getNameByApidVcidClass(unsigned int apid, 
			 						             	  unsigned int vcid,
			 						             	  const string& fClass,
												 	  string& result)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getNameByApidVcidClass : method called with input parameters APID=" << apid 
			<< " - VCID=" << vcid << " - FileClass=" << fClass ) ;

	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	bool exit=false;
	vector<string> fileNameRoots;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end())
		  && (((*it).second).getVCID() == vcid) )
		{
			_fileNamesHandler->getNamesById(it->first,fileNameRoots);	
            size_t fnr_size = fileNameRoots.size() ;
		    for(unsigned int i=0; i<fnr_size; i++)
			{
				if(fClass == _fileNamesHandler->getFileClassFromRoot(fileNameRoots[i]))
				{
					result = fileNameRoots[i];
					break;
				}
			}
			exit=true;
		}
		it++;
	}	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getNameByApidVcidClass : result is '" << result << "'" );
}

void L0ProductTypeDescriptor::getNameByApidVcidClassOrig(unsigned int apid, 
			 											 unsigned int vcid,
			 											 const string& fClass, 
														 const string& originator,
														 string& result)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getNameByApidVcidClassOrig : method called with input parameters APID=" << apid 
			<< " - VCID=" << vcid << " - FileClass=" << fClass << " - Originator = " << originator );

	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	bool exit=false;
	vector<string> fileNameRoots;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end()) && (((*it).second).getVCID() == vcid) )
		{
		    _fileNamesHandler->getNamesById(it->first,fileNameRoots);		
            size_t fnr_size = fileNameRoots.size() ;
		    for(unsigned int i=0; i<fnr_size; i++)
			{
				if(	fClass == _fileNamesHandler->getFileClassFromRoot(fileNameRoots[i]) 
					&& originator == _fileNamesHandler->getOriginatorFromRoot(fileNameRoots[i])
				   )
				{
					result = fileNameRoots[i];
					break;
				}
			}
			exit=true;
		}
		it++;
	}	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getNameByApidVcidClass : result is '" << result << "'" );
}


void L0ProductTypeDescriptor::getNamesById(const string& prodId, 
										   vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	//check if prodId is a valid Product Id
	if( _productInfo.find(prodId) == _productInfo.end() )
	{
		ACS_THROW(L0ProductTypeDescriptorException("Incorrect ProductId " + prodId)) ; // PRQA S 3081
	}
	_fileNamesHandler->getNamesById(prodId,names);	
}

void L0ProductTypeDescriptor::getNamesById(const string& prodId, 
								           const string& startTime,
								           const string& stopTime,
								  		   vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	//check if prodId is a valid Product Id
	if( _productInfo.find(prodId) == _productInfo.end() )
	{
		ACS_THROW(L0ProductTypeDescriptorException("Incorrect ProductId " + prodId)) ; // PRQA S 3081
	}
	
	_fileNamesHandler->getNamesById(prodId,startTime,stopTime,names);
}



void L0ProductTypeDescriptor::getNamesByMission(const string& missionId, 
												vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByMission(missionId,it->first,names);
		it++;	
	}
}

void L0ProductTypeDescriptor::getNamesByMission(const string& missionId, 
									            const string& startTime,
								  	   			const string& stopTime,
									   			vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
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

void L0ProductTypeDescriptor::getNamesByExtension(const string& ext, 
												  vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		_fileNamesHandler->getNamesByExtension(ext,it->first,names);
		it++;
	}	
}

void L0ProductTypeDescriptor::getNamesByExtension(const string& ext, 
										 		  const string& startTime,
								  	     		  const string& stopTime,
										 		  vector<string>& names)
{
	//check for satellite setting
	ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
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
//Level0 Specific Methods implementation		
/////////////////////////////////////////

void L0ProductTypeDescriptor::setSatellite(const string& satName)
{
	if(_satFlag)
	{
		delete _fileNamesHandler;
		_fileNamesHandler = 0;
	}
	_fileNamesHandler = _fileNameFactoryObject.newObject(satName);
	ACS_COND_THROW(!_fileNamesHandler, L0ProductTypeDescriptorException("Error in instantiating ProductFileName for satellite "+satName) ); // PRQA S 3081
	_satFlag = true;
}

string L0ProductTypeDescriptor::getProductIdByApidVcid(unsigned int apid, 
									 				   unsigned int vcid)
{
	string result="";
	bool exit=false;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end()) && (((*it).second).getVCID() == vcid) )
		{
		  result = (*it).first;
		  exit=true;
		}
		it++;
	}
	if(!exit)
	{
		ostringstream msg;
		msg << "Error : No Product Id found. Bad APID " << apid << " and/or VCID " << vcid;
		ACS_THROW(L0ProductTypeDescriptorException(msg.str())); // PRQA S 3081
	}
	return result;
}


unsigned int L0ProductTypeDescriptor::getDataFieldHeaderLenByApidVcid(unsigned int apid, 
									 						          unsigned int vcid)
{
	unsigned int result=0;
	bool exit=false;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end()) 
		  && (((*it).second).getVCID() == vcid) )
		{
		  result = ((*it).second).getDataFieldHeaderLength();
		  exit=true;
		}
		it++;
	}
	if(!exit)
	{
		ostringstream msg;
		msg << "Error : No DataFieldHeaderLength found. Bad APID "
			<< apid
			<< " or VCID "
			<< vcid;
		ACS_THROW(L0ProductTypeDescriptorException(msg.str())); // PRQA S 3081
	}
	return result;
}

unsigned int L0ProductTypeDescriptor::getApplicationDataLenByApidVcid(unsigned int apid, 
									 						          unsigned int vcid)
{
	unsigned int result=0;
	bool exit=false;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end()) 
		  && (((*it).second).getVCID() == vcid) )
		{
		  result = ((*it).second).getApplicationDataLength();
		  exit=true;
		}
		it++;
	}
	if(!exit)
	{
		ostringstream msg;
		msg << "Error : No ApplicationDataLength found. Bad APID "
			<< apid
			<< " or VCID "
			<< vcid;
		ACS_THROW(L0ProductTypeDescriptorException(msg.str())); // PRQA S 3081
	}
	return result;
}


unsigned int L0ProductTypeDescriptor::getErrorControlByApidVcid(unsigned int apid, 
									 						    unsigned int vcid)
{
	unsigned int result=0;
	bool exit=false;
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != _productInfo.end()) )
	{
		apids = ((*it).second).getAPIDList();
		vector<unsigned int>::iterator res = find(apids.begin(),apids.end(),apid);
		
		if(  (res != apids.end()) 
		  && (((*it).second).getVCID() == vcid) )
		{
		  result = ((*it).second).getPacketErrorControlLength();
		  exit=true;
		}
		it++;
	}
	if(!exit)
	{
		ostringstream msg;
		msg << "Error : No Packet Error Control Length found. Bad APID "
			<< apid
			<< " or VCID "
			<< vcid;
		ACS_THROW(L0ProductTypeDescriptorException(msg.str())); // PRQA S 3081
	}
	return result;
}


unsigned int L0ProductTypeDescriptor::getVersion(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getVersion();
}

unsigned int L0ProductTypeDescriptor::getDataFieldHeaderLength(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getDataFieldHeaderLength();
}

unsigned int L0ProductTypeDescriptor::getPacketErrorControlLength(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getPacketErrorControlLength();
}


unsigned int L0ProductTypeDescriptor::getApplicationDataLength(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getApplicationDataLength();
}

vector<unsigned int> L0ProductTypeDescriptor::getAPID(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getAPIDList();
}

unsigned int L0ProductTypeDescriptor::getVCID(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getVCID();
}

unsigned int L0ProductTypeDescriptor::getChannel(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getChannel();
}

string L0ProductTypeDescriptor::getSensorName(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getSensorName();
}

string L0ProductTypeDescriptor::getSensorMode(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getSensorMode();
}

bool L0ProductTypeDescriptor::getSegmentedPacketsFlag(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getSegmentedPacketsFlag();
}


double L0ProductTypeDescriptor::getDeltaNominalTime(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getDeltaNominalTime();
}


double L0ProductTypeDescriptor::getGapThresold(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getGapThresold();
}

string L0ProductTypeDescriptor::getDescription(const string& prodId)
{
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("LIncorrect ProductId " + prodId) ) ; // PRQA S 3081
	return ((*it).second).getDescription();
}

vector<unsigned int> L0ProductTypeDescriptor::getAPIDByVCID(unsigned int vcid)
{
	vector<unsigned int> result ;	
	map<string, L0ProductInfo>::iterator it = _productInfo.begin();
	while( it != _productInfo.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getAPIDByVCID : Scanning product with VCID: " << ((*it).second).getVCID() );
    
		if(((*it).second).getVCID() == vcid)//the product scanned has the VCID specified in input
		{
			//retrieve the list of APIDs associated to that product
			vector<unsigned int> apids = ((*it).second).getAPIDList();
			//scan the list of APIDs and add an APID to the output list ONLY IF the APID IS NOT PRESENT
			size_t ap_size = apids.size() ;
            for(unsigned int i=0; i < ap_size; i++)
			{
				if( find(result.begin(), result.end(),apids[i]) == result.end() ) {
					result.push_back(apids[i]);
				}
			}
		}
		it++;
	}
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        size_t r_size = result.size(); 
		for(unsigned int i=0; i < r_size; i++)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getAPIDByVCID : APID["  << i << "] = " << result[i] );
		}
	ACS_CLASS_END_DEBUG
	return result;
}

bool L0ProductTypeDescriptor::isTFL0FromRoot(const string& fileName) // PRQA S 4020
{
	string prodId = getProductIDFromRoot(fileName);
	vector<unsigned int> apids = getAPID(prodId);
	vector<unsigned int>::iterator res = find(apids.begin(), apids.end(), 0);
	//HTMK APID is 0 : check input prodId APID
	if( res != apids.end() ) {
		return true;
	}
	return false;
}


bool L0ProductTypeDescriptor::isTFL0FromName(const string& fileName)// PRQA S 4020
{
	string prodId = this->getProductIDFromName(fileName);
	vector<unsigned int> apids = this->getAPID(prodId);
	vector<unsigned int>::iterator res = find(apids.begin(), apids.end(), 0);	
	//HTMK APID is 0 : check input prodId APID
	if( res != apids.end() ) {
		return true;
	}
	return false;
}

vector<ProductField> L0ProductTypeDescriptor::getFields(const string& prodId)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getFields called for prodId '" << prodId << "'");

	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("Incorrect ProductId " + prodId) );// PRQA S 3081

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg;
		msg << "L0ProductTypeDescriptor::getFields returning : ";
        size_t gp_size = ((*it).second.getProductFields()).size() ;
		for(unsigned int i=0; i<gp_size; ++i) {
			msg << ((*it).second.getProductFields())[i];
        }
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,msg.str());
	ACS_CLASS_END_DEBUG
	
	return (*it).second.getProductFields();
}

void L0ProductTypeDescriptor::getSliceParams(const std::string& prodId, double& sliceLength, double& sliceOverlap)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getSliceParams called for prodId '" << prodId << "'" ) ;
	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::iterator it = _productInfo.find(prodId);
	ACS_COND_THROW( it == _productInfo.end() , L0ProductTypeDescriptorException("Incorrect ProductId " + prodId) ); // PRQA S 3081
	(*it).second.getSliceParams(sliceLength,sliceOverlap) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getSliceParams return values sliceLength '" << sliceLength << "'" 
			<< " sliceOverlap '" << sliceOverlap << "'" );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////

//read data from rsResourceSet and set private attributes
void L0ProductTypeDescriptor::init()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	//storing ProductIds and ProductInfo objects
	unsigned int numberOfFiles = 0;
	_rsConf.getValue("L0ProductDescriptor.NumberOfProducts", numberOfFiles);
	string validName="";
	for (unsigned int i=1; i <= numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "L0ProductDescriptor.Product" + temp + ".Id";
		_rsConf.getValue(key,productID);
		_l0ProductIds.push_back(productID);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loading key " << key  << " with value " << productID );
		L0ProductInfo prInfo ;
		_rsConf.getValue("L0ProductDescriptor.Product"+temp,prInfo);
		_productInfo[productID] = prInfo;
	}
	
	//storing other general informations
	_rsConf.getValue("L0ProductDescriptor.SourcePacketHeaderSize", _sourcePacketHeaderSize);
	_rsConf.getValue("L0ProductDescriptor.AnnotationSize", _annotationSize);
	try{
		_rsConf.getValue("L0ProductDescriptor.AnnotationIsInUTC", _annotationIsInUTC);
	}
	catch(exception &e)
	{
		_annotationIsInUTC = true;
	}
	_rsConf.getValue("L0ProductDescriptor.RefDocName", _refDocName);
	_rsConf.getValue("L0ProductDescriptor.AcquisitionReportFileType", _acqRepFileType);
	_rsConf.getValue("L0ProductDescriptor.SatelliteName", _satelliteName);
} 

_ACS_END_NAMESPACE

