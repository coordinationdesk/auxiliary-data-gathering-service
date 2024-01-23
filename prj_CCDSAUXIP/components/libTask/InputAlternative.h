// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 2.8  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.7  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.6  2011/09/16 14:02:11  marfav
	Adding Stream tag support to alternatives
	
	Revision 2.5  2009/11/26 14:20:23  marfav
	Added rootTag in filter attributes
	
	Revision 2.4  2009/11/20 17:27:09  marfav
	Addedd support for additional Filter XML section
	
	Revision 2.3  2009/03/04 20:58:52  marpas
	CODECHECK parsing strategy improved: no regressions
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _INPUTALTERNATIVE_H
#define _INPUTALTERNATIVE_H

#include <acs_c++config.hpp>
#include <exException.h>
#include <TimeRange.h>

#include <TaskTableVisitable.h>
#include <TaskDataTypes.h>


_ACS_BEGIN_NAMESPACE(acs)
	
/**
 *\brief This class is the representation of an input alternative
 *       It is a Task Table Element and will accept ElementVisitors 
 *       for specific action purposes
 */
	
class InputAlternative : public TaskTableVisitable
{

public:
	/**
	 * CTOR and copy CTOR
	 */ 
	InputAlternative();
	InputAlternative(const InputAlternative& a);
    virtual ~InputAlternative() throw() {} // PRQA S 2131

	/**
	 *  Methods for setting and getting the Alternative FileType
	 *  the FileType is a plain string mapped to database or local filetypes
	 */
	bool isSetFileType () const;
	std::string getFileType() const ;
	void setFileType (const std::string& aType) ;

	/**
	 *  Methods for setting and getting the Alternative FileClass
	 *  the FileClass is a plain string mapped to database or local FileClass
	 */
	bool isSetFileClass() const;
	std::string getFileClass() const;
	void setFileClass (const std::string& aClass) ;

	/**
	 *  Methods for setting and getting the Filename Type
	 *  the parameter is defined using the enumerative type FileNameType
	 */
	bool isSetFileNameType () const;
	FileNameType getFileNameType () const;
	void setFileNameType (FileNameType aType);

	/**
	 *  Methods for setting and getting the Pathnames
	 *  Pathnames can be added using single strings or array of strings
	 *  and represent absolute filenames
	 */
	void getPathNames(std::vector<std::string>& v) const ;
	void addPathName(const std::string& aPath);
	void addPathNames(const std::vector<std::string>& paths);

	/**
	 *  Methods for setting and getting the Time Intervals related to every Pathname
	 *  Intervals are defined using the TimeRange class and 
	 *  using the pointer (integer index) to the related Pathname
	 */
	void getIntervals(std::vector< std::pair <int, acs::TimeRange> >& v) const ;
	void addInterval(const std::pair <int, acs::TimeRange>& anInterval);
	void addIntervals(const std::vector< std::pair <int, acs::TimeRange> >& v);

	/**
	 *  Methods for setting and getting the Origin of the Input Alternative
	 *  Origins are defined using the enumerative type InputOrigin
	 */
	bool isSetOrigin() const;
	InputOrigin getOrigin() const;
	void setOrigin(InputOrigin anOrigin);

	/**
	 *  Methods for setting and getting the Retrieval Mode of the Input Alternative
	 *  The retrieval mode is a string filed identifying a download stategy 
	 *  for files stored into the database.
	 */
	bool isSetRetrievalMode() const;
	std::string getRetrievalMode() const;
	void setRetrievalMode(const std::string& aMode);

	/**
	 *  Methods for setting and getting the delta times
	 *  Delta times define the margin of tolerance to use when
	 *  querying the database for a certain alternative
	 *  The time interval is extended using T0 for left time and T1 for right time
	 */
	bool isSetT0 () const;
	int getT0() const;
	void setT0(int sec);
	bool isSetT1 () const;
	int getT1() const;
	void setT1(int sec);

	/**
	 *  Methods for setting and getting the preference order of the alternative
	 *  The preference order is used to sort the alternatives in case of
	 *  input descriptor having multiple choiche
	 */
	bool isSetPreferenceOrder () const;
	int getPreferenceOrder() const;
	void setPreferenceOrder(int aPreferenceOrder);

	/**
	 * Methods for setting and getting the additional filter
	 * The filter is an XML fragment to be used in the dedicated special queries
	*/
	bool isSetFilter () const;
	std::string getFilterContent (bool* isSet=0) const;
	std::string getFilterRootTag (bool* isSet=0) const;
	void setFilter (const std::string&, const std::string&);

	/**
	 * Methods for setting and getting the Stream attribute
	 * Stream means that an alternative is a live stream and is mutable in time
	 * even during the processing execution
	*/
	bool isSetStream () const;
	bool getStream (bool* isSet=0) const;
	void setStream (bool);

	/**
	 *  Operators
	 *  The alternatives are sorted using this operators and STL algos
	 *  See the implementation for details about the meaning of the operators
	 */
	bool operator < (const InputAlternative& a) const; // PRQA S 2070 2
	bool operator == (const InputAlternative& a) const;
	
	/**
	 * Copy operator
	 */
	InputAlternative& operator = (const InputAlternative& a);
	
	/**
	 *  Dumps the content of the Alternative
	 *  used mainly for debug purposes
	 */
	void dump() const;


	/**
	 * Methods for setting and getting the satellite filtering
	*/
	bool isSetFilterBySatellite () const;
	bool getFilterBySatellite(bool* isSet=0) const;
	void setFilterBySatellite(bool) ;

	/**
	 * Methods for setting and getting the mission filtering
	*/
	bool isSetFilterByMission () const;
	bool getFilterByMission(bool* isSet=0) const;
	void setFilterByMission(bool) ;

	/**
	 * Methods for setting and getting the Alternative Activation Time
	*/
	bool isSetAltActivationTime () const;
	double getAltActivationTime(bool* isSet=0) const;
	void setAltActivationTime(double) ;
	
	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

private:
	// ATTRIBUTES  
	int _PreferenceOrder;
	int _T0;
	int _T1;
	InputOrigin _Origin;
	std::string _RetrievalMode;
	std::string _FileType;
	std::string _FileClass;
	FileNameType _NameType;
	bool _PreferenceOrderSet;
	bool _T0Set; 
    bool _T1Set;
	bool _OriginSet;
	bool _RetrievalModeSet;
	bool _FileTypeSet;
	bool _FileClassSet;
	bool _NameTypeSet;
	bool _altActivationTimeSet;
	std::vector<std::string> _PathNames;
	std::vector< std::pair <int, acs::TimeRange> > _intervals;
	std::string _filterContent;
	std::string _filterRootTag;
	bool _filterSet;
	bool _stream;
	bool _streamSet;
	bool _filterBySatellite;
	bool _filterBySatelliteSet;
	bool _filterByMission;
	bool _filterByMissionSet;
	double _altActivationTime ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(InputAlternative) ;
};

_ACS_END_NAMESPACE

#endif //_INPUTALTERNATIVE_H

