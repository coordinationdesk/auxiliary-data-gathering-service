// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.16  2014/07/04 13:32:09  tergem
	Fixing from S3PDGS-1890 comments

	Revision 1.15  2014/06/26 15:09:21  tergem
	alignment to format 3.4

	Revision 1.14  2014/04/24 16:43:28  tergem
	Porting to head

	Revision 1.13  2014/04/17 12:39:04  marpas
	work in progress

	Revision 1.12  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)

	Revision 1.11  2013/10/29 19:08:57  nicvac
	compilation warning processed.

	Revision 1.10  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.

	Revision 1.9  2012/12/21 14:47:31  damdec
	Operator= and copy constructor for Processing class added

	Revision 1.8  2012/11/28 10:29:16  damdec
	Handling of the attribute versionin Resource added

	Revision 1.7  2012/08/03 13:01:04  damdec
	Method for reading processing metadata type fixed.

	Revision 1.6  2012/07/26 12:54:10  damdec
	Methods for reading processing metadata added. writeXmlStream method made virtual.

	Revision 1.5  2012/05/03 10:50:07  damdec
	_processing, attribute of ProcessLog, initialized in the constructor

	Revision 1.4  2012/04/19 13:53:49  damdec
	Classes reviewed: methods parameters are now passed as references; classes attributes are no longer pointers.

	Revision 1.3  2012/04/17 15:56:51  damdec
	Safe namespace passed as parameter to the constructor.

	Revision 1.2  2012/03/27 08:34:12  damdec
	Classes renamed. Default constructors removed.

	Revision 1.1  2012/03/19 18:10:07  damdec
	Class added.


*/
#include <Processing.h>

#include <Filterables.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Processing)


Facility::Facility(const string &name,
                   const string & organization,
                   const string & site,
                   const string & country) :
    _name(name),
    _organization(organization),
    _site(site),
    _country(country),
    _software(),
    _hardware(),
    _facility()
{
}

Facility::Facility() :
    _name(),
    _organization(),
    _site(),
    _country(),
    _software(),
    _hardware(),
    _facility()
{
}

Facility::Facility(const Facility& f):
   	_name(f._name),
 	_organization(f._organization),
 	_site(f._site),
 	_country(f._country),
 	_software(f._software),
    _hardware(f._hardware),
	_facility(f._facility)
{
}

Facility::~Facility() throw()
{
}

Facility &Facility::operator=(const Facility& f)
{
 	if (this != &f) {
        _name = f._name ;
 		_organization = f._organization ;
 		_site = f._site ;
 		_country = f._country ;
		_software = f._software;
        _hardware = f._hardware ;
        _facility = f._facility ;
 	}
 	return *this ;
}

void Facility::addSoftware(const Software& softwareLog)
{
	_software.push_back(softwareLog);
}

void Facility::addHardware(const Hardware& hardwareLog)
{
        _hardware.push_back(hardwareLog);
}

void Facility::dump() const
{
	ACS_LOG_DEBUG("Facility content: ") ;
	ACS_LOG_DEBUG("\tName: " << name()) ;
    ACS_LOG_DEBUG("\tOrganization: " << organization()) ;
    ACS_LOG_DEBUG("\tSite: " << site()) ;
    ACS_LOG_DEBUG("\tCountry: " << country()) ;
}

Hardware::Hardware(const string &name) :
    _name(name),
    _hardware()
{
}

Hardware::Hardware() :
    _name(),
    _hardware()
{
}

Hardware::Hardware(const Hardware& s):
        _name(s._name),
        _hardware(s._hardware)
{
}

Hardware::~Hardware() throw()
{
}

Hardware &Hardware::operator=(const Hardware& s)
{
    if (this != &s) {
            _name = s._name ;
            _hardware = s._hardware;
    }
    return *this ;
}


void Hardware::dump() const
{
	ACS_LOG_DEBUG("Hardware content: ") ;

    ACS_LOG_DEBUG("\tName: " << name()) ;
    ACS_LOG_DEBUG("\tHardware size = " << _hardware.size()) ;
}


Software::Software(const string & name, const string & version) :
    _name(name),
    _version(version),
    _software()
{
}

