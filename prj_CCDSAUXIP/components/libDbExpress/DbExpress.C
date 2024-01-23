// PRQA S 1050 EOF
/*
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libDbExpress$

	$Id$

	$Author$

	$Log$
	Revision 3.0  2010/09/10 16:11:59  marpas
	new file dbxpressIf.h with template code to support DbExpress types and I/F
	new documentation
	other improvements in existing files
	
	Revision 2.0  2010/02/19 11:18:57  marpas
	ORACLE dependency from now on
	
	Revision 1.1.1.1  2008/07/21 15:28:10  paoscu
	Importing dbExpress
	
        
*/ 

#include <DbExpress.h>
#include "DbExpressV.h"

namespace acs {

namespace {
    const DbExpressV version; // NOSONAR 
}

DbExpress::DbExpress() {} // NOSONAR - to load the object file into the lib

}
