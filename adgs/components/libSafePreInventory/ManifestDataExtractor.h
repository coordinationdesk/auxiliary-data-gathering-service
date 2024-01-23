// PRQA S 1050 EOF
/*

	Copyright 2013-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Author$

	Id: $
	
	$Log$
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	
	
	
*/

#ifndef _ManifestDataExtractor_H_
#define _ManifestDataExtractor_H_ 

#include <SafePreInventory.h>

#include <exException.h>

namespace acs {

	/**
	  * \brief 
	  */
	class ManifestDataExtractor
	{
		
	public:
			

      	/** ManifestDataExtractor Exceptions */
      	exDECLARE_EXCEPTION(exManifestDataExtractorException, exException) ;
		
		/**
		 * \brief  Class constructor 
		 *    
		 */
		ManifestDataExtractor();

		/**
		 * \brief  Class destructor 
		 */    	
		virtual ~ManifestDataExtractor() throw();
		
		
		/**
		 * \brief Fill ManifestData struct. To be specialized in derived classes.
	  	 * \param The manifest filepath 
	  	 * \param ManifestData struct 
		 */
		virtual void doWork(std::string const &, SafePreInventory::ManifestData &) = 0;
		
		/**
		 * \brief Fill _additionalDataFields vector.
	  	 * \param Input pairs vector
		 */
		void setAdditionalDataFields(std::vector< std::pair<std::string, std::string> > const &);
		
		/**
		 * \brief Get _additionalDataFields vector.
	  	 * \return _additionalDataFields vector
		 */
		std::vector< std::pair<std::string, std::string> > getAdditionalDataFields();
		
		/**
		 * \brief Fill _filetype attribute.
	  	 * \param Input filetype
		 */
		void setFiletype(const std::string & v) throw() { _filetype = v; }
		
		/**
		 * \brief Return _filetype attribute.
	  	 * \return filetype attribute
		 */
		std::string getFiletype() const throw() { return _filetype; }
		
		/**
		 * \brief Fill _confSpaceId attribute.
	  	 * \param Input conf space id
		 */
		void setConfSpaceId(const std::string & v) throw() { _confSpaceId = v; }
		
		/**
		 * \brief Return _confSpaceId attribute.
	  	 * \return conf space id attribute
		 */
		std::string getConfSpaceId() const throw() { return _confSpaceId; }
		
	private:
		// copy constructor and operator= defined but not implemented
		ManifestDataExtractor(const ManifestDataExtractor &); 
		ManifestDataExtractor &operator=(const ManifestDataExtractor &);
				

// data
	private:
			
		std::vector< std::pair<std::string, std::string> > _additionalDataFields; // (XPaths, name of metadata field)
		std::string _filetype;
		std::string _confSpaceId;

	
		ACS_CLASS_DECLARE_DEBUG_LEVEL(ManifestDataExtractor);
	};


}// Closing namespace acs

#endif //_ManifestDataExtractor_H_
