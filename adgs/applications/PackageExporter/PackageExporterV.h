/*
	Copyright 1995-2021, Exprivia SpA - DADF 
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE ofExprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libPackageExporter$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/11/09 13:28:07  nicvac
	Import PackageExporter
	
        
*/

#ifndef _PackageExporterV_H_
#define _PackageExporterV_H_

namespace acs {

//! \brief support class for versioning

class PackageExporterV {
public:
	PackageExporterV() ;
	~PackageExporterV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif 


