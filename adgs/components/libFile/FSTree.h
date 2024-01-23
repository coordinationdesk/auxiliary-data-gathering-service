// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	Revision 5.8  2016/03/08 14:59:13  marpas
	removing mutex in File class
	This was originally due to some system call thread safety issues (e.g. readdir)
	The latter was explicitely changed into readdir_r call
	Other syscall were checked and up to now seems there is none having thread safety problems
	
	Revision 5.7  2015/10/30 08:20:07  lucio.pulvirenti
	ARK-4: stddef.h include added to compile on EL7.
	
	Revision 5.6  2015/06/04 14:07:17  marfav
	S2PDGS-1239 FSTree using thread safe system call readdir_r
	
	Revision 5.5  2014/10/10 12:09:37  enrcar
	EC:: dirent replaced with stat to handle the type of a dir.entry (dir, reg.file, symlink...)
	
	Revision 5.4  2014/03/12 09:53:22  marpas
	qa warning
	
	Revision 5.3  2014/02/11 10:48:10  marfav
	Implementation completed
	QA Tool warning managed
	
	Revision 5.2  2014/02/10 17:49:27  marfav
	Forward conditioned iterator and const iterator added
	
	Revision 5.1  2014/02/07 15:32:59  marfav
	FSTree class added
	

*/

#ifndef _FSTREE_H_
#define _FSTREE_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <Filterables.h>
#include <Guard.hpp>

#include <iostream>
#include <sys/types.h> // PRQA S 1013 2
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <cstddef>

#include <string>
#include <cstring>
#include <map>

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)



class FSTree // PRQA S 2109, 2185
{

public:

//forward declaration
template <typename iterator_Predicate> class filtered_iterator;
template <typename iterator_Predicate> class filtered_const_iterator;

// Types used into FSTree
struct Entry // PRQA S 2173, 2175 
{
    struct stat stat_r; // PRQA S 2100 2 
    struct dirent dirent_r;

    Entry();
    ~Entry() throw() {}

    Entry(const Entry&);
    const Entry& operator= (const Entry&);
};

typedef std::map <std::string, struct Entry> EntriesMap;

// FSTree operations
public:
    explicit FSTree (const std::string&);
    ~FSTree () throw() {}


    // Copy CTOR and Assignment
    FSTree (const FSTree&);
    const FSTree& operator= (const FSTree&);

    void load();

    template <typename Predicate> 
        void load (Predicate p);

    const EntriesMap & getMap() const { return _tree; }

    void hidden (bool b) { _hidden = b; }
    bool hidden () const { return _hidden; };

    void resolveLinks (bool b) { _resolveLinks = b; }
    bool resolveLinks () const { return _resolveLinks; }

    void fatalFailures (bool b) { _fatalFailures = b; }
    bool fatalFailures () const { return _fatalFailures; }

    void maxDepth (unsigned int d) {_maxDepth = d; }
    unsigned int maxDepth() const { return _maxDepth; }

    template <typename Predicate> 
        filtered_iterator<Predicate> begin(Predicate&);

    template <typename Predicate> 
        filtered_iterator<Predicate> end(Predicate&);

    template <typename Predicate> 
        filtered_const_iterator<Predicate> begin(Predicate&) const;

    template <typename Predicate> 
        filtered_const_iterator<Predicate> end(Predicate&) const;

private:
    template <typename Predicate> 
        void internalLoad (std::string, unsigned int, Predicate, bool = true);

// FSTree attributes
private:
    std::string _path;
    unsigned int _maxDepth;
    bool _hidden;
    bool _resolveLinks;
    bool _fatalFailures;
    EntriesMap _tree;
 

/** Inner class : Filtered iterator on the map
 *  Implements a forward iterator on the FSTree map structure
 */
public:
template <typename iterator_Predicate>
class filtered_iterator // PRQA S 2109
{
public:
    typedef EntriesMap::iterator iterator_type;

    filtered_iterator(iterator_type current, EntriesMap& map, iterator_Predicate& predicate);
    filtered_iterator (const filtered_iterator& i);

    const filtered_iterator& operator= (const filtered_iterator& i);

