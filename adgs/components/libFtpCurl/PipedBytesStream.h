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
 *  Revision 5.1  2013/07/15 16:29:59  marpas
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.0  2013/06/06 18:08:21  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.5  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.4  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.3  2012/01/20 16:07:18  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.2  2012/01/17 16:52:12  micmaz
 *  removed unused methods.
 *
 *  Revision 2.1  2012/01/11 18:46:45  micmaz
 *  IN PROGRESS - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.7  2011/09/02 15:35:01  micmaz
 *  updated the CircularBuffer
 *
 *  Revision 1.6  2011/08/26 15:52:04  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.5  2011/07/16 11:05:45  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.4  2011/07/16 09:49:15  micmaz
 *  corrected the stream curl class: now it computes correctly the total amount of read and wrote bytes.
 *
 *  Revision 1.3  2011/05/12 17:22:13  micmaz
 *  work in progress
 *
 *  Revision 1.2  2011/02/24 11:41:10  micmaz
 *  Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.
 *
 *  Revision 1.1  2011/02/23 14:39:25  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *
 *
 *
 */

#ifndef PIPEDBUFFERSTREAM_H_
#define PIPEDBUFFERSTREAM_H_

#include <BytesInputStream.h>
#include <BytesOutputStream.h>
#include <CircularBuffer.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, io) ;


template<typename IoBuffer, typename IStream = BytesInputStream<IoBuffer>,
        typename OStream = BytesOutputStream<IoBuffer> >
class PipedBytesStream: public OutputStream, public InputStream { // PRQA S 2109

public:
    explicit PipedBytesStream(IoBuffer& buff) :
        Stream(),
        OutputStream(), 
        InputStream(),
        _buff(buff), // PRQA S 2528 3
        _in(_buff), 
        _out(_buff), 
        _outputStreamClosed(false) {}

    virtual ~PipedBytesStream() throw()
    {
        _in.close();
        _out.close();
    }

    //
    //Output Stream interface
    //

    virtual void flush() { _out.flush(); }                  // PRQA S 2131, 2502 2
    virtual void close() { _outputStreamClosed = true; }

    virtual ssize_t write(char* buffer, size_t length)      // PRQA S 2131, 2502
    {
        ssize_t w = _out.write(buffer, length);
        addBytesWrote(w) ;
        return w;
    }

    //
    //InputStream interface
    //

    virtual size_t read(char* buffer, size_t length)        // PRQA S 2131, 2502
    {
        size_t r = _in.read(buffer, length);
        addBytesRead(r) ;
        return r;
    }
    
    virtual bool closed() const                              // PRQA S 2131, 2502
    {
        return _outputStreamClosed && !getInputStreamSize();
    }
    
    virtual size_t getInputStreamSize() const { return _in.available(); }
private:
    PipedBytesStream() ;
    PipedBytesStream(const PipedBytesStream &) ;
    PipedBytesStream& operator= (const PipedBytesStream &) ;
private:
    IoBuffer& _buff;
    IStream _in;
    OStream _out;
    bool _outputStreamClosed;

};

typedef acs::io::CircularBuffer Buffer;

typedef acs::io::PipedBytesStream<Buffer> PipedStream;

_ACS_END_NESTED_NAMESPACE

#endif /* PIPEDBUFFERSTREAM_H_ */
