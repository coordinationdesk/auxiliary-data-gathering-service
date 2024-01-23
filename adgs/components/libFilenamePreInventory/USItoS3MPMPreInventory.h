// PRQA S 1050 EOF
/*
	Copyright 2016-2021, Advanced Computer Systems , Inc.
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
	Revision 1.1  2016/04/12 14:42:14  lucio.pulvirenti
	S3MPM-695: first issue.
	
		
	
        
*/

#ifndef _USItoS3MPMPreInventory_H_
#define _USItoS3MPMPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for User Service Interface to MPMF Product Activity Rules & Subscriptions: ICD MPMF [AD-17] (D006.E_S3PDGS ICD 015 -i2r1- S3PDGS ICD Volume 3) 
 **/

class USItoS3MPMPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** USItoS3MPMPreInventory Exceptions */
	exDECLARE_EXCEPTION(exUSItoS3MPMPreInventoryException, exException) ; // Base USItoS3MPMPreInventory Exception. // PRQA S 2131, 2502


	/** Default Class constructor */
	USItoS3MPMPreInventory();
	/** Destructor */
	virtual ~USItoS3MPMPreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(USItoS3MPMPreInventory)

};

_ACS_END_NAMESPACE

#endif //_USItoS3MPMPreInventory_H_

