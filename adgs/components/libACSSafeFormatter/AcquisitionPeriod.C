// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia Spa;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.11  2016/05/11 09:45:43  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.10  2014/04/24 16:43:28  tergem
	Porting to head
	
	Revision 1.9  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.8  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.7  2012/11/29 09:17:52  damdec
	Check on mandatory field startTime modified.
	
	Revision 1.6  2012/08/03 12:57:09  damdec
	readXmlStream method updated.
	
	Revision 1.5  2012/07/26 12:49:43  damdec
	Debug messages added.
	
	Revision 1.4  2012/06/29 11:01:54  damdec
	readXmlStream method added.
	
	Revision 1.3  2012/04/17 15:56:52  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.2  2012/03/15 14:59:22  damdec
	Implementation finalized.
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/ 
#include <AcquisitionPeriod.h>

#include <Filterables.h>
#include <StringUtils.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AcquisitionPeriod)



AcquisitionPeriod::AcquisitionPeriod(const string & safeNamespace, 
                                     const std::string& startTime, 
                                     const std::string& stopTime,
                                     const std::string& safePrefix) : 
    WrappedMetadata(safeNamespace, safePrefix),
    _startTime(startTime), 
    _stopTime(stopTime)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("AcquisitionPeriod: empty namespace. ");
	}
	AcquisitionPeriod::writeXmlStream();
}
AcquisitionPeriod::AcquisitionPeriod(const rsResourceSet & rs) :
    WrappedMetadata("",""),
    _startTime(), 
    _stopTime()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	AcquisitionPeriod::readXmlStream(rs);
}

AcquisitionPeriod::~AcquisitionPeriod() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

AcquisitionPeriod::AcquisitionPeriod(const AcquisitionPeriod & r) : 
    WrappedMetadata(r),
    _startTime(r._startTime), 
    _stopTime(r._stopTime)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

AcquisitionPeriod& AcquisitionPeriod::operator =(const AcquisitionPeriod & r) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (this != &r) {
		WrappedMetadata::operator=(r) ;
		_startTime = r._startTime;
		_stopTime = r._stopTime;
		
	}
	return *this;
	
}

void AcquisitionPeriod::writeXmlStream()
{

        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		const string &spfx = getSafePrefix() ;

        ostringstream xmlInfo;
        xmlInfo << "<xmlData>\n" ;
        xmlInfo << "\t<"<< spfx <<"acquisitionPeriod>\n" ;
        xmlInfo << "\t\t<"<< spfx <<"startTime>" << _startTime << "</"<< spfx <<"startTime>\n" ;
        if (!_stopTime.empty())
                xmlInfo << "\t\t<"<< spfx <<"stopTime>"  << _stopTime  << "</"<< spfx <<"stopTime>\n" ;
        xmlInfo << "\t</"<< spfx <<"acquisitionPeriod>\n" ;
        xmlInfo << "</xmlData>\n" ;

        setXMLStream(xmlInfo.str());

}

void AcquisitionPeriod::readXmlStream(const rsResourceSet & rs)
{

        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

        size_t numObjects = rs.getArraySize("metadataSection.metadataObject");
		string spfx = getSafePrefix();

        for (uint i = 0; i < numObjects; i++)
        {
                std::ostringstream objectKey; 
		objectKey << "metadataSection.metadataObject" << "[" << i << "]";

                std::string id;
                rs.getValue(objectKey.str()+".ID.value", id);

                if (id=="acquisitionPeriod")
                {
                        objectKey << ".metadataWrap.";

                        if (rs.hasKey(objectKey.str()+"xmlData."+spfx+"acquisitionPeriod.startTime"))
                        {
                                rs.getValue(objectKey.str()+"xmlData."+spfx+"acquisitionPeriod.startTime", _startTime);
                                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AcquisitionPeriod: xmlData.acquisitionPeriod.startTime metadata content <" << _startTime << ">");
                        }
                        else
                                ACS_THROW(exAcquisitionPeriodException("Mandatory field acquisitionPeriod/startTime not found\""));

                        if (rs.hasKey(objectKey.str()+"xmlData."+spfx+"acquisitionPeriod.stopTime"))
                        {
                                rs.getValue(objectKey.str()+"xmlData."+spfx+"acquisitionPeriod.stopTime", _stopTime);
                                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AcquisitionPeriod: xmlData.acquisitionPeriod.stopTime metadata content <" << _stopTime << ">");

                        }
                        break;
                }
        }

}

_ACS_END_NAMESPACE
