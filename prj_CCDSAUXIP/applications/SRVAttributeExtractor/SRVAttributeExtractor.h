// PRQA S 1050 EOF
/*

    Copyright 2020, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    SRV Attribute Extractor main

*/ 
#ifndef _SRVAttributeExtractor_H_
#define _SRVAttributeExtractor_H_ 

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

//#include <ProjectAppIncludes>
#include <ProjectStandaloneAppIncludes>
#include <acs_c++config.hpp>
#include <FileGuard.h>

_ACS_BEGIN_NAMESPACE(acs)

class SRVAttributeExtractor : // PRQA S 2109, 2153
	public PROJECT_STANDALONE_APP 
{
public:
	exDECLARE_EXCEPTION(SRVAttributeExtractorException, exException); // PRQA S 2131, 2502
	
public:
	SRVAttributeExtractor( const std::string&, const std::string& );
	virtual ~SRVAttributeExtractor() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

	// inherited from Application
// 	virtual void customSigIntHandler(); 
// 	virtual void customSigTermHandler(); 
	virtual int usage(const std::string &) const ;
private:
	// Deleted constructors
	SRVAttributeExtractor(const SRVAttributeExtractor&) = delete;
	SRVAttributeExtractor & operator=(const SRVAttributeExtractor&) = delete;

	bool loadConf();
	void loadConfFiles();
	void execute(const std::string & jobOrderFileName);
	virtual std::string getDefaultLogName() const;

private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVAttributeExtractor) ;

private:
	static const unsigned int _defaultRestartTime=3600*24 ; // [sec] (one day)
	static const std::string _defaultExtension;
	
	std::string _extension;
	bool	_cleanTempDir;		// read by conf space - indicates if temporary directory are to be removed after run
	bool	_optionalAttributes;	// read by conf space - indicates if all the attributes (and metadata files) can be considered optional
};

_ACS_END_NAMESPACE

#endif /* _SRVAttributeExtractor_H_ */
