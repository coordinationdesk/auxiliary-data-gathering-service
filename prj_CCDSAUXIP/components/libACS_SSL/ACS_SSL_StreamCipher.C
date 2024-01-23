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
//    $Prod: A.C.S. SSL Library - Module: StreamCipher $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//
//////////////////////////////////////////////////////////////////////////////////////


#include <openssl/rsa.h> // PRQA S 1013
#include <openssl/rand.h> // PRQA S 1013
#include <ACS_SSL_StreamCipher.h> 
#include <Filterables.h> 
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>

#define ACS_SSL_HAS_DEBUG_ENABLED 1	

_ACS_BEGIN_NAMESPACE(acs)

namespace {
}

using namespace std ;



/*-----------------------------------------------------------------------------------

	CLASS ACS_SSL_StreamCipher

-----------------------------------------------------------------------------------*/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACS_SSL_StreamCipher);


/*-----------------------------------------------------------------------------------

	CLASS ACS_SSL_StreamCipher::CircularBufferFillerThread

-----------------------------------------------------------------------------------*/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACS_SSL_StreamCipher::CircularBufferFillerThread);


/************************************************************************************
**                                                                                 **
		ACS_SSL_StreamCipher::CircularBufferFillerThread
**                                                                                 **
************************************************************************************/
ACS_SSL_StreamCipher::CircularBufferFillerThread::CircularBufferFillerThread(				\
													ACS_SSL_StreamCipher& stream_cipher,	\
													const off64_t& fill_bytes,				\
													const off64_t& delay_millisec) :
	_stream_cipher(stream_cipher),
	_fill_bytes(fill_bytes),
	_delay_millisec(delay_millisec)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. fill_bytes: " << fill_bytes << "  delay_millisec: " << delay_millisec << endl ;
	ACS_CLASS_END_DEBUG
#endif 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_StreamCipher::~CircularBufferFillerThread
**                                                                                 **
************************************************************************************/
ACS_SSL_StreamCipher::CircularBufferFillerThread::~CircularBufferFillerThread() throw()
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) 
#endif 

	Thread::stop() ; 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_StreamCipher::CircularBufferFillerThread::run
