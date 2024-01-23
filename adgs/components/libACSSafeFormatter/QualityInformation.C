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
	Revision 1.9  2018/06/21 13:43:08  giucas
	Typos fixed
	
	Revision 1.8  2016/05/11 09:45:43  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.7  2014/04/17 12:39:05  marpas
	work in progress
	
	Revision 1.6  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.5  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.4  2012/12/21 14:28:10  damdec
	Main tag renamed as measurementQualityInformation
	
	Revision 1.3  2012/08/03 13:04:43  damdec
	Methods for reading quality information metadata type introduced.
	
	Revision 1.2  2012/04/17 15:57:48  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.1  2012/03/20 14:12:11  damdec
	Class added.
	
        
*/ 

#include <QualityInformation.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(QualityInformation)

void Location::setPath(const std::string & path, int prec, int fol, int bef, int aft) 
{
	_path = path;
	_preceding = prec;
	_following = fol;
	_before = bef;
	_after = aft;
}

QualityInformation::QualityInformation(
            const string & safeNamespace, 
            MissingElements * miss, 
            CorruptedElements* corr) : 
    WrappedMetadata(safeNamespace, ""),
	_missingElements(miss), 
	_corruptedElements(corr)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("QualityInformation: empty namespace. ");
	}
	QualityInformation::writeXmlStream();
	
}
QualityInformation::QualityInformation(const rsResourceSet & rs):
    WrappedMetadata("", ""),
	_missingElements(), 
	_corruptedElements()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	QualityInformation::readXmlStream(rs);
}

QualityInformation::~QualityInformation() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

QualityInformation::QualityInformation(const QualityInformation& aQInfo) : 
    WrappedMetadata(aQInfo),
    _missingElements(aQInfo._missingElements), 
    _corruptedElements(aQInfo._corruptedElements)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

}

QualityInformation& QualityInformation::operator = (const QualityInformation& aQualityInformation) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (this != &aQualityInformation) {
        WrappedMetadata::operator=(aQualityInformation),
		_missingElements = aQualityInformation._missingElements;
		_corruptedElements = aQualityInformation._corruptedElements;
	}
	return *this;
}

