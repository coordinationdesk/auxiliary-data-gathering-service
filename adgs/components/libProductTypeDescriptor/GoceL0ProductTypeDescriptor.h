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
	Revision 2.1  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2006/02/27 14:38:00  giucas
	getOrderedProductIds method implemented
	
	Revision 1.3  2005/01/14 14:27:19  giucas
	Added addAPIDList method to add APIDs runtime for a specific filetype
	
	Revision 1.2  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.1  2004/08/02 16:04:06  giucas
	Imported source.
	


*/

#ifndef _GoceL0ProductTypeDescriptor_H_
#define _GoceL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)

class GoceL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{

public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	GoceL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit GoceL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~GoceL0ProductTypeDescriptor() throw();


//////////////////////////
//Level0 Specific Methods		
//////////////////////////

	/**
	 * \brief  Return the list of product fields configured for a specific product id.
	 * \param  Product ID
	 */
	std::vector<ProductField> getFields(const std::string&);

	/**
	 * \brief  Return a list of lists of product fields given an APID
	 * \param  An APID
	 */
	std::vector< std::pair< std::string, std::vector<ProductField> > > getFieldsByApidVcid(unsigned int,unsigned int);

	/**
	 * \brief  Return the list of product fields configured for a specific product id.
	 * \param  Product ID
	 */
	std::string getProdIdByApidFields(unsigned int,const std::vector<ProductField>&);

	/**
	 * \brief Return a Product file name given a Product name,
	 *		  start/stop dates in string format and the file extension
	 */
	virtual void getName(const std::string& root, // PRQA S 2131
						const std::string& startDate, 
						const std::string& stopDate, 
						const std::string& ext, 
						std::string& result)
    {
	    ACS_COND_THROW(!getSatFlag() , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,ext,result);
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
	    ACS_COND_THROW(!getSatFlag() , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,ext,result);
    }


	/**
	 * \brief  Return a Product file name given APID, list of Fields and file class.
	 */
	void getNameByApidFieldsClass(unsigned int, 
								  const std::vector<ProductField>&,
								  const std::string&,
								  std::string&);

	/**
	 * \brief  Return a Product file name given APID, list of Fields and file class.
	 */
	void getNameByProdIdClass(const std::string&, 
							  const std::string&,
							  std::string&);

private :
	/**
	 * \brief  Copy constructor
	 */
	GoceL0ProductTypeDescriptor(const GoceL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	GoceL0ProductTypeDescriptor &operator=(const GoceL0ProductTypeDescriptor &);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(GoceL0ProductTypeDescriptor) ;


};
	

	
_ACS_END_NAMESPACE


#endif //_GoceL0ProductTypeDescriptor_H_
