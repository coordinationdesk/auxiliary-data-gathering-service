// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Statistical Report Generator $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/02 11:22:11  marpas
	adopting libException 5.x standards
	coding best practices
	qa warnings
	
	Revision 2.4  2013/03/28 13:47:55  marpas
	statistical message improved to allow integrators to check report generation efficiency
	
	Revision 2.3  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.2  2012/03/08 13:28:01  marpas
	robustness and messages improved
	
	Revision 2.1  2008/11/14 14:39:23  marpas
	moved here from application StatisticalReportGenerator
	
	Revision 2.0  2006/02/28 16:50:51  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/03/10 13:52:09  marpas
	maquillage
	
	Revision 1.4  2003/10/07 17:40:57  paoscu
	Base class Query interface changed.
	Better error message.
	
	Revision 1.3  2003/06/03 17:00:54  paoscu
	std namespace management changed.
	Extra info on log file about master/slave.
	
	Revision 1.2  2003/03/19 19:34:41  paoscu
	Working version. No Patrol support.
	
	Revision 1.1.1.1  2003/03/14 19:11:24  paoscu
	Import StatisticalReportGenerator
	
	

*/

#include<StatisticalReportGeneratorQuery.h>

using namespace acs;
using namespace std;

StatisticalReportGeneratorQuery::StatisticalReportGeneratorQuery(const string &s, const string &stat)
: Query(s, stat)
{}


StatisticalReportGeneratorQuery::~StatisticalReportGeneratorQuery() throw() {}

string StatisticalReportGeneratorQuery::getParameters(const string &s, std::string const & nameSpace)
{
	NotImplementedMethod e("Cannot run interactive queries: can't get value for parameter \"" + s + "\".");
	ACS_THROW(e);
}





