/*

	Copyright 1995-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$
	
	$Log$
	Revision 1.1  2007/09/03 10:21:12  aleber
	moved from libMDPSStationManagement
	
	Revision 1.2  2006/11/29 18:05:37  lucio.pulvirenti
	VDS: aligned to smFtdcf class modified by LP
	
	Revision 1.1.1.1  2006/10/27 09:21:47  valspa
	VDS: first porting of the MDPS SGI Station Management to new LINUX environment

*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _smConstants_h_
#define _smConstants_h_

 
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char const * smConstants="@(#) A.C.S. SATELLITE Library: smConstants.h 1.1 98/07/27";


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions.											! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef NULL
#define NULL 0
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions: min free space for productions.							! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				NUM_OF_SENSORS				    4	/* number of sensors   */
#define				BANDS_NUMBER				   16	/* oversized number of sensor bands */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   End of code redefinition control.								! */
/* +----------------------------------------------------------------------------------------------------+ */
#endif
