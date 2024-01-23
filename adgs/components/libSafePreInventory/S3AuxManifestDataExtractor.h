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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.5  2014/02/25 15:21:31  matteo.airoldi
	Added management of mission = _
	
	Revision 1.4  2013/11/13 17:21:25  lucio.pulvirenti
	Value of productSize tag does not include manifest size: overall size (product + manifest) saved.
	
	Revision 1.3  2013/07/02 10:24:21  lucio.pulvirenti
	Static maps added for satelliteId, quality info and class. Method to extract quality information implemented.
	
	Revision 1.2  2013/07/01 16:42:30  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/06/28 16:29:02  lucio.pulvirenti
	Import libSafePreInventory
	
	
	

*/

#ifndef _S3AuxManifestDataExtractor_H_
#define _S3AuxManifestDataExtractor_H_ 

#include <ManifestDataExtractor.h>

#include <exException.h>


namespace acs {

class rsResourceSet;

	/**
	 * \brief This class define a generic interface to handle output processing of data
	 */
	class S3AuxManifestDataExtractor : public ManifestDataExtractor
	{
		public:
				
      		/** S3AuxManifestDataExtractor Exceptions */
      		exDECLARE_EXCEPTION(exS3AuxManifestDataExtractorException, exException) ;

			/**
			 * \brief Class constructor
			 */	
			S3AuxManifestDataExtractor();

			/**
			 * \brief  Class destructor   
			 */ 
			virtual ~S3AuxManifestDataExtractor() throw();


	//////////////////
	// public methods
	//////////////////
			virtual void doWork(std::string const &, SafePreInventory::ManifestData &);

		protected:
				
			virtual void extractGeneralProductInformationData(rsResourceSet const &, std::string const &, int, SafePreInventory::ManifestData &);
			virtual void extractProcessingData(rsResourceSet const &, std::string const &, int, SafePreInventory::ManifestData &);
			virtual void extractQualityInformationData(rsResourceSet const &, std::string const &, int, SafePreInventory::ManifestData &);

			
		private:
			S3AuxManifestDataExtractor(const S3AuxManifestDataExtractor&);
			S3AuxManifestDataExtractor& operator=(const S3AuxManifestDataExtractor&);

		public:
			static std::map<std::string, std::string> createClassMap()
        	{
        	  std::map<std::string, std::string>  m;

			  m["OPE"] = "OPER";
			  m["REP"] = "RPRO";
			  m["REF"] = "TEST";
			  m["DEV"] = "TEST";
        	  return m;
        	}

			static std::map<std::string, std::string> createQualityCheckMap()
        	{
        	  std::map<std::string, std::string>  m;

			  m["PASSED"] = "VALID_PRODUCT";
			  m["DEGRADED"] = "PRODUCT_HAS_WARNINGS";
			  m["FAILED"] = "PRODUCT_HAS_ERRORS";
        	  return m;
        	}

			static std::map<std::string, std::string> createSatellitesIdMap()
        	{
        	  std::map<std::string, std::string>  m;

			  m["SENTINEL-3"] = "S3_";
			  m["SENTINEL-3_"] = "S3_";
			  m["SENTINEL-3A"] = "S3A";
			  m["SENTINEL-3B"] = "S3B";
			  m["SENTINEL-3C"] = "S3C";
			  m["SENTINEL-3D"] = "S3D";
        	  return m;
        	}
			
		private:
				
			static const std::map< std::string, std::string > _classMap ;
			static const std::map< std::string, std::string > _qualityCheckMap ;
			static const std::map< std::string, std::string > _satellitesIdMap ;
			
			std::string _manifestFullpath;
			
			ACS_CLASS_DECLARE_DEBUG_LEVEL(S3AuxManifestDataExtractor);
			
	};//end of class definition


}//end of namespace acs


#endif //_S3AuxManifestDataExtractor_H_
