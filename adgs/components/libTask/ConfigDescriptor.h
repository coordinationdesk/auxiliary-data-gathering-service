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

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _CONFIGDESCRIPTOR_H_
#define _CONFIGDESCRIPTOR_H_

#include <acs_c++config.hpp>

#include <exException.h>
#include <TaskTableVisitable.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)


/**
 *\brief This class is the representation of a processor configuration
 *       for every processor is possible to define several configuration files
 */

class ConfigDescriptor : public TaskTableVisitable // PRQA S 2109
{
public:
	/**
	 * CTOR and copy CTOR
	 */ 
	ConfigDescriptor(const std::string& aVersion, const std::string& aPath);
	ConfigDescriptor(const ConfigDescriptor& d);
    virtual ~ConfigDescriptor() throw() {}  // PRQA S 2131

	/**
	 * Copy operator
	 */ 
	ConfigDescriptor& operator = (const ConfigDescriptor& d);

	/**
	 *  Methods for setting and getting the Version of the configuration file
	 */
	bool isSetVersion() const;
	std::string getVersion() const;
	void setVersion(const std::string& aVersion);

	/**
	 *  Methods for setting and getting the Path of the configuration file
	 */
	bool isSetPath () const;
	std::string getPath() const;
	void setPath(const std::string& aPath);

	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);
	
private:
    ConfigDescriptor() ; // not implemented
private:
	// attributes
	std::string _Version;
	bool _VersionSet;
	std::string _Path;
	bool _PathSet;
	
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ConfigDescriptor) ;
};

_ACS_END_NAMESPACE

  
#endif //_CONFIGDESCRIPTOR_H_

