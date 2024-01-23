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

	$Prod: A.C.S. ThinLayer TaskTable Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.10  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.9  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.8  2011/05/09 16:10:00  nicvac
	Clone method returns TaskTable pointer.
	
	Revision 2.7  2010/08/03 14:28:06  nicvac
	Task Table plugins management
	
	Revision 2.6  2009/11/20 17:26:40  marfav
	Added support for Alternative_Filter_Tag optional field
	
	Revision 2.5  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.4  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.3  2006/03/09 14:27:44  giucas
	Operators [] fixed
	
	Revision 2.2  2006/03/07 15:13:09  marfav
	Task Table normalization can be disabled when reading from file
	
	Revision 2.1  2006/03/06 13:37:45  marfav
	AtExit Sequences structure support added
	
	Revision 2.0  2006/02/28 10:07:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/06/28 08:22:22  marfav
	Modified Task interface
	The order types are mapped to plain text strings
	
	Revision 1.10  2005/06/08 17:33:41  fracar
	writing to file in support of Task table merger application for IPF
	
	Revision 1.9  2004/12/03 18:03:32  marfav
	Header fixed
	
	Revision 1.8  2004/09/28 15:06:01  marfav
	Added percentage support
	
	Revision 1.7  2004/04/15 12:32:02  marfav
	Added messages for ProcessorTracker support
	
	Revision 1.6  2004/03/11 18:32:57  marfav
	stop method sends the killing signal specified in the Task Table
	kill method sends a signal to the Task or Pool
	
	Revision 1.5  2003/07/10 14:06:12  marfav
	clone method added to exceptions
	
	Revision 1.4  2003/04/30 14:50:15  marfav
	Align to ICD v2.0
	
	Revision 1.3  2003/04/18 12:49:24  marfav
	Using namespaces removed from .h
	
	Revision 1.2  2003/02/05 13:41:43  fracar
	handling T0 and T1 fields inside the task table
	
	Revision 1.1.1.1  2003/01/31 11:05:00  marfav
	Import libTaskTable
	
	

*/


#ifndef _TaskTable_H_
#define _TaskTable_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <TaskTableElement.h>
#include <AtExitSequence.h>
#include <WeightNormalizer.h>
#include <rsResourceSet.h>
#include <TaskTableVisitor.h>
#include <DynProcParam.h>

#include <vector>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

/**
 *\brief This class maps the XML file containing a task table. Upon creation, it
 *       loads the table from the XML path and stores it internally,
 *       allowing to query it.
 */

class TaskTable : public TaskTableElement // PRQA S 2109
{

	//using TaskTableVisitable::operator[];
	
public:

	/**
	 * This exception is raised when trying to select a task that does not exists
	 */ 
	exDECLARE_EXCEPTION(TaskTableNotFoundException, TaskTableElementException) ; // PRQA S 2131, 2153, 2502

	/**
	 * CTORs and Copy Ctor
	 * The constructor having a PathName as parameter, will try to load
	 * the task table from an XML file
	 */ 
	TaskTable ();
	explicit TaskTable (const std::string& PathName, bool doNormalization=true);

	/**
	 * DTOR
	 */ 
	virtual ~TaskTable () throw() ;

	/**
	 * Clone method implementation
	 */ 
	virtual TaskTable* clone() const; // PRQA S 2502

	void fillBase(TaskTable &t) const { t = *this ; } 
    
    // Un-hide not reimplemented methods
    using TaskTableElement::getLocalInputList;
    
	/**
	 * These methods gets the input/output/breakpoint/task list referred to the local object
	 */ 
	virtual void getLocalInputList (std::vector<InputDescriptor>& aList, 
									const std::string& theOrderType, 
									bool erase = false) const;
	
	virtual void getExtLocalInputList (std::vector<InputDescriptor>& aList, 
									   const std::string& theOrderType, 
									   bool erase = false) const;

	virtual void getLocalOutputList (std::vector<OutputDescriptor>& aList,
									 bool erase=false) const;
	
	virtual void getLocalBreakpointList (std::vector<BreakpointDescriptor>& aList,
										 bool erase=false) const;

	virtual void getLocalTaskList (std::vector<Task> & aList, bool erase = false) const;

	
	/**
	 * This method returns the list of config spaces included in the task table
	 */ 
	void getConfigSpaceList (std::vector<ConfigSpaceDescriptor>& aList) const ;
	
	/**
	 * This method returns the list of configuration files included in the task table
	 */ 
	void getConfigFileList (std::vector<ConfigDescriptor>& aList) const ;

	/**
	 * This method returns the list of configuration files included in the task table
	 */ 
	void getDynProcParamList (std::vector<DynProcParam>& aList) const ;
	
