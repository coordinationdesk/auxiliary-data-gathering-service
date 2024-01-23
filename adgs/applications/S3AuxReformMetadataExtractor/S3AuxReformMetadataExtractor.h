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
	Revision 1.1.1.1  2013/06/13 10:06:40  marfav
	Importing S3AuxReformMetadataExtractor
	

*/

#ifndef _S3AuxReformMetadataExtractor_H_
#define _S3AuxReformMetadataExtractor_H_ 


#if defined (HAS_MISSION_INCLUDES)
	#include <ProjectAppIncludes>
#else
	#include <CryosatApp.h>
#endif

#include <exMacros.h>

namespace acs {

  
#if   defined (HAS_MISSION_INCLUDES)
	class S3AuxReformMetadataExtractor : public PROJECT_APP
#else
	class S3AuxReformMetadataExtractor : public CryosatApp
#endif
	{
	public:

		struct TriggeringFile
		{
			std::string fileName;
			std::string fileVersion;

			TriggeringFile() : fileName(), fileVersion() {}
		};
			
		/*! class S3AuxReformMetadataExtractorException declaration */
		exDECLARE_EXCEPTION(S3AuxReformMetadataExtractorException,exException) ;
		
		/**
		 * \brief  Class constructor 
		 *    
		 */
		S3AuxReformMetadataExtractor(const std::string& appName, const std::string& subSys);
		
		/**
		 * \brief  Class destructor 
		 *    
		 */    	
		~S3AuxReformMetadataExtractor() throw() ;
		
		virtual int main(int argc, char const * const * argv, char const * const * env);
		
		using Application::usage;		

	private:

		// copy constructor and operator= defined but not implemented
		S3AuxReformMetadataExtractor(const S3AuxReformMetadataExtractor & ); 
		S3AuxReformMetadataExtractor &operator=(const S3AuxReformMetadataExtractor &);
			
		//methods
		void usage(int ,  char const * const *) const;
		int execute(const std::string & jobOrderFileName);

		TriggeringFile getTriggerFile (const std::string& ) const;

		// Generate the metadata file
		void createMetadata (const std::string&, const TriggeringFile&);

		
	private:
		// Attributes and macros

		ACS_CLASS_DECLARE_DEBUG_LEVEL (S3AuxReformMetadataExtractor);

	};
}


#endif //_S3AuxReformMetadataExtractor_H_
