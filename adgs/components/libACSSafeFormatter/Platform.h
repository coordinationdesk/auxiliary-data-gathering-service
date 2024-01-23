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

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.12  2014/04/17 12:39:04  marpas
	work in progress
	
	Revision 1.11  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.10  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.9  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.8  2012/10/11 15:01:26  damdec
	Instrument ctr updated.
	
	Revision 1.7  2012/08/03 13:00:14  damdec
	Constructor for reading platform metadata and related method added.
	
	Revision 1.6  2012/07/26 12:51:51  damdec
	writeXmlStream method made virtual
	
	Revision 1.5  2012/06/29 11:04:27  damdec
	Methods returning private attribute added. New constructors added.
	
	Revision 1.4  2012/04/17 15:56:52  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.3  2012/03/21 13:31:26  damdec
	Instrument class attribute updated
	
	Revision 1.2  2012/03/15 15:47:09  damdec
	Implementation finalized
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/
#ifndef _Platform_H_
#define _Platform_H_

#include <exException.h>
#include <WrappedMetadata.h>

_ACS_BEGIN_NAMESPACE(acs)
class rsResourceSet ;


class Instrument 
{

public: 
	std::string familyName; // PRQA S 2100 5
	std::string abbreviation;
	std::string number;
	double sideLookingAngle;
	double fieldOfView;

	Instrument() : 
        familyName(), 
        abbreviation(), 
        number(), 
        sideLookingAngle(), 
        fieldOfView() {}
	
	Instrument(const std::string & famName, 
               const std::string & abbr, 
               const std::string & num="", 
               double sideLA = 0, double FOV = 0 ) : 
	    familyName(famName), 
        abbreviation(abbr), 
        number(num), 
        sideLookingAngle(sideLA), 
        fieldOfView(FOV) 
    { };
	
	virtual ~Instrument() throw() {}; // PRQA S 2131
	
	/** Copy Constructor */
	Instrument(const Instrument & r): 
        familyName(r.familyName), 
        abbreviation(r.abbreviation), 
        number(r.number),
	    sideLookingAngle(r.sideLookingAngle), 
        fieldOfView(r.fieldOfView)
    {};
	/** Operator = */
	Instrument &operator=(const Instrument & r) 
	{
		if (this != &r) {
			//<Attribute copy>
			familyName = r.familyName;
			abbreviation = r.abbreviation;
			number = r.number;
			sideLookingAngle = r.sideLookingAngle;
			fieldOfView = r.fieldOfView;
		}
		return *this;
	};
} ;

class TimeRef 
{
public:	
	std::string utc; // PRQA S 2100 3
	long int clock ;
	long int clockStep ;
	
	TimeRef() : 
        utc(""), 
        clock(0), 
        clockStep(0) { }
        
	explicit TimeRef(const std::string &utcTime, long cl=0, long clStep=0) : 
        utc(utcTime), 
        clock(cl), 
        clockStep(clStep) 
	{ 
	};
	~TimeRef() throw() {};

	/** Copy Constructor */
	TimeRef(const TimeRef & r): utc(r.utc), clock(r.clock), clockStep(r.clockStep) {};
	/** Operator = */
	TimeRef &operator=(const TimeRef & r) 
	{
		if (this != &r) {
			//<Attribute copy>
			utc = r.utc;
			clock = r.clock;
			clockStep = r.clockStep;
		}
		return *this;
	};
private:

} ;

/**
* \brief
* Class mapping the SAFE PlatformType  
**/

class Platform : public WrappedMetadata { // PRQA S 2109
public:

	/** Platform Exceptions */
	exDECLARE_EXCEPTION(exPlatformException, exException) ; // Base Platform Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exPlatformCriticalException, exPlatformException) ; // Critical Exception. // PRQA S 2153


	/** Class constructors */
	Platform(const std::string& safeNamespace, 
             const std::string& family_name, 
             const std::string& nssdc_id, 
             const std::string& number,
			 const Instrument& instrument=Instrument(),
			 const std::vector<TimeRef>& timeRef = std::vector<TimeRef>(),
			 const std::string& safePrefix="sentinel-safe:");
	Platform() ;
	explicit Platform(const rsResourceSet & rs);
	
	/** Destructor */
	virtual ~Platform() throw() ;
	/** Copy Constructor */
	Platform(const Platform & );
	/** Operator = */
	Platform &operator=(const Platform &);
	
	const std::string &getFamilyName() const throw() {return _familyName;};
	const std::string &getNssdcIdentifier() const throw() {return _nssdcId;};
	const std::string &getNumber() const throw() {return _number;};
	const Instrument & getInstrument() const throw() {return _instrument;};
	const std::vector <TimeRef> & getTimeRef() const throw() {return _timeRef;};
	
protected:
	virtual void writeXmlStream() ;
	virtual void readXmlStream(const rsResourceSet & rs);
	
private:  
	std::string _familyName;
	std::string _nssdcId;
	std::string _number;
	Instrument  _instrument;
	std::vector <TimeRef> _timeRef;
	
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Platform)

};


_ACS_END_NAMESPACE

#endif //_Platform_H_