Software::Software() :
    _name(),
    _version(),
    _software()
{
}

Software::Software(const Software& s):
    _name(s._name),
 	_version(s._version),
 	_software(s._software)
{
}

Software::~Software() throw()
{
}

Software &Software::operator=(const Software& s)
{
 	if (this != &s) {
        _name = s._name ;
 		_version = s._version ;
 		_software = s._software;
	}
 	return *this ;
}


void Software::dump() const
{

	ACS_LOG_DEBUG("Software content: ") ;
	ACS_LOG_DEBUG("\tName: " << name()) ;
	ACS_LOG_DEBUG("\tVersion: " << version()) ;
	ACS_LOG_DEBUG("\tSoftware size = " << _software.size()) ;
}

string Software::toStr() const {
  ostringstream os ;
  os 	<< "\t\tSoftware" << endl
		  << "\t\t Name         : \t" << name() << endl
		  << "\t\t Version      : \t" << version()  ;
  return os.str() ; // PRQA S 4028
}

Resource::Resource(const string & name,
                   const string & role,
                   const string & href,
                   const string & version) :
    _name(name),
    _role(role),
    _href(href),
    _version(version),
    _processing()
{}

Resource::Resource() :
    _name(),
    _role(),
    _href(),
    _version(),
    _processing()
{}

Resource::Resource(const Resource& r):
    _name(r._name),
    _role(r._role),
    _href(r._href),
    _version(r._version),
    _processing(r._processing)
{
}

Resource::~Resource() throw()
{
}

Resource &Resource::operator=(const Resource& r)
{
 	if (this != &r) {
        _name = r._name ;
        _role = r._role ;
        _href = r._href ;
        _version = r._version;
        _processing = r._processing;
 	}
 	return *this ;
}

bool operator==(const Resource& a, const Resource& b )
{
#warning MP prcessing should be compared ?
	return ( (a.name() == b.name()) && (a.role() == b.role()) &&
             (a.href() == b.href()) && (a.version() == b.version()) );

}


void Resource::addProcessing(const ProcessLog & processing)
{
	_processing.push_back( processing );
}


void Resource::dump() const
{
    ACS_LOG_DEBUG("Resource content: ") ;
	ACS_LOG_DEBUG("\tName: " << name()) ;
	ACS_LOG_DEBUG("\tRole: " << role()) ;
    if (!version().empty()) {
        ACS_LOG_DEBUG("\tVersion: " << version()) ;
    }
	ACS_LOG_DEBUG("\tHref: " << href()) ;
}

string Resource::toStr() const {
  ostringstream os ;
  os 	<< "\t\tResource" << endl
		  << "\t\t Name         : \t" << name() << endl
		  << "\t\t Role         : \t" << role()  ;
  if (!version().empty()) {
    os << endl
      << "\t\tVersion       : \t" << version();
  }
  return os.str() ; // PRQA S 4028
}


ProcessLog::ProcessLog(
	                    const string &name ,
	                    const string &startTime, // start processing time provided as ASCII string of the type yyyy-mm-ddThh:nn:ss.uuuuuu
	                    const string &stopTime)  :
	_name(name),
    _startTime(startTime),
    _stopTime(stopTime),
    _software(),
    _hardware(),
    _resource(),
    _facility(),
    _processing()
{
}

ProcessLog::~ProcessLog() throw()
{
}

// copy constructor
ProcessLog::ProcessLog(const ProcessLog& p):
    _name(p._name),
 	_startTime(p._startTime),
 	_stopTime(p._stopTime),
 	_software(p._software),
    _hardware(p._hardware),
 	_resource(p._resource),
	_facility(p._facility),
	_processing(p._processing)
{
}


// operator =
ProcessLog &ProcessLog::operator=(const ProcessLog& p)
{
 	if (this != &p) {
	    _name = p._name ;
 		_startTime = p._startTime ;
 		_stopTime = p._stopTime ;
		_software = p._software;
        _hardware = p._hardware ;
		_resource = p._resource;
		_facility = p._facility;
		_processing = p._processing;
 	}
 	return *this ;
}

void ProcessLog::addResource(const Resource& resourceLog)
{
	if(find(_resource.begin(), _resource.end(), resourceLog) == _resource.end()) { //skip duplicates
		_resource.push_back(resourceLog);
    }
}

