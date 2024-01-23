// PRQA S 1050 EOF
/*

	Copyright 2019-2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA PDS Db Library $

*/

#include <acs_c++config.hpp>
#include <FileTypeRecognition.h>

#include <RegExp.h>
#include <Filterables.h>
#include <exFMCFilterables.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <File.h>

#include <vector>
#include <set>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

using namespace std ;

Mutex FileTypeRecognition::_mutex ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(FileTypeRecognition)
map <string, time_t> 									FileTypeRecognition::_lastUpdateMap ;
map <string, FileTypeRecognition::FtypesCompType>		FileTypeRecognition::_ftypesMap ;
map <string, FileTypeRecognition::FtypeTraitsType>	FileTypeRecognition::_ftypeTraitsMap ;



FileTypeRecognition::FileTypeRecognition()
{
}



FileTypeRecognition::~FileTypeRecognition() throw()
{
}

string FileTypeRecognition::fname2RealFType(string const & fn, dbConnection & conn ) const // PRQA S 4214
{
    Mutex::Lock l (_mutex) ;
	loadMap(conn) ;
	
	const string &key = conn.getKey() ; 

	// This function will loop over all the filetypes retrieved on the given connection
	// and will
	// - skip any virtual filetype (regexp set to "")
	// - return on the first not virtual filetype with a matching filename	
	if (_ftypesMap.find(key) != _ftypesMap.end())
	{
		for (map<string, pair <string, vector<string> > >::iterator i=_ftypesMap[key].begin(); // PRQA S 4238
	    	 i != _ftypesMap[key].end() ; 
			 i++) 
		{

			if ((*i).second.first == "") // virtual ftype
			{ 
				continue ;
            }

			// Real file type at this point. Retrieve its regexp
			RegExp regex((*i).second.first) ;

			try 
			{
				if (regex.match(fn)) 
				{
					// The regexp matched the filename. Return the associated (real) filetype
					return (*i).first ;
                }
			}
			catch(exception &e) 
			{
				ACS_THROW(exIllegalValueException(e,"Checking filetype "+ (*i).first)) ; // PRQA S 3081
			}
		}
	}
	
	ACS_THROW(NotFound("Can't find a file type that matchs name \"" + fn + "\" .")); // PRQA S 3081
}


namespace {

    class RecursionGuard // PRQA S 2109
    {

        public:
            explicit RecursionGuard (int& i) : _counter(i) // PRQA S 2528
            {
                // increase the counter
                ++_counter;
            }
    
            ~RecursionGuard ()
            {
                // restore old value
                --_counter;
            }
    
        private:
            RecursionGuard ();
            RecursionGuard (const RecursionGuard&);
            RecursionGuard& operator= (const RecursionGuard&);

        private:
            int& _counter;
    };

}

bool FileTypeRecognition::fnameBelong2Ftype(string const & ftype, // PRQA S 4020, 4214
                                              string const & fname, 
                                              dbConnection & conn) const
{
    Mutex::Lock l(_mutex) ;

	// Search the real filetype associated to the filename
	string f = fnameBelong2RFtype (ftype, fname, conn);

	// if not found the string is empty
	return (not f.empty());
}

