// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 1.1  2014/03/11 17:02:15  lucio.pulvirenti
	Work in progress.
	
		
	
        
*/

#ifndef _JersSAROPSPreInventory_H_
#define _JersSAROPSPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for products following S3PDGS naming convention
 **/

class JersSAROPSPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** JersSAROPSPreInventory Exceptions */
	exDECLARE_EXCEPTION(exJersSAROPSPreInventoryException, exException) ; // Base JersSAROPSPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exJersSAROPSPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	JersSAROPSPreInventory();
	/** Destructor */
	virtual ~JersSAROPSPreInventory() throw();

	virtual std::string getClassId();
	

private:

	JersSAROPSPreInventory(const JersSAROPSPreInventory & ); // not implemented
	JersSAROPSPreInventory &operator=(const JersSAROPSPreInventory &); // not implemented
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	/**
	* \brief  Get the satellite id according to db record given satellite name (e.g. Jers)
	* \param satellite name/acronym
	* \param dbConnection
	* \retval satelliteId
	*/    	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);


private:

	static const std::string satelliteName	;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(JersSAROPSPreInventory)

};

_ACS_END_NAMESPACE

#endif //_JersSAROPSPreInventory_H_

