// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 2.8  2016/05/10 14:39:49  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.7  2015/04/29 10:30:07  marfav
	Warning removed
	
	Revision 2.6  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2012/06/05 15:39:18  marfav
	code improved to remove compiler warnings
	
	Revision 2.4  2012/02/10 17:37:02  marpas
	sing dtor definition added
	
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

#include <InputDescriptor.h>
#include <TaskTableVisitor.h>


#include <algorithm>
#include <functional>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;				   

ACS_CLASS_DEFINE_DEBUG_LEVEL(InputDescriptor)
ACS_CLASS_DEFINE_DEBUG_LEVEL(InputList)


InputList::InputList(): TaskTableVisitable()
{
}


InputList::InputList(const InputList& l) : TaskTableVisitable()
{
	*this = l;
}

InputList&
InputList::operator =(const InputList& l)
{
	if (this != &l)
	{
		TaskTableVisitable::operator= (l);
	}
	return *this;
}



TaskTableVisitable*
InputList::clone() const
{
	InputList* newInputList = new InputList (*this);
	return newInputList;
}


bool
InputList::acceptEnterVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB );
    return v.enterVisit(*this);
}


bool
InputList::acceptVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB );
    return v.visit(*this);
}


bool
InputList::acceptExitVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB );
    return v.exitVisit(*this);
}



class compare_altern_by_file_type // helper class in sorting alternatives
{
  public:
    int operator() (TaskTableVisitable* const& p1, TaskTableVisitable* const& p2) const
    {
	const InputAlternative *ap1 = dynamic_cast<const InputAlternative *>(p1); // PRQA S 3081
	const InputAlternative *ap2 = dynamic_cast<const InputAlternative *>(p2); // PRQA S 3081

	if (!ap1 || !ap2)
		ACS_THROW(exIllegalValueException("Object not of class InputAlternative found inside input elements while sorting alternatives by type")); // PRQA S 3081

    	return ap1->getFileType() < ap2->getFileType();
    }
};


class compare_altern_by_preference_order // helper class in sorting alternatives
{
  public:
    int operator() (TaskTableVisitable* const& p1, TaskTableVisitable* const& p2) const
    {
	const InputAlternative *ap1 = dynamic_cast<const InputAlternative *>(p1); // PRQA S 3081
	const InputAlternative *ap2 = dynamic_cast<const InputAlternative *>(p2); // PRQA S 3081

	if (!ap1 || !ap2)
		ACS_THROW(exIllegalValueException("Object not of class InputAlternative found inside input elements while sorting alternatives by preference")); // PRQA S 3081

    	return ap1->getPreferenceOrder() < ap2->getPreferenceOrder();
    }
};


class compare_altern // default helper class in sorting alternatives
{
  public:
    int operator() (TaskTableVisitable* const& p1, TaskTableVisitable* const& p2) const
    {
	const InputAlternative *ap1 = dynamic_cast<const InputAlternative *>(p1); // PRQA S 3081
	const InputAlternative *ap2 = dynamic_cast<const InputAlternative *>(p2); // PRQA S 3081

	if (!ap1 || !ap2)
		ACS_THROW(exIllegalValueException("Object not of class InputAlternative found inside input elements while sorting alternatives")); // PRQA S 3081

    	return (*ap1) < (*ap2);
    }
};



/**
 * class InputDescriptor
 */ 

InputDescriptor::InputDescriptor() : 
	TaskTableVisitable(),
	_OrderType(""),
	_Id(""),
	_Ref(""),
	_IsMandatory(false),
	_OrderTypeSet(false),
	_IdSet(false),
	_RefSet(false),
	_IsMandatorySet(false)

{
}

InputDescriptor::InputDescriptor(const InputDescriptor& d) : 
	TaskTableVisitable(d),
	_OrderType(d._OrderType),
	_Id(d._Id),
	_Ref(d._Ref),
	_IsMandatory(d._IsMandatory),
	_OrderTypeSet(d._OrderTypeSet),
	_IdSet(d._IdSet),
	_RefSet(d._RefSet),
	_IsMandatorySet(d._IsMandatorySet)
{
}