string FileTypeRecognition::fnameBelong2RFtype(string const & ftype, // PRQA S 4020, 4214
                                                 string const & fname, 
                                                 dbConnection & conn) const
{
    Mutex::Lock l(_mutex) ;

    static int recursionCounter = 0;
	static set<string> visitedFileTypes;

    if (0 == recursionCounter)
    {
        // allow map refresh only when no recursion is detected
    	loadMap(conn) ;

		// clean the visitedFileTypes set
		visitedFileTypes.clear();
    }

	// Avoid visiting loops verifying the visitedFileTypes structure and returning if the filetype has been already visited
	if ( (visitedFileTypes.insert (ftype)).second == false ) // Already inserted 
	{
		// Oops.. this is a loop. Signal it and go ahead 
		ACS_LOG_WARNING ("Found a virtual filetype loop involving the fileytpe " << ftype);
		// Clearly the relation has not been found
		return "";
	}

	// The recursionCounter will be increased here and will be restored by the "theGuard" DTOR
    RecursionGuard theGuard (recursionCounter);

	const string &key = conn.getKey() ; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"looking for ftype: " << ftype) ;
	
	// In case the given connection returned an empty filetype list
	// can simply return false and avoid any further processing
	if (_ftypesMap.find(key) == _ftypesMap.end()) { return "" ; }

	// Verify if the given filetype exists in the _ftypesMap related to the given connection key
	map<string, pair <string, vector<string> > >::iterator r = _ftypesMap[key].find(ftype) ;

	if (r == _ftypesMap[key].end()) 
	{ 
		// Sorry, not found.
		return "" ; 
	}

	// Ok found it. Go into the details now.
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"found ftype: " << r->first) ;
	
	if ( r->second.first == "")  // virtual ftype
	{
		// It is a virtual filetype... start visiting all the composing filetypes
		// Keep in mind that any "child" filetype can be a virtual one
		// so I need to keep track of all the visited filetypes to avoid loops
		// this is done using the "visitedFileTypes" structure
		for (size_t i=0; i < r->second.second.size(); i++) 
		{
			// recursively search any child filetype and, if found, exit now 
			string f = fnameBelong2RFtype(r->second.second[i], fname, conn) ;
			if (not f.empty()) 
			{
				return f ;
            }
        }

		// Not found in any of the filetypes composing the virtual one
		return "" ;
	}

	// If here the filetype was a real one.
	// Go checking the associated regexp 
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"checking fnameame: " << fname << " with regex: \"" << r->second.first << "\"") ;

	try 
	{
		RegExp regex(r->second.first) ;
		if (regex.match(fname))
		{
			// Found it
			return r->first;
		}
		// It did not match
		return "" ;
	}
	catch(exception &e) 
	{
		ACS_THROW(exIllegalValueException(e,"Checking filetype "+(*r).first)) ;
	}
}



vector<string> FileTypeRecognition::getRealFTypesList (const string& ftype, dbConnection& conn) const // PRQA S 4214
{
    Mutex::Lock l(_mutex) ;

	// Return the collection excluding the virtual filetypes
	vector<string> theList;
	internalGetFTypesList (ftype, conn, theList, false);

	return theList;

}

void FileTypeRecognition::internalGetFTypesList (const string& ftype, dbConnection& conn, vector<string>& theList, bool alsoVirtual) const // PRQA S 4020, 4214
{
    Mutex::Lock l(_mutex) ;

    static int recursionCounter = 0;
	static set<string> visitedFileTypes;

    if (0 == recursionCounter)
    {
        // allow map refresh only when no recursion is detected
    	loadMap(conn) ;

		// clean the visitedFileTypes set
		visitedFileTypes.clear();

		// clean also the given list
		theList.clear();
    }

	// Avoid visiting loops verifying the visitedFileTypes structure and returning if the filetype has been already visited
	if ( (visitedFileTypes.insert (ftype)).second == false ) // Already inserted 
	{
		// Oops.. this is a loop. Signal it and go ahead 
		ACS_LOG_WARNING ("Found a virtual filetype loop involving the fileytpe " << ftype);
		// Clearly the relation has not been found
		return;
	}

	// The recursionCounter will be increased here and will be restored by the "theGuard" DTOR
    RecursionGuard theGuard (recursionCounter);

	const string &key = conn.getKey() ; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"looking for ftype: " << ftype) ;
	
	// In case the given connection returned an empty filetype list
	// can simply return and avoid any further processing
	if (_ftypesMap.find(key) == _ftypesMap.end()) { return; }

	// Verify if the given filetype exists in the _ftypesMap related to the given connection key
	map<string, pair <string, vector<string> > >::iterator r = _ftypesMap[key].find(ftype) ;

	if (r == _ftypesMap[key].end()) 
	{ 
		// Sorry, not found.
		return; 
	}

	// Ok found it. Go into the details now.
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"found ftype: " << r->first) ;
	
	// detect if virtual ftype
	bool isVirtual = ( (r->second.first).empty());

	// Add it to the list if pertinent
	if ( ( not isVirtual ) || (isVirtual && alsoVirtual) )
	{
		theList.push_back (r->first);
	}

	if (isVirtual)
	{
		// Go through recursion to collect all the composing filetypes
		for (size_t i=0; i < r->second.second.size(); i++) 
		{
			// recursively search any child filetype and, if found, exit now 
			internalGetFTypesList (r->second.second[i], conn, theList, alsoVirtual) ;
        }
	}

	// Done!
}




