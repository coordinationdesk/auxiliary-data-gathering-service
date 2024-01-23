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

	$Prod: A.C.S. Resources Library $

	$Id$

	$Author$

	$Log$
	Revision 6.8  2017/11/23 10:06:10  marpas
	regression fixed after last code revision
	
	Revision 6.7  2017/11/21 14:51:57  marpas
	coding best practice applied
	
	Revision 6.6  2017/11/20 16:51:20  marpas
	useless conversion removed
	
	Revision 6.5  2017/10/31 12:55:11  marpas
	bug fixing in extending boolean values with illegal strings
	
	Revision 6.4  2014/12/09 14:13:46  marpas
	minor change
	
	Revision 6.3  2014/02/07 18:44:33  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 6.2  2013/10/03 18:03:55  marpas
	minor changes
	
	Revision 6.1  2013/07/17 09:23:05  marpas
	getValue and setValue are now template methods that can be spacialized to get/set data structures
	see rsSetGetArrayTest and rsValueTest for examples
	
	Revision 6.0  2013/07/16 17:00:44  marpas
	getArraySize returns 0 if the array is not present
	returns 1 if the array is not a real array but a single value
	getArray and setArray are now template methods managing the above two statements.
	
	Revision 5.0  2013/06/11 09:29:11  marpas
	adopting libException 5.x standards
	
	Revision 2.16  2013/02/22 13:56:41  marpas
	rsResourceSet does not longer inherit virtually from ThreadSafe, it has an internal mutex instead
	Interface rationalization
	coding best practices
	
	Revision 2.15  2012/11/20 22:39:55  marpas
	total quality: code, coverage etc.
	
	Revision 2.14  2012/07/26 17:30:14  marfav
	Addedd root access to resources with leading double slash
	
	Revision 2.13  2012/02/20 15:04:15  marpas
	refactoring
	
	Revision 2.12  2012/02/08 16:23:15  marpas
	deprecated macro removed
	
	Revision 2.11  2012/02/07 10:24:34  marpas
	removing compiler warnings
	
	Revision 2.10  2012/01/31 18:13:57  marpas
	compiler warnings removed
	
	Revision 2.9  2011/03/01 11:24:19  marpas
	GCC 4.4.x support
	
	Revision 2.8  2009/07/01 16:46:04  marpas
	minor changes
	
	Revision 2.7  2008/09/17 16:38:28  marpas
	new methods only for x86_64 ... (64 bits)
	void rsResourceSet::setArray(const std::string &, const std::vector<unsigned long> &, size_t num=0) ;
	void rsResourceSet::getArray(const std::string &, std::vector<unsigned long> &, size_t n=0, size_t offset = 0) const ;
	
	Revision 2.6  2008/01/21 16:15:04  clanas
	fixed wrong ifdef
	
	Revision 2.5  2008/01/21 15:28:06  clanas
	added implementation for setArray of unsigned int for 64bit
	
	Revision 2.4  2008/01/21 15:07:11  clanas
	added unsigned int& get funtion for 64bit platform
	
	Revision 2.3  2008/01/21 10:43:54  clanas
	fixed for 64bit port
	
	Revision 2.2  2006/05/29 08:40:56  marpas
	setArray for vector<float|size_t> implemented
	
	Revision 2.1  2006/05/23 10:38:24  marpas
	getArray method overloaded for vector<float|size_t>
	
	Revision 2.0  2006/02/28 08:34:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.22  2005/06/09 19:27:21  marpas
	signed & unsigned long long int supported
	
	Revision 1.21  2005/03/01 18:52:26  marpas
	long double supporto added
	
	Revision 1.20  2005/01/12 13:33:58  marpas
	headers fixed
	
	Revision 1.19  2004/07/20 08:57:18  marpas
	some method to use exostream
	
	Revision 1.18  2004/04/14 09:21:55  marpas
	ThreadSafe class used
	
	Revision 1.17  2003/11/06 19:37:49  marpas
	Fixed a bug in rsResources::merge
	
	Revision 1.16  2003/09/25 08:47:36  achval
	resourceSet can be deleted even if some rsPushNSpace is not yet out of scope
	
	Revision 1.15  2003/07/31 15:38:42  marpas
	compareContents renamed to isEqual
	
	Revision 1.14  2003/07/31 15:34:42  marpas
	rsResourceSet::compareContents method added
	
	Revision 1.13  2003/07/28 21:45:43  marpas
	_trueString and _falseString were not copied in operator=
	
	Revision 1.12  2003/07/15 18:14:29  marpas
	operator= potential bug removed
	
	Revision 1.11  2003/07/15 18:10:36  marpas
	global namespace concept implemented
	
	Revision 1.10  2003/07/11 12:56:42  marpas
	exException::clone implemented
	
	Revision 1.9  2003/06/24 16:09:48  marpas
	some namespaces related functions are now protected due to improper use.
	namespace computation take into account empty namespace
	
	Revision 1.8  2003/04/30 09:20:38  marpas
	using namespace std removed from includes
	
	Revision 1.7  2003/04/17 10:18:12  marpas
	get rid of carriage return if any
	
	Revision 1.6  2003/04/15 18:00:34  marpas
	GCC3.2
	
	Revision 1.5  2003/03/12 14:11:04  marpas
	multithread bug fixed
	
	Revision 1.4  2003/03/11 17:46:23  marpas
	performances slightly improved
	
	Revision 1.3  2003/03/10 18:23:51  marpas
	Now is really thread safe !
	
	Revision 1.2  2002/11/29 16:38:01  marpas
	STL use improved.
	
	Revision 1.1.1.1  2002/10/03 20:54:59  marpas
	libResources import
		

