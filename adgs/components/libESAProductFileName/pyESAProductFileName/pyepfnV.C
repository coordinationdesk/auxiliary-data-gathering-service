/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2007/02/27 18:10:00  marqua
	proper versionfile handling
	
	Revision 1.1  2007/02/27 17:04:09  marqua
	proper versionfile handling
	
	Revision 1.1  2007/02/27 16:52:14  marqua
	version files
	
	Revision 2.17  2006/12/21 14:26:04  marpas
	Tag Rev_2_17 created
	
	Revision 2.16  2006/11/24 08:59:13  davcas
	Tag Rev_2_16 created
	
	Revision 2.15  2006/11/22 11:18:21  marpas
	Tag Rev_2_15 created
	
	Revision 2.14  2006/11/03 00:30:30  marpas
	Tag Rev_2_14 created
	
	Revision 2.13  2006/11/03 00:24:14  marpas
	Tag Rev_2_13 created
	
	Revision 2.12  2006/10/16 10:52:13  marpas
	Tag Rev_2_12 created
	
	Revision 2.11  2006/09/19 10:53:15  marpas
	Tag Rev_2_11 created
	
	Revision 2.10  2006/09/19 10:37:46  crivig
	Tag Rev_2_10 created
	
	Revision 2.9  2006/08/23 15:44:00  paoscu
	Tag Rev_2_9 created
	
	Revision 2.8  2006/07/13 13:31:00  marpas
	Tag Rev_2_8 created
	
	Revision 2.7  2006/07/13 12:40:37  marpas
	Tag Rev_2_7 created
	
	Revision 2.6  2006/07/13 09:36:04  marpas
	Tag Rev_2_6 created
	
	Revision 2.5  2006/07/12 09:40:31  marpas
	Tag Rev_2_5 created
	
	Revision 2.4  2006/07/03 14:36:44  marpas
	Tag Rev_2_4 created
	
	Revision 2.3  2006/06/28 16:59:12  marpas
	Tag Rev_2_3 created
	
	Revision 2.2  2006/06/20 09:16:41  marpas
	Tag Rev_2_2 created
	
	Revision 2.1  2006/02/28 08:41:07  marpas
	Tag Rev_2_1 created
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.31  2005/11/17 18:12:07  marpas
	Tag Rev_1_31 created
	
	Revision 1.30  2005/11/15 11:31:03  marpas
	Tag Rev_1_30 created
	
	Revision 1.29  2005/11/10 11:16:19  danalt
	Tag Rev_1_29 created
	
	Revision 1.28  2005/11/08 19:40:50  danalt
	Tag Rev_1_28 created
	
	Revision 1.27  2005/11/07 13:28:44  enrcar
	Tag Rev_1_27 created
	
	Revision 1.26  2005/10/22 17:02:19  marpas
	Tag Rev_1_26 created
	
	Revision 1.25  2005/09/29 15:13:11  marpas
	Tag Rev_1_25 created
	
	Revision 1.24  2005/09/29 09:28:55  marpas
	Tag Rev_1_24 created
	
	Revision 1.23  2005/09/23 16:43:47  marpas
	Tag Rev_1_23 created
	
	Revision 1.22  2005/07/01 09:05:18  marfav
	Tag Rev_1_22 created
	
	Revision 1.21  2005/03/10 13:19:45  marpas
	Tag Rev_1_21 created
	
	Revision 1.20  2005/03/10 13:02:13  marpas
	Tag Rev_1_20 created
	
	Revision 1.19  2005/03/03 14:11:06  marpas
	Tag Rev_1_19 created
	
	Revision 1.18  2005/02/07 09:39:48  marpas
	Tag Rev_1_18 created
	
	Revision 1.17  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.16  2004/11/26 11:48:10  marpas
	Tag Rev_1_16 created
	
	Revision 1.15  2004/09/08 13:55:42  marpas
	Tag Rev_1_15 created
	
	Revision 1.14  2004/08/30 09:37:56  marpas
	Tag Rev_1_14 created
	
	Revision 1.13  2004/08/30 09:36:17  marpas
	Tag Rev_1_13 created
	
	Revision 1.12  2004/08/30 09:14:13  marpas
	Tag Rev_1_12 created
	
	Revision 1.11  2004/08/30 08:27:31  marpas
	Tag Rev_1_11 created
	
	Revision 1.10  2004/08/27 15:43:48  marpas
	Tag Rev_1_10 created
	
	Revision 1.9  2004/05/17 08:57:40  marpas
	Tag Rev_1_9 created
	
	Revision 1.8  2003/08/29 11:44:27  paoscu
	Tag Rev_1_8 created
	
	Revision 1.7  2003/03/18 13:19:27  paoscu
	Tag Rev_1_7 created
	
	Revision 1.6  2003/02/19 16:54:12  marpas
	Tag Rev_1_6 created
	
	Revision 1.5  2003/02/19 16:41:41  marpas
	Tag Rev_1_5 created
	
	Revision 1.4  2003/02/11 16:51:11  paoscu
	Tag Rev_1_4 created
	
	Revision 1.3  2003/01/31 14:58:13  paoscu
	Tag Rev_1_3 created
	
	Revision 1.2  2002/12/16 12:19:01  paoscu
	Tag Rev_1_2 created
	
	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
	Import libDateTime
	

*/

#include <pyepfnV.h>

using namespace acs ;

const char * pyepfnV::_name = "@(#) $Prod: A.C.S. pyESAProductFileName $$Revision$$Date$" ;
const char* pyepfnV::_tag = "$Name$" ;


pyepfnV::pyepfnV() {}
pyepfnV::~pyepfnV() {}

