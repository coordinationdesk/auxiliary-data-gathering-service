// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Resources Library $

	$Id$

	$Author$

	$Log$
	Revision 6.12  2016/02/10 17:49:52  marpas
	perfo slightly improved
	
	Revision 6.11  2015/03/19 18:51:21  marpas
	qa warning removed
	
	Revision 6.10  2014/02/20 11:17:05  marpas
	qa warning fixed
	
	Revision 6.9  2014/02/19 09:53:56  marpas
	getValue with mandatory option introduced
	
	Revision 6.8  2013/10/10 11:54:54  marpas
	getArray allocation otpimization over vectors
	
	Revision 6.7  2013/10/07 18:21:59  marpas
	qa warnings
	
	Revision 6.6  2013/10/07 14:33:58  marpas
	qa warning fixed
	
	Revision 6.5  2013/10/07 08:19:07  marpas
	qa warning removed
	
	Revision 6.4  2013/10/03 09:58:45  marpas
	getValue<bool> rewritten with template specialization
	
	Revision 6.3  2013/09/06 16:08:10  marfav
	Boolean extension functionality restored
	
	Revision 6.2  2013/07/22 10:39:52  marpas
	qa warnings removed
	
	Revision 6.1  2013/07/17 09:23:05  marpas
	getValue and setValue are now template methods that can be spacialized to get/set data structures
	see rsSetGetArrayTest and rsValueTest for examples
	
	Revision 6.0  2013/07/16 17:00:45  marpas
	getArraySize returns 0 if the array is not present
	returns 1 if the array is not a real array but a single value
	getArray and setArray are now template methods managing the above two statements.
	
	Revision 5.0  2013/06/11 09:29:11  marpas
	adopting libException 5.x standards
	
	Revision 2.10  2013/02/22 13:56:41  marpas
	rsResourceSet does not longer inherit virtually from ThreadSafe, it has an internal mutex instead
	Interface rationalization
	coding best practices
	
	Revision 2.9  2012/12/01 13:38:43  marpas
	qa warning removed
	
	Revision 2.8  2012/11/20 22:39:55  marpas
	total quality: code, coverage etc.
	
	Revision 2.7  2012/02/20 15:04:15  marpas
	refactoring
	
	Revision 2.6  2009/07/01 16:46:04  marpas
	minor changes
	
	Revision 2.5  2008/09/17 16:38:28  marpas
	new methods only for x86_64 ... (64 bits)
	void rsResourceSet::setArray(const std::string &, const std::vector<unsigned long> &, size_t num=0) ;
	void rsResourceSet::getArray(const std::string &, std::vector<unsigned long> &, size_t n=0, size_t offset = 0) const ;
	
	Revision 2.4  2008/01/21 15:28:06  clanas
	added implementation for setArray of unsigned int for 64bit
	
	Revision 2.3  2008/01/21 10:43:54  clanas
	fixed for 64bit port
	
	Revision 2.2  2006/05/29 08:40:56  marpas
	setArray for vector<float|size_t> implemented
	
	Revision 2.1  2006/05/23 10:38:24  marpas
	getArray method overloaded for vector<float|size_t>
	
	Revision 2.0  2006/02/28 08:34:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.18  2005/06/09 19:27:22  marpas
	signed & unsigned long long int supported
	
	Revision 1.17  2005/03/01 18:52:26  marpas
	long double supporto added
	
	Revision 1.16  2005/01/12 13:33:58  marpas
	headers fixed
	
	Revision 1.15  2004/07/20 09:06:48  marpas
	class declaration forwarded
	
	Revision 1.14  2004/07/20 08:57:18  marpas
	some method to use exostream
	
	Revision 1.13  2004/04/14 09:21:55  marpas
	ThreadSafe class used
	
	Revision 1.12  2003/09/25 08:47:36  achval
	resourceSet can be deleted even if some rsPushNSpace is not yet out of scope
	
	Revision 1.11  2003/07/31 15:38:42  marpas
	compareContents renamed to isEqual
	
	Revision 1.10  2003/07/31 15:34:42  marpas
	rsResourceSet::compareContents method added
	
	Revision 1.9  2003/07/28 21:45:43  marpas
	_trueString and _falseString were not copied in operator=
	
	Revision 1.8  2003/07/15 18:10:36  marpas
	global namespace concept implemented
	
	Revision 1.7  2003/07/11 12:56:42  marpas
	exException::clone implemented
	
	Revision 1.6  2003/06/24 16:09:48  marpas
	some namespaces related functions are now protected due to improper use.
	namespace computation take into account empty namespace
	
	Revision 1.5  2003/04/30 10:35:37  marpas
	just minor changes
	
	Revision 1.4  2003/04/30 09:20:38  marpas
	using namespace std removed from includes
	
	Revision 1.3  2003/03/10 18:23:51  marpas
	Now is really thread safe !
	
	Revision 1.2  2002/11/29 16:38:01  marpas
	STL use improved.
	
	Revision 1.1.1.1  2002/10/03 20:54:59  marpas
	libResources import
		

