/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageFmtEOFpackTask$

	$Id$

	$Author$

	$Log$
	Revision 1.10  2017/02/21 18:09:25  nicvac
	S3EVOL-13: Datapackage serialization.
	
	Revision 1.9  2016/10/05 12:36:49  nicvac
	OPMAN-197: To support for QCC, headerseparation strategy has been modified.
	
	Revision 1.8  2016/02/26 11:12:13  nicvac
	OPMAN-151: fixed final extension. Fixed gzip on symlink.
	
	Revision 1.7  2016/02/25 11:31:26  nicvac
	Test and fix loop
	
	Revision 1.6  2016/02/24 17:03:30  nicvac
	OPMAN-151: Implementation completed.
	
	Revision 1.5  2016/02/23 17:20:52  nicvac
	OPMAN-151: format implementation
	
	Revision 1.4  2013/10/04 15:19:51  nicvac
	acs sdk warnings processed.
	
	Revision 1.3  2013/01/22 16:52:37  nicvac
	Fixed after unit test.
	
	Revision 1.2  2013/01/22 13:33:29  nicvac
	Implemented
	
	Revision 1.1.1.1  2013/01/21 16:33:30  nicvac
	Import PackageFmtEOFpackTask
	
        
*/

#ifndef _PackageFmtEOFpackTask_H_
#define _PackageFmtEOFpackTask_H_

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#endif

#include <ExpProcInfo.h>
#include <SpawnProc.h>
#include <exException.h>

#include <boost/program_options.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Takes EOF files (DBL and HDR), append HDR to DBL and archive it (tgz) 
**/

class PackageFmtEOFpackTask: public PROJECT_APP {
public:

	/** PackageFmtEOFpackTask Exceptions */
	exDECLARE_EXCEPTION(exPackageFmtEOFpackTaskException, exException) ; // Base PackageFmtEOFpackTask Exception.
	exDECLARE_EXCEPTION(exPackageFmtEOFpackTaskCriticalException, exPackageFmtEOFpackTaskException) ; // Critical Exception.


	/** Class constructor */
	PackageFmtEOFpackTask(const std::string& app, const std::string& subsys);
	/** Destructor */
	virtual ~PackageFmtEOFpackTask() throw();

	/** Run entry point */
	virtual int main(int argc, char const * const * argv, 
					 char const * const * env);

	/** Custom Signal Handlers */
	virtual void customSigIntHandler() ;
	virtual void customSigTermHandler() ;

    /**Command line Parameters*/
    static class CommandLineParameters {
    public:
    	CommandLineParameters():
    		expProcInfo       ("expProcInfo"),
    		headerOnly        ("headerOnly"),
    		formatFrom        ("formatFrom"),
    		cmdDoTar          ("cmdDoTar"),
    		cmdCompression    ("cmdCompression"),
    		cmdExtension      ("cmdExtension"),
    		zipLevel	      ("zipLevel"),
    		IgnoredPositionals("IgnoredPositionals")
    	{}
        const std::string expProcInfo       ;
    	const std::string headerOnly        ;
		const std::string formatFrom        ;
		const std::string cmdDoTar          ;
		const std::string cmdCompression    ;
		const std::string cmdExtension      ;
		const std::string zipLevel		;
        const std::string IgnoredPositionals;
    } cl;


private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageFmtEOFpackTask(const PackageFmtEOFpackTask & );
	/** Operator = */
	PackageFmtEOFpackTask &operator=(const PackageFmtEOFpackTask &);

private:

	/** Compression type */
	typedef enum {
		CompressionTypeNONE=0,
		CompressionTypeZIP,
		CompressionTypeGZIP
	} CompressionType;


	/** Execute general command configured in proc */
	void doCommand( SpawnProc& proc, std::string const& tag, std::vector<int> allowedExitCodes = std::vector<int>(1,0) );
	/** Tar */
	void doTar( std::string const& inAbsPathDir, std::vector<std::string> const& inRelPaths,
			std::string const& outAbsFilename );
	/** Gzip */
	void doGzip(bool preserveInput, std::string const& inAbsPathDir, std::vector<std::string> const& inRelPaths,
			std::string const& outAbsFilename );
	/** Zip */
	void doZip( std::string const& inAbsPathDir, std::vector<std::string> const& inRelPaths,
			std::string const& outAbsFilename );


	/** doTar: true/false
	 * compressionType: cf. CompressionType
	 * archiveExtension: extension to be used */
	void getCompressionOptions(	bool& doTar, CompressionType& compressionType, std::string& archiveExtension );

	/** get the Hdr and Dbl extension */
	void getHdrDblExtension( bool& headerSeparation, std::string& hdrExt, std::string& dblExt, std::string& filetypeRegExp );

	/** Compute the output product name (package name) */
	std::string getOutputProductName(std::vector<std::string>& packageContentRelPaths,
									bool doTar, CompressionType compressionType,
									std::string const& filetypeRegExp);

	/** Core of the app */
	int execute();

protected:
	/** I/O interface with the PackageExporter */
	ExpProcInfo _expProcInfo;

	/** Datapackage info */
	dataprovider::DataPackage _datapackage;

	/** Argv parameters */
	boost::program_options::variables_map _argvParameters;

	/** True when signel received */
	bool _stop;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageFmtEOFpackTask)

};

_ACS_END_NAMESPACE

#endif //_PackageFmtEOFpackTask_H_
