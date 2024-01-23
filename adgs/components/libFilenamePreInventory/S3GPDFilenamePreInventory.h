// PRQA S 1050 EOF
/*
	Copyright 2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.
		
*/

#ifndef _S3GPDFilenamePreInventory_H_
#define _S3GPDFilenamePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for GNSS (Global Navigation Satellite Systems) derived Path Delay Plus (GPD+) Wet Tropospheric Correction (WTC) products for the Sentinel-3 altimeter missions (GPD4S3) on filename base
 * Naming convention is detailed in GPD4S3 Product Specification Document (PSD) Issue 1.2 of 05/07/2021
 **/

class S3GPDFilenamePreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S3GPDFilenamePreInventory Exceptions */
	exDECLARE_EXCEPTION(exS3GPDFilenamePreInventoryException, exException) ; // Base S3GPDFilenamePreInventory Exception. // PRQA S 2131, 2502


	/** Default Class constructor */
	S3GPDFilenamePreInventory();
	/** Destructor */
	virtual ~S3GPDFilenamePreInventory() throw();

	virtual std::string getClassId();
	
#ifdef TestingParseFilename
public:
#else
private:
#endif
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3GPDFilenamePreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3GPDFilenamePreInventory_H_

