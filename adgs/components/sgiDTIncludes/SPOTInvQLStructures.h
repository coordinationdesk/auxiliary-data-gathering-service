/*

	Copyright 1995-2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SPOT Interface structures between SPOTPrepareMetadata Processor and QLBrowse $

	$Id$

	$Author$
	
	$Log$
	Revision 1.1  2008/03/19 14:38:06  giusy.sinatra
	First issue
	

*/

#ifndef _SPOTInvQLStructures_H_
#define _SPOTInvQLStructures_H_ 

namespace acs {


	typedef enum {
		PAN = 0, 
		XS, 
		XI,
		MX_SPECTRAL_MODE
	} INV_SPOT_SPECTRAL_MODE;


	struct InvSpotFrame {

		int32_t	Path,
			Row,
			MissLines,
			FirstValidFc,
			LastValidFc,
			SATMinVal,
			SATMaxVal,
			DummySceneFlag,
			BegFormat,
			EndFormat;

		float	SCLat,
			SCLon,
			SCDevLat,
			SCDevLon,
			ULLat,
			ULLon,
			URLat,
			URLon,
			LLLat,
			LLLon,
			LRLat,
			LRLon,
			Incidence,
			Orientation,
			SunAz,
			SunEl;

		double	BegTimeCod,
			EndTimeCod,
			CenterTimeCod;

	};


	struct InvSpotSegment {

		INV_SPOT_SPECTRAL_MODE	Instr_mode;
		int32_t		NPass,			// = 1,
				SatId,			// = 4,
				AscendingFlag;		// = 0;
		int32_t		ReceivStRec,
				NumOfFrames,
				Orbit,
				cycle,
				Mission,
				HRVNum,
				Gain[4],
				MirrorStep,
				MediumId,
				MediumType,
				BegFormat,
				EndFormat,
				FirstValidFc,
				LastValidFc,
				HRVconf,
				TriodeQualityFactor,
				Channel;

		float		LookingAngle,
				ILatMin,
				ILonMin,
				ILatMax,
				ILonMax;

		double		BegTimeCod,
				EndTimeCod,
				BegRecordDate,
				EndRecordDate;

		int32_t		AbsoluteOrbit;

	};


	struct InvSpotVertex {

		double	Lat;
		double	Lon;

	};

}

#endif	//_SPOTInvQLStructures_H_
