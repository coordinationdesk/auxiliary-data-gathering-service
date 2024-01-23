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
	Revision 2.9  2015/04/29 10:30:07  marfav
	Warning removed
	
	Revision 2.8  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.7  2012/06/05 15:39:18  marfav
	code improved to remove compiler warnings
	
	Revision 2.6  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.5  2011/09/16 14:02:11  marfav
	Adding Stream tag support to alternatives
	
	Revision 2.4  2009/11/26 14:20:23  marfav
	Added rootTag in filter attributes
	
	Revision 2.3  2009/11/20 17:27:09  marfav
	Addedd support for additional Filter XML section
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/


#include <InputAlternative.h>

#include <TaskTableVisitor.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InputAlternative)

// Class InputAlternative

InputAlternative::InputAlternative() : 
	TaskTableVisitable(),
	_PreferenceOrder(0),
	_T0(0),
	_T1(0),
	_Origin (InputOriginDB),
	_RetrievalMode(),
	_FileType(),
	_FileClass(),
	_NameType (FileNameTypeUNKNOWN),
	_PreferenceOrderSet(false),
	_T0Set(false),
	_T1Set(false),
	_OriginSet(false),
	_RetrievalModeSet(false),
	_FileTypeSet(false),
	_FileClassSet(false),
	_NameTypeSet(false),
	_altActivationTimeSet(false),
    _PathNames(),
    _intervals(),
	_filterContent (),
	_filterRootTag (),
	_filterSet (false),
	_stream (false),
    _streamSet (false),
	_filterBySatellite(true),
	_filterBySatelliteSet(false),
	_filterByMission(true),
	_filterByMissionSet(false),
	_altActivationTime(0.0)
{
}

InputAlternative::InputAlternative(const InputAlternative& a) : 
    TaskTableVisitable(),
	_PreferenceOrder(0),
	_T0(0),
	_T1(0),
	_Origin (InputOriginDB),
	_RetrievalMode(),
	_FileType(),
	_FileClass(),
	_NameType (FileNameTypeUNKNOWN),
	_PreferenceOrderSet(false),
	_T0Set(false),
	_T1Set(false),
	_OriginSet(false),
	_RetrievalModeSet(false),
	_FileTypeSet(false),
	_FileClassSet(false),
	_NameTypeSet(false),
	_altActivationTimeSet(false),
    _PathNames(),
    _intervals(),
	_filterContent (),
	_filterRootTag (),
	_filterSet (false),
	_stream (false),
    _streamSet (false),
	_filterBySatellite(true),
	_filterBySatelliteSet(false),
	_filterByMission(true),
	_filterByMissionSet(false),
	_altActivationTime(0.0)
{
	*this = a;
}

InputAlternative&
InputAlternative::operator =(const InputAlternative& a)
{
	if (this != &a)
	{
		_FileType = a._FileType;
		_FileClass = a._FileClass;
		_T0 = a._T0;
		_T1 = a._T1;
		_Origin = a._Origin;
		_PathNames = a._PathNames;
		_NameType = a._NameType;
		_intervals = a._intervals;
		_PreferenceOrder = a._PreferenceOrder;
		_RetrievalMode = a._RetrievalMode;
		_FileTypeSet = a._FileTypeSet;
		_FileClassSet = a._FileClassSet;
		_T0Set = a._T0Set;
		_T1Set = a._T1Set;
		_OriginSet = a._OriginSet;
		_NameTypeSet = a._NameTypeSet;
		_altActivationTimeSet = a._altActivationTimeSet;
		_PreferenceOrderSet = a._PreferenceOrderSet;
		_RetrievalModeSet = a._RetrievalModeSet;
		_filterContent = a._filterContent;
		_filterRootTag = a._filterRootTag;
		_filterSet = a._filterSet;
		_stream = a._stream;
		_streamSet = a._streamSet;
		_filterBySatellite = a._filterBySatellite;
		_filterBySatelliteSet = a._filterBySatelliteSet;
		_filterByMission = 	a._filterByMission;
		_filterByMissionSet = a._filterByMissionSet;
		_altActivationTime = a._altActivationTime;
		TaskTableVisitable::operator= (a);
	}
	return *this;
}

void
InputAlternative::dump() const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG ( 
        ACS_MID_VERB, 
        "InputAlternative [" << getFileType() << ", " << getFileClass() << ", " << 
        InputOrigin2String(getOrigin()) << ", " << getRetrievalMode() << "]"
    );
}

