// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	
	Revision 5.0  2013/07/07 18:24:06  marpas
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

#include <DefaultRWDefinitions.h>


_ACS_BEGIN_NAMESPACE(acs)

const char * DefaultReadWriteTaskTableDefs::_diskSpaceKey            = "Min_Disk_Space";  // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_maxTimeKey              = "Max_Time";        // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_processorNameKey        = "Processor_Name";	 // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_processorVersionKey     = "Version";         // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_testKey                 = "Test";            // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_missionKey              = "Mission";         // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_satelliteKey            = "Satellite";       // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_alternativeFilterTagKey = "Alternative_Filter_Tag"; // to be used with no namespace

const char * DefaultReadWriteTaskTableDefs::_pconfKey       = "Private_Config";			    // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_cfgListKey     = "List_of_Cfg_Files";		    // to be used inside _pcofKey
const char * DefaultReadWriteTaskTableDefs::_defaultKey     = "Default";				    // to be used inside _pcofKey
const char * DefaultReadWriteTaskTableDefs::_defCfgKey      = "Private_Config.Default";			    // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_cfgCountKey    = "Private_Config.List_of_Cfg_Files.count.value"; // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_cfgArrayKey    = "Private_Config.List_of_Cfg_Files";		    // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_cfgKey         = "Cfg_File";		    	    	    // to be used inside namespace _cfgArrayKey
const char * DefaultReadWriteTaskTableDefs::_cfgVersionKey  = "Version";				    // to be used inside namespace _cfgArrayKey._cfgKey[i]
const char * DefaultReadWriteTaskTableDefs::_cfgPathKey     = "File_Name";			    // to be used inside namespace _cfgArrayKey._cfgKey[i]

//dynamic parameters keys
const char * DefaultReadWriteTaskTableDefs::_dynParamListKey    = "List_of_Dyn_ProcParams";
const char * DefaultReadWriteTaskTableDefs::_dynParamArrayKey   = "List_of_Dyn_ProcParams.Dyn_ProcParam";
const char * DefaultReadWriteTaskTableDefs::_dynParamCountKey   = "List_of_Dyn_ProcParams.count.value";
const char * DefaultReadWriteTaskTableDefs::_dynParamKey        = "Dyn_ProcParam";
const char * DefaultReadWriteTaskTableDefs::_dynParamNameKey    = "Param_Name";
const char * DefaultReadWriteTaskTableDefs::_dynParamTypeKey    = "Param_Type";
const char * DefaultReadWriteTaskTableDefs::_dynParamDefaultKey = "Param_Default";

const char * DefaultReadWriteTaskTableDefs::_sequenceCountKey       = "List_of_Sequences.count.value";	// to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_sequenceArrayKey       = "List_of_Sequences";		// to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_sequenceKey            = "Sequence";				// to be used inside namespace _sequenceArrayKey
const char * DefaultReadWriteTaskTableDefs::_sequenceDetachedKey    = "Detached";			// to be used inside namespace _sequenceArrayKey._sequenceKey[i]
const char * DefaultReadWriteTaskTableDefs::_sequenceKillSignalKey  = "Killing_Signal";	// to be used inside namespace _sequenceArrayKey._sequenceKey[i]
const char * DefaultReadWriteTaskTableDefs::_sequenceQuitSignalKey  = "Quitting_Signal"; // to be used inside namespace _sequenceArrayKey._sequenceKey[i]
const char * DefaultReadWriteTaskTableDefs::_sequenceWeightKey      = "Sequence_Weight";     // to be used inside namespace _sequenceArrayKey._sequenceKey[i]

const char * DefaultReadWriteTaskTableDefs::_poolCountKey       = "List_of_Pools.count.value";	// to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_poolArrayKey       = "List_of_Pools";		// to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_poolKey            = "Pool";				// to be used inside namespace _poolArrayKey
const char * DefaultReadWriteTaskTableDefs::_poolDetachedKey    = "Detached";			// to be used inside namespace _poolArrayKey._poolKey[i]
const char * DefaultReadWriteTaskTableDefs::_poolKillSignalKey  = "Killing_Signal";	// to be used inside namespace _poolArrayKey._poolKey[i]
const char * DefaultReadWriteTaskTableDefs::_poolQuitSignalKey  = "Quitting_Signal"; // to be used inside namespace _poolArrayKey._poolKey[i]
const char * DefaultReadWriteTaskTableDefs::_poolWeightKey      = "Pool_Weight";     // to be used inside namespace _poolArrayKey._poolKey[i]

const char * DefaultReadWriteTaskTableDefs::_atExitCountKey    = "List_of_Atexit_Sequences.count.value"; // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_atExitArrayKey    = "List_of_Atexit_Sequences"; // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_atExitKey         = "Atexit_Sequence"; // to be used inside namespace _atExitArrayKey
const char * DefaultReadWriteTaskTableDefs::_atExitStatusKey   = "On_Exit_Status"; // to be used inside namespace _atExitArrayKey._atExitKey[i]
const char * DefaultReadWriteTaskTableDefs::_atExitOverrideKey = "Override_Status"; // to be used inside namespace _atExitArrayKey._atExitKey[i]

const char * DefaultReadWriteTaskTableDefs::_cfgSpaceArrayKey = "List_of_Config_Spaces"; // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_cfgSpaceCountKey = "List_of_Config_Spaces.count.value"; // to be used with no namespace
const char * DefaultReadWriteTaskTableDefs::_cfgSpaceNameKey = "Config_Space"; // to be used inside namespace _cfgSpaceArrayKey

