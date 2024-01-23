// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
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
 *  Revision 5.1  2013/10/22 17:31:49  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.1  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.5  2011/09/29 16:46:53  marpas
 *  rules fixing
 *
 *  Revision 1.4  2011/09/07 12:12:34  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *  RESOLVED - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
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

#ifndef _CircularBuffer_H_
#define _CircularBuffer_H_

#include <ThreadSafe.h>
#include <exException.h>
#include <exMacros.h>
#include <exStream.h>
#include <FtpCurlConstants.h>

#include <iostream>
#include <sstream>


_ACS_BEGIN_NESTED_NAMESPACE(acs,io)

class CircularBuffer : public ThreadSafe { // PRQA S 2153
public:
    static const size_t defaultBufferSize = 32 * acs::curl::Constants::Megabyte ;

    explicit CircularBuffer(size_t size=defaultBufferSize) ;
    CircularBuffer(CircularBuffer const&) ;
    CircularBuffer& operator=(CircularBuffer const&) ;
    virtual ~CircularBuffer() throw () ;

    size_t write(const char*, size_t) ; // PRQA S 2502 2
    size_t read(char*, size_t ) ;
    bool empty() const throw() ;
    size_t size() const throw() ;
    void clear() ;
    void swap(CircularBuffer &) ; // PRQA S 2502

    template<typename Stream> Stream& dump(Stream & os) const // PRQA S 2502
    {
        os << "CircularBuffer{bufferSize: " << _bufferSize 
           << " startIndex: " << _startIndex 
           << " endIndex: " << _endIndex << "}";
        return os ;
    }

#ifdef ACS_TEST
public:
#else
private:
#endif
    size_t _startIndex;
    size_t _endIndex;
    size_t _bufferSize;
    char* _buffer ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CircularBuffer) ;
};


template<typename S>
inline
S& operator<<(S& stream, const CircularBuffer obj) // PRQA S 2072
{
    return obj.dump(stream);
}


_ACS_END_NESTED_NAMESPACE

#endif /* _CircularBuffer_H_ */
