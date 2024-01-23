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
	Revision 1.1  2016/04/18 19:22:42  francesco.avanzi
	first addition
	
	Revision 1.1  2016/04/04 14:02:54  francesco.avanzi
	first addition
	
	Revision 1.2  2015/10/14 13:00:20  francesco.avanzi
	test variable IFDEF implemented
	
	Revision 1.1  2015/08/04 16:02:05  francesco.avanzi
	added
	
	Revision 1.1  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: S3ExelisFilenamePreInventory class developed.
	
		
	
        
*/

#ifndef _S3ExelisFilenamePreInventory_H_
#define _S3ExelisFilenamePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for S3MPM Request Products following naming convention detailed in S3PDGS ICD 014 Volume 2 issue 2.4 Annex E
 **/

class S3ExelisFilenamePreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** S3ExelisFilenamePreInventory Exceptions */
	exDECLARE_EXCEPTION(exS3ExelisFilenamePreInventoryException, exException) ; // Base S3ExelisFilenamePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3ExelisFilenamePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	S3ExelisFilenamePreInventory();
	/** Destructor */
	virtual ~S3ExelisFilenamePreInventory() throw();

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
	

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3ExelisFilenamePreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3ExelisFilenamePreInventory_H_

