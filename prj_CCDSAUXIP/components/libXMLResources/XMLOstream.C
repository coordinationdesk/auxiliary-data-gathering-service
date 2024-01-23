// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. XMLResources Library $

	$Id$

	$Author$

	$Log$
	Revision 5.9  2015/03/03 09:59:37  marpas
	fixing possible infinite loop in XMLOStream::insertXML when inserting lines are very long (> 65535).
	test function added
	
	Revision 5.8  2014/06/23 13:11:47  marpas
	namespace fixed - test improved
	
	Revision 5.7  2014/05/26 09:27:37  marpas
	xml namespace support added to XMLOstream class.
	xml::Ns and xml::NoNs convenience classes added.
	xml::Tag class supports namespaces.
	code simplified
	
	Revision 5.6  2014/05/23 06:49:30  marpas
	redundant type definitions housekeeping - using typedef to keep backward compatibility
	
	Revision 5.5  2014/05/23 05:30:39  marpas
	introducing xml::Tag class and related test
	
	Revision 5.4  2014/02/13 13:18:36  marpas
	attribute and values conversion performance improved when writing XML streams
	now it uses the StringUtils::transform with a map of char conversion.
	Internally class XMLOstream::Tag uses a singleton to avoid creatign conversion maps multiple times
	
	Revision 5.3  2014/02/11 18:28:14  marpas
	CDATA support added
	
	Revision 5.2  2013/10/02 18:09:55  marpas
	Tag with unsigned short fixed
	
	Revision 5.1  2013/07/19 09:01:20  marpas
	doc fixed
	qa warnings fixed
	compilation warnings fixed
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.10  2013/03/26 16:37:31  marpas
	minor changes
	
	Revision 2.9  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.8  2011/03/01 12:09:34  marpas
	GCC 4.4.x support
	
	Revision 2.7  2009/11/13 11:30:44  marpas
	class can be casted to an std::ostream
	
	Revision 2.6  2009/04/08 11:03:43  danalt
	added unsigned long long Tag
	
	Revision 2.5  2008/07/18 15:31:57  marpas
	method insertXML changed
	
	Revision 2.4  2008/02/28 13:40:35  lucio.pulvirenti
	Tags with attributes and no values are closed just after
	the attribute if closeInline = true (e.g. <myTag MyAttribute=OnlyAttributeNoValueClosedInline/>)
	
	Revision 2.3  2007/07/13 13:41:16  marpas
	XMLOstream & XMLIstream classes now support encoding - see ctors
	
	Revision 2.2  2007/06/19 09:48:48  marpas
	default encoding is UTF-8
	applied even when geerating a new file
	
	Revision 2.1  2006/03/20 10:10:59  danalt
	added doctype parameter with default value in CTOR
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.21  2005/09/21 13:47:09  marpas
	line buffer extended to 64K (65536) char - previously was 4K
	
	Revision 1.20  2005/01/11 20:02:31  marpas
	attributes special chars are now converted according with XML specs
	
	Revision 1.19  2004/12/22 16:05:56  seralb
	Added unsigned int overload
	
	Revision 1.18  2004/12/07 17:00:13  marpas
	debug messages improved
	
	Revision 1.17  2004/12/06 14:09:22  marpas
	header fixed
	
	Revision 1.16  2004/09/29 17:06:40  giucas
	Added int parameter to XMLOstream::Tag ctor for double values to set the precision.Default is 10.
	
	Revision 1.15  2004/08/30 09:59:57  danalt
	More constructors for Tag Class
	
	Revision 1.14  2004/06/11 07:05:17  giucas
	GC:Added parameter to XMLOstream ctor to set attributes.Default is null string.
	
	Revision 1.13  2004/01/08 17:14:47  paoscu
	More constructors for Tag class.
	
	Revision 1.12  2004/01/07 14:37:03  paoscu
	init() method and more constructors added.
	
	Revision 1.11  2003/04/30 10:30:59  marpas
	using namespace std was removed from includes
	
	Revision 1.10  2003/04/15 18:08:02  marpas
	GCC3.2
	
	Revision 1.9  2003/03/17 17:48:54  marpas
	iinsertXML method and a new constructor added
	
	Revision 1.8  2003/03/11 13:40:53  marpas
	some string::lenght substituted with string::empty
	
	Revision 1.7  2003/02/13 13:35:08  paoscu
	A bug fixed.
	
	Revision 1.6  2003/02/11 17:19:17  paoscu
	tab bug fixed.
	
	Revision 1.5  2003/02/07 14:49:11  paoscu
	closeInLine option added in Tag constructor.
	
	Revision 1.4  2002/12/13 11:35:29  giucas
	Added close method to XMLOstream class
	
	Revision 1.3  2002/12/03 14:35:23  paoscu
	Now XMLOstream can handle illegal chars.
	
	Revision 1.2  2002/11/29 15:47:21  paoscu
	Now XML tag closure is correct.
	
	Revision 1.1  2002/11/26 16:52:08  paoscu
	Imported.
	