bool
InputAlternative::operator == (const InputAlternative& a) const // PRQA S 4214
{
	// two input alternatives are defined equal if they have:
	// 1 - same file type
	// 2 - same fileclass
	// 3 - same origin
	// 4 - same retrieval mode
	// 5 - same T0
	// 6 - same T1
	// 7 - same file name type
	// 8 - same filter
	// 9 - same stream attribute

    ACS_CLASS_WRITE_DEBUG (
        ACS_MID_VERB,
        "comparing [" << getFileType() << ", " << getFileClass() << ", " << InputOrigin2String(getOrigin()) << ", " << getRetrievalMode() <<
        "] with [" <<
        a.getFileType() << ", " << a.getFileClass() << ", " << InputOrigin2String(a.getOrigin()) << ", " << a.getRetrievalMode() << "]"
    );

	bool result = !((*this) < a) && !(a < (*this));

    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Returning " << boolalpha << result );

	return result;
}

bool
InputAlternative::operator < (const InputAlternative& a) const // PRQA S 4020
{
  // this is sort of an artificial definition, only needed to make standard stl algorithms work
  // (like merge and unique, which need to operate on sorted sequences);
  // hence, an input alternative is defined less than another if:
  // 1 - file types lexicographically compare <	
  //   OR
  // 2 - file types are equal and file classes lexicographically compare <	
  //   OR
  // 3 - file types and file classes are equal and origin numerically compare <	
  //   OR
  // 4 - file types and file classes and origin are equal, and retrieval mod lexicographically compare < 
  //   OR
  // 5 - file types and file classes and origin are equal, retrieval mod lexicographically compare equal,
  //     and T0 compare < 
  //   OR
  // 6 - file types and file classes and origin are equal, retrieval mod lexicographically compare equal,
  //     T0 compare equal, and T1 compare < 
  //   OR
  // 7 - file types and file classes and origin are equal, retrieval mod lexicographically compare equal,
  //     T0 compare equal, T1 compare equal, and FileNameType numerically compare <
  //   OR
  // 8 - all fileds at #7 compare and filter lexicographically compare <
  //   OR
  // 9 - all fields at #8 and stream compare <
  //   OR
  // 10 - all fields at #9 and filter by Satellite flag compare <
  //   OR
  // 11 - all fields at #10 and Alternative Activation time flag compare <
  //   OR
  // 12 - all fields at #11 and filter by Mission flag compare <
  //
  // The SET flag is not checked in the less than operator. Set and unset field are allowed to be equal
  //
  // Note that intervals must NOT be taken into account while comparing alternatives, otherwise
  // all equality checks during precondition stage (when no access to SDF and no download have been performed)
  // would be skrewed up
  

	if (getFileType() == a.getFileType())
	{
		if (getFileClass() == a.getFileClass() ) 
		{
			if (getOrigin() == a.getOrigin())
			{
				if (getRetrievalMode() == a.getRetrievalMode())
				{
					if (getT0() == a.getT0())
					{
						if (getT1() == a.getT1())
						{
							if (getFileNameType() == a.getFileNameType())
							{
								if (getFilterContent() == a.getFilterContent())
								{
									if( getStream() == a.getStream() )
									{
										if( getFilterBySatellite() == a.getFilterBySatellite() ) 
										{
											if( getAltActivationTime() == a.getAltActivationTime() ) {
												return getFilterByMission() < a.getFilterByMission();
											}
											else {
												return getAltActivationTime() < a.getAltActivationTime() ;
											}
										}
										else { return getFilterBySatellite() < a.getFilterBySatellite(); }
									}
									else { return getStream() < a.getStream(); }
								}
								else { return getFilterContent() < a.getFilterContent(); }
							}
							else { return getFileNameType() < a.getFileNameType(); }
						}
						else { return getT1() < a.getT1(); }
					}
					else { return getT0() < a.getT0(); }
				}
				else { return getRetrievalMode() < a.getRetrievalMode(); }
			}
			else { return getOrigin() < a.getOrigin(); }
		}
		else { return getFileClass() < a.getFileClass(); }
	}
	else { return getFileType() < a.getFileType(); }

}

bool
InputAlternative::isSetFileNameType() const // PRQA S 4120
{
	return _NameTypeSet;
}

FileNameType
InputAlternative::getFileNameType() const // PRQA S 4120
{
	return _NameType;
}

void
InputAlternative::setFileNameType(FileNameType aType)
{
	_NameTypeSet = true;
	_NameType = aType;
}

bool
InputAlternative::isSetFileClass () const // PRQA S 4120
{
	return _FileClassSet;
}

string
InputAlternative::getFileClass() const // PRQA S 4120
{
	return _FileClass;
}

void
InputAlternative::setFileClass(const string& aClass)
{
	_FileClassSet = true;
	_FileClass = aClass;
}


bool 
InputAlternative::isSetFileType () const // PRQA S 4120
{
	return _FileTypeSet;
}

string
InputAlternative::getFileType() const // PRQA S 4120
{
	return _FileType;
}

void
InputAlternative::setFileType(const string& aType)
{
	_FileTypeSet = true;
	_FileType = aType;
}

void
InputAlternative::getPathNames(vector<string>& v) const
{
	copy(_PathNames.begin(), _PathNames.end(), back_inserter(v));
}