// all task related and nested keys to be used inside namespace
// 		_poolArrayKey._poolKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskCountKey     = "List_of_Tasks.count.value";	// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_taskArrayKey     = "List_of_Tasks";		// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_taskKey          = "Task";				// to be used inside _taskArrayKey
const char * DefaultReadWriteTaskTableDefs::_taskNameKey      = "Name";				// to be used inside _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskVersionKey   = "Version";			// to be used inside _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskPathKey      = "File_Name";			// to be used inside _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskCriticalKey  = "Critical";			// to be used inside _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskCrLevelKey   = "Criticality_Level";	// to be used inside _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskWeightKey  	 = "Task_Weight";       // to be used inside _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskDetachedKey    = "Detached";			// to be used inside namespace _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskKillSignalKey  = "Killing_Signal";	// to be used inside namespace _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskQuitSignalKey  = "Quitting_Signal"; // to be used inside namespace _taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_taskPosixExitCodeKey  = "Posix_ExitCode"; // to be used inside namespace _taskArrayKey._taskKey[i]



// all task parameter related and nested key to be used inside namespace
// .......taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_paramCountKey     = "List_of_Parameters.count.value";
const char * DefaultReadWriteTaskTableDefs::_paramArrayKey     = "List_of_Parameters";
const char * DefaultReadWriteTaskTableDefs::_paramKey          = "Parameter"; // to be used inside _paramArrayKey
const char * DefaultReadWriteTaskTableDefs::_paramMandatoryKey = "Parameter.mandatory.value"; // to be used inside _paramArrayKey
const char * DefaultReadWriteTaskTableDefs::_paramMandatoryTag = "mandatory"; // to be used writing _paramArrayKey._paramKey[i]
const char * DefaultReadWriteTaskTableDefs::_paramNameKey      = "Name"; // to be used inside _paramArrayKey._paramKey[i]
const char * DefaultReadWriteTaskTableDefs::_paramValueKey     = "Value"; // to be used inside _paramArrayKey._paramKey[i]


// all input related and nested keys to be used inside namespace 
// 		_poolArrayKey._poolKey[i]._taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_inputCountKey      = "List_of_Inputs.count.value";	// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_inputArrayKey      = "List_of_Inputs";		// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_inputKey           = "Input";			// to be used inside _inputArrayKey
const char * DefaultReadWriteTaskTableDefs::_inputModeKey       = "Mode";			// to be used inside _inputArrayKey._inputKey[i]
const char * DefaultReadWriteTaskTableDefs::_inputMandatoryKey  = "Mandatory";			// to be used inside _inputArrayKey._inputKey[i]
const char * DefaultReadWriteTaskTableDefs::_inputIdKey  = "id.value";				// to be used inside _inputArrayKey._inputKey[i]
const char * DefaultReadWriteTaskTableDefs::_inputRefKey  = "ref.value";			// to be used inside _inputArrayKey._inputKey[i]

// all alternative related and nested keys to be used inside namespace 
// 		_poolArrayKey._poolKey[i]._taskArrayKey._taskKey[i]._inputArrayKey._inputKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternCountKey = "List_of_Alternatives.count.value";	// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_alternArrayKey = "List_of_Alternatives";			// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_alternKey      = "Alternative";				// to be used inside _alternArrayKey
const char * DefaultReadWriteTaskTableDefs::_alternModeKey  = "Retrieval_Mode";				// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternT0Key    = "T0";					// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternT1Key    = "T1";					// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternTypeKey  = "File_Type";				// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternClassKey  = "File_Class";				// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternOrderKey = "Order";					// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternOriginKey= "Origin";					// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternFileNameTypeKey= "File_Name_Type";			// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternStreamKey= "Stream";					// to be used inside _alternArrayKey._alternKey[i]
const char * DefaultReadWriteTaskTableDefs::_alternFilterBySatelliteKey = "FilterBySatellite";
const char * DefaultReadWriteTaskTableDefs::_alternFilterByMissionKey = "FilterByMission";
const char * DefaultReadWriteTaskTableDefs::_alternActivationTimeKey = "AlternativeActivationTime";

// all output related and nested keys to be used inside namespace 
// 		_poolArrayKey._poolKey[i]._taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_outputCountKey = "List_of_Outputs.count.value";	// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_outputArrayKey = "List_of_Outputs";			// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_outputKey = "Output";				// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_outputDestinationKey = "Destination";		// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_outputMandatoryKey = "Mandatory";			// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_outputFileTypeKey= "File_Type";			// to be used inside _outputArrayKey._outputKey[i]
const char * DefaultReadWriteTaskTableDefs::_outputFileTypeKey2= "Type";			// alternative to _outputFileTypeKey according to Gen ICD 1.8
const char * DefaultReadWriteTaskTableDefs::_outputFileNameTypeKey= "File_Name_Type";		// to be used inside _outputArrayKey._outputKey[i]


// all breakpoint related and nested keys to be used inside namespace 
// 		_poolArrayKey._poolKey[i]._taskArrayKey._taskKey[i]
const char * DefaultReadWriteTaskTableDefs::_brkptCountKey = "List_of_Breakpoints.count.value";	// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_brkptArrayKey = "List_of_Breakpoints";		// no additional namespace
const char * DefaultReadWriteTaskTableDefs::_brkptKey      = "Breakpoint";			// to be used inside _brkptArrayKey

// Default values
const char * DefaultReadWriteTaskTableDefs::_missionDefaultValue = "CRYOSAT";
const char * DefaultReadWriteTaskTableDefs::_satelliteDefaultValue = "CRYOSAT";



_ACS_END_NAMESPACE

