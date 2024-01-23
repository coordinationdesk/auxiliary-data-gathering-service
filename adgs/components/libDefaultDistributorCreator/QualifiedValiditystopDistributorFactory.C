// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDefaultDistributorCreator$

	$Id$

	$Author$

	$Log$
	Revision 2.3  2016/06/01 13:03:10  lucio.pulvirenti
	APF-379: QUALOK, QUALOKVALSTOPLT, QUALOKVALSTOPGT algorithms added.
	
	Revision 2.2  2015/08/19 12:15:30  nicvac
	APF-306: Algo QUALIFIEDALWAYS
	
	Revision 2.1  2015/05/15 16:31:05  nicvac
	APF-264
	
        
*/ 

#include <QualifiedValiditystopDistributorFactory.h>

#include <QualifiedValiditystopDistributorCreator.h>

#include <StringUtils.h>
#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QualifiedValiditystopDistributorFactory)


QualifiedValiditystopDistributorFactory::QualifiedValiditystopDistributorFactory() :
    DistributorCreatorFactory() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

QualifiedValiditystopDistributorFactory::~QualifiedValiditystopDistributorFactory() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

DistributorCreator *QualifiedValiditystopDistributorFactory::newCreator(const string &algo) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"algo:\"" << algo <<"\"");

	if ( StringUtils::equalsNoCase(algo, QUALIFIEDVALSTOPGT) ) {

		return new QualifiedValiditystopDistributorCreator( QUALIFIEDVALSTOPGT );

	} else if ( StringUtils::equalsNoCase(algo, QUALIFIEDVALSTOPLT) ) {

		return new QualifiedValiditystopDistributorCreator( QUALIFIEDVALSTOPLT );

	} else if ( StringUtils::equalsNoCase(algo, QUALIFIEDALWAYS) ) {

		return new QualifiedValiditystopDistributorCreator( QUALIFIEDALWAYS );

	} else if ( StringUtils::equalsNoCase(algo, QUALOK) ) {

		return new QualifiedValiditystopDistributorCreator( QUALOK );

	} else if ( StringUtils::equalsNoCase(algo, QUALOKVALSTOPGT) ) {

		return new QualifiedValiditystopDistributorCreator( QUALOKVALSTOPGT );

	} else if ( StringUtils::equalsNoCase(algo, QUALOKVALSTOPLT) ) {

		return new QualifiedValiditystopDistributorCreator( QUALOKVALSTOPLT );

	} else {

		return 0;

	}
}


void QualifiedValiditystopDistributorFactory::capabilities(vector <string> &cap) const
{
	cap.push_back( string(" ")+ QUALIFIEDVALSTOPGT +'\n'); // PRQA S 3081 6
	cap.push_back( string(" ")+ QUALIFIEDVALSTOPLT +'\n');
	cap.push_back( string(" ")+ QUALIFIEDALWAYS +'\n');
	cap.push_back( string(" ")+ QUALOK +'\n');
	cap.push_back( string(" ")+ QUALOKVALSTOPGT +'\n');
	cap.push_back( string(" ")+ QUALOKVALSTOPLT +'\n');

	return ;
}



_ACS_END_NAMESPACE