void
InputAlternative::addPathName(const string& aPath)
{
	_PathNames.push_back(aPath);
}

void
InputAlternative::addPathNames(const vector<string>& paths)
{
	copy(paths.begin(), paths.end(), back_inserter(_PathNames));
}

void
InputAlternative::getIntervals(vector< pair <int, TimeRange> >& v) const // PRQA S 4120
{
	v = _intervals;
}

void
InputAlternative::addInterval(const pair <int, TimeRange>& anInterval)
{
	_intervals.push_back(anInterval);
}

void
InputAlternative::addIntervals(const vector< pair <int, TimeRange> >& v) // PRQA S 4121
{
	_intervals = v;
}


bool
InputAlternative::isSetOrigin() const // PRQA S 4120
{
	return _OriginSet;
}

InputOrigin
InputAlternative::getOrigin() const // PRQA S 4120
{
	return _Origin;
}

void
InputAlternative::setOrigin(InputOrigin anOrigin)
{
	_OriginSet = true;
	_Origin = anOrigin;
}


bool
InputAlternative::isSetRetrievalMode () const // PRQA S 4120
{
	return _RetrievalModeSet;
}

string
InputAlternative::getRetrievalMode() const // PRQA S 4120
{
	return _RetrievalMode;
}

void
InputAlternative::setRetrievalMode(const string& aMode)
{
	_RetrievalModeSet = true;
	_RetrievalMode = aMode;
}


bool
InputAlternative::isSetPreferenceOrder() const // PRQA S 4120
{
	return _PreferenceOrderSet;
}

int
InputAlternative::getPreferenceOrder() const // PRQA S 4120
{
	return _PreferenceOrder;
}

void
InputAlternative::setPreferenceOrder(int aPreferenceOrder)
{
	_PreferenceOrderSet = true;
	_PreferenceOrder = aPreferenceOrder;
}


bool 
InputAlternative::isSetT0 () const // PRQA S 4120
{
	return _T0Set;
}

int
InputAlternative::getT0() const // PRQA S 4120
{
	return _T0;
}

void
InputAlternative::setT0(int sec)
{
	_T0Set = true;
	_T0 = sec;
}

bool 
InputAlternative::isSetT1 () const // PRQA S 4120
{
	return _T1Set;
}

int
InputAlternative::getT1() const // PRQA S 4120
{
	return _T1;
}

void
InputAlternative::setT1(int sec)
{
	_T1Set = true;
	_T1 = sec;
}

bool 
InputAlternative::isSetFilter () const // PRQA S 4120
{
	return _filterSet;
}

string 
InputAlternative::getFilterContent (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _filterSet;
	}
	return _filterContent;
}

string 
InputAlternative::getFilterRootTag (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _filterSet;
	}
	return _filterRootTag;
}

void 
InputAlternative::setFilter (const string& content, const string& rootTag)
{
	_filterSet = true;
	_filterContent = content;
	_filterRootTag = rootTag;
}

bool InputAlternative::isSetAltActivationTime () const
{
	return _altActivationTimeSet ;
}

double InputAlternative::getAltActivationTime(bool* isSet) const
{
	if (isSet)
	{
		*isSet = _altActivationTimeSet;
	}
	return _altActivationTime;
}

void InputAlternative::setAltActivationTime(double v)
{
	_altActivationTimeSet = true ;
	_altActivationTime = v ;
}



bool 
InputAlternative::isSetStream () const // PRQA S 4120
{
	return _streamSet;
}

bool 
InputAlternative::getStream (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _streamSet;
	}
	return _stream;
}

void 
InputAlternative::setStream (bool stream)
{
	_streamSet = true;
	_stream = stream;
}

bool
InputAlternative::acceptEnterVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.enterVisit (*this);
}

bool
InputAlternative::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}

bool
InputAlternative::acceptExitVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.exitVisit (*this);
}

TaskTableVisitable*
InputAlternative::clone() const
{
	InputAlternative* newInputAlternative = new InputAlternative (*this);
	return newInputAlternative;
}

bool InputAlternative::isSetFilterBySatellite() const
{
	return _filterBySatelliteSet;
}


bool InputAlternative::getFilterBySatellite(bool *isSet) const
{
	if (isSet) {
		*isSet = _filterBySatelliteSet;
	}
	return _filterBySatellite;
}

void InputAlternative::setFilterBySatellite(bool v)
{
	_filterBySatelliteSet = true;
	_filterBySatellite = v;
}


bool InputAlternative::isSetFilterByMission() const
{
	return _filterByMissionSet;
}

bool InputAlternative::getFilterByMission(bool *isSet) const
{
	if (isSet) {
		*isSet = _filterByMissionSet;
	}
	return _filterByMission;
}

void InputAlternative::setFilterByMission(bool v)
{
	_filterByMissionSet = true;
	_filterByMission = v;
}

_ACS_END_NAMESPACE
