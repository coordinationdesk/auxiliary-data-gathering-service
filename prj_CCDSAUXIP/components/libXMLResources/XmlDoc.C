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

	$Id$

	$Author$

	$Log$
	Revision 1.17  2017/12/12 14:14:46  lucio.pulvirenti
	S3EVOL-101: xmlRegisterRootNamespaces default parameters added to check existence of input namespace (if any).  checkNamespaceExists method added.

	Revision 1.16  2017/11/28 16:22:10  marpas
	qa warnings fixed; coding best practice application in progress

	Revision 1.15  2017/05/09 12:41:29  nicvac
	Threadsafe

	Revision 1.14  2016/01/28 14:46:59  davide.tiriticco
	removing unused parameters name in functions

	Revision 1.13  2016/01/14 11:35:25  nicvac
	Improved XPathResult handling.

	Revision 1.12  2016/01/13 17:03:44  nicvac
	S2PDGS-1394: Handle xpath functions (such as count)

	Revision 1.11  2015/12/03 19:16:06  marpas
	coding best practices application in progress

	Revision 1.10  2015/11/16 17:14:30  nicvac
	xpath and logical root node. Improved.

	Revision 1.9  2015/11/13 17:03:29  nicvac
	Ultimate solution to apply xpath starting from the logical root element.

	Revision 1.8  2015/10/29 12:12:48  nicvac
	Root tag in Xpath patch

	Revision 1.7  2015/10/28 19:51:11  nicvac
	Patch for root tag in xpath.

	Revision 1.6  2015/10/08 17:03:32  nicvac
	Fixed xpath: xpath is relative to _rootNodePtr->parent

	Revision 1.5  2015/09/25 15:48:52  nicvac
	Fixed xpath: xpath is relative to _rootNodePtr.

	Revision 1.4  2015/09/24 13:52:03  nicvac
	addTag and addAttr renaming

	Revision 1.3  2015/09/15 13:56:31  damdec
	addT modified to add tags without values.

	Revision 1.2  2015/09/07 10:57:53  nicvac
	Can update and create xml

	Revision 1.1  2015/09/03 10:14:20  nicvac
	Refactoring. Update implemented.


 */

#include <XmlDoc.h>

#include <Filterables.h>

#include <libxml/xpath.h> // PRQA S 1013 4
#include <libxml/xpathInternals.h>
#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>

#include <cstdarg>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LibXmlDocGuard)
ACS_CLASS_DEFINE_DEBUG_LEVEL(XmlDoc)


namespace {
	inline const xmlChar* toXmlChar(const char *c) { return reinterpret_cast<const xmlChar*>(c)  ; } // PRQA S 2134, 3030, 3081 3
	inline const xmlChar* toXmlChar(const string &c) { return reinterpret_cast<const xmlChar*>(c.c_str())  ; }
	inline const char * fromXmlChar(const xmlChar* xc) { return reinterpret_cast<const char*>(xc)  ; }
}

Mutex XmlDoc::mutexErrorMsg;


LibXmlDocGuard::LibXmlDocGuard(_xmlDoc* const internalDoc):
				_internalDoc(internalDoc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631 2
}

LibXmlDocGuard::~LibXmlDocGuard() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631 2
	xmlFreeDoc(_internalDoc);
}

XmlDoc::XmlDoc():
    		mutex_(  make_shared<Mutex>() ),
    		_originatorDocSptr(),
    		_rootNodePtr(0),
    		_xmlFilename(),
    		_parameters( parDefault ),
    		_defaultNamespace("ns")
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

XmlDoc::~XmlDoc() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631 2
}

XmlDoc::XPathResult::XPathResult(): _type(UNSET), _resBool(false), _resStr(), _resNum(0.0) {}
XmlDoc::XPathResult::~XPathResult() throw() {}
XmlDoc::XPathResult::XPathResult(const XPathResult &r): 
    		_type(r._type), _resBool(r._resBool), _resStr(r._resStr), _resNum(r._resNum) {}
