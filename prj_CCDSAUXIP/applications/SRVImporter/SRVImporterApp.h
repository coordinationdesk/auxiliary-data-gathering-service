/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libSRVImporterApp$
        
*/

#ifndef _SRVImporterApp_H_
#define _SRVImporterApp_H_

#include <Importer.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* SRV specialization of Importer
**/

class SRVImporterApp: public Importer  {
public:

	/** Class constructor */
	SRVImporterApp(std::string const& appName, std::string const& subSystem);
	/** Destructor */
	virtual ~SRVImporterApp() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS(SRVImporterApp);
	
protected:
	/** IF implementation. cf. Base class */
	virtual void getConfigurationSpaceInfo( std::string & mainNamespace, std::vector<std::string> & confSpaceList );

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVImporterApp)

};

_ACS_END_NAMESPACE

#endif //_SRVImporterApp_H_

