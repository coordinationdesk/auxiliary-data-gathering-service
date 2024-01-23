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

#ifndef _S2PreInventoryReformatterTask_H_
#define _S2PreInventoryReformatterTask_H_


#if defined (HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
	#include <StationApp.h>					
#elif COSMO
	#include <CosmoApp.h>					
#else
    #include <CryosatApp.h>
#endif


#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class Metadata ;
/**
* \brief
* S2PreInventoryReformatterTask application 
**/

class S2PreInventoryReformatterTask: public PROJECT_APP // PRQA S 2109, 2153
{ 
public:

	/** S2PreInventoryReformatterTask Exceptions */
	exDECLARE_EXCEPTION(exS2PreInventoryReformatterTaskException, exException) ; // Base S2PreInventoryReformatterTask Exception. // PRQA S 2131, 2502 2


	/** Class constructor */
	S2PreInventoryReformatterTask(const std::string& subsys, const std::string& app);
	/** Destructor */
	~S2PreInventoryReformatterTask() throw() ;
    int usage ( const std::string & ) const ;

private:
	S2PreInventoryReformatterTask(); // not implemented
	S2PreInventoryReformatterTask(const S2PreInventoryReformatterTask & ); // not implemented
	S2PreInventoryReformatterTask &operator=(const S2PreInventoryReformatterTask &); // not implemented

	void loadConf();
	void execute();
	std::string getFilepathFromExt(const std::string & fileExt) const ;

	void reformat(  const std::string & filetype, const std::string & finalFiletype, const std::string & metadataFilepath, Metadata & );

	void writeHdrFile(const std::string & hdrFilePath, const std::string & filename, const std::string & fileType, const Metadata & ) ;
	void createPackage( const std::string & hdrFilePath, const std::string & dblFilePath, const std::string & tgzFilePath ) const;
	void updateMetadata( const std::string & filename, const std::string & filetype, off_t fileSize, Metadata &  ) const;

	std::string getFilenameFromFileTypeConf( const std::string & filename, const std::string & filetype, bool toMetadata ) const ;
	std::string getMission( const Metadata & ) const;
	std::string getTime( const Metadata &, const std::string & field ) const ;
	void updateListFile( const std::string & oldFilename, const std::string & newFilename, const std::string & listFilepath ) const ;
	
public:
	/** Entry point */
	virtual int main(int argc, char const * const * argv, char const * const * env);

private:
	std::string _mtdExtension;
	std::string _listExtension;
	std::string _wdir;
	std::map<std::string, std::string>  _filetypesToManage;
	long int _tarExecutionTimeout;
	std::string _hdrSystemFromFilename;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S2PreInventoryReformatterTask)

};


_ACS_END_NAMESPACE

#endif //_S2PreInventoryReformatterTask_H_