XmlDoc::XPathResult &XmlDoc::XPathResult::operator=(const XPathResult &r)
{
	if (this != &r) {
		_type = r._type ;
		_resBool = r._resBool ;
		_resStr = r._resStr ;
		_resNum = r._resNum ;
	}
	return *this ;
}


void XmlDoc::XPathResult::set( bool const& result ) 		{ _type=BOOL; _resBool = result; }
void XmlDoc::XPathResult::set( std::string const& result ) 	{ _type=STR;  _resStr = result; }
void XmlDoc::XPathResult::set( double const& result ) 		{ _type=NUM;  _resNum = result; }

bool XmlDoc::XPathResult::isSet(Type& type) const throw() { type=_type; return type!=UNSET ;}

void XmlDoc::XPathResult::get( bool& result ) const			{
	ACS_COND_THROW( _type!=BOOL, exXmlDocNotFoundException("Boolean result not set.") ); // PRQA S 3081
	result = _resBool;
}

void XmlDoc::XPathResult::get( std::string& result ) const	{
	ACS_COND_THROW( _type!=STR, exXmlDocNotFoundException("String result not set.") ); // PRQA S 3081
	result = _resStr;
}

void XmlDoc::XPathResult::get( double& result ) const 		{
	ACS_COND_THROW( _type!=NUM, exXmlDocNotFoundException("Number result not set.") ); // PRQA S 3081
	result = _resNum;
}


const string& XmlDoc::getXmlFilename() const { // PRQA S 4120
	return _xmlFilename;
}

u_int XmlDoc::getParameters() const { // PRQA S 4120
	Lock l(*mutex_) ;

	return _parameters;
}

XmlDoc& XmlDoc::setParameters(u_int parameters) {
	Lock lock_(*mutex_);

	_parameters = parameters;

	return *this;
}

bool XmlDoc::isSet( u_int parameter ) const {
	Lock lock_(*mutex_);

	return ( parameter == (_parameters & parameter) );
}

XmlDoc& XmlDoc::setOn(u_int parameter) {
	Lock lock_(*mutex_);

	//std::bitset< sizeof(u_int) > a( _parameters ); std::cout << "before: \t" << a <<endl;
	_parameters |= parameter;
	//std::bitset< sizeof(u_int) > b( _parameters ); std::cout << "after: \t\t" << b <<endl;
	return *this;
}

XmlDoc& XmlDoc::setOff(u_int parameter) {
	Lock lock_(*mutex_);

	//std::bitset< sizeof(u_int) > a( _parameters ); std::cout << "before: \t" << a <<endl;
	if ( isSet(parameter) ) {
		_parameters ^= parameter;
	}
	//std::bitset< sizeof(u_int) > b( _parameters ); std::cout << "after: \t\t" << b <<endl;
	return *this;
}

XmlDoc& XmlDoc::setDefaultNamespaceName(string const& defaultNamespaceName) {
	Lock lock_(*mutex_);

	_defaultNamespace = defaultNamespaceName;
	return *this;
}

const vector<XmlDoc::XNamespace> XmlDoc::getXnamespaces() const {
	Lock lock_(*mutex_);

	/** List of namespace */
	std::vector< XNamespace > xnamespaces;

	Map_nsPrefix_nsPtr nsprefixToXmlNsPtr;
	this->loadNamespaces(nsprefixToXmlNsPtr);

	for ( auto p : nsprefixToXmlNsPtr ) {
		xmlNsPtr ns = p.second;

		string nsPrefix( ns->prefix? fromXmlChar(ns->prefix) : "" );
		string nsHref( fromXmlChar(ns->href) );

		XNamespace xns(nsPrefix, nsHref);
		xnamespaces.push_back( xns );
	}

	return xnamespaces;
}


