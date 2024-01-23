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
        
*/

#ifndef _SnavonsPreInventory_H_
#define _SsnavonsPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * 
 **/

class SnavonsPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** SnavonsPreInventory Exceptions */
	exDECLARE_EXCEPTION(exSnavonsPreInventoryException, exException) ; // Base SnavonsPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exSnavonsPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	SnavonsPreInventory();
	/** Destructor */
	virtual ~SnavonsPreInventory() throw(){}

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	void extractParamFromConf( const std::string &ftype, const std::string &parName, unsigned int &parValue) ;
private:
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SnavonsPreInventory) ;

};

_ACS_END_NAMESPACE
#endif //_SnavonsPreInventory_H_
