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

#ifndef _CONFIGSPACEDESCRIPTOR_H_
#define _CONFIGSPACEDESCRIPTOR_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <TaskTableVisitable.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)


/**
 *\brief This class is the representation of a configuration space
 *       every processor can request one or more configuration spaces 
 *       to be placed in the working directory (downloaded from the database)
 *       this class is a wrapper for the pair (ConfigSpaceName, PathName)
 */
class ConfigSpaceDescriptor : public TaskTableVisitable // PRQA S 2109
{
public:
	
	/**
	 * CTOR and copy CTOR
	 */ 
	explicit ConfigSpaceDescriptor(const std::string& aSpaceName, const std::string& aPath="");
	ConfigSpaceDescriptor(const ConfigSpaceDescriptor& d);
    virtual ~ConfigSpaceDescriptor() throw() {} // PRQA S 2131
	/**
	 * Copy operator
	 */ 
	ConfigSpaceDescriptor& operator = (const ConfigSpaceDescriptor& d);

	/**
	 *  Methods for setting and getting the configuration space name
	 */
	bool isSetSpaceName() const;
	std::string getSpaceName () const;
	void setSpaceName (const std::string& aSpaceName);

	/**
	 *  Methods for setting and getting the path of the downloaded config space
	 */
	bool isSetPath() const;
	std::string getPath () const;
	void setPath(const std::string& aPath);

	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);
private:
    ConfigSpaceDescriptor() ; // not implemented
private:
	// attributes
	std::string _SpaceName;
	std::string _Path;
	bool _SpaceNameSet;
	bool _PathSet;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ConfigSpaceDescriptor) ;
};

_ACS_END_NAMESPACE

#endif //_CONFIGSPACEDESCRIPTOR_H_


