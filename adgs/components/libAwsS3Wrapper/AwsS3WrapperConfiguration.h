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

	$Id$

	$Author$

	$Log$

*/


#ifndef _AWSS3WRAPPERCONFIGURATION_H_
#define _AWSS3WRAPPERCONFIGURATION_H_


#include <aws/core/Aws.h>
#include <exException.h>
#include <Singleton.hpp>	
#include <string>


_ACS_BEGIN_NAMESPACE(acs)


/*! \class AwsS3WrapperConfiguration
	
	Utility class to support the AwsS3WrapperConfiguration.
*/



class AwsS3WrapperConfiguration : public pattern::Singleton <AwsS3WrapperConfiguration, pattern::NoReleaseAtExitTraits<AwsS3WrapperConfiguration> > // PRQA S 2109, 2153
{ 
    friend AwsS3WrapperConfiguration* pattern::Singleton<AwsS3WrapperConfiguration, pattern::NoReleaseAtExitTraits<AwsS3WrapperConfiguration> >::instance(); // PRQA S 2107

public:
	virtual ~AwsS3WrapperConfiguration() throw() ;

protected:
	AwsS3WrapperConfiguration() ;
	
private:
	AwsS3WrapperConfiguration(const AwsS3WrapperConfiguration &) ;
	AwsS3WrapperConfiguration &operator=(const AwsS3WrapperConfiguration &) ;
	
private:	
	Aws::SDKOptions	options_ ;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AwsS3WrapperConfiguration) ;
} ;

typedef AwsS3WrapperConfiguration AwsS3WrapperConfigurationSingleton ;

_ACS_END_NAMESPACE

#endif	/* _AWSS3WRAPPERCONFIGURATION_H_ */