void QualityInformation::writeXmlStream()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ostringstream xmlInfo;
	xmlInfo << "<xmlData>\n" ;
	xmlInfo << "\t<measurementQualityInformation  xmlns=\"" << getSafeNamespace() << "\">\n" ;
	
	if (_missingElements)
	{
		xmlInfo << "\t\t<missingElements>\n" ;
			xmlInfo << "\t\t\t<location>\n" ;
			bool timeEmpty(true);
			bool pathEmpty(true);
			if (!_missingElements->location.startTime().empty() && !_missingElements->location.stopTime().empty())
			{
				timeEmpty = false;
				xmlInfo << "\t\t\t\t<time>\n";
				xmlInfo << "\t\t\t\t\t<start>" << _missingElements->location.startTime() << "</start>\n" ;
				xmlInfo << "\t\t\t\t\t<stop>"<< _missingElements->location.stopTime() << "</stop>\n" ;
				xmlInfo << "\t\t\t\t</time>\n";
			}
			if (!_missingElements->location.path().empty())
			{
				pathEmpty = false;
				xmlInfo << "\t\t\t\t<path";
				if (0 != _missingElements->location.preceding()) {
					xmlInfo << " preceding=\"" << _missingElements->location.preceding() << "\"";
                }
				if (0 != _missingElements->location.following()) {
					xmlInfo << " following=\"" << _missingElements->location.following() << "\"";
                }
				if (0 != _missingElements->location.before()) {
					xmlInfo << " before=\"" << _missingElements->location.before() << "\"";
                }
				if (0 != _missingElements->location.after()) {
					xmlInfo << " after=\"" << _missingElements->location.after() << "\"";
                }
				xmlInfo << ">" << _missingElements->location.path() << "</path>\n";
			}
			ACS_COND_THROW(timeEmpty && pathEmpty, exQualityInformationException("Both location.time and location.path fields not filled!") ); // PRQA S 3081
			xmlInfo << "\t\t\t</location>\n" ;
		if (0 != _missingElements->count)
		{
			bool emptyTag = (_missingElements->over == 0);
			xmlInfo << "\t\t\t<count value=\"" << _missingElements->count << "\"";
			if (emptyTag) {
				xmlInfo << "/>\n" ;
            }
			else {
				xmlInfo << " over=\"" << _missingElements->over << "\"/>\n" ;
            }
		}
		if (!_missingElements->cause.empty())
		{
			xmlInfo << "\t\t\t<cause type=\"" << _missingElements->cause << "\"/>\n" ;
		}

		xmlInfo << "\t\t</missingElements>\n" ;
	}
	if (_corruptedElements)
	{
		xmlInfo << "\t\t<corruptedElements>\n" ;
			xmlInfo << "\t\t\t<location>\n" ;
			bool timeEmpty(true);
			bool pathEmpty(true);
			if (!_corruptedElements->location.startTime().empty() && !_corruptedElements->location.stopTime().empty())
			{
				timeEmpty = false;
				xmlInfo << "\t\t\t\t<time>\n";
				xmlInfo << "\t\t\t\t\t<start>" << _corruptedElements->location.startTime() << "</start>\n" ;
				xmlInfo << "\t\t\t\t\t<stop>"<< _corruptedElements->location.stopTime() << "</stop>\n" ;
				xmlInfo << "\t\t\t\t</time>\n";
			}
			if (!_corruptedElements->location.path().empty())
			{
				pathEmpty = false;
				xmlInfo << "\t\t\t\t<path";
				if (0 != _corruptedElements->location.preceding()) {
					xmlInfo << " preceding=\"" << _corruptedElements->location.preceding() << "\"";
                }
				if (0 != _corruptedElements->location.following()) {
					xmlInfo << " following=\"" << _corruptedElements->location.following() << "\"";
                }
				if (0 != _corruptedElements->location.before()) {
					xmlInfo << " before=\"" << _corruptedElements->location.before() << "\"";
                }
				if (0 != _corruptedElements->location.after()) {
					xmlInfo << " after=\"" << _corruptedElements->location.after() << "\"";
                }
				xmlInfo << ">" << _corruptedElements->location.path() << "</path>\n";
			}
			ACS_COND_THROW(timeEmpty && pathEmpty, exQualityInformationException("Both location.time and location.path fields not filled!") ); // PRQA S 3081
			xmlInfo << "\t\t\t</location>\n" ;
		if (0 != _corruptedElements->count)
		{
			bool emptyTag = _corruptedElements->over != 0 ;
			xmlInfo << "\t\t\t<count value=\"" << _corruptedElements->count;
			if (emptyTag) {
				xmlInfo << "\"/>\n" ;
            }
			else {
				xmlInfo << " over=\"" << _corruptedElements->over << "\"/>\n" ;
            }
		}
		if (!_corruptedElements->type.empty() || !_corruptedElements->otherType.empty() || ( 0 != _corruptedElements->severity))
		{
			xmlInfo << "\t\t\t<evidence";
			if (!_corruptedElements->type.empty()) {
				xmlInfo << " type=\"" << _corruptedElements->type << "\"";
            }
			if (!_corruptedElements->otherType.empty()) {
				xmlInfo << " otherType=\"" << _corruptedElements->otherType << "\"";
            }
			xmlInfo << " severity=\"" << _corruptedElements->severity << "\"";
			xmlInfo << "/>\n" ;
		}

		xmlInfo << "\t\t</corruptedElements>\n" ;
	}
	xmlInfo << "\t</measurementQualityInformation>\n" ;
	xmlInfo << "</xmlData>\n" ;

	setXMLStream(xmlInfo.str()) ;
}

