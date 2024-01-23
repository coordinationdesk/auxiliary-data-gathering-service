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

#ifndef _S6MOG2DFilenamePreInventory_H_
#define _S6MOG2DFilenamePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for MOG2D products of Sentinel-6 on filename base
 **/

class S6MOG2DFilenamePreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S6MOG2DFilenamePreInventory Exceptions */
	exDECLARE_EXCEPTION(exS6MOG2DFilenamePreInventoryException, exException) ; // Base S6MOG2DFilenamePreInventory Exception. // PRQA S 2131, 2502

	/** Default Class constructor */
	S6MOG2DFilenamePreInventory();
	/** Destructor */
	virtual ~S6MOG2DFilenamePreInventory() throw();

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
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S6MOG2DFilenamePreInventory)

};

_ACS_END_NAMESPACE

#endif //_S6MOG2DFilenamePreInventory_H_

