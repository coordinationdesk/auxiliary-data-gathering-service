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
	Revision 1.13  2018/07/17 09:37:33  giucas
	Safe Namaspace attribute not added if empty.
	
	Revision 1.12  2018/06/21 13:43:08  giucas
	Typos fixed
	
	Revision 1.11  2014/07/04 13:32:09  tergem
	Fixing from S3PDGS-1890 comments
	
	Revision 1.10  2014/04/24 16:43:28  tergem
	Porting to head
	
	Revision 1.9  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.8  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.7  2013/10/11 07:05:26  damdec
	S3PDGS-1609 fixing. Performed by tergem
	
	Revision 1.6  2012/12/21 14:28:39  damdec
	Main tag renamed as measurementOrbitReference
	
	Revision 1.5  2012/11/29 09:17:06  damdec
	Methods for reading orbit/relative orbit added.
	
	Revision 1.4  2012/08/03 13:05:28  damdec
	Methods for reading orbit reference metadata type introduced.
	
	Revision 1.3  2012/06/29 11:03:25  damdec
	Methods returning private attribute added. Default constructor added.
	
	Revision 1.2  2012/04/17 15:57:22  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.1  2012/03/15 14:54:42  damdec
	OrbitReference class added.
	
        
*/ 
#include <OrbitReference.h>

#include <Filterables.h>
#include <StringUtils.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrbitReference)



OrbitReference::OrbitReference(const string & safeNamespace, 
                               unsigned int cycle, 
                               const std::string& phase,  
                               unsigned int start_orbit, 
                               unsigned int stop_orbit,
                               unsigned int relative_start_orbit, 
                               unsigned int relative_stop_orbit,
                               const std::string& safePrefix) :
    WrappedMetadata(safeNamespace, safePrefix),
    _startOrbit(start_orbit), 
    _stopOrbit(stop_orbit),  
    _relStartOrbit(relative_start_orbit), 
    _relStopOrbit(relative_stop_orbit),
	_cycle(cycle), 
    _phase(phase)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("OrbitReference: empty namespace. ");
	}
	OrbitReference::writeXmlStream();
}

OrbitReference::OrbitReference():
    WrappedMetadata("", ""),
	_startOrbit(), 
	_stopOrbit(),  
	_relStartOrbit(), 
	_relStopOrbit(),
	_cycle(), 
	_phase()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

OrbitReference::OrbitReference(const rsResourceSet & rs) :
    WrappedMetadata("", ""),
	_startOrbit(), 
	_stopOrbit(),  
	_relStartOrbit(), 
	_relStopOrbit(),
	_cycle(), 
	_phase()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	OrbitReference::readXmlStream(rs);
}

OrbitReference::~OrbitReference() throw() {}

OrbitReference::OrbitReference(const OrbitReference& r) :
    WrappedMetadata(r),
    _startOrbit(r._startOrbit), 
    _stopOrbit(r._stopOrbit), 
    _relStartOrbit(r._relStartOrbit), 
    _relStopOrbit(r._relStopOrbit), 
    _cycle(r._cycle), 
    _phase(r._phase)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

}

OrbitReference& OrbitReference::operator = (const OrbitReference& r) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (this != &r) {
		//<Attribute copy>
        WrappedMetadata::operator=(r) ;
		_startOrbit = r._startOrbit;
		_stopOrbit = r._stopOrbit;
		_relStartOrbit = r._relStartOrbit;
		_relStopOrbit = r._relStopOrbit;
		_cycle = r._cycle;
		_phase = r._phase;
	}
	return *this;
}

void OrbitReference::writeXmlStream()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const string &spfx = getSafePrefix() ;
	ostringstream xmlInfo;
	xmlInfo << "<xmlData>\n" ;
	xmlInfo << "\t<"<<spfx<<"orbitReference";
	xmlInfo << ( !getSafeNamespace().empty()? " xmlns=\"" + getSafeNamespace() + "\">\n" : ">\n" ); // PRQA S 3380, 3385
	writeCommonXmlStream(xmlInfo);
	xmlInfo << "\t</"<<spfx<<"orbitReference>\n" ;
	xmlInfo << "</xmlData>\n" ;
	setXMLStream(xmlInfo.str()) ;
    
}
void OrbitReference::writeCommonXmlStream(ostringstream & xmlInfo)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	const string &spfx = getSafePrefix() ;
	
	if (0 != _startOrbit) {
		xmlInfo << "\t\t<"<< spfx<< "orbitNumber type=\"start\">" << _startOrbit << "</"<< spfx << "orbitNumber>\n" ;
    }
	// S3PDGS-1609 start
	if (_startOrbit != _stopOrbit)
	{
		if (0 != _stopOrbit) {
			xmlInfo << "\t\t<"<< spfx<<"orbitNumber type=\"stop\">" << _stopOrbit << "</"<< spfx << "orbitNumber>\n" ;
        }
	}
	// S3PDGS-1609 stop
	if (0 != _relStartOrbit) {
		xmlInfo << "\t\t<"<< spfx << "relativeOrbitNumber type=\"start\">" << _relStartOrbit << "</"<< spfx << "relativeOrbitNumber>\n" ;
    }
	// S3PDGS-1609 start
	if (_relStopOrbit != _relStartOrbit)
	{
		if (0 != _relStopOrbit) {
			xmlInfo << "\t\t<"<< spfx << "relativeOrbitNumber type=\"stop\">" << _relStopOrbit << "</"<< spfx << "relativeOrbitNumber>\n" ;
        }
	}
	// S3PDGS-1609 stop
	if (0 != _cycle) {
		xmlInfo << "\t\t<"<< spfx << "cycleNumber>" << _cycle << "</"<< spfx << "cycleNumber>\n" ;
    }
	if (!_phase.empty()) {
		xmlInfo << "\t\t<"<< spfx << "phaseIdentifier>" << _phase << "</"<< spfx << "phaseIdentifier>\n" ;
    }
    
}