InputDescriptor&
InputDescriptor::operator =(const InputDescriptor& d)
{
	if (this != &d)
	{
		_OrderType      = d._OrderType;
		_Id             = d._Id;
		_Ref            = d._Ref;
		_IsMandatory    = d._IsMandatory;
		_OrderTypeSet   = d._OrderTypeSet;
		_IdSet          = d._IdSet;
		_RefSet         = d._RefSet;
		_IsMandatorySet = d._IsMandatorySet;
		TaskTableVisitable::operator= (d);
	}
	return *this;
}

bool 
InputDescriptor::isSetMandatory () const // PRQA S 4120
{
	return _IsMandatorySet;
}

bool
InputDescriptor::isMandatory() const // PRQA S 4120
{
	return _IsMandatory;
}

void
InputDescriptor::isMandatory(bool val)
{
	_IsMandatorySet = true;
	_IsMandatory = val;
}

bool
InputDescriptor::isSetOrderType() const // PRQA S 4120
{
	return _OrderTypeSet;
}

string
InputDescriptor::getOrderType() const // PRQA S 4120
{
	return _OrderType;
}

void
InputDescriptor::setOrderType(const string& aType)
{
	_OrderTypeSet = true;
	_OrderType = aType;
}

bool
InputDescriptor::isSetId() const // PRQA S 4120
{
	return _IdSet;
}

string
InputDescriptor::getId() const // PRQA S 4120
{
	return _Id;
}

void
InputDescriptor::setId(const string& anId)
{
	_IdSet = true;
	_Id    = anId;
}

void
InputDescriptor::resetId()
{
	_IdSet = false;
	_Id    = "";
}

bool
InputDescriptor::isSetRef() const // PRQA S 4120
{
	return _RefSet;
}

string
InputDescriptor::getRef() const // PRQA S 4120
{
	return _Ref;
}

void
InputDescriptor::setRef(const string& aRef)
{
	_RefSet = true;
	_Ref    = aRef;
}

void
InputDescriptor::resetRef()
{
	_RefSet = false;
	_Ref    = "";
}


class AlternativeGetter : public TaskTableVisitor // PRQA S 2109, 2153
{
public:
	virtual ~AlternativeGetter() throw() {} // PRQA S 2131

	explicit AlternativeGetter(const InputAlternative& a) : TaskTableVisitor(), _a(a), _found(false), _foundAlt() {} // PRQA S 2528
    
    using TaskTableVisitor::visit;
    using TaskTableVisitor::exitVisit;
    
	virtual bool visit (InputAlternative& alt) // PRQA S 2131
	{
		if (alt == _a)
		{
			_found = true;
			_foundAlt = alt;
		}

		return !_found;
	}
	virtual bool exitVisit (InputAlternative&) {return !_found;} // PRQA S 2131
	bool found() const {return _found;}
	InputAlternative alternative() const {return _foundAlt;}
private:
    AlternativeGetter() ; // not implemented
    AlternativeGetter(const AlternativeGetter& ) ; // not implemented
    AlternativeGetter& operator= (const AlternativeGetter& ) ; // not implemented
private:
	const InputAlternative& _a;
	bool _found;
	InputAlternative _foundAlt;
};


