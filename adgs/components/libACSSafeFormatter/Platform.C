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
	Revision 1.11  2016/05/11 09:45:43  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.10  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.9  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.8  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.7  2013/03/27 15:17:50  marfav
	WorkInProgress trying to read files during PUG pre-acceptance
	
	Revision 1.6  2012/08/03 13:00:14  damdec
	Constructor for reading platform metadata and related method added.
	
	Revision 1.5  2012/06/29 11:04:27  damdec
	Methods returning private attribute added. New constructors added.
	
	Revision 1.4  2012/04/17 15:56:51  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.3  2012/03/21 13:31:26  damdec
	Instrument class attribute updated
	
	Revision 1.2  2012/03/15 15:47:09  damdec
	Implementation finalized
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/ 

#include <Platform.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Platform)



Platform::Platform(	const string & safeNamespace, const std::string& family_name, const std::string& nssdc_id, const std::string& number,
					const Instrument& instrument, const vector<TimeRef>& timeRef,
					const string& safePrefix):
    WrappedMetadata(safeNamespace, safePrefix),
	_familyName(family_name),
	_nssdcId(nssdc_id),
	_number(number),
	_instrument(instrument),
	_timeRef(timeRef)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("Platform: empty namespace. ");
	}
	Platform::writeXmlStream();
}

Platform::Platform() : 
    WrappedMetadata("",""), 
    _familyName(), 
    _nssdcId(), 
    _number(), 
    _instrument(), 
    _timeRef()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
}


Platform::Platform(const rsResourceSet & rs) :
    WrappedMetadata("",""), 
	_familyName(), 
	_nssdcId(), 
	_number(), 
	_instrument(), 
	_timeRef()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Platform::readXmlStream(rs);
}

Platform::~Platform() throw() 
{

}

Platform::Platform(const Platform & r) : 
    WrappedMetadata(r), 
    _familyName(r._familyName), 
    _nssdcId(r._nssdcId), 
    _number(r._number),
    _instrument(r._instrument), 
    _timeRef(r._timeRef)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

Platform& Platform::operator =(const Platform & r) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (this != &r) {
		WrappedMetadata::operator=(r) ;
		_familyName = r._familyName;
		_nssdcId = r._nssdcId;
		_number = r._number;
		_instrument = r._instrument;
		_timeRef = r._timeRef;
	}
	return *this;
	
}

