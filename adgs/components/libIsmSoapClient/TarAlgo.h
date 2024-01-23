// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	
	
	
*/

#ifndef _TarAlgo_H_
#define _TarAlgo_H_ 

#include <PackagingAlgo.h>

_ACS_BEGIN_NAMESPACE(acs)

class TarAlgo :  public PackagingAlgo // PRQA S 2109
{

public:

	/**
	 * \brief  Class constructor 
	 *    
	 */
	TarAlgo(const std::map<std::string, std::string> &archiveCustomOptions, const std::map<std::string, std::string> &extractCustomOptions);

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~TarAlgo() throw();
	
	/**
	 * \brief pack item
	 * \param 
	 */
	virtual void pack( const std::string & itemPath, const std::string & packagePath, int computationTimeout, const long long int itemSize ) ;
	
	/**
	 * \brief unpack item
	 * \param 
	 */
	virtual void unpack( const std::string & packagePath, const std::string & itemPath, int computationTimeout );
	
	/**
	 * \brief estimate package size
	  * \param 
	 */
	virtual long long int estimatePackageSize( const long long int itemSize );
	
protected:
	
	void doWork(const std::string & options, const std::string & packagePath, const std::string & itemPath, bool extract, int computationTimeout, const long long int itemSize = 0 );


private:
	// copy constructor and operator= defined but not implemented
	TarAlgo(const TarAlgo &); 
	TarAlgo &operator=(const TarAlgo &);
	
	void checkBlockingFactor(const long long int itemSize, std::map<std::string, std::string> & customOptions, int & blockingFactor) ;


// data
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TarAlgo)
};


_ACS_END_NAMESPACE

#endif //_TarAlgo_H_
