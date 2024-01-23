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
	Revision 1.3  2015/10/06 15:22:12  lucio.pulvirenti
	S3MPM-628: DoyDateFormat tag added to configuration to manage DOYDate properly. Flag added to getDateFromFilename and getStdFormatDate method added to
	convert DOYDate to std one (e.g. YYMMDDThhmmss).
	
	Revision 1.2  2015/06/05 13:50:41  lucio.pulvirenti
	S3MPM-504: Check on dates demanded to new getDateFromFilename method. Also OLTC Data Files managed.
	
	Revision 1.1  2015/06/04 16:03:10  lucio.pulvirenti
	S3MPM-504: First issue.
	
		
	
        
*/

#ifndef _EOFlikePreInventory_H_
#define _EOFlikePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for products following EOF naming convention according to Earth Observation Ground Segment File Format Standard PE-TN-ESA-GS-0001 2.0 
 **/

class EOFlikePreInventory : public FilenamePreInventory // PRQA S 2109
{ 
public:

	/** EOFlikePreInventory Exceptions */
	exDECLARE_EXCEPTION(exEOFlikePreInventoryException, exException) ; // Base EOFlikePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exEOFlikePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	EOFlikePreInventory();
	/** Destructor */
	virtual ~EOFlikePreInventory() throw();

	virtual std::string getClassId();
	

private:
		
	//extracts information from filename
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );
	
	virtual short int getSatelliteId(const std::string& satellite, dbConnection &);
	
	std::string getDateFromFilename(const std::string& filename, size_t pos, bool DOYFmt);
	std::string getGeneratingCentreFromFilename(const std::string& filename, size_t pos) ;
	std::string getStdFormatDate(const std::string & DOYDate);

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFlikePreInventory)

};

_ACS_END_NAMESPACE

#endif //_EOFlikePreInventory_H_