string FileTypeRecognition::getRealRegExp(const string &ftype, dbConnection & conn) const // PRQA S 4214
{
    Mutex::Lock l(_mutex) ;
	loadMap(conn) ;
	
	const string key = conn.getKey() ; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"looking for ftype: " << ftype) ;
	
	if (_ftypesMap.find(key) == _ftypesMap.end()) 
	{ ACS_THROW(NotFound("Can't find a file type named \"" + ftype + "\" .")); } // PRQA S 3081

	map<string, pair <string, vector<string> > >::iterator r = _ftypesMap[key].find(ftype) ;
	ACS_COND_THROW(r == _ftypesMap[key].end(), NotFound("Can't find a file type named \"" + ftype + "\" .")); // PRQA S 3081	

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"found ftype: " << (*r).first) ;
	return (*r).second.first ;
}




FileTypeRecognition::ftype_traits FileTypeRecognition::getTraits(string const & ftype, dbConnection & conn) const // PRQA S 4214
{
    Mutex::Lock l (_mutex) ;
	
	if (! loadMapIfNeeded(ftype, conn)) {
		// ftype not found in the proper map 
		ACS_THROW(NotFound("Can't find a file type named \"" + ftype + "\" ."));	// PRQA S 3081
	}
		
	// WHEN HERE: method above returned TRUE i.e. entry _ftypeTraitsMap[conn.getKey()] does exist

	map<string,ftype_traits>::iterator r = _ftypeTraitsMap[conn.getKey()].find(ftype) ;

    return (*r).second ;
}


string FileTypeRecognition::getRegExp(const string &ftype, dbConnection & conn) const // PRQA S 4214
{
    Mutex::Lock l(_mutex) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"looking for ftype: " << ftype) ;

	string connKey = conn.getKey();
	
	if (_ftypesMap.find(connKey) == _ftypesMap.end()) 
	{ ACS_THROW(NotFound("Can't find a file type named \"" + ftype + "\" .")); } // PRQA S 3081

	map<string, pair <string, vector<string> > >::iterator r = _ftypesMap[connKey].find(ftype) ;
	ACS_COND_THROW(r == _ftypesMap[connKey].end(),NotFound("Can't find a file type named \"" + ftype + "\" ."));	 // PRQA S 3081

	vector<string> realFtypes = getRealFTypesList (ftype, conn);
	size_t realTypesListSize = realFtypes.size();

	string s;

	for (size_t idx=0; idx<realTypesListSize; ++idx) 
	{
		if (not s.empty())
		{
			s+="|";
		}
		// Add the new regexp taken from the map
		const string& currentFtype = realFtypes.at(idx);
		r = _ftypesMap[connKey].find(currentFtype) ;
		s += r->second.first ;
	}
	
	return s ; // PRQA S 4028
}