*/

#include <rsResourceSet.h>
#include <Filterables.h>
#include <rsPushNSpace.h>
#include <StringUtils.h>
#include <rsV.h>


#include <sstream>
#include <iomanip>

#ifdef _AIX
#include <algorithm>
#elif defined __linux__
  #if (__GNUC__ < 3)
	#include <algorithm>
  #elif (__GNUC__ == 4)
	#include <algorithm>
  	#include <iterator>
  #else
  	#include <iterator>
  #endif
#endif

#include <cstdlib>
#include <cstring>
#include <cctype>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    rsV version ;
}


//////////////////////////
//						//
// classe rsValue		//
//						//
//////////////////////////
const std::string rsResourceSet::rsValue::literal_false = "false" ;
const std::string rsResourceSet::rsValue::literal_true = "true" ;

rsResourceSet::rsValue::rsValue(const string &name) :
    _comments(),
	_key(name),
	_value("")
{
}


rsResourceSet::rsValue::rsValue(const rsResourceSet::rsValue &r) :
    _comments(r._comments),
	_key(r._key),
	_value(r._value)

{
}



rsResourceSet::rsValue::~rsValue() throw() 
{
}



rsResourceSet::rsValue &rsResourceSet::rsValue::operator=(const rsValue &v)
{
	if (this != &v) {
		_comments = v._comments ;
		_value = v._value ;
		_key = v._key ;
	}
	
	return *this ;
}

bool operator==(const rsResourceSet::rsValue &r1, const rsResourceSet::rsValue &r2) throw() 
{
	return (r1._key == r2._key) && (r1._value == r2._value) && (r1._comments == r2._comments) ;
}

const string &rsResourceSet::rsValue::key() const // PRQA S 4120
{
	return _key ;
}

// per prendere il valore
template<> void rsResourceSet::rsValue::get<string>(string &v) const { v = value() ; }


template<>
void rsResourceSet::rsValue::get<char>(char &v) const { v = _value[0] ; }


template<>
void rsResourceSet::rsValue::get<unsigned char>(unsigned char &v) const
{
	v = static_cast<unsigned char>(strtoul(_value.c_str(), 0, 10)) ; // PRQA S 3081, 4400
}


template<>
void rsResourceSet::rsValue::get<long double>(long double &v) const
{
	char *p ; // PRQA S 4101
	v = strtold(_value.c_str(), &p) ;
}


template<>
void rsResourceSet::rsValue::get<double>(double &v) const
{
	v = atof(_value.c_str()) ;
}


template<>
void rsResourceSet::rsValue::get<float>(float &v) const
{
	v = float(atof(_value.c_str())) ; // PRQA S 3081
}


template<>
void rsResourceSet::rsValue::get<long>(long &v) const
{
	v = atol(_value.c_str()) ;
}


template<>
void rsResourceSet::rsValue::get<short>(short &v) const
{
	v = short(atoi(_value.c_str())) ; // PRQA S 3081
}


template<>
void rsResourceSet::rsValue::get<int>(int &v) const
{
	v = atoi(_value.c_str()) ;
}


template<>
void rsResourceSet::rsValue::get<unsigned long>(unsigned long &v) const
{
	v = strtoul(_value.c_str(), 0, 10) ; // PRQA S 4400
}


template<>
void rsResourceSet::rsValue::get<unsigned short>(unsigned short &v) const
{
	v = static_cast<unsigned short>(strtoul(_value.c_str(), 0, 10)) ; // PRQA S 3081, 4400
}


template<>
void rsResourceSet::rsValue::get< long long signed int>( long long signed int &v) const
{
	v = strtoll(_value.c_str(), 0, 10) ;  // PRQA S 4400
}

template<>
void rsResourceSet::rsValue::get< long long unsigned int>(long long unsigned int &v) const
{
	v = strtoull(_value.c_str(), 0, 10) ;  // PRQA S 4400
}

template<>
void rsResourceSet::rsValue::get<unsigned int>(unsigned int &v) const
{
	v = size_t(strtoul(_value.c_str(), 0, 10)) ; // PRQA S 3010, 3081, 4400
}


void rsResourceSet::rsValue::get(bool &v, const string &fs, const string &ts) const // PRQA S 4020
{
	switch(_value[0]) {
	case 'O':
	case 'o':
		// try againt On or Off
		{
			if (StringUtils::equalsNoCase(_value, "on")) 
			{
				v = true ;
				return ;
			}
			
			if (StringUtils::equalsNoCase(_value, "off")) 
			{
				v = false ;
				return ;
			}
		}
		break ;
	case 'Y':
	case 'y':
		// could be yes
		{
			if (StringUtils::equalsNoCase(_value, "yes")) 
			{
				v = true ;
				return ;
			}
		}
		break ;
	case 'n':
	case 'N':
		// could be no
		{
			if (StringUtils::equalsNoCase(_value, "no")) 
			{
				v = false ;
				return ;
			}
		}
		break ;
	case 'T':
	case 't':
        { 
		    if (_value.length() == 1) {
			    v = true ;
			    return ;
		    }
        }
        break ;
	case 'F':
	case 'f': 
		{
            if (_value.length() == 1) {
			    v = false ;
			    return ;
		    }
        }
        break ;
    default:  
        {
            break ;
        }
	}

	if (!ts.empty() && StringUtils::equalsNoCase(_value, ts)) 
	{
		v = true ;
		return ;
	}

	if (!fs.empty() && StringUtils::equalsNoCase(_value, fs))
	{
		v = false ;
		return ;
	}

	if (StringUtils::equalsNoCase(_value, literal_true)) {
		v = true ;
    }
	else {
		if (StringUtils::equalsNoCase(_value, literal_false)) {
			v = false ;
        }
		else {
			v = bool(atol(_value.c_str())) ; // PRQA S 3081
        }
	}
}

