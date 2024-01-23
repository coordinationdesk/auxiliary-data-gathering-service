// PRQA S 1050 EOF
/*

	Copyright 2014-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/05/20 12:16:13  marfav
	Addedd support for IPF Standalone app
	

*/


#ifndef _IPFLogLevelFilter_H_
#define _IPFLogLevelFilter_H_ 

#include <exFilter.h>

namespace acs {


/*! \class exLevelFilter implements a level filter based on the following msg class hierarchy:
    Debug, Info, Warning, Progress, Error
	
*/
class IPFLogLevelFilter : public exFilter // PRQA S 2153
{
public:
	explicit IPFLogLevelFilter(exostream*);
	virtual ~IPFLogLevelFilter() throw();

	IPFLogLevelFilter (const IPFLogLevelFilter&);
	IPFLogLevelFilter& operator= (const IPFLogLevelFilter&);
	
	virtual bool filterInsertion(exFilterable &);
private:
	IPFLogLevelFilter(); // not implemented

} ;

} // end namespace

#endif // _exLevelFilter_H_
