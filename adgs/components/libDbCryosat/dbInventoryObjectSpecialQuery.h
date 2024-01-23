// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.3  2017/04/07 13:27:25  marfav
	addLinkedFileTypesToWait operation const
	
	Revision 6.2  2017/04/07 10:36:57  marfav
	APF-412
	Added support to multiple waiting filetypes
	
	Revision 6.1  2017/02/02 16:07:36  marfav
	CSGACQ-110
	Explicit declaration of expiration dates in case of waiting order insertion
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.0  2013/09/30 12:30:58  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.10  2012/03/07 17:30:46  marpas
	new special query: dbInventoryOrderTriggeringFilesQuery added
	
	Revision 2.9  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.8  2010/03/23 17:29:04  marfav
	Added expiration date computation from available inventory data
	
	Revision 2.7  2010/03/12 17:26:31  marfav
	Added Delayed Inventory Objects Special Queries
	
	Revision 2.6  2009/11/25 19:32:23  marpas
	optional filter parameters stored in private data section.
	protected method to retrieve them added.
	
	Revision 2.5  2009/11/25 17:43:35  marpas
	dbInventoryObjectSpecialQuery::setOptionalFilter virtual method implemented
	
	Revision 2.4  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.3  2009/02/04 15:48:23  marfav
	Addedd doesOverlap method
	
	Revision 2.2  2008/12/17 14:41:29  paoscu
	Overloaded setParametersCLOB
	
	Revision 2.1  2006/10/24 16:01:59  marfav
	Addedd parameter file string attribute
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/03/08 10:17:35  marpas
	getTriggerType method interface fixed
	
	Revision 1.8  2005/03/04 23:11:06  marpas
	getTriggerType method added to special queries interface
	By default it returns always true, it has been, hence, reimplemented for some cases
	
	Revision 1.7  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.6  2004/07/14 09:36:32  paoscu
	documentation and some fixes
	
	Revision 1.5  2004/04/13 16:51:54  paoscu
	_queriedType added.
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.4  2003/07/25 15:17:53  paoscu
	potential memroy leaks removed
	order by management improved
	
	Revision 1.3  2003/05/06 11:15:31  paoscu
	std namespace policy changed.
	
	Revision 1.2  2002/12/17 17:45:57  paoscu
	A few changes in protected methods.
	
	Revision 1.1  2002/12/13 18:36:24  paoscu
	Imported
	
		

*/

#ifndef _dbInventoryObjectSpecialQuery_H_
#define _dbInventoryObjectSpecialQuery_H_

#include <dbInventoryObjectQuery.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)


class dbInventoryObjectSpecialQuery : // PRQA S 2109, 2153
	public dbInventoryObjectQuery
{
public:
	explicit dbInventoryObjectSpecialQuery(const std::string &dbkey)  ;
	virtual ~dbInventoryObjectSpecialQuery() throw() ;

	dbInventoryObjectSpecialQuery(dbInventoryObjectSpecialQuery const &) ;
	dbInventoryObjectSpecialQuery &operator=(const dbInventoryObjectSpecialQuery &) ;


	virtual DateTime const &t0() const;
	virtual DateTime const &t1() const;

	virtual double dt0() const;
	virtual double dt1() const;

	virtual void t0( DateTime const &);	
	virtual void t1( DateTime const &);	

	virtual void dt0(double);
	virtual void dt1(double);
	// returns a suitable trigger type for waiting orders defaulted to "FILETYPEMATCH" to be conservative
	virtual std::string getTriggerType() const ;

	virtual void setParametersCLOB ( std::istream& );
	virtual void setParametersCLOB ( std::string const & );
	virtual const std::string &getParametersCLOB () const;

	virtual void setOptionalFilter( const std::string &xml, const std::string &root) ;
	// returns if the special query has overlapping results
	// that needs to be filtered using the OverlapStrategy algos
	virtual bool doesOverlap () const;

	// Set and Get Delay Time
	virtual long long getDelayTime () const;
	virtual void setDelayTime (long long);

	virtual bool hasExpirationDate() const;
	virtual DateTime getComputedExpirationDate () const;

	// APF-412
	// Add the possibility to return a set of additional filetypes to be 
	// inserted in the waiting orders list
	// This is needed to manage coupled filetypes 
	virtual void addLinkedFileTypesToWait (std::vector <std::string>& ftArray) const;

	virtual std::string startTimeString() const;
	virtual std::string endTimeString() const;
	virtual DateTime startTime() const;
	virtual DateTime endTime() const;

protected:
	virtual std::string getValidityStartField() const;
	virtual std::string getValidityStopField() const;
	virtual std::string getOrderByField()	const;
	const std::string &getOptFilterXml() const ;
	const std::string &getOptFilterRoot() const ;
private: 
    dbInventoryObjectSpecialQuery() ; // not implemented
private:
	DateTime _t0;
	DateTime _t1;
	double _dt0;
	double _dt1;
	std::string _parameterCLOB;		
	std::string _optFilterXml;		
	std::string _optFilterRoot;		
	// delay time in seconds
	long long _delayTime;
		
};


_ACS_END_NAMESPACE

#endif // _dbInventoryObjectSpecialQuery_H_