void ProcessLog::setResource(const vector<Resource>& resourceLog)
{
	_resource.clear();
    size_t rl_size = resourceLog.size() ;
	for (size_t i = 0; i <rl_size; ++i) {
		addResource(resourceLog[i]);
    }
}


Processing::Processing(const std::string & safeNamespace,
                       const ProcessLog& processing,
                       const string& safePrefix):
    WrappedMetadata(safeNamespace, safePrefix),
	_processingLog(processing)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("Processing: empty namespace. ");
	}
	Processing::writeXmlStream();
}

Processing::Processing(const rsResourceSet & rs) :
    WrappedMetadata("", ""),
    _processingLog()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Processing::readXmlStream(rs);
}


Processing::Processing() :
    WrappedMetadata("", ""),
    _processingLog()
{};


// copy constructor
Processing::Processing(const Processing& r):
	WrappedMetadata(r),
    _processingLog(r._processingLog)
{
}

Processing::~Processing() throw() {
}

// operator =
Processing &Processing::operator=(const Processing& r)
{
 	if (this != &r) {
        WrappedMetadata::operator=(r) ;
		_processingLog = r._processingLog;
 	}
 	return *this ;
}

void Processing::writeSoftware(const Software& sw, ostringstream& xmlInfo, const string& indent)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	const vector<Software> &softw = sw.software();
	bool emptyTag = softw.empty();

	xmlInfo << indent << "<"<<getSafePrefix()
            <<"software name=\"" << sw.name()
            << "\" version=\"" << sw.version() << (emptyTag?"\"/>\n":"\">\n"); // PRQA S 3380

	if(!emptyTag)
	{
        size_t sw_size = softw.size() ;
		for(size_t i=0; i< sw_size; ++i) {
			writeSoftware(softw[i], xmlInfo, indent + "\t");
        }

		xmlInfo << indent << "</"<<getSafePrefix()<<"software>\n";
	}
}

void Processing::writeHardware(const Hardware& hw, ostringstream& xmlInfo, const string& indent)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    const vector<Hardware> &hardw = hw.hardware();
    bool emptyTag = hardw.empty();

    xmlInfo << indent << "<"<<getSafePrefix()<<"hardware name=\"" << hw.name() << (emptyTag?"\"/>\n":"\">\n");

    if(!emptyTag)
    {
        size_t hw_size = hardw.size() ;
        for(unsigned int i=0; i< hw_size; ++i) {
            writeHardware(hardw[i], xmlInfo, indent + "\t");
        }

        xmlInfo << indent << "</"<<getSafePrefix()<<"hardware>\n";
    }
}

void Processing::writeFacility(const Facility& fac, ostringstream& xmlInfo, const string& indent)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

        const vector<Software> sw = fac.software();
        // bool emptyTag = sw.empty();
        const vector<Hardware> hw = fac.hardware();
        // bool emptyTagHd = hw.empty();

	xmlInfo << indent << "<"<<getSafePrefix()<<"facility name=\"" << fac.name() << "\"" ;
	if(!fac.organization().empty()) { xmlInfo << " organisation=\"" << fac.organization() << "\""; }
	if(!fac.site().empty()) 	{ xmlInfo << " site=\"" << fac.site() << "\""; }
	if(!fac.country().empty())	{ xmlInfo << " country=\"" << fac.country() << "\""; }
	//xmlInfo <<  (emptyTag?"/>\n":">\n");
	xmlInfo <<  ">\n";
	/*if(!emptyTag)
	{
        size_t sw_size = sw.size() ;
		for(unsigned int i=0; i< sw_size; ++i) {
			writeSoftware(sw[i], xmlInfo, indent + "\t");
        }

		xmlInfo << indent << "</"<<getSafePrefix()<<"sentinel-safe:facility>\n";
	}*/
}

