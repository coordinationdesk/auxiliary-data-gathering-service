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

#include <MigrationReport.h>
#include <XMLOstream.h>
#include <UtilXml.h>

#include <vector>


using namespace std;
using namespace acs;

// Class Ctor - does nothing
MigrationReport::MigrationReport() :
    _actionsMap(),
    _actionsIt (_actionsMap.begin()),
    _validIt (true)
{} 


// Copy and assignment will reset the internal iterator

// Copy-CTOR
MigrationReport::MigrationReport (const MigrationReport& m) :
    _actionsMap (m._actionsMap),
    _actionsIt (_actionsMap.begin()),
    _validIt (true)
{}

// Assignment operator
MigrationReport& MigrationReport::operator= (const MigrationReport& m)
{
    if (this != &m)
    {
        _actionsMap = m._actionsMap;
        _actionsIt = _actionsMap.begin();
        _validIt = true;
    }
    return *this;
}


// clean-up everything 
void MigrationReport::reset()
{
    _actionsMap.clear();
    resetIterator();
}

// Brings back the iterator to the begin of the structure and makes it valid
void MigrationReport::resetIterator()
{
    _actionsIt = _actionsMap.begin();
    _validIt = true;
}

bool MigrationReport::isEmpty() const
{
	return _actionsMap.empty();
}

// It will reset the iterator and start back from the beginning in case 
// the iterator is no more valid
bool MigrationReport::getNextItem(ActionItem& action)
{
    bool retval = false;
    // If the iterator is not valid reset it
    if (!_validIt)
    {
        resetIterator();
    }
    if (_actionsIt != _actionsMap.end())
    {
        // Return something only if there is something to return
        action = *_actionsIt;
        ++_actionsIt;
        retval= true;
    }
    return retval;
}


// Inserts a new action in the map
// it invalidates the internal iterator
void MigrationReport::setAction (const string& id, const string& action)
{
    _actionsMap[id] = action;
    _validIt = false;
}

// Returns the action associated to an id 
// In case no action is found the empty string is returned
// and the boolean is set accordingly (if passed)
string MigrationReport::getAction (const string& id, bool* valid) const
{
    string retval("");

    ActionsMapType::const_iterator it = _actionsMap.find(id);
    bool found = (it != _actionsMap.end());

    // Return also the valid flag if possibile
    if (valid != 0)
    {
        *valid = found;
    }

    // if found return the action
    if (found)
    {
        retval = it->second;
    }

    return retval;
}

string MigrationReport::serialize(bool nested) const
{

    // convert the id -> action map in a reverse map
    // action -> list_of_ids
    typedef map < string, vector <string> > ReverseMapType;
    ReverseMapType reverseMap;

    for (ActionsMapType::const_iterator it=_actionsMap.begin(); it!=_actionsMap.end(); ++it)
    {
        reverseMap[it->second].push_back(it->first);   
    }

    // Start with the report
    ostringstream os;
    XMLOstream xst(os,"MigrationReport");

    for (ReverseMapType::iterator it=reverseMap.begin(); it!=reverseMap.end(); ++it) // PRQA S 4238
    {
        XMLOstream::Tag actiontag (xst,"Action", "", "type=\""+it->first+"\"", false);

        vector<string>& ids = it->second;
    
        // For each type of action create a section
        size_t ids_size = ids.size() ;
        for (size_t idx=0; idx<ids_size; ++idx)
        {
            XMLOstream::Tag uniqueidtag (xst, "uniqueId", ids[idx], "", true);
        }
    }

    xst.close();

    // remove 3 consecutive white spaces and cr+lf
    string result = os.str();

    // If nested remove the XML header (the first line)
    if (nested)
    {
        result.erase (0, result.find ("\n"));
    }

    size_t pos = 0 ;
    while ( (pos = result.find("   ")) != string::npos) // PRQA S 4244
    {
        result.erase (pos, 3); // PRQA S 4400
    }
    while ( (pos = result.find("\n")) != string::npos) // PRQA S 4244
    {
        result.erase (pos, 1);
    }

    return result;;
}

void MigrationReport::deserialize (const string& xmls, const string& basepath)
{
    this->reset(); 

    // If deserialize from an empty string just reset the object
    if (!xmls.empty())
    {
        //*/Read Parameters File.
        XmlDocSptr xmlDocSptr( new XmlDoc );
        xmlDocSptr->initWithString (xmls);

        string xpathBase = ( basepath.empty() ? "//" : basepath+"/" ) + "Action"; // PRQA S 3380, 3385

	    vector<XmlDocSptr> actions= xmlDocSptr->xPath(xpathBase);
	    for ( vector<XmlDocSptr>::iterator it = actions.begin(); it!=actions.end(); ++it ) // PRQA S 4238
        {
		    string actionType = (*it)->xPath("/Action/@type").at(0)->str() ;
            vector <XmlDocSptr> items = (*it)->xPath("//uniqueId/text()");
        	for ( vector<XmlDocSptr>::iterator it2 = items.begin(); it2!=items.end(); ++it2 ) // PRQA S 4238
            {
                string uniqueid = (*it2)->str() ;
                this->setAction(uniqueid, actionType);
            }
        }
    }
}



