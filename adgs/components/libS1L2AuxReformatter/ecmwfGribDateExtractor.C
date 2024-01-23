/*
	Copyright 1995-2023, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
	* 
	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2013/12/11 15:47:57  nicvac
	Coding rules implementation (by marpas?)
	
	Revision 1.3  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.2  2012/12/12 13:47:14  damdec
	Compilation warning fixed.
	
	Revision 1.1  2012/04/05 09:06:09  damdec
	Class added.
	
        
*/ 

#include <ecmwfGribDateExtractor.h>

#include <Filterables.h>
#include <cstdio>
#include <StringUtils.h>
#include <File.h>

#include <boost/format.hpp> // PRQA S 1013



using namespace std;

_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(ecmwfGribDateExtractor)


ecmwfGribDateExtractor::ecmwfGribDateExtractor() 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
	
}

ecmwfGribDateExtractor::~ecmwfGribDateExtractor() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void ecmwfGribDateExtractor::getDate(const std::string & filename, std::string & year, std::string & month, std::string &day, std::string & hour, std::string &min, bool validityStart)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	FILE * in=NULL;
	int err(0);
	grib_handle * h=NULL;
	in = fopen(filename.c_str(),"r");
	
	ACS_COND_THROW(!in, execmwfGribDateExtractorException("ERROR: unable to open file "+filename));
	h = grib_handle_new_from_file(0, in, &err);	
	ACS_COND_THROW(h==NULL, execmwfGribDateExtractorException("ERROR: unable to create handle from file "+filename));
	
	long date(0);
	GRIB_CHECK(grib_get_long(h, "dataDate", &date), 0);
	
	ostringstream dt; dt << date;
	string dataDate = dt.str();
	
	year = StringUtils::substring(dataDate, 0, 4);
	month = StringUtils::substring(dataDate, 4, 6);
	day = StringUtils::substring(dataDate, 6, 8);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation date: " << year << month << day);
	
	if( validityStart ) {
		
	// The following code is for data  across midnight of 31 Dec w.r.t. generation time and validity start time
		unsigned short generationMonth = 0;
		{
			istringstream is(month); // PRQA S 4400 
			is >> generationMonth;
		}
		string basename = File::getFileName(filename);
		string startValidityMonthStr = basename.substr( 11, 2);
		unsigned short startValidityMonth = 0;
		{
			istringstream is(startValidityMonthStr); // PRQA S 4400 
			is >> startValidityMonth;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start validity month " << startValidityMonth);

		if( startValidityMonth < generationMonth ) {

			unsigned short yearInt = 0 ;
			istringstream is(year);
			is >> yearInt;
			yearInt++;

			ostringstream os;
			os << yearInt;
			year = os.str();
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start validity month smaller than generation month: validity year set to " << year);

		}

		// *VALIDITY_START
		//from filename ccSMMDDHHIImmddhhiiE.grb the validity start is mmddhhii

		month = startValidityMonthStr;
		day = basename.substr( 13, 2);
		hour = basename.substr( 15, 2); 
		min = basename.substr( 17, 2);    
	}
	else {
		
		long time(0);
		GRIB_CHECK(grib_get_long(h, "dataTime", &time), 0);
		boost::format timeFmt("%04d");
		timeFmt % int(time); // PRQA S 3081
		string dataTime = timeFmt.str();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation time: " << dataTime);

		if (dataTime.length()<4)
		{
			hour = StringUtils::substring(dataTime, 0, 1);
			hour = "0" + hour ;
			min = StringUtils::substring(dataTime, 1, 3);
		}
		else
		{
			hour = StringUtils::substring(dataTime, 0, 2);
			min = StringUtils::substring(dataTime, 2, 4);
		}
		
	}
	grib_handle_delete(h);
	fclose(in);
	
}

_ACS_END_NAMESPACE
