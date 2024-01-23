/*
	Copyright 2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

*/

#ifndef _S1ProdManifestDataExtractor_H_
#define _S1ProdManifestDataExtractor_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <ManifestDataExtractor.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Product like Safe Manifest extractor - Sentinel 1
**/

class S1ProdManifestDataExtractor: public ManifestDataExtractor  {
public:

	/** S1ProdManifestDataExtractor Exceptions */
	exDECLARE_EXCEPTION(exS1ProdManifestDataExtractorException, exException); // S1ProdManifestDataExtractor Exception.

	/** Default Class constructor */
	S1ProdManifestDataExtractor();
	/** Destructor */
	virtual ~S1ProdManifestDataExtractor() throw();


	/** Interface implementation */
	virtual void doWork(std::string const &, SafePreInventory::ManifestData &);
	
	static std::map<std::string, std::string> createSatellitesIdMap()
    {
      std::map<std::string, std::string>  m;

	  m["SENTINEL-1"] = "S1_";
	  m["SENTINEL-1A"] = "S1A";
	  m["SENTINEL-1B"] = "S1B";
      return m;
    }


private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	S1ProdManifestDataExtractor(const S1ProdManifestDataExtractor & );
	/** Operator = */
	S1ProdManifestDataExtractor &operator=(const S1ProdManifestDataExtractor &);
	

private:
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1ProdManifestDataExtractor)

};

_ACS_END_NAMESPACE

#endif //_S1ProdManifestDataExtractor_H_

