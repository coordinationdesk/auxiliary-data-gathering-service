// PRQA S 1050 EOF
/*
	Copyright 2013-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	
        
*/

#ifndef _S3MPMReportsStdLikePreInventory_H_
#define _S3MPMReportsStdLikePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for S3MPM Reports following S3PDGS Products adaptation naming convention
 **/

class S3MPMReportsStdLikePreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S3MPMReportsStdLikePreInventory Exceptions */
	exDECLARE_EXCEPTION(exS3MPMReportsStdLikePreInventoryException, exException) ; // Base S3MPMReportsStdLikePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3MPMReportsStdLikePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	S3MPMReportsStdLikePreInventory();
	/** Destructor */
	virtual ~S3MPMReportsStdLikePreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3MPMReportsStdLikePreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3MPMReportsStdLikePreInventory_H_

