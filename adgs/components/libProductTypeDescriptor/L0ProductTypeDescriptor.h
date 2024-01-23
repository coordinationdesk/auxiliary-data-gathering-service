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
	Revision 2.5  2013/12/09 14:01:12  giucas
	Library completely revisited.
	

*/

#ifndef _L0ProductTypeDescriptor_H_
#define _L0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <rsResourceSet.h>
#include <ProductTypeDescriptor.h>
#include <L0ProductInfo.h>
#include <ProductFileName.h>
#include <ProductFileNameFactory.h>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)

class L0ProductTypeDescriptor : public ProductTypeDescriptor // PRQA S 2109
{
public:

	exDECLARE_EXCEPTION(L0ProductTypeDescriptorException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	L0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit L0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~L0ProductTypeDescriptor() throw() ;

//////////////////////////////////
//public methods
//////////////////////////////////


//////////////////////////////////
//base class methods declaration
//////////////////////////////////

	/**
	 * \brief Return a list of Product Names given a Product Id  
	 */
	virtual void getNamesById(const std::string&, std::vector<std::string>&);

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product Id  
	 */
	virtual void getNamesById(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);


	/**
	 * \brief Return a list of Product Names given a Mission Id  
	 */
	virtual void getNamesByMission(const std::string&, std::vector<std::string>&);

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Mission Id  
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);


	/**
	 * \brief Return a list of Product Names given a Product file extension  
	 */
	virtual void getNamesByExtension(const std::string&, std::vector<std::string>&);


	/**
	 * \brief Return a list of Product Names ncluding Sensing Start and Stop times
	 *        given a Product file extension  
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);


	/**
	 * \brief  Return true if the string in input represent a valid Product name, false otherwise. 
	 */
	virtual bool isValidName(const std::string& name) // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    return _fileNamesHandler->isValidL0Name(name);
    }


	/**
	 * \brief  Return the ordered Product Id list as specified in configuration file 
	 */
	virtual const std::vector<std::string> &getOrderedProductIds() const { return _l0ProductIds; } ; // PRQA S 2131


