// PRQA S 1050 EOF
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
 *  Revision 5.1  2013/07/15 16:29:59  marpas
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.6  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.5  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.4  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.3  2012/01/20 16:05:57  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.2  2012/01/17 16:52:12  micmaz
 *  removed unused methods.
 *
 *  Revision 2.1  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.1  2011/09/07 12:12:34  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *  RESOLVED - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.7  2011/09/06 14:14:18  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.6  2011/08/26 15:52:04  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.5  2011/07/16 09:49:15  micmaz
 *  corrected the stream curl class: now it computes correctly the total amount of read and wrote bytes.
 *
 *  Revision 1.4  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.3  2011/03/02 10:54:23  marpas
 *  iostrems.h name changed into IOStreams.h
 *
 *  Revision 1.2  2011/02/24 11:41:10  micmaz
 *  Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.
 *
 *  Revision 1.1  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *
 *
 *
 */

#ifndef INPUTSTREAM_H_
#define INPUTSTREAM_H_

#include<exException.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs,io) 

class Stream {                                     
public:
    Stream() = default ;

    Stream(Stream const&) = delete ;
    Stream& operator=(Stream  const&) = delete ;

    virtual ~Stream() = default ;

    virtual bool closed() const = 0 ;
    virtual void close() = 0 ;
};

class InputStream: virtual public Stream {                  
public:
    InputStream() = default ;

    InputStream(InputStream const&) = delete ;
    InputStream& operator=(InputStream const &) = delete ;

    ~InputStream() override = default ;            

    virtual size_t read(char* b, size_t len) = 0;

    long long bytesRead() const noexcept { return _bytesRead ; }                    
    void bytesRead(long long r) noexcept { _bytesRead = r ; }
    void addBytesRead(long long r) noexcept { _bytesRead += r ; }
    
private:
    long long _bytesRead = 0 ; 
};

class OutputStream: virtual public Stream {
public:
    OutputStream() = default ;

    OutputStream(OutputStream const&) = delete ;
    OutputStream& operator=(OutputStream const&) = delete ;

    ~OutputStream() override = default ;          

    virtual void flush() { /* nothing to do */ }  
    virtual ssize_t write(char* buffer, size_t length) = 0;

    long long bytesWrote() const noexcept { return _bytesWrote; } 
    void bytesWrote(long long w) noexcept { _bytesWrote = w ; }
    void addBytesWrote(long long w) noexcept { _bytesWrote += w ; }
private:
    long long _bytesWrote = 0 ; 
};


_ACS_END_NESTED_NAMESPACE

#endif /* INPUTSTREAM_H_ */
