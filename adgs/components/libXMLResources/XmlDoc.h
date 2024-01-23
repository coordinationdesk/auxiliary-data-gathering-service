// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libXMLResources$

	$Log$
	Revision 1.11  2017/11/28 16:21:17  marpas
	qa warnings fixed, coding best practice applied

	Revision 1.10  2017/05/09 12:41:29  nicvac
	Threadsafe

	Revision 1.9  2016/03/03 14:19:55  nicvac
	Fixed boolalpha to bool type conversion.

	Revision 1.8  2016/01/14 11:35:25  nicvac
	Improved XPathResult handling.

	Revision 1.7  2016/01/13 17:03:44  nicvac
	S2PDGS-1394: Handle xpath functions (such as count)

	Revision 1.6  2015/12/03 19:16:06  marpas
	coding best practices application in progress

	Revision 1.5  2015/09/24 13:52:03  nicvac
	addTag and addAttr renaming

	Revision 1.4  2015/09/15 13:56:32  damdec
	addT modified to add tags without values.

	Revision 1.3  2015/09/07 10:57:53  nicvac
	Can update and create xml

	Revision 1.2  2015/09/03 16:29:53  nicvac
	Refactoring

	Revision 1.1  2015/09/03 10:14:20  nicvac
	Refactoring. Update implemented.


 */

#ifndef _XmlDoc_H_
#define _XmlDoc_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <ThreadSafe.h>

#include <memory> // PRQA S 1013

#include <set>

//*/ Prototype definition instead of header include. Prevent use of "-I/usr/include/libxml2" in *.defs.mk
//#include <libxml/xinclude.h>
class _xmlNode;
class _xmlDoc;
class _xmlXPathContext;
class _xmlNs;
//*/


_ACS_BEGIN_NAMESPACE(acs)



/**
 * \brief
 * Read, write Xml using Xpath.
 * Uses libxml2
 **/

/** Guard for the internal libxml2 doc */
class LibXmlDocGuard { // PRQA S 2109
public:
	explicit LibXmlDocGuard(_xmlDoc* internalDoc);
	~LibXmlDocGuard();
	_xmlDoc* getDocPtr() { return _internalDoc; } // PRQA S 4211
private:
	LibXmlDocGuard(); // not implemented
	LibXmlDocGuard(const LibXmlDocGuard & ); // not implemented
	LibXmlDocGuard &operator=(const LibXmlDocGuard &); // not implemented
private:
	_xmlDoc* _internalDoc;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LibXmlDocGuard)
};

typedef std::shared_ptr<LibXmlDocGuard> LibXmlDocGuardSptr;


class XmlDoc;
typedef std::shared_ptr<XmlDoc> XmlDocSptr;


/** Xml Doc Wrapper using libxml2 */
class XmlDoc  { // PRQA S 2109
public:

	/** XmlDoc Exceptions */
	exDECLARE_EXCEPTION(exXmlDocException, exException); // Base XmlDoc Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exXmlDocNotFoundException, exXmlDocException); // Item Not found. // PRQA S 2153
	exDECLARE_EXCEPTION(exXmlDocCriticalException, exXmlDocException); // Critical Exception. // PRQA S 2153

	/** Default Class constructor */
	XmlDoc();
	/** Destructor */
	virtual ~XmlDoc() throw();

	/** Utility to compose Xpath expression with no namespace specification
	 * Hint: in your code shortcut this method to build xpath expression. For example, if you want
	 *  "//Description/inv/text()" matching any namespace, use:
	 *
	 *  #define x( tag_name ) +XmlDoc::tagNoNs( #tag_name )+
	 *  string xpath = "//" x(Description) "/" x(inv) "/text()";
	 * */
	static std::string tagNoNs(const std::string& tag_name ) { return "*[local-name() = '"+tag_name+"']"; }

	/** Parameters getter */
	u_int getParameters() const;
	/** Parameters setter */
	XmlDoc& setParameters(u_int parameters);
	/** true if parameter is set */
	bool isSet( u_int parameter ) const;
	/** Switch on parameter */
	XmlDoc& setOn(u_int parameter);
	/** Switch off parameter*/
	XmlDoc& setOff(u_int parameter);

	/** Set the name to be used for the default namespace */
	XmlDoc& setDefaultNamespaceName(std::string const& defaultNamespaceName);

	/** Init with xml Filename */
	XmlDoc& initWithFile(std::string const& xmlFilename);

	/** Init with xml content as string */
	XmlDoc& initWithString(std::string const& xmlContent);

	/** Return the xml filename, if any */
	const std::string& getXmlFilename() const;



	/** Xml namespces */
	class XNamespace {
	public:
		XNamespace(): _prefix(), _href() {}
		XNamespace( const std::string& prefix, const std::string& href ): _prefix(prefix), _href(href) {}
		~XNamespace() {}
		const std::string & getPrefix() const { return _prefix; }
		const std::string & getHref() const { return _href; }
	private:
		std::string _prefix;
		std::string _href;
	};

