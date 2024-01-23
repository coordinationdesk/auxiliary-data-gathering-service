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

#ifndef _S1AuxManifestDataExtractor_H_
#define _S1AuxManifestDataExtractor_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <ManifestDataExtractor.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Auxuct like Safe Manifest extractor - Sentinel 1
**/

class S1AuxManifestDataExtractor: public ManifestDataExtractor  {
public:

	/** S1AuxManifestDataExtractor Exceptions */
	exDECLARE_EXCEPTION(exS1AuxManifestDataExtractorException, exException); // S1AuxManifestDataExtractor Exception.

	/** Default Class constructor */
	S1AuxManifestDataExtractor();
	/** Destructor */
	virtual ~S1AuxManifestDataExtractor() throw();

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
	S1AuxManifestDataExtractor(const S1AuxManifestDataExtractor & );
	/** Operator = */
	S1AuxManifestDataExtractor &operator=(const S1AuxManifestDataExtractor &);

private:
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1AuxManifestDataExtractor)
};

_ACS_END_NAMESPACE

#endif //_S1AuxManifestDataExtractor_H_