void rsResourceSet::rsValue::setString(const string &s) 
{
	strip(_value,s) ;
}


void rsResourceSet::rsValue::set( const char * v)
{
	strip(_value,v) ;
}

template <> void rsResourceSet::rsValue::set<string>(const string &v)
{
	strip(_value,v) ;
}

template <> void rsResourceSet::rsValue::set<char >(const char &v)
{
	setString(string(1,v)) ;
}

template <> void rsResourceSet::rsValue::set<unsigned char>(const unsigned char &v)
{
	ostringstream os ;
	os << unsigned(v) ; // PRQA S 3081
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<long double>(const long double &v)
{
	ostringstream os ;
	os << setiosflags(ios::fixed) << setprecision(20) << v ; // PRQA S 4400
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<double>(const double &v)
{
	ostringstream os ;
	os << setiosflags(ios::fixed) << setprecision(17) << v ; // PRQA S 4400
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<float>(const float &v)
{
	ostringstream os ;
	os << setiosflags(ios::fixed) << setprecision(8) << v ; // PRQA S 4400
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<long>(const long &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<short>(const short &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}


template <> void rsResourceSet::rsValue::set<int>(const int &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<unsigned long>(const unsigned long &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<unsigned short>(const unsigned short &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<signed long long int>(const signed long long int &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<unsigned long long int>(const unsigned long long int &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<unsigned int>(const unsigned int &v)
{
	ostringstream os ;
	os << v ;
	setString(os.str()) ;
}

template <> void rsResourceSet::rsValue::set<bool>(const bool &v)
{
	_value = (v ? "true" : "false") ;  // PRQA S 3380
}

void rsResourceSet::rsValue::comments(const vector<string> &c)
{
	_comments.erase(_comments.begin(),_comments.end()) ;
	string tmp ;
	for (size_t i = 0; i < c.size(); ++i) {
		strip(tmp,c[i]) ;
        // PRQA S 3380, 3385 4
		_comments.push_back(((tmp[0] != '#' ) && ( tmp[0] != '!') && (tmp[0] != ';')) ? 
                                ("! " + tmp) :  // PRQA S 3081
                                tmp) ;
	}
}

const vector<string> &rsResourceSet::rsValue::comments() const // PRQA S 4120
{
	return _comments ;
}

bool rsResourceSet::rsValue::existComment(const string &cmt)
{
	return find(_comments.begin(),_comments.end(),cmt) != _comments.end() ;
}

void rsResourceSet::rsValue::mergeComments(const vector<string> &c)
{
	string tmp ;
	for (size_t i = 0; i < c.size(); ++i) {
		strip(tmp,c[i]) ;
         // PRQA S 3380, 3384, 3385 4
		string cmt = ((tmp[0] != '#') && (tmp[0] != '!') && (tmp[0] != ';')) ? 
                        ("! " + tmp) : // PRQA S 3081
                        tmp ;
		if (!existComment(cmt)) { _comments.push_back(cmt) ; }
	}
}

ostream &rsResourceSet::rsValue::write(ostream &os, char sep, bool cmt) const
{
	if (cmt) { writeComments(os) ; }

	// key<sep> value
	os << _key << sep << _value << '\n' ;
	if (os.rdstate()) { // PRQA S 3054
		ostringstream o ;
		o << "Write error writing key " << _key << " whose value is " << _value ;
		exIOException e(o.str(),errno) ;
		ACS_THROW(exIOException(o.str(),errno)) ;
	}
	
	return os ; // PRQA S 4028
}

exostream &rsResourceSet::rsValue::write(exostream &os, char sep, bool cmt) const
{
	try {
		if (cmt) { writeComments(os) ;}

		// key<sep> value
		os << _key << sep << _value << '\n' ;
	}
	catch(exception &x) {
		ostringstream o ;
		o << "Error writing key " << _key << " whose value is " << _value ;
		ACS_THROW(exIOException(x,o.str(),errno)) ;
	}
	
	return os ;// PRQA S 4028
}



void rsResourceSet::rsValue::writeComments(ostream &os) const
{
	copy(_comments.begin(), _comments.end(),ostream_iterator<string>(os,"\n")) ;
	if (os.rdstate()) { // PRQA S 3054
		ostringstream o ;
		o << "Write error writing comments for key " << _key << " whose value is " << _value ;
		exIOException e(o.str(),errno) ;
		ACS_THROW(exIOException(o.str(),errno)) ;
	}
}


void rsResourceSet::rsValue::writeComments(exostream &os) const
{
	try {
		for (size_t i=0; i < _comments.size(); ++i) {
			os << _comments[i] << "\n" ;
        }
	}
	catch(exception &x) {
		ostringstream o ;
		o << "Error writing comments for key " << _key << " whose value is " << _value ;
		ACS_THROW(exIOException(x,o.str(),errno)) ;
	}
}


















//////////////////////////////////
//								//
// classe rsResourceSet			//
//								//
//////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(rsResourceSet) 
const char rsResourceSet::_defaultWSeparator = ':' ;
const char rsResourceSet::_alternateWSeparator = '=' ;

rsResourceSet::rsResourceSet() :
    _nameSpaces(),
    _nSpacesMap(),
	_writeCmts(_defaultWriteComments),	
	_mergeMode(ModeNoChange),
    _nameSpacesMap(),
	_wSeparator(_defaultWSeparator),
    _resources(),
    _resOrdered(),
    _trueString(),
    _falseString(),
    _mutex()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

rsResourceSet::rsResourceSet(const rsResourceSet &rs) :
    _nameSpaces(),
    _nSpacesMap(),
	_writeCmts(_defaultWriteComments),	
	_mergeMode(ModeNoChange),
    _nameSpacesMap(),
 	_wSeparator(_defaultWSeparator),
    _resources(),
    _resOrdered(),
    _trueString(),
    _falseString(),
    _mutex()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	*this = rs ;
}

rsResourceSet::~rsResourceSet() throw() 
{
// PRQA S 4631 L1
    try {
	    Lock const lock(_mutex) ;
	    map<string,rsValue *>::iterator i = _resources.begin() ;
	    int count =0;
	    for (;i != _resources.end(); ++i, ++count) { // PRQA S 4238
		    delete i->second ;
        }
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "deleted: " << count << " entries from _resources") ;
	    invalidateNameSpaces() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
// PRQA L:L1
}

rsResourceSet &rsResourceSet::operator=(const rsResourceSet &rs)
{
	if (this != &rs) {
		Lock const lock1(_mutex) ;
		Lock const lock2(rs._mutex) ;
		//_nSpacesMap = rs._nSpacesMap ; not copied see below
		_writeCmts = rs._writeCmts ;
		_mergeMode = rs._mergeMode ;
		//_nameSpacesMap = rs._nameSpacesMap ; not copied remains the original one because:
		// 1) the target namespace is a newly created one, hence nobody will never remove the current namespace 
		//    if any !!!
		// 2) the target namespace is an old one with a current namespace maintaned by one ore more instances 
		//    of rsPushNSpace class instances. When they should be removed, the popping should happen with no problem.  
		_wSeparator = rs._wSeparator ;
		
		_resOrdered.clear() ;
		map<string,rsValue *>::iterator tbd = _resources.begin() ;
		for (;tbd != _resources.end(); ++tbd) { // PRQA S 4238
			delete tbd->second ;
        }
		_resources.clear() ;
		
		// copy the resources 
		for (size_t i=0; i < rs.size(); ++i) {
			
			string k = rs._resOrdered[i] ; // get the resources names
			rsValue *v = new rsValue(*rs._resources.find(k)->second) ;
			_resources.insert(pair<string,rsValue*>(k,v)) ;
			_resOrdered.push_back(k) ;
		}
		_trueString = rs._trueString ;
		_falseString = rs._falseString ;
	}

	return *this ;
}

bool rsResourceSet::isEqual(const rsResourceSet &rm) const throw() // PRQA S 4020, 4214
{
	if (_resources.size() != rm._resources.size()) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "size1: " << _resources.size()
				   << " size2: " << rm._resources.size()) ;
		return false ;
	}
	
	size_t rosize = _resOrdered.size() ;
	for(size_t i=0; i < rosize; ++i) {
		const string & key = _resOrdered[i] ; 
		map<string,rsValue *>::const_iterator r = rm._resources.find(key) ;
		if (r == _resources.end()) { // not found: hence different
			return false ;
        }
		if (*(_resources.find(key)->second) != *(r->second)) {
			return false ;
        }
	}
	return true ;
}



void rsResourceSet::defaultSeparator() 
{
	Lock const lock(_mutex) ;
	_wSeparator = _defaultWSeparator ;
}


void rsResourceSet::alternateSeparator()
{
	Lock const lock(_mutex) ;
	_wSeparator = _alternateWSeparator ;
}


bool rsResourceSet::isDefaultSeparator() const
{
	return _wSeparator == _defaultWSeparator ;
}

bool rsResourceSet::isArray(const string &res)
{
	size_t l = res.length() ;
	return ((res[l-1] == ']') && (res[l-2] == '[')) ; // PRQA S 3084, 4400
}

string rsResourceSet::composeKey(const string &res) const // PRQA S 4214
{
	string tmp ;
    // Ignore the currentNameSpace in case the key begins with a double slash 
    bool rootAccess ( 0 == res.find ("//") );   // PRQA S 4412
	const string ns = ( rootAccess ? "" : currentNameSpace() );  // PRQA S 3380, 3384, 3385
    
    // Strip the leading slashes from the key in case are present
    string key = rootAccess ? res.substr(2) : res;  // PRQA S 3380, 3384
	return ns + ((!ns.empty()) ? "." : "") + strip(tmp,key) ;  // PRQA S 3380, 3383
}

string rsResourceSet::composeArrayKey(const string &res) const // PRQA S 4214
{
	string tmp ;
    // Ignore the currentNameSpace in case the key begins with a double slash 
    bool rootAccess ( 0 == res.find ("//") );   // PRQA S 4412
	const string ns = ( rootAccess ? "" : currentNameSpace() );  // PRQA S 3380, 3384, 3385

    // Strip the leading slashes from the key in case are present
    string key = rootAccess ? res.substr(2) : res;  // PRQA S 3380, 3384
    string ret = ns + (!ns.empty() ? "." : "") + strip(tmp,key) + "[]" ;  // PRQA S 3380, 3383
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "ret: " << ret) ;
   	return ret ;  // PRQA S 3380, 3383
}

const rsResourceSet::rsValue *rsResourceSet::findArrayValue(const string &res) const // PRQA S 4020, 4214
{
	Lock const lock(_mutex) ;
	map<string,rsValue *>::const_iterator p = _resources.find(composeArrayKey(res)) ;
    if (p == _resources.end()) {
        return 0 ;
    }
    else {
        return p->second ;
    }
}

const rsResourceSet::rsValue *rsResourceSet::findValue(const string &res) const // PRQA S 4214
{
	Lock const lock(_mutex) ;
	map<string,rsValue *>::const_iterator p = _resources.find(composeKey(res)) ;
	return p != _resources.end() ? p->second : 0 ;  // PRQA S 3380, 3382
}


const rsResourceSet::rsValue *rsResourceSet::getValue(const string &res) const // PRQA S 4214
{	
	Lock const lock(_mutex) ;
	const rsValue *value = findValue(res) ;
    // PRQA S 3081 3 
	ACS_COND_THROW(0 == value,
        NotFoundException("Resource not found: " + res + " in namespace \"" + currentNameSpace() + "\"")) ; // PRQA S 3081
	
	return value ;
}

rsResourceSet::rsValue *rsResourceSet::findValue(const string &res)
{
	Lock const lock(_mutex) ;
	map<string,rsValue*>::iterator p = _resources.find(composeKey(res)) ;
	return p !=  _resources.end() ? p->second : 0 ;  // PRQA S 3380, 3382
}

rsResourceSet::rsValue *rsResourceSet::findOrInsertValue(const std::string &res) 
{
	Lock const lock(_mutex) ;
    rsValue *value = findValue(res) ;
	if (0 == value) { 
		value = new rsValue(composeKey(res)) ;
		insert(value) ;
    }
    return value ; 
}


rsResourceSet::rsValue *rsResourceSet::getValue(const string &res) 
{
	Lock const lock(_mutex) ;
	rsValue *value = findValue(res) ;
	ACS_COND_THROW(!value,NotFoundException("Resource not found: " + res + " in namespace \"" + currentNameSpace() + "\"")) ; // PRQA S 3081
	
	return value ;
}

void rsResourceSet::setValue(const string &res, const char *v)
{
	Lock const lock(_mutex) ;
	setValue(res,string(v)) ; // PRQA S 3081
}

size_t rsResourceSet::getArraySize(const string &res, bool *isSingle) const // PRQA S 4020, 4214
{
	Lock const lock(_mutex) ;
	const rsValue *value = findArrayValue(res) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "res: " << res) ;
	if (0 == value) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "res: " << res << " not found. - trying single") ;
        // try to get a single value
        value = findValue(res) ;
        
	    if (0 != isSingle) { *isSingle = true ; }
		return (0 != value) ?  1 : 0 ; // 1 if a single value // PRQA S 3000, 3382
	}
	if (0 != isSingle) { *isSingle = false ; }
	unsigned long v = 0 ;
	value->get(v) ;
	return v ;
}


void rsResourceSet::setComments(const string &res, const vector<string> &cmt)
{
	Lock const lock(_mutex) ;
	rsValue *value = getValue(res) ;
	value->comments(cmt) ;
}

void rsResourceSet::setArrayComments(const string &res, const vector<string> &cmt, long elem)
{
	Lock const lock(_mutex) ;
	string kk = res ; // do not compute name, because setCommentss will do the job
	if (elem < 0) {
		kk += "[]" ;
    }
	else {
		ostringstream os ;
		os << elem ;
		string key = kk + "[" + os.str() + "]" ;
		kk = key ;
	}
	setComments(kk, cmt) ;
}

const vector<string>& rsResourceSet::getComments(const string &res) const // PRQA S 4214
{
	Lock const lock(_mutex) ;
	return getValue(res)->comments() ;
}

const vector<string>& rsResourceSet::getArrayComments(const string &res, long elem) const // PRQA S 4214
{
	Lock const lock(_mutex) ;
	string kk = composeKey(res) ;
	if (elem < 0) {
		kk += "[]" ;
    }
	else {
		ostringstream os ;
		os << elem ;
		string key = kk + "[" + os.str() + "]" ;
		kk = key ;
	}
	return getValue(kk)->comments() ;
}

void rsResourceSet::deleteResource(const string &res)
{
	Lock const lock(_mutex) ;
	string key = composeKey(res) ;
	map<string,rsValue*>::iterator tbd = _resources.find(key) ;
	if (tbd != _resources.end()) {
		delete tbd->second ;
		_resources.erase(tbd) ;
		_resOrdered.erase(find(_resOrdered.begin(),_resOrdered.end(),key)) ;
	}
}

void rsResourceSet::insert(rsValue *v)
{
	Lock const lock(_mutex) ;
	_resources.insert(pair<string,rsValue *>(v->key(),v)) ;
	_resOrdered.push_back(v->key()) ;
}


void rsResourceSet::deleteArray(const string &res)
{
	Lock const lock(_mutex) ;
	string kk = composeKey(res) ;
	string key = kk + "[]" ;
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "deleting: " << key) ;
	map<string,rsValue *>::iterator atbd = _resources.find(key) ;
	if (atbd != _resources.end()) {
		delete atbd->second ;
		_resOrdered.erase(find(_resOrdered.begin(),_resOrdered.end(),key)) ;
		_resources.erase(atbd) ;
	    std::vector<std::string> _new_resOrdered ;
        string rootkey = kk + "[" ;
        for (size_t i=0; i < _resOrdered.size(); ++i) {
            string k= _resOrdered[i] ;
            if (0 == k.find (rootkey, 0)) { // rootkey is entirely at beginning: candidate to be removed
			    map<string,rsValue *>::iterator etbd = _resources.find(k) ;
			    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "deleting: " << k) ;
			    if (etbd != _resources.end()) {
			        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, k << " deleted") ;
				    delete etbd->second ;
				    _resources.erase(etbd) ;
			    }
            }
            else {
                _new_resOrdered.push_back(k) ;
            }
        }
        _resOrdered.swap(_new_resOrdered) ;
	}
}

