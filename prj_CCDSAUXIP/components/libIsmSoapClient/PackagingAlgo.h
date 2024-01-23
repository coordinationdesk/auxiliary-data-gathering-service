// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	
*/

#ifndef _PackagingAlgo_H_
#define _PackagingAlgo_H_ 

#include <rsResourceSet.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class SpawnProc; 

class PackagingAlgo // PRQA S 2109
{

public:

	exDECLARE_EXCEPTION(PackagingAlgoException, exException) ; // base exception // PRQA S 2131, 2502

	struct RetryParameters { // PRQA S 2173, 2175
		
		RetryParameters() ; 
		~RetryParameters() throw() {} 
		RetryParameters(const RetryParameters& ) ;
		RetryParameters& operator=(const RetryParameters& ) ;
		
		int maxNumber;
		unsigned long int waitingPeriod; // ms
	} ;
	
	struct FilesChangeFixStrategy { // PRQA S 2173, 2175
		
		FilesChangeFixStrategy() ; 
		~FilesChangeFixStrategy() throw() {} 
		FilesChangeFixStrategy(const FilesChangeFixStrategy& ) ;
		FilesChangeFixStrategy& operator=(const FilesChangeFixStrategy& ) ;
		
		RetryParameters retryParameters;
		bool ignoreAttributesChange;
	} ;
	

	
	/**
	 * \brief  Class constructor 
	 *    
	 */
	PackagingAlgo(const std::map<std::string, std::string> &archiveCustomOptions, const std::map<std::string, std::string> &extractCustomOptions, const FilesChangeFixStrategy & );

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

	int executeSpawn( SpawnProc & proc, const std::string & command, const std::string & pathToWorkOn, int computationTimeout, std::string & outmsg) ;
	std::map<std::string, std::string> getArchiveCustomOptions() const throw() { return _archiveCustomOptions; }
	std::map<std::string, std::string> getExtractCustomOptions() const throw() { return _extractCustomOptions; }
	FilesChangeFixStrategy getFilesChangeFixStrategy() const throw() { return _filesChangeFixStrategy; }
	
private:
		
	// copy constructor and operator= defined but not implemented
	PackagingAlgo(const PackagingAlgo &); 
	PackagingAlgo &operator=(const PackagingAlgo &);


// data
private:
	std::map<std::string, std::string> _archiveCustomOptions;
	std::map<std::string, std::string> _extractCustomOptions ;
	FilesChangeFixStrategy _filesChangeFixStrategy ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagingAlgo)
};


std::ostream &operator<<(std::ostream &s, const PackagingAlgo::RetryParameters & );

exostream &operator<<(exostream &s, const PackagingAlgo::RetryParameters & ) ;

template<>
void rsResourceSet::getValue(const std::string &res, PackagingAlgo::RetryParameters &v) const ;

std::ostream &operator<<(std::ostream &s, const PackagingAlgo::FilesChangeFixStrategy & );

exostream &operator<<(exostream &s, const PackagingAlgo::FilesChangeFixStrategy & ) ;

template<>
void rsResourceSet::getValue(const std::string &res, PackagingAlgo::FilesChangeFixStrategy &v) const ;

_ACS_END_NAMESPACE
		
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //_PackagingAlgo_H_
