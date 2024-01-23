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
	Revision 1.1  2014/03/11 17:02:14  lucio.pulvirenti
	Work in progress.
	
		
	
        
*/

#ifndef _ESAStandardPreInventory_H_
#define _ESAStandardPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for products following S3PDGS naming convention
 **/

class ESAStandardPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** ESAStandardPreInventory Exceptions */
	exDECLARE_EXCEPTION(exESAStandardPreInventoryException, exException) ; // Base ESAStandardPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exESAStandardPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	ESAStandardPreInventory();
	/** Destructor */
	virtual ~ESAStandardPreInventory() throw();

	virtual std::string getClassId();
	

private:

	ESAStandardPreInventory(const ESAStandardPreInventory & ); // not implemented
	ESAStandardPreInventory &operator=(const ESAStandardPreInventory &); // not implemented

		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	/**
	* \brief  Get the satellite id according to db record given satellite name (e.g. Jers)
	* \param satellite name/acronym
	* \param dbConnection
	* \retval satelliteId
	*/    	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);
	
	
	static std::map<std::string, std::string> createSatellitesIdMap()
    {
      std::map<std::string, std::string>  m;

	  m["JE"] = "Jers";
      return m;
    }


private:

	static const std::map< std::string, std::string > _satellitesIdMap ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ESAStandardPreInventory)

};

_ACS_END_NAMESPACE

#endif //_ESAStandardPreInventory_H_

