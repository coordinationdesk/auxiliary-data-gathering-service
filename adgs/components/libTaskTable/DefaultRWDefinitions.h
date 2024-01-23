// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2016/09/16 11:52:37  lucio.pulvirenti
	PDSEV-41: both mission and satellite filter keys defined.
	
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.5  2011/09/16 14:02:20  marfav
	Adding Stream tag support to alternatives
	
	Revision 2.4  2009/11/20 17:26:40  marfav
	Added support for Alternative_Filter_Tag optional field
	
	Revision 2.3  2008/10/20 13:19:25  marfav
	added support to task cmdline parameters
	
	Revision 2.2  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/

#ifndef DEFAULT_READWRITE_DEFINITIONS
#define DEFAULT_READWRITE_DEFINITIONS
#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)


class DefaultReadWriteTaskTableDefs
{

/**
 * This is a set of definition used by the 
 * Default WRITER and READER for the Task Table xml structure
 * Every definition represents a TAG name to be used inside the xml file 
 */ 

public:
	// PRQA S 2100 L1
	static const char *_diskSpaceKey;
	static const char *_maxTimeKey;
	static const char *_processorNameKey;
	static const char *_processorVersionKey;
	static const char *_testKey;
	static const char *_missionKey;
	static const char *_satelliteKey;
	static const char *_alternativeFilterTagKey;

	static const char *_pconfKey;
	static const char *_cfgListKey;
	static const char *_defaultKey;
	static const char *_defCfgKey;
	static const char *_cfgCountKey;
	static const char *_cfgArrayKey;
	static const char *_cfgKey;
	static const char *_cfgVersionKey;
	static const char *_cfgPathKey;

	static const char *_sequenceCountKey;
	static const char *_sequenceArrayKey;
	static const char *_sequenceKey;
	static const char *_sequenceDetachedKey;
	static const char *_sequenceKillSignalKey;
	static const char *_sequenceQuitSignalKey;
	static const char *_sequenceWeightKey;
	
	static const char *_poolCountKey;
	static const char *_poolArrayKey;
	static const char *_poolKey;
	static const char *_poolDetachedKey;
	static const char *_poolKillSignalKey;
	static const char *_poolQuitSignalKey;
	static const char *_poolWeightKey;

	static const char *_atExitCountKey;
	static const char *_atExitArrayKey;
	static const char *_atExitKey;
	static const char *_atExitStatusKey;
	static const char *_atExitOverrideKey;
	
	static const char *_cfgSpaceArrayKey;
	static const char *_cfgSpaceCountKey;
	static const char *_cfgSpaceNameKey;
	
	//dynamic parameters keys
	static const char *_dynParamListKey;
	static const char *_dynParamArrayKey;
	static const char *_dynParamCountKey;
	static const char *_dynParamKey;
	static const char *_dynParamNameKey;
	static const char *_dynParamTypeKey;
	static const char *_dynParamDefaultKey;
	
	// all task related and nested key;
	// 		_poolArrayKey;
	static const char *_taskCountKey;
	static const char *_taskArrayKey;
	static const char *_taskKey;
	static const char *_taskNameKey;
	static const char *_taskVersionKey;
	static const char *_taskPathKey;
	static const char *_taskCriticalKey;
	static const char *_taskCrLevelKey;
	static const char *_taskWeightKey;
	static const char *_taskDetachedKey;
	static const char *_taskKillSignalKey;
	static const char *_taskQuitSignalKey;
	static const char *_taskPosixExitCodeKey;
	
    // all task parameter related and nested key;
    static const char *_paramCountKey;
    static const char *_paramArrayKey;
    static const char *_paramKey;
    static const char *_paramMandatoryKey;
    static const char *_paramMandatoryTag;
    static const char *_paramNameKey;
    static const char *_paramValueKey;

    // all input related and nested key;
	// 		_poolArrayKey;
	static const char *_inputCountKey;
	static const char *_inputArrayKey;
	static const char *_inputKey;
	static const char *_inputModeKey;
	static const char *_inputMandatoryKey;
	static const char *_inputIdKey;
	static const char *_inputRefKey;
	
	// all alternative related and nested key;
	// 		_poolArrayKey;
	static const char *_alternCountKey;
	static const char *_alternArrayKey;
	static const char *_alternKey;
	static const char *_alternModeKey;
	static const char *_alternT0Key;
	static const char *_alternT1Key;
	static const char *_alternTypeKey;
	static const char *_alternClassKey;
	static const char *_alternOrderKey;
	static const char *_alternOriginKey;
	static const char *_alternFileNameTypeKey;
    static const char *_alternStreamKey;
    static const char *_alternFilterBySatelliteKey;
    static const char *_alternFilterByMissionKey;
    static const char *_alternActivationTimeKey;
	
	// all output related and nested key;
	// 		_poolArrayKey;
	static const char *_outputCountKey;
	static const char *_outputArrayKey;
	static const char *_outputKey;
	static const char *_outputDestinationKey;
	static const char *_outputMandatoryKey;
	static const char *_outputFileTypeKey;
	static const char *_outputFileTypeKey2;
	static const char *_outputFileNameTypeKey;
	
	
	// all breakpoint related and nested key;
	// 		_poolArrayKey;
	static const char *_brkptCountKey;
	static const char *_brkptArrayKey;
	static const char *_brkptKey;
	
	// Default values
	static const char *_missionDefaultValue;
	static const char *_satelliteDefaultValue;
    // PRQA L:L1

}; // class DefaultReadWriteTaskTableDefs

_ACS_END_NAMESPACE


#endif //DEFAULT_READWRITE_DEFINITIONS