//Based on:
// http://www.nco.ncep.noaa.gov/pmb/codes/nwprod/gempak/nawips2/extlibs/xml2/v2.6.31/debugXML.c
int XmlDoc::xmlRegisterRootNamespaces(_xmlXPathContext* ctxt) const // PRQA S 4020, 4214
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (ctxt == 0) {
		return -1 ;
	}

	//xmlNodePtr root = _doc->children;
	/* xmlNodePtr root = */ xmlDocGetRootElement( _originatorDocSptr->getDocPtr() );

	Map_nsPrefix_nsPtr nsprefixToXmlNsPtr;
	this->loadNamespaces(nsprefixToXmlNsPtr);

	for ( auto p: nsprefixToXmlNsPtr ) {
		xmlNsPtr ns = p.second;

		if (ns->prefix != 0) {
			xmlXPathRegisterNs(ctxt, ns->prefix, ns->href);
		} else {
			xmlXPathRegisterNs(ctxt, toXmlChar(_defaultNamespace), ns->href);
		}
	}

	return 0 ;
}

void XmlDoc::loadNamespaces_recursion( _xmlNode * a_node, Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr ) const {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			xmlNsPtr ns = cur_node->nsDef;
			while (ns != 0) {

				string nsPrefix( ns->prefix? fromXmlChar(ns->prefix) : "" );
				string nsHref( fromXmlChar(ns->href) );

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ns prefix: ["<< nsPrefix<<"]; ns href: ["<< nsHref <<"]"); // PRQA S 3380, 3385

				nsprefixToXmlNsPtr[ nsPrefix ] = ns;

				ns = ns->next;
			}
		}

		this->loadNamespaces_recursion(cur_node->children, nsprefixToXmlNsPtr);
	}
}

void XmlDoc::loadNamespaces( Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr ) const {

	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nsprefixToXmlNsPtr.clear();

	//xmlNodePtr root = _doc->children;
	xmlNodePtr root = xmlDocGetRootElement( _originatorDocSptr->getDocPtr() );

	this->loadNamespaces_recursion(root, nsprefixToXmlNsPtr);

}

XmlDoc& XmlDoc::initWithFile(string const& xmlFilename) {
	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_COND_THROW( _originatorDocSptr, // PRQA S 3050, 3081
			exXmlDocCriticalException("Internal Error.") );

	_xmlFilename = xmlFilename;

	xmlInitParser();
	_xmlDoc* doc = xmlParseFile(xmlFilename.c_str());

	ACS_COND_THROW( doc == 0, // PRQA S 3081
			exXmlDocCriticalException(xmlFilename + " is not a well-formed XML file.") );

	_originatorDocSptr.reset( new LibXmlDocGuard(doc) );
	_rootNodePtr =  xmlDocGetRootElement( _originatorDocSptr->getDocPtr() );
	//_rootNodePtr = _originatorDocSptr->getDocPtr()->children;

	return *this;
}

XmlDoc& XmlDoc::initWithString(string const& xmlContent) {
	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_COND_THROW( _originatorDocSptr, // PRQA S 3050, 3081
			exXmlDocCriticalException("Internal Error.") );

	xmlInitParser();
	_xmlDoc* doc = xmlParseDoc( toXmlChar( xmlContent.c_str() ));

	ACS_COND_THROW( doc == 0, // PRQA S 3081
			exXmlDocCriticalException("Passed content is not a well-formed XML file.") );

	_originatorDocSptr.reset( new LibXmlDocGuard(doc) );
	_rootNodePtr =  xmlDocGetRootElement( _originatorDocSptr->getDocPtr() );
	//_rootNodePtr = _originatorDocSptr->getDocPtr()->children;

	return *this;
}


void XmlDoc::setAsMyFather( XmlDoc const& father ) {

	this->_originatorDocSptr = father._originatorDocSptr; //retain libxml2 doc
	this->setParameters( father.getParameters() );
	this->setDefaultNamespaceName( father._defaultNamespace );
}

//Scope Guard utility
class XmlDoc_ScopeGuard { // PRQA S 2109
public:
	XmlDoc_ScopeGuard(xmlXPathContextPtr& xpathCtx,
			xmlXPathObjectPtr& xpathObj):
				_xpathCtx(xpathCtx), _xpathObj(xpathObj) {}
	~XmlDoc_ScopeGuard() {
		if (_xpathCtx) { xmlXPathFreeContext(_xpathCtx);  } // PRQA S 4631 2
		if (_xpathObj) { xmlXPathFreeObject(_xpathObj); }
	}
private:
	XmlDoc_ScopeGuard() ; // not implemented
	XmlDoc_ScopeGuard(const XmlDoc_ScopeGuard &) ; // not implemented
	XmlDoc_ScopeGuard & operator= (const XmlDoc_ScopeGuard &) ; // not implemented
private:
	xmlXPathContextPtr& _xpathCtx;
	xmlXPathObjectPtr& _xpathObj;
};

