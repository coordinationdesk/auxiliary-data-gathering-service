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
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.2  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.1  2012/01/20 16:03:14  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.4  2011/09/02 15:35:01  micmaz
 *  updated the CircularBuffer
 *
 *  Revision 1.3  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
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

#ifndef _BytesInputStream_H_
#define _BytesInputStream_H_

#include <AbstrBytesStream.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, io) ;

template<typename IoBuffer>
class BytesInputStream: public AbstrBytesStream<IoBuffer>, public InputStream 
{
public:

    explicit BytesInputStream(IoBuffer& buffer) :
        Stream(),
        AbstrBytesStream<IoBuffer>(buffer), 
        InputStream() 
    {
    }

    BytesInputStream(const BytesInputStream &) = delete ;
    BytesInputStream & operator= (const BytesInputStream &) = delete ;

    ~BytesInputStream() override = default ;
    
    void close() override  
    {
        this->getBuffer().clear();
        this->setClosed(true) ;
    }
    
    size_t read(char* b, size_t len) override 
    {
        return this->getBuffer().read(b, len);
    }

    virtual void clear()
    {
        this->getBuffer().clear();
        this->setClosed(false) ;
    }
    
};

_ACS_END_NESTED_NAMESPACE

#endif /* _BytesInputStream_H_ */