void rsResourceSet::clear()
{
	Lock const lock(_mutex) ;
	map<string,rsValue *>::iterator i = _resources.begin() ;
	for (;i != _resources.end(); ++i) { // PRQA S 4238
		delete i->second ;
    }
	_resources.clear() ;
	_resOrdered.clear() ;
}



size_t rsResourceSet::keys(string *k, string *v, vector<string> *c) const // PRQA S 4020
{
	Lock const lock(_mutex) ;
	if ((0 == k) && (0 == v) && (0 == c)) {
		return _resources.size() ;
    }
		
	map<string, rsValue*>::const_iterator it ;
	size_t i = 0 ;
	for (it=_resources.begin();it != _resources.end(); ++it,++i) {
		if (0 != k) { k[i] = it->second->key() ; }
		if (0 != v) { it->second->get(v[i]) ; }
		if (0 != c) { c[i] = it->second->comments() ; }
	}

	return i ;
}

bool rsResourceSet::hasKey(const string &res, bool &isArray) const // PRQA S 4020, 4214
{
	Lock const lock(_mutex) ;
	isArray = false ;
		
	const rsValue *value = findValue(res) ;
	if (0 == value) {
		value = findValue(res+"[]") ;
		if (0 == value) {
			return false ;
        }
		isArray = true ;
	}
	return true ;
}