//Update Doc to support xpath on logical root node and restore the document structure at scope exit
class XmlDoc_UpdateDocAndRestoreAtScopeExit { // PRQA S 2109
public:
	XmlDoc_UpdateDocAndRestoreAtScopeExit( 	_xmlDoc* doc, _xmlNode* logicalRootNode) :
		_doc                ( doc              ),
		_logicalRootNode	( logicalRootNode  ),
		_originalRootElement( 0 ),
		_refParent			( 0 ),
		_refPrevSibling		( 0 ),
		_refNextSibling		( 0 ),
		_toRestore(false)
{
		_originalRootElement = xmlDocGetRootElement( doc );
		_refParent = _logicalRootNode->parent;
		_refPrevSibling = _logicalRootNode->prev;
		_refNextSibling = _logicalRootNode->next;

		if ( _logicalRootNode != _originalRootElement ) {
			xmlDocSetRootElement( _doc, _logicalRootNode );
			_toRestore = true;
		}

}

	~XmlDoc_UpdateDocAndRestoreAtScopeExit() {

		if ( _toRestore ) { // PRQA S 4631 end_dtor

			//Restore the root node
			xmlDocSetRootElement( _doc, _originalRootElement );

			//Restore the node at its original place
			if ( 0 != _refPrevSibling ) {
				xmlAddNextSibling( _refPrevSibling, _logicalRootNode );

			} else if ( 0 != _refNextSibling )  {
				xmlAddPrevSibling( _refNextSibling, _logicalRootNode );

			} else {
				xmlAddChild( _refParent, _logicalRootNode );
			}
		}
		// PRQA L:end_dtor
	}
private:
	XmlDoc_UpdateDocAndRestoreAtScopeExit() ; // not implemented
	XmlDoc_UpdateDocAndRestoreAtScopeExit(const XmlDoc_UpdateDocAndRestoreAtScopeExit &) ; // not implemented
	XmlDoc_UpdateDocAndRestoreAtScopeExit & operator= (const XmlDoc_UpdateDocAndRestoreAtScopeExit &) ; // not implemented
private:
	_xmlDoc* _doc;
	_xmlNode* _logicalRootNode;
	_xmlNode* _originalRootElement;
	_xmlNode* _refParent;
	_xmlNode* _refPrevSibling;
	_xmlNode* _refNextSibling;
	bool _toRestore;
};

