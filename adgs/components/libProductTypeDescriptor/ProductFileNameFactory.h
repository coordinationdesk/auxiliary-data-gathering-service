// PRQA S 1050 EOF
/*

	Copyright 2002-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.6  2014/05/15 15:38:16  marpas
	newObject is now const
	
	Revision 2.5  2014/03/26 09:57:37  marpas
	coding best practices applied
	warnings fixed (qa & g++)
	
	Revision 2.4  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.3  2008/07/22 13:26:52  giucas
	Modified Ers factory object
	
	Revision 2.2  2007/03/14 09:03:10  giucas
	AeolusProductFileName added
	
	Revision 2.1  2006/10/05 12:54:21  giucas
	Added Eos and Landsat as Dummy
	
	Revision 2.0  2006/02/28 09:31:27  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.3  2004/08/06 14:12:35  giucas
	Added builder method for Cosmo ProductFileName
	
	Revision 1.2  2004/08/02 16:01:56  giucas
	Added handling of GOCE
	
	Revision 1.1  2003/09/09 13:13:51  giucas
	First release
	


*/

#ifndef _ProductFileNameFactory_H_
#define _ProductFileNameFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <ProductFileName.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class ProductFileNameFactory
{
public:

	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502

	/**
	  * \brief Factory class constructor registers instances of ProductFileName objects
	  */
	ProductFileNameFactory();

	/**
	  * \brief Factory class destructor unregisters instances of ProductFileName objects
	  */				
	~ProductFileNameFactory();

	/**
	  * \brief Builder method for CryosatProductFileName object retrieving
	  */						
	static ProductFileName* buildCryosatProductFileName();		

	/**
	  * \brief Builder method for EnvisatProductFileName object retrieving
	  */								
	static ProductFileName* buildEnvisatProductFileName();

	/**
	  * \brief Builder method for GoceProductFileName object retrieving
	  */								
	static ProductFileName* buildGoceProductFileName();

	/**
	  * \brief Builder method for GoceProductFileName object retrieving
	  */								
	static ProductFileName* buildCosmoProductFileName();

	/**
	  * \brief Builder method for DUMMY ProductFileName object retrieving
	  */								
	static ProductFileName* buildDummyProductFileName();

	/**
	  * \brief Builder method for AeolusProductFileName object retrieving
	  */						
	static ProductFileName* buildAeolusProductFileName();		

	/**
	  * \brief Builder method for ErsProductFileName object retrieving
	  */						
	static ProductFileName* buildErsProductFileName();	
		
	/**
	  * \brief Builder method for EarthCAREWithVersProductFileName object retrieving
	  */						
	static ProductFileName* buildEarthCAREWithVersProductFileName();		
	/**
	  * \brief Return the object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	ProductFileName* newObject(std::string const &) const ;



private:
	ProductFileNameFactory(const ProductFileNameFactory &); // not implemented
	ProductFileNameFactory& operator=(const ProductFileNameFactory &); // not implemented

	void registerFactory(std::string const &, ProductFileName* (*)() );
	void unregisterFactory(std::string const &, ProductFileName* (*)() );

	//map storing the association between key and builder method
	std::map<std::string, ProductFileName* (*)()> _map;

};


_ACS_END_NAMESPACE


#endif // _ProductFileNameFactory_H_