	/** Return xml namespaces */
	const std::vector<XNamespace> getXnamespaces() const;

	/** Xpath Result Info. Cf xPath method documentation
	 * Xpath function result can be only: boolean, string, number
	 * */
	class XPathResult {
	public:
		XPathResult();
		XPathResult(const XPathResult &);
		~XPathResult() throw() ;
		XPathResult& operator=(const XPathResult &);

		void set( bool const& result );
		void set( std::string const& result );
		void set( double const& result );

		typedef enum {UNSET, BOOL, STR, NUM} Type;

		bool isSet(Type& type) const throw() ;

		void get( bool& result ) const;
		void get( std::string& result ) const;
		void get( double& result ) const;

		template < typename T > inline
		void get( T& val ) const { ACS_THROW( exXmlDocCriticalException("Wrong type requested. Allowed: [bool, string, double].")); }
	private:
		Type _type;
		bool _resBool;
		std::string _resStr;
		double _resNum;
	};

	/** Return a set of XmlDoc matching XPath
	 *  xPathResult: If requested (i.e. if passed as parameter), xPathResult will contains xpath result
	 *  	in the case the xpath expression is something like "count(...)", "max(...)", etc.
	 *  	In this case the returned set of XmlDoc is (obviously) empty.
	 * */
	std::vector< XmlDocSptr > xPath( std::string const& xPath, XPathResult* xPathResult=0 ) const;

	/** Return the XmlDoc as a string */
	std::string str() const;

	/** Set content: Update my tag content.
	 * Used for example to update a value of xml document when the instance is the result of a xPath query. */
	void setContent( std::string const& value);

	/** Unlink me from the xml doc I belong to */
	void unlink(); // PRQA S 2502

	/** Add to me a new tag.
	 * Return the XmlDoc of the new tag */
	XmlDocSptr addTag( std::string const& tagName, std::string const& tagValue="", std::string const& nsPrefix="");

	/** Add to me a new attribute.
	 * Return myself */
	XmlDoc& addAttr( std::string const& attName, std::string const& attValue);
    

    /** Insert into my root tag another xml doc.
     * To insert into a child tag, first get the tag child calling xPath on me.
     * mergeNamespaceInMyRootDoc: true  ==> the namespace declarations of added nodes are moved to my document root tag.
     *                            false ==> the namespace declarations of added nodes are moved to the tag I'm poiting to */
    void insert( XmlDocSptr const & xmlDocSptrToBeAdded, bool mergeNamespaceInMyRootDoc=true );

	/** Get a single value from xml */
	template < typename T >
	bool getValue( std::string const& xpath, T& val, bool mandatory = true ) const;

	/** Get multiple values from xml */
	template < typename T >
	bool getValue( std::string const& xpath, std::vector<T>& val, bool mandatory = true ) const;

	/** Return a XmlDoc with a customized root tag, containing nodes matching xPath */
	XmlDocSptr xPathWrap (std::string const& xPath, std::string const& rootTag="Root" ) const;

	/** return true if namespace is in the xml */
	bool checkNamespaceExists(const std::string &ns) const;

	/** Redirect libxml2 errors */
	static void redirectErrorMsg();
	/** Silent libxml2 errors messages */
	static void silentErrorMsg();


	/** Params */
	static const u_int parNone       =   0x00;  //... 0000 0000: No parameter
	static const u_int parSkipXmlHdr =   0x01;  //... 0000 0001: Skip the xml header
	static const u_int parStripCdata =   0x02;  //... 0000 0010: In the case the whole XmlDoc is '<![CDATA[value]]>' => return 'value'.
	                                            //               The whole XmlDoc can be '<![CDATA[value]]>' when for example it is obtained from a Xpath like '//tag/text()'
	static const u_int parGetAttrValue = 0x04;  //... 0000 0100: Get the attribute value
	static const u_int parGetAttrName =  0x08;  //... 0000 1000: Get the attribute name

	static const u_int parSetRootNs =    0x10;  //... 0001 0000: Set root namespaces (as setrootns for xmllint). To be used if xPath contains namespaces,
	                                            //               like in '//pds:RequestID/text()'

	/** INIT MODES **/
	/** Default init mode */
	static const u_int parDefault = parSkipXmlHdr | parStripCdata | parGetAttrValue | parSetRootNs;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomment"
	/// Faster xpath: This init mode runs xpath query faster, but queries has to be in the form:
	///  "//*[local-name() = 'RequestID']/text()"
	/// instead of
	/// "//pds:RequestID/text()".
	/// Hint: use tagNoNs() to compose xpath expression.
#pragma GCC diagnostic pop
	static const u_int parFastXpath = parSkipXmlHdr | parStripCdata | parGetAttrValue ;


private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	XmlDoc(const XmlDoc & );
	/** Operator = */
	XmlDoc &operator=(const XmlDoc &);

	/** If I'm the result of a xpath, I want behave as my father
	 * Call me when you are returning XmlDocSptr from a function */
	void setAsMyFather(XmlDoc const& father);

