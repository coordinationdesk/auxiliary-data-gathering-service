// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

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
	
	Revision 1.1  2004/05/06 17:16:11  envisat
	Added for new class BWSProductTypeDescriptor - micepi
	
	

*/
	/**modi by micepi: 
	 * aligned with L0ProductTypeDescriptor.h 
	 * removed methods declarations and attributes not requested for BWS
	 */

#ifndef _BWSProductTypeDescriptor_H_
#define _BWSProductTypeDescriptor_H_

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>
#include <ProductTypeDescriptor.h>
#include <BWSProductInfo.h>
#include <map>

#include <ProductFileName.h>
#include <ProductFileNameFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

class BWSProductTypeDescriptor : public ProductTypeDescriptor
{
public:

	/*! class BWSProductTypeDescriptorException declaration */
	exDECLARE_EXCEPTION(BWSProductTypeDescriptorException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 *    
	 */	
	BWSProductTypeDescriptor();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~BWSProductTypeDescriptor() throw();


/////////////////////////////////////////////		
//public methods
/////////////////////////////////////////////


//////////////////////////////////
//base class methods declaration
//////////////////////////////////

	/**
	 * \brief Return a list of Product Names given a Product Id  
	 *    
	 */
	virtual void getNamesById(const std::string&, std::vector<std::string>&);

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product Id  
	 *    
	 */
	virtual void getNamesById(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);

	/**
	 * \brief Return a list of Product Names given a Mission Id  
	 *    
	 */
	virtual void getNamesByMission(const std::string&, std::vector<std::string>&);

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Mission Id  
	 *    
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);
	/**
	 * \brief Return a list of Product Names given a Product file extension  
	 *    
	 */
	virtual void getNamesByExtension(const std::string&, std::vector<std::string>&);

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product file extension  
	 *    
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);

	/**
	 * \brief  Return true if the string in input represent a valid Product name, false otherwise. 
	 *    
	 */
	virtual bool isValidName(const std::string& name) // PRQA S 2131
    {
      ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
      return _fileNamesHandler->isValidBWSName(name);
    };



	/**
	 * \brief  Return the ordered Product Id list as specified in configuration file 
	 *    
	 */
	virtual const std::vector<std::string> &getOrderedProductIds() const { return _bwsProductIds; } // PRQA S 2131



//////////////////////////
//LeveBWS Specific Methods
//////////////////////////
	/**
	 * \brief Return the Satellite name in ACS syntax(First Upper - Rest Lower)
	 */
	std::string setSatelliteAcsName() const;

	/**
	 * \brief Set the current satellite name
	 * \param Satellite name as string (i.e. Cryosat for CRYOSAT or Envisat for ENVISAT)
	 */
	void setSatellite(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid Root name, false otherwise. 
	 */
	virtual bool isValidRoot(const std::string& name) // PRQA S 2131
    {
        ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")) ; // PRQA S 3081
        return _fileNamesHandler->isValidBWSRoot(name);
    };

	/**
	 * \brief Return a Product file name given a Product name,
	 *		  start/stop dates in string format and the file extension
	 */
	void getName(const std::string& root, // PRQA S 2131, 4211
				 const std::string& startDate, 
				 const std::string& stopDate, 
				 const std::string& ext, 
				 std::string& result)
    {
	    ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")); // PRQA S 3081
	    _fileNamesHandler->getName(root,startDate,stopDate,ext,result);
    }

	/**
	 * \brief Return a Product file name given a Product name root,
	 *		  start/stop dates in UTC double format and the file extension
	 */
	virtual void getName(const std::string& root, // PRQA S 2131
						double startDate, 
						double stopDate, 
						const std::string& ext, 
						std::string& result)
    {
	    ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")); // PRQA S 3081
	    _fileNamesHandler->getName(root,startDate,stopDate,ext,result);
    }

	/**
	 * \brief  Return Satellite name.
	 *
	 */
	virtual const std::string& getSatelliteName() { return _satelliteName; } // PRQA S 2131

	/**
	 * \brief  Return Sensor name for a given Product ID.
	 *
	 */
	std::string getSensorName(const std::string&);

	/**
	 * \brief  Return Sensor mode for a given Product ID.
	 *
	 */
	std::string getSensorMode(const std::string&);

	/**
	 * \brief  Return LeveBWS Default File Name Version.
	 *
	 */
	virtual std::string getDefaultFileNameVersion() // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")); // PRQA S 3081
	    return _fileNamesHandler->getDefaultFileNameVersion();
    }

	/**
	 * \brief  Return the Product Id given a 'short' Product File Name Root.Ext
	 *		   If the file name is not a valid Level0 Product File Name Root an exception is thrown.
	 */
	virtual std::string getProductIDFromRoot(const std::string& name) // PRQA S 2131
    {
        ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")); // PRQA S 3081
	    return _fileNamesHandler->getProductIDFromRoot(name);
    }

	/**
	 * \brief  Return the Product Id given a 'complete' Product File Name.
	 *		   If the file name is not a valid Level0 Product File Name an exception is thrown.
	 */
	virtual std::string getProductIDFromName(const std::string& name)// PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")); // PRQA S 3081
	    return _fileNamesHandler->getProductIDFromName(name);
    }


	/**
	 * \brief  Return Ref Doc name.
	 *    
	 */
	virtual std::string getRefDocName() { return _refDocName; } // PRQA S 2131


	/**
	 * \brief  Return the Mission ID.
	 *
	 */
	virtual std::string getMissionID() // PRQA S 2131
    {
        ACS_COND_THROW(!_satFlag,BWSProductTypeDescriptorException("Satellite name not set")); // PRQA S 3081
        return _fileNamesHandler->getMissionID();
    }


private:
	/**
	 * \brief  Copy constructor
	 *    
	 */
	BWSProductTypeDescriptor(const BWSProductTypeDescriptor & );


	/**
	 * \brief  Operator=
	 *    
	 */
	BWSProductTypeDescriptor &operator=(const BWSProductTypeDescriptor &);


/////////////////////////////////////////////		
//private methods
/////////////////////////////////////////////

	/**
	 * \brief init info
	 */
	void init();

/////////////////////////////////////////////		
//private attributes
/////////////////////////////////////////////

	rsResourceSet&	_rsConf;			
	std::string		_refDocName;
	std::string		_satelliteName;
	std::map<std::string, BWSProductInfo*>	_productInfo;
	std::vector<std::string>				_bwsProductIds;
	ProductFileName*						_fileNamesHandler;
	ProductFileNameFactory					_fileNameFactoryObject;
	bool									_satFlag;		

	ACS_CLASS_DECLARE_DEBUG_LEVEL(BWSProductTypeDescriptor) ;

};

//////////////////
// inline methods
//////////////////




_ACS_END_NAMESPACE


#endif //_BWSProductTypeDescriptor_H_