void Processing::writeResource(const Resource& resource, ostringstream& xmlInfo, const string& indent)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	const vector<ProcessLog> &proc = resource.processing();
	bool emptyTag = proc.empty();

	xmlInfo << indent << "<"<<getSafePrefix()<<"resource name=\"" << resource.name() << "\" role=\"" << resource.role() << "\"";
	if(!resource.version().empty()) { xmlInfo << " version=\"" << resource.version() << "\""; }
	if(!resource.href().empty()) { xmlInfo << " href=\"" << resource.href() << "\""; }
	xmlInfo << (emptyTag?"/>\n":">\n");
	if(!emptyTag)
	{
        size_t pr_size = proc.size() ;
		for(size_t i=0; i< pr_size; ++i) {
			writeProcessLog(proc[i], xmlInfo, indent + "\t");
        }

		xmlInfo << indent << "</"<<getSafePrefix()<<"resource>\n";
	}
}

void Processing::writeProcessLog(const ProcessLog& processing, ostringstream& xmlInfo, const string& indent)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (!processing.startTime().empty() || !processing.stopTime().empty())
		xmlInfo << indent << "<"<<getSafePrefix()<<"processing name=\"" << processing.name() << "\" start=\"" << processing.startTime() << "\" stop=\""  << processing.stopTime()<< "\"";
	else
		xmlInfo << indent << "<"<<getSafePrefix()<<"processing name=\"" << processing.name() << "\"";
	xmlInfo <<">\n" ;

	const vector<Facility> &facilities =  processing.facility();

    size_t fac_size = facilities.size() ;
	for(size_t i=0; i< fac_size; ++i) {
		writeFacility(facilities[i], xmlInfo, indent + "\t");
    }

    const vector<Hardware> &hardware = processing.hardware() ;
    size_t hw_size = hardware.size() ;
    for(size_t i=0; i< hw_size; ++i) {
        writeHardware(hardware[i], xmlInfo, indent + "\t\t");
    }

	const vector<Software> &software = processing.software() ;
    size_t sw_size = software.size() ;
	for(size_t i=0; i< sw_size; ++i) {
		writeSoftware(software[i], xmlInfo, indent + "\t\t");
    }
	xmlInfo << indent << "\t</"<<getSafePrefix() << "facility>\n";

	const vector<Resource> &resource = processing.resource() ;
    size_t rs_size = resource.size() ;
	for(size_t i=0; i< rs_size; ++i) {
		writeResource(resource[i], xmlInfo, indent + "\t");
    }

	const vector<ProcessLog> &proc = processing.processing() ;
    size_t pr_size = proc.size() ;
	for(unsigned int i=0; i< pr_size; ++i) {
		writeProcessLog(proc[i], xmlInfo, indent + "\t");
    }

	xmlInfo << indent << "</"<<getSafePrefix()<<"processing>\n" ;

}

void Processing::writeXmlStream()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream xmlInfo;
	xmlInfo << "<xmlData>\n" ;

	writeProcessLog(_processingLog, xmlInfo, "\t");

	xmlInfo << "</xmlData>\n" ;

	setXMLStream(xmlInfo.str()) ;

}

void Processing::readXmlStream(const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	readProcessLog(rs);
}
void Processing::readProcessLog(const rsResourceSet & rs, const string & rootKey)
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readProcessLog() called with key " << rootKey);

	uint numObjects = rs.getArraySize("metadataSection.metadataObject");

	for (uint i = 0; i < numObjects; i++)
	{
		std::ostringstream objectKey; objectKey << "metadataSection.metadataObject" << "[" << i << "]";

		std::string id;
		rs.getValue(objectKey.str()+".ID.value", id);

		if (id=="processing")
		{

			objectKey << ".metadataWrap.";
			readSingleProcessLog(objectKey.str() + rootKey, rs, _processingLog);
			break;
		}
	}


}

