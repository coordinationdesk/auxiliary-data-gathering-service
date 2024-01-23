// PRQA S 1050 EOF
/*
 *  hash.hpp - hash table template
 *
 *    Copyright (c) 2001-2004, Luxena Software Company. All rights reserved.
 *
 *  Purpose:
 *
 */

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include <memory.h>


/////////////////////////////////////////////////////////////////////////////
// _hash_t - hash table template class

template <typename _key_t, typename _value_t, class _compare_t>
class _hash_t
{
public:

	// typedef class _iterator_t iterator;
	//
	class _iterator_t;

	// hash item - contain hash key, associated value and chain next item
	//
	class _item_t
	{
	public:
		_key_t key;
		_value_t value;
	private:
		_item_t *pnext;
		friend class _hash_t<_key_t, _value_t, _compare_t>;
		friend class _iterator_t;
	};

	// iterator for hash table
	//
	class _iterator_t
	{
	public:
		_iterator_t()
		{
			pitem = 0;
		}
		_iterator_t& operator ++()
		{
			if (pitem != 0)
				setitem(pitem->pnext);
			return *this;
		}
		_iterator_t operator ++(int)
		{
			_item_t **begin = this->ppitems;
			_item_t **end = this->pplast;
			if (pitem != 0)
				setitem(pitem->pnext);
			return _iterator_t(begin, end);
		}
		_item_t* operator ->()
		{
			return pitem;
		}
		operator _item_t*()
		{
			return pitem;
		}
		friend class _hash_t<_key_t, _value_t, _compare_t>;
	private:
		void setitem(_item_t *pitem)
		{
			while ((pitem == 0) && (ppitems != pplast))
				pitem = *++ppitems;
			this->pitem = pitem;
		}
		_iterator_t(_item_t **begin, _item_t **end)
		{
			ppitems = begin;
			pplast = end;
			setitem(*ppitems);
		}
		_item_t **ppitems;
		_item_t **pplast;
		_item_t *pitem;
	};

	// constructor
	//
	_hash_t(unsigned int size) :
	    m_size(size), 
	    m_ppitems(0),
	    m_count(0),
        attribute1()
	{
		//m_size = size;
		m_ppitems = new _item_t * [m_size];
		memset(m_ppitems, 0, sizeof(_item_t *) * m_size);
		//m_count = 0;
	}

	// destructor
	//
	virtual ~_hash_t()
	{
		clear();
		delete [] m_ppitems;
	}

	// add new item
	//
	_item_t* add(const _key_t& key)
	{
		_item_t **ppitem = &m_ppitems[_compare_t::evaluate(key, m_size)];

		// create new hash item
		//
		_item_t *pitem = new _item_t;
		pitem->pnext = *ppitem;
		*ppitem = pitem;
		m_count++;
		pitem->key = key;

		return pitem;
	}

	// put if key already exists return associated item otherwise add new item
	//
	_item_t* put(const _key_t& key)
	{
		_item_t **ppitem = &m_ppitems[_compare_t::evaluate(key, m_size)];

		_item_t *pitem;

		// search for item
		//
		while ((pitem = *ppitem) != 0)
		{
			if (_compare_t::delta(pitem->key, key) == 0)
				return pitem;
			ppitem = &pitem->pnext;
		}
	
		// create new if not found
		//
		pitem = new _item_t;
		pitem->pnext = 0;
		*ppitem = pitem;
		m_count++;
		pitem->key = key;

		return pitem;
	}

	// get item
	//
	_item_t* get(const _key_t& key)
	{
		_item_t *pitem = m_ppitems[_compare_t::evaluate(key, m_size)];

		// search for item
		//
		while (pitem != 0)
		{
			if (_compare_t::delta(pitem->key, key) == 0)
				return pitem;
			pitem = pitem->pnext;
		}

		return 0;
	}

	// remove item
	//
	void * remove(const _key_t& key)
	{
		_item_t **ppitem = &m_ppitems[_compare_t::evaluate(key, m_size)];

		_item_t *pitem;

		// search for item
		//
		while ((pitem = *ppitem) != 0)
		{
			if (_compare_t::delta(pitem->key, key) == 0)
			{
				_item_t *pnext = pitem->pnext;
				delete pitem;
				m_count--;
				*ppitem = pnext;
				return pitem;
			}
			ppitem = &pitem->pnext;
		}

		return 0;
	}

	// clear
	//
	void clear()
	{
		_item_t **ppitem = m_ppitems;
		for (unsigned int i = 0; i < m_size; i++, ppitem++)
		{
			_item_t *pitem;
			while ((pitem = *ppitem) != 0)
			{
				_item_t *pnext = pitem->pnext;
				delete pitem;
				m_count--;
				*ppitem = pnext;
			}
		}
	}

	// iterators for hash enumerating
	//
	_iterator_t begin()
	{
		return _iterator_t(m_ppitems, &m_ppitems[m_size - 1]);
	}
	_iterator_t end()
	{
		return _iterator_t();
	}

	// number of items into the hash table
	//
	unsigned long count()
	{
		return m_count;
	}
private:
    _hash_t(const _hash_t &) ;
    _hash_t & operator= (const _hash_t &) ;
    
private:
	unsigned int m_size;
	_item_t **m_ppitems;
	unsigned int m_count;
    int attribute1;
};
