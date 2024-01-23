// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.13  2014/04/24 16:43:28  tergem
	Porting to head

	Revision 1.12  2014/04/17 12:39:05  marpas
	work in progress

	Revision 1.11  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)

	Revision 1.10  2013/10/29 19:08:57  nicvac
	compilation warning processed.

	Revision 1.9  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.

	Revision 1.8  2012/12/21 14:47:31  damdec
	Operator= and copy constructor for Processing class added

	Revision 1.7  2012/11/28 10:29:17  damdec
	Handling of the attribute versionin Resource added

	Revision 1.6  2012/08/03 13:01:04  damdec
	Method for reading processing metadata type fixed.

	Revision 1.5  2012/07/26 12:54:10  damdec
	Methods for reading processing metadata added. writeXmlStream method made virtual.

	Revision 1.4  2012/04/19 13:53:49  damdec
	Classes reviewed: methods parameters are now passed as references; classes attributes are no longer pointers.

	Revision 1.3  2012/04/17 15:56:51  damdec
	Safe namespace passed as parameter to the constructor.

	Revision 1.2  2012/03/27 08:34:12  damdec
	Classes renamed. Default constructors removed.

	Revision 1.1  2012/03/19 18:10:07  damdec
	Class added.


*/
#ifndef _Processing_H_
#define _Processing_H_

#include <exException.h>
#include <WrappedMetadata.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class Resource ;
/**
* \brief
* Utility classes supporting the definition of the class mapping the SAFE ProcessingType
**/

class Software
{

public:

	explicit Software(const std::string &name, const std::string &version = "");
	Software();
	virtual ~Software() throw() ;

	void dump() const ;
	std::string toStr() const ;

	const std::string& name() const throw() { return _name; }
	const std::string& version() const throw() { return _version; }
	const std::vector<Software> &software() const throw() { return _software; }

	Software(const Software&);
	Software &operator=(const Software&);
	// bool operator==(const Software&)	const;

private:

    std::string _name;		// mandatory
    std::string _version;
	std::vector<Software> _software;  	// vector of Software elements, sons of Facility

};


class Hardware
{

public:

    explicit Hardware(const std::string &name);
    Hardware();
    virtual ~Hardware() throw() ;

    void dump() const ;

    const std::string& name() const throw() { return _name ; }
    const std::vector<Hardware> &hardware() const throw() { return _hardware ; }

    Hardware(const Hardware&);
    Hardware &operator=(const Hardware&);

private:

    std::string                     _name;          // mandatory
    std::vector<Hardware>           _hardware;      // vector of Hardware elements, sons of Facility
};


bool operator==(const Hardware&, const Hardware&) ;

class Facility
{

public:

	explicit Facility(const std::string &name,	// Name of the organisation authority. MANDATORY
			         const std::string &organization= "",
                     const std::string &site= "",
                     const std::string &country= "") ;
	Facility();
	virtual ~Facility() throw() ;

	void addSoftware(const Software &);
	void addHardware(const Hardware &);

	void dump() const ;
	const std::string& name() const throw() { return _name ; }
	const std::string& organization() const throw() { return _organization ; }
	const std::string& site() const throw() { return _site ; }
	const std::string& country() const throw() { return _country ; }
	const std::vector<Software> &software() const throw() { return _software ; }
	const std::vector<Hardware> &hardware() const throw() { return _hardware ; }
	const std::vector<Facility> &facility() const throw() { return _facility ; }


	Facility(const Facility&);
	Facility &operator=(const Facility&);
	//bool operator==(const Facility&)	const;

private:

    std::string _name;
	std::string _organization;
	std::string _site;
    std::string _country;
	std::vector<Software> _software ;      // vector of Software elements, sons of Facility
	std::vector<Hardware> _hardware ;
	std::vector<Facility> _facility ;      // vector of Facility elements, sons of Facility

    ACS_CLASS_DECLARE_DEBUG_LEVEL(Facility)

};


class ProcessLog
{

public:

	explicit ProcessLog(const std::string &name = "",
		    const std::string & startTime = "",
		    const std::string & stopTime = "");

	ProcessLog(const ProcessLog&);
	ProcessLog &operator=(const ProcessLog&);
	virtual ~ProcessLog() throw() ;

	void setName(const std::string &v) { _name = v ; }
	void setStartTime(const std::string &v) { _startTime = v ; }
	void setStopTime(const std::string &v) { _stopTime = v ; }
	void addSoftware(const Software &v) { _software.push_back(v) ; }
	void setSoftware(const std::vector<Software> &v) { _software = v ; }
	void clearSoftware() { _software.clear() ; }
	void addHardware(const Hardware &v) { _hardware.push_back(v) ; }
	void setHardware(const std::vector<Hardware> &v) { _hardware = v ; }
	void clearHardware() { _hardware.clear() ; }
	void addResource(const Resource & );
	void setResource(const std::vector<Resource> &) ;
	void clearResource() { _resource.clear() ; }
	void addFacility(const Facility &v) { _facility.push_back(v) ; }
	void setFacility(const std::vector<Facility> &v) { _facility = v ; }