//////////////////////////
//Level0 Specific Methods		
//////////////////////////

	/**
	 * \brief Set the current satellite name. Used to instatiate the correct object from the ProductFileName Factory
	 * \param Satellite name as string (i.e. Cryosat for CRYOSAT or Envisat for ENVISAT)  
	 */
	virtual void setSatellite(const std::string&);

	/**
	 * \brief  Return a list of Product file names given APID and VCID.
	 */
	virtual void getNamesByApidVcid(unsigned int,unsigned int,std::vector<std::string>&);

	/**
	 * \brief  Return a Product file name given APID and VCID and file class.
	 */
	virtual void getNameByApidVcidClass(unsigned int,unsigned int, const std::string&,std::string&);

	/**
	 * \brief  Return a Product file name given APID , VCID , file class and OriginatorId (USED ONLY FOR ENVISAT)
	 */
	virtual void getNameByApidVcidClassOrig(unsigned int,unsigned int,const std::string&,const std::string&,std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid Root name, false otherwise. 
	 */
	virtual bool isValidRoot(const std::string& name) // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    return _fileNamesHandler->isValidL0Root(name);
    }



	/**
	 * \brief  Return the Product Id given APID and VCID.
	 *    
	 */						
	virtual std::string getProductIdByApidVcid(unsigned int, unsigned int);

	/**
	 * \brief  Return the Data Field Header length given APID and VCID.
	 *    
	 */						
	virtual unsigned int getDataFieldHeaderLenByApidVcid(unsigned int, unsigned int);


	/**
	 * \brief  Return the Application Data length given APID and VCID.
	 *    
	 */						
	virtual unsigned int getApplicationDataLenByApidVcid(unsigned int, 
										         unsigned int);

	/**
	 * \brief  Return Packet Error Control Length given APID and VCID.
	 *    
	 */
	virtual unsigned int getErrorControlByApidVcid(unsigned int, 
										   unsigned int);

	/**
	 * \brief  Return packet Version for a given Product ID.
	 *    
	 */
	virtual unsigned int getVersion(const std::string&);

	/**
	 * \brief  Return Data Field Header Length for a given Product ID.
	 *    
	 */
	virtual unsigned int getDataFieldHeaderLength(const std::string&);

	/**
	 * \brief  Return Packet Error Control Length for a given Product ID.
	 *    
	 */
	virtual unsigned int getPacketErrorControlLength(const std::string&);

	/**
	 * \brief  Return Application Data Length for a given Product ID.
	 *    
	 */
	virtual unsigned int getApplicationDataLength(const std::string&);

	/**
	 * \brief  Return List of Application Process IDs for a given Product ID.
	 *    
	 */
	virtual std::vector<unsigned int> getAPID(const std::string&);

	/**
	 * \brief  Return Virtual Channel ID for a given Product ID.
	 *    
	 */
	virtual unsigned int getVCID(const std::string&);

	/**
	 * \brief  Return expected channel for a given Product ID.
	 *    
	 */
	virtual unsigned int getChannel(const std::string&);

	/**
	 * \brief  Return Satellite name.
	 *    
	 */
	virtual const std::string& getSatelliteName() const throw() { return _satelliteName;} // PRQA S 2131

	/**
	 * \brief  Return Sensor name for a given Product ID.
	 *    
	 */
	virtual std::string getSensorName(const std::string&);

	/**
	 * \brief  Return Sensor mode for a given Product ID.
	 *    
	 */
	virtual std::string getSensorMode(const std::string&);

	/**
	 * \brief Return true if packets are segmented false otherwise,for a given Product ID.
	 */
	virtual bool getSegmentedPacketsFlag(const std::string&);

	/**
	 * \brief  Return Delta Nominal Time for a given Product ID.
	 *    
	 */
	virtual double getDeltaNominalTime(const std::string&);

	/**
	 * \brief  Return Gap Thresold for a given Product ID.
	 */
	virtual double getGapThresold(const std::string&);

	/**
	 * \brief  Return Level0 SourcePacketHeader Size.
	 */
	virtual unsigned int getSourcePacketHeaderSize() const throw() {return _sourcePacketHeaderSize;}  // PRQA S 2131

	/**
	 * \brief  Return Level0 Annotation Size.
	 */
	virtual unsigned int getAnnotationSize() const throw() { return _annotationSize; } // PRQA S 2131

	/**
	 * \brief  Return Level0 Default File Name Version.
	 */
	virtual std::string getDefaultFileNameVersion() // PRQA S 2131
    { 
	    ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    return _fileNamesHandler->getDefaultFileNameVersion();
    }


	/**
	 * \brief  Return the list of APIDs associated to a given VCID as unsigned int
	 */
	virtual std::vector<unsigned int> getAPIDByVCID(unsigned int);

	/**
	 * \brief  Return the Product Id given a 'short' Product File Name Root.Ext
	 *		   If the file name is not a valid Level0 Product File Name Root an exception is thrown.
	 */
	virtual std::string getProductIDFromRoot(const std::string& name) // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    return _fileNamesHandler->getProductIDFromRoot(name);
    }


	/**
	 * \brief  Return the Product Id given a 'complete' Product File Name.
	 *		   If the file name is not a valid Level0 Product File Name an exception is thrown.
	 */
	virtual std::string getProductIDFromName(const std::string& name) // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    return _fileNamesHandler->getProductIDFromName(name);
    }

	/**
	 * \brief  Return Ref Doc name.
	 */
	virtual const std::string& getRefDocName() const throw() { return _refDocName; } // PRQA S 2131

	/**
	 * \brief  Return true if the file name root in input is a Transfer Frame Level0 file,
	 *		   false otherwise.	
	 */
	virtual bool isTFL0FromRoot(const std::string&) ;

	/**
	 * \brief  Return true if the file name in input is a Transfer Frame Level0 file,
	 *		   false otherwise.	
	 */
	virtual bool isTFL0FromName(const std::string&) ;

	/**
	 * \brief  Return the Acquisition Report file type
	 */
	virtual const std::string& getAcqReportFileType() const throw() {return _acqRepFileType;} // PRQA S 2131

	/**
	 * \brief  Return Description for a given Product ID.
	 *    
	 */
	virtual std::string getDescription(const std::string&);

	/**
	 * \brief  Return the Mission ID.
	 *    
	 */
	virtual std::string getMissionID() // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    return _fileNamesHandler->getMissionID();
    }

	/**
	 * \brief  Return the flag to indicate that Sensing Time in annotation is UTC.
	 *    
	 */
	virtual bool annotationIsInUTC() const throw() { return _annotationIsInUTC; } // PRQA S 2131
	
	/**
	 * \brief  Return the list of product fields configured for a specific product id.
	 * \param  Product ID
	 */
	std::vector<ProductField> getFields(const std::string&);

	/**
	 * \brief  Return the sliceLength and sliceOverlap parameters for a specific product id.
	 * \param  prodId Product ID
	 * \retval sliceLength for the given Product ID
	 * \retval sliceOverlap for the given Product ID
	 */
	void getSliceParams(const std::string& prodId, double& sliceLength, double& sliceOverlap);


protected:

	rsResourceSet& getRsConf() throw() { return _rsConf; } // PRQA S 4024
	const std::map<std::string, L0ProductInfo> &getProductInfo() const throw()  { return _productInfo; } 
	ProductFileName* getFileNamesHandler() const throw() { return _fileNamesHandler; } 
	bool getSatFlag() const throw() { return _satFlag; } 
	
private:

	/**
	 * \brief  Copy constructor not implemented
	 */
	L0ProductTypeDescriptor(const L0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator= not implemented
	 */
	L0ProductTypeDescriptor &operator=(const L0ProductTypeDescriptor &);


private:

	rsResourceSet&							_rsConf;
	std::string								_refDocName;
	std::string								_satelliteName;
	std::string								_acqRepFileType;
	unsigned int 							_sourcePacketHeaderSize;
	unsigned int 							_annotationSize;
	std::map<std::string, L0ProductInfo> 	_productInfo;
	std::vector<std::string>				_l0ProductIds;
	ProductFileName*						_fileNamesHandler;
	ProductFileNameFactory    				_fileNameFactoryObject;
	bool									_satFlag;
	bool									_annotationIsInUTC;


/////////////////////////////////////////////		
//private methods
/////////////////////////////////////////////

	void init();

/////////////////////////////////////////////		
//private attributes
/////////////////////////////////////////////

	ACS_CLASS_DECLARE_DEBUG_LEVEL(L0ProductTypeDescriptor) ;

};


	
_ACS_END_NAMESPACE


#endif //_L0ProductTypeDescriptor_H_
