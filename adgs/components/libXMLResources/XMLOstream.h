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
	Revision 5.9  2014/05/26 09:27:37  marpas
	xml namespace support added to XMLOstream class.
	xml::Ns and xml::NoNs convenience classes added.
	xml::Tag class supports namespaces.
	code simplified
	
	Revision 5.8  2014/05/23 06:49:30  marpas
	redundant type definitions housekeeping - using typedef to keep backward compatibility
	
	Revision 5.7  2014/05/23 05:30:39  marpas
	introducing xml::Tag class and related test
	
	Revision 5.6  2014/05/15 12:13:19  marpas
	minor changes
	
	Revision 5.5  2014/05/15 12:09:56  marpas
	function added in namespace acs:
	template <typename T>
	XMLOstream & operator<< (XMLOstream &, const T &) ;
	this is a starting point (to be re implemented) to allow code like this:
	  ...
	  MyStruct myStruct ;
	  XMLOstream xst(...);
	  xst << myStruct ;
	  ...
	
	Revision 5.4  2014/02/13 15:50:12  marpas
	qa warnings
	
	Revision 5.3  2014/02/13 13:18:36  marpas
	attribute and values conversion performance improved when writing XML streams
	now it uses the StringUtils::transform with a map of char conversion.
	Internally class XMLOstream::Tag uses a singleton to avoid creatign conversion maps multiple times
	
	Revision 5.2  2014/02/11 18:28:14  marpas
	CDATA support added
	
	Revision 5.1  2013/10/02 18:09:56  marpas
	Tag with unsigned short fixed
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.7  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.6  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.5  2009/11/13 11:30:45  marpas
	class can be casted to an std::ostream
	
	Revision 2.4  2009/04/08 11:03:44  danalt
	added unsigned long long Tag
	
	Revision 2.3  2008/07/18 15:31:57  marpas
	method insertXML changed
	
	Revision 2.2  2007/07/13 13:41:16  marpas
	XMLOstream & XMLIstream classes now support encoding - see ctors
	
	Revision 2.1  2006/03/20 10:10:59  danalt
	added doctype parameter with default value in CTOR
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.14  2004/12/22 16:05:56  seralb
	Added unsigned int overload
	
	Revision 1.13  2004/12/06 14:09:23  marpas
	header fixed
	
	Revision 1.12  2004/09/30 09:24:09  giucas
	Changed precision in Tag ctor for double to 6
	
	Revision 1.11  2004/09/29 17:06:40  giucas
	Added int parameter to XMLOstream::Tag ctor for double values to set the precision.Default is 10.
	
	Revision 1.10  2004/08/30 09:59:52  danalt
	More constructors for Tag Class
	
	Revision 1.9  2004/06/11 07:05:01  giucas
	Added parameter to XMLOstream ctor to set attributes.Default is null string.
	
	Revision 1.8  2004/01/08 17:14:47  paoscu
	More constructors for Tag class.
	
	Revision 1.7  2004/01/07 14:37:03  paoscu
	init() method and more constructors added.
	
	Revision 1.6  2003/04/30 10:30:59  marpas
	using namespace std was removed from includes
	
	Revision 1.5  2003/03/17 17:48:54  marpas
	iinsertXML method and a new constructor added
	
	Revision 1.4  2003/02/07 14:49:11  paoscu
	closeInLine option added in Tag constructor.
	
	Revision 1.3  2002/12/13 11:35:32  giucas
	Added close method to XMLOstream class
	
	Revision 1.2  2002/12/03 14:35:23  paoscu
	Now XMLOstream can handle illegal chars.
	
	Revision 1.1  2002/11/26 16:52:08  paoscu
	Imported.
	

*/


#ifndef _XMLOstream_H_
#define _XMLOstream_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>
#include <StringUtils.h>
#include <map>
#include <stack>

_ACS_BEGIN_NAMESPACE(acs)
_ACS_BEGIN_NAMESPACE(xml)

	class Tag ; // fwd declaration

    class CharConv { // PRQA S 2109
    public:
	    CharConv() : attributeConversionMap_(), valueConversionMap_() 
        { 
            attributeConversionMap_['&'] = "&amp;" ;  
            attributeConversionMap_['<'] = "&lt;" ;   
            attributeConversionMap_['>'] = "&gt;" ;   
            attributeConversionMap_['\''] = "&apos;" ; 
            
            valueConversionMap_ = attributeConversionMap_ ;
            valueConversionMap_['\"'] = "&quot;" ;   
        } 
        
	    ~CharConv() throw() { }
	    const std::map< char, std::string > & getAttributeMap() const throw() { return attributeConversionMap_ ; }
	    const std::map< char, std::string > & getValueMap() const throw() { return valueConversionMap_ ; } 
    private:
	    CharConv (const CharConv &) ;
	    CharConv& operator=(const CharConv &) ;
    private:
        std::map< char, std::string > attributeConversionMap_ ;
        std::map< char, std::string > valueConversionMap_ ;

    } ;
    
    typedef acs::pattern::Singleton<CharConv> CharConvSingleton ;

    class CData {
    public:
        CData() : cdata_() {}
        explicit CData(const std::string &s) : cdata_(s) {}
        CData(const CData &r) : cdata_(r.cdata_) {}
        CData & operator=(const CData &r) {
            if (this != &r) { cdata_ = r.cdata_ ; }
            return *this ;
        }
		~CData() throw() {}
        const std::string &cdata() const throw() { return cdata_ ; }
    private:
        std::string cdata_ ;
    } ;
    
