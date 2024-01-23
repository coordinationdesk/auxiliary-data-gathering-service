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

	$Prod: A.C.S. SoftwarRecord Library $

	$Id$

	$Author$
	$Log$
	Revision 1.12  2014/06/25 13:11:40  marpas
	interface robustness improved
	
	Revision 1.11  2014/02/12 14:26:50  enrcar
	EnrCar:: Added member _datastrip in the ProcessingLog
	
	Revision 1.10  2014/02/11 14:24:55  enrcar
	EnrCar:: optional argument instance added
	
	Revision 1.9  2014/01/28 17:15:20  enrcar
	ENRCAR:: Added some non mandatory attributes to containers. This doesn't cause regressions.
	
	Revision 1.8  2013/12/03 17:51:11  marpas
	date time in variables with specific types
	
	Revision 1.7  2013/11/14 14:44:40  marpas
	coding best practices applied,
	interface robustness and performances improved
	
	Revision 1.6  2013/10/18 08:08:32  masdal
	RicFer:: code re-aligned
	
	Revision 1.5  2013/10/04 09:03:26  marpas
	some warnigns removed
	
	Revision 1.4  2013/01/21 16:32:07  aleber
	SPPDU version added as last argument of function addProcessing of ProcessingContainer class
	
	Revision 1.3  2011/09/08 13:51:06  aleber
	creation of ProcessingLog in the Manifest tested
	
	Revision 1.2  2011/09/08 07:36:28  aleber
	ProcessingLog and ResourceLog implemented
	
	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
	First implementation
	
	
*/

#ifndef _ProcessingContainerBase_H_
#define _ProcessingContainerBase_H_

#include <Filterables.h>       
#include <DateTime.h>       
#include <string>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class SoftwareLog ;
class HardwareLog ;
class FacilityLog ;
class ResourceLog ;
class ProcessingLog ;

class SoftwareLog
{

public:

	explicit SoftwareLog(const std::string &name = "" ,
		                const std::string &version = "");

	virtual ~SoftwareLog() throw();

	void setName(const std::string& name);
	void setVersion(const std::string& version);

	std::string toStr() const ;

	const std::string& name() const throw() { return _name; }
	const std::string& version() const throw() { return _version; }
	const std::vector<SoftwareLog*>& software() const throw() { return _software; }	
	
	SoftwareLog(const SoftwareLog&);
	SoftwareLog &operator=(const SoftwareLog&);
private:

    std::string 				_name;	
    std::string 				_version;
	std::vector<SoftwareLog*>	_software;  	// vector of Software elements, sons of Facility

};
	
std::ostream &operator << (std::ostream &, const SoftwareLog &) ;
acs::exostream &operator << (acs::exostream &, const SoftwareLog &) ; // PRQA S 2072
	
class HardwareLog
{

public:

	HardwareLog();

	virtual ~HardwareLog() throw();

	void setName(const std::pair<std::string, std::string>& name);

	std::string toStr() const ;

	const std::pair<std::string, std::string>& name() const throw() { return _name; };
	
	HardwareLog(const HardwareLog&);
	HardwareLog &operator=(const HardwareLog&);

private:

    std::pair<std::string, std::string>	_name;	
};

std::ostream &operator << (std::ostream &, const HardwareLog &) ;
acs::exostream &operator << (acs::exostream &, const HardwareLog &) ; // PRQA S 2072
	
class FacilityLog
{

public:

	explicit FacilityLog(const std::string & name = "",	// Name of the organization authority. MANDATORY
		    	const std::string & organization= "",
		    	const std::string & site= "",
		    	const std::string & country= "");

	virtual ~FacilityLog() throw();

	void setId(const std::string& id);
	void setName(const std::string& name);
	void setOrganization(const std::string& organization);
	void setSite(const std::string& site);
	void setCountry(const std::string& country);
	void addSoftware(SoftwareLog* );
	void addHardware(HardwareLog* );
	
	std::string getId() {return _id;};

	std::string toStr() const ;

	const std::string& id() const throw() { return _id; }
	const std::string& name() const throw() { return _name; }
	const std::string& organization() const throw() { return _organization; }	
	const std::string& site() const throw() { return _site; }
	const std::string& country() const throw() { return _country; } 
	const std::vector<SoftwareLog*>& software() const throw() { return _software; }	
	const std::vector<HardwareLog*>& hardware() const throw() { return _hardware; }	
	const std::vector<FacilityLog*>& facility() const throw() { return _facility; }
	
	FacilityLog(const FacilityLog&);
	FacilityLog &operator=(const FacilityLog&);

private:

    std::string _id;
    std::string _name;
	std::string _organization;
	std::string _site;
    std::string _country;
	std::vector<SoftwareLog*> _software;  	// vector of Software elements, sons of Facility
	std::vector<HardwareLog*> _hardware;  	// vector of Hardware elements, sons of Facility
	std::vector<FacilityLog*> _facility;  	// vector of Facility elements, sons of Facility

    ACS_CLASS_DECLARE_DEBUG_LEVEL(FacilityLog)

};

std::ostream &operator << (std::ostream &, const FacilityLog &) ;
acs::exostream &operator << (acs::exostream &, const FacilityLog &) ; // PRQA S 2072
	
class ResourceLog																			
{																					

public: 																				

	explicit ResourceLog(const std::string & name = "",																
		        const std::string & role = "",														  
		        const std::string & href = "",
				const std::string & version = "");													  
	virtual ~ResourceLog() throw() ; 																		

	void setId(const std::string&);
	void setName(const std::string& );																
	void setRole(const std::string& );																
	void setHref(const std::string& );																
	void setVersion(const std::string& );																
	void addProcessing(ProcessingLog* );												