bool rsResourceSet::hasKey(const string &res) const // PRQA S 4214
{
	Lock const lock(_mutex) ;
	bool arr = false ;
	return hasKey(res,arr) ;
}

void rsResourceSet::useNameSpace(const string &ns) // PRQA S 4211
{
	Lock const lock(_mutex) ;
	_nameSpacesMap[pthread_self()] = ns ;
}



void rsResourceSet::extendBoolean(const string &fs, const string &ts) 
{
	if (not fs.empty() || not ts.empty() ) {
		// please do not remove spaces
		string bad="on off yes no true false 1 0 t f " ;
		string fs1 = fs + " " ;
		if (!fs.empty() && ( StringUtils::findNoCase(bad, fs1) != string::npos) ) {
			ACS_THROW(DuplicateKey ("wrong translation \"" + fs +"\" as false")) ; // PRQA S 3081
		}
		string ts1 = ts + " " ;
		if (!ts.empty() && ( StringUtils::findNoCase(bad, ts1) != string::npos) ) {
			ACS_THROW(DuplicateKey("wrong translation \"" + ts +"\" as true")) ; // PRQA S 3081
		}
		if (StringUtils::equalsNoCase(ts,fs)) {
			ACS_THROW(DuplicateKey("True/false translation ambiguity: \"" + ts +"\" and \""+fs +"\"")) ; // PRQA S 3081
		}
	}
	
	Lock const lock(_mutex) ;
	_falseString = fs ;
	_trueString = ts ;
}


