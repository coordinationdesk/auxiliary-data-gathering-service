/*

	Copyright 2019-, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. S3 Wrapper Library HEADER $

	$Log$

*/


#include <AwsS3WrapperConfiguration.h>
#include <exException.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



////////////////////////////////////////////////////////////////////////////
//
//   class AwsS3WrapperConfiguration
//
////////////////////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(AwsS3WrapperConfiguration) 


/**********************************************************************************
  *	                                                                              *
  *	  C'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3WrapperConfiguration::AwsS3WrapperConfiguration() 
: options_()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor called." ) ;

	// VERY IMPORTANT NOTE: This should be called exactly once-per-process.
	Aws::InitAPI(options_);

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor finished." ) ;
}



/**********************************************************************************
  *	                                                                              *
  *	  D'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3WrapperConfiguration::~AwsS3WrapperConfiguration() throw()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "D'tor called." ) ;
		
	// VERY IMPORTANT NOTE: This should be called exactly once-per-process.
	Aws::ShutdownAPI(options_);

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "D'tor finished." ) ;
}



_ACS_END_NAMESPACE