bool																	/* OUT: TRUE if filetype is mapped for given key, FALSE otherwise */		
		FileTypeRecognition::loadMapIfNeeded(	const string& ftype, \
												dbConnection & conn) 
{
    Mutex::Lock l(_mutex) ;
		
	// Obtain connection key: 
	const string key = conn.getKey() ; 
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Method called.  ftype: \"" << ftype << "\"  connection key: \"" << key << "\"" );

	bool found = false ;
    bool map_loaded = false ; 

	while (!found) /* The loop will be executed 1 or 2 times */
	{
		// Using the connection-key, ftypeTraits is filled with the proper map (or it's blank)
		if ( _ftypeTraitsMap.find(key) != _ftypeTraitsMap.end() ) {
			FtypeTraitsType& ftypeTraits ( _ftypeTraitsMap[key] ) ;
			found = ( ftypeTraits.find(ftype) != ftypeTraits.end() ) ; // TRUE if ftype found in proper map
		}
		
		if ( (!found) && (!map_loaded) ) 
		{ 
			// IF HERE: ftype not found and map not loaded yet 
			loadMap(conn) ; map_loaded = true ; 
		}
		else
		{ 
			// IF HERE: Either the ftype was found, -OR- the map was already loaded once
			break ; 
		}
	}	// END: while (!found) ; 
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. Returning: " << boolalpha << found) ;

	return found ; 
}

// Breaking down this function in sub-functions to keep low complexity and lengh indicators
// is not feasible due to functional implications
void FileTypeRecognition::loadMap(dbConnection & conn) // PRQA S 4020, 5500, 5510
{
    Mutex::Lock l(_mutex) ;
	
	// Obtain connection key: 
	const string key = conn.getKey() ; 

	if( _lastUpdateMap.find(key) == _lastUpdateMap.end()) { _lastUpdateMap[key] = 0 ; }	/* Undefined */

	time_t now = ::time(0) ;
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream upd ; 
        if (_lastUpdateMap[key] == 0) { 
            upd << "N/A" ; 
        } else { 
            upd << (now - _lastUpdateMap[key]) ; 
        }
		size_t s=0 ; 
		if (_ftypesMap.find(key) != _ftypesMap.end()) { s = _ftypesMap[key].size() ; }
		ACS_LOG_DEBUG(SimpleDebugSignature << "seconds since last update: " << upd.str() 
			   << " map size: " << s) ;
	ACS_CLASS_END_DEBUG
	
	if ( (_ftypesMap.find(key) != _ftypesMap.end())
		&& (now - _lastUpdateMap[key] < 30) && (! _ftypesMap[key].empty()) ) // PRQA S 4400
	{ 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Load not needing. Skipping") ;

		return ; 
	}
		
	dbSet set ;
	{
    	SQLString ftypesSql(key, SQLString::SELECT ) ;

    	ftypesSql.addSelectList("filetype"); 
		ftypesSql.addSelectList("nameregularexpression");
		ftypesSql.addSelectList("id_filetype");
		ftypesSql.addSelectList("mngsngext");
		ftypesSql.addSelectList("sngext");
		ftypesSql.addSelectList("hdrext");
		ftypesSql.addSelectList("dblext");
		ftypesSql.addSelectList("headerseparation");
    	ftypesSql.addTableList("t_filetypes");

		dbQuery query(conn, ftypesSql, "FTRSEL t_filetypes") ;
		query.execForSet(set) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"loading: " << set.size() << " filetypes") ;
	if (set.empty()) {
		return ;
    }
	_lastUpdateMap[key] = time(0) ;
	
	_ftypesMap.erase(key) ;
	_ftypeTraitsMap.erase(key) ; 
	
	FtypeTraitsType ftypeTraits ;
	FtypesCompType ftypesComp ;
	
	dbRow row(conn.getKey()) ;
	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		string ftype;
		row.column(0, ftype) ;
		bool isNull = false ;
        uint16_t colno = 1 ;
        string regex ;
		row.column(colno++, regex, &isNull) ;
		ftypesComp[ftype].first = isNull ? "" : regex ; // PRQA S 3380, 3385
        ftype_traits ftt ;
        row.column(colno++,ftt.id_filetype, &isNull) ;
        row.column(colno++,ftt.mngsngext, &isNull) ; 
        row.column(colno++,ftt.sngext, &isNull) ; if (isNull) { ftt.sngext = "" ; }
        row.column(colno++,ftt.hdrext, &isNull) ; if (isNull) { ftt.hdrext = "" ; }
        row.column(colno++,ftt.datext, &isNull) ; if (isNull) { ftt.datext = "" ; }
        row.column(colno++,ftt.header_separator, &isNull) ;
        ftypeTraits[ftype] = ftt ;
	    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"loaded: " << ftype << ": " << ftt.str()) ;
	}
	
	if (!ftypeTraits.empty()) {	_ftypeTraitsMap[key] = ftypeTraits ; }
	if (!ftypesComp.empty()) {	_ftypesMap[key] = ftypesComp ; }
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"loaded: " << set.size() << " filetypes") ;
	
	{
		SQLString ftypeHier(key, SQLString::SELECT ) ;
    	ftypeHier.addSelectList("ftparent.filetype"); 
		ftypeHier.addSelectList("ftchild.filetype");
    	ftypeHier.addTableList("t_filetypescomps");

        // The Join on parent
        {
            dbQueryParameters pp(key) ;
            pp.addParameter(dbParam(key, "t_filetypescomps","pfiletype" , dbParam::EQUAL_TO , "ftparent.id_filetype" )) ;
            ftypeHier.addJoinList(SQLDialect::INNER,"T_filetypes", "ftparent",pp) ;
        }

        // The Join on child
        {
            dbQueryParameters pp(key) ;
            pp.addParameter(dbParam(key, "t_filetypescomps","cfiletype" , dbParam::EQUAL_TO , "ftchild.id_filetype" )) ;
            ftypeHier.addJoinList(SQLDialect::INNER,"T_filetypes",  "ftchild",pp) ;
        }

		dbQuery query(conn, ftypeHier, "FTRSEL t_filetypescomps") ;
		query.execForSet(set) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"loading: " << set.size() << " rows for hierarchy") ;
	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		string parent ;
		row.column(0, parent) ;
        string child ;
		row.column(1, child) ;
				
		if ( _ftypesMap.find(key) == _ftypesMap.end() )
		{ FtypesCompType ftypesC1 ; _ftypesMap[key] = ftypesC1 ; }

		((_ftypesMap[key])[parent]).second.push_back(child) ;
	}
}


