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
//    $Prod: A.C.S. SSL Library - Module: CircularBuffer $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//
//////////////////////////////////////////////////////////////////////////////////////


#include <ACS_SSL_CircularBuffer.h> 
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

ThreadSafe ACS_SSL_CircularBuffer::_cursorMutex ; 


/*-----------------------------------------------------------------------------------

	CLASS ACS_SSL_CircularBuffer

-----------------------------------------------------------------------------------*/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACS_SSL_CircularBuffer);


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer [internal allocation]
**                                                                                 **
************************************************************************************/
ACS_SSL_CircularBuffer::ACS_SSL_CircularBuffer(const off64_t& buffer_size) :
	_cbuffer(NULL),				
	_cbuffer_size(buffer_size),			
	_handle_alloc_dealloc(true),	
	_cursor_R(0),
	_occupation(0), 
	_isReading(false),
	_isWriting(false)
{
/**
	Class constructor with self-allocation / deallocation of the buffer:
*/
        
#if(!0)
    // IF HERE, DEBUG ALWAYS ALLOWED
    #define ACS_SSL_HAS_DEBUG_ENABLED 1	
#endif


#if(0)
    // IF HERE, NO DEBUG CAN BE ALLOWED
    #undef ACS_SSL_HAS_DEBUG_ENABLED 
#endif


#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. buffer_size to allocate: " << buffer_size << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW( (buffer_size < 1), exIllegalValueException("Invalid buffer size: " + to_string(buffer_size)) ) ;

	_cbuffer = static_cast<unsigned char*>(malloc(buffer_size)) ;
	ACS_COND_THROW( (NULL == _cbuffer), exIllegalValueException("Unable to allocate memory: " + to_string(buffer_size)) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Allocated pointer: " << static_cast<void*>(_cbuffer) << endl ;
	ACS_CLASS_END_DEBUG
#endif 
} 


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer [external buffer pointer]
**                                                                                 **
************************************************************************************/
ACS_SSL_CircularBuffer::ACS_SSL_CircularBuffer(unsigned char* external_buffer_ptr, const off64_t& buffer_size) :
	_cbuffer(external_buffer_ptr),				
	_cbuffer_size(buffer_size),			
	_handle_alloc_dealloc(false),	
	_cursor_R(0),
	_occupation(0), 
	_isReading(false),
	_isWriting(false)
{        
	/**
		Class constructor with external, pre-allocated buffer:
	*/
	
#if(!0)
    // IF HERE, DEBUG ALWAYS ALLOWED
    #define ACS_SSL_HAS_DEBUG_ENABLED 1	
#endif


#if(0)
    // IF HERE, NO DEBUG CAN BE ALLOWED
    #undef ACS_SSL_HAS_DEBUG_ENABLED 
#endif


#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. external_buffer_ptr: " << static_cast<void*>(_cbuffer) \
			<< "  external buffer_size: " << buffer_size << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW( (buffer_size < 1), exIllegalValueException("Invalid buffer size: " + to_string(buffer_size)) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
#endif 
} 


/************************************************************************************
**                                                                                 **
		~ACS_SSL_CircularBuffer 
**                                                                                 **
************************************************************************************/
ACS_SSL_CircularBuffer::~ACS_SSL_CircularBuffer() throw() 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) 
#endif 

	if (false == _handle_alloc_dealloc)
	{ 
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Deallocation not handled by the d'tor.") ; 
#endif 
	}
	else
	{
		if (NULL != _cbuffer)
		{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "D'tor deallocating internal buffer ...") ; 
#endif 
			free (_cbuffer) ; 
		}
		
		_cbuffer		= NULL ; 
		_cbuffer_size	= 0 ;
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ; 
#endif 
} 	


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::extract_parameters 
**                                                                                 **
************************************************************************************/
void ACS_SSL_CircularBuffer::extract_parameters(bool& is_reading, bool& is_writing, off64_t& cursor_R, off64_t& occupation) const
{
	/**
		Extract parameters for debugging purposes:
	*/
	
	{
		Lock guard(_cursorMutex); 

		is_reading =  _isReading ;
		is_writing =  _isWriting ;
		cursor_R =	  _cursor_R ;
		occupation = _occupation ;
		
	}	// END SCOPE: guard
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::dump_parameters 
**                                                                                 **
************************************************************************************/
string ACS_SSL_CircularBuffer::dump_parameters(const string& t) 
{
	/**
		Dump parameters for debugging purposes:
	*/

	bool is_reading, is_writing ;
	off64_t occupation, cursor_R ;
	extract_parameters(is_reading, is_writing, cursor_R, occupation) ;

	ostringstream o ; o \
		<< t << "cursor_R: " << cursor_R << "\n" \
		<< t << "cursor_W [computed]: " << ((cursor_R + occupation) % cbuffsize()) << "\n" \
		<< t << "occupation: " << occupation << "; " \
		<< t << "is_reading: " << (is_reading?"T":"F") << "; " \
		<< t << "is_writing: " << (is_writing?"T":"F") \
	;
	
	return o.str() ;
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::prepare_reader 
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::prepare_reader(const off64_t& to_read, off64_t& cursor_R) 
{
	/**
		Verify if the reading is allowed. Raise exception if another reading operation is in progress.
		Declare the reading operation and return the correct maximum amount to read (even 0), and the starting cursor.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to read: " << to_read << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t ret_value ;

	Lock guard(_cursorMutex); 

	if ( ! _isReading )
	{
		_isReading = true ; // Declare reading has started

		cursor_R = _cursor_R = (_cursor_R % cbuffsize()) ;
		
		// The amount to read cannot exceed the end of the buffer, nor the capacity, nor the amount of user-data
		ret_value = min( to_read, _occupation, (cbuffsize() - cursor_R) ) ;			
	}
	else
	{
		// IF HERE: Another reader is running at the same time !
		// THIS CAN BRING TO UNWANTED RESULTS, since the reading buffer is accessed
		// in some unregulated mode !
		// ONLY ONE READER CAN ACCESS THE ACS_SSL_CircularBuffer CLASS !
		ACS_THROW(exIllegalValueException("Reading not allowed. Either a finalized was skipped, or another reader is running !")) ;
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Amount allowed to read: " << ret_value << " from offset: " << cursor_R << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return ret_value ; // Always >= 0
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::finalize_reader [success]
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::finalize_reader(const off64_t& read_bytes) throw() 
{
	/**
		Successful reading. Update the internal cursor and clear the reading operation.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Successful reading: " << read_bytes << endl ;
	ACS_CLASS_END_DEBUG
#endif 
	
	off64_t ret_value ;

	{
		Lock guard(_cursorMutex); 

		if (true == _isReading)
		{
			_isReading = false ; // Declare reading has finished
					
			_occupation = ( ((_occupation - read_bytes) > 0)? (_occupation - read_bytes) : 0 ) ;
			_cursor_R += read_bytes ;
		}
		else
		{
			// This should never happen
		}
		
		_cursor_R %= cbuffsize() ;
		ret_value = _cursor_R ;	

	}	// END SCOPE: guard

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. New position of the reading cursor: " << ret_value << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return ret_value ;
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::finalize_reader [failure]
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::finalize_reader() throw() 
{
	/**
		Unsuccessful reading. The internal cursor is not updated, just clear the reading operation.
	*/
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
#endif 
	off64_t ret_value ;

	{
		Lock guard(_cursorMutex); 

		if (true == _isReading) { _isReading = false ; /* Declare reading has finished */ }

		_cursor_R %= cbuffsize() ;
		ret_value = _cursor_R ;	

	}	// END SCOPE: guard

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Position of the reading cursor: " << ret_value << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return ret_value ;
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::prepare_writer 
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::prepare_writer(const off64_t& to_write, off64_t& cursor_W) throw() 
{
	/**
		Verify if the writing is allowed. Return -1 if another writing operation is in progress.
		Otherwise, declare the writing operation and return the correct maximum amount to write (even 0), and the starting cursor.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to write: " << to_write << endl ;
	ACS_CLASS_END_DEBUG
#endif 
	
	off64_t ret_value ;
	
	Lock guard(_cursorMutex); 

	cursor_W = ( (_cursor_R + _occupation) % cbuffsize() ) ;

	if ( ! _isWriting )
	{
		_isWriting = true ; // Declare writing has started

		// The amount to write cannot exceed the end of the buffer, nor the capacity, nor the amount of user-data
		ret_value = min( to_write, (cbuffsize() - _occupation), (cbuffsize() - cursor_W) ) ;			
	}
	else
	{
		ret_value = -1 ; // Writing not enabled -- the caller won't have to call the finalize !
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Amount allowed to write: " << ret_value << " from offset: " << cursor_W << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return ret_value ; 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::finalize_writer [success]
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::finalize_writer(const off64_t& written_bytes) throw() 
{
	/**
		Successful writing. Update the internal cursor and clear the writing operation.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Successful writing: " << written_bytes << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t ret_value ;

	{
		Lock guard(_cursorMutex); 

		if (true == _isWriting)
		{
			_isWriting = false ; // Declare writing has finished
		
			_occupation = min( (_occupation + written_bytes), cbuffsize()) ; 
		}
		else
		{
			// This should never happen
		}			
			
		ret_value = ( (_cursor_R + _occupation) % cbuffsize() ) ;
		
	}	// END SCOPE: guard

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. New position of the writing cursor: " << ret_value << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return ret_value ;
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::finalize_writer [failure]
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::finalize_writer() throw() 
{
	/**
		Unsuccessful writing. The internal cursor is not updated, just clear the writing operation.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t ret_value ;

	{
		Lock guard(_cursorMutex); 

		if (true == _isWriting) { _isWriting = false ; /* Declare reading has finished */ }
		
		ret_value = ( (_cursor_R + _occupation) % cbuffsize() ) ;

	}	// END SCOPE: guard

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Position of the writing cursor: " << ret_value << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return ret_value ;
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::read_circular_buffer
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::read_circular_buffer(const off64_t& to_read_in, unsigned char* buffer, const off64_t& bufsize) 
{
	/**
		Read from the circular buffer and copy to the caller's buffer.
		Returns the amount actually copied. It will be <= the request, or even 0.
	*/
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to read: " << to_read_in << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t still_to_read (to_read_in) ;
	off64_t offset (0) ; // offset on the user buffer

	do 
	{
		ACS_COND_THROW( (still_to_read < 0), exBoundsException("Invalid (negative) amount to read") ) ;
	
		off64_t cb_offset ;
		const off64_t to_read ( prepare_reader(still_to_read, cb_offset) ) ; // to_read is always >= 0, it cannot be negative.

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			excerr << SimpleDebugSignatureThread << "Method called. Amount allowed to read: " << to_read << endl ;
		ACS_CLASS_END_DEBUG
#endif 
		if (to_read > 0)
		{
			try 
			{
				ACS_COND_THROW( ((cb_offset + to_read) > cbuffsize()), exIllegalValueException("Circular Buffer Exceeded") ) ;
				ACS_COND_THROW( ((offset + to_read) > bufsize), exIllegalValueException("Invalid buffer provided: too small") ) ;

				memcpy((buffer+offset), (_cbuffer + cb_offset), to_read) ; 

				offset += to_read; 
				if (still_to_read >= to_read) { still_to_read -= to_read ; } else { still_to_read = 0 ; }

				finalize_reader(to_read) ;
			}
			catch(exception& e)
			{
				finalize_reader() ;
				throw ; 
			}
		}
		else if (to_read == 0)
		{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				excerr << SimpleDebugSignatureThread << "Circular buffer empty." << endl ;
			ACS_CLASS_END_DEBUG
#endif 

			// The circular buffer is empty. Finalize (abort) the reading and quit immediately
			finalize_reader() ;
			break ; 
		}
		else
		{
			// This should never happen.
			
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				excerr << SimpleDebugSignatureThread << "Reading from circular buffer not allowed." << endl ;
			ACS_CLASS_END_DEBUG
#endif 

			// Reading is not allowed. Operation has not started, so quit immediately
			break ; 
		}
		
	} while (still_to_read > 0) ;


#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Amount read: " << offset << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return offset ; 
}


/************************************************************************************
**                                                                                 **
		ACS_SSL_CircularBuffer::write_circular_buffer
**                                                                                 **
************************************************************************************/
off64_t ACS_SSL_CircularBuffer::write_circular_buffer(const off64_t& to_write_in, const unsigned char* buffer, const off64_t& bufsize) 
{
	/**
		Read the caller's buffer and copy to the circular buffer.
		Returns the amount actually copied. It will be <= the request, or even 0.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called. Amount requested to write: " << to_write_in << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	off64_t still_to_write(to_write_in) ; 
	off64_t written(0) ;  

	do
	{
		ACS_COND_THROW( (still_to_write < 0), exBoundsException("Invalid (negative) amount to write") ) ;

		off64_t cb_offset ;
		const off64_t to_write ( prepare_writer(still_to_write, cb_offset) ) ; // It could be negative if another writer is running at the same time.

#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			excerr << SimpleDebugSignatureThread << "Method called. Amount allowed to write: " << to_write << endl ;
		ACS_CLASS_END_DEBUG
#endif 

		if (to_write > 0)
		{
			try 
			{
				ACS_COND_THROW( ((cb_offset + to_write) > cbuffsize()), exIllegalValueException("Circular Buffer Exceeded") ) ;
				ACS_COND_THROW( ((written + to_write) > bufsize), exIllegalValueException("Invalid buffer provided: too small") ) ;

				memcpy((_cbuffer + cb_offset), (buffer + written), to_write) ; 

				written += to_write; 
				if (still_to_write >= to_write) { still_to_write -= to_write ; } else { still_to_write = 0 ; }

				finalize_writer(to_write) ;
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

			// Writing is not allowed. The writing has not started, so quit immediately
			break ; 
		}
		
	} while (still_to_write > 0) ;
	
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Amount written: " << written << endl ;
	ACS_CLASS_END_DEBUG
#endif 

	return written ; 
}


_ACS_END_NAMESPACE
