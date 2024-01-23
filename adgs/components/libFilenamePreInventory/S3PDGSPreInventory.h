// PRQA S 1050 EOF
/*
	Copyright 2013-2015, Advanced Computer Systems , Inc.
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
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
		
	
        
*/

#ifndef _S3PDGSPreInventory_H_
#define _S3PDGSPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for products following S3PDGS naming convention
 **/

class S3PDGSPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S3PDGSPreInventory Exceptions */
	exDECLARE_EXCEPTION(exS3PDGSPreInventoryException, exException) ; // Base S3PDGSPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3PDGSPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	S3PDGSPreInventory();
	/** Destructor */
	virtual ~S3PDGSPreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3PDGSPreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3PDGSPreInventory_H_

