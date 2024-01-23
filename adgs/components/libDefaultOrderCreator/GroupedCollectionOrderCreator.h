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

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.2  2012/03/07 09:42:42  marpas
	refactoring in progress
	
	Revision 2.1  2006/04/05 15:07:52  giucas
	nested class debug added
	
	Revision 2.0  2006/02/28 10:24:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/

#ifndef _GroupedCollectionOrderCreator_H_
#define _GroupedCollectionOrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <TemporaryOrder.h>



_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbOrder ;

class GroupedCollectionOrderCreator: public OrderCreator // PRQA S 2109
{
public:
	GroupedCollectionOrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection & );
	virtual ~GroupedCollectionOrderCreator() throw() ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;

private:
	// declared but not implemented 
	GroupedCollectionOrderCreator();
	GroupedCollectionOrderCreator(const GroupedCollectionOrderCreator&);
	GroupedCollectionOrderCreator & operator=(const GroupedCollectionOrderCreator&);
private:
	void createOrder(std::vector<TemporaryOrder> &tl, dbConnection &) const;
    void fillOrder(dbOrder &, const std::vector<TemporaryOrder> &) const ;
	void loadGroups() ;
	size_t assignToGroup(const TemporaryOrder &) const ;

	class Group { // PRQA S 2109
	public:
		Group(const std::vector<std::string> &, double wait, double timewindow, bool anyFTypeMatch=false) ;
		~Group() throw() ;
		Group(const Group &) ;
		Group &operator=(const Group &) ;

		bool matchFileType(const std::string &) const ;
		double maxWaitingTime() const ;
		double timeWindow() const ;
		void addToGroup(const TemporaryOrder &) const ;
		void getGroup(std::vector<TemporaryOrder> &) const ;
		bool getChain(std::vector<TemporaryOrder> &) const ;
	private:
		bool addFirstToChain(std::vector<TemporaryOrder> &) const ;
        Group() ; // not implemented
	private:
		mutable std::vector<TemporaryOrder> _group ;
		std::vector<std::string> _filetypes ;
		double _maxWaitingTime ;
		double _timeWindow ;
		bool _anyFTypeMatch ;
		ACS_CLASS_DECLARE_DEBUG_LEVEL(Group) ;
	} ;

private:
	std::vector<Group> _groups;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(GroupedCollectionOrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _GroupedCollectionOrderCreator_H_ */
