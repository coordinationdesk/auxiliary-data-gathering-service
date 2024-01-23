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
	Revision 1.7  2014/04/24 16:43:28  tergem
	Porting to head
	
	Revision 1.6  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.5  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.4  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.3  2012/08/03 12:58:41  damdec
	virtual pure readXmlStream method added.
	
	Revision 1.2  2012/04/17 15:45:50  damdec
	_safeNamespace attribute added.
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/
#ifndef _WrappedMetadata_H_
#define _WrappedMetadata_H_

#include <exException.h>
#include <rsResourceSet.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
/**
* \brief
* Parent class representing the wrapped metadata 
**/

class WrappedMetadata  {  // PRQA S 2109
public:

	/** WrappedMetadata Exceptions */
	exDECLARE_EXCEPTION(exWrappedMetadataException, exException) ; // Base WrappedMetadata Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exWrappedMetadataCriticalException, exWrappedMetadataException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	WrappedMetadata(const std::string &safeNameSpace, const std::string &safePrefix);
	/** Destructor */
	virtual ~WrappedMetadata() throw() ;

	void setXMLStream(const std::string& v) { _xmlStream = v ; }
	const std::string& getXMLStream() const throw() { return _xmlStream; }
	const std::string & getSafeNamespace() const throw() { return _safeNamespace;} 
        void setSafeNamespace(const std::string & n) {_safeNamespace = n;}
	void setSafePrefix(const std::string & v) {_safePrefix = v;}
	const std::string & getSafePrefix() const throw() { return _safePrefix;} 
	
protected:
	WrappedMetadata(const WrappedMetadata & );
	WrappedMetadata &operator=(const WrappedMetadata &);
	virtual void writeXmlStream() = 0; //to be implemented in the derived classes
	virtual void readXmlStream(const rsResourceSet & rs) = 0; //to be implemented in the derived classes
private:
    WrappedMetadata() ; // not implemented
private:
	std::string _safeNamespace;
	std::string _safePrefix;
	std::string _xmlStream;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(WrappedMetadata)

};

_ACS_END_NAMESPACE


#endif //_WrappedMetadata_H_

