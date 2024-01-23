// PRQA S 1050 EOF
/*
	Copyright 2013-2021, Advanced Computer Systems , Inc.
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
	Revision 1.1  2015/06/05 16:19:52  lucio.pulvirenti
	S3MPM-504: First issue.
	
		
	
        
*/

#ifndef _S3PMRequestProductPreInventory_H_
#define _S3PMRequestProductPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for S3MPM Request Products following naming convention detailed in S3PDGS ICD 014 Volume 2 issue 2.4 Annex E
 **/

class S3PMRequestProductPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S3PMRequestProductPreInventory Exceptions */
	exDECLARE_EXCEPTION(exS3PMRequestProductPreInventoryException, exException) ; // Base S3PMRequestProductPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3PMRequestProductPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	S3PMRequestProductPreInventory();
	/** Destructor */
	virtual ~S3PMRequestProductPreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3PMRequestProductPreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3PMRequestProductPreInventory_H_