void rsResourceSet::computeNameSpace() 
{
	Lock const lock(_mutex) ;
	string & ns= _nameSpacesMap[pthread_self()] ;
	
	const stack<string> & nss = _nSpacesMap[pthread_self()] ;

	ns = nss.empty() ? "" : nss.top() ;  // PRQA S 3380, 3385
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "actual namespace: \"" <<  ns << "\"") ;
}



void rsResourceSet::pushNameSpace(const string &s, bool global) 
{
	Lock const lock(_mutex) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "pushing: \"" <<  s << "\"") ;

	stack<string> & nss = _nSpacesMap[pthread_self()] ;
	string newtop = s ;
	if (!global) {
		if (!nss.empty()) {
			newtop = nss.top() ;
			if (!newtop.empty() && !s.empty()) {
				newtop += "." ;
            }
			if (!s.empty()) {
				newtop+= s ;
            }
		}
	}
	nss.push(newtop) ;
		
	computeNameSpace() ;
}


void rsResourceSet::popNameSpace() 
{
	Lock const lock(_mutex) ;
	stack<string> & nss = _nSpacesMap[pthread_self()] ;
	
	ACS_COND_THROW(nss.size() == 0,NameSpaceException("Stack is empty")) ; // PRQA S 3081

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	nss.pop() ;

	computeNameSpace() ;
}



bool rsResourceSet::containsOnly(const string &s1, const string &s2)  // PRQA S 4020
{
	const char *cs1 = s1.c_str() ;
	const char *cs2 = s2.c_str() ;
	while (0 != *cs1) {
		if (!strchr(cs2,*(cs1++))) { // search each char from S1 into S2 
			return false ; // a char from S1 is not in S2
        }
    }
	return true ; // S1 is composed only by chars contained in S2
}