*/

#ifndef _rsResourceSet_H_
#define _rsResourceSet_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Filterables.h>

#include <string>
#include <vector>
#include <stack>
#include <unordered_map>


_ACS_BEGIN_NAMESPACE(acs)

class rsPushNSpace ;
class exostream ;

class rsResourceSet  // PRQA S 2109
{
public:

	//! read the resources from a stream.
	friend std::istream &operator>>(std::istream &, rsResourceSet &) ; // PRQA S 2107
	
	//! write the resources to a stream
	friend std::ostream &operator<<(std::ostream &, const rsResourceSet &) ; ; // PRQA S 2072, 2107

	friend acs::exostream &operator<<(acs::exostream &, const rsResourceSet &) ; ; // PRQA S 2072, 2107

	/*! class NameSpaceException declaration */
	exDECLARE_EXCEPTION(NameSpaceException  ,exException) ; // PRQA S 2131, 2502
	
	/*! class NotFoundException declaration */
	exDECLARE_EXCEPTION(NotFoundException  ,exException) ; // PRQA S 2131, 2502
	
	/*! class DuplicateKey declaration */
	exDECLARE_EXCEPTION(DuplicateKey  ,exException) ; // PRQA S 2131, 2502

	/*! class FileFormatException declaration */
	exDECLARE_EXCEPTION(FileFormatException  ,exException) ; // PRQA S 2131, 2502
	
	
	//////////////////////////////
	//							//
	// classe privata rsValue	//
	//							//
	//////////////////////////////

	class rsValue // PRQA S 2109
	{ // valore
        friend bool operator==(const rsResourceSet::rsValue &, const rsResourceSet::rsValue &) noexcept ; // PRQA S 2107
        friend std::istream &operator>>(std::istream &, rsResourceSet &) ;

	public:
		//! Ctor
		explicit rsValue(const std::string &) ;
		//! Dtor
		~rsValue() = default ;

		const std::string &key() const ;
		const std::string &value() const { return _value ; }
        
        void get(bool &, const std::string &fs="", const std::string &ts="") const ;

		// per prendere il valore
        template <typename T> void get(T &) const ;
        
		// per assegnare il valore
		template <typename T> void set(T const &) ;
        void set(const char *) ;
        
		//! sets comments to a resource
		void comments(const std::vector<std::string> &) ;
		
		//! gets resource's comments
		const std::vector<std::string> &comments() const ;
		
		//! merge resource's comments
		void mergeComments(const std::vector<std::string> &) ;

		//! write the resource
		std::ostream &write(std::ostream &, char sep, bool cmt) const ; // PRQA S 2502
		acs::exostream &write(acs::exostream &, char sep, bool cmt) const ; // PRQA S 2502
		
	private:
        // not implemented
        rsValue(); 
		//! to write comments
		void writeComments(std::ostream &) const ;
		//! to write comments
		void writeComments(acs::exostream &) const ;
		
		bool existComment(const std::string &) ;
		void setString(const std::string &) ;

	// private data members
		std::vector<std::string> _comments = {} ;	
		std::string _key ;
		std::string _value = "" ;
    public:
        static const std::string literal_false ; // PRQA S 2100
        static const std::string literal_true ;  // PRQA S 2100
	} ;

public: // NOSONAR - for clarity
	//! merge mode between 2 resource sets - how to deal with conflicting keys \see rsResourceSet::merge
	enum MergeMode { // NOSONAR
            //! error if key is found in both resource sets
        ModeError,		
            //! uses the value of the incoming resource set
        ModeChange,
            //! keeps the value of the current resource set
        ModeNoChange,
            /*! in case of arrays, joins the two - it is a bit tricky in 
                case of arrays of unions: the behaviour is that the unions in 
                the same position are merged
            */
        ModeUnion
					
    } ;

	rsResourceSet() = default ;									// costruttore

	rsResourceSet(const rsResourceSet &) ;				// costruttore di copia

