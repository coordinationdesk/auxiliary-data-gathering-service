// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer WS Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.8  2015/11/19 15:44:28  marpas
	coding best practices and rule applied (partially)
	
	Revision 2.7  2015/06/17 10:53:45  damdec
	Compilation warnings removed.
	
	Revision 2.6  2013/03/11 16:24:49  marfav
	Compiler warning reduced
	
	Revision 2.5  2012/12/05 11:43:46  chicas
	removing compiler warnings
	
	Revision 2.4  2011/01/24 15:56:13  marpas
	Mutex.h does not longer exixts
	
	Revision 2.3  2007/03/13 15:44:04  fracar
	added DEBUG capability
	
	Revision 2.2  2006/04/21 12:17:05  marfav
	Added get methods for Stdout and Stderr log levels
	
	Revision 2.1  2006/04/21 10:22:06  marfav
	Standard output and standard error indipendent log levels support added
	
	Revision 2.0  2006/02/28 10:10:54  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.13  2005/06/28 08:31:42  marfav
	Multiple WSConfig schemas supported
	OrderType translation supported by libTlOrder manager
	
	Revision 1.12  2005/06/10 13:51:07  fracar
	added empty constructor
	
	Revision 1.11  2004/12/23 09:48:00  marfav
	Added support for using ostream in operator >>
	
	Revision 1.10  2004/12/03 18:05:07  marfav
	Header fixed
	
	Revision 1.9  2004/04/06 15:42:01  marfav
	Does not use ACE mutex anymore
	
	Revision 1.8  2004/03/12 09:11:13  marfav
	Added support for merging WSConfiguration files
	
	Revision 1.7  2003/09/22 11:04:53  fracar
	operator = now thread safe
	
	Revision 1.6  2003/08/01 10:45:19  marfav
	Check for configuration variations on reload added
	
	Revision 1.5  2003/07/28 14:58:08  marfav
	Copy-CTOR and operator= implemented
	
	Revision 1.4  2003/07/10 14:03:50  marfav
	clone method added to exceptions
	
	Revision 1.3  2003/04/18 12:50:57  marfav
	Using namespaces removed from .h
	
	Revision 1.2  2003/02/04 17:26:25  fracar
	got rid of ^M
	
	Revision 1.1.1.1  2003/01/31 16:03:28  marfav
	Import libWSConfig
	
	
	

*/


#ifndef _WS_CONFIG_H
#define _WS_CONFIG_H

#include <Task.h>

#include <ThreadSafe.h>

#include <exException.h>
#include <rsResourceSet.h>

#include <vector>
#include <string>
#include <fstream>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

/*
 * This class maps the XML workstation configuration file.
 * The XML file is loaded inside the  constructor and stored internally,
 * then the loaded configuration can be queried
 */

class WSConfigElement
{
public:
	WSConfigElement();
	virtual ~WSConfigElement() throw() ;

	WSConfigElement(const WSConfigElement &);
	WSConfigElement& operator = (const WSConfigElement &);

	std::string name; // PRQA S 2100 7
	std::string version;
	std::string logLevel;
	std::string outLogLevel;
	std::string errLogLevel;
	std::string taskTable;
	std::map <std::string, bool> orderTypeStatus;
	bool hasPosixExitCode;
};

bool operator == (const WSConfigElement &, const WSConfigElement &);

class WSConfig // PRQA S 2109, 2153
{
public:
	/*! class WSConfigException declaration */
	exDECLARE_EXCEPTION(WSConfigException,acs::exException) ; // PRQA S 2131, 2502

    WSConfig ();
    explicit WSConfig (const std::string& PathName);
	~WSConfig() throw() {} 
    void load(const std::string& PathName);
    void load();

    bool getTaskTable(const std::string& aProc, const std::string& aVersion, std::string& tt) const;
    bool getLogLevel(const std::string& aProc, const std::string& aVersion, std::string& ll) const;
    bool getOutLogLevel(const std::string& aProc, const std::string& aVersion, std::string& ll) const;
    bool getErrLogLevel(const std::string& aProc, const std::string& aVersion, std::string& ll) const;
    bool isOrderProcessable(const std::string& aProc, const std::string& aVersion, const std::string& order);
    void getProcessors(std::vector<std::pair<std::string, std::string> >& ) const;
    void getOrders(const std::string& aProc, const std::string& aVersion, std::vector<std::string>& orders) const;
    bool getProcessor(const std::string& aProc, const std::string& aVersion, WSConfigElement&) const;

	// APF-411
    bool hasPosixExitCode (const std::string& aProc, const std::string& aVersion, bool&) const;


    // Returns TRUE if the LAST load operation has changed the internal resource set
    bool hasChanged() const;
	
	// Sets or gets the legacy support
	// Legacy = true -> Cryosat-like schema
	bool isLegacy() const;
	void setLegacy (bool);


    void toStream(std::ostream&) const ;

    std::string getPath() const;


    WSConfig& operator = (const WSConfig&);
    WSConfig (const WSConfig&);

    const WSConfigElement& operator [] (const int& s) const; // PRQA S 2141
    const WSConfigElement& at (const int & s) const; // PRQA S 2141

    std::vector<WSConfigElement> getWSConfigElements () const;
	size_t size() const;
	void clear();
	void push_back(WSConfigElement & t);
	std::vector<WSConfigElement>::iterator  erase(std::vector<WSConfigElement>::const_iterator position);
	std::vector<WSConfigElement>::iterator  erase(std::vector<WSConfigElement>::const_iterator firstPos, std::vector<WSConfigElement>::const_iterator lastPos);

	std::vector<WSConfigElement>::iterator  erase(std::vector<WSConfigElement>::iterator position);
	std::vector<WSConfigElement>::iterator  erase(std::vector<WSConfigElement>::iterator firstPos, std::vector<WSConfigElement>::iterator lastPos);

	std::vector<WSConfigElement>::const_iterator begin() const;
	std::vector<WSConfigElement>::const_iterator end() const;
	
	std::vector<WSConfigElement>::iterator begin(); 
	std::vector<WSConfigElement>::iterator end(); 
	// operator implicit conversion
	operator const std::vector<WSConfigElement>& () const;
	operator std::vector<WSConfigElement>& () ;

  private: // methods
    bool getProcKey(const std::string& aProc, const std::string& aVersion, const std::string& aKey, std::string& s) const;

  private: // attributes
    std::string _path;

  private: // associations
    bool _hasChanged;
	bool _legacyMode;
    mutable acs::ThreadSafe _mutex;  // needed to load resources from file at run-time, outside the ctor

    std::vector<WSConfigElement> _wsElements;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(WSConfig)
};

std::ostream &operator << (std::ostream &, const WSConfig &) ;



_ACS_END_NAMESPACE

#endif // _WS_CONFIG_H
