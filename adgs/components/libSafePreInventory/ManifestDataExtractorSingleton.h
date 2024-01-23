// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	
*/ 


#ifndef _ManifestDataExtractorSingleton_H_
#define _ManifestDataExtractorSingleton_H_

#include <ManifestDataExtractor.h>
#include <exException.h>
#include <map>


namespace acs{

	/**
     * \brief .
     **/

class ManifestDataExtractorSingleton 
{
	public:
		/*! class KeyNotFound declaration */
		exDECLARE_EXCEPTION(KeyNotFound,exException) ;
		 /** ManifestDataExtractorSingleton Exception */
      	exDECLARE_EXCEPTION(exManifestDataExtractorSingletonException, exException) ;
				
		static ManifestDataExtractorSingleton* instance();
		
		void registerFactory(std::string const &, ManifestDataExtractor* (*)() );
		
		void unregisterFactory(std::string const &, ManifestDataExtractor* (*)() );

		/**
		 * \brief  Return an object derived from ManifestDataExtractor class
	  	 * \param The map key
	  	 * \param The manifest file
		 */    	
		ManifestDataExtractor *newManifestDataExtractor(std::string const &) const;
		

		static void release() ; 

	protected:
		
		ManifestDataExtractorSingleton();
	
	private:

		
// data
	private:
	
		static ManifestDataExtractorSingleton* _instance;
		static ThreadSafe 					_mutex;
		
		std::map<std::string, ManifestDataExtractor* (*)()> _map;


		ACS_CLASS_DECLARE_DEBUG_LEVEL(ManifestDataExtractorSingleton);
 
};


}// Closing namespace acs

#endif //_ManifestDataExtractorSingleton_H_