vector< XmlDocSptr > XmlDoc::xPath( string const& xPath, XPathResult* xPathResult ) const { // PRQA S 4214

	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Set the scope guard to prevent memory leak
	xmlXPathContextPtr xpathCtx(0);
	xmlXPathObjectPtr xpathObj(0);
	XmlDoc_ScopeGuard xmlDoc_ScopeGuard(xpathCtx, xpathObj);

	string usedXPath=xPath;

	// Create xpath evaluation context
	xpathCtx = xmlXPathNewContext(_originatorDocSptr->getDocPtr() );

	ACS_COND_THROW( xpathCtx == 0, // PRQA S 3081
			exXmlDocCriticalException("Unable to create the xpath context. Internal error.") );

	//Set root namespace if required
	if ( isSet(parSetRootNs) ) {
		this->xmlRegisterRootNamespaces( xpathCtx );
		//Return -1 for xml files without namespace. -1 ==> setrootns ignored.
	}

	//A little tricky, but XmlDocTest.C with valgrind is ok.
	//Here set the _rootNodePtr as the root of the document to use xpath on your "logical" root
	//At scope exit the document's structure will be restored.
	XmlDoc_UpdateDocAndRestoreAtScopeExit xmlDoc_UpdateDocAndRestoreAtScopeExit( _originatorDocSptr->getDocPtr(), _rootNodePtr );

	//Evaluate xpath expression
	xpathObj = xmlXPathEvalExpression (toXmlChar(usedXPath.c_str()), xpathCtx);
	ACS_COND_THROW( xpathObj == 0, // PRQA S 3081
			exXmlDocCriticalException("Unable to evaluate the xPath string ["+usedXPath+"]") );

	//Create the new documents coming from the xpath
	vector< XmlDocSptr > xmlDocs;

	xmlNodeSetPtr nodes (xpathObj->nodesetval);

	if ( nodes ) {

		for(int i = 0; i < nodes->nodeNr; ++i) {

			XmlDocSptr xmlDocSptr( new XmlDoc );

			//inherit parameters - and retain the internal doc.
			xmlDocSptr->setAsMyFather( *this );

			xmlDocSptr->_rootNodePtr = nodes->nodeTab[i];

			xmlDocs.push_back( xmlDocSptr );
		}

	} else {

		if ( xPathResult != 0 ) {
			string result;
			switch(xpathObj->type) {
			case XPATH_STRING:
			{
				result = fromXmlChar( xpathObj->stringval );
				xPathResult->set( result );
				break;
			}
			case XPATH_NUMBER:
			{
				xPathResult->set( xpathObj->floatval );
				break;
			}
			case XPATH_BOOLEAN:
			{
				xPathResult->set( xpathObj->boolval == 1 );
				break;
			}
			default: { break; }
			}
		}

	}

	//Here doc's structure is restored because xmlDoc_RestoreAtScopeExit
	//Here memory is released because xmlDoc_ScopeGuard

	return xmlDocs;
}

XmlDocSptr XmlDoc::xPathWrap (string const& xPath, string const& rootTag ) const {

	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Get nodes matching xpath
	vector< XmlDocSptr > xmlDocs = this->xPath(xPath);

	//Build wrapper root node and add children nodes
	xmlNodePtr rootNode = xmlNewNode(0, toXmlChar (rootTag.c_str()) );

	for ( vector< XmlDocSptr >::iterator it = xmlDocs.begin(); it!=xmlDocs.end(); ++it ) { // PRQA S 4238

		//Copy the root node from the current result doc to the wrapper root node
		xmlNodePtr rootOfResult = xmlCopyNode ( (*it)->_rootNodePtr, 1 );
		xmlAddChild( rootNode, rootOfResult );
	}

	//Return the wrapping doc
	XmlDocSptr xmlDocSptr( new XmlDoc );

	_xmlDoc* doc = xmlNewDoc (toXmlChar ("1.0")); // PRQA S 4412

	// Note! xmlDocSetRootElement move the node from the original document to this new one - but rootNode here is not shared with other docs.
	xmlDocSetRootElement( doc, rootNode);

	xmlDocSptr->_originatorDocSptr.reset( new LibXmlDocGuard(doc) );
	xmlDocSptr->_rootNodePtr = xmlDocGetRootElement(doc);
	//xmlDocSptr->_rootNodePtr = xmlDocSptr->_originatorDocSptr->getDocPtr()->children;

	//inherit parameters
	xmlDocSptr->setParameters( this->getParameters() );
	xmlDocSptr->setDefaultNamespaceName( this->_defaultNamespace );

	return xmlDocSptr;

}

