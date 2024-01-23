// PRQA S 1050 EOF
/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/12/11 15:47:57  nicvac
	Coding rules implementation (by marpas?)
	
	Revision 1.2  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.1  2012/04/05 09:06:09  damdec
	Class added.
	
        
*/

#ifndef _ecmwfGribDateExtractor_H_
#define _ecmwfGribDateExtractor_H_

#include <exException.h>
#include "grib_api.h"

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Utility class for the retrieval of datation info from the ECMWF GRIB files 
**/

class ecmwfGribDateExtractor  { 
public:

	/** ecmwfGribDateExtractor Exceptions */
	exDECLARE_EXCEPTION(execmwfGribDateExtractorException, exException) ; // Base ecmwfGribDateExtractor Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(execmwfGribDateExtractorCriticalException, execmwfGribDateExtractorException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	ecmwfGribDateExtractor();
	/** Destructor */
	virtual ~ecmwfGribDateExtractor() throw() ;
	
	static void getDate(const std::string & filename, std::string & year, std::string & month, std::string &day, std::string & hour, std::string &min, bool validityStart = false);

private:  // copy constructor and operator= defined but not implemented
	ecmwfGribDateExtractor(const ecmwfGribDateExtractor & ); // not implemented
	ecmwfGribDateExtractor &operator=(const ecmwfGribDateExtractor &); // not implemented
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ecmwfGribDateExtractor)

};

_ACS_END_NAMESPACE

#endif //_ecmwfGribDateExtractor_H_

