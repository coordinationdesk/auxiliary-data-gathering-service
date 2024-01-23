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

	$Prod: A.C.S. XMLResources Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/05/26 16:37:40  marfav
	Thread safety improved
	Destructors performs try-catch and notify
	hpTrimmedString robustness improved
	
	Revision 5.1  2014/02/13 12:42:29  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.5  2011/03/01 12:09:34  marpas
	GCC 4.4.x support
	
	Revision 2.4  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.3  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.2  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/08/08 12:48:31  marpas
	diagnostics and realted tests improved
	
	Revision 1.2  2005/08/08 12:01:44  marpas
	bug fixed now works even for long tranlation sessions
	
	Revision 1.1  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	
	
*/

#include <XMLFilterHandler.h>
#include <XMLDataConsumer.h>
#include <Filterables.h>


#include <xercesc/sax/AttributeList.hpp> // PRQA S 1013 2
#include <xercesc/util/XMLUniDefs.hpp>

#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef XERCES_HAS_NAME_SPACE
using namespace XERCES_CPP_NAMESPACE;
#endif

namespace { // unnamed
// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
//XERCES_CPP_NAMESPACE::
const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
const XMLCh  gEndPI[] = { chQuestion, chCloseAngle, chNull };
const XMLCh  gStartPI[] = { chOpenAngle, chQuestion, chNull };
const XMLCh  gXMLDecl1[] =
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chDigit_1, chPeriod
    ,   chDigit_0, chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};

const XMLCh  gXMLDecl2[] =
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chLF, chNull
};
} // end namespace 

_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLFilterHandler)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLFilterHandler::XMLFilterHandler(
	                                std::vector<XMLDataConsumer *> &dc, 
	                                const std::string &path,
	                                const std::string &encodingName,
	                                XMLFormatter::UnRepFlags unRepFlags) : 
	XMLExtractorHandlerBase(dc, false),
    XERCES_CPP_NAMESPACE::XMLFormatTarget(),
	fFormatter(encodingName.c_str(),this,XMLFormatter::NoEscapes,unRepFlags),
	_out(path.c_str()),
	_tmpout(0)

{
	if (!_out.good()) {
		ACS_THROW(exIOException("Output path \"" + path + "\" problems.", errno)) ; // PRQA S 3081
	}
	
    //
    //  Go ahead and output an XML Decl with our known encoding. This
    //  is not the best answer, but its the best we can do until we
    //  have SAX2 support.
    //
    fFormatter << gXMLDecl1 << fFormatter.getEncodingName() << gXMLDecl2;
}

XMLFilterHandler::~XMLFilterHandler() throw() 
{
	delete _tmpout ; // PRQA S 4631
}

//
// notify start of an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 )
	void XMLFilterHandler::startElement(const XMLCh* const name, XERCES_CPP_NAMESPACE::AttributeList& attributes) // PRQA S 4020
 #else
	void XMLFilterHandler::startElement(const XMLCh* name, XERCES_CPP_NAMESPACE::AttributeList& attributes) // PRQA S 4020
 #endif

{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		string t ;
    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found element: " << hpTrimmedString(name,t)) ;
	ACS_CLASS_END_DEBUG
	if (_endOfExtraction) {
		writeStart(name, attributes) ;
		return ;
	}

	string en ;
	hpTrimmedString(name,en) ;
	_interestedConsumers.clear() ;
	bool anyInterested = false ;
    size_t dcon_size = _dataConsumer.size() ;
	for (size_t i=0; i < dcon_size; ++i) {
		if (_dataConsumer[i]->nodeMatch(*this, _currentNode,en,attributes, name)) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "intcons:" << _dataConsumer[i]) ;
			if (find(_interestedConsumers.begin(),_interestedConsumers.end(), _dataConsumer[i]) == _interestedConsumers.end()) {
				_interestedConsumers.push_back(_dataConsumer[i]) ;
            }
			anyInterested = true ;
		}
	}
	_parentNode = _currentNode ;
	_currentNode += "/" ;
	_currentNode += en ;
	_startNewEl = true ;
	
	if (!anyInterested) {
		writeStart(name, attributes) ;
	}
}

void XMLFilterHandler::writeStart(const XMLCh* name, const XERCES_CPP_NAMESPACE::AttributeList& attributes)
{
    // The name has to be representable without any escapes
    fFormatter  << XMLFormatter::NoEscapes
                << chOpenAngle << name;

    size_t len = attributes.getLength();
    for (size_t index = 0; index < len; ++index)
    {
        //
        //  Again the name has to be completely representable. But the
        //  attribute can have refs and requires the attribute style
        //  escaping.
        //
        fFormatter  << XMLFormatter::NoEscapes
                    << chSpace << attributes.getName(index)
                    << chEqual << chDoubleQuote
                    << XMLFormatter::AttrEscapes
		            << attributes.getValue(index)
                    << XMLFormatter::NoEscapes
                    << chDoubleQuote;
    }
    fFormatter << chCloseAngle;
}

