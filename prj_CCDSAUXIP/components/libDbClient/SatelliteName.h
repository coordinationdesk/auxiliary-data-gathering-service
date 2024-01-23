// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/06/19 18:12:07  marpas
	removing compilation warning over deprecated include files
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.7  2013/04/09 07:53:07  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.6  2013/04/05 09:36:03  marpas
	NameIdAssociator partial refactoring
	
	Revision 4.5  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.1  2010/06/23 14:11:54  marpas
	nullSatName and nullSatId methods added
	
	Revision 3.0  2009/10/05 14:47:48  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/15 17:40:16  enrcar
	*** empty log message ***
	
	Revision 1.2  2004/04/14 15:39:00  marpas
	NameIdAssociator is a template
	
	Revision 1.1  2004/04/13 17:18:08  ivafam
	Header file contain only one class
	

*/

#ifndef _SatelliteName_H_
#define _SatelliteName_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;

//
//
//! class SatelliteName a NameIdAssociator singleton for t_satellites table mapping satelliteid and satellitename
//
//

class SatelliteName: // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<SatelliteName, std::string>
{
    friend SatelliteName* ParamConstructibleSingletonMap<SatelliteName, std::string>::instance(const std::string &); // PRQA S 2107
public:
	SatelliteName &operator=(SatelliteName &&) = delete ;
	SatelliteName (SatelliteName &&) = delete ;
	SatelliteName &operator=(const SatelliteName &) = delete ;
	SatelliteName (const SatelliteName &) = delete ;
	SatelliteName () = delete ;
	virtual ~SatelliteName() = default ;

protected:
	explicit SatelliteName(const std::string &key) :
        NameIdAssociator<unsigned int>("T_Satellites", "satelliteid", "satellitename", key),
        ParamConstructibleSingletonMap<SatelliteName, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }
} ; 


_ACS_END_NAMESPACE




#endif // _SatelliteName_H_