	~rsResourceSet() noexcept ;							// distruttore

	/*! assignement operator. It guarantee only that after the completion, 
		the resources are equally comparable. The status will be left untouched.
		This applies to the namespace too.
	*/
	rsResourceSet &operator=(const rsResourceSet &) ;
    
	/*! cast operator to Mutex. It allows to lock a resource set from external.
	*/
    operator Mutex &() const noexcept { return _mutex ; } // NOSONAR - implicit cast for locking

	bool isEqual(const rsResourceSet &) const noexcept ;

	void writeCmts(bool) ;		// per assegnare il valore al flag di scrittura dei commenti
	bool writeCmts() const ;	// per sapere il valore corrente del flag di scrittura dei commenti

	
    /*! sets the merge mode \see rsResourceSet::MergeMode \see rsResourceSet::merge */
	void mergeMode(MergeMode) ;
    /*! returns the actual merge mode \see rsResourceSet::MergeMode \see rsResourceSet::merge*/
	MergeMode mergeMode() const ;

	// per assegnare al valore del separatore (tra chiave e valore) usato in scrittura
	// quello di default o no
	void defaultSeparator() ;
	void alternateSeparator() ;
	// per sapere se il valore corrente del separatore (tra chiave e valore) usato in
	// scrittura e' quello di default o no
	bool isDefaultSeparator() const ;

	// per prendere il valore di una risorsa
    template<typename T>
	void getValue(const std::string &res, T &v) const 
    {
    	Lock lock(_mutex) ;
	    getValue(res)->get(v) ;
    } 


    template<typename T>
	bool getValue(const std::string &res, T &v, bool mandatory) const // PRQA S 4020
    {
    	Lock lock(_mutex) ;
	    const rsValue * value = findValue(res) ;
        if (value) { // found
            value->get(v) ;
            return true ;
        }
        
        ACS_COND_THROW(mandatory,NotFoundException("Resource not found: " + res + " in namespace \"" + currentNameSpace() + "\"")) ; // PRQA S 3081
        return false ; // if not mandatory 
    } 

	// per assegnare il valore ad una risorsa
    template <typename T>
    void setValue(const std::string &res, const T &v)
    {
	    Lock lock(_mutex) ;
	    findOrInsertValue(res)->set(v) ;
    }


	void setValue(const std::string &, const char *) ; 

	// per prendere la grandezza di un array
	size_t getArraySize(const std::string &, bool *isSingle = nullptr ) const ;
    
    template <typename T>
	size_t getArray(const std::string &res, std::vector<T> &v, size_t n=0, size_t offset = 0) const // PRQA S 4020
    {
	    Lock lock(_mutex) ;
	    bool isSingle = false ;
	    size_t sz = getArraySize(res, &isSingle) ;
	    if (0 == sz || offset >= sz) {
		    return 0 ;
        }
        if (! isSingle) { // a standard Array or a Single value with "count" attribute coming from XML
	        n = (n > 0) ? n : sz ; // if no element requested, read the whole array  // PRQA S 3380
	        size_t toGet = (offset+n > sz) ? sz-offset : n ;  // PRQA S 3084, 3380, 3384, 3084, 3384
	        size_t i ;  // PRQA S 4101
            const std::string pfx = res + "[" ;
            v.reserve(toGet) ; // optimize capacity
	        for (i = 0; i < toGet; ++i) { // PRQA S 4230
		        std::ostringstream os ;
		        os << offset+i ; // PRQA S 3084 // PRQA S 3084
		        T vi = T();
		        getValue(pfx + os.str() + ']', vi) ;
		        v.push_back(vi) ;
	        }

	        return i ;
        }
        else {  // isSingle - meaning that exists and has no "count" attributes if it comes from XML
           T vi = T();
           getValue(res, vi) ;
           v.reserve(1) ;
           v.push_back(vi) ;
           return 1 ;
        }
    }
    
    template <typename T>
	size_t getArray(const std::string &res, T *v, size_t n=0, size_t offset = 0) const // PRQA S 4020
    {
	    Lock lock(_mutex) ;
	    bool isSingle = false ;
	    size_t sz = getArraySize(res, &isSingle) ;
	    if ((0 == sz) || (offset >= sz)) {
		    return 0 ;
        }
        if (! isSingle) {
	        size_t toGet = (offset+n > sz) ? sz-offset : n ;  // PRQA S 3084, 3380, 3384
	        const std::string pfx = res + "[" ;
	        size_t i ;  // PRQA S 4101
	        for (i = 0; i < toGet; ++i) { // PRQA S 4230
		        std::ostringstream os ;
		        os << offset+i ; // PRQA S 3084
		        getValue(pfx + os.str() + ']', v[i]) ;
	        }

	        return i ;
        }
        else {
            getValue(res, v[0]) ;
            return 1 ;
        }
    }
    
