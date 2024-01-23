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
	Revision 5.3  2013/11/14 09:16:22  marpas
	implementing abstract clear method()
	
	Revision 5.2  2013/09/27 15:27:30  marpas
	streams have name
	
	Revision 5.1  2013/09/26 19:34:36  marpas
	adopting libException Rev_5_16 interfaces
	test improved
	still doubles messages
	
	Revision 5.0  2013/07/07 18:27:50  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.5  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.4  2008/02/21 16:39:06  clanas
	alligned to new libException interface
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/27 13:30:05  marfav
	insertionLock method added
	
	Revision 1.1  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	


*/


#ifndef _ScFilterable_H_
#define _ScFilterable_H_

#include <acs_c++config.hpp>
#include <Filterables.h>
#include <exStream.h>


_ACS_BEGIN_NAMESPACE(acs)

class ScFilterable : // PRQA S 2109, 2153
	public exFilterable,
	public exostream
{

public:
	// Default CTOR
	explicit ScFilterable(const std::string &name) ;
	
	// CTOR setting the message originator
	explicit ScFilterable(const std::string &name, const std::string& originator);
	
	// DTOR
	virtual ~ScFilterable() throw() ;
	
	// inherited from exFilterable, returns the msg string and locks the internal stream
	virtual std::string text() ;
	
    /*- clears any previous content*/
    virtual void clear() ;

	// returns the unformatted msg and does not lock the internal stream
	virtual std::string message() const ;

	// lock the internal stream
	virtual void insertionLock() ;
	
	// inherited from ostream
	virtual std::ostream &stream() ;
	
	// set/get the originator of the message (TaskName)
	void setOriginatorTaskName (const std::string&);
	std::string getOriginatorTaskName () const ;
	virtual std::string detail() const { return "" ; } // PRQA S 2131 2
	virtual void detail(const std::string &) {} ;

	// set/get the order signature to redirect the messages in the correct stream
	void setOrderSignature (const std::string&) ;
	std::string getOrderSignature () const;
	

private:
	ScFilterable() ; // not implemented
	ScFilterable(const ScFilterable &) ; // not implemented
	ScFilterable &operator=(const ScFilterable &) ; // not implemented

private:
	std::ostringstream *_os ;
	bool _osLocked ;
	std::string _originatorTaskName ;
	std::string _orderSignature;
};


_ACS_END_NAMESPACE



#endif //_ScFilterable_H_