void XmlDoc::setContent( string const& value) { // PRQA S 4211

	Lock lock_(*mutex_);

	//Cf. http://www.xmlsoft.org/examples/xpath2.c

	// Comments below are taken from the above ref.

	xmlNodePtr rootNodePtr = _rootNodePtr;

	/*
	 * NOTE: xmlNodeSetContent can actually free up descendant
	 *       of the node and such nodes may have been referenced by other XmlDoc object selected from xpath!
	 *       Mixing XPath and modifications on a tree must be done carefully !
	 */

	if ( rootNodePtr ) {

		xmlNodeSetContent(rootNodePtr, toXmlChar(value) );
		/*
		 * All the elements returned by an XPath query are pointers to
		 * elements from the tree *except* namespace nodes where the XPath
		 * semantic is different from the implementation in libxml2 tree.
		 * As a result when a returned node set is freed when
		 * xmlXPathFreeObject() is called, that routine must check the
		 * element type. But node from the returned set may have been removed
		 * by xmlNodeSetContent() resulting in access to freed data.
		 * This can be exercised by running
		 *       valgrind xpath2 test3.xml '//discarded' discarded
		 * There is 2 ways around it:
		 *   - make a copy of the pointers to the nodes from the result set
		 *     then call xmlXPathFreeObject() and then modify the nodes
		 * or
		 *   - remove the reference to the modified nodes from the node set
		 *     as they are processed, if they are not namespace nodes.
		 */
		if (rootNodePtr->type != XML_NAMESPACE_DECL) {
			rootNodePtr = 0;
		}

	}
}

void XmlDoc::unlink() {

	Lock lock_(*mutex_);

	if ( _rootNodePtr ) {
		xmlUnlinkNode( _rootNodePtr );
		xmlFreeNode( _rootNodePtr );
		_rootNodePtr = 0;
	}
}

XmlDocSptr XmlDoc::addTag( string const& tagName, string const& tagValue, std::string const& nsPrefix) { // PRQA S 4211

	Lock lock_(*mutex_);

	//Get namespace from prefix
	xmlNsPtr nsPtr = 0;
	if ( not nsPrefix.empty() ) {
		Map_nsPrefix_nsPtr nsprefixToXmlNsPtr;
		this->loadNamespaces(nsprefixToXmlNsPtr);
		if ( nsprefixToXmlNsPtr.count( nsPrefix ) ) {
			nsPtr = nsprefixToXmlNsPtr.at( nsPrefix );
		}
	}

	XmlDocSptr xmlDocSptr( new XmlDoc );

	xmlNodePtr pNode = xmlNewNode(nsPtr, toXmlChar(tagName) );

	xmlDocSptr->_rootNodePtr = pNode;
	if (!tagValue.empty())
	{
		xmlDocSptr->setContent( tagValue );
	}

	xmlAddChild( this->_rootNodePtr, xmlDocSptr->_rootNodePtr);

	//inherit parameters - and retain the internal doc.
	xmlDocSptr->setAsMyFather( *this );

	return xmlDocSptr;
}

XmlDoc& XmlDoc::addAttr( string const& attName, string const& attValue) {
	Lock lock_(*mutex_);

	xmlNewProp ( _rootNodePtr, toXmlChar(attName), toXmlChar(attValue) );

	return *this;
}

void XmlDoc::insert_namespaceSetDecl_recursion( _xmlNode * a_node, const Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr, std::set< xmlNsPtr > & nsToMove ) const {

	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			//Remove namespaces already defined on me
			xmlNsPtr nsDef = cur_node->nsDef;
			while (nsDef != 0) {
				string nsPrefix( nsDef->prefix? fromXmlChar(nsDef->prefix) : "" );
				string nsHref( fromXmlChar(nsDef->href) );

				if ( nsprefixToXmlNsPtr.count( nsPrefix ) == 0 ) {
					//Declared ns not declared on me. Remember it.
					nsToMove.insert( nsDef );
				}
				nsDef = nsDef->next;
			}
			cur_node->nsDef = 0;
		}
		this->insert_namespaceSetDecl_recursion(cur_node->children, nsprefixToXmlNsPtr, nsToMove);
	}
}

void XmlDoc::insert_namespaceSet_recursion( _xmlNode * a_node, const Map_nsPrefix_nsPtr & nsprefixToXmlNsPtr, std::set< xmlNsPtr > & nsToForget ) const {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			//Set namespaces to mine
			xmlNsPtr ns = cur_node->ns;
			if (ns) {
				string nsPrefix( ns->prefix? fromXmlChar(ns->prefix) : "" );
				string nsHref( fromXmlChar(ns->href) );

				if ( nsprefixToXmlNsPtr.count( nsPrefix ) ) {
					xmlNsPtr nsPtr = nsprefixToXmlNsPtr.at( nsPrefix );
					xmlSetNs( cur_node, nsPtr );

					nsToForget.insert( ns );
				}
			}
		}
		this->insert_namespaceSet_recursion(cur_node->children, nsprefixToXmlNsPtr, nsToForget);
	}
}