*/


#include <XMLOstream.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <RegExp.h>
#include <cstring>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLOstream) 

const string XMLOstream::tab_ = "   " ;

XMLOstream::XMLOstream(std::ostream & os, 
                       std::string const & root, 
                       std::string const & attributes, 
                       std::string const & doctype, 
                       std::string const & encoding) : 
    _os(os) , // PRQA S 2528
    _tabCount(0) , 
    _root(root), 
    _closeRoot(false),
    ns_() 
{
	_os<<"<?xml version =\"1.0\" encoding=\"" << encoding << "\"?>\n" ;
	if(!doctype.empty()) {
		_os<<"<!DOCTYPE " << _root  << " SYSTEM \"" << doctype << "\">\n";
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "root: \"" << _root << "\""
			   << " attributes: \"" << attributes << "\"") ;
	if (!_root.empty()) {
		_closeRoot=true ;
		_os<<"<"<<_root;
		if(!attributes.empty()) {
			_os<<" "<<attributes;
        }
		_os<<">\n" ;
	}
}


XMLOstream::XMLOstream(std::ostream & os) :
    _os(os) , // PRQA S 2528
    _tabCount(0), 
    _root(), 
    _closeRoot(false),
    ns_() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "root: \"" << _root << "\"") ;
}


XMLOstream::~XMLOstream() throw() 
{
    try {
	    close(); // PRQA S 4631
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }
}

void XMLOstream::close()
{
	if (_closeRoot)
	{ 
		_os<<"</"<<_root<<">\n";
		_closeRoot=false;
	}
}

void XMLOstream::insertXML(std::istream &in, bool removeXmlHeader) 
{
	RegExp re;
	bool foundAndRemoved = true ;
	if (removeXmlHeader) {
		re.setReg( "<?xml.*>[\n]*" );
		foundAndRemoved = false ;
	}
	inc_indent() ;
	do {
        string line ;
		getline(in, line) ;
		if (!foundAndRemoved) {
			regoff_t beg = 0 ; 
            regoff_t end = 0 ;
			if (re.match(line, beg, end)) {
				foundAndRemoved = true ;
				line = line.substr(end) ; // PRQA S 3000
			}
		}
		if (not line.empty()) {
            indent() ;
            _os << line << "\n" ;
		}
	} while (!in.eof())  ;
	dec_indent() ;
}

XMLOstream::operator std::ostream&()
{
	return _os ; // PRQA S 4024
}


void XMLOstream::indent() 
{
	for(size_t i=0 ; i < _tabCount ; ++i) {
		_os << tab_;
    }
}


void XMLOstream::pushNs(const std::string &ns) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ns: \"" << ns << "\"") ;
    ns_.push(ns) ;
}

void XMLOstream::popNs(const std::string &ns) throw() // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ns: \"" << ns << "\"") ;
    if (ns_.empty()) {
        ACS_LOG_WARNING(exDebugSignatureThread << " popping ns: \"" << ns << "\" from empty stack") ;
        return ;
    } 
    if (ns_.top() != ns) {
        ACS_LOG_WARNING(exDebugSignatureThread << " popping ns: \"" << ns << "\" but top is \"" << ns_.top() <<'\"') ;
        return ;
    }
    ns_.pop() ;
}

string XMLOstream::currentNs() const // PRQA S 4020
{
    if (ns_.empty()) {
        return "" ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ns: \"" << ns_.top() << "\"") ;
    
    return ns_.top() ;
    
}

_ACS_BEGIN_NAMESPACE(xml)

Ns::Ns(XMLOstream &os, const std::string &ns) :
    os_(os), // PRQA S 2528
    ns_(ns)
{
    os_.pushNs(ns_) ;
}

