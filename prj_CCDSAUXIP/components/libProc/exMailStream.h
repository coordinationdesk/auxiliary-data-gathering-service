// PRQA S 1050 EOF
/*

	Copyright 2009-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.4  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.3  2009/06/26 10:46:49  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/04/03 10:47:56  marpas
	subjectPrefix method added
	
	Revision 2.1  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	

*/

#ifndef _exMailStream_H_
#define _exMailStream_H_ 

#include <exStream.h>



_ACS_BEGIN_NAMESPACE(acs)


/*! \class exMailStream

	\brief base class for specialized streams with exception handling support

	This is the base class for specialized exception handlers. 
	Only exceptions will have a special treatment.
	To use this, simply inherit from this and reimplement the notifyException method
*/
class exMailStream: public exBaseStream  // PRQA S 2153
{
public:
	/*! This constructor build an handler and makes it child of excerr.
		
		\see exBaseStream
	*/
	explicit exMailStream(const std::string &from) ;
	
	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exBaseStream
	*/
	exMailStream(exostream *,const std::string &from) ;

	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exBaseStream
	*/
	exMailStream(const std::vector<exostream *> &, const std::string &from) ;

	/*! Destructor.
	*/
	virtual ~exMailStream() throw() ;

	/*! return the output stream.
		reimplemented from exostream::stream() 
	*/
	virtual std::ostream &stream() ;
	
	void addTo(const std::string &) ;
	void addTo(const std::vector<std::string> &) ;

	void subjectPrefix(const std::string &) ;
	
protected:
	/*! This method is notifies exceptions via mail.
	*/
	virtual bool notifyException(const std::exception &) throw () ;

	/*! This method is notifies filterable via mail.
	*/
	virtual void notifyFilterable(exFilterable &) ;

private:
	void sendmail(const std::string &subject) throw() ;
	
	// declared but not defined
	exMailStream() ;
	exMailStream(const exMailStream&) ;
	exMailStream &operator=(const exMailStream&) ;
	
	std::string _from ;
	std::vector<std::string> _to ;
	std::ostringstream *_tmp ;
	std::string _subjectPrefix ;
} ;

_ACS_END_NAMESPACE

#endif // _exStream_H_
