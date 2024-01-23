/*
	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libLTAProductExtractor$

	$Id$

	$Author$
*/
#ifndef _LTAProductExtractor_H_
#define _LTAProductExtractor_H_

#include <exException.h>
#include <iostream>
#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <LTAUtilities.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief Base class 
 */
class LTAProductExtractor 
{

public:

	exDECLARE_EXCEPTION(LTAProductExtractorException, exException) ; 
	
	/**
	 * \brief Default Class constructor
	 */
	LTAProductExtractor(std::string const & filename, const std::vector <Attribute> & attributes, const std::vector<std::string> &extractionRule);
	/**
	 * \brief  Class destructor
	 */
	virtual ~LTAProductExtractor() ACS_NOEXCEPT = default;

	void extract(const std::string & repository, const std::vector<std::string> & metadataFile, const ExtractionStrategy & extractionStrategy);
	void getMetadataFiles(std::vector<std::string> & metadataFile);
	void getMetadataFromRule(const std::string & rule, std::string & metadataFile);

protected:	
	/**
	 * \brief  Class copy constructor
	 */
	LTAProductExtractor(const LTAProductExtractor & );

	/**
	 * \brief  Operator=
	 */
	LTAProductExtractor &operator=(const LTAProductExtractor &);

private:

	void init(const std::vector <Attribute> & attributes, const std::vector<std::string> &extractionRule);
	void setTarArgs(const std::string & repository, const std::vector<std::string> & metadataFile, std::vector<std::string> & args);
	void setArgs(const std::string & repository, const std::vector<std::string> & metadataFile, std::vector<std::string> & args);
	void setArgs(std::vector<std::string> & args);
	
	std::string 		_productName;
	std::vector<std::string> _metadataFiles;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAProductExtractor) ;

};
_ACS_END_NAMESPACE

#endif //_LTAProductExtractor_H_
