// PRQA S 1050 EOF
/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
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
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.0  2013/09/30 12:30:50  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:18  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/05/13 15:12:12  marpas
	rules fixing and comments makeup
	
	Revision 2.1  2006/04/04 15:03:41  paoscu
	GoceFixedHeaderFactory added.
	EarthExplorerFixedHeaderWriter replaces CryosatFixedHeaderWriter
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2004/05/03 14:53:59  paoscu
	New haeader writers.
	

*/

#ifndef _EarthExplorerFixedHeaderWriter_H_
#define _EarthExplorerFixedHeaderWriter_H_


#include <dbFixedHeaderWriter.h>

#include <XMLOstream.h>
#include <exException.h>

namespace acs {



class EarthExplorerFixedHeaderWriter : public dbFixedHeaderWriter // PRQA S 2109, 2153
{
public:
	/**
	 * EarthExplorerFixedHeaderWriter stores a reference to the dbInventoryObject
	 * PAY ATTENTION in multy thread applications!
	 */
	EarthExplorerFixedHeaderWriter(std::string const & satelliteId, dbInventoryObject const &, std::string const &system, XMLOstream &);
	virtual ~EarthExplorerFixedHeaderWriter() throw() ;

	virtual void start() ;
	virtual void run(bool addDataBlock) ;
	virtual void end() ;

 private:
	//Not implemented
	EarthExplorerFixedHeaderWriter();
	EarthExplorerFixedHeaderWriter(const EarthExplorerFixedHeaderWriter&);
	EarthExplorerFixedHeaderWriter &operator=(const EarthExplorerFixedHeaderWriter&);


private:
	std::string _satelliteId ;
	XMLOstream::Tag *_eartExplorerFileTag ;
	XMLOstream::Tag *_dataBlockTag ;
		
};

} // namespace


#endif // _EarthExplorerFixedHeaderWriter_H_