void 
InputDescriptor::fillAlternative (InputAlternative& theAlternative) const
{
	AlternativeGetter ag(theAlternative);
	ACS_SMARTPTR<InputDescriptor> p(static_cast<InputDescriptor*>(this->clone())); // need clone to keep const // PRQA S 3081
	p->acceptVisitor(ag);
	if (!ag.found())
	{
		// If no alternative found raise an exception
		// Only existent alternative are expected to be selected here
		ostringstream os;
		os << "Alternative not found: [" <<
			theAlternative.getFileType() << ", " <<
			theAlternative.getFileClass() << ", " <<
			int(theAlternative.getOrigin()) << ", " << // PRQA S 3081
			theAlternative.getRetrievalMode() << ", " <<
			int(theAlternative.getFileNameType()) << "]"; // PRQA S 3081
			
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	else
	{
		// fill "theAlternative" with real pathnames and time intervals
		theAlternative = ag.alternative();
	}
}


/**
 * This is an helper class used in STL algos
 * It defines a search algorithm that is able to
 * find an alternative having a particulare filetype
 */
class FileType_eq  // PRQA S 2109
{
public:
    explicit FileType_eq(const string& ss) : s(ss) {} // PRQA S 2528
    bool operator() (TaskTableVisitable* const& a) const // PRQA S 4020
    {
		InputAlternative* theAlternative = dynamic_cast<InputAlternative*>(a); // PRQA S 3081
		if (!theAlternative)
		{
			return false;
		}
		else
		{
            return theAlternative->getFileType() == s;
		}
    }
private:
    FileType_eq() ; // not implemented
private:
    // private attribute. The fileclass to search
    const string& s;

};


InputAlternative const&
InputDescriptor::getAlternative(const string& fileType) const
{
	// The find_if algo will use the FileType_eq class to find the filetype
	vector<TaskTableVisitable*>::const_iterator p = find_if(const_getElements().begin(), const_getElements().end(), FileType_eq(fileType)); // PRQA S 3081

	if (p == const_getElements().end())
	{
		// If no alternative found raise an exception
		// Only existent alternative are expected to be selected here
		ACS_THROW(exIllegalValueException("InputDescriptor: "+ fileType)); // PRQA S 3081
	}

	return *(static_cast<InputAlternative*>(*p)); // PRQA S 3081
}


class Origin_eq : public TaskTableVisitor // PRQA S 2109, 2153
{
public:
	explicit Origin_eq(const InputOrigin oo) : TaskTableVisitor(), o(oo), _found(false) {}
	virtual ~Origin_eq() throw() {} // PRQA S 2131

    using TaskTableVisitor::visit;
    using TaskTableVisitor::exitVisit;

	virtual bool visit (InputAlternative& a) // PRQA S 2131
	{
		_found = a.getOrigin() == o;
		return !_found;
	}
	virtual bool exitVisit (InputAlternative&) {return !_found;} // PRQA S 2131
	bool found() const {return _found;}

    private:
private:
    Origin_eq() ; // not implemented
    Origin_eq(const Origin_eq&) ; // not implemented
    Origin_eq& operator= (const Origin_eq&) ; // not implemented
private:
	// private attribute. The inputorigin to search
	InputOrigin o;
	bool _found;

};


bool
InputDescriptor::needsDownload() const
{
	// check if any of the alternatives has origin set to DB
	Origin_eq v(InputOriginDB);
	ACS_SMARTPTR<InputDescriptor> p(static_cast<InputDescriptor*>(this->clone())); // need clone to keep const // PRQA S 3081
	p->acceptVisitor(v);
	return v.found();
}

void
InputDescriptor::sortAlternatives(InputDescriptor::SortType type, const string& orderClob)
{
	switch (type)
	{
    case InputDescriptor::SortByPreferenceOrder:
        {
		    sort(_aggregates.begin(), _aggregates.end(), compare_altern_by_preference_order());
	        break;
        }
    case InputDescriptor::SortByFileType:
        {
		    sort(_aggregates.begin(), _aggregates.end(), compare_altern_by_file_type());
	        break;
        }
    default:
        {
		    sort(_aggregates.begin(), _aggregates.end(), compare_altern());
        }
	}
}

void
InputDescriptor::push_back(const InputAlternative& a)
{
    addElement(a);
    // since alternatives can be specified in any order in the task table,
    // we need to sort them out in order to be able to compare them later on
    sortAlternatives();
}

void
InputDescriptor::dump() const // PRQA S 4214
{
	ACS_CLASS_BGN_DEBUG ( ACS_MID_VERB )
		excout << "InputDescriptor [" << _OrderType << ", " <<
			boolalpha << _IsMandatory << noboolalpha << ", " << size() << "]\n"; 
	
		for (vector<TaskTableVisitable*>::const_iterator p = _aggregates.begin(); p != _aggregates.end(); ++p)
		{
			const InputAlternative *pp = dynamic_cast<const InputAlternative *>(*p); // PRQA S 3081
			if (pp) {
			    pp->dump();
            }
			else {
			    ACS_THROW(exIllegalValueException("Object not of class InputAlternative found inside input elements while dumping")); // PRQA S 3081
            }
		}
	ACS_CLASS_END_DEBUG
}

bool
InputDescriptor::operator == (const InputDescriptor& d) const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG ( 
        ACS_MID_VERB,
        "Comparing [" << _OrderType << ", " << int(_IsMandatory) << ", " << size() << // PRQA S 3081
        "] with [" <<
        d._OrderType << ", " << int(d._IsMandatory) << ", " << d.size() << "]"    
    );

	// two input descriptors are defined equals if:
	// 1 - the first is not < the second, and
	// 2 - the second is not < the first, and
	//
	// (we trust goodness of < operator, of course)

	bool result = !((*this) < d) && !(d < (*this));

    ACS_CLASS_WRITE_DEBUG ( ACS_MID_VERB, "Returning " << boolalpha << result );

	return result;
}

