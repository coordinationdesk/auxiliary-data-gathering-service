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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1  2011/04/05 17:14:24  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#ifndef CirculationItem_H_
#define CirculationItem_H_

#include<ConsumableCirculation.h>
#include<RegistrableItem.h>

namespace acs {
namespace circulation {

typedef dc::RegistrableItem<ConsumableCirculation, std::string> CirculationItem;

}

}

#endif /* CirculationItem_H_ */