bool FileTypeRecognition::relationship(const std::string &desc,  const std::string &anc, dbConnection & conn) const  // PRQA S 4020, 4214
{
    Mutex::Lock l (_mutex) ;

	// Staring from the ancestor, get the list of all the descendent, and verify if desc is included
	vector<string> descList;

	// get all descendent, including virtual ones
	internalGetFTypesList (anc, conn, descList, true);

	// check if desc is included
	bool foundIt = ( find (descList.begin(), descList.end(), desc) != descList.end() );

	return foundIt;
}



// FIXME ENRCAR: this method should be modified in order NOT to load the map from db if ftype already handled by _ftypeTraitsMap
dbPersistent::IdType FileTypeRecognition::FTName2FTypeId(const std::string &ftype ,dbConnection & conn) 
{
    Mutex::Lock l (_mutex) ;
	loadMap(conn) ;

	const string key = conn.getKey() ; 

	map<string,ftype_traits >::iterator r = _ftypeTraitsMap[key].find(ftype) ;
	ACS_COND_THROW(r == _ftypeTraitsMap[key].end(),NotFound("Can't find a file type named \"" + ftype + "\" .")); // PRQA S 3081	
    return (*r).second.id_filetype ;
}

// PRQA S 2171, 2173, 2175, 2180, 2185, 4211 L1
struct id_compare { 
    explicit id_compare(dbPersistent::IdType i) : id_(i) {} 
 
    bool operator()(const pair <std::string, FileTypeRecognition::ftype_traits > &me) 
    { 
        return (me.second.id_filetype == id_) ; 
    }
private:
    dbPersistent::IdType id_ ;
} ;
// PRQA L:L1


