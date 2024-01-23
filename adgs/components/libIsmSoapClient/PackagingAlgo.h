// PRQA S 1050 EOF
/*

	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	
*/

#ifndef _PackagingAlgo_H_
#define _PackagingAlgo_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class SpawnProc; 

class PackagingAlgo // PRQA S 2109
{

public:

	exDECLARE_EXCEPTION(PackagingAlgoException, exException) ; // base exception // PRQA S 2131, 2502
	
	/**
	 * \brief  Class constructor 
	 *    
	 */
	PackagingAlgo(const std::map<std::string, std::string> &archiveCustomOptions, const std::map<std::string, std::string> &extractCustomOptions );

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~PackagingAlgo() throw();


	/**
	 * \brief pack item
	 * \param 
	 */
	virtual void pack( const std::string & itemPath, const std::string & packagePath, int computationTimeout, const long long int itemSize  ) = 0;
	
	/**
	 * \brief unpack item
	 * \param 
	 */
	virtual void unpack( const std::string & packagePath, const std::string & itemPath,  int computationTimeout  ) = 0;
	
	/**
	 * \brief estimate package size
	  * \param 
	 */
	virtual long long int estimatePackageSize( const long long int itemSize ) = 0;

protected:	

	void executeSpawn( SpawnProc & proc, const std::string & command, const std::string & pathToWorkOn, int computationTimeout) ;
	std::map<std::string, std::string> getArchiveCustomOptions() const throw() { return _archiveCustomOptions; }
	std::map<std::string, std::string> getExtractCustomOptions() const throw() { return _extractCustomOptions; }
	
private:
		
	// copy constructor and operator= defined but not implemented
	PackagingAlgo(const PackagingAlgo &); 
	PackagingAlgo &operator=(const PackagingAlgo &);


// data
private:
	std::map<std::string, std::string> _archiveCustomOptions;
	std::map<std::string, std::string> _extractCustomOptions ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagingAlgo)
};




_ACS_END_NAMESPACE

#endif //_PackagingAlgo_H_
