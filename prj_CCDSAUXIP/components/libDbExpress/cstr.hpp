// PRQA S 1050 EOF
/*
 *  cstr.hpp - C string wrapper and compare class for hash
 *
 *    Copyright (c) 2001-2004, Luxena Software Company. All rights reserved.
 *
 *  Purpose:
 *
 */

#ifndef __cstr_hpp__
#define __cstr_hpp__

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include <cctype>
#include <cstdlib>
#include <cstring>


/////////////////////////////////////////////////////////////////////////////
// C string wrapper

class _cstr_t // PRQA S 2109
{
public:
	_cstr_t() : _value(0) {}

	explicit _cstr_t(size_t size) : _value(0) { _value = new char [size]; }

	explicit _cstr_t(const char *value) : _value(0) { set(value); }

	_cstr_t(const _cstr_t& str) : _value(0) { set(str._value); }

	~_cstr_t() throw() { delete [] _value; }

	_cstr_t& operator = (const char *value) {
		delete [] _value;
		set(value);
		return *this;
	}

	_cstr_t& operator = (const _cstr_t& str) { if (&str != this ) { this->operator=(str._value) ; } return *this ; }

	operator char * () const { return _value; } // PRQA S 2181, 4628

	void attach (char *value) { delete [] _value; _value = value; }

	char * detach () { char *value = _value; _value = 0; return value; }

	static char * trim (char *value) {
		char *zero = value;
		while (*value != '\0') {
			if (*(value++) != ' ') {
                zero = value;
            }
        }
		*zero = '\0';
		return value;
	}
	
	static char * upperCase(char *value) {
		char *p = value;
		while (*p != '\0') {
			*p = toupper(*p); // PRQA S 3010
			p++ ;
		}
		return value;
	}

private:
	void set(const char *value) {
		if (value != 0) {
			size_t size = strlen(value) + 1; // PRQA S 3084
			_value = new char [size];
			memcpy(_value, value, size);
		} else {
			_value = 0;
        }
	}
private:

	char *_value;
};


/////////////////////////////////////////////////////////////////////////////
// C string compare class for hash table

class compare_cstr
{
public:
	static int delta(const char *left, const char *right) {
		return strcmp(left, right);
	}

	static unsigned int evaluate(const char *key, long tablesize) {
		unsigned int result = 0;
		while (*key != 0) {
			result = ((result << 1) ^ tablesize) + *(key++); // PRQA S 3000, 3003, 3010
        }
		return result % tablesize; // PRQA S 3000, 3010, 3084
	}
};
#endif /*  __cstr_hpp__ */
