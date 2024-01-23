// PRQA S 1050 EOF
/*
 * 
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.4  2014/12/02 17:42:15  marfav
	S2PDGS-950 granule filtering and migration report
	
	Revision 6.3  2014/11/27 18:47:36  nicvac
	isEmpty added.
	
	Revision 6.2  2014/11/27 16:45:17  marfav
	S2PDGS-992 completed
	
	Revision 6.1  2014/11/27 15:15:27  marfav
	Added MigrationReport class
	

*/

#ifndef _MigrationReport_H_
#define _MigrationReport_H_

#include <acs_c++config.hpp>

#include <map>
#include <string>


_ACS_BEGIN_NAMESPACE (acs)

class MigrationReport
{
public:

    typedef std::pair <std::string, std::string> ActionItem;

    // The map is a map[id] -> action
    typedef std::map <std::string, std::string> ActionsMapType;

    // Standard set of CTOR-DTOR-Copy CTOR-Assignment
    MigrationReport ();
    virtual ~MigrationReport () = default ;

    MigrationReport (const MigrationReport&);
    MigrationReport& operator= (const MigrationReport&);

    MigrationReport (MigrationReport&&) = delete ;
    MigrationReport& operator= (MigrationReport&&) = delete ;


    // clean-up everything 
    void reset();

    // Brings back the iterator to the begin of the structure and makes it valid
    void resetIterator();

    // true if empty
    bool isEmpty() const;

    // It will reset the iterator and start back from the beginning in case 
    // the iterator is no more valid
    bool getNextItem(ActionItem&);

    // Inserts a new action in the map
    // it invalidates the internal iterator
    void setAction (const std::string& id, const std::string& action);

    // Returns the action associated to an id 
    // In case no action is found the empty string is returned
    // and the boolean is set accordingly (if passed)
    std::string getAction (const std::string& id, bool* valid = 0) const;

    // Returns a string containing the XML serialization of the object
    std::string serialize(bool nested=false) const;

    // Resets the object end reads from an XML formatted string
    void deserialize (const std::string&, const std::string& = "");

private:
    ActionsMapType _actionsMap;
    ActionsMapType::iterator _actionsIt;
    bool _validIt;
    
};

_ACS_END_NAMESPACE


#endif //_MigrationReport_H_