_ACS_END_NAMESPACE

class XMLOstream // PRQA S 2109
{
    friend class xml::Tag ; // PRQA S 2107

public:
    typedef xml::CData CData ; // for code that used previous XMLOstream::CData type - tbr in future
    typedef xml::Tag Tag ; // for code that used previous XMLOstream::Tag type - tbr in future

	XMLOstream(std::ostream & os, std::string const & root, std::string const & attributes="", std::string const & doctype="", std::string const & encoding="UTF-8");
	// no root, neither initialization
	explicit XMLOstream(std::ostream & os);
	~XMLOstream() throw() ;

	void close(); // PRQA S 2502
	// inserts a stream inside an in-progress XML. Note, the method only cares for right indentation,
	// no other assumptions are made over the input stream (e.g. if is is a well formed XML). 
	void insertXML(std::istream &, bool removeXmlHeader = false) ;

	operator std::ostream &() ; // PRQA S 2181
    void pushNs(const std::string &) ; 
    void popNs(const std::string &) throw() ;
    std::string currentNs() const ;
private:
	//Not implemented
	XMLOstream();
	XMLOstream(XMLOstream const &);
	XMLOstream& operator=(XMLOstream const &);

    void inc_indent() { ++ _tabCount ; }
    void dec_indent() { -- _tabCount ; }
    void indent() ;
    
private:
    static const std::string tab_ ;
	std::ostream & _os;
	unsigned _tabCount;
	std::string _root;
	bool _closeRoot;
    std::stack<std::string> ns_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLOstream) ;
};

/* NB to be reimplemented for each specific aggregate type
 */ 
 
template <typename T>
XMLOstream & operator<< (XMLOstream &, const T &) ; // PRQA S 2072

_ACS_BEGIN_NAMESPACE(xml)

    /*- convenience class to set scoped xml namespace */
    class Ns { // PRQA S 2109
    public:
        Ns(XMLOstream &, const std::string &ns) ;
		virtual ~Ns() throw() ;
        const std::string &ns() const throw() { return ns_ ; }
    private:
        Ns() ; // not implemented
        Ns(const Ns &) ; // not implemented
        Ns& operator=(const Ns &) ; // not implemented
    private:
        XMLOstream &os_ ;
        std::string ns_ ;
    } ;

    /*- convenience class to set scoped empty namespaces */
    class NoNs : public Ns { // PRQA S 2153
    public:
        explicit NoNs(XMLOstream & os) : Ns(os,"") {}
		virtual ~NoNs() throw() {} // PRQA S 2131
    private:
        NoNs() ; // not implemented
        NoNs(const NoNs &) ; // not implemented
        NoNs& operator=(const NoNs &) ; // not implemented
    } ;

	class Tag // PRQA S 2109
	{
	public:
		Tag(XMLOstream &, std::string const & name, CData const & value, std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, std::string const & value="", std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, bool value,                  std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, int value,                   std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, unsigned short value,         std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, unsigned int value,           std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, long value,                  std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, long long value,             std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, unsigned long value,          std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, unsigned long long value,     std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, char const * value,           std::string const & attributes="", bool closeInLine=false);
		Tag(XMLOstream &, std::string const & name, double value,                std::string const & attributes="", bool closeInLine=false, int precision=6);

        template <typename T>
		Tag(XMLOstream &stream, 
            std::string const & name, 
            T const & value, 
            std::string const & attributes="") :
                        _stream(stream),    // PRQA S 2528
                        _name(computeQualifiedName(_stream, name)), 
                        _toClose(true)
        {
	        init(value, attributes);
        }

		~Tag() throw() ;
		void close() ; // PRQA S 2502
	private:
		
		Tag();                        //Not implemented
		Tag(Tag const &);              //Not implemented
		Tag& operator=(Tag const &);    //Not implemented

        static std::string computeQualifiedName(XMLOstream &, const std::string &n) ;

		void init(std::string const & value, std::string const & attributes, bool closeInLine, bool isCData);
        
        template<typename T>
		void init(T const & value, std::string const & attributes)
        {

	        _stream.inc_indent() ;

            _stream.indent() ;

	        _stream._os<<"<"<<_name;
	        if(not attributes.empty()) {	
		        _stream._os<<" "<< StringUtils::transform(attributes, CharConvSingleton::instance()->getAttributeMap()) ;
	        }
	        _stream._os << ">";

	        _stream._os<< "\n";
    
            _stream << value ;
       }

	private:
		XMLOstream & _stream;
		std::string _name;
		bool _toClose;
	};

_ACS_END_NAMESPACE

_ACS_END_NAMESPACE




#endif //_XMLOstream_H_
