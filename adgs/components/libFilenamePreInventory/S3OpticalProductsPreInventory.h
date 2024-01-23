// PRQA S 1050 EOF
/*
	Copyright 2014-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2014/09/30 08:11:59  lucio.pulvirenti
	updateProductAndMetadata method added to IF.
	
	Revision 1.1  2014/09/29 14:35:52  lucio.pulvirenti
	First issue.
	
	
        
*/

#ifndef _S3OpticalProductsPreInventory_H_
#define _S3OpticalProductsPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for S2 optical products
 **/

class S3OpticalProductsPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S3OpticalProductsPreInventory Exceptions */
	exDECLARE_EXCEPTION(exS3OpticalProductsPreInventoryException, exException) ; // Base S3OpticalProductsPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3OpticalProductsPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	S3OpticalProductsPreInventory();
	/** Destructor */
	virtual ~S3OpticalProductsPreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );

	/**
	* \brief  Get the satellite id according to db record given satellite acronym (i.e. S3A, S3B)
	* \param satellite acronym
	* \param dbConnection
	* \retval satelliteId
	*/    	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);
	
	virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata);

	
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3OpticalProductsPreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3OpticalProductsPreInventory_H_

