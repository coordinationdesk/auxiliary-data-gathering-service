// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia Spa;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.9  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)

	Revision 1.8  2013/10/29 19:08:57  nicvac
	compilation warning processed.

	Revision 1.7  2012/11/29 09:17:06  damdec
	Methods for reading orbit/relative orbit added.

	Revision 1.6  2012/09/03 12:56:01  damdec
	Added methods for getting attributes

	Revision 1.5  2012/08/03 13:05:28  damdec
	Methods for reading orbit reference metadata type introduced.

	Revision 1.4  2012/07/26 12:51:51  damdec
	writeXmlStream method made virtual

	Revision 1.3  2012/06/29 11:03:25  damdec
	Methods returning private attribute added. Default constructor added.

	Revision 1.2  2012/04/17 15:57:22  damdec
	Safe namespace passed as parameter to the constructor.

	Revision 1.1  2012/03/15 14:54:42  damdec
	OrbitReference class added.


*/

#ifndef _OrbitReference_H_
#define _OrbitReference_H_

#include <exException.h>
#include <WrappedMetadata.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Class mapping the SAFE OrbitReferenceType
**/

class OrbitReference : public WrappedMetadata { // PRQA S 2109
public:

	/** OrbitReference Exceptions */
	exDECLARE_EXCEPTION(exOrbitReferenceException, exException) ; // Base OrbitReference Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exOrbitReferenceCriticalException, exOrbitReferenceException) ; // Critical Exception. // PRQA S 2153


	/** Class constructors */
	OrbitReference();
	explicit OrbitReference(const rsResourceSet & rs);
	/** Class constructor */
	explicit OrbitReference(const std::string & safeNamespace,
                          unsigned int cycle = 0,
                          const std::string& phase="",
                          unsigned int start_orbit=0,
                          unsigned int stop_orbit=0,
                          unsigned int relative_start_orbit= 0,
                          unsigned int relative_stop_orbit=0, 
                          const std::string& safePrefix="sentinel-safe:");
	/** Destructor */
	virtual ~OrbitReference() throw() ;


	/** Copy Constructor */
	OrbitReference(const OrbitReference & );
	/** Operator = */
	OrbitReference &operator=(const OrbitReference &);

	unsigned int startOrbit() const throw() { return _startOrbit; }
	unsigned int stopOrbit()  const throw() { return _stopOrbit; }
	unsigned int relativeStartOrbit()  const throw() { return _relStartOrbit; }
	unsigned int relativeStopOrbit() const throw()  { return _relStopOrbit; }
	unsigned int cycle()  const throw() { return _cycle; }
	const std::string &phase() const throw() { return _phase; }

	void setStartOrbit(unsigned int v) { _startOrbit = v; }
	void setStopOrbit(unsigned int v) {_stopOrbit = v ; }
	void setRelativeStartOrbit(unsigned int v) {_relStartOrbit = v ;}
	void setRelativeStopOrbit(unsigned int v) {_relStopOrbit = v ;}
	void setCycle(unsigned int v) {_cycle = v ;}
	void setPhase(const std::string& v) {_phase = v ;}

protected:

	virtual void writeXmlStream() ;
	virtual void writeCommonXmlStream(std::ostringstream & xmlInfo);
	virtual void readXmlStream(const rsResourceSet & rs);

private:
	void readOrbit(const rsResourceSet & rs, const std::string & tmpKey);
	void readRelativeOrbit(const rsResourceSet & rs, const std::string & tmpKey);

private:
	unsigned int _startOrbit;
	unsigned int _stopOrbit;
	unsigned int _relStartOrbit;
	unsigned int _relStopOrbit;
	unsigned int _cycle;
	std::string _phase;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrbitReference)

};


_ACS_END_NAMESPACE

#endif //_OrbitReference_H_