    template <typename T>
    void setArray(const std::string &res, const std::vector<T> &v)
    {
	    Lock lock(_mutex) ;
	    size_t sz = v.size() ;
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "res: " << res << " size: " << sz ) ;
	    size_t oldsz = getArraySize(res) ;
	    if (0 != oldsz) {
		    deleteArray(res) ; // do not remove single entry if it was not an array (conservative)
        }
	    setValue(res + "[]", sz) ;

	    for (size_t i = 0; i < sz; ++i) { // PRQA S 4230
		    std::ostringstream os ;
		    os << i ;
		    setValue(res + '[' + os.str() + ']', v[i]) ;
	    }
    }
    
    template <typename T>
    void setArray(const std::string &res, const T * const v, size_t sz)
    {
	    Lock lock(_mutex) ;
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "res: " << res << " size: " << sz ) ;
	    size_t oldsz = getArraySize(res) ;
	    if (0 != oldsz) {
		    deleteArray(res) ;
        }
	    setValue(res + "[]", sz) ;
        
	    for (size_t i = 0; i < sz; ++i) { // PRQA S 4230
		    std::ostringstream os ;
		    os << i ;
		    setValue(res + '[' + os.str() + ']', v[i]) ;
	    }
    }

	const std::vector<std::string>& getComments(const std::string &) const ;							// per avere i commenti di una risorsa
	const std::vector<std::string>& getArrayComments(const std::string &, long elem = -1) const ;	// per avere i commenti di una risorsa di tipo array o ad un suo elemento

	// per assegnare dei commenti ad una risorsa
	void setComments(const std::string &, const std::vector<std::string> &) ;
	// per assegnare dei commenti ad una risorsa di tipo array o ad un suo elemento
	void setArrayComments(const std::string &, const std::vector<std::string> &, long elem = -1) ;

	// cancella una risorsa dal set
	void deleteResource(const std::string &) ;
	// cancella un array di risorse dal set
	void deleteArray(const std::string &) ;

	void clear() ;		// cancella tutti gli elementi del set
	size_t size() const ;
	bool empty() const ;

	size_t keys(std::string * = nullptr, 
                std::string *v= nullptr, 
                std::vector<std::string> *c= nullptr) const ;
	bool hasKey(const std::string &) const ;
	bool hasKey(const std::string &, bool &) const ;
	
    /*! \brief merge the incoming resource set into the current one
        \param other the incoming resource set
        The incoming resource set is merged into the current one according to the MergeMode \see rsResourceSet::mergeMode
        For arrays and arrays of complex structures and/or unions if the mode is ModeChange, the whole array is entirely substituted by the incoming one 
        if the merge mode is ModeUnion the behaviour is to join the current and the incoming ones.
        This could lead to uncoherent results for the unions/structures as in the following example
  
        current content
        \code
pods[]:3

pods[0].integer:1
pods[0].floating:2
pods[0].label:pod1

pods[1].integer:2
pods[1].floating:3.4
pods[1].label:pod2

pods[2].integer:3
pods[2].floating:4.5
pods[2].label:pod3
           
        \endcode

        other content

        \code
pods[]:4

pods[0].label:newpod1

pods[3].integer:999
pods[3].floating:999.9
pods[3].label:pod999

pods[2].newfield: fieldvalue

        \endcode

        result

        \code
pods[]:4

pods[0].integer:1
pods[0].floating:2
pods[0].label:newpod1

pods[1].integer:2
pods[1].floating:3.4
pods[1].label:pod2

pods[2].integer:3
pods[2].floating:4.5
pods[2].label:pod3

pods[3].integer:999
pods[3].floating:999.9
pods[3].label:pod999

pods[2].newfield: fieldvalue
        \endcode
        
     */
	void merge(const rsResourceSet &other) ; // PRQA S 2502

	const std::string &currentNameSpace() const ;	// torna il name space correntemente in uso

	// strip leading and trailing <spaces>
	static std::string &strip(std::string &) ;
	static std::string &strip(std::string &,const std::string &) ;
	
	//! check if s1 contains only characters from s2
	static bool containsOnly(const std::string &s1, const std::string &s2) ;
	
	/*! Extends the translation from std::string to boolean values 
	    Actually the class implements the following case insensitive mapping:
		\code

"true" maps to true
"false" maps to false
"t" maps to true
"f" maps to false
"on" maps to true
"off" maps to false
"yes" maps to true
"no" maps to false
"0" maps to false
"<any other integer value != 0>" maps to true

"<any other std::string>" maps to false

		\endcode
		the two std::string passed to this method extend the mapping.
		Hence, calling 
		\code
		
extendBoolean("Niet", "Da")
		
		\endcode
		
		will extend the previous mapping, translating "Niet" into false and "Da" into true.
		The translation is case insensitive.
	*/
	void extendBoolean(const std::string &fs, const std::string &ts) ;
	void unregisterNameSpace(rsPushNSpace *n) ;
	void registerNameSpace(rsPushNSpace *n) ;
	
