// PRQA S 1050 EOF
//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 2022-, Exprivia S.p.A.  -  DFDA-AS
//    Via Della Bufalotta, 378 - 00139 Roma - Italy
//    http://www.acsys.it
//
//    All Rights Reserved.
//
//    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
//    the contents of this file may not be disclosed to third parties, copied or
//    duplicated in any form, in whole or in part, without the prior written
//    permission of Exprivia S.p.A. 
//
//    $Prod: A.C.S. SSL Library - Module: CircularBuffer - HEADER FILE $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    
//////////////////////////////////////////////////////////////////////////////////////



#ifndef _ACS_SSL_CIRCULARBUFFER_H_
#define _ACS_SSL_CIRCULARBUFFER_H_

#include <acs_c++config.hpp>
#include <ACS_SSL.h> 
#include <exException.h>
#include <Filterables.h> 
#include <Guard.hpp>
#include <algorithm>

#include <stdint.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
	Implementation of a standard Circular Buffer.

	The main aim of this class is the integration with StreamCiphers, which use a circular buffer to store the
	pre-computed ciphering sequence (the user-data is simply XORed with the ciphering sequence).
	
	The buffer has a maximum size (capacity) and can be either pre-allocated by the caller 
	or automatically allocated/deallocated by the class.
	
	The class offers two standard methods:
	- read_circular_buffer, to copy data from the circular buffer to the user's local buffer
	- write_circular_buffer, to copy data from the user's local buffer to the circular buffer
	
	It's worth noting that both methods return the ACTUAL AMOUNT OF BYTES READ/WRITTEN.
	It's up to the user to verify the portion of data actually transferred, which can be LESS than the total amount to transfer,
	or even be zero (if the buffer is empty when reading, or full when writing)
	
	
	Example of a typical session:
	
	Circular buffer of 1024 bytes, initially empty.
	
	- read_circular_buffer(64) -> return 0 
	- read_circular_buffer(1025) -> return 0 
	- write_circular_buffer(24) -> return 24 
	- write_circular_buffer(1001) -> return 1000 
	- read_circular_buffer(1000) -> return 1000 
	- read_circular_buffer(25) -> return 24 
	- read_circular_buffer(1) -> return 0 
	- write_circular_buffer(1025) -> return 1024
	- write_circular_buffer(1) -> return 0
	- read_circular_buffer(2048) -> return 1024
	 
	...

*/


/************************************************************************************
**                                                                                 **
            ACS_SSL_CircularBuffer
**                                                                                 **
************************************************************************************/
class ACS_SSL_CircularBuffer
{
public:
	/**
		Class constructor with self-allocation / deallocation of the buffer:
	*/
	ACS_SSL_CircularBuffer(const off64_t& buffer_size) ;	
	/**
		Class constructor with external, pre-allocated buffer:
	*/
	ACS_SSL_CircularBuffer(unsigned char* external_buffer_ptr, const off64_t& buffer_size) ;	
	virtual ~ACS_SSL_CircularBuffer() throw() ;	
	ACS_SSL_CircularBuffer(const ACS_SSL_CircularBuffer&) = delete ;			/* Unimplemented */
	ACS_SSL_CircularBuffer &operator=(const ACS_SSL_CircularBuffer&) = delete ;	/* Unimplemented */

public:

	/**
		Read from the circular buffer and copy to the caller's buffer.
		Returns the amount actually copied. It will be <= the request, or even 0.
	*/
	off64_t read_circular_buffer(const off64_t& to_read, unsigned char* buffer, const off64_t& bufsize) ;
	/**
		Read from the caller's buffer and copy to the circular buffer.
		Returns the amount actually copied. It will be <= the request, or even 0.
	*/
	off64_t write_circular_buffer(const off64_t& to_write, const unsigned char* buffer, const off64_t& bufsize) ;

#ifdef ACS_TEST
public:
#else
protected:
#endif
	/**
		Dump parameters for debugging purposes:
	*/
	std::string dump_parameters (const std::string& t="") ; 
	inline off64_t cbuffsize() const { return _cbuffer_size ; }
	inline unsigned char* cbuffer(const off64_t& offset=0) const { return (_cbuffer + offset) ; }
	/**
		Extract parameters for debugging purposes:
	*/
	void extract_parameters(bool& is_reading, bool& is_writing, off64_t& cursor_R, off64_t& occupation) const ; 

	/**
		Verify if the reading is allowed. Raise exception if another reading operation is in progress.
		Declare the reading operation and return the correct maximum amount to read (even 0), and the starting cursor.
	*/
	off64_t prepare_reader(const off64_t& to_read, off64_t& cursor)	; 
	/**
		Successful reading. Update the internal cursor and clear the reading operation.
	*/
	off64_t finalize_reader(const off64_t& read_bytes)			throw() ; 	/* Successfull Reading */
	/**
		Unsuccessful reading. The internal cursor is not updated, just clear the reading operation.
	*/
	off64_t finalize_reader()									throw() ;	/* Not successfull Reading */
	/**
		Verify if the writing is allowed. Return -1 if another writing operation is in progress.
		Otherwise, declare the writing operation and return the correct maximum amount to write (even 0), and the starting cursor.
	*/
	off64_t prepare_writer(const off64_t& to_write, off64_t& cursor)	throw() ; 
	/**
		Successful writing. Update the internal cursor and clear the writing operation.
	*/
	off64_t finalize_writer(const off64_t& written_bytes)		throw() ; 	/* Successfull Writing */
	/**
		Unsuccessful writing. The internal cursor is not updated, just clear the writing operation.
	*/
	off64_t finalize_writer()									throw() ;	/* Not successfull Writing */

	inline static off64_t min(const off64_t& a, const off64_t& b) { return ( (a<b)?a : b)  ; }
	inline static off64_t min(const off64_t& a, const off64_t& b, const off64_t& c) { return min(a, min(b,c)) ; } 

private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACS_SSL_CircularBuffer) ;

private:	
	static acs::ThreadSafe	_cursorMutex ;
	unsigned char*			_cbuffer ;				/* The pointer to the circular buffer */ 
	off64_t					_cbuffer_size ;			/* The size of the circular buffer */ 
	bool					_handle_alloc_dealloc ;	/* If true, the deallocation (i.e. the free) is in charge of this class */ 
	off64_t					_cursor_R ;
	off64_t 				_occupation ;
	bool 					_isReading ;
	bool 					_isWriting ;
} ;


_ACS_END_NAMESPACE

#endif	// _ACS_SSL_CIRCULARBUFFER_H_
