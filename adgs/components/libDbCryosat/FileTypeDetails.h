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

	$Prod: A.C.S. DataBase Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/03/29 16:27:33  marpas
	changed FileTypeDetails to adhere to new db i/f and calling protocols.
	TL uses it during precondition check
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2015/12/01 16:07:15  marpas
	coding best practices applied, improved robustness
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.0  2013/09/30 12:30:51  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.1  2013/09/23 18:49:35  marpas
	some qa warnings removed
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.5  2013/03/11 16:08:54  marfav
	Connection passed to the initialization function
	
	Revision 2.4  2013/02/01 11:44:23  marfav
	Friedship to Singleton added
	
	Revision 2.3  2013/01/25 11:55:32  marpas
	using pattern::Singleton from libException
	
	Revision 2.2  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2009/03/06 16:48:04  marfav
	Added FileTypeDetails class
	
    
*/

#ifndef _FileTypeDetails_H_
#define _FileTypeDetails_H_


#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <ThreadSafe.h>

#include <string>
#include <map>

#include <sstream>
#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::ParamInitializableSingleton ;
using pattern::Singleton ;


// Template class
// Creating a LUT between a key and a data structure in a database table
template <typename Key, typename Data>
class DbLUT : protected ThreadSafe // PRQA S 2109, 2194
{
public:
    DbLUT () : ThreadSafe(), _values() {}
    virtual ~DbLUT() throw() {} // PRQA S 2131

    // returns the data associated with the given key
    // an exception of type exIllegalValueException is thrown if key is not found
    Data getData (const Key& key) const
    {
        Lock lock(*this);
	    typename std::map<Key, Data>::const_iterator iter = _values.find(key);
	    if (iter == _values.end() )
	    {
		    std::ostringstream os ; 
		    os << "Requested key " << key << " not found "; 
		    ACS_THROW(exIllegalValueException(os.str())) ;  // PRQA S 3081
	    }
	    return iter->second ; 
    } 


    // returns the key associated with the given data
    // an exception of type exIllegalValueException is thrown if data is not found
    Key getKey (const Data& data) const
    {
        Lock lock(*this);
        typename std::map<Key, Data>::const_iterator i;
	    for(i=_values.begin(); i != _values.end() ; i++)
	    {
            if ( i->second == data )
            {
                return i->first ; 
            }
	    }

        std::ostringstream os;
        os << "Requested data \"" << data << "\" not found";
	    ACS_THROW(exIllegalValueException(os.str())) ;  // PRQA S 3081
    }

	// return the element number
	unsigned int size() const { Lock lock(*this); return _values.size(); } 

    // return the i-th column
	Data operator[](unsigned int idx) const // PRQA S 2141
    {
        Lock lock(*this);
	    if (idx >= size()) {
		    std::ostringstream os ;
		    os << "Cannot addrss element # " << idx << " in a collection of " << size() << " elements " ;
		    ACS_THROW(exIllegalValueException(os.str())) ;  // PRQA S 3081
	    }

	    typename std::map<Key,Data>::const_iterator i=_values.begin() ;
        advance(i,idx) ;
	    return i->second ; 

    }

	// return the i-th key
	Key keyByPos(unsigned int idx) const 
    {
        Lock lock(*this);
	    if (idx >= size()) {
		    std::ostringstream os ;
		    os << "Cannot addrss element # " << idx << " in a collection of " << size() << " elements " ;
		    ACS_THROW(exIllegalValueException(os.str())) ;  // PRQA S 3081
	    }

	    typename std::map<Key, Data>::const_iterator i=_values.begin() ;
        advance(i, idx) ;
	    return i->first ; 

    }


protected:

    // Virtual function to load the _values structure
    virtual void load(const std::string &) = 0;

    virtual void init(const std::string& value) { load(value); } // PRQA S 2131
    void clear() { _values.clear() ; }
    Data &value (const Key &k) { return _values[k] ; }
private:
    DbLUT (const DbLUT&); // not implemented
    DbLUT& operator= (const DbLUT&); // not implemented

private:
    // Attributes
    std::map <Key, Data> _values;


};


// Struct containing the information about filetypes
struct FileTypeDetailsStruct // PRQA S 2173, 2175
{
    FileTypeDetailsStruct() : manageSingleExt(), singleExt(), hdrExt(), dblExt(), regExp() {}
    bool manageSingleExt; // PRQA S 2100 5
    std::string singleExt;
    std::string hdrExt;
    std::string dblExt;
    std::string regExp;
};


class FileTypeDetails : // PRQA S 2109, 2153
    public ParamInitializableSingleton<FileTypeDetails, std::string>, 
    public DbLUT<std::string, FileTypeDetailsStruct> 
{
    // Friendship needed by the Singleton Template pattern
    friend FileTypeDetails* ParamInitializableSingleton<FileTypeDetails, std::string>::instance(const std::string&); // PRQA S 2107 2 
    friend FileTypeDetails* Singleton<FileTypeDetails>::instance();

public:

    // DTOR
    virtual ~FileTypeDetails() throw();


protected:

    // It is a singleton !!
    FileTypeDetails();

    // function that loads the values from the database
    virtual void load(const std::string &);

    static const std::string defaultInitValue; // PRQA S 2101

private:
    FileTypeDetails (const FileTypeDetails&); // not implemented
    FileTypeDetails& operator= (const FileTypeDetails&); // not implemented
    void internalLoad(const std::string &);

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(FileTypeDetails)
};


// functions definition
std::ostream &operator<<(std::ostream &, const FileTypeDetailsStruct &) ;  
exostream &operator<<(exostream &, const FileTypeDetailsStruct &) ; // PRQA S 2072 

_ACS_END_NAMESPACE




#endif //_FileTypeDetails_H_