//
// notify end of an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 )
	void XMLFilterHandler::endElement(const XMLCh* const name) // PRQA S 4020
 #else
	void XMLFilterHandler::endElement(const XMLCh* name) // PRQA S 4020
 #endif
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		string t ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "element: " << hpTrimmedString(name,t)) ;
	ACS_CLASS_END_DEBUG
	if (_endOfExtraction) {
    	// No escapes are legal here
    	fFormatter << XMLFormatter::NoEscapes << gEndElement << name << chCloseAngle;
		return ;
	}

	string en ;
	hpTrimmedString(name,en) ;
	if (!_startNewEl) {
		_parentNode.erase(_parentNode.rfind(en)-1) ; // PRQA S 3084
	}
	bool someoneWroteEnd = false ;
	if (!_interestedConsumers.empty()) {
        size_t ic_size = _interestedConsumers.size() ;
		for (size_t i=0; i < ic_size; ++i) {
			if (_interestedConsumers[i]->nodeValue(*this, _parentNode,en,_currentValue, name)) {
				someoneWroteEnd = true ;
            }
        }
		_currentValue = "" ;
	}
	_currentNode.erase(_currentNode.rfind(en)-1) ; // PRQA S 3084
	_startNewEl = false ;


	if (!someoneWroteEnd) {
    	// No escapes are legal here
    	fFormatter << XMLFormatter::NoEscapes << gEndElement << name << chCloseAngle;
	}
}


void XMLFilterHandler::writeEnd(const XMLCh* name)
{
	// No escapes are legal here
	fFormatter << XMLFormatter::NoEscapes << gEndElement << name << chCloseAngle;
}

//
// notify characters data into an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 )
	void XMLFilterHandler::characters(const XMLCh* const chars, const XMLSize_t lenght) // PRQA S 4020
 #else
	void XMLFilterHandler::characters(const XMLCh* chars, const unsigned int lenght) // PRQA S 4020
 #endif
{
	if (_endOfExtraction) {
		fFormatter.formatBuf(chars, lenght, XMLFormatter::CharEscapes);
		return ;
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "'" << toString(chars,t,!_currentValue.empty()) << "'");
	ACS_CLASS_END_DEBUG
	if (!_interestedConsumers.empty()) {
		string c ;
		_currentValue += toString(chars,c,!_currentValue.empty()) ;
	}
	
	bool someoneWrote = false ; 
	if (!_interestedConsumers.empty()) {
        size_t ic_size = _interestedConsumers.size() ;
		for (size_t i=0; i < ic_size; ++i)  {
			if (_interestedConsumers[i]->characters(*this, chars, lenght)) {
				someoneWrote = true ; 
            }
        }
				
	}
	if (!someoneWrote) {
   		fFormatter.formatBuf(chars, lenght, XMLFormatter::CharEscapes);
    }
}

 #if ( XERCES_VERSION_MAJOR > 2 )
void XMLFilterHandler::formatBuf(const XMLCh* chars, const XMLSize_t lenght) 
 #else
void XMLFilterHandler::formatBuf(const XMLCh* chars, const unsigned int lenght) 
 #endif
{
	fFormatter.formatBuf(chars, lenght, XMLFormatter::CharEscapes);
}



void XMLFilterHandler::formatBuf(const std::string &s) 
{
	fFormatter.formatBuf(XMLString::transcode(s.c_str()), s.length(), XMLFormatter::CharEscapes);
}

void XMLFilterHandler::writeChars(const XMLByte* const )
{
}

#if ( XERCES_VERSION_MAJOR > 2 )
void XMLFilterHandler::writeChars(const XMLByte* toWrite,
                                  XMLSize_t count,
                                  XMLFormatter* )
#else
void XMLFilterHandler::writeChars(const XMLByte* const toWrite,
                                  const unsigned int count,
                                  XMLFormatter* const formatter)
#endif
{
    // For this one, just dump them to the standard output
    // Surprisingly, Solaris was the only platform on which
    // required the char* cast to print out the string correctly.
    // Without the cast, it was printing the pointer value in hex.
    // Quite annoying, considering every other platform printed
    // the string with the explicit cast to char* below.
  	_out.write(reinterpret_cast<const char *>(toWrite), count); // PRQA S 3000, 3030, 3081
	if (!_out.good()) {
		ACS_THROW(exIOException("Cannot write to output file.", errno)) ; // PRQA S 3081
	}
	_out.flush();
}



//
// receive notification of the beginning of the document
//
void XMLFilterHandler::startDocument()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "XML document begins") ;
	_interestedConsumers.clear() ;
	_currentValue = "" ;
	_currentNode = "" ;
	_startNewEl = false ;
}

//
// receive notification of the end of the document
//
void XMLFilterHandler::endDocument()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "XML document ends") ;
	_interestedConsumers.clear() ;
	_currentValue = "" ;
	_startNewEl = false ;
}

void XMLFilterHandler::ignorableWhitespace(const   XMLCh* const chars,  XMLSize_t length)
{
    fFormatter.formatBuf(chars, length, XMLFormatter::NoEscapes);
}



void XMLFilterHandler::processingInstruction(const  XMLCh* const target, const XMLCh* const data)
{
    fFormatter << XMLFormatter::NoEscapes << gStartPI  << target;
    if (data) {
        fFormatter << chSpace << data;
    }
    fFormatter << XMLFormatter::NoEscapes << gEndPI;
}


void XMLFilterHandler::writeTmpOut() 
{
	if (_tmpout) {
		_out << _tmpout->str() ;
		delete _tmpout ;
		_tmpout= new ostringstream ;
	}
}


_ACS_END_NAMESPACE