	std::string toStr() const ;

    const std::string& id() const throw() { return _id; }												  
	const std::string& name() const throw() { return _name; }														 
	const std::string& role() const throw() { return _role; }													 
	const std::string& href() const throw() { return _href; }														 
	const std::string& version() const throw() { return _version; }															 
	const std::vector<ProcessingLog*>& processing() const throw() { return _processing; }
																					
	ResourceLog(const ResourceLog&);																
	ResourceLog &operator=(const ResourceLog&);															

private:																				

    std::string _id;    // name of the resource
	std::string _name;  // The name of the logged software. MANDATORY                                           
	std::string _role;  // The role of the software with regard to the holding processing. MANDATORY                            
	std::string _href;                                                                  
	std::string _version;                                                                   
	std::vector<ProcessingLog*> _processing;  	// vector of ProcessingLog elements, sons of ResourceLog						

    ACS_CLASS_DECLARE_DEBUG_LEVEL(ResourceLog)

};	
																				
std::ostream &operator << (std::ostream &, const ResourceLog &) ;
acs::exostream &operator << (acs::exostream &, const ResourceLog &) ; // PRQA S 2072
	

class ProcessingLog
{

public:

	explicit ProcessingLog(const std::string & name,
		    const DateTime::MJD2000 & startTimeStr,
		    const DateTime::MJD2000 & stopTimeStr,
			const std::string & sppduVersion="",
			const std::string & outputLevel="",
			const off_t& datastrip=0,
			const off_t& instance=0);

	explicit ProcessingLog(
		const std::string & name=""
	);


	~ProcessingLog() throw();

	void setId(const std::string&);
	void setName(const std::string&);
	void setStartTime(const DateTime::MJD2000&);
	void setStopTime(const DateTime::MJD2000&);
	void setSppduVersion(const std::string&);
	void setOutputLevel(const std::string&);
	void setXmlns(const std::string&);
	void setDatastrip(const off_t&);
	void setInstance(const off_t&);
	void addSoftware(SoftwareLog*);
	void addResource(ResourceLog*);
	void addFacility(FacilityLog*);
	void addProcessing(ProcessingLog*) ;

	std::string getId() {return _id;};

	std::string toStr() const ;
	
    const std::string& id() const throw() { return _id; }												  
    const std::string& name() const throw() { return _name; }												  
    const DateTime::MJD2000& getStartTime() const throw() { return _startTime; } 							   
    const DateTime::MJD2000& getStopTime() const throw() { return _stopTime; } 									
	const bool isStartTimeSet() const throw() { return _startTimeSet; } 	
	const bool isStopTimeSet() const throw() { return _stopTimeSet; } 	
	const off_t datastrip() const throw() { return _datastrip; } 	
	const off_t instance() const throw() { return _instance; } 	
	const std::string& sppduVersion() const throw() { return _sppduVersion; }
	const std::string& outputLevel() const throw() { return _outputLevel; }									
	const std::string& xmlns() const throw() { return _xmlns; }									
	const std::vector<SoftwareLog*>& software() const throw() { return _software; }
	const std::vector<HardwareLog*>& hardware() const throw() { return _hardware; }
	const std::vector<ResourceLog*>& resource() const throw() { return _resource; }
	const std::vector<FacilityLog*>& facility() const throw() { return _facility; }
	const std::vector<ProcessingLog*>& processing() const throw() { return _processing; }	

	ProcessingLog(const ProcessingLog&);
	ProcessingLog &operator=(const ProcessingLog&);

	const FacilityLog* facility(const std::string& id) const ;

private:

    std::string _id;		// name of the processing: MANDATORY
    std::string _name;		// name of the processing: MANDATORY
	DateTime::MJD2000 _startTime;
	DateTime::MJD2000 _stopTime;
	bool _startTimeSet;
	bool _stopTimeSet;
	std::string _sppduVersion;
	std::string _outputLevel;
	std::string _xmlns;
	off_t _datastrip;
	off_t _instance;
	std::vector<SoftwareLog*> _software;
	std::vector<HardwareLog*> _hardware;
	std::vector<ResourceLog*> _resource;  
	std::vector<FacilityLog*> _facility;  
	std::vector<ProcessingLog*> _processing;  
	std::map<std::string, FacilityLog> _facilityMap;

};

																				
std::ostream &operator << (std::ostream &, const ProcessingLog &) ;
acs::exostream &operator << (acs::exostream &, const ProcessingLog &) ; // PRQA S 2072
	

class ProcessingContainerBase
{
public:

	/**
	 * \brief  Class constructor
	 *    
	 */

	ProcessingContainerBase();

	/**
	 * \brief  Class destructor 
	 *    
	 */
    ~ProcessingContainerBase() throw() ;
	
	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	ProcessingContainerBase(const ProcessingContainerBase & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	ProcessingContainerBase &operator=(const ProcessingContainerBase &) ;
	
	void addProcessing(ProcessingLog* processing);												
	void addProcessing(const std::string& id, 
					   const std::string& name, 
					   const DateTime::MJD2000& startTime, 
					   const DateTime::MJD2000& stopTime, 
					   const std::string& sppduVersion = "");  

	std::string toStr() const ;

	const std::vector<ProcessingLog*>& processing() const throw() { return _processing; }
	const ProcessingLog* processing(const std::string& id) const ;

private:
		
	std::vector<ProcessingLog*>	_processing;  
	std::map<std::string, ProcessingLog> _processingMap;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProcessingContainerBase)

};


std::ostream &operator << (std::ostream &, const ProcessingContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const ProcessingContainerBase &) ; // PRQA S 2072


_ACS_END_NAMESPACE


#endif //