// FIXME ENRCAR: this method should be modified in order NOT to load the map from db if IdType already contained into _ftypeTraitsMap
std::string FileTypeRecognition::FTypeId2FTName(dbPersistent::IdType id,dbConnection & conn ) 
{
    Mutex::Lock l (_mutex) ;
	loadMap(conn) ;

	const string key = conn.getKey() ; 
	
	map<string,ftype_traits >::iterator r = 
        find_if(_ftypeTraitsMap[key].begin(), _ftypeTraitsMap[key].end(),id_compare(id) ) ; // PRQA S 3081
    ostringstream os ;
    os << "Can't find file type id: " << id << "." ;
	ACS_COND_THROW(r == _ftypeTraitsMap[key].end(),NotFound(os.str()));	 // PRQA S 3081
    return (*r).first;
}

bool FileTypeRecognition::isSNG(const string &ftype, // PRQA S 4020
                                  const string &fname,
                                  dbConnection & conn,
								  bool ignoreExtCase) 
{
	FileTypeRecognition ftr ;
	FileTypeRecognition::ftype_traits ftt = ftr.getTraits(ftype, conn) ;
	const std::string &ext = File::getExt(fname) ;
	if (ignoreExtCase) {
		return StringUtils::equalsNoCase(ext, ftt.sngext) ;
	}
	return ext == ftt.sngext ;
}	

bool FileTypeRecognition::isHDR(const string &ftype, // PRQA S 4020
                                  const string &fname,
                                  dbConnection & conn,
								  bool ignoreExtCase) 
{
	FileTypeRecognition ftr ;
	FileTypeRecognition::ftype_traits ftt = ftr.getTraits(ftype, conn) ;
	const std::string &ext = File::getExt(fname) ;
	if (ignoreExtCase) {
		return StringUtils::equalsNoCase(ext, ftt.hdrext) ;
	}
	return ext == ftt.hdrext ;
}	

bool FileTypeRecognition::isDAT(const string &ftype, // PRQA S 4020
                                  const string &fname,
                                  dbConnection & conn,
								  bool ignoreExtCase) 
{
	FileTypeRecognition ftr ;
	FileTypeRecognition::ftype_traits ftt = ftr.getTraits(ftype, conn) ;
	const std::string &ext = File::getExt(fname) ;
	if (ignoreExtCase) {
		return StringUtils::equalsNoCase(ext, ftt.datext) ;
	}
	return ext == ftt.datext ;
}	

bool FileTypeRecognition::checkFNameExtension(const string &ftype, // PRQA S 4020 
                                			   const string &fname,
												ExtensionInfo & extInfo,
                                			   dbConnection & conn ) 
                        
{
	
    Mutex::Lock l (_mutex) ;
	
	
	if ( ! loadMapIfNeeded(ftype, conn) ) {
		ACS_THROW(NotFound("Can't find a file type named \"" + ftype + "\" .")); // PRQA S 3081
	} 
		
	// WHEN HERE: method above returned TRUE i.e. entry _ftypeTraitsMap[conn.getKey()] does exist
    const string &dbKey = conn.getKey() ;
	map<string,ftype_traits >::iterator	r = _ftypeTraitsMap[dbKey].find(ftype) ;
    ACS_COND_THROW(r == _ftypeTraitsMap[dbKey].end(),NotFound("Can't find a file type named \"" + ftype + "\" .")) ; // PRQA S 3081 
    const std::string &ext = File::getExt(fname) ;

	if( r->second.header_separator ) {
     	if (ext == r->second.hdrext) {
			extInfo.extId = HDR;
        	extInfo.expectedExtNr = 2; // PRQA S 4400
        	extInfo.otherExt = r->second.datext;
        	extInfo.mngSngExt = r->second.mngsngext ;
        	return true ;
    	}
    	else if (ext == r->second.datext) {
			extInfo.extId = DAT;
			extInfo.expectedExtNr = 2; // PRQA S 4400
			extInfo.otherExt = r->second.hdrext;
			extInfo.mngSngExt = r->second.mngsngext ;
        	return true ;
    	}
        else { /* nothing */ }
	}
	else { // r->second.header_separator is FALSE
   		if (r->second.sngext.empty() || ext == r->second.sngext) {
       		extInfo.extId = SNG;
        	extInfo.expectedExtNr = 1;
        	extInfo.otherExt = "";
        	extInfo.mngSngExt = r->second.mngsngext ;
            if (r->second.sngext.empty() && r->second.mngsngext) {
                ACS_LOG_WARNING("Filetype " << ftype << " misconfigured - single estension is empty or null, but mange single extension is true. Please correct.") ;
            }
        	return true ;
    	}
	}	

// return the object after assigment of a new instance for reset.
	extInfo = ExtensionInfo();
    return false ;
}                                                 

