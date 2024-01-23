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
	Revision 1.1  2016/01/12 14:00:42  francesco.avanzi
	module added
	
	
		
	
        
*/
#ifndef _SpaceNavPreInventory_H_
#define _SpaceNavPreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for products following EOF naming convention according to Earth Observation Ground Segment File Format Standard PE-TN-ESA-GS-0001 2.0 
 **/

class SpaceNavPreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** SpaceNavPreInventory Exceptions */
	exDECLARE_EXCEPTION(exSpaceNavPreInventoryException, exException) ; // Base SpaceNavPreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exSpaceNavPreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	SpaceNavPreInventory();
	/** Destructor */
	virtual ~SpaceNavPreInventory() throw(){}

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	void extractParamFromConf( const std::string &ftype, const std::string &parName, unsigned int &parValue) ;
private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SpaceNavPreInventory) ;

};

_ACS_END_NAMESPACE
#endif //_SpaceNavPreInventory_H_
