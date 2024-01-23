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

	$Prod:A.C.S. Safe Wrapper Base Library  $

	$Id$

	$Author$
	$Log$
	Revision 1.4  2013/02/25 16:03:15  aleber
	Reed-Solomon parameters in QualityInformation section handled
	
	Revision 1.3  2011/09/12 10:44:57  aleber
	keyword inline placed outside class scope
	
	Revision 1.2  2011/07/16 17:05:29  aleber
	numOfPackets added in QualityContainerBase constructor
	
	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
	First implementation
	
	
*/

#ifndef _QualityContainerBase_H_
#define _QualityContainerBase_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <string>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)


class QualityLocation
{

public:

	QualityLocation() :
        _content(),
	    _after(),
	    _before(),
	    _following(),
	    _preceding(),
        _start(),
        _stop()
    {}

	~QualityLocation() throw() {}
	
    QualityLocation(const QualityLocation & r) :
        _content(r._content),
        _after(r._after),
        _before(r._before),
        _following(r._following),
        _preceding(r._preceding),
        _start(r._start),
        _stop(r._stop)
    {}

	QualityLocation &operator=(const QualityLocation & r)
    {
	    if (this != &r) {
		    _content  	   = r._content;  	 
		    _after    	   = r._after;	  	 
		    _before   	   = r._before;   	 
		    _following	   = r._following;
		    _preceding	   = r._preceding;
		    _start	       = r._start;
		    _stop 	       = r._stop;
	    }
	    return *this ;
    }
    
	void setContent(const std::string& v) { _content = v; }
	void setAfter(int v) throw() { _after = v; }
	void setBefore(int v) throw() { _before = v; }
	void setFollowing(int v) throw() { _following = v; }
	void setPreceding(int v) throw() { _preceding = v; }
	void setStart(const std::string& v) { _start = v; }
	void setStop(const std::string& v) { _stop = v; }

	const std::string& content() const ;
	int after() const throw() { return _after; }
	int before() const throw() { return _before; } 
	int following() const throw() { return _following; }
	int preceding() const throw() { return _preceding; }
	const std::string&  start() const throw() { return _start; }
	const std::string&  stop() const throw() { return _stop; }

private:

    std::string	    _content;
	int	            _after;
	int	            _before;
	int	            _following;
	int	            _preceding;
	std::string	    _start;
	std::string	    _stop;

};

template <typename T>
class QualityMisCorrElements
{
public:

	QualityMisCorrElements() : _qualityLocation(), _count(), _over(), _cause() {}
	~QualityMisCorrElements() throw() {} 
	QualityMisCorrElements(const QualityMisCorrElements & r) : 
        _qualityLocation(r._qualityLocation), 
        _count(r._count), 
        _over(r._over), 
        _cause(r._cause) 
    {}
    
	QualityMisCorrElements &operator=(const QualityMisCorrElements & r)
    {
	    if (this != &r) {
		    _qualityLocation = r._qualityLocation;	
		    _count   	   = r._count;  	
		    _over   	   = r._over;  	
		    _cause   	   = r._cause;
	    }
	    return *this ;
    }
	void setQualityLocation(const QualityLocation & v) { _qualityLocation = v ; }
	void setCount(int v) throw() { _count = v ; }
	void setOver(int v) throw() { _over = v ; } 
	void setCause(const T & v) { _cause = v ; } 

	const QualityLocation &qualityLocation() const throw() { return _qualityLocation ; } 
	int count() const throw() { return _count ; } // PRQA S 2502
	int over() const throw() { return _over ; }
	const T & cause() const throw() { return _cause ; }


private:

    QualityLocation	_qualityLocation;
	int _count;
	int _over;
	T _cause;
} ;


class QualityCorruptedCause
{

public:

	QualityCorruptedCause() : _type(), _other_type(), _severity() {}
	~QualityCorruptedCause() throw() {}
	QualityCorruptedCause(const QualityCorruptedCause& r) :
      _type(r._type), _other_type(r._other_type), _severity(r._severity) {}
	QualityCorruptedCause &operator=(const QualityCorruptedCause & r)
    {
	    if (this != &r) {
		    _type = r._type;	
		    _other_type = r._other_type;  	
		    _severity = r._severity;  	
	    }
	    return *this ;
    }

	void setType(const std::string& v) { _type = v ; } 
	void setOtherType(const std::string& v) { _other_type = v ; } 
	void setSeverity(int v) throw() { _severity = v ;} 

	const std::string& type() const throw() { return _type ; } 
	const std::string& other_type() const throw() { return _other_type ; }
	int severity() const throw() { return _severity ; }


private:

    std::string	_type;
   	std::string	_other_type;
	int _severity;

};

typedef QualityMisCorrElements<std::string> QualityMissingElements ;
typedef QualityMisCorrElements<QualityCorruptedCause> QualityCorruptedElements ;

class QualityContainerBase
{

public:

	explicit QualityContainerBase(const std::string &dataObjectId = "", 
						 long int numOfPackets = 0,
						 long int numOfRSIncorrigibleElements = 0,
						 long int numOfRSCorrectedElements = 0,
						 long int numOfRSCorrectedSymbols = 0
						 );
	~QualityContainerBase() throw() ;

	QualityContainerBase(const QualityContainerBase&);
	QualityContainerBase &operator=(const QualityContainerBase&);

	void addQualityMissingElements(int count,				   
				       const std::string &content="", 		   
				       const std::string &cause="",
				       int after=-1,
				       int before=-1,
				       int following=-1,
				       int preceding=-1,
 				       int over=0,
                	   const std::string &startTime = "",
                	   const std::string &stopTime = "");
	
	void addQualityCorruptedElements( int count,
					                  const std::string &content="",
					                  int after=-1,
					                  int before=-1,				   
					                  int following=-1,
					                  int preceding=-1,
					                  const std::string &type="",
					                  const std::string &other_type="",
					                  int severity=-1,			   
					                  int over=0,
					                  const std::string &startTime = "",
					                  const std::string &stopTime = "");
	
	void setQualityMissingElements(const std::vector< QualityMissingElements > & );
	void setQualityCorruptedElements(const std::vector< QualityCorruptedElements > & );
	void resetQualityMissingElements();
	void resetQualityCorruptedElements();
	void groupQualityElements(unsigned int, unsigned int, unsigned int, const std::string &dataUnitXpath = "");

    const std::vector<QualityMissingElements>  &qualityMissingElements() const throw() { return _missingElements; }		
    const std::vector<QualityCorruptedElements> &qualityCorruptedElements() const throw() { return _corruptedElements; }	
	const std::string &dataObjectId() const throw() { return _dataObjectId; }
	long int numOfPackets() const throw() { return _numOfPackets; }
	long int numOfRSIncorrigibleElements() const throw() { return _numOfRSIncorrigibleElements; }	
	long int numOfRSCorrectedElements() const throw() { return _numOfRSCorrectedElements; }	
	long int numOfRSCorrectedSymbols() const throw() { return _numOfRSCorrectedSymbols; }	


    std::string toStr() const ;
private:

	std::string _dataObjectId;
	long int _numOfPackets;

	std::vector<QualityMissingElements>	_missingElements;
	std::vector<QualityCorruptedElements>	_corruptedElements;

	long int _numOfRSIncorrigibleElements;
	long int _numOfRSCorrectedElements;
	long int _numOfRSCorrectedSymbols;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualityContainerBase) ;


};



std::ostream &operator << (std::ostream &, const QualityContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const QualityContainerBase &) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif //_QualityContainerBase_H_
