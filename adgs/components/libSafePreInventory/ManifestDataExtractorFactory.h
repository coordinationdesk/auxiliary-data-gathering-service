// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	

*/


#ifndef _ManifestDataExtractorFactory_H_
#define _ManifestDataExtractorFactory_H_ 

#include <exException.h>

namespace acs
{

class ManifestDataExtractor;

class ManifestDataExtractorFactory
{
	public:

		ManifestDataExtractorFactory();
		
		~ManifestDataExtractorFactory();

		static ManifestDataExtractor* buildS3AuxManifestDataExtractor();		
		static ManifestDataExtractor* buildS3ProdManifestDataExtractor();
		static ManifestDataExtractor* buildS1ProdManifestDataExtractor();
		static ManifestDataExtractor* buildS1AuxManifestDataExtractor();

	private:

		ACS_CLASS_DECLARE_DEBUG_LEVEL(ManifestDataExtractorFactory);

};

}//namespace

#endif // _ManifestDataExtractorFactory_H_

