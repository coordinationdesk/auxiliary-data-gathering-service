/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Algo Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/09/26 19:25:16  nicvac
	S2PDGS-303: Rolling actions can be ordered on inventory fields criteria.
	
	Revision 2.0  2013/09/23 12:19:34  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 1.7  2013/09/19 17:33:52  nicvac
	S2PDGS-303: implementation.
	
	Revision 1.6  2013/09/13 15:34:23  nicvac
	S2PDGS-303 implementing.
	
	Revision 1.5  2012/06/06 16:25:51  marfav
	Code improved removing compiler warnings
	
	Revision 1.4  2008/10/09 17:18:18  ivafam
	Work in progress
	
	Revision 1.3  2008/10/01 13:54:00  ivafam
	Comment improved
	
	Revision 1.2  2008/09/18 07:05:05  ivafam
	Work in progress
	
	Revision 1.1.1.1  2008/05/27 17:09:02  ivafam
	Import libRollingAction
	
	
*/



#ifndef _RollingAction_H_
#define _RollingAction_H_ 

#include <string>

#include <Functional.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)
/** This class is a container of information related to RollingArchive activities  */
class RollingAction  // PRQA S 2109
{		
public:
	RollingAction( const dbPersistent::IdType&, const unsigned int & actionid, const unsigned int & storage=0);
	~RollingAction();

	RollingAction(const RollingAction&);
	RollingAction & operator=(const RollingAction&);

	/** Return Id of the invetory object on which apply the action */
	dbPersistent::IdType getObject() const throw() { return _obj ; }

    /**  Returns the actionid */
	unsigned int getRollingAction() const throw() { return _RollingActionid ; }

    /** Returns storageid on which apply the action */
	unsigned int getStorage() const throw() { return _storageid ; }

private:
    RollingAction() ; // not implemented 
    
private:

    // The object Id on which apply the action
	dbPersistent::IdType _obj;

	// The action to apply
	unsigned int _RollingActionid;

	// The storage on whith apply the action
	unsigned int _storageid ;

};


bool operator<(const RollingAction &, const RollingAction &) throw() ;
bool operator>(const RollingAction &, const RollingAction &) throw() ;
bool operator==(const RollingAction &, const RollingAction &) throw() ;
bool operator!=(const RollingAction &, const RollingAction &) throw() ;

_ACS_END_NAMESPACE

#endif /* _RollingAction_H_ */