void XmlDoc::insert( XmlDocSptr const & xmlDocSptrToBeAdded, bool mergeNamespaceInMyRootDoc ) {

	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Load my namespace
	Map_nsPrefix_nsPtr nsprefixToXmlNsPtr;
	this->loadNamespaces(nsprefixToXmlNsPtr);

	xmlNodePtr deep_copy_node = xmlCopyNode( xmlDocSptrToBeAdded->_rootNodePtr, 1);
	ACS_COND_THROW( deep_copy_node == 0, // PRQA S 3081
			exXmlDocCriticalException("Unable to copy a xmldoc during insert.") );

	//Move NEW namespace definitions from deep copy to my root node
	std::set< xmlNsPtr > nsToMove;
	this->insert_namespaceSetDecl_recursion( deep_copy_node, nsprefixToXmlNsPtr, nsToMove );
	xmlNodePtr docRootNode = xmlDocGetRootElement( _originatorDocSptr->getDocPtr() );

	xmlNodePtr nodeWithDecl = mergeNamespaceInMyRootDoc ? docRootNode : _rootNodePtr;

	for ( auto nsPtr : nsToMove ) {
		string nsPrefix( nsPtr->prefix? fromXmlChar(nsPtr->prefix) : "" );
		if ( nsprefixToXmlNsPtr.count( nsPrefix ) == 0 ) {
			xmlNewNs( nodeWithDecl, nsPtr->href, nsPtr->prefix );
		}
	}

	//Update my namespaces
	this->loadNamespaces(nsprefixToXmlNsPtr);

	//Set namespaces of the deep copy to my namespaces instances
	// and remove namespace declarations from deep copy (the new ones have been moved to me)
	std::set< xmlNsPtr > nsToForget;
	this->insert_namespaceSet_recursion( deep_copy_node, nsprefixToXmlNsPtr, nsToForget );

	xmlAddChild( _rootNodePtr, deep_copy_node);

	xmlReconciliateNs( _originatorDocSptr->getDocPtr(), _rootNodePtr );

	//Free moved and reassigned ns
	auto nsToFree = nsToMove;
	nsToFree.insert( nsToForget.begin(), nsToForget.end());
	for ( auto nsPtr : nsToFree ) {
		xmlFreeNs( nsPtr );
	}

}


string XmlDoc::str() const { // PRQA S 4214

	Lock lock_(*mutex_);

	string currResult;

	bool stripCdata =    isSet(parStripCdata  );
	bool skipXmlHeader = isSet(parSkipXmlHdr  );

	bool getAttrValue =  isSet(parGetAttrValue);
	bool getAttrName =   isSet(parGetAttrName );

	// std::bitset< sizeof(u_int) > b( _parameters ); std::cout << "param: \t\t" << b <<endl;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called. stripCdata: "<<stripCdata<<"; skipXmlHeader: "<<skipXmlHeader<<"; getAttrValue: "<<getAttrValue<<"; getAttrName: "<<getAttrName);

	xmlNodePtr rootNode = _rootNodePtr;

	//Check if attribute
	if ( currResult.empty() && rootNode && (rootNode->type == XML_ATTRIBUTE_NODE) ) {
		if ( getAttrValue ) {
			currResult = fromXmlChar( rootNode->children->content );
		} else if ( getAttrName ) {
			currResult = fromXmlChar( rootNode->name );
		}
		else { /* nothing */ }
	}
	//If it is XML_ATTRIBUTE_NODE, but no attr parameters passed ==> return attrName="attrValue01"

	//Check if CDATA
	if ( stripCdata && currResult.empty() && rootNode && (rootNode->type == XML_CDATA_SECTION_NODE) ) {
		currResult = fromXmlChar( rootNode->content );
	}

	//All cases, if result still empty
	if ( currResult.empty() && rootNode ) {

		xmlKeepBlanksDefault(0); //Format setting

		if ( skipXmlHeader ) {
			//Dump the node
			xmlBufferPtr bufferPtr = xmlBufferCreate();
			xmlNodeDump(bufferPtr, _originatorDocSptr->getDocPtr(), rootNode, 0, 1);

			currResult = fromXmlChar (bufferPtr->content);

			xmlBufferFree(bufferPtr);

		} else {
			//Dump the node within a new document

			//_xmlDoc* doc = xmlNewDoc (toXmlChar ("1.0")); // PRQA S 4412
			_xmlDoc* doc = xmlCopyDoc( _originatorDocSptr->getDocPtr(), 0 ); //Copy, not recursive. Preserve xml header (Encoding, version, etc...)

			xmlNodePtr root_node = xmlCopyNode (rootNode, 1); //Recursive copy

			// Note! xmlDocSetRootElement move the node from the original document to this new one - but here root_node is a new copy
			xmlDocSetRootElement(doc, root_node);

			xmlChar* bufferPtr = 0 ;
			int size = 0 ;
			xmlDocDumpFormatMemory(doc, &bufferPtr, &size, 1);

			currResult = fromXmlChar (bufferPtr);

			xmlFree (bufferPtr);
			xmlFreeDoc(doc);
		}

	}

	return currResult;
}