void QualityInformation::readXmlStream(const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	size_t numObjects = rs.getArraySize("metadataSection.metadataObject"); // PRQA S 4412

	for (size_t i = 0; i < numObjects; i++)
	{
		std::ostringstream objectKey; objectKey << "metadataSection.metadataObject" << "[" << i << "]";
		
		std::string id;
		rs.getValue(objectKey.str()+".ID.value", id);
		if (id=="measurementQualityInformation")
		{
			objectKey << ".metadataWrap.";
			
			int count = 0 ;
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.count.value.value"))
			{
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.count.value.value", count);
			}
			int over = 0 ;
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.over.value"))
			{
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.over.value", over);
			}
				
			string cause("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.cause.type.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.cause.type.value", cause);
			}
            string path("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path", path);
            }
				
			string start("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.time.start")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.time.start", start);
            }
				
			string stop("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.time.stop")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.time.stop", stop);
            }
				
			int prec(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.preceding.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.preceding.value", prec);
            }
			int fol(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.following.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.following.value", fol);
            }
				
			int bef(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.before.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.before.value", bef);
            }

			int aft(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.after.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.missingElements.location.path.after.value", aft);
            }

			Location locM(path, prec, fol, bef, aft, start, stop);
			MissingElements missingElements(count, over, locM, cause);
			_missingElements = new MissingElements(missingElements);
			
			int countC = 0 ;
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.count.value.value"))
			{
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.count.value.value", countC);
			}
			int  overC = 0 ;
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.over.value"))
			{
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.over.value", overC);
			}

			string pathC("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path", pathC);
            }
				
			string startC("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.time.start")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.time.start", startC);
            }
				
			string stopC("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.time.stop")){
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.time.stop", stopC);
            }
				
			int precC(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.preceding.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.preceding.value", precC);
            }
			int folC(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.following.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.following.value", folC);
            }
				
			int befC(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.before.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.before.value", befC);
            }

			int aftC(0);
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.after.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.location.path.after.value", aftC);
            }

			Location locC(pathC, precC, folC, befC, aftC, startC, stopC);
			
			string type("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.evidence.type.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.evidence.type.value", type);
            }
			string otype("");
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.evidence.otherType.value")) {
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.evidence.otherType.value", otype);
            }

			int sev = 0 ;
			if (rs.hasKey(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.evidence.severity.value"))
			{
				rs.getValue(objectKey.str()+"xmlData.measurementQualityInformation.corruptedElements.evidence.severity.value", sev);
			}
			
			CorruptedElements corrupted(countC, overC, locC, type, otype, sev);
			
			_corruptedElements = new CorruptedElements(corrupted);
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
                msg << "QualityInformation: QualityInformation metadata content: " << '\n';
				if (0 != _missingElements->count) {
					msg << "\t\t Missing elements.count: \t"  << _missingElements->count << '\n';
                }
				if (0 != _missingElements->over) {
					msg << "\t\t Missing elements.over: \t"  << _missingElements->over << '\n';
                }
				if (!(_missingElements)->cause.empty()) {
					msg << "\t\t Missing elements.cause: \t"  << _missingElements->cause << '\n';
                }
				if (!_missingElements->location.path().empty()) {
					msg << "\t\t Missing elements.location.path: \t"  << _missingElements->location.path() << '\n';
                }
				if (!_missingElements->location.startTime().empty()) {
					msg << "\t\t Missing elements.location.time.start: \t"  << _missingElements->location.startTime() << '\n';
                }
				if (!_missingElements->location.stopTime().empty()) {
					msg << "\t\t Missing elements.location.time.stop: \t"  << _missingElements->location.stopTime() << '\n';
                }
				if (0 != _missingElements->location.preceding()) {
					msg << "\t\t Missing elements.location.time.preceding: \t"  << _missingElements->location.preceding() << '\n';
                }
				if (0 != _missingElements->location.following()) {
					msg << "\t\t Missing elements.location.time.following: \t"  << _missingElements->location.following() << '\n';
                }
				if (0 != _missingElements->location.before()) {
					msg << "\t\t Missing elements.location.time.before: \t"  << _missingElements->location.before() << '\n';
                }
				if (0 != _missingElements->location.after()) {
					msg << "\t\t Missing elements.location.time.after: \t"  << _missingElements->location.after() << '\n';
                }
					
				if (0 != _corruptedElements->count) {
					msg << "\t\t Corrupted elements.count: \t"  << _corruptedElements->count << '\n';
                }
				if (0 != _corruptedElements->over) {
					msg << "\t\t Corrupted elements.over: \t"  << _corruptedElements->over << '\n';
                }
				if (!_corruptedElements->location.path().empty()) {
					msg << "\t\t Corrupted elements.location.path: \t"  << _corruptedElements->location.path() << '\n';
                }
				if (!_corruptedElements->location.startTime().empty()) {
					msg << "\t\t Corrupted elements.location.time.start: \t"  << _corruptedElements->location.startTime() << '\n';
                }
				if (!_corruptedElements->location.stopTime().empty()) {
					msg << "\t\t Corrupted elements.location.time.stop: \t"  << _corruptedElements->location.stopTime() << '\n';
                }
				if (0 != _corruptedElements->location.preceding()) {
					msg << "\t\t Corrupted elements.location.time.preceding: \t"  << _corruptedElements->location.preceding() << '\n';
                }
				if (0 != _corruptedElements->location.following()) {
					msg << "\t\t Corrupted elements.location.time.following: \t"  << _corruptedElements->location.following() << '\n';
                }
				if (0 != _corruptedElements->location.before()) {
					msg << "\t\t Corrupted elements.location.time.before: \t"  << _corruptedElements->location.before() << '\n';
                }
				if (0 != _corruptedElements->location.after()) {
					msg << "\t\t Corrupted elements.location.time.after: \t"  << _corruptedElements->location.after() << '\n';
                }
				if (0 != _corruptedElements->severity)	 {
					msg << "\t\t Corrupted elements.evidence.severity: \t"  << _corruptedElements->severity << '\n';
                }
				if (!_corruptedElements->type.empty())	 {
					msg << "\t\t Corrupted elements.evidence.type: \t"  << _corruptedElements->type << '\n';
                }
				if (!_corruptedElements->otherType.empty())	 {
					msg << "\t\t Corrupted elements.evidence.otherType: \t"  << _corruptedElements->otherType << '\n';
                }
				ACS_LOG_DEBUG(msg.str()) ;
			ACS_CLASS_END_DEBUG
			break;
		}
	}
}


_ACS_END_NAMESPACE
