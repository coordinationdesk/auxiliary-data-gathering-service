// PRQA S 1050 EOF
/*
	Copyright 2019-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	
	
        
*/

#ifndef _LTAS1SafePreInventory_H_
#define _LTAS1SafePreInventory_H_

#include <SafePreInventory.h>
#include <FileGuard.h>



_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * PreInventory plugin for S1 LTA SAFE products
 **/

class LTAS1SafePreInventory : public SafePreInventory // PRQA S 2109
{ 
public:

	/** LTAS1SafePreInventory Exceptions */
	exDECLARE_EXCEPTION(exLTAS1SafePreInventoryException, exException) ; // Base LTAS1SafePreInventory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exLTAS1SafePreInventoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502


	/** Default Class constructor */
	LTAS1SafePreInventory();
	/** Destructor */
	virtual ~LTAS1SafePreInventory() throw();

	virtual std::string getClassId();
	
protected:
	
    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &);
		
	/**
	 * \brief It responds to need of setting other metadata fields (or change something) in derivated classes
	 * \param Metadata object
	 * \param dbConnection object
	 */    	
	virtual void setFurther(Metadata &, dbConnection &);
	
	/**
	 * \brief It manages foot points list depending on SAFE format
	 * \param ManifestData object
	 * \param Metadata object
	 */    	
	virtual void manageFootPrintPosList(const ManifestData& , Metadata &) ;

private:
		
	LTAS1SafePreInventory(const LTAS1SafePreInventory & ); // not implemented
	LTAS1SafePreInventory &operator=(const LTAS1SafePreInventory &); // not implemented

	void extractFilesFromZipArchive( const std::string & zipArchive, ACS_SMARTPTR<FileGuard> & , std::string & productDir ) ;
			
public:
    //classe di Test CPPUnit
#ifdef CLASS_UNDER_TEST
    friend class LTAS1SafePreInventoryTest;
#endif //CLASS_UNDER_TEST

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAS1SafePreInventory);

};

_ACS_END_NAMESPACE

#endif //_LTAS1SafePreInventory_H_

