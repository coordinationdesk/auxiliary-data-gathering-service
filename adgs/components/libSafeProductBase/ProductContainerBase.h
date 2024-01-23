// PRQA S 1050 EOF
/*

	Copyright 2006-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. SafeWrapper Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.1  2011/09/12 08:56:52  aleber
 	first implementation
 	
*/

#ifndef _ProductContainerBase_H_
#define _ProductContainerBase_H_ 

#include <exStream.h>


_ACS_BEGIN_NAMESPACE(acs)

class ProductContainerBase
{
public:

	/**
	 * \brief  Class constructor
	 *    
	 */
	explicit ProductContainerBase(
			const std::string &productClass = "",
			const std::string &productClassDescription = ""
	);	

	/**
	 * \brief  Class destructor 
	 *    
	 */
    virtual ~ProductContainerBase() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	ProductContainerBase(const ProductContainerBase & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	ProductContainerBase &operator=(const ProductContainerBase &) ;

	void setProductClass(const std::string& v )  {  _productClass = v; } 
	void setProductClassDescription(const std::string& v ) { _productClassDescription = v; } 

	const std::string &productClass() const throw() { return _productClass; }
	const std::string &productClassDescription() const throw()  { return _productClassDescription; } 

	virtual std::string toStr() const ;

private:

	std::string 	_productClass;  // product class (S, A, N or C)
	std::string   	_productClassDescription;  // SLICE, PARTIAL (if data take is not complete
												// (e.g.) firts packets missing), FULL (if data
												// take is complete)


	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProductContainerBase)

};


_ACS_END_NAMESPACE


#endif //_ProductContainerBase_H_