    EntriesMap::value_type& operator*() const {return *_current;}
    EntriesMap::value_type* operator->() const {return &(*_current);}

    bool equals (const filtered_iterator& i) const {return _current == i._current;}
    // bool operator!= (const filtered_iterator& i) {return !operator==(i);}

    filtered_iterator& operator++() { increment(); return *this;}
    filtered_iterator operator++(int) { filtered_iterator& current = *this; increment(); return current;}

    // conversion operator
    operator filtered_const_iterator<iterator_Predicate> () const; // PRQA S 2181

private:
    void increment();

private:
    iterator_type _current;
    EntriesMap& _map;
    iterator_Predicate& _predicate;
};


/** Inner class : Filtered const iterator on the map
 *  Implements a forward iterator on the FSTree map structure
 */
public:
template <typename iterator_Predicate>
class filtered_const_iterator // PRQA S 2109
{
public:
    typedef EntriesMap::const_iterator iterator_type;

    filtered_const_iterator (iterator_type, const EntriesMap&, iterator_Predicate&);
    filtered_const_iterator (const filtered_const_iterator&);
    // filtered_const_iterator (const filtered_iterator<iterator_Predicate>&);

    const filtered_const_iterator& operator= (const filtered_const_iterator& i);

    const EntriesMap::value_type& operator*() const {return *_current;}
    const EntriesMap::value_type* operator->() const {return &(*_current);}

    bool equals (const filtered_const_iterator& i) const {return _current == i._current;}
    // bool operator!= (const filtered_const_iterator& i) {return !operator==(i);}

    filtered_const_iterator& operator++() { increment(); return *this;}
    filtered_const_iterator operator++(int) { filtered_const_iterator& current = *this; increment(); return current;}


private:
    void increment();

private:
    iterator_type _current;
    const EntriesMap& _map;
    iterator_Predicate& _predicate;
};


};


/** 
 *  FORWARD ITERATOR filtered_iterator
 *  template operation implementations
 */
template <typename iterator_Predicate>
FSTree::filtered_iterator<iterator_Predicate>::filtered_iterator 
    (FSTree::filtered_iterator<iterator_Predicate>::iterator_type current, FSTree::EntriesMap& map, iterator_Predicate& predicate) :
    _current(current),
    _map(map),
    _predicate(predicate)
{}

template <typename iterator_Predicate>
FSTree::filtered_iterator<iterator_Predicate>::filtered_iterator 
    (const FSTree::filtered_iterator<iterator_Predicate>& i) :
    _current (i._current),
    _map(i._map),
    _predicate(i._predicate)
{}

template <typename iterator_Predicate>
const FSTree::filtered_iterator<iterator_Predicate>& FSTree::filtered_iterator<iterator_Predicate>::operator= (const FSTree::filtered_iterator<iterator_Predicate>& i)
{
    if (this != &i)
    {
        _current = i._current;
        _map = i._map;
        _predicate = i._predicate;
    }
    return *this;
}

template <typename iterator_Predicate>
void FSTree::filtered_iterator<iterator_Predicate>::increment ()
{
    if (_current!=_map.end())
    {
        do
        {
            ++_current;
        }
        while ( (_current!=_map.end()) && (!_predicate(*_current)) );
    }
}

template <typename iterator_Predicate>
FSTree::filtered_iterator<iterator_Predicate>::operator FSTree::filtered_const_iterator<iterator_Predicate> () const
{
    return filtered_const_iterator<iterator_Predicate> (_current, _map, _predicate);
}



/** 
 *  FORWARD CONST ITERATOR filtered_const_iterator
 *  template operation implementations
 */
template <typename iterator_Predicate>
FSTree::filtered_const_iterator<iterator_Predicate>::filtered_const_iterator 
    (FSTree::filtered_const_iterator<iterator_Predicate>::iterator_type current, const FSTree::EntriesMap& map, iterator_Predicate& predicate) :
    _current(current),
    _map(map),
    _predicate(predicate)
{}

template <typename iterator_Predicate>
FSTree::filtered_const_iterator<iterator_Predicate>::filtered_const_iterator 
    (const FSTree::filtered_const_iterator<iterator_Predicate>& i) :
    _current (i._current),
    _map(i._map),
    _predicate(i._predicate)
{}