	/**
	 * Methods for setting and getting Min Disk Space parameter
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setMinDiskSpace(int);
	bool isSetMinDiskSpace () const;
	int getMinDiskSpace() const;
	
	/**
	 * Methods for setting and getting Max Allotted Time
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setMaxAllottedTime(int);
	bool isSetMaxAllottedTime() const;
	int getMaxAllottedTime() const;
	
	/**
	 * Methods for setting and getting Processor Name
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setProcName (const std::string&);
	bool isSetProcName() const;
	std::string getProcName() const;

	/**
	 * Methods for setting and getting Processor Version
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setProcVersion(const std::string&);
	bool isSetProcVersion() const;
	std::string getProcVersion() const;

	/**
	 * Methods for setting and getting the Default Config Version
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setDefaultConfigVersIdx (int&);
	bool isSetDefaultConfigVersIdx() const;
	int getDefaultConfigVersIdx () const;
	
	std::string getDefaultConfigVers() const;


	/**
	 * Methods for setting and getting the Satellite
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setSatellite (const std::string&);
	bool isSetSatellite() const;
	std::string getSatellite() const;


	/**
	 * Methods for setting and getting the Mission
	 * It is possible to check if the parameter has been set or not
	 */ 
	void setMission (const std::string&);
	bool isSetMission() const;
	std::string getMission() const;


	/**
	 * Methods for setting and getting the Test Flag
	 * It is possible to check if the parameter has been set or not
	 */ 
	void isTest(bool);
	bool isSetTest() const;
	bool isTest() const;


	/**
	 * Inserts new elements in the array of AtExitSequences
	 */ 
	void addAtExitSequence (const AtExitSequence&);


	/**
	 * Inserts new elements in the array of ConfigDescriptors
	 */ 
	void push_back (const ConfigDescriptor&);


	/**
	 * Inserts new elements in the array of ConfigSpace Descriptors
	 */ 
	void push_back (const ConfigSpaceDescriptor&);

	/**
	 * Inserts new elements in the array of Dynamic Parameters
	 */ 
	void push_back (const DynProcParam&);



	const std::vector<AtExitSequence>& getAtExitSequences() const;

	/**
	 * Returns the pathname related to the requested version of the configuration file
	 * if no version is specified, the default configuration file is returned
	 */ 
	std::string getConfigFile(const std::string& Version = "") const; 

	/**
	 * This operator allows to search into the list of pools (if any)
	 * using an index
	 */ 
	//TaskTableVisitable* operator[] (int idx) const;

	
	/**
	 * Returns the pathname of the xml file used to load the task table
	 */ 
	void setPath (const std::string&);
	std::string getPath (bool* = 0) const;
	void resetPath ();

	/** Apply pools, sequences and tasks normalization
	 *  It is possible to specify a normalizer or to use the default one
	 */ 
	virtual void normalize(WeightNormalizer* = 0);


	/**
	 * Methods for read and write the Task Table using xml files
	 * It is possible to pass a Reader or Writer in order to change 
	 * xml format on-the-fly
	 * Default Reader/Writer objects are used if no valid pointer passed
	 */ 
	virtual void readFromFile (const std::string& filename, bool doNormalization=true);
	virtual void writeOnFile  (const std::string& filename, const std::string& rootTag = "Task_Table");
	
	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

	/**
	 * Conversion operator 
	 * Are used to access (for writing) internal vectors
	 */
	 operator std::vector<ConfigDescriptor>& (); // PRQA S 2181 3
	 operator std::vector<ConfigSpaceDescriptor>& ();
	 operator std::vector<AtExitSequence>& ();
     operator std::vector<DynProcParam>& ();

	/** 
	 * Methods for setting and getting the Alternative Filter tag
	*/
	bool isSetAlternativeFilterTag() const;
	void setAlternativeFilterTag (const std::string&);
	std::string getAlternativeFilterTag (bool* isSet=0) const;

	/** 
	 * Methods for setting and getting the Sensing Time Flag
	*/
	bool isSetSensingTimeFlag() const;
	void setSensingTimeFlag (bool);
	bool getSensingTimeFlag (bool* isSet=0) const;
	
	/** 
	 * Methods for setting and getting the _isReadOnlyProcessor flag
	 */
	bool isReadOnlyProcessor() const { return _isReadOnlyProcessor; };
protected:
	TaskTable (const TaskTable& t);
	/**
	 * Copy operator
	 */ 
	TaskTable& operator = (const TaskTable& t);
    
private: // attributes
	std::string _path;
	bool _pathSet;
	std::string _satellite;
	std::string _mission;
	std::string _processorName;
	std::string _processorVersion;
	bool _isTest;
	int _minDiskSpace;
	int _maxAllottedTime;
	acs::rsResourceSet _myResources;
	int _defaultCfgIdx;
	bool _satelliteSet;
	bool _missionSet;
	bool _processorNameSet;
	bool _processorVersionSet;
	bool _isTestSet;
	bool _minDiskSpaceSet;
	bool _maxAllottedTimeSet;
	bool _myResourcesSet;
	bool _defaultCfgIdxSet;

	// Alternative Filter Tag definition
	// allowing special queries additional XML filters definition
	std::string _alternativeFilterTag;
	bool _alternativeFilterTagSet;

	// Tl2.0 attiributes
	bool _isReadOnlyProcessor;
    
    // GMES TT attirbutes
    bool _isSetSensingTimeFlag;
    bool _sensingTimeFlag;
	
private: // associations
	std::vector<ConfigDescriptor> _theConfigs;
	std::vector<ConfigSpaceDescriptor> _theConfigSpaces;
	std::vector<AtExitSequence> _theExitSequences;
    std::vector<DynProcParam> _theDynProcParams;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTable) ;
};


_ACS_END_NAMESPACE

#endif // _TaskTable_H_