	/** Load namespaces
	 *  nsprefixToXmlNsPtr: From ns prefix to ns object */
	typedef std::map< std::string, _xmlNs * > Map_nsPrefix_nsPtr;
	void loadNamespaces( Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr ) const;
	void loadNamespaces_recursion( _xmlNode * a_node, Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr ) const;

	/** Implements setrootns as in xmllint */
	int  xmlRegisterRootNamespaces(_xmlXPathContext* ctxt) const;

	/** Used by insert method to update namespaces */
	void insert_namespaceSet_recursion( _xmlNode * a_node, const Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr, std::set< _xmlNs * > & nsToForget ) const;
	void insert_namespaceSetDecl_recursion( _xmlNode * a_node, const Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr, std::set< _xmlNs * > & nsToMove ) const;

	/** Implements error handling */
	static void setError(void *ctx, const char *msg, ...);
	/** Implements error handling: make them silent */
	static void silentError(void *ctx, const char *msg, ...);

private:

	std::shared_ptr<Mutex> mutex_ ;

	/**
	 * DEFINITION:
	 *  Originator: is XmlDoc instance that received the messages initWithFile, initWithString.
	 * */

	/** Main xml doc the rootNode belongs to.
	 * It's a shared pointer to the Originator's libxml2 doc.
	 * This allow the libxml2 doc to be freed when last instance of XmlDoc pointing to it dies. */
	LibXmlDocGuardSptr _originatorDocSptr;

	/** My root node. May be a pointer to a inner node of Originator. */
	_xmlNode* _rootNodePtr;

	/** Xml file, if any */
	std::string _xmlFilename;

	/** Parameters */
	u_int  _parameters;

	/** Default namespace */
	std::string _defaultNamespace;

	static Mutex mutexErrorMsg;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XmlDoc)
};



inline
void XmlDoc_conv( std::string const& from, std::string& to ) { // PRQA S 2134
	to = from;
}

inline
void XmlDoc_conv( std::string const& from, bool& to ) { // PRQA S 2134

	std::istringstream iss(from);

	if ( ! (iss >> std::boolalpha >> to) ) {
		ACS_THROW( XmlDoc::exXmlDocNotFoundException(std::string("Error converting [")+from+"] to bool type.") ); // PRQA S 3081
	}
}

template < typename T > inline
void XmlDoc_conv( std::string const& from, T& to ) {

	std::istringstream iss(from);

	if ( ! (iss >> to) ) {
		ACS_THROW( XmlDoc::exXmlDocNotFoundException(std::string("Error converting [")+from+"] to requested type.") );
	}
}


template < typename T > inline
bool XmlDoc::getValue( std::string const& xpath, T& val, bool mandatory) const { // PRQA S 4020

	try {

		XPathResult xPathResult;
		std::vector< XmlDocSptr > xmls = this->xPath( xpath, &xPathResult );

		XPathResult::Type type = XPathResult::UNSET ;
		if ( xmls.empty() && !xPathResult.isSet(type) ) {
			ACS_COND_THROW( mandatory, exXmlDocNotFoundException("Empty result matching xpath ["+xpath+"]") ); // PRQA S 3081
			return false;

		} else if ( xmls.size() > 1 ) {
			ACS_THROW( exXmlDocNotFoundException("Found more than one value matching xpath ["+xpath+"]" ) ); // PRQA S 3081
		}
		else { /* nothing */ }

		//Here if There is exactly one item or there is a result from xpath function (count, max, ...)
		if ( xPathResult.isSet(type) ) {
			xPathResult.get( val );
		} else {
			XmlDoc_conv( xmls.at(0)->str(), val );
		}

		return true;

	} catch(std::exception& ex) {
		ACS_THROW( exXmlDocNotFoundException(ex, "Error getting xml from xpath ["+xpath+"]" ) );
	}
}

template< typename T > inline
bool XmlDoc::getValue( std::string const& xpath, std::vector<T> & val, bool mandatory) const {

	try {

		XPathResult xPathResult;
		std::vector< XmlDocSptr > xmls = this->xPath( xpath, &xPathResult );

		XPathResult::Type type;
		if ( xmls.empty() && !xPathResult.isSet(type) ) {
			ACS_COND_THROW( mandatory, exXmlDocNotFoundException("Empty result matching xpath ["+xpath+"]") );
			return false;
		}

		//Here if There are some items or there is a result from xpath function (count, max, ...)
		if ( xPathResult.isSet(type) ) {
			T v;
			xPathResult.get( val );
			val.push_back( v );

		} else {
			for ( std::vector< XmlDocSptr >::iterator it = xmls.begin(); it!=xmls.end(); ++it ) {
				T v;
				XmlDoc_conv( (*it)->str(), v );
				val.push_back( v );
			}
		}

		return true;

	} catch(std::exception& ex) {
		ACS_THROW( exXmlDocNotFoundException(ex, "Error getting xml from xpath ["+xpath+"]" ) );
	}

}


_ACS_END_NAMESPACE

#endif //_XmlDoc_H_

