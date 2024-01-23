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
//    $Prod: A.C.S. SSL Library - Module: StreamCipher - HEADER FILE $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    
//////////////////////////////////////////////////////////////////////////////////////



#ifndef _ACS_SSL_STREAMCIPHER_H_
#define _ACS_SSL_STREAMCIPHER_H_

#include <acs_c++config.hpp>
#include <ACS_SSL.h> 
#include <ACS_SSL_CircularBuffer.h> 
#include <Thread.h> 
#include <exException.h>
#include <Filterables.h> 
#include <Guard.hpp>
#include <algorithm>

#include <stdint.h>


_ACS_BEGIN_NAMESPACE(acs)


/************************************************************************************
**                                                                                 **
            ACS_SSL_StreamCipher
**                                                                                 **
************************************************************************************/
class ACS_SSL_StreamCipher: public ACS_SSL_CircularBuffer
{
public:
	/************************************************************************************
	**                                                                                 **
            	ACS_SSL_StreamCipher::CircularBufferFillerThread
	**                                                                                 **
	************************************************************************************/
	/**
		Aim of this class is to provide a running thread to automatically fill the stream cipher buffer.
		The amount of bytes to fill in each call and the period of the loop are user defined.
	*/
	class CircularBufferFillerThread: public Thread  
	{
	public:

		explicit CircularBufferFillerThread(
			ACS_SSL_StreamCipher&,
			const off64_t& fill_bytes,
			const off64_t& delay_millisec
		 ) ;

		/** Destructor */
		virtual ~CircularBufferFillerThread() throw();

	public:


	private:  // copy constructor and operator= defined but not implemented
		/** Copy Constructor */
		CircularBufferFillerThread(const CircularBufferFillerThread & );
		/** Operator = */
		CircularBufferFillerThread &operator=(const CircularBufferFillerThread &);

	protected:
		/** Thread interface implementation */
		virtual void run();

	private:
		ACS_SSL_StreamCipher&		_stream_cipher ;
		off64_t 					_fill_bytes ;
		off64_t						_delay_millisec ;

	private:
		ACS_CLASS_DECLARE_DEBUG_LEVEL(CircularBufferFillerThread)

	};


public:
	/**
		Class constructor with self-allocation / deallocation of the buffer:
	*/
	ACS_SSL_StreamCipher(ACS_SSL::ACS_AES_handle& aes_handler, const off64_t& buffer_size) ;   
	/**
		Class constructor with external, pre-allocated buffer:
	*/
	ACS_SSL_StreamCipher(ACS_SSL::ACS_AES_handle& aes_handler, unsigned char* external_buffer_ptr, const off64_t& buffer_size) ;   
	~ACS_SSL_StreamCipher() throw() ;	

public:
	/**
		Read from the circular buffer and XOR with the user's buffer (therefore, ciphering and deciphering)
		Returns the amount actually copied. It will be <= the request, or even 0.
		If self_refill is set (recommended) the circular buffer will be automatically refilled if empty.
		This might result in a degradation of the performances (best performances are obtaining by filling
		the circular buffer BEFORE the actually cipering of deciphering)
	*/
	off64_t xor_with_circular_buffer(const off64_t& to_read, const unsigned char* buffer_in, const off64_t& bufsize_in, \
		unsigned char* buffer_out, const off64_t& bufsize_out, const bool self_refill=true) ; 		/* Different IN/OUT buffers */
	inline off64_t xor_with_circular_buffer(const off64_t& to_read, unsigned char* buffer, const off64_t& bufsize, const bool self_refill=true) 
	{ return xor_with_circular_buffer(to_read, buffer, bufsize, buffer, bufsize, self_refill) ; }	/* Same IN/OUT buffers */

	/**
		Fast-forward the circular buffer. Usually needed in case of USER'S DATA LOSS.
		Returns the amount actually skipped. It will be <= the request, or even 0.
		If self_refill is set (recommended) the circular buffer will be automatically refilled if empty.
		This might result in a degradation of the performances (best performances are obtaining by filling
		the circular buffer BEFORE the actually cipering of deciphering)
	*/
	off64_t advance_circular_buffer(const off64_t& to_skip, const bool self_refill=true) ; 	

	/**
		Read from the internal AES cipher and copy to the circular buffer.
		Returns the amount actually copied. It will be always <= the request, or 0 if the buffer is already full, 
		or even -1 if a different thread is filling at the same moment (so a second writer is not allowed)
	*/
	off64_t fill_circular_buffer(const off64_t& to_write) ;

private:
	inline static void xor3 (const unsigned char* src1, const unsigned char* src2, unsigned char* dst, const off64_t& length) 
    {
	    /* dst = XOR (src1, src2)  where src1 & src2 (and dst of course) are sequences of "length" bytes */
	    for (off64_t i=0; i<length; i++) { dst[i] = (src1[i] ^ src2[i]) ; } 
    }

private:			
	ACS_SSL_StreamCipher(const ACS_SSL_StreamCipher&) ;             /* Unimplemented */
	ACS_SSL_StreamCipher &operator=(const ACS_SSL_StreamCipher&) ;	/* Unimplemented */

#ifdef ACS_TEST
public:
#else
private:
#endif

private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACS_SSL_StreamCipher) ;

	ACS_SSL::ACS_AES_handle& _aes_handler ;
	off64_t _zerobuffer_size ; 
	unsigned char* _zerobuffer ;
} ;


_ACS_END_NAMESPACE

#endif	// _ACS_SSL_STREAMCIPHER_H_
