/*
	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libSafePreInventory$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _S3ProdManifestDataExtractor_H_
#define _S3ProdManifestDataExtractor_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <ManifestDataExtractor.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Product like Safe Manifest extractor - Sentinel 3
**/

class S3ProdManifestDataExtractor: public ManifestDataExtractor  {
public:

	/** S3ProdManifestDataExtractor Exceptions */
	exDECLARE_EXCEPTION(exS3ProdManifestDataExtractorException, exException); // S3ProdManifestDataExtractor Exception.

	/** Default Class constructor */
	S3ProdManifestDataExtractor();
	/** Destructor */
	virtual ~S3ProdManifestDataExtractor() throw();


	/** Interface implementation */
	virtual void doWork(std::string const &, SafePreInventory::ManifestData &);

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	S3ProdManifestDataExtractor(const S3ProdManifestDataExtractor & );
	/** Operator = */
	S3ProdManifestDataExtractor &operator=(const S3ProdManifestDataExtractor &);
	

protected:

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3ProdManifestDataExtractor)

};

_ACS_END_NAMESPACE

#endif //_S3ProdManifestDataExtractor_H_