std::string FileTypeRecognition::guessMtdFileNameWExtension( // PRQA S 4020, 4214
                                    const std::string &ftype, 
                                    const std::string &fname,
                                    dbConnection &conn) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"ftype: " << ftype << " fname: \"" << fname << "\"") 
    
    Mutex::Lock l (_mutex) ;
	
	
	if ( ! loadMapIfNeeded(ftype, conn) ) {
		ACS_THROW(NotFound("Can't find a file type named \"" + ftype + "\" .")); // PRQA S 3081
	} 
    const string &dbKey = conn.getKey() ;
	map<string,ftype_traits >::iterator	r = _ftypeTraitsMap[dbKey].find(ftype) ;
    ACS_COND_THROW(r == _ftypeTraitsMap[dbKey].end(),NotFound("Can't find a file type named \"" + ftype + "\" .")) ; // PRQA S 3081 
    const std::string &fext = File::getExt(fname) ;
    /*
        mngsngext = false
        fname <ANY> -> <ANY>
        
        mngsng = true sngext = BBB
        
        fname AAA -> AAA (no ext in mtd)
        fname AAA.BBB -> AAA (no ext in mtd)
        fname AAA.CCC -> ? 
        
    */
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"mngsngext: " << boolalpha << r->second.mngsngext) 
    if (r->second.mngsngext) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"fname ext is: \"" << fext << "\"") 
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"sngext is: \"" << r->second.sngext << "\"") 
        
        if (fext.empty()) {
            return fname ;
        }
        else if (fext == r->second.sngext ) {
            return File::removeExt(fname) ;
        }
        else {
            return fname ;
        }
        
    }
    else {
        return fname ;
    }
}                                


ostream &operator << (ostream &os,FileTypeRecognition::ExtensionId i)
{
    switch(i) {
    case FileTypeRecognition::UNDEFINED: { os <<  "UNDEFINED" ; break ; }
    case FileTypeRecognition::SNG:       { os <<  "SNG" ; break ; }
    case FileTypeRecognition::HDR:       { os <<  "HDR" ; break ; }
    case FileTypeRecognition::DAT:       { os <<  "DAT" ; break ; }
    default:
        { 
             os << "<unknown FileTypeRecognition::ExtensionId: " << int(i) << ">" ; // PRQA S 3081
            break ;
        }
    }

    return os ; // PRQA S 4028
}

acs::exostream &operator << (acs::exostream &os ,FileTypeRecognition::ExtensionId i) // PRQA S 4222
{
    switch(i) {
    case FileTypeRecognition::UNDEFINED: { os << "UNDEFINED" ; break ; }
    case FileTypeRecognition::SNG:       { os << "SNG" ; break ; }
    case FileTypeRecognition::HDR:       { os << "HDR" ; break ; }
    case FileTypeRecognition::DAT:       { os << "DAT" ; break ; }
    default:
        { 
            os << "<unknown FileTypeRecognition::ExtensionId: " << int(i) << ">" ; // PRQA S 3081
            break ;
        }
    }

    return os ; // PRQA S 4028
}

_ACS_END_NESTED_NAMESPACE
