/*
 *
 *  Copyright 1995-2011, Advanced Computer Systems , Inc.
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
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/01/20 16:03:13  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.1  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.2  2011/09/02 15:35:01  micmaz
 *  updated the CircularBuffer
 *
 *  Revision 1.1  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *
 *
 *
 */

#ifndef _BytesOutputStream_H_
#define _BytesOutputStream_H_

#include<AbstrBytesStream.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, io) ;

template<typename IoBuffer>
class BytesOutputStream: public AbstrBytesStream<IoBuffer> , public OutputStream {
public:

   explicit BytesOutputStream(IoBuffer& buffer) :
      Stream(),
      AbstrBytesStream<IoBuffer> (buffer),
      OutputStream()
   {
       //no-op
   }

   virtual ~BytesOutputStream() throw() {}

   virtual void flush() {} // PRQA S 2502
   
   virtual void close()    // PRQA S 2502
   {
      this->getBuffer().clear();
      this->setClosed(true) ;
   }

   virtual ssize_t write(char* b, size_t length) // PRQA S 2502
   {
      return this->getBuffer().write(b, length);
   }
private:
    BytesOutputStream() ; // not implemented
    BytesOutputStream(const BytesOutputStream &) ; // not implemented
    BytesOutputStream& operator= (const BytesOutputStream &) ; // not implemented
};

_ACS_END_NESTED_NAMESPACE

#endif /* _BytesOutputStream_H_ */