	const std::string& name() const throw() { return _name; }
	const std::string& startTime() const throw() { return _startTime; }
	const std::string& stopTime() const throw() { return _stopTime; }
	const std::vector<Software> &software() const throw() { return _software; }
	const std::vector<Hardware> &hardware() const throw() { return _hardware; }
	const std::vector<Resource> &resource() const throw() { return _resource; }
	const std::vector<Facility> &facility() const throw() { return _facility; }
	const std::vector<ProcessLog> &processing() const throw() { return _processing; }

private:

    std::string 			_name;
	std::string 			_startTime;
	std::string 			_stopTime;
	std::vector<Software>		_software;
	std::vector<Hardware>       _hardware;
	std::vector<Resource>		_resource;
	std::vector<Facility>		_facility;
	std::vector<ProcessLog>		_processing;
};

class Resource
{

public:

	Resource(const std::string & name, const std::string & role, const std::string & href = "", const std::string & version = "");
	Resource();
	~Resource() throw() ;
	void addProcessing(const ProcessLog &processing);

	void dump() const ;
	std::string toStr() const ;

	Resource(const Resource&);
	Resource &operator=(const Resource&);

	const std::string& name() const throw() { return _name; }
	const std::string& role() const throw() { return _role; }
	const std::string& href() const throw() { return _href; }
	const std::string& version() const throw() { return _version; }
	const std::vector<ProcessLog> &processing() const throw() { return _processing; }

private:

	std::string 		_name;	// The name of the logged software. MANDATORY
	std::string 		_role;	// The role of the software with regard to the holding processing. MANDATORY
	std::string 		_href;
	std::string 		_version;	// Sentinel-SAFE
	std::vector<ProcessLog>	_processing;  	// vector of ProcessLog elements, sons of Resource

    ACS_CLASS_DECLARE_DEBUG_LEVEL(Resource)

};

bool operator==(const Resource &, const Resource &) ;

/**
* \brief
* Class mapping the SAFE ProcessingType
**/

class Processing : public WrappedMetadata { // PRQA S 2109
public:

	/** Processing Exceptions */
	exDECLARE_EXCEPTION(exProcessingException, exException) ; // Base Processing Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exProcessingCriticalException, exProcessingException) ; // Critical Exception. // PRQA S 2153


	Processing(const std::string & safeNamespace, const ProcessLog&, const std::string& safePrefix="sentinel-safe:");
	Processing() ;

	explicit Processing(const rsResourceSet & rs);

	Processing(const Processing&);
	Processing &operator=(const Processing&);
	virtual ~Processing() throw() ;
	const ProcessLog &processing() const throw() { return _processingLog; }

protected:

	ProcessLog _processingLog;

	virtual void writeXmlStream() ;
	virtual void readXmlStream(const rsResourceSet & rs);

	void addFacility(const Facility &v) { _processingLog.addFacility(v) ; }

	virtual void writeSoftware(const Software & sw, std::ostringstream& xmlInfo, const std::string& indent);
	virtual void writeFacility(const Facility & fac, std::ostringstream& xmlInfo, const std::string& indent);
	virtual void writeResource(const Resource & resource, std::ostringstream& xmlInfo, const std::string& indent);
	virtual void writeHardware(const Hardware & hd, std::ostringstream& xmlInfo, const std::string& indent);
	virtual void writeProcessLog(const ProcessLog& processing, std::ostringstream& xmlInfo, const std::string& indent);

	virtual void readProcessLog(const rsResourceSet & rs, const std::string & rootKey="xmlData.processing");
	virtual void readFacility(const rsResourceSet & rs, ProcessLog & processingLog, const std::string & rootKey="xmlData.processing.facility");
	virtual void readSoftware(const rsResourceSet & rs, ProcessLog & processingLog, const std::string & rootKey="xmlData.processing.software");
	virtual void readResources(const rsResourceSet & rs, ProcessLog & processingLog, const std::string & rootKey="xmlData.processing.resource");
	virtual void readHardware(const rsResourceSet & rs, ProcessLog & processingLog, const std::string & rootKey="xmlData.processing.hardware");
	virtual void readSingleFacility(const std::string & objectKey, const rsResourceSet & rs, ProcessLog & processingLog);

private:

	void readSingleSoftware(const std::string & objectKey, const rsResourceSet & rs, ProcessLog & processingLog);
	void readSingleHardware(const std::string & objectKey, const rsResourceSet & rs, ProcessLog & processingLog);
	void readSingleResource(const std::string & rootKey, const rsResourceSet & rs, ProcessLog & processingLog);
	void readSingleProcessLog(const std::string & rootKey, const rsResourceSet & rs, ProcessLog & processingLog);
private:
//	ProcessLog _processingLog;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Processing)

};

_ACS_END_NAMESPACE

#endif //_Processing_H_