bool alt_eq_pred (TaskTableVisitable* const& p1, TaskTableVisitable* const& p2)
{
    const InputAlternative *ap1 = dynamic_cast<const InputAlternative *>(p1);  // PRQA S 3081
    const InputAlternative *ap2 = dynamic_cast<const InputAlternative *>(p2);  // PRQA S 3081

    if (!ap1 || !ap2) {
	    ACS_THROW(exIllegalValueException("Object not of class InputAlternative found inside input elements while comparing alternative"));   // PRQA S 3081
    }
    
    return (*ap1) == (*ap2);
}

bool
InputDescriptor::operator < (const InputDescriptor& d) const // PRQA S 4020
{
	// this is sort of an artificial definition, only needed to make standard stl algorithms work
	// (like merge and unique, which need to operate on sorted sequences);
	// hence, an input descriptor is defined less than another if:
	// 1 - its _OrderType is lexicographically < 
	//   OR
	// 2 - _OrderType is the same and it is mandatory and the other is optional 
	//   OR
	// 3 - _OrderType is the same and both are mandatory and has less inputs 
	//   OR
	// 4 - _OrderType is the same and both are mandatory and have same number of inputs and
	//     alternatives compare < 
	//     (the first not matching alternative will decide)
	// The field set and not set are not taken in account

	if (_OrderType == d._OrderType)
	{
		if (_IsMandatory == d._IsMandatory) 
		{
			if (size() == d.size())
			{
				// The descriptor has the same ordertype, the same mandatory flag
				// and the same alternative size
				// Check if ALL the alternatives are the same (using alternative's operators)

				pair<vector<TaskTableVisitable*>::const_iterator, vector<TaskTableVisitable*>::const_iterator> p;
				p = mismatch( _aggregates.begin(), _aggregates.end(), d._aggregates.begin(), alt_eq_pred);

				if (p.first == _aggregates.end()) { // sequences are equal
					return false;
                }
				else
				{
					// sequences are different
					// the mismatched alternative will decide
					const InputAlternative *ap1 = dynamic_cast<const InputAlternative *>(*(p.first)); // PRQA S 3081 2
					const InputAlternative *ap2 = dynamic_cast<const InputAlternative *>(*(p.second));
					return (*ap1 < *ap2);
				}
				
			}
			else { return (size() < d.size()); }
		}
		else { return (_IsMandatory && !d._IsMandatory); }
	}
	else { return (_OrderType < d._OrderType); }

}

bool
InputDescriptor::acceptVisit (TaskTableVisitor& theVisitor)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return theVisitor.visit (*this);
}


bool
InputDescriptor::acceptEnterVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return v.enterVisit(*this);
}


bool
InputDescriptor::acceptExitVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return v.exitVisit(*this);
}


TaskTableVisitable*
InputDescriptor::clone() const
{
	InputDescriptor* newInputDescriptor = new InputDescriptor (*this);
	return newInputDescriptor;
}

_ACS_END_NAMESPACE