void Processing::readSingleProcessLog(const string & rootKey, const rsResourceSet & rs, ProcessLog & processingLog)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readSingleProcessLog() called with key " << rootKey);

	string name("");
	if (rs.hasKey(rootKey+".name.value"))
		rs.getValue(rootKey+".name.value", name);

	string start;
	if (rs.hasKey(rootKey+".start.value"))
		rs.getValue(rootKey+".start.value", start);
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Optional metadata 'processing.start' missing");
	}
	string stop;
	if (rs.hasKey(rootKey+".stop.value"))
		rs.getValue(rootKey+".stop.value", stop);
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Optional metadata 'processing.start' missing");
	}


	ACS_CLASS_BGN_DEBUG(10)
		ostringstream msg; msg << "Processing: processing metadata content:" << endl;
		msg << "\tname \t<" << name << ">" << endl;
		msg << "\tstart \t<" << start << ">" << endl;
		msg << "\tstop \t<" << stop << ">" << endl;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	if (!name.empty())
		processingLog.setName(name);
	processingLog.setStartTime(start);
	processingLog.setStopTime(stop);

	readFacility(rs, processingLog, rootKey+".facility");
	readSoftware(rs, processingLog, rootKey+".software");
	readHardware(rs, processingLog, rootKey+".hardware");
	readResources(rs, processingLog, rootKey+".resource");

}
void Processing::readFacility(const rsResourceSet & rs, ProcessLog & processingLog, const string & rootKey)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readFacility called with key " << rootKey);
	if (rs.hasKey(rootKey))
	{
		uint count = rs.getArraySize(rootKey);
		if (count==1)
		{
			readSingleFacility(rootKey, rs, processingLog);
		}
		else
		{
			for (uint i = 0 ; i < count; i++)
			{
				ostringstream newKey; newKey << rootKey <<"[" << i << "]";
				readSingleFacility(newKey.str(), rs, processingLog);
			}
		}
	}

}

void Processing::readSingleFacility(const string & objectKey, const rsResourceSet & rs, ProcessLog & processingLog)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readSingleFacility called with key " << objectKey);
	string country("");
	if (rs.hasKey(objectKey+".country.value"))
		rs.getValue(objectKey+".country.value", country);
	string name;
	if (rs.hasKey(objectKey+".name.value"))
		rs.getValue(objectKey+".name.value", name);
	else
	    	ACS_THROW(exProcessingException("Mandatory metadata 'processing.facility.name' missing \""));

	string org("");
	if (rs.hasKey(objectKey+".organisation.value"))
		rs.getValue(objectKey+".organisation.value", org);

	string site("");
	if (rs.hasKey(objectKey+".site.value"))
		rs.getValue(objectKey+".site.value", site);

	Facility fac(name, org, site, country);
	processingLog.addFacility(fac);

	ACS_CLASS_BGN_DEBUG(10)
		ostringstream msg; msg << "Processing: processing.facility metadata content:" << endl;
		msg << "\tname \t<" << name << ">" << endl;
		msg << "\tcountry <" << country << ">" << endl;
		msg << "\torganisation <" << org << ">" << endl;
		msg << "\tsite \t<" << site << ">" << endl;
		ACS_LOG_DEBUG(msg.str()) ;
	ACS_CLASS_END_DEBUG

	if (rs.hasKey(objectKey+".software"))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: processing.facility metadata content: inner software found!");
		readSoftware(rs, processingLog, objectKey+".software");
	}

	if (rs.hasKey(objectKey+".hardware"))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: processing.facility metadata content: inner hardware found!");
		readHardware(rs, processingLog, objectKey+".hardware");
	}


	if (rs.hasKey(objectKey+".facility"))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: processing.facility metadata content: inner facility found!");
		readSingleFacility(objectKey+".facility", rs, processingLog);
	}

}

void Processing::readSoftware(const rsResourceSet & rs, ProcessLog & processingLog, const string & rootKey)
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readSoftware called with key " << rootKey);

	if (rs.hasKey(rootKey))
	{
		uint count = rs.getArraySize(rootKey);

		if (count==1)
		{
			readSingleSoftware(rootKey, rs, processingLog);
		}
		else
		{
			for (uint i = 0 ; i < count; i++)
			{
				ostringstream newKey; newKey << rootKey << "[" << i << "]";
				readSingleSoftware(newKey.str(), rs, processingLog);
			}
		}
	}

}

