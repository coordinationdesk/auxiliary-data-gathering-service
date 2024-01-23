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
	Revision 2.4  2016/11/29 16:08:41  francesco.avanzi
	workInProgres
	
	Revision 2.3  2016/06/01 12:58:39  lucio.pulvirenti
	APF-379: QUALOK, QUALOKVALSTOPLT, QUALOKVALSTOPGT algorithm names added. _qualOk boolean attribute added.
	
	Revision 2.2  2015/08/19 12:15:30  nicvac
	APF-306: Algo QUALIFIEDALWAYS
	
	Revision 2.1  2015/05/15 16:31:05  nicvac
	APF-264
	
        
*/

#ifndef _QualifiedValiditystopDistributorCreator_H_
#define _QualifiedValiditystopDistributorCreator_H_

#include <DistributorCreator.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
*
* Cf. http://jira.acsys.it/browse/APF-264
* Cf. http://jira.acsys.it/browse/APF-251
* Cf. http://jira.acsys.it/browse/APF-306
* Cf. http://jira.acsys.it/browse/APF-379
*
* Create a distribution if:
*
* algorithm QUALIFIEDALWAYS case:
	- Qualified report is not applicable OR Qualified report is applicable and available
*
* algorithm QUALIFIEDVALSTOPLT, QUALIFIEDVALSTOPGT case:
	- Qualified report is not applicable OR Qualified report is applicable and available
	AND
	- if |valStop - now| < threshold. Or >, according to chosen algorithm (QUALIFIEDVALSTOPLT, QUALIFIEDVALSTOPGT);

* algorithm QUALOK case:
	- Qualified report is not applicable OR ( Quality info is true and Qualified report is valid ) OR ( Quality info is false and Qualified report is available )

* algorithms QUALOKVALSTOPLT, QUALOKVALSTOPGT case:
	- Qualified report is not applicable OR ( Quality info is true and Qualified report is valid ) OR ( Quality info is false and Qualified report is available )
	AND
	- if |valStop - now| < threshold. Or >, according to chosen algorithm (QUALOKVALSTOPLT, QUALOKVALSTOPGT);

* System level notes:
* The Qualified report (if applicable) will be downloaded during the Export processing.
* The export processing will fail if report cannot be downloaded.

**/

static const char QUALIFIEDVALSTOPGT[] = "QUALIFIEDVALSTOPGT";
static const char QUALIFIEDVALSTOPLT[] = "QUALIFIEDVALSTOPLT";
static const char QUALIFIEDALWAYS[] = "QUALIFIEDALWAYS";
static const char QUALOK[] = "QUALOK";
static const char QUALOKVALSTOPGT[] = "QUALOKVALSTOPGT";
static const char QUALOKVALSTOPLT[] = "QUALOKVALSTOPLT";

class dbConnectionPool ;

class QualifiedValiditystopDistributorCreator: public DistributorCreator { // PRQA S 2109
public:

	/** Class constructor */
	explicit QualifiedValiditystopDistributorCreator(const std::string &algo);
	/** Destructor */
	virtual ~QualifiedValiditystopDistributorCreator() throw();

	/** Interface implementation */
	virtual void createDistribution(dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) const ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	QualifiedValiditystopDistributorCreator(const QualifiedValiditystopDistributorCreator & );
	/** Operator = */
	QualifiedValiditystopDistributorCreator &operator=(const QualifiedValiditystopDistributorCreator &);
	QualifiedValiditystopDistributorCreator();
	
private:
	/** Time threshold. [sec]*/
	double _threshold;

	/** Less than or greater than mode */
	typedef enum { less, greater, noCheck } CheckMode;
	CheckMode _checkMode;
	
	bool _qualOk;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifiedValiditystopDistributorCreator)

};

_ACS_END_NAMESPACE

#endif //_QualifiedValiditystopDistributorCreator_H_

