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

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.5  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.4  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.3  2012/08/03 13:04:43  damdec
	Methods for reading quality information metadata type introduced.
	
	Revision 1.2  2012/04/17 15:57:48  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.1  2012/03/20 14:12:11  damdec
	Class added.
	
        
*/
#ifndef _QualityInformation_H_
#define _QualityInformation_H_

#include <exException.h>
#include <WrappedMetadata.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Utility Class mapping the SAFE locationType 
**/

class Location
{
public:
	Location(const std::string & path, 
             int prec, 
             int fol, 
             int bef, 
             int aft, 
             const std::string & start="", 
             const std::string & stop="") : 
		_startTime(start), 
        _stopTime(stop), 
        _path(path), 
        _preceding(prec), 
        _following(fol), 
        _before(bef), 
        _after(aft) 
    {};
	Location(const std::string &start, const std::string &stop) : 
        _startTime(start), 
        _stopTime(stop), 
        _path(), 
        _preceding(), 
        _following(), 
        _before(), 
        _after() 
    {};
	
	Location() : 
        _startTime(), 
        _stopTime(), 
        _path(), 
        _preceding(), 
        _following(), 
        _before(), 
        _after() 
    {};
    
	void setLocationTime(const std::string & start, const std::string & stop)
     { _startTime = start; _stopTime = stop; }
	
    void setPath(const std::string & path, int prec = 0, int fol = 0, int bef = 0 , int aft= 0);
	
	const std::string& startTime() const throw() { return _startTime; }
	const std::string& stopTime() const throw()  { return _stopTime; }
	const std::string& path() const throw() { return _path; }
	int preceding() const throw() { return _preceding; }
	int following() const throw()  { return _following; }
	int before() const throw()  { return _before; }
	int after() const throw()  { return _after; }
private:	
	std::string _startTime;
	std::string _stopTime;
	std::string _path;
	int _preceding;
	int _following;
	int _before;
	int _after;
	
};

/**
* \brief
* Utility Class mapping the SAFE missingElementsType 
**/

class MissingElements
{
public:
	explicit MissingElements(int c = 0, 
                           int o = 0, 
                           const Location &loc = Location(), 
                           const std::string &causes="") : 
        location(loc), 
        count(c), 
        over(o), 
        cause(causes) 
    {};
    
	~MissingElements() throw() {};
	
	/** Copy Constructor */
	MissingElements(const MissingElements & r) :
		location(r.location),
		count(r.count),
		over(r.over),
		cause(r.cause) 
    { };
        
	/** Operator = */
	MissingElements &operator=(const MissingElements & r) 
	{
		if (this != &r) {
			//<Attribute copy>
			location = r.location;
			count = r.count;
			over = r.over;
			cause = r.cause;
		}
		return *this;
	};

	Location location; // PRQA S 2100 4
	int count;
	int over;
	std::string cause;
};

/**
* \brief
* Utility Class mapping the SAFE corruptedElementsType 
**/

class CorruptedElements
{
public:
	explicit CorruptedElements(int c = 0, 
                              int o = 0, 
                              const Location & loc = Location(), 
                              const std::string &typ="", 
                              const std::string &oType="", 
                              int  sev=0) : 
        location(loc), 
        count(c), 
        over(o), 
        type(typ), 
        otherType(oType), 
        severity(sev) 
    {};
    
	~CorruptedElements() throw() {};
	
	/** Copy Constructor */
	CorruptedElements(const CorruptedElements & r) : 
		location(r.location), 
		count(r.count), 
		over(r.over), 
		type(r.type), 
		otherType(r.otherType), 
		severity(r.severity) {};
	/** Operator = */
	CorruptedElements &operator=(const CorruptedElements &r)
	{
		if (this != &r) {
			//<Attribute copy>
			location = r.location; 
			count = r.count; 
			over = r.over; 
			type = r.type; 
			otherType = r.otherType; 
			severity = r.severity;
		}
		return *this;
	};

	Location location; // PRQA S 2100 6
	int count;
	int over;
	std::string type;
	std::string otherType;
	int severity;

};

/**
* \brief
* Class mapping the SAFE QualityInformationType 
**/

class QualityInformation : public WrappedMetadata { // PRQA S 2109
public:

	/** QualityInformation Exceptions */
	exDECLARE_EXCEPTION(exQualityInformationException, exException) ; // Base QualityInformation Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exQualityInformationCriticalException, exQualityInformationException) ; // Critical Exception. // PRQA S 2153


	/** Class constructor */
	explicit QualityInformation(const std::string & safeNamespace, MissingElements * miss = 0, CorruptedElements* corr=0);
	explicit QualityInformation(const rsResourceSet & rs);
	/** Destructor */
	virtual ~QualityInformation() throw() ;


	/** Copy Constructor */
	QualityInformation(const QualityInformation & );
	/** Operator = */
	QualityInformation &operator=(const QualityInformation &);
	

protected:
	virtual void writeXmlStream() ;
	virtual void readXmlStream(const rsResourceSet & rs);
private:
    QualityInformation() ; // not implemented
private:

	MissingElements * _missingElements;
	CorruptedElements * _corruptedElements;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualityInformation)

};


_ACS_END_NAMESPACE

#endif //_QualityInformation_H_

