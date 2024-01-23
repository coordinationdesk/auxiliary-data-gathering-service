/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel1 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$

*/

#ifndef _S1AuxReformatting_H_
#define _S1AuxReformatting_H_ 

#if defined (HAS_MISSION_INCLUDES)
	#include <ProjectStandaloneAppIncludes>
#else
	#include <StandaloneApp.h>
#endif

#include <exMsgFmtFactory.h>

#include <JobOrder.h>
#include <S1L2AuxReformatter.h>

#include <vector>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)
  
#if   defined (HAS_MISSION_INCLUDES)
	class S1AuxReformatting : public PROJECT_STANDALONE_APP
#else
	class S1AuxReformatting : public StandaloneApp
#endif
	{
	public:
			
		/*! class S1AuxReformattingException declaration */
		exDECLARE_EXCEPTION(S1AuxReformattingException,exException) ;
		
		/**
		 * \brief  Class constructor 
		 *    
		 */
		S1AuxReformatting(const std::string& appName, const std::string& subSys);
		
		/**
		 * \brief  Class destructor 
		 *    
		 */    	
		~S1AuxReformatting() throw() ;
		
		virtual int main(int argc, char const * const * argv, char const * const * env);
		
		

	private:

		// copy constructor and operator= defined but not implemented
		S1AuxReformatting(const S1AuxReformatting & ); 
		S1AuxReformatting &operator=(const S1AuxReformatting &);
			
		//methods
		
		void printUsage(int ,  char const * const *) const;
		void execute(const std::string & jobOrderFileName);
		void loadConfigResources(const std::string& path);
		void getFileList(const std::string &joFilename, std::vector<std::string>& in, std::vector<std::string>& out);
		
		void createInventoryMetadata( const JobOrder& jo, const std::string& outFiletype, const std::string& safeFilename ) const;

//        	void catchSigInt(int) throw() ;
//        	void catchSigTerm(int) throw() ;
				
		//attributes

		rsResourceSet 				_privateConf;
		
		ACS_CLASS_DECLARE_DEBUG_LEVEL(S1AuxReformatting)

	};

_ACS_END_NAMESPACE


#endif //_S1AuxReformatting_H_
