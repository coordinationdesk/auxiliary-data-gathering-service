// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
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

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CircularBuffer);

CircularBuffer::CircularBuffer(size_t size) :
    _startIndex(0),
    _endIndex(0), 
    _bufferSize(size), 
    _buffer(_bufferSize ? new char[_bufferSize] : 0)  // PRQA S 3054, 3380, 3385
{
}

CircularBuffer::~CircularBuffer() throw () 
{
    delete [] _buffer;
}

CircularBuffer::CircularBuffer(CircularBuffer const& r) :
    _startIndex(r._startIndex), 
    _endIndex(r._endIndex), 
    _bufferSize(r._bufferSize), 
    _buffer(_bufferSize ? new char[_bufferSize] : 0)  // PRQA S 3054, 3380, 3385
{
    ThreadSafe::Lock l(r);
    ::memcpy(_buffer, r._buffer, _bufferSize);
}

CircularBuffer& CircularBuffer::operator=(CircularBuffer const& r) {
    if (this != &r) {
        ThreadSafe::Lock l(*this);
        ThreadSafe::Lock l1(r);
        CircularBuffer tmp(r);
        swap(tmp);
    }
    return *this;
}

void CircularBuffer::swap(CircularBuffer &r) {
    ThreadSafe::Lock l(*this);
    ThreadSafe::Lock l1(r);
    std::swap(_startIndex, r._startIndex);
    std::swap(_endIndex, r._endIndex);
    std::swap(_bufferSize, r._bufferSize);
    std::swap(_buffer, r._buffer);
}

size_t CircularBuffer::write(const char* b, size_t length) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[writeWithLength: " << length << "]");
    if (0 == length) {
        return 0;
    }
    ACS_COND_THROW(b == 0, exIllegalValueException("The source buffer is null")); // PRQA S 3081
    ThreadSafe::Lock l(*this);
    size_t toWrite = std::min(length, _bufferSize - size() - 1); // PRQA S 3084
    size_t newEnd = (_endIndex + toWrite) % _bufferSize; // PRQA S 3084
    if (newEnd >= _endIndex) {
        memcpy(_buffer + _endIndex, b, toWrite); // PRQA S 3084
    } 
    else {
        size_t sizeToend = _bufferSize - _endIndex; // PRQA S 3084
        memcpy(_buffer + _endIndex, b, sizeToend); // PRQA S 3084
        memcpy(_buffer, b + sizeToend, toWrite - sizeToend); // PRQA S 3084
    }
    _endIndex = newEnd;
    if (toWrite < length) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Buffer Full! Wrote only: " << toWrite << " bytes over " << length << ".");
    }
    return toWrite;
}

size_t CircularBuffer::read(char* b, size_t length) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[readWithLength: " << length << "]");
    if (0 == length) {
        return 0;
    }
    ACS_COND_THROW(b == 0, exIllegalValueException("The source buffer is null")); // PRQA S 3081
    ThreadSafe::Lock l(*this);
    size_t toRead = std::min(length, size());
    size_t newStart = (_startIndex + toRead) % _bufferSize; // PRQA S 3084
    size_t sizeToend = 0 ;
    if (newStart >= _startIndex) {
        sizeToend = toRead;
    } 
    else {
        sizeToend = _bufferSize - _startIndex; // PRQA S 3084
    }
    memcpy(b, _buffer + _startIndex, sizeToend); // PRQA S 3084
    memcpy(b + sizeToend, _buffer, toRead - sizeToend); // memcpy will check the size // PRQA S 3084
    _startIndex = newStart;
    return toRead;
}

size_t CircularBuffer::size() const throw() // PRQA S 4020
{
    if (_endIndex >= _startIndex) {
        return _endIndex - _startIndex; // PRQA S 3084
    }
    return _bufferSize - _startIndex + _endIndex; // PRQA S 3084
}


void CircularBuffer::clear() 
{
    ThreadSafe::Lock l(*this);
    _startIndex = 0;
    _endIndex = 0;
}

bool CircularBuffer::empty() const throw() 
{
    return _endIndex == _startIndex;
}

_ACS_END_NESTED_NAMESPACE
