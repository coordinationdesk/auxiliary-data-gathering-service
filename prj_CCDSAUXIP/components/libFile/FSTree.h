
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

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
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <cstddef>

#include <string>
#include <cstring>
#include <map>

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)



class FSTree 
{

public:

    //forward declaration
    template <typename iterator_Predicate> class filtered_iterator;
    template <typename iterator_Predicate> class filtered_const_iterator;

    // Types used into FSTree
    struct Entry 
    {
        struct stat stat_r = {} ; 
        struct dirent dirent_r = {} ;
    };

    using EntriesMap = std::map <std::string, struct Entry> ;

// FSTree operations
public: // NOSONAR - for clarity
    explicit FSTree (const std::string&);
    ~FSTree () = default ;


    // Copy CTOR and Assignment
    FSTree (const FSTree&) = default ;
    FSTree& operator= (const FSTree&) = default ;

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
        void internalLoad (const std::string &, unsigned int, Predicate, bool = true);

// private FSTree attributes

    std::string _path;
    unsigned int _maxDepth = 0 ;
    bool _hidden = true ;
    bool _resolveLinks = true ;
    bool _fatalFailures = true ;
    EntriesMap _tree = {} ;
 

/** Inner class : Filtered iterator on the map
 *  Implements a forward iterator on the FSTree map structure
 */
public:
    template <typename iterator_Predicate>
    class filtered_iterator 
    {
    public:
        using iterator_type = EntriesMap::iterator ;

        filtered_iterator(iterator_type current, EntriesMap& map, iterator_Predicate& predicate) :
           _current(current),
           _map(map),
           _predicate(predicate)
        {}

        filtered_iterator (const filtered_iterator& ) = default ;

        filtered_iterator& operator= (const filtered_iterator& ) = default ;

        EntriesMap::value_type& operator*() const {return *_current;}
        EntriesMap::value_type* operator->() const {return &(*_current);}

        bool equals (const filtered_iterator& r) const {return _current == r._current;}

        filtered_iterator& operator++() { increment(); return *this;}
        filtered_iterator operator++(int) { filtered_iterator& current = *this; increment(); return current;}

        // conversion operator
        operator filtered_const_iterator<iterator_Predicate> () const // NOSONAR - implicit cast to const iterator
        {
            return filtered_const_iterator<iterator_Predicate> (_current, _map, _predicate);
        }


    private:
        void increment() {
             if (_current!=_map.end())
             {
                 do
                 {
                     ++_current;
                 }
                 while ( (_current!=_map.end()) && (!_predicate(*_current)) );
             }
         }

    private: // NOSONAR - for clarity
        iterator_type _current;
        EntriesMap& _map;
        iterator_Predicate& _predicate;
    };


/** Inner class : Filtered const iterator on the map
 *  Implements a forward iterator on the FSTree map structure
 */
public: // NOSONAR - for clarity
    template <typename iterator_Predicate>
    class filtered_const_iterator 
    {
    public:
        using iterator_type = EntriesMap::const_iterator ;

        filtered_const_iterator (iterator_type, const EntriesMap&, iterator_Predicate&);
        filtered_const_iterator (const filtered_const_iterator&) = default ;

        filtered_const_iterator& operator= (const filtered_const_iterator&) = default ;


        const EntriesMap::value_type& operator*() const {return *_current;}
        const EntriesMap::value_type* operator->() const {return &(*_current);}

        bool equals (const filtered_const_iterator& r) const {return _current == r._current;}

        filtered_const_iterator& operator++() { increment(); return *this;}
        filtered_const_iterator operator++(int) { filtered_const_iterator& current = *this; increment(); return current;}


    private:
        void increment();

        iterator_type _current;
        const EntriesMap& _map;
        iterator_Predicate& _predicate;
    };


};


/** 
 *  FORWARD ITERATOR filtered_iterator
 *  template operation implementations
 */



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
void FSTree::load (Predicate p) 
{
    _tree.clear();
    internalLoad (_path, 0, p, true);
}


template <typename Predicate>
void FSTree::internalLoad (const std::string &path, unsigned int curDepth, Predicate predicate, bool scream) 
{
    // This function has multiple return point to keep its coplexity as low as possible
    
    if ( (0 == _maxDepth) || (curDepth != _maxDepth))
    {
       	// Scan directory
    	::DIR *dir = ::opendir(path.c_str()) ;
        if (!dir) // null ptr
        {
            if (errno == ENOTDIR)
            {
                // it is not a directory... have to scream?
                if (scream)
                {
                    if (_fatalFailures)
                    {
                        ACS_THROW (exIOException(errno) << "Unable to read the path " << path << " in FSTree. Path is not a directory");
                    }
                    else
                    {   
                        ACS_LOG_WARNING ("Got an error opening dir. Stack follows");
                        ACS_LOG_TRYCATCHNOTIFY_EX (exIOException(errno) << "Unable to read the path " << path << " in FSTree. Path is not a directory");
                    }
                }
            }
            else
            {
                if (_fatalFailures)
                {
                    ACS_THROW (exIOException(errno)<< "Unable to read the path " << path << " in FSTree");
                }
                else
                {   
                    ACS_LOG_WARNING ("Got an error opening dir. Stack follows");
                    ACS_LOG_TRYCATCHNOTIFY_EX (exIOException(errno)<< "Unable to read the path " << path << " in FSTree");
                }
            }
            return;
        }

		// reentrant version of readdir is used here
		// see man 3 readdir for details on the malloc used here

        size_t len = offsetof(struct dirent, d_name) + pathconf(path.c_str(), _PC_NAME_MAX) + 1;

        struct dirent* entry = reinterpret_cast<struct dirent*> ( malloc(len) ); //NOSONAR - dirent
		struct dirent* result = nullptr ;

		// This will automatically free the memory used by entry at dtor time
		pattern::CMemoryScopeGuard theGuard (entry);

		int readdirval = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		while ( ( 0 == (readdirval = readdir_r(dir, entry, &result) ) ) && result != nullptr )
        {
#pragma GCC diagnostic pop
            std::string name=entry->d_name;
            bool managingDot = ("." == name); 

		    if ( managingDot && curDepth) {
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
            struct stat s; 

            std::string filePath = path+"/"+(managingDot ? "" : name); 

            int statresult = (_resolveLinks ? stat (filePath.c_str(), &s) : lstat (filePath.c_str(), &s)); 

            if (0 != statresult)
            {
               ACS_LOG_WARNING ("Cannot stat the file " << filePath << "Continuing with the scan. ");
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
            if (_fatalFailures)
            {
                ::closedir (dir);
                ACS_THROW (exIOException(readdirval)<< "Cannot read the directory " << path);
            }
            else
            {   
                ACS_LOG_WARNING ("Cannot read the directory. Stack follows");
                ACS_LOG_TRYCATCHNOTIFY_EX (exIOException(readdirval)<< "Cannot read the directory " << path);
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