template <typename iterator_Predicate>
const FSTree::filtered_const_iterator<iterator_Predicate>& FSTree::filtered_const_iterator<iterator_Predicate>::operator= 
    (const FSTree::filtered_const_iterator<iterator_Predicate>& i)
{
    if (this != &i)
    {
        _current = i._current;
        _map = i._map;
        _predicate = i._predicate;
    }
    return *this;
}

template <typename iterator_Predicate>
void FSTree::filtered_const_iterator<iterator_Predicate>::increment ()
{
    if (_current!=_map.end())
    {
        do
        {
            ++_current;
        }
        while ( (_current!=_map.end()) && (!_predicate(*_current)) );
    }
}


/** 
 *  CLASS FSTREE
 *  template operation implementations
 */
template <typename Predicate> 
FSTree::filtered_iterator<Predicate> FSTree::begin(Predicate& p) 
{
    filtered_iterator<Predicate> it (_tree.begin(), _tree, p);
    if (!p(*it))
    {
        ++it;
    }
    return it;
}

template <typename Predicate> 
FSTree::filtered_iterator<Predicate> FSTree::end(Predicate& p)
{
    return filtered_iterator<Predicate> (_tree.end(), _tree, p);
}

template <typename Predicate> 
FSTree::filtered_const_iterator<Predicate> FSTree::begin(Predicate& p) const
{
    filtered_const_iterator<Predicate> it (_tree.begin(), _tree, p);
    if (!p(*it))
    {
        ++it;
    }
    return it;
}

template <typename Predicate> 
FSTree::filtered_const_iterator<Predicate> FSTree::end(Predicate& p) const
{
    return filtered_const_iterator<Predicate> (_tree.end(), _tree, p);
}

template <typename Predicate> 
void FSTree::load (Predicate p) // PRQA S 2010
{
    _tree.clear();
    internalLoad (_path, 0, p, true);
}