void Platform::writeXmlStream()
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	const string &spfx = getSafePrefix() ;
	ostringstream xmlInfo;
	xmlInfo << "<xmlData>\n" ;
	xmlInfo << "\t<"<<spfx<<"platform";
	xmlInfo << ( spfx.empty()? " xmlns=\"" + getSafeNamespace() + "\">\n" : ">\n" ); // PRQA S 3380, 3385
	if (!_nssdcId.empty()) {
		xmlInfo << "\t\t<"<<spfx<<"nssdcIdentifier>" << _nssdcId << "</"<<spfx<<"nssdcIdentifier>\n" ;
    }
	if (!_familyName.empty()) {
		xmlInfo << "\t\t<"<<spfx<<"familyName>"  << _familyName  << "</"<<spfx<<"familyName>\n" ;
    }
	if (!_number.empty()) {
		xmlInfo << "\t\t<"<<spfx<<"number>" << _number << "</"<<spfx<<"number>\n" ;
    }

	if (!_instrument.familyName.empty() || 
        !_instrument.number.empty() || 
        (0.0 != _instrument.sideLookingAngle) || // PRQA S 3270 2
        (0.0 != _instrument.fieldOfView))
	{
		xmlInfo << "\t\t<"<<spfx<<"instrument>\n";

			ACS_COND_THROW(_instrument.familyName.empty(), exPlatformException("Mandatory element instrument.familyName not filled!") ); // PRQA S 3081
			xmlInfo << "\t\t\t<"<<spfx<<"familyName abbreviation=\"" << _instrument.abbreviation << "\">" << _instrument.familyName << "</"<<spfx<<"familyName>\n" ;
			if (!_instrument.number.empty()) {
				xmlInfo << "\t\t\t<"<<spfx<<"number>" << _instrument.number << "</"<<spfx<<"number>\n" ;
            }
			if (0.0 != _instrument.sideLookingAngle) { // PRQA S 3270
				xmlInfo << "\t\t\t<"<<spfx<<"sideLookingAngle>" << _instrument.sideLookingAngle << "</"<<spfx<<"sideLookingAngle>\n" ;
            }
			if (0.0 != _instrument.fieldOfView) { // PRQA S 3270
				xmlInfo << "\t\t\t<"<<spfx<<"fieldOfView>" << _instrument.fieldOfView << "</"<<spfx<<"fieldOfView>\n" ;
            }
		xmlInfo << "\t\t</"<<spfx<<"instrument>\n";
	}

    size_t tr_size = _timeRef.size() ;
	for (size_t i=0; i < tr_size; ++i)
	{
		xmlInfo << "\t\t<"<<spfx<<"timeReference>\n" ;
			if (!_timeRef[i].utc.empty()) {
				xmlInfo << "\t\t\t<"<<spfx<<"utc>" << _timeRef[i].utc << "</"<<spfx<<"utc>\n" ;
            }
			if (0 != _timeRef[i].clock) {
				xmlInfo << "\t\t\t<"<<spfx<<"clock>" << _timeRef[i].clock << "</"<<spfx<<"clock>\n" ;
            }
			if (0 != _timeRef[i].clockStep) {
				xmlInfo << "\t\t\t<"<<spfx<<"clockStep>" << _timeRef[i].clockStep << "</"<<spfx<<"clockStep>\n" ;
            }
		xmlInfo << "\t\t</"<<spfx<<"timeReference>\n" ;
	}
	xmlInfo << "\t</"<<spfx<<"platform>\n" ;
	xmlInfo << "</xmlData>\n" ;

	setXMLStream(xmlInfo.str());
}

