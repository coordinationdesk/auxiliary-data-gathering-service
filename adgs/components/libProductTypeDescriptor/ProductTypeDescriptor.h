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
	Revision 2.1  2013/12/09 14:01:12  giucas
	Library completely revisited.
	

*/

#ifndef _ProductTypeDescriptor_H_
#define _ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class ProductTypeDescriptor
{
public:

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	ProductTypeDescriptor();
	ProductTypeDescriptor(const ProductTypeDescriptor &);

	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~ProductTypeDescriptor() throw();


/////////////////////////////////////////////		
//public pure virtual methods
/////////////////////////////////////////////

	/**
	 * \brief Return a list of Product Names given a Product Id  
	 *    
	 */
	virtual void getNamesById(const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product Id  
	 *    
	 */
	virtual void getNamesById(const std::string&, const std::string&, const std::string&, std::vector<std::string>&)=0;


	/**
	 * \brief Return a list of Product Names given a Mission Id  
	 *    
	 */
	virtual void getNamesByMission(const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Mission Id  
	 *    
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, const std::string&, std::vector<std::string>&)=0;


	/**
	 * \brief Return a list of Product Names given a Product file extension  
	 *    
	 */
	virtual void getNamesByExtension(const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief Return a list of Product Names ncluding Sensing Start and Stop times
	 *        given a Product file extension  
	 *    
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, const std::string&, std::vector<std::string>&)=0;


	/**
	 * \brief  Return true if the std::string in input represent a valid Product name, false otherwise. 
	 *    
	 */
	virtual bool isValidName(const std::string&)=0;

	/**
	 * \brief  Return the ordered Product Id list as specified in configuration file 
	 *    
	 */
	virtual const std::vector<std::string> &getOrderedProductIds() const = 0;
protected:
	ProductTypeDescriptor& operator= (const ProductTypeDescriptor &);

};

_ACS_END_NAMESPACE


#endif //_ProductTypeDescriptor_H_
