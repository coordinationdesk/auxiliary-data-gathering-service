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
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
		
	
        
*/

#ifndef _GHRSSTL4PreInventory_H_
#define _GHRSSTL4PreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for products following S3PDGS naming convention
 **/

class GHRSSTL4PreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** GHRSSTL4PreInventory Exceptions */
	exDECLARE_EXCEPTION(exGHRSSTL4PreInventoryException, exException) ; // Base GHRSSTL4PreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exGHRSSTL4PreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	GHRSSTL4PreInventory();
	/** Destructor */
	virtual ~GHRSSTL4PreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );

	void fillGeoCoordinates( const std::string &, std::vector<std::string> & ); 
	
private:

	static const std::string InventoryTimeForMissionEnd	;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(GHRSSTL4PreInventory)

};

_ACS_END_NAMESPACE

#endif //_GHRSSTL4PreInventory_H_

