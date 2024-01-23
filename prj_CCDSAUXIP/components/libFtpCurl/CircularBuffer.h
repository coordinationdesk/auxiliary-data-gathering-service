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

class CircularBuffer {
public:
    static const size_t defaultBufferSize = 32 * acs::curl::Constants::Megabyte ;

    explicit CircularBuffer(size_t size=defaultBufferSize) ;
    CircularBuffer(CircularBuffer const&) = default ;
    CircularBuffer& operator=(CircularBuffer const&) = default ;
    virtual ~CircularBuffer() = default ;

    size_t write(const char*, size_t) ;
    size_t read(char*, size_t ) ;
    bool empty() const noexcept ;
    size_t size() const noexcept ;
    void clear() ;

    template<typename Stream> Stream& dump(Stream & os) const
    {
        os << "CircularBuffer{bufferSize: " << _buffer.size() 
           << " startIndex: " << _startIndex 
           << " endIndex: " << _endIndex << "}";
        return os ;
    }

#ifdef ACS_TEST
public:
#else
private:
#endif
    size_t _startIndex = 0 ;
    size_t _endIndex = 0 ;
    std::vector<char> _buffer ;
    Mutex _mutex = {} ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CircularBuffer) ;
};


template<typename S>
inline
S& operator<<(S& stream, const CircularBuffer &obj)
{
    return obj.dump(stream);
}


_ACS_END_NESTED_NAMESPACE

#endif /* _CircularBuffer_H_ */
