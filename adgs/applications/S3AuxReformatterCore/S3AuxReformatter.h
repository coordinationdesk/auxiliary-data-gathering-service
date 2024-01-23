/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel3 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$
	Revision 1.7  2015/05/12 13:30:24  matteo.airoldi
	Added order_id to metadata file for ingestion
	
	Revision 1.6  2014/06/11 14:43:10  matteo.airoldi
	GenerationCentre now configured for filetype
	
	Revision 1.5  2013/06/14 10:33:29  marfav
	Aligned to PDGS V2 schema
	
	Revision 1.4  2013/06/12 14:59:31  marfav
	First issue of ReformatterCore completed
	
	Revision 1.3  2013/06/12 10:53:06  marfav
	Work in progress
	
	Revision 1.2  2013/06/11 17:02:22  marfav
	Work in progress. Started configuraion management
	
	Revision 1.1.1.1  2013/06/11 13:33:29  marfav
	Importing S3AuxReformatterCore
	

*/

#ifndef _S3AuxReformatter_H_
#define _S3AuxReformatter_H_ 

#include <S3AuxManifest.h>

#if defined (HAS_MISSION_INCLUDES)
	#include <ProjectStandaloneAppIncludes>
#else
	#include <StandaloneApp.h>
#endif

namespace acs {

  
#if   defined (HAS_MISSION_INCLUDES)
	class S3AuxReformatter : public PROJECT_STANDALONE_APP
#else
	class S3AuxReformatter : public StandaloneApp
#endif
	{
	public:
			
		/*! class S3AuxReformatterException declaration */
		exDECLARE_EXCEPTION(S3AuxReformatterException,exException) ;
		
		/**
		 * \brief  Class constructor 
		 *    
		 */
		S3AuxReformatter(const std::string& appName, const std::string& subSys);
		
		/**
		 * \brief  Class destructor 
		 *    
		 */    	
		~S3AuxReformatter() throw() ;
		
		virtual int main(int argc, char const * const * argv, char const * const * env);
		
		using Application::usage;		

	private:

		// copy constructor and operator= defined but not implemented
		S3AuxReformatter(const S3AuxReformatter & ); 
		S3AuxReformatter &operator=(const S3AuxReformatter &);
			
		//methods
		void usage(int ,  char const * const *) const;
		int execute(const std::string & jobOrderFileName);

		//read the pconf file and store the configuration in internal attributes
		void readPConf (const std::string&);

		//fill the safe package content
		// uncompressInput = true ==> unpack inputName into packagePath and return unpacked files in packageContent
		// uncompressInput = false ==> hard link inputName into packagePath and return the linked file in packageContent
		void fillSafePackage( std::string const& packagePath, std::string const& inputName, bool uncompressInput,
				std::vector<std::string>& packageContent ) const;

		// create the SAFE directory with the manifest file
		void createSafePackage (const std::string&);

		// Date format converters
		std::string invTime2SafeNameTime (const std::string& ) const;
		
	private:
		// Attributes and macros

		S3AuxManifest::FTypeDescMap _ftypeDescMap;
		std::string _platform;
		std::string _baseline;

		std::string _facilityName;
		std::string _organization;
		std::string _site;
		std::string _country;
		std::string _hwName;
		std::string _compName;
		std::string _compVers;

		std::string _swName;
		std::string _swVersion;
		unsigned int _ordId;

		ACS_CLASS_DECLARE_DEBUG_LEVEL (S3AuxReformatter);

	};
}


#endif //_S3AuxReformatter_H_
