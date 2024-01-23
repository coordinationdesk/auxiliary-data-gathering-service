/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.4  2014/06/04 17:29:20  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.3  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/23 17:28:10  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:49  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2012/11/27 13:26:31  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.2  2012/01/20 16:03:31  micmaz
 *  modified logs
 *
 *  Revision 2.1  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.3  2011/09/07 09:34:13  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.2  2011/09/02 15:35:01  micmaz
 *  updated the CircularBuffer
 *
 *
 *
 */

#include <CircularBuffer.h>
#include <Filterables.h>

#include <algorithm>
#include <cstdio>
#include <cstring>

_ACS_BEGIN_NESTED_NAMESPACE(acs,io)

ACS_CLASS_DEFINE_DEBUG_LEVEL(CircularBuffer);

CircularBuffer::CircularBuffer(size_t size) :
    _buffer(size) 
{
}

size_t CircularBuffer::write(const char* b, size_t length)
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[writeWithLength: " << length << "]");
    if (0 == length) {
        return 0;
    }
    ACS_COND_THROW(b == 0, exIllegalValueException("The source buffer is null"));
    Mutex::Lock l(_mutex);
    size_t toWrite = std::min(length, _buffer.size() - size() - 1);
    size_t newEnd = (_endIndex + toWrite) % _buffer.size();
    if (newEnd >= _endIndex) {
        memcpy(&_buffer[0] + _endIndex, b, toWrite);
    } 
    else {
        size_t sizeToend = _buffer.size() - _endIndex;
        memcpy(&_buffer[0] + _endIndex, b, sizeToend);
        memcpy(&_buffer[0], b + sizeToend, toWrite - sizeToend);
    }
    _endIndex = newEnd;
    if (toWrite < length) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Buffer Full! Wrote only: " << toWrite << " bytes over " << length << ".");
    }
    return toWrite;
}

size_t CircularBuffer::read(char* b, size_t length)
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[readWithLength: " << length << "]");
    if (0 == length) {
        return 0;
    }
    ACS_COND_THROW(b == 0, exIllegalValueException("The source buffer is null"));
    Mutex::Lock l(_mutex);
    size_t toRead = std::min(length, size());
    size_t newStart = (_startIndex + toRead) % _buffer.size();
    size_t sizeToend = 0 ;
    if (newStart >= _startIndex) {
        sizeToend = toRead;
    } 
    else {
        sizeToend = _buffer.size() - _startIndex;
    }
    memcpy(b, &_buffer[0] + _startIndex, sizeToend);
    memcpy(b + sizeToend, &_buffer[0], toRead - sizeToend); // memcpy will check the size
    _startIndex = newStart;
    return toRead;
}

size_t CircularBuffer::size() const noexcept
{
    Mutex::Lock l(_mutex);
    if (_endIndex >= _startIndex) {
        return _endIndex - _startIndex;
    }
    return _buffer.size() - _startIndex + _endIndex;
}


void CircularBuffer::clear() 
{
    Mutex::Lock l(_mutex);
    _startIndex = 0;
    _endIndex = 0;
}

bool CircularBuffer::empty() const noexcept 
{
    Mutex::Lock l(_mutex);
    return _endIndex == _startIndex;
}

_ACS_END_NESTED_NAMESPACE