void Processing::readSingleSoftware(const string & objectKey, const rsResourceSet & rs, ProcessLog & processingLog)
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readSingleSoftware called with key " << objectKey);

	string name;
	if (rs.hasKey(objectKey+".name.value"))
		rs.getValue(objectKey+".name.value", name);
	else
	    	ACS_THROW(exProcessingException("Mandatory metadata 'processing.software.name' missing \""));

	string version("");
	if (rs.hasKey(objectKey+".version.value"))
		rs.getValue(objectKey+".version.value", version);

	Software sw(name, version);
	processingLog.addSoftware(sw);

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Processing: processing.software metadata content:") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\tname \t<" << name << ">") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\tversion \t<" << version << ">") ;
	ACS_CLASS_END_DEBUG

	if (rs.hasKey(objectKey+".software"))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: processing.software metadata content: inner software found!");

		readSoftware(rs, processingLog, objectKey+".software");
	}

}
void Processing::readHardware(const rsResourceSet & rs, ProcessLog & processingLog, const string & rootKey)
{

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readHardware called with key " << rootKey);

    if (rs.hasKey(rootKey))
    {
        uint count = rs.getArraySize(rootKey);

        if (count==1)
        {
            readSingleHardware(rootKey, rs, processingLog);
        }
        else
        {
            for (uint i = 0 ; i < count; i++)
            {
                ostringstream newKey; newKey << rootKey << "[" << i << "]";
                readSingleHardware(newKey.str(), rs, processingLog);
            }
        }
    }

}

void Processing::readSingleHardware(const string & objectKey, const rsResourceSet & rs, ProcessLog & processingLog)
{

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readSingleHardware called with key " << objectKey);

    string name;
    if (rs.hasKey(objectKey+".name.value")) {
        rs.getValue(objectKey+".name.value", name);
    }
    else {
        ACS_THROW(exProcessingException("Mandatory metadata 'processing.hardware.name' missing \"")); // PRQA S 3081
    }

    Hardware hd(name);
    processingLog.addHardware(hd);

    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Processing: processing.hardware metadata content:") ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\tname \t<" << name << ">") ;
    ACS_CLASS_END_DEBUG

    if (rs.hasKey(objectKey+".hardware"))
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: processing.hardware metadata content: inner software found!");

        readHardware(rs, processingLog, objectKey+".hardware");
    }

}
void Processing::readResources(const rsResourceSet & rs, ProcessLog & processingLog, const string & rootKey)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readResources called with key " << rootKey);

	if (rs.hasKey(rootKey))
	{
		uint count = rs.getArraySize(rootKey);

		if (count==1)
		{
			readSingleResource(rootKey, rs, processingLog);
		}
		else
		{
			for (uint i = 0 ; i < count; i++)
			{
				ostringstream newKey; newKey << rootKey << "[" << i << "]";
				readSingleResource(newKey.str(), rs, processingLog);
			}
		}
 	}
}

void Processing::readSingleResource(const string & rootKey, const rsResourceSet & rs, ProcessLog & processingLog)
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: readSingleResource called with key " << rootKey);

	string name;
	if (rs.hasKey(rootKey+".name.value"))
		rs.getValue(rootKey+".name.value", name);
	else
	    	ACS_THROW(exProcessingException("Mandatory metadata 'processing.resource.name' missing \""));
	string role;
	if (rs.hasKey(rootKey+".role.value"))
		rs.getValue(rootKey+".role.value", role);
	else
	    	ACS_THROW(exProcessingException("Mandatory metadata 'processing.resource.role' missing \""));
	string href("");
	if (rs.hasKey(rootKey+".href.value"))
		rs.getValue(rootKey+".href.value", href);

	string version("");
	if (rs.hasKey(rootKey+".version.value"))
		rs.getValue(rootKey+".version.value", version);

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << "Processing: processing.resource metadata content:" << endl;
		msg << "\tname \t<" << name << ">" << endl;
		msg << "\trole \t<" << role << ">" << endl;
		msg << "\thref \t<" << href << ">" << endl;
		if (!version.empty())
			msg << "\tversion \t<" << version << ">" << endl;
		ACS_LOG_DEBUG(msg.str()) ;
	ACS_CLASS_END_DEBUG

	Resource res(name, role, href, version);
	if (rs.hasKey(rootKey+".processing"))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing: processing.resource metadata content: inner processing found!");
		ProcessLog procLog;
		readSingleProcessLog(rootKey+".processing", rs, procLog);
		res.addProcessing(procLog);
	}
	processingLog.addResource(res);
}


_ACS_END_NAMESPACE