template <typename Predicate>
void FSTree::internalLoad (std::string path, unsigned int curDepth, Predicate predicate, bool scream) // PRQA S 2010, 4020, 5500, 5510
{
    // This function has multiple return point to keep its coplexity as low as possible
    
    if ( (0 == _maxDepth) || (curDepth != _maxDepth))
    {
       	// Scan directory
    	::DIR *dir = ::opendir(path.c_str()) ;
        if (0 == dir)
        {
            if (errno == ENOTDIR)
            {
                // it is not a directory... have to scream?
                if (scream)
                {
                    std::ostringstream os;      
                    os << "Unable to read the path " << path << " in FSTree. Path is not a directory";
                    if (_fatalFailures)
                    {
                        ACS_THROW (exIOException(os.str(), errno));
                    }
                    else
                    {   
                        ACS_LOG_WARNING ("Got an error opening dir. Stack follows");
                        ACS_LOG_TRYCATCHNOTIFY_EX (exIOException(os.str(), errno));
                    }
                }
            }
            else
            {
                std::ostringstream os;      
                os << "Unable to read the path " << path << " in FSTree";
                if (_fatalFailures)
                {
                    ACS_THROW (exIOException(os.str(), errno));
                }
                else
                {   
                    ACS_LOG_WARNING ("Got an error opening dir. Stack follows");
                    ACS_LOG_TRYCATCHNOTIFY_EX (exIOException(os.str(), errno));
                }
            }
            return;
        }

		// reentrant version of readdir is used here
		// see man 3 readdir for details on the malloc used here

#ifndef PRQA_ANALYZER
        size_t len = offsetof(struct dirent, d_name) + pathconf(path.c_str(), _PC_NAME_MAX) + 1;
#else
        size_t len = 16384 ; // just a number to avoid QA analyzer warning // PRQA S 4400
#endif
        struct dirent* entry = reinterpret_cast<struct dirent*> ( malloc(len) ); // PRQA S 3081
		struct dirent* result = 0;

		// This will automatically free the memory used by entry at dtor time
		pattern::CMemoryScopeGuard theGuard (entry);

		int readdirval = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		while ( ( 0 == (readdirval = readdir_r(dir, entry, &result) ) ) && ( 0 != result) )
        {
#pragma GCC diagnostic pop
            std::string name=entry->d_name;
            bool managingDot = ("." == name); // PRQA S 4412

		    if ( (managingDot) && (0 != curDepth)) {
			    continue ;	/* Ignore . but not at the first level.. */
            }
		    if (name == "..") {
			    continue ;	/* Ignore .. */
            }
		    if (!_hidden && (name[0] == '.')) {
			    continue ;	/* Ignore hidden files */
            }

            struct Entry fsentry;
            fsentry.dirent_r = *entry;
            struct stat s; // PRQA S 4102

            std::string filePath = path+"/"+(managingDot ? "" : name); // PRQA S 3380, 3385

            int statresult = (_resolveLinks ? stat (filePath.c_str(), &s) : lstat (filePath.c_str(), &s)); // PRQA S 3384

            if (0 != statresult)
            {
                std::ostringstream os;      
                os << "Cannot stat the file " << filePath;
                ACS_LOG_WARNING ("Got an error during tree scan. Continuing with the scan. " << os.str());
                // Ignore this file and go ahead with the next one
                continue;
            }
            fsentry.stat_r = s;

            // decide if to include the entry or not
            if (! predicate (filePath, fsentry))
            {
                // If predicate fails. Skip the item.
                continue;
            }

            _tree [filePath] = fsentry;

            if (_resolveLinks && S_ISLNK(s.st_mode) )
            {
                // try to navigate the symlink assuming it is a directory
                // do not scream if not able to read the directory
                this->internalLoad (filePath, curDepth+1, predicate, false); //PRQA S 3000, 3084
            }

            if (S_ISDIR(s.st_mode)  && !managingDot)
            {
                // recursion . another level down if allowed
                this->internalLoad (filePath, curDepth+1, predicate); //PRQA S 3000, 3084
            }
        }   	

        if (0 != readdirval)
        {
            std::ostringstream os;      
            os << "Cannot read the directory " << path;
            if (_fatalFailures)
            {
                ::closedir (dir);
                ACS_THROW (exIOException(os.str(), readdirval));
            }
            else
            {   
                ACS_LOG_WARNING ("Cannot read the directory. Stack follows");
                ACS_LOG_TRYCATCHNOTIFY_EX (exIOException(os.str(), readdirval));
            }
        }
        ::closedir(dir) ;
    }
}

// Operators == and != external from class and based on equals class member

template <typename pred>
bool operator== (const FSTree::filtered_iterator<pred>& lval, const FSTree::filtered_iterator<pred>& rval) 
{
    return lval.equals(rval);
}

template <typename pred>
bool operator== (const FSTree::filtered_const_iterator<pred>& lval, const FSTree::filtered_const_iterator<pred>& rval) 
{
    return lval.equals(rval);
}

template <typename pred>
bool operator== (const FSTree::filtered_iterator<pred>& lval, const FSTree::filtered_const_iterator<pred>& rval) 
{
    // NON CONST can be casted to CONST but not the reverse so in this case must swap lval and rval
    return rval.equals(lval);
}

template <typename pred>
bool operator== (const FSTree::filtered_const_iterator<pred>& lval, const FSTree::filtered_iterator<pred>& rval) 
{
    return lval.equals(rval);
}




template <typename pred>
bool operator!= (const FSTree::filtered_iterator<pred>& lval, const FSTree::filtered_iterator<pred>& rval) 
{
    return ! (lval == rval);
}


template <typename pred>
bool operator!= (const FSTree::filtered_const_iterator<pred>& lval, const FSTree::filtered_const_iterator<pred>& rval) 
{
    return ! (lval == rval);
}


template <typename pred>
bool operator!= (const FSTree::filtered_iterator<pred>& lval, const FSTree::filtered_const_iterator<pred>& rval) 
{
    return ! (lval == rval);
}


template <typename pred>
bool operator!= (const FSTree::filtered_const_iterator<pred>& lval, const FSTree::filtered_iterator<pred>& rval) 
{
    return ! (lval == rval);
}

_ACS_END_NESTED_NAMESPACE


#endif //_FSTREE_H_