Ns::~Ns() throw() 
{
    os_.popNs(ns_) ;
}




Tag::Tag(
         XMLOstream & stream, 
         std::string const & name,
         CData const & value, 
         std::string const & attributes, 
         bool closeInLine) :
    _stream(stream),  // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	init(value.cdata(), attributes, closeInLine, true );
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name,
         std::string const & value, 
         std::string const & attributes, 
         bool closeInLine) :
    _stream(stream),  // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	init(value, attributes, closeInLine, false );
}


Tag::Tag(
         XMLOstream & stream, 
         std::string const & name,
         char const * value, 
         std::string const & attributes, 
         bool closeInLine) :
    _stream(stream), // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	init(string(value), attributes, closeInLine, false ); // PRQA S 3081
}





Tag::Tag(
         XMLOstream & stream, 
         std::string const & name,
         bool value, 
         std::string const & attributes, 
         bool closeInLine) :
    _stream(stream),  // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	init(value ? "t" : "f", attributes, closeInLine, false ); // PRQA S 3380
}


Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         int value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         unsigned int value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         unsigned short value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         long value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         long long value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         unsigned long value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         unsigned long long value, 
         std::string const & attributes, 
         bool closeInLine):
    _stream(stream),   // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< value;
	init(os.str() , attributes, closeInLine, false );

}

Tag::Tag(
         XMLOstream & stream, 
         std::string const & name, 
         double value, 
         std::string const & attributes, 
         bool closeInLine, 
         int precision):
    _stream(stream),    // PRQA S 2528
    _name(computeQualifiedName(_stream, name)), 
    _toClose()
{
	ostringstream os;
	os<< setprecision(precision) << value;
	init(os.str() , attributes, closeInLine, false );
	
}

Tag::~Tag() throw() 
{
    try {
	    if(_toClose) { close(); } // PRQA S 4631
    }
    catch( exception &x ) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }
}


string Tag::computeQualifiedName(XMLOstream &os, const std::string &n) // PRQA S 4020
{
    const string &ns = os.currentNs() ;
    if (ns.empty()) {
        return n ;
    }
    else {
        return ns + ':' + n; 
    }
}

void Tag::init(std::string const & value, 
               std::string const & attributes, 
               bool closeInLine,
               bool isCData)
{
	_toClose=true;

	_stream._tabCount++;
	
    _stream.indent() ;

	_stream._os<<"<"<<_name;
	if(not attributes.empty()) {	
		_stream._os<<" "<< StringUtils::transform(attributes, xml::CharConvSingleton::instance()->getAttributeMap()) ;
		if( closeInLine && value.empty() ) {
			_stream._os << "/";
        }
	}
	_stream._os << ">";

	if(!closeInLine) {
		_stream._os<< "\n";
    }

	if(not value.empty())
	{
		if( not closeInLine) { _stream.indent() ; }
			
		string correctedValue;
        if (not isCData) {
            correctedValue = StringUtils::transform(value, xml::CharConvSingleton::instance()->getValueMap()) ;
        }else {
            correctedValue=value ;
        }
        
		if(closeInLine) {
            if (isCData) { _stream._os << "<![CDATA[" ; }
			_stream._os << correctedValue ;
            if (isCData) { _stream._os << "]]>" ; }
        }
		else {

            if (isCData) { 
                _stream._os << XMLOstream::tab_ << "<![CDATA[\n" ; 
            	_stream.indent() ;
            }

			_stream._os << XMLOstream::tab_ << correctedValue << "\n";

            if (isCData) { 
                _stream.indent() ;
                _stream._os << XMLOstream::tab_ << "]]>\n" ; 
            }
        }
	}

	if( closeInLine )
	{
		// if only attribute is valorized and value is not, do not write the tag name at the end 
        // empty Cdata are conform: no CDATA !
		if( !attributes.empty() && value.empty() ) {
			_stream._os<<"\n";
        }
		else {
			_stream._os<<"</"<<_name<<">\n" ;
        }
		
		_stream.dec_indent() ;
		_toClose=false;	
	}


}

void Tag::close() // PRQA S 4020
{
	if(!_toClose) {
		return;
    }
    _stream.indent() ;
	_stream._os << "</" << _name<< ">\n" ;
	_stream.dec_indent() ;
	_toClose=false;

}
_ACS_END_NAMESPACE


_ACS_END_NAMESPACE

