// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Archive $
	
*/



#ifndef _RemovePerformer_H_
#define _RemovePerformer_H_ 

#include <ActionPerformer.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* This class implements the InvPolicyPerformer class in order to:
* Given an inventory item,
* - remove the item itself: remove the physical file from ALL the storages and remove the item from the inventory
**/
class RemovePerformer : public ActionPerformer // PRQA S 2109
{		
public:
	RemovePerformer();
 	virtual ~RemovePerformer() throw() ;

	virtual void dowork( const RollingAction & actionToProcess, bool & errorOccurred, RollingReport& rollingReport, ConfigurationSingleton::StopPredicate &, dbConnection &) const;

private:
    RemovePerformer(const RemovePerformer&); // not implemented
	RemovePerformer & operator=(const RemovePerformer&); // not implemented

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RemovePerformer)

};


_ACS_END_NAMESPACE

#endif /* _RemovePerformer_H_ */
