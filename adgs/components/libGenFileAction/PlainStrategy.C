// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Generic FileAction Library $

	$Id$

	$Author$

	$Log$
	Revision 6.1  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.0  2013/09/19 15:40:44  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.5  2013/03/28 14:30:52  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.4  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.3  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.2  2011/04/05 16:52:35  marpas
	Tree management work in progress
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
	
*/


#include <PlainStrategy.h>
#include <File.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(PlainStrategy)

PlainStrategy::PlainStrategy(dbPersistent &obj, dbConnectionPool &p) : SMPackingStrategy(obj, p)
{
}
	
PlainStrategy::~PlainStrategy() throw() {}

void PlainStrategy::pack(const std::string &input, XMLIstream &, std::string &output)
{

	output = input ; // nothing to change 
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input " << input << '\n'
			<< " Exists Input " << boolalpha << File::exists(input) 
			<< " output " << output) ;
}

void PlainStrategy::unpack(const std::string &input, XMLIstream &, std::string &output)
{
	output = input;
}

_ACS_END_NAMESPACE
