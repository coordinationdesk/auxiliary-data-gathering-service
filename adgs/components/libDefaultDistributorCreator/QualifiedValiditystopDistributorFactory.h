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
	Revision 2.1  2015/05/15 16:31:05  nicvac
	APF-264
	
        
*/

#ifndef _QualifiedValiditystopDistributorFactory_H_
#define _QualifiedValiditystopDistributorFactory_H_

#include <DistributorCreatorFactory.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Factory for both QualifiedValiditystop Less and Greater Algo
**/

class QualifiedValiditystopDistributorFactory: public DistributorCreatorFactory { // PRQA S 2109
public:

	/** Default Class constructor */
	QualifiedValiditystopDistributorFactory();
	/** Destructor */
	virtual ~QualifiedValiditystopDistributorFactory() throw();

	/** Interface implementation */
	virtual DistributorCreator *newCreator(const std::string &algo) ;
	virtual void capabilities(std::vector<std::string> &) const ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	QualifiedValiditystopDistributorFactory(const QualifiedValiditystopDistributorFactory & );
	/** Operator = */
	QualifiedValiditystopDistributorFactory &operator=(const QualifiedValiditystopDistributorFactory &);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifiedValiditystopDistributorFactory)

};

_ACS_END_NAMESPACE

#endif //_QualifiedValiditystopDistributorFactory_H_