string &rsResourceSet::strip(string &s)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "s: \"" << s << "\"") ;
      
	long len = s.length() ; // PRQA S 3000
	long start = 0; 
	long end = -1 ;
	for (long i = len-1; i >= 0; --i) {
        if (not isspace(s[i])) { // PRQA S 3000, 3054
			end = i ;
			break ;
		}
    }
    
	for (long i = 0; i <= end; ++i) {
        if (not isspace(s[i])) { // PRQA S 3000, 3054
			start = i ;
			break ;
		}
	}
    	
	s = (end >= 0) ? string(s, start, end-start+1) : string("") ; // PRQA S 3000, 3081, 3380, 3385
    
    
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "end: " << end 
			   << " start: " << start 
			   << " returning: \"" << s << "\"") ;
	return s ; // PRQA S 4028
}



string &rsResourceSet::strip(string &s, const string &tbs)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "tbs: \"" << tbs << "\"") ;
		
	long len = tbs.length() ; // PRQA S 3000
	long start = 0; 
	long end = 0 ;
	for (long i = len-1; i >= 0; --i) {
        if (not isspace(tbs[i])) { // PRQA S 3000, 3054
			end = i ;
			break ;
		}
	}	
	for (long i = 0; i <= end; ++i) {
        if (not isspace(tbs[i])) { // PRQA S 3000, 3054
			start = i ;
			break ;
		}
	}
	s = (end >=0) ? string(tbs, start, end-start+1) : string("") ; // PRQA S 3000, 3081, 3380, 3385
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "end: " << end 
			   << " start: " << start 
			   << " returning: \"" << s << "\"") ;
	return s ; // PRQA S 4028
}

void rsResourceSet::merge(const rsResourceSet &other)
{
	Lock const lock1(_mutex) ;
	Lock const lock2(other._mutex) ;

	// no namespace 
	string oldNameSpace = currentNameSpace() ;
	useNameSpace("") ;

	try {	
		size_t len = other.size();
		for (size_t i=0; i < len; ++i) { // every entry in the other rs
			// get the resource name

			rsValue *otherValue = other._resources.find(other._resOrdered[i])->second ;

			// check if present
			const string &otherKey = otherValue->key() ;
			map<string,rsValue *>::iterator myIter = _resources.find(otherKey) ;

			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "finding \"" << otherKey << "\" into myself")  ;
			if (myIter != _resources.end()) { // found
				// get the value 
				rsValue *myValue = myIter->second ;

				switch (_mergeMode) {
				case ModeUnion:
					{
						ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "checking isArray\"" << otherKey) ;
						if (isArray(otherKey)) { 
                            // tricky case: max of entries number shall be written
                            unsigned long mySize ;
	                        myValue->get(mySize) ;
                            unsigned long otherSize ; 
	                        otherValue->get(otherSize) ;
                            
                            myValue->set(max(otherSize, mySize)) ;
                            myValue->mergeComments(otherValue->comments()) ;
							continue ;
						}

						string v ;
						otherValue->get(v) ;
						myValue->set(v) ;
						myValue->mergeComments(otherValue->comments()) ;
					    break ;
					}
				case ModeChange:
					{
						ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "checking isArray\"" << otherKey) ;
						if (isArray(otherKey)) {
							// delete the array
							string darr(otherKey, 0, otherKey.length()-2) ; // PRQA S 3084
							ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "found isArray -> removing \"" << darr << "\"") ;
							deleteArray(darr) ;
							insert(new rsValue(*otherValue)) ;
							continue ;
						}

						string v ;
						otherValue->get(v) ;
						myValue->set(v) ;
						myValue->mergeComments(otherValue->comments()) ;
					    break ;
					}
				case ModeNoChange:
                    {
					    // nothing to do
					    break ;
                    }
				case ModeError:
					ACS_THROW(DuplicateKey("Duplicate key: " + otherKey)) ;  // PRQA S 3081
				default:
					{
						ostringstream os ;
						os << int(_mergeMode) ; // PRQA S 3081
						ACS_THROW(exIllegalValueException("Invalid MergeMode: " + os.str())) ;  // PRQA S 3081
					}
				}
			}
			else {
				insert(new rsValue(*otherValue)) ;
			}
		}
	}
	catch (...) {
		useNameSpace(oldNameSpace) ;
		throw ;
	}
	useNameSpace(oldNameSpace) ;
}


