/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/02/04 15:09:25  chicas
	 Configurable Decompress app from conf space
	
	Revision 1.1.1.1  2013/01/23 16:33:10  chicas
	Imported sources
	
	
        
*/

#ifndef _TTDecompress_H_
#define _TTDecompress_H_

#include <ProjectAppIncludes>


#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)
    
    /**
     * \brief
     *  TaskInPreInventory Main application 
     **/
     
   class TTDecompress: public PROJECT_APP { 
   public:
        
      /** TTDecompress Exceptions */
      exDECLARE_EXCEPTION(exTTDecompressException, exException) ; // Base TTDecompress Exception.

   
      /** Class constructor */
      TTDecompress(const std::string& subsys, const std::string& app);
      /** Destructor */
      virtual ~TTDecompress() throw() ;

   private:  // copy constructor and operator= defined but not implemented
      /** Copy Constructor */
      TTDecompress(const TTDecompress & );
      /** Operator = */
      TTDecompress &operator=(const TTDecompress &);

      //methods
		virtual int usage(const std::string &exename) const; 


		void execute(const std::string & jobOrderFileName);
        void unpackFile(std::string ) const;

        void readConfiguration();

   public:
      /** Entry point */
      virtual int main(int argc, char const * const * argv, char const * const * env);

   private:
      std::string _decompressApp;
      
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TTDecompress)
     
   };

_ACS_END_NAMESPACE

#endif //_TTDecompress_H_

