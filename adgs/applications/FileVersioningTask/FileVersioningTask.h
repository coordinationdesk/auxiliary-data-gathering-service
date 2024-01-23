// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 1.1.1.1  2014/06/20 15:22:31  lucio.pulvirenti
	Import FileVersioningTask
	
        
*/

#ifndef _FileVersioningTask_H_
#define _FileVersioningTask_H_

#include <ProjectAppIncludes>


#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class Metadata;
class dbConnection;

class FileVersioningTask: public PROJECT_APP { 
public:


  /** FileVersioningTask Exceptions */
  exDECLARE_EXCEPTION(exFileVersioningTaskException, exException) ; // Base FileVersioningTask Exception.


  /** Class constructor */
  FileVersioningTask(const std::string& subsys, const std::string& app);
  /** Destructor */
  virtual ~FileVersioningTask() throw() ;
  
private:  
	FileVersioningTask();	
	/** Copy Constructor */
	FileVersioningTask(const FileVersioningTask & );
	/** Operator = */
	FileVersioningTask &operator=(const FileVersioningTask &);

	//methods
	virtual int usage(const std::string &exename) const; 

	void readConfiguration();
	void execute(const std::string &) const ;
	void doVersioning(Metadata& metadata, const std::string & metadataFilename, dbConnection &) const ;
	
public:
  /** Entry point */
  	virtual int main(int argc, char const * const * argv, char const * const * env);


private:
	
	int _confSetw; 
	char _confSetFill;

	static const std::string _confSpaceName;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FileVersioningTask)

};

_ACS_END_NAMESPACE

#endif //_FileVersioningTask_H_

