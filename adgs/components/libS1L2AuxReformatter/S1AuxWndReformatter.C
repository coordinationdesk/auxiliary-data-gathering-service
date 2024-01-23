/*
	Copyright 1995-2023, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.6  2014/09/09 10:32:59  nicvac
	Format compliant with the updated libDateTime.
	
	Revision 1.5  2013/12/11 15:47:56  nicvac
	Coding rules implementation (by marpas?)
	
	Revision 1.4  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.3  2012/12/12 13:47:14  damdec
	Compilation warning fixed.
	
	Revision 1.2  2012/10/11 15:11:45  damdec
	Validity time computation updated (only year is retrieved from the file content, othe info are read in the input filename
	
	Revision 1.1  2012/04/23 08:02:38  damdec
	Class added.
	
        
*/ 

#include <S1AuxWndReformatter.h>

#include <Filterables.h>
#include <RegExp.h>
#include <ecmwfGribDateExtractor.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxWndReformatter)


S1AuxWndReformatter::S1AuxWndReformatter() : S1L2AuxReformatter()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
}

S1AuxWndReformatter::~S1AuxWndReformatter() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void S1AuxWndReformatter::setValidity(const string & v)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// filename validity v not used
	ostringstream filesize; 
    filesize << getInFilename().size();
	ACS_COND_THROW( getInFilename().size() > 1, exS1L2AuxReformatterException("Unexpected number of input files: <"+ filesize.str() + ">"));
	string year, month, day ;
	string hour, min;
	ecmwfGribDateExtractor::getDate(getInFilename()[0], year, month, day, hour, min, true);
	
	ostringstream val;
	val << year << month << day << "T" << hour << min << "00";; 
	validity(val.str());
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity start time: " << val.str() );
}

_ACS_END_NAMESPACE