bool XmlDoc::checkNamespaceExists(const string &ns) const {
	Lock lock_(*mutex_);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	const std::vector<XNamespace> & xnamespaces = this->getXnamespaces();

	bool found(false);
	for ( vector<XNamespace>::const_iterator it = xnamespaces.begin(); it!=xnamespaces.end() && !found ; ++it ) {
		const XNamespace& xns = *it;
		found = (xns.getPrefix() == ns);
	}

	return found;
}

namespace {
const int TMP_BUF_SIZE = 256 ;
}

void XmlDoc::setError(void *, const char *msg, ...) {
	Lock lock( mutexErrorMsg );
	char text[TMP_BUF_SIZE];
	va_list arg_ptr ; // PRQA S 4101
	va_start(arg_ptr, msg); // PRQA S 3080, 3081
	vsnprintf(text, TMP_BUF_SIZE, msg, arg_ptr);
	va_end(arg_ptr); // PRQA S 3080, 3081
	ACS_LOG_ERROR(text);
}

void XmlDoc::silentError(void *, const char *, ...) {
	Lock lock( mutexErrorMsg );
	//Nop
}

void XmlDoc::redirectErrorMsg()
{
	Lock lock( mutexErrorMsg );
	xmlGenericErrorFunc handler = reinterpret_cast<xmlGenericErrorFunc>(setError); // PRQA S 3081
	initGenericErrorDefaultFunc(&handler);
}
void XmlDoc::silentErrorMsg()
{
	Lock lock( mutexErrorMsg );
	xmlGenericErrorFunc handler = reinterpret_cast<xmlGenericErrorFunc>(silentError) ; // PRQA S 3081
	initGenericErrorDefaultFunc(&handler);
}

/*static*/ void XmlDoc::ReplaceString(std::string& subject, const std::string& search,
				const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

/*static*/ std::string XmlDoc::XNpath(const std::string& Npath) {
	string Xpath = Npath;
	ReplaceString(Xpath,"##","/text()");
	ReplaceString(Xpath,"$","'][./*[local-name() = '");
	ReplaceString(Xpath,"{","*[local-name() = '");
	ReplaceString(Xpath,"#","']/text()='");
	ReplaceString(Xpath,"}","']");
	return Xpath;
}

/*static*/ std::string XmlDoc::Npath(const std::string& Xpath) {
	string Npath = Xpath;
	ReplaceString(Npath,"'][./*[local-name() = '","$");
	ReplaceString(Npath,"*[local-name() = '","{");
	ReplaceString(Npath,"']/text()='","#");
	ReplaceString(Npath,"']","}");
	ReplaceString(Npath,"/text()","##");
	return Npath;
}


_ACS_END_NAMESPACE

