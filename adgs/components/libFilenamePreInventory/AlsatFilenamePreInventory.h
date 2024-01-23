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
	Revision 1.1  2015/08/04 16:02:05  francesco.avanzi
	added
	
	Revision 1.1  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: AlsatFilenamePreInventory class developed.
	
		
	
        
*/

#ifndef _AlsatFilenamePreInventory_H_
#define _AlsatFilenamePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for S3MPM Request Products following naming convention detailed in S3PDGS ICD 014 Volume 2 issue 2.4 Annex E
 **/

class AlsatFilenamePreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** AlsatFilenamePreInventory Exceptions */
	exDECLARE_EXCEPTION(exAlsatFilenamePreInventoryException, exException) ; // Base AlsatFilenamePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exAlsatFilenamePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	AlsatFilenamePreInventory();
	/** Destructor */
	virtual ~AlsatFilenamePreInventory() throw();

	virtual std::string getClassId();
	

protected:
		
	//extracts information from filename
#ifdef TestingParseFilename
public:
#endif
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
#ifdef TestingParseFilename
private:
#endif
	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AlsatFilenamePreInventory)

};

_ACS_END_NAMESPACE

#endif //_AlsatFilenamePreInventory_H_

