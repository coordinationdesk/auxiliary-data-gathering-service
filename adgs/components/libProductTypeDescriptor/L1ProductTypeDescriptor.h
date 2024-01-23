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
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.1  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.5  2003/10/28 09:35:17  marfav
	Using isValidL1Filename and isValidL1Root
	
	Revision 1.4  2003/09/29 14:00:50  marfav
	aligned with L0ProductTypeDescriptor.C
	removed method definitions not requested for L1 processing (micepi)
	

*/
	/**modi by micepi: 
	 * aligned with L0ProductTypeDescriptor.h 
	 * removed methods declarations and attributes not requested for L1
	 */

#ifndef _L1ProductTypeDescriptor_H_
#define _L1ProductTypeDescriptor_H_

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>
#include <ProductTypeDescriptor.h>
#include <L1ProductInfo.h>
#include <map>

#include <ProductFileName.h>
#include <ProductFileNameFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

class L1ProductTypeDescriptor : public ProductTypeDescriptor
{
public:

	/*! class L1ProductTypeDescriptorException declaration */
	exDECLARE_EXCEPTION(L1ProductTypeDescriptorException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 *    
	 */	
	L1ProductTypeDescriptor();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~L1ProductTypeDescriptor() throw();

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
  	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
  	    return _fileNamesHandler->isValidL1Name(name);
    };


	/**
	 * \brief  Return the ordered Product Id list as specified in configuration file 
	 *    
	 */
	virtual const std::vector<std::string> &getOrderedProductIds() const { return _l1ProductIds; } // PRQA S 2131

//////////////////////////
//LeveL1 Specific Methods
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
 	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    return _fileNamesHandler->isValidL1Root(name);
    };

	/**
	 * \brief Return a Product file name given a Product name,
	 *		  start/stop dates in string format and the file extension
	 */
	virtual void getName(const std::string& root,  // PRQA S 2131
				        const std::string& startDate, 
				        const std::string& stopDate, 
				        const std::string& ext, 
				        std::string& result)
    {
	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    _fileNamesHandler->getName(root,startDate,stopDate,ext,result);
    }

	/**
	 * \brief Return a Product file name given a Product name root,
	 *		  start/stop dates in UTC double format and the file extension
	 */
	virtual void getName(const std::string& root,  // PRQA S 2131
						double startDate, 
						double stopDate, 
						const std::string& ext, 
						std::string& result)
    {
	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    _fileNamesHandler->getName(root,startDate,stopDate,ext,result);
    }

	/**
	 * \brief  Return Satellite name.
	 *
	 */
	const std::string& getSatelliteName() const throw() { return _satelliteName; }

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
	 * \brief  Return LeveL1 Default File Name Version.
	 *
	 */
	virtual std::string getDefaultFileNameVersion() // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    return _fileNamesHandler->getDefaultFileNameVersion();
    }

	/**
	 * \brief  Return the Product Id given a 'short' Product File Name Root.Ext
	 *		   If the file name is not a valid Level0 Product File Name Root an exception is thrown.
	 */
	virtual std::string getProductIDFromRoot(const std::string& name) // PRQA S 2131
    {
  	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    return _fileNamesHandler->getProductIDFromRoot(name);
    }


	/**
	 * \brief  Return the Product Id given a 'complete' Product File Name.
	 *		   If the file name is not a valid Level0 Product File Name an exception is thrown.
	 */
	virtual std::string getProductIDFromName(const std::string& name) // PRQA S 2131
    {
	    ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    return _fileNamesHandler->getProductIDFromName(name);
    }


	/**
	 * \brief  Return Ref Doc name.
	 *    
	 */
	const std::string& getRefDocName() const throw() {return _refDocName;} ;

	/**
	 * \brief  Return the Mission ID.
	 *
	 */
	virtual std::string getMissionID() // PRQA S 2131
    {
      ACS_COND_THROW(!_satFlag, L1ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
      return _fileNamesHandler->getMissionID();
    }


private:

	/**
	 * \brief  Copy constructor not implemented
	 *    
	 */
	L1ProductTypeDescriptor(const L1ProductTypeDescriptor & );


	/**
	 * \brief  Operator= not implemented
	 *    
	 */
	L1ProductTypeDescriptor &operator=(const L1ProductTypeDescriptor &);



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
	std::map<std::string, L1ProductInfo*>	_productInfo;
	std::vector<std::string>				_l1ProductIds;
	ProductFileName*						_fileNamesHandler;
	ProductFileNameFactory					_fileNameFactoryObject;
	bool									_satFlag;		

	ACS_CLASS_DECLARE_DEBUG_LEVEL(L1ProductTypeDescriptor) ;

};


_ACS_END_NAMESPACE


#endif //_L1ProductTypeDescriptor_H_