void Platform::readXmlStream(const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	size_t numObjects = rs.getArraySize("metadataSection.metadataObject"); // PRQA S 4412

	for (size_t i = 0; i < numObjects; i++)
	{
		std::ostringstream objectKey; 
        objectKey << "metadataSection.metadataObject" << "[" << i << "]";
		
		std::string id;
		rs.getValue(objectKey.str()+".ID.value", id);


		if (id=="platform")
		{
			objectKey << ".metadataWrap.";

			// Check the namespace exixtance
			string safeNS ("");
			if (!rs.hasKey(objectKey.str()+"xmlData.platform"))
			{
				safeNS = getSafePrefix();
			}

			string key;
			key = objectKey.str()+"xmlData."+safeNS+"platform."+safeNS+"nssdcIdentifier";
			if (rs.hasKey(key)) {
				rs.getValue(key, _nssdcId);
            }
			else {
                ACS_THROW(exPlatformException("Mandatory metadata 'platform.nssdcIdentifier' missing \" key is ["+key+"]"));	 // PRQA S 3081
            }
			
			key = objectKey.str()+"xmlData."+safeNS+"platform."+safeNS+"familyName";
			if (rs.hasKey(key)) {
				rs.getValue(key, _familyName);
            }

			key = objectKey.str()+"xmlData."+safeNS+"platform."+safeNS+"number";
			if (rs.hasKey(key)) {
				rs.getValue(key, _number);
            }

			string abbreviation;
			key = objectKey.str()+"xmlData."+safeNS+"platform."+safeNS+"instrument."+safeNS+"familyName.abbreviation.value";
			if (rs.hasKey(key)) {
				rs.getValue(key, abbreviation);
            }
			
			string instFamilyName("");
			key = objectKey.str()+"xmlData."+safeNS+"platform."+safeNS+"instrument."+safeNS+"familyName";
			if (rs.hasKey(key)) {
				rs.getValue(key, instFamilyName);
            }

#warning Some tags are not aligned to applicable documents. Cross check ASAP.
			string instNumber("");
			if (rs.hasKey(objectKey.str()+"xmlData.platform.instrument.number")) {
				rs.getValue(objectKey.str()+"xmlData.platform.instrument.number", instNumber);
            }

			double sideLookingAngle(0);
			if (rs.hasKey(objectKey.str()+"xmlData.platform.instrument.sideLookingAngle"))
			{
				rs.getValue(objectKey.str()+"xmlData.platform.instrument.sideLookingAngle", sideLookingAngle);
			}
				 
			double fieldOfView(0);
			if (rs.hasKey(objectKey.str()+"xmlData.platform.instrument.fieldOfView"))
			{
				rs.getValue(objectKey.str()+"xmlData.platform.instrument.fieldOfView", fieldOfView);
			}

			Instrument inst(instFamilyName, abbreviation, instNumber, sideLookingAngle, fieldOfView);	 
			_instrument = inst;  
			 
			size_t count = rs.getArraySize(objectKey.str()+"xmlData.platform.timeReference"); // PRQA S 4412
			if (count == 0)
			{
				ostringstream key1; 
                key1 << objectKey.str() << "xmlData.platform.timeReference";
				string utc;
				if (rs.hasKey(key1.str()+".utc")) {
					rs.getValue(key1.str()+".utc", utc);
                }

				long int clock(0);
				if (rs.hasKey(key1.str()+".clock"))
				{
					rs.getValue(key1.str()+".clock", clock);
				}
				long int clockStep(0);
				if (rs.hasKey(key1.str()+".clockStep"))
				{
					rs.getValue(key1.str()+".clockStep", clockStep);
				}
				TimeRef tmp(utc, clock, clockStep);
				_timeRef.push_back(tmp);
			}
			for (size_t j = 0; j < count; ++j)
			{
				ostringstream key1; 
                key1 << objectKey.str() << "xmlData.platform.timeReference[" << j << "]";
				string utc;
				if (rs.hasKey(key1.str()+".utc")) {
					rs.getValue(key1.str()+".utc", utc);
                }

				long int clock(0);
				if (rs.hasKey(key1.str()+".clock"))
				{
					rs.getValue(key1.str()+".clock", clock);
				}
				long int clockStep(0);
				if (rs.hasKey(key1.str()+".clockStep"))
				{
					rs.getValue(key1.str()+".clockStep", clockStep);
				}
				TimeRef tmp(utc, clock, clockStep);
				_timeRef.push_back(tmp);
			}
			 
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << "Platform: Platform metadata content: " << '\n';
				msg << "\t\t nssdcIdentifier: \t"  << _nssdcId << '\n';
				msg << "\t\t familyName: \t"  << _familyName << '\n';
				msg << "\t\t number: \t"  << _number << '\n';
				msg << "\t\t instrument.familyName.abbreviation: \t"  << _instrument.abbreviation << '\n';
				msg << "\t\t instrument.familyName: \t"  <<  _instrument.familyName << '\n';
				msg << "\t\t instrument.number: \t"  <<  _instrument.number << '\n';
				msg << "\t\t instrument.sideLookingAngle: \t"  <<  _instrument.sideLookingAngle << '\n';
				msg << "\t\t instrument.fieldOfView: \t"  <<  _instrument.fieldOfView << '\n';
				size_t tr_size =  _timeRef.size() ;
                for (size_t j = 0 ; j < tr_size; ++j)
				{
					msg << "\t\t timeRef.utc: \t"  <<  _timeRef[j].utc << '\n';
					msg << "\t\t timeRef.clock: \t"  <<  _timeRef[j].clock << '\n';
					msg << "\t\t timeRef.clockStep: \t"  <<  _timeRef[j].clockStep << '\n';
				}
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
			break;
		}
	}   

}



_ACS_END_NAMESPACE
