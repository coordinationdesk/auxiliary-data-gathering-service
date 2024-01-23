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
	Revision 2.1  2017/11/22 13:52:30  nicvac
	ECICMF-97: QualifiedFileclassOriginatorDistributorCreator added.
	
        
*/ 

#include <QualifiedFileclassOriginatorDistributorFactory.h>

#include <QualifiedFileclassOriginatorDistributorCreator.h>

#include <StringUtils.h>
#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QualifiedFileclassOriginatorDistributorFactory)



QualifiedFileclassOriginatorDistributorFactory::QualifiedFileclassOriginatorDistributorFactory() : 
    DistributorCreatorFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

QualifiedFileclassOriginatorDistributorFactory::~QualifiedFileclassOriginatorDistributorFactory() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

DistributorCreator * QualifiedFileclassOriginatorDistributorFactory::newCreator(const string &algo) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"algo:\"" << algo <<"\"");

	if ( StringUtils::equalsNoCase(algo, QUALIFIEDFILECLASSORIGINATOR) ) {

		return new QualifiedFileclassOriginatorDistributorCreator( QUALIFIEDFILECLASSORIGINATOR );

	} else {

		return 0;
	}
}

void QualifiedFileclassOriginatorDistributorFactory::capabilities(vector <string> &cap) const
{
	cap.push_back( string(" ")+ QUALIFIEDFILECLASSORIGINATOR +'\n'); // PRQA S 3081

	return ;
}

_ACS_END_NAMESPACE
