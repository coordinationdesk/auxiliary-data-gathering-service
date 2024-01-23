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

	$Prod: A.C.S. Scheduler Log Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:27:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.5  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.4  2006/11/03 05:52:47  marpas
	new method according the new exFilterable I/F
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	
	Revision 1.1  2004/09/15 09:14:22  marfav
	TrkFilterable added to library
	
	

*/



#include <TrkFilterable.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


//
//
// TrkFilterable
//
//
TrkFilterable::TrkFilterable() :
	ScFilterable("TrkFilterable")
{
}

TrkFilterable::TrkFilterable(const string& originator) :
	ScFilterable("TrkFilterable",originator)
{
}


TrkFilterable::~TrkFilterable() throw()
{
}

void TrkFilterable::notify(const string &msg, const string& originator, exostream &o)
{
	TrkFilterable f(originator) ;
	f << msg ;
	o << f ;
}


bool TrkFilterable::defaultNotify() const
{
	return true ;
}


_ACS_END_NAMESPACE