void OrbitReference::readXmlStream(const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	size_t numObjects = rs.getArraySize("metadataSection.metadataObject"); // PRQA S 4412
	string spfx = getSafePrefix();
	
	for (size_t i = 0; i < numObjects; i++)
	{
		std::ostringstream objectKey; 
        objectKey << "metadataSection.metadataObject" << "[" << i << "]";
		
		std::string id;
		rs.getValue(objectKey.str()+".ID.value", id);

		if (id=="measurementOrbitReference")
		{
			objectKey << ".metadataWrap.";
			
			size_t n = rs.getArraySize(objectKey.str()+"xmlData."+spfx+"orbitReference.orbitNumber"); // PRQA S 4412
			if (0 == n)
			{
				readOrbit(rs, objectKey.str() + "xmlData."+spfx+"orbitReference.orbitNumber");
			}
			else
			{
				for (size_t j = 0; j < n; ++j)
				{
					ostringstream tmpKey; 
                    tmpKey << objectKey.str() << "xmlData."+spfx+"orbitReference.orbitNumber[" << j << "]";
					readOrbit(rs, tmpKey.str());
				}
			}
			size_t nR = rs.getArraySize(objectKey.str()+"xmlData."+spfx+"orbitReference.relativeOrbitNumber"); // PRQA S 4412
			if (0 == nR)
			{
				readRelativeOrbit(rs, objectKey.str() + "xmlData."+spfx+"orbitReference.relativeOrbitNumber");
			}
			else
			{
				for (size_t j =0; j < nR; j++)
				{
					ostringstream tmpKey; 
                    tmpKey << objectKey.str() << "xmlData."+spfx+"orbitReference.relativeOrbitNumber[" << j << "]";
					readRelativeOrbit(rs, tmpKey.str());
				}
			}
			if (rs.hasKey(objectKey.str()+"xmlData."+spfx+"orbitReference.cycleNumber"))
			{
				rs.getValue(objectKey.str()+"xmlData."+spfx+"orbitReference.cycleNumber", _cycle);
			}

			if (rs.hasKey(objectKey.str()+"xmlData."+spfx+"orbitReference.phaseIdentifier")) {
				rs.getValue(objectKey.str()+"xmlData."+spfx+"orbitReference.phaseIdentifier", _phase);
            }
				
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << "OrbitReference: measurementOrbitReference metadata content: " << '\n';
				msg << "\t\t startOrbit: \t"  << _startOrbit << '\n';
				msg << "\t\t stopOrbit: \t"  <<  _stopOrbit<< '\n';
				msg << "\t\t relativeStartOrbit: \t"  << _relStartOrbit << '\n';
				msg << "\t\t relativeStopOrbit: \t"  << _relStopOrbit << '\n';
				msg << "\t\t cycle: \t"  << _cycle << '\n';
				msg << "\t\t phase: \t"  << _phase << '\n';
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
			break;
		}
	} 
}

void OrbitReference::readOrbit(const rsResourceSet & rs, const string & tmpKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (rs.hasKey(tmpKey+".type.value"))
	{
	    string tagName ;
		rs.getValue(tmpKey+".type.value", tagName);
		
		if (tagName == "start") {
            rs.getValue(tmpKey, _startOrbit);
        }
		else if (tagName == "stop") {
            rs.getValue(tmpKey, _stopOrbit) ;
        }
        else { /* nothing */ }
	}
}
void OrbitReference::readRelativeOrbit(const rsResourceSet & rs, const string & tmpKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (rs.hasKey(tmpKey+".type.value"))
	{
	    string tagName ;
		rs.getValue(tmpKey+".type.value", tagName);
		if (tagName == "start") {
		    rs.getValue(tmpKey, _relStartOrbit);
        }
		else if (tagName == "stop") {
		    rs.getValue(tmpKey, _relStopOrbit);
    	}
        else { /* nothing */ }
    }
}

_ACS_END_NAMESPACE