**                                                                                 **
************************************************************************************/
void ACS_SSL_StreamCipher::CircularBufferFillerThread::run()
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	while ( false == requestedCancel() )
	{
		off64_t filled ( _stream_cipher.fill_circular_buffer(_fill_bytes) ) ;
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Filled the circual buffer with: " << filled << " bytes" << endl ;
	ACS_CLASS_END_DEBUG
#endif 

		filled ++ ; // Totally useless -- just avoid a warning of "variable not used" !!!

		for (off64_t i=0; ((i<_delay_millisec) && (false == requestedCancel())); i++)
		{ usleep (1000) ; /* 1 millisec */ }
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
#endif 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_StreamCipher [internal allocation]
**                                                                                 **
************************************************************************************/
ACS_SSL_StreamCipher::ACS_SSL_StreamCipher(ACS_SSL::ACS_AES_handle& aes_handler, const off64_t& buffer_size) :
	ACS_SSL_CircularBuffer(buffer_size), \
	_aes_handler(aes_handler),
	_zerobuffer_size(16*1024*1024),
	_zerobuffer(NULL)
{
	/**
		Class constructor with self-allocation / deallocation of the buffer:
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. buffer_size to allocate: " << buffer_size << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	// This constructor also allocates a buffer of zeros, used to feed the underlying AES cipher
	ACS_COND_THROW( (_zerobuffer_size < 1), exIllegalValueException("Invalid zerobuffer size: " + to_string(_zerobuffer_size)) ) ;

	_zerobuffer = static_cast<unsigned char*>(malloc(_zerobuffer_size)) ;
	ACS_COND_THROW( (NULL == _zerobuffer), exIllegalValueException("Unable to allocate zerobuffer: " + to_string(_zerobuffer_size)) ) ;
	memset (_zerobuffer, 0x00, _zerobuffer_size) ; 


	const ACS_SSL::ACS_AES_Cipher cipher(aes_handler.getCipher()) ; 
	
	if ( (cipher == ACS_SSL::AES_128_OFB) || (cipher == ACS_SSL::AES_192_OFB)|| (cipher == ACS_SSL::AES_256_OFB) )
	{
		// Cipher supported.
		
		ACS_COND_THROW( (true == aes_handler.isFinalized()), \
			exIllegalValueException("AES Handler already finalized") ) ; 
	}
	else
	{ ACS_THROW( exIllegalValueException("Unsupported cipher: " + ACS_SSL::toString(cipher)) ) ; }

	ACS_COND_THROW( (false == aes_handler.paddingSet()), exIllegalValueException("The padding should be enforced") ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
#endif 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_StreamCipher [external buffer pointer]
**                                                                                 **
************************************************************************************/
ACS_SSL_StreamCipher::ACS_SSL_StreamCipher(ACS_SSL::ACS_AES_handle& aes_handler, unsigned char* external_buffer_ptr, const off64_t& buffer_size) :
	ACS_SSL_CircularBuffer(external_buffer_ptr, buffer_size), \
	_aes_handler(aes_handler),
	_zerobuffer_size(16*1024*1024),
	_zerobuffer(NULL)
{
	/**
		Class constructor with external, pre-allocated buffer:
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. external_buffer_ptr: " << static_cast<void*>(external_buffer_ptr) << "  external buffer_size: " << buffer_size << endl ;
	ACS_CLASS_END_DEBUG
#endif 
	
	ACS_COND_THROW( (_zerobuffer_size < 1), exIllegalValueException("Invalid zerobuffer size: " + to_string(_zerobuffer_size)) ) ;

	// This constructor also allocates a buffer of zeros, used to feed the underlying AES cipher
	_zerobuffer = static_cast<unsigned char*>(malloc(_zerobuffer_size)) ;
	ACS_COND_THROW( (NULL == _zerobuffer), exIllegalValueException("Unable to allocate zerobuffer: " + to_string(_zerobuffer_size)) ) ;
	memset (_zerobuffer, 0x00, _zerobuffer_size) ; 

	
	const ACS_SSL::ACS_AES_Cipher cipher(aes_handler.getCipher()) ; 
	
	if ( (cipher == ACS_SSL::AES_128_OFB) || (cipher == ACS_SSL::AES_192_OFB)|| (cipher == ACS_SSL::AES_256_OFB) )
	{
		// Cipher supported.

		ACS_COND_THROW( (true == aes_handler.isFinalized()), \
			exIllegalValueException("AES Handler already finalized") ) ; 
	}
	else
	{ ACS_THROW( exIllegalValueException("Unsupported cipher: " + ACS_SSL::toString(cipher)) ) ; }

	ACS_COND_THROW( (false == aes_handler.paddingSet()), exIllegalValueException("The padding should be enforced") ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
#endif 
}


/************************************************************************************
**                                                                                 **
		~ACS_SSL_StreamCipher 
**                                                                                 **
************************************************************************************/
ACS_SSL_StreamCipher::~ACS_SSL_StreamCipher() throw() 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) 
#endif 

	if (NULL != _zerobuffer)
	{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Deallocating zerobuffer") ; 
#endif 
		free (_zerobuffer) ; 
	}

	_zerobuffer			= NULL ; 
	_zerobuffer_size	= 0 ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
#endif 
} 	


/************************************************************************************
**                                                                                 **
		ACS_SSL_StreamCipher::fill_circular_buffer
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_StreamCipher::fill_circular_buffer(const off64_t& to_write_in)
{
	/**
		Read from the internal AES cipher and copy to the circular buffer.
		Returns the amount actually copied. It will be always <= the request, or 0 if the buffer is already full, 
		or even -1 if a different thread is filling at the same moment (so a second writer is not allowed)
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to write: " << to_write_in << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t still_to_write(to_write_in) ; 
	off64_t written(0) ;  
	
	if (to_write_in > 0)
	{
		do
		{
			ACS_COND_THROW( (still_to_write < 0), exBoundsException("Invalid (negative) amount to write") ) ;

			off64_t cb_offset ;
			const off64_t to_write ( prepare_writer(still_to_write, cb_offset) ) ; 

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				excerr << SimpleDebugSignatureThread << "Amount allowed to write in this iteration: " << to_write << "  from offset: " << cb_offset<< endl ;
			ACS_CLASS_END_DEBUG
#endif 

			if (to_write > 0)
			{
				// The circular buffer has free space
				try
				{
					const off64_t w (min(to_write, _zerobuffer_size)) ; 
					ACS_COND_THROW( ((cb_offset + w) > cbuffsize()), exIllegalValueException("Circular Buffer Exceeded") ) ;

					size_t bufout_size(cbuffsize() - cb_offset) ;
					ACS_SSL::ACS_AES_encrypt(_aes_handler, _zerobuffer, w, cbuffer(cb_offset), bufout_size) ; 

					if (static_cast<off64_t>(bufout_size) != w)
					{
						ostringstream o ; 
						o << "Internal error when filling circular buffer. cb_offset: " << cb_offset \
							<< "  Expected to fill: " << w << "  filled: " << bufout_size ;
						ACS_THROW( exIllegalValueException(o.str()) ) ; 
					}

					written += w; 
					if ((w >=0) && (still_to_write >= w)) { still_to_write -= w ; } else { still_to_write = 0 ; }

					finalize_writer(w) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
					ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
						excerr << SimpleDebugSignatureThread << "Amount actually written in this iteration: " << w << endl ;
					ACS_CLASS_END_DEBUG
#endif 
				}
				catch(exception& e)
				{
					finalize_writer() ;
					throw ;
				}		
			}
			else if (to_write == 0)
			{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					excerr << SimpleDebugSignatureThread << "Circular buffer full." << endl ;
				ACS_CLASS_END_DEBUG
#endif 

				// The circular buffer does not have any free space. Finalize (abort) the writing and quit immediately
				finalize_writer() ;
				break ; 
			}
			else
			{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					excerr << SimpleDebugSignatureThread << "Writing in circular buffer not allowed." << endl ;
				ACS_CLASS_END_DEBUG
#endif 

				// Writing is not allowed. The writing has not started, so quit immediately.
				
				// The point of the "IF" below is the following:
				// The caller is waiting for a buffer to be filled. If at least a portion of the amount is written, 
				// (this method returning a positive value) the caller will process this amount then try again and so on.
				// If no portion of buffer if written by this method (returning 0) the meaning is that 
				// "the buffer is already filled", so the caller should have a whole buffer ready. 
				// No further loops to fill the buffer will be attempted by the caller.
				// If the writer is locked (this block of code), either return A POSITIVE VALUE of the (few) bytes written here,
				// or return -1 since the writing is performed by a different thread.
				//
				// Vice-versa, returning 0 would be interpreted as: "nothing to write, buffer is ready" which is indeed false !
				// Returning -1 will make the caller to just wait for the second thread to fill the buffer.
				
				if (0 == written) { written = -1 ; }	// Signal that writing is not allowed. Return either a positive value 
														// or a negative value, never return 0.
				
				break ; 
			}
		}
		while (still_to_write > 0) ;

	}	// END: if (to_write_in > 0)

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Total amount written in circular buffer: " << written << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return written ;
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::xor_with_circular_buffer
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_StreamCipher::xor_with_circular_buffer(const off64_t& to_read_in, \
	const unsigned char* buffer_in, const off64_t& bufsize_in, \
	unsigned char* buffer_out, const off64_t& bufsize_out, const bool self_refill) 
{
	/**
		Read from the circular buffer and XOR with the user's buffer (therefore, ciphering and deciphering)
		Returns the amount actually copied. It will be <= the request, or even 0.
		If self_refill is set (recommended) the circular buffer will be automatically refilled if empty.
		This might result in a degradation of the performances (best performances are obtaining by filling
		the circular buffer BEFORE the actually cipering of deciphering)
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to read: " << to_read_in << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t still_to_read (to_read_in), to_read ;
	off64_t offset (0) ; // offset on the user buffer
	off64_t filled (0) ; // amount filled in the last iteration

	do 
	{
		ACS_COND_THROW( (still_to_read < 0), exBoundsException("Invalid (negative) amount to read") ) ;
		off64_t cb_offset ;	// offset on the circular buffer
		to_read = prepare_reader(still_to_read, cb_offset) ; // to_read is always >= 0, it cannot be negative. 

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			excerr << SimpleDebugSignatureThread << "Amount allowed to read in this iteration: " << to_read << "  from offset: " << cb_offset<< endl ;
		ACS_CLASS_END_DEBUG
#endif 

		if (to_read >= 0)
		{
			ACS_COND_THROW( ((cb_offset + to_read) > cbuffsize()), exIllegalValueException("Circular Buffer Exceeded") ) ;
			ACS_COND_THROW( ((offset + to_read) > bufsize_in), exIllegalValueException("Invalid input buffer provided: too small") ) ;
			ACS_COND_THROW( ((offset + to_read) > bufsize_out), exIllegalValueException("Invalid output buffer provided: too small") ) ;
		
			try 
			{
				if (to_read > 0)
				{
					// APPLY THE STREAM CIPHER.
					// That is, XOR the circular buffer and the user buffer
					xor3( cbuffer(cb_offset), (buffer_in + offset), (buffer_out + offset), to_read) ;

					offset += to_read ;
				}

				// Compute the remainder for the next iteration (assuming that self_refill is set) :
				if (still_to_read >= to_read) { still_to_read -= to_read ; } else { still_to_read = 0 ; }

				// Close the reader (so far)
				finalize_reader(to_read) ;
			}
			catch(exception& e)
			{
				finalize_reader() ;
				throw ; 
			}
		}
		else
		{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				excerr << SimpleDebugSignatureThread << "Reading not allowed" << endl ;
			ACS_CLASS_END_DEBUG
#endif 

			// Reading is not allowed -- THIS SHOULD BE PREVENTED (an exception should have been thrown before reaching this point)
			break ; 
		}
		
		// If needed, fill the circular buffer with the remainder:
		if ( (true == self_refill) && (still_to_read > 0) )
		{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				excerr << SimpleDebugSignatureThread << "Attempting self refilling of buffer by: " <<  still_to_read << endl ; 
			ACS_CLASS_END_DEBUG
#endif 
		
			if ((filled = fill_circular_buffer(still_to_read)) < 0) 
			{
				// Another code unit is writing right now, therefore the writer is busy.
				usleep(100) ; 

				filled = 1 ; // Fake value, used only to force next iteration
			}
			
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				excerr << SimpleDebugSignatureThread << "Buffer self refilled by: " <<  filled << endl ; 
			ACS_CLASS_END_DEBUG
#endif 
		}
		else
		{
			filled = 0 ; // Nothing new added to the buffer
		}
		
		// The loop is continued if:
		// There is STILL something to read (of course)
		// and the loop is actually working, that is, in the next iteration, either was able to partially read
		// or was able to partially write -- since the class can have only one reader, if it has just written 
		// it will be able to read in the next iteration.
		// The only possible deadlock should be NOTHING TO WRITE and NOTHING TO READ but the "while"
		// prevents that.
		
	} while ( (still_to_read > 0) && ((to_read > 0) || (filled > 0)) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Total amount read from circular buffer: " << offset << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return offset ; 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::advance_circular_buffer
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_StreamCipher::advance_circular_buffer(const off64_t& to_skip_in, const bool self_refill) 
{
	/**
		Fast-forward the circular buffer. Usually needed in case of USER'S DATA LOSS.
		Returns the amount actually skipped. It will be <= the request, or even 0.
		If self_refill is set (recommended) the circular buffer will be automatically refilled if empty.
		This might result in a degradation of the performances (best performances are obtaining by filling
		the circular buffer BEFORE the actually cipering of deciphering)
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to skip: " << to_skip_in << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t still_to_skip (to_skip_in), to_skip ;
	off64_t offset (0) ; // offset on the user buffer
	off64_t filled (0) ; // amount filled in the last iteration

	do 
	{
		ACS_COND_THROW( (still_to_skip < 0), exBoundsException("Invalid (negative) amount to skip") ) ;
		off64_t cb_offset ;	// offset on the circular buffer
		to_skip = prepare_reader(still_to_skip, cb_offset) ; // to_skip is always >= 0, it cannot be negative. 

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			excerr << SimpleDebugSignatureThread << "Amount allowed to skip in this iteration: " << to_skip << "  from offset: " << cb_offset<< endl ;
		ACS_CLASS_END_DEBUG
#endif 

		if (to_skip >= 0)
		{
			ACS_COND_THROW( ((cb_offset + to_skip) > cbuffsize()), exIllegalValueException("Circular Buffer Exceeded") ) ;
		
			try 
			{
				offset += to_skip ;
			
				// Compute the remainder for the next iteration (assuming that self_refill is set) :
				if (still_to_skip >= to_skip) { still_to_skip -= to_skip ; } else { still_to_skip = 0 ; }

				// Close the reader (so far)
				finalize_reader(to_skip) ;
			}
			catch(exception& e)
			{
				finalize_reader() ;
				throw ; 
			}
		}
		else
		{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				excerr << SimpleDebugSignatureThread << "Reading not allowed" << endl ;
			ACS_CLASS_END_DEBUG
#endif 

			// Reading is not allowed -- THIS SHOULD BE PREVENTED (an exception should have been thrown before reaching this point)
			break ; 
		}
		
		// If needed, fill the circular buffer with the remainder:
		if ( (true == self_refill) && (still_to_skip > 0) )
		{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				excerr << SimpleDebugSignatureThread << "Attempting self refilling of buffer by: " <<  still_to_skip << endl ; 
			ACS_CLASS_END_DEBUG
#endif 
		
			if ((filled = fill_circular_buffer(still_to_skip)) < 0) 
			{
				// Another code unit is writing right now, therefore the writer is busy.
				usleep(100) ; 

				filled = 1 ; // Fake value, used only to force next iteration
			}
			
#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				excerr << SimpleDebugSignatureThread << "Buffer self refilled by: " <<  filled << endl ; 
			ACS_CLASS_END_DEBUG
#endif 
		}
		else
		{
			filled = 0 ; // Nothing new added to the buffer
		}
		
		// The loop is continued if:
		// There is STILL something to read (of course)
		// and the loop is actually working, that is, in the next iteration, either was able to partially read
		// or was able to partially write -- since the class can have only one reader, if it has just written 
		// it will be able to read in the next iteration.
		// The only possible deadlock should be NOTHING TO WRITE and NOTHING TO READ but the "while"
		// prevents that.
		
	} while ( (still_to_skip > 0) && ((to_skip > 0) || (filled > 0)) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED        
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Total amount skipped from circular buffer: " << offset << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return offset ; 
}

_ACS_END_NAMESPACE
