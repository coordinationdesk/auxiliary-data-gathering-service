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

    BytesOutputStream(const BytesOutputStream &) = default ;
    BytesOutputStream& operator= (const BytesOutputStream &) = default ;

   ~BytesOutputStream() override = default ;

   void flush() override { /* no op */ } 
   
   void close() override 
   {
      this->getBuffer().clear();
      this->setClosed(true) ;
   }

   ssize_t write(char* b, size_t length) override 
   {
      return this->getBuffer().write(b, length);
   }

};

_ACS_END_NESTED_NAMESPACE

#endif /* _BytesOutputStream_H_ */
