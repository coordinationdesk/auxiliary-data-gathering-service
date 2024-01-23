/*
	Copyright 1995-2021, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libFilenamePreInventory$
        
*/ 

#include <S3DimsaxFilenamePreInventory.h>

#include <Filterables.h>

#include <DateTime.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <RegExp.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3DimsaxFilenamePreInventory)

S3DimsaxFilenamePreInventory::S3DimsaxFilenamePreInventory() :
	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

S3DimsaxFilenamePreInventory::~S3DimsaxFilenamePreInventory() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

string S3DimsaxFilenamePreInventory::getClassId() {
     //return the class name
	return "S3DimsaxFilenamePreInventory" ;
}

void S3DimsaxFilenamePreInventory::parseFilename( const string& pathFileName, const string& fileType, FilenameElements& fne) { // PRQA S 4211

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "pathFileName: "<< pathFileName );

    string filename = File::getFileName(pathFileName);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');
    //rsResourceSet& rs = ConfigurationSingleton::instance()->get();

    fne.File_Type = fileType ;

    //Filename example to be parsed: ims2021070_1km_v1.3.asc.gz

    const string reStr("^...(....)(...)_.*gz$");
	RegExp::KeyMatches km;
	enum Keys {all, kYear, kDoy};

	RegExp re(reStr);
	ACS_COND_THROW( ! re.match( filename, km ),
			exCriticalException("Error matching filename ["+filename+"] with ["+reStr+"]") );

	const string & yearStr = km[kYear].getMatched();
	const string & doyStr = km[kDoy].getMatched();

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "String Fields: "<<yearStr<<"; "<<doyStr );

	unsigned short year(0);
	{
		std::istringstream ss(yearStr);
		if ( not( ss >> year) ) {
			ACS_THROW( exCriticalException("Bad year field ["+yearStr+"] matching filename ["+filename+"] with ["+reStr+"]") );
		}
	}
	unsigned short doy(0);
	{
		std::istringstream ss(doyStr);
		if ( not( ss >> doy) ) {
			ACS_THROW( exCriticalException("Bad day of year field ["+doyStr+"] matching filename ["+filename+"] with ["+reStr+"]") );
		}
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Fields: "<<year<<"; "<<doy );

	unsigned short day(0);
	unsigned short month(0);
    DateTime::UTCD::dayAndMonthFromDOY( doy, year, day, month ) ;
    DateTime::UTCD dtUtcStart( year, month, day, 0, 0, 0);
    DateTime::UTCD dtUtcStop ( year, month, day, 23, 59, 59);

    DateTime dtStart(dtUtcStart) ;
    DateTime dtStop (dtUtcStop ) ;

    fne.StartDate = dtStart.asISO8601(0) ;
    fne.CreationDate = fne.StartDate ;
    fne.StopDate = dtStop.asISO8601(0) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity: ["<<fne.StartDate<<"; "<<fne.StopDate<<"]" );


}


_ACS_END_NAMESPACE

