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

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/12/11 11:53:49  marfav
	Issuing major release 6.0
	
	Revision 5.3  2013/12/05 18:18:14  marfav
	S1PDGS-30219: including smqueue parameters as pure text when embedded in xml requests
	
	Revision 5.2  2013/12/04 11:02:37  marfav
	S1PDGS-30219: addedd suport for getStatus request file generation
	
	Revision 5.1  2013/12/02 16:33:06  marfav
	S1PDGS-30219: Implementation started
	
	Revision 5.0  2013/06/18 14:40:01  marpas
	adoption of libException 5.x standards
	
	Revision 2.9  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.8  2013/03/14 15:31:27  marpas
	interface is symmetric with SMInterface
	dbConnectionPool introduced
	
	Revision 2.7  2013/02/25 17:00:17  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.6  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.5  2009/01/29 17:22:33  ivafam
	Bux fixed
	
	Revision 2.4  2008/11/11 18:13:30  ivafam
	ScopeGard Removed
	
	Revision 2.3  2008/11/11 17:53:33  ivafam
	ScopeGuard implemetation changed
	
	Revision 2.2  2008/11/11 11:14:09  ivafam
	work in progess
	
	Revision 2.1  2008/11/11 11:00:54  ivafam
	Added
	

*/


#include <AsynchSMInterface.h>
#include <dbConnectionPool.h>
#include <stormgrV.h>

#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

AsynchSMInterface::AsynchSMInterface(const std::string &smtype, size_t smid, dbConnectionPool &connPool) :
	_smType(smtype),
	_storageManagerId(smid),
    _connectionPool(connPool) // PRQA S 2528
{

}
	
AsynchSMInterface::~AsynchSMInterface() throw() { }

const std::string & AsynchSMInterface::SMType() const // PRQA S 4120
{
	return _smType;
}

size_t AsynchSMInterface::SMId() const // PRQA S 4120
{
	return _storageManagerId;
}

void AsynchSMInterface::readkey(const std::string &keyfile)
{
	AsynchSMInterfaceException e("AsynchSMInterface::readkey method not implemented") ;
	ACS_THROW(e) ;
}

void AsynchSMInterface::writekey(const std::string &keyfile)
{
	AsynchSMInterfaceException e("AsynchSMInterface::readkey method not implemented") ;
	ACS_THROW(e) ;
}

AsynchSMInterface::operationStatus AsynchSMInterface::getStatus (const std::string&, const std::string&)
{
	AsynchSMInterfaceException e("AsynchSMInterface::getStatus method not implemented") ;
	ACS_THROW(e) ;
    return AsynchSMInterface::ERROR;
}

dbConnectionPool &AsynchSMInterface::getPool() { return _connectionPool ; } // PRQA S 4024, 4120 

void AsynchSMInterface::buildStatusRequestFile (const string& type, const string& ifparams, const string& extparams, const string& outfile)
{
        ofstream coutF(outfile.c_str());
        XMLOstream xst(coutF,"Request");

        {
                XMLOstream::Tag txml0(xst, "Type", type); // PRQA S 3081
        }
        {
                XMLOstream::Tag txml1(xst, "AsynchIFParams");  // PRQA S 3081
                coutF << ifparams;
        }
        {
                XMLOstream::Tag txml1(xst, "ExtParams");  // PRQA S 3081
                coutF << extparams;
        }

        xst.close();
        coutF.close();
}


_ACS_END_NAMESPACE

