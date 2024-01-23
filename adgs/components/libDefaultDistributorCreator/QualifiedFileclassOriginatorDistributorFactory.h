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

#ifndef _QualifiedFileclassOriginatorDistributorFactory_H_
#define _QualifiedFileclassOriginatorDistributorFactory_H_

#include <DistributorCreatorFactory.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Factory for QualifiedFileclassOriginator Algo
**/

class QualifiedFileclassOriginatorDistributorFactory: public DistributorCreatorFactory { // PRQA S 2109
public:

	/** Default Class constructor */
	QualifiedFileclassOriginatorDistributorFactory();
	/** Destructor */
	virtual ~QualifiedFileclassOriginatorDistributorFactory() throw();

	/** Interface implementation */
	virtual DistributorCreator *newCreator(const std::string &algo) ;
	virtual void capabilities(std::vector<std::string> &) const ;

private:
	//Copy not allowed
	QualifiedFileclassOriginatorDistributorFactory(const QualifiedFileclassOriginatorDistributorFactory & );
	QualifiedFileclassOriginatorDistributorFactory &operator=(const QualifiedFileclassOriginatorDistributorFactory &);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifiedFileclassOriginatorDistributorFactory)

};

_ACS_END_NAMESPACE

#endif //_QualifiedFileclassOriginatorDistributorFactory_H_