protected:
	friend class rsPushNSpace ; // deal with namespaces // PRQA S 2107

	// if global is true, the new string completely replaces the top of the stack instead
	// of being appended ...
	void pushNameSpace(const std::string &, bool global=false) ;
	void popNameSpace() ;

#ifdef TEST_MODE
public:
#else
protected: // NOSONAR - for clarity
#endif	
	/*! set the current namespace until pushNameSpace or popNameSpace are called.
	*/
	void useNameSpace(const std::string &) ;
    
private:
	
	void invalidateNameSpaces() ;
	
	const rsValue *getValue(const std::string &) const ;
	const rsValue *findValue(const std::string &) const ;
	rsValue *findOrInsertValue(const std::string &) ;
	rsValue *getValue(const std::string &) ;
	rsValue *findValue(const std::string &) ;
	const rsValue *findArrayValue(const std::string &) const ;


	void insert(rsValue *) ;
	
	void computeNameSpace() ;
	//! create the key taking into accont current namespace
	std::string composeKey(const std::string &) const ;

	//! create the array key taking into accont current namespace
	std::string composeArrayKey(const std::string &) const ;	// compone la chiave di una risorsa con il name space


	//! checks if the resource ends with the "[]" as in this_is_an_array[]
	static bool isArray(const std::string &) ;
// sezione dati

public:
	// default (usato in scrittura) per separatore tra chiave e valore
	static const bool _defaultWriteComments = true ;
	static const char _defaultWSeparator = ':' ;   // = ':' ;
	static const char _alternateWSeparator = '=' ; // = '=' ;
private:

	std::vector <rsPushNSpace *> _nameSpaces = {} ;
	
	std::unordered_map<pthread_t,std::stack<std::string> > _nSpacesMap = {} ; // map of namespace's stack

	bool _writeCmts = _defaultWriteComments ;				// flag di scrittura dei commenti
	MergeMode _mergeMode = ModeNoChange ;			// merge mode 
	mutable std::unordered_map<pthread_t,std::string> _nameSpacesMap = {} ; // current namespace
	char _wSeparator = _defaultWSeparator ; // separatore tra chiave e valore usato in scrittura

	std::unordered_map<std::string,std::unique_ptr<rsValue> >	_resources = {} ;
	std::vector<std::string> 		_resOrdered = {} ;
	std::string _trueString = "" ;
	std::string _falseString = "" ;
    mutable Mutex   _mutex = {} ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(rsResourceSet) ;
} ; // rsResourceSet


std::istream &operator>>(std::istream &, rsResourceSet &) ;
	
std::ostream &operator<<(std::ostream &, const rsResourceSet &) ;
exostream &operator<<(exostream &, const rsResourceSet &) ;

bool operator==(const rsResourceSet::rsValue &,   const rsResourceSet::rsValue &)   noexcept ;

// PRQA S 2134 L1
inline
bool operator!=(const rsResourceSet::rsValue &v1, const rsResourceSet::rsValue &v2) noexcept { return ! operator==(v1, v2) ; }

inline
bool operator==(const rsResourceSet &r1, const rsResourceSet &r2) noexcept { return r1.isEqual(r2) ; }

inline
bool operator!=(const rsResourceSet &r1, const rsResourceSet &r2) noexcept { return ! r1.isEqual(r2) ; }

template<>
inline
void rsResourceSet::getValue<bool>(const std::string& res, bool& v) const // PRQA S 2133
{
    Lock lock(_mutex) ;
    getValue(res)->get(v, _falseString, _trueString) ;
}

// PRQA L:L1


_ACS_END_NAMESPACE


#endif	// _rsResourceSet_H_