std::istream &operator>>(std::istream &str, rsResourceSet &rs)
{
	Lock const lock1(rs._mutex) ;

	rsResourceSet tmprs ;
	
	// the working resource set is the given one if empty, else a temporary one
	rsResourceSet *wrk = rs.size() ? &tmprs : &rs ;  // PRQA S 3054, 3380, 3384

	string emptyLineContains = "\t " ;
	size_t lineno = 0 ;
	char tmps = 0 ;
	vector<string> comments ;
	
	while (!str.eof()) {
		string k ;
		char line[4097] ; // PRQA S 4403
		str.getline(line,sizeof(line),'\n') ;
		
		// just to avoid DOS format carriage-return
		size_t lenline = strlen(line) ;
		if ((0 != lenline) && (line[lenline-1] == '\r')) { // PRQA S 3084
			line[lenline-1] = 0 ; // PRQA S 3084
        }

		const string & tmp=line;

		if ((tmp.empty()) && str.eof()) {
			break ;
        }
			
		if (str.bad()) {
			ACS_THROW(exIOException("Read error",errno)) ; // PRQA S 3081
		}

		str.clear() ;
			
		lineno++ ;
		ACS_FUNC_WRITE_DEBUG_TH(rsResourceSet,ACS_VLO_VERB, "line: " << lineno << " \"" << tmp << "\"") ;
		
		if (tmp.empty()) { continue ; }  // empty ? go to next
			
		
		if (rsResourceSet::containsOnly(tmp,emptyLineContains)) { continue ; } // empty ? go to next

		// comment ? add it 
        switch(tmp[0]) {
        case '#':
        case '!':
        case ';':
            {
    			comments.push_back(tmp) ;
	    		continue ;
		    }
        default: { /* nothing */ }
        }
		
		// check if it is a resource
		string::size_type n = 0 ;
		if (0 == tmps) {
			// check the separator, once found use it for the whole insertion function
			char s = rsResourceSet::_defaultWSeparator ;
			n = tmp.find(s) ;
			if (n == string::npos) {
				s = rsResourceSet::_alternateWSeparator ;
				n = tmp.find(s) ;
				if (n == string::npos) {
					ostringstream os ;
					os << lineno << " \"" << tmp << "\"" ;
					ACS_THROW(rsResourceSet::FileFormatException("Resource line: "+os.str())) ; // PRQA S 3081
				}
			}
			tmps = s ;
		}
		else {
			n = tmp.find(tmps) ;
			if (n == string::npos) {
				ostringstream os ;
				os << lineno << " \"" << tmp << "\"" ;
				ACS_THROW(rsResourceSet::FileFormatException("Resource line: "+os.str())) ; // PRQA S 3081
			}
		}
		
		if (0 == n) {
			ostringstream os ;
			os << lineno << " \"" << tmp << "\"" ;
			ACS_THROW(rsResourceSet::FileFormatException("Resource line: "+os.str())) ; // PRQA S 3081
		}
		
		k = tmp.substr(0, n) ;
		rsResourceSet::strip(k) ;
		if (k.empty()) {
			ostringstream os ;
			os << lineno << " \"" << tmp << "\"" ;
			ACS_THROW(rsResourceSet::FileFormatException("Resource line: "+os.str())) ; // PRQA S 3081
		}
		
		rsResourceSet::rsValue *v = new rsResourceSet::rsValue(k) ;
		v->comments(comments) ;
		comments.erase(comments.begin(),comments.end()) ;
		
		string val = tmp.substr( n+1, (tmp.length()-n-1)) ; // PRQA S 3084
		v->set(rsResourceSet::strip(val)) ;
		
		ACS_FUNC_WRITE_DEBUG_TH(rsResourceSet,ACS_MID_VERB, "line: " << lineno << " key: \"" << k << "\" val: \"" << val << "\"") ;
		
		// aggiungi la risorsa alla tabella temporanea
		wrk->insert(v) ;
	}


	if (wrk != &rs) { // not on the given one, but on a temporary
		// fai il merge tra i dati gia' presenti e quelli nella tabella temporanea
		rs.merge(tmprs) ;
    }
	
	return str ; // PRQA S 4028
}

ostream &operator<<(ostream &str, const rsResourceSet &rs)
{
	Lock const lock(rs._mutex) ;
	
	size_t len = rs._resOrdered.size() ;
	for (size_t i=0; i < len; ++i) {		
		rs._resources.find(rs._resOrdered[i])->second->write(str,rs._wSeparator, rs._writeCmts) ;
    }
	
	return str ; // PRQA S 4028
}

exostream &operator<<(exostream &str, const rsResourceSet &rs) // PRQA S 4222
{
	Lock const lock(rs._mutex) ;
	
	size_t len = rs._resOrdered.size() ;
	for (size_t i=0; i < len; ++i) {	
		rs._resources.find(rs._resOrdered[i])->second->write(str,rs._wSeparator, rs._writeCmts) ;
	}
	
	return str ; // PRQA S 4028
}



void rsResourceSet::writeCmts(bool f) 
{
	Lock const lock(_mutex) ;
	_writeCmts = f ; 
}

bool rsResourceSet::writeCmts() const { return _writeCmts ; } // PRQA S 4120

void rsResourceSet::mergeMode(rsResourceSet::MergeMode m) 
{ 
	Lock const lock(_mutex) ;
	_mergeMode = m ; 
}

rsResourceSet::MergeMode rsResourceSet::mergeMode() const { return _mergeMode ; } // PRQA S 4120

const string &rsResourceSet::currentNameSpace() const 
{ 
	Lock const lock(_mutex) ;
	return _nameSpacesMap[pthread_self()] ; 
}


size_t rsResourceSet::size() const { return _resOrdered.size() ; }
bool rsResourceSet::empty() const { return _resOrdered.empty() ; }



void  rsResourceSet::invalidateNameSpaces()
{
	Lock const lock(_mutex) ;
    size_t size = _nameSpaces.size() ;
	for(size_t i = 0 ; i < size ; ++i) {
		_nameSpaces[i]->invalidate() ;
    }
}

void rsResourceSet::unregisterNameSpace(rsPushNSpace *n) 
{
	Lock const lock(_mutex) ;
	vector<rsPushNSpace *>::iterator tbd = find( _nameSpaces.begin(), _nameSpaces.end(),n) ;
	if(tbd != _nameSpaces.end()) {
		_nameSpaces.erase(tbd) ;
	}	
}

void rsResourceSet::registerNameSpace(rsPushNSpace *n) 
{
	Lock const lock(_mutex) ;
	_nameSpaces.push_back(n) ;	
}


_ACS_END_NAMESPACE
