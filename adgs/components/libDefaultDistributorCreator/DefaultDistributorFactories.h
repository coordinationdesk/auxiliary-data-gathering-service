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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2015/05/15 16:31:05  nicvac
	APF-264
	
	Revision 2.1  2014/02/05 15:39:14  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.2  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.1  2012/02/29 15:20:41  chicas
	Added
	
	
	
*/

#ifndef _DefaultDistributorFactories_H_
#define _DefaultDistributorFactories_H_

#include <DistributorCreatorFactory.h>
#include <PreselectionDistributorFactory.h> 
#include <AlwaysDistributorFactory.h>
#include <QualifyReportDistributorFactory.h>
#include <QCCReportDistributorFactory.h>
#include <QualifiedDistributorFactory.h>
#include <RedistributionDistributorFactory.h>
#include <PartialCvrgDistributorFactory.h>
#include <TotalCvrgDistributorFactory.h>
#include <QualifiedValiditystopDistributorFactory.h>
#include <QualifiedFileclassOriginatorDistributorFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

class DefaultDistributorFactories // PRQA S 2109
{	

public:

	DefaultDistributorFactories() {} ;
	~DefaultDistributorFactories() throw() {};

private:

	// declared but not implemented	
	DefaultDistributorFactories(const DefaultDistributorFactories &) ;
	DefaultDistributorFactories &operator=(const DefaultDistributorFactories &) ; 
	

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultDistributorFactories);

    static PreselectionDistributorFactory thePreselectionDistributorFactory ;
    static AlwaysDistributorFactory theAlwaysDistributorFactory ;
    static QualifiedDistributorFactory theQualifiedDistributorFactory ;
    static QualifyReportDistributorFactory theQualifyReportDistributorFactory ;
    static QCCReportDistributorFactory theQCCReportDistributorFactory ;
    static RedistributionDistributorFactory theRedistributionDistributorFactory ;
    static TotalCvrgDistributorFactory theTotalCvrgDistributorFactory ;
    static PartialCvrgDistributorFactory thePartialCvrgDistributorFactory ;
    static QualifiedValiditystopDistributorFactory theQualifiedValiditystopDistributorFactory;
    static QualifiedFileclassOriginatorDistributorFactory theQualifiedFileclassOriginatorDistributorFactory;

};


_ACS_END_NAMESPACE

#endif /* _DefaultDistributorFactories_H_ */

