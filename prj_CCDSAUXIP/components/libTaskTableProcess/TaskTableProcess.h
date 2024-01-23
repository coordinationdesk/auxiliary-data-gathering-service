// PRQA S 1050 EOF
/*
	Copyright 2012-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	$Prod: $

	$Id$

	$Author $

	$Log$
	Revision 5.17  2016/03/23 15:02:56  marfav
	S3PDGS-2809
	added the possibility to generate the log file in the default log repository
	
	Revision 5.16  2016/03/14 14:22:11  nicvac
	OPMAN-163: Task table refresh at start.
	
	Revision 5.15  2016/03/14 10:52:21  marpas
	log improved to check addresses with blanks
	
	Revision 5.14  2016/03/10 14:13:27  nicvac
	OPMAN-158: satallite, mission, sensor on import rules.
	
	Revision 5.13  2015/06/17 13:36:01  marpas
	fixing potential raise (unprotected variable) condition
	coding best practice applied
	
	Revision 5.12  2015/05/25 15:46:02  lucio.pulvirenti
	S3PDGS-2382" read only product flag and method added to IF. If flag set, product permission set as read-only after moving to production dir.
	
	Revision 5.11  2015/03/18 08:46:46  lucio.pulvirenti
	S2PDGS-1129: _inputFilefullpath attribute added to interface along with the proper get method.
	
	Revision 5.10  2015/03/03 14:22:32  lucio.pulvirenti
	S2PDGS-1115: mutex to read configuration introduced. Use of string variables improved in createWorkingDir method.
	
	Revision 5.9  2014/05/28 17:50:14  marpas
	minor changes
	
	Revision 5.8  2014/03/21 14:45:19  chicas
	Messages improved
	
	Revision 5.7  2014/01/24 11:53:36  lucio.pulvirenti
	S2PDGS-289: Method to set production dir added: if production dir not set, built as before. File guard list removed since no more useful.
	
	Revision 5.6  2014/01/13 17:15:37  lucio.pulvirenti
	S2PDGS-289: renaming substituted with hard link for files from working dir to production dir and a file guard on source path is set. It allows to detach file guard if an error occurs
	so that circulation destination path is not removed and circulation must not be repeated. If all OK, file guard automatically remove working dir path in DTOR.
	
	Revision 5.5  2013/12/17 15:42:14  marpas
	no mutex when downloading - speed ups them
	downloading a too old tt is done on temporary file and then moved into the final destination.
	
	Revision 5.4  2013/11/06 11:56:42  marpas
	identifier is now a dbPersistent::IdType
	
	Revision 5.3  2013/07/08 14:33:10  chicas
	Bug fixed: using File::lock when downloading/reading TaskTable
	
	Revision 5.2  2013/07/07 19:16:10  marpas
	k table can be downloaded in a temporary filename
	adoption of libException 5.x standards
	qa and compilation warnings removed
	
	Revision 5.1  2013/06/19 13:00:08  chicas
	bug fixed adding _ttMutex
	
	Revision 5.0  2013/06/19 08:41:08  chicas
	*** empty log message ***
	
	Revision 1.18  2013/06/14 13:20:22  chicas
	new debugging and logging macros adopted
	
	Revision 1.17  2013/06/12 10:35:33  nicvac
	S2PDGS-298: Notifications on success.
	
	Revision 1.16  2013/04/29 08:54:54  chicas
	 useless include rmoeved; setExt added
	
	Revision 1.15  2013/04/03 13:44:23  marfav
	Rolling back to previous thread::run()
	
	Revision 1.14  2013/04/03 12:44:10  marfav
	thread::run takes a void * as argument
	
	Revision 1.13  2013/03/07 08:46:09  chicas
	setMainConfSpace instead of setApplicationName
	
	Revision 1.12  2013/02/28 14:09:35  chicas
	Added identifier and downloadTaskTable static functions
	
	Revision 1.11  2013/02/25 13:33:04  chicas
	comments changed
	
	Revision 1.10  2013/02/25 12:19:24  chicas
	Added _wdirMutex
	
	Revision 1.9  2013/02/13 15:47:31  chicas
	setProcName setProcVersion removed (always taken form tasktable); input file with empty filetype moved in working dir but NOT added to JobOrder
	
	Revision 1.8  2013/01/10 15:59:48  chicas
	simplified buildJobOrder : list all passed inputs , no read task table ; WARNING buildJobOrder interface changed
	
	Revision 1.7  2013/01/09 13:01:57  chicas
	Move files in backupDir in case of failure
	
	Revision 1.6  2013/01/07 10:27:21  chicas
	setApplicationName instead of setRootKey
	
	Revision 1.5  2012/12/11 15:05:18  chicas
	Configurable Root Namespace; added sendMail in case of failure; message improved
	
	Revision 1.4  2012/11/30 12:42:03  chicas
	work in progress
	
	Revision 1.3  2012/11/29 17:15:47  chicas
	work in progress
	
	Revision 1.2  2012/11/27 17:24:47  chicas
	working for static buildJobOrder
	
	Revision 1.1.1.1  2012/11/26 14:45:46  chicas
	Imported sources
	
	
	
*/


#ifndef _TaskTableProcess_H_
#define _TaskTableProcess_H_

#include <acs_c++config.hpp>
#include <Thread.h>

#include <exException.h>
#include <scOrder.h>
#include <SchedulerExitStatus.h>
#include <dbPersistent.h>

#include <ThreadSafe.h>

#include <TaskTableProcessConfig.h>

_ACS_BEGIN_NAMESPACE(acs)


class DSProcessingMgr;
class SpawnedScheduler;
class dbConnection;
class TaskTable;

class TaskTableProcess : public acs::Thread // PRQA S 2109
{
public:
    exDECLARE_EXCEPTION(TaskTableProcessException, acs::exException) ; // PRQA S 2131, 2502
	

//! ctor 
/*! \param fullpathfile input file 
 *  \param filetype of input file 
 *  \param aTaskTable task table file name 
 *  \param workingDir common production dir  
 *
 *  \note Add others input files (if any) using addInputFile()
 *
 *  \par Example:
 *  \code
 *  {
 *       ..
 *       string inputfile = .. ; // "/tmp/ccc/filename.xx"
 *          //this could be downloaded form db
 *       string taskpath = ... ; // "/usr/acs/conf/TaskTable_12.xml" 
 *       
 *       TaskTableProcess* ttp = new TaskTableProcess(inputfile,
 *                                                    "MSI_L0_DS",
 *                                                    taskpath ,
 *                                                    "/raid0/production/12");
 *
 *       ttp->addInputFile(..,..);
 *       ttp->addInputFile(..,..);
 *
 *       //optional configurations 
 *       ttp->setOrderType("Import");
 *       ttp->setSatellite("Sentinel2");
 *       string clobdata = ..// downloaded from db
 *       ttp->setParameters(clobdata);
 *
 *       ttp->start();
 *      
 *      //..
 *      while (ttp->running() )
 *      {
 *          //..
 *      }
 *      //manage exception if any
 *
 *      //otherwise get exit status 
 *      SchedulerExitStatus status = ttp->exitStatus()
 *
 *      //..
 *
 * }
 *
 *  \endcode
 *
 *
 *  */

    TaskTableProcess(  const TaskTableProcessConfig::Sptr& configSptr,
                       const std::string& fullpathfile,
                       const std::string& filetype,
                       const std::string& aTaskTable,
                       const std::string& workingDir );

    virtual ~TaskTableProcess() throw();


    //! start the process
    /*! 
     *  Perform the followings:
     *  - build job order
     *  - build work order
     *  - start Scheduler
     *  - wait process end 
     *  - register final status
     *
     * \see   exitStatus()
     *  */
    virtual void run();
    
    //! Check if running
    bool isValid() const throw() ;
   
    //! Add an input file for the process 
    /*! \param filename \bcomplete \path of input file name
     *  \param filetype
     *  
     * \bAll input files will be copied (or moved) inside production dir.
     * \n
     * \bOnly files with valid filetypes (not empty) are added to joborder.
     * */ 
    void addInputFile(const std::string &filename, const std::string &filetype = "");

    //!return exit scheduler status;
    /*! To be used after the thread is finished to verify final status */
    SchedulerExitStatus exitStatus() const throw() { return _schedulerExitStatus;}
   
   //! return main input file 
    const std::string &mainInputFile() const throw() {return _inputFile;}
    const std::string &mainInputFilefullpath() const throw() {return _inputFilefullpath;}
    const std::string &workDir() const throw() {return _baseDir;}

    void setId(dbPersistent::IdType i) throw() {_identifier = i; } 
    dbPersistent::IdType getId() const throw()  {return _identifier;}

    //! set parameters for optional Parameters File
    /*! Data will be written inside the Parameter file in the working dir (and notified through joborder ) */
    void setParameters(const std::string &clobdata) { _joParams = clobdata; }
   
    void setSatellite(const std::string &s)      { _satelliteName = s; }  

    //! set mission number associated with this processing ; optional;
     /*! used for : 
     *  - WorkOrder CmdLineParameters  if requested from TT ( "${MISSION}" token inside command line param section ) 
     *  - WorkOrder satellite name ( used by Scheduler for building TT key)
     *  - JobOrder satellite name ( for the processor)

     *! \see setSatellite() */; 
    void setMission(const std::string &m)        { _missionNumber = m;}  
    void setSatelliteId(const std::string &v)	{ _satelliteId = v; }
    void setSensorId(const std::string &v)	{ _sensorId = v; }


    //! OrderType string; optional. 
    /*! OrderType string is: 
     *  - written inside job order 
     * \note Use this for notify the caller (DS, Import, Export) to the processor if needed.
     * */
    void setOrderType(const std::string & ot)    { _orderType = ot; }
   
    void setBackupDir(const std::string & bcdir)    { _thrashDir = bcdir; }

   
    //! Set an optional extension for builded files (workorder, joborder, parameterfile..) 
    /*!
     * */ 
    void setExtension( const std::string & ext) { _extension = ext; _setExt = true;}
    //! set address for sending mail in case of failure; optional
    void setMailNotificationAddresses( const std::string & succAddress, const std::string & failAddress ) { _successAddress = succAddress;  _failureAddress = failAddress; }
    
    void halt_(); 
    virtual void stop(bool joinAlso=true);
 
    /** ttType values in t_operationtypes */
    static void downloadTaskTablesOfType( const std::string& ttType, const std::string & ttdir, dbConnection & conn );

    static std::string downloadTaskTable(const std::string & ttname, const std::string & ttdir, dbConnection & conn);
    static std::string downloadTaskTable(dbPersistent::IdType id, const std::string & ttdir, dbConnection & conn, bool forceDownload=false);

    static void buildJobOrder( std::multimap<std::string,std::string> const & filetypeVsInputFile,
                                      const std::string& productiondir, 
                                      const std::string& jobordername = "JobOrder.xml", //no full path!
                                      const std::string& parametersFileName = "",        // full path!
                                      const std::string& orderType = "",
                                      const std::string& satelliteName = "" ,
                                      const std::string& missionNumber = "",
                                      const std::string& satelliteId = "",
                                      const std::string& sensorId = "" );

    static void buildJobOrder(        const std::string& filename, 
                                      const std::string& filetype, 
                                      const std::string& productiondir, 
                                      const std::string& jobordername = "JobOrder.xml", //no full path!
                                      const std::string& parametersFileName = "",        // full path!
                                      const std::string& orderType = "",
                                      const std::string& satelliteName = "" ,
                                      const std::string& missionNumber = "",
                                      const std::string& satelliteId = "",
                                      const std::string& sensorId = "" );

	
	void setProductionDir(const std::string& s) { _productionDir = s; }
	
	void setReadOnlyProduct(bool v) { _readOnlyProduct = v; }

	void taskLogInDefaultLogDir (bool) throw() ;
	bool taskLogInDefaultLogDir () const throw() ;
	
	void setBasketForWdsInError( const std::string & ) ;
	std::string getBasketForWdsInError() const;
	
	std::string getLogDirInError() const ;
	
protected:

private: // methods
    TaskTableProcess& operator = (const TaskTableProcess& ); // not implemented
    TaskTableProcess(const TaskTableProcess& ); // not implemented
    TaskTableProcess(); // not implemented
 
    //! Prepare all for the process
    /*! - create working dir  
     *  - move input file inside working dir
     *  - write parameter file ( inside working dir )
     * */
    void prepareOrder(); //call before run!

    void createWorkingDir();
    acs::scOrder buildWorkOrder();
    //void buildJobOrder();
    std::string genWorkOrderFileName() const;
    std::string genParametersFileName() const;
    std::string genJobOrderFileName(bool fullpath=true) const;
    std::string genLogFileName() const;
    std::string genTrackerFileName() const;
    //std::string genInterimFullFileName(const std::string& file_name) const;

    //static std::string genJobOrderFileName(const std::string& workdir,const std::string& ext) ;
    //static std::string genLogFileName(const std::string& workdir,const std::string& ext) ;
    static std::string genInterimFullFileName(const std::string& workdir,const std::string& file_name) ;

    void downloadParametersFile ();

    std::string notifyExitStatus(const SchedulerExitStatus& ec) const;

    //static std::string getFileTypeRegExp (const std::string&);


    acs::SpawnedScheduler *getScheduler();

	void moveWdOnError() ACS_NOEXCEPT ;

//called asyncronously if IMMEDIATE STOP

private: // attributes

    static acs::ThreadSafe _wdirMutex;

    /** Configuration */
    TaskTableProcessConfig::Sptr _configSptr;

    int  _maxTime;
    std::string _baseDir;
    acs::TaskTable * _taskTable;
    acs::SpawnedScheduler *_scheduler;
    bool _valid;
    std::string _extension;
    std::string _parametersFileName;

    std::multimap<std::string, std::string> _filetypeVsInputFile;

    std::string _inputFile;
    std::string _fileType;
    std::string _thrashDir;
    std::string _joParams;
    std::string _orderType;
	std::string _successAddress;
    std::string _failureAddress;
    SchedulerExitStatus _schedulerExitStatus;
    //T * _parent;
    std::string  _satelliteName ;
    std::string _missionNumber;
    std::string _satelliteId;
    std::string _sensorId;
    dbPersistent::IdType _identifier;

    bool _setExt; 
	
	std::string _productionDir;
	std::string _inputFilefullpath;
	bool _readOnlyProduct;
	bool _taskLogInDefaultLogDir;
	std::string _wdsInErrorBasket;
	ThreadSafe _basketMutex;
	bool _wdMovedInErrorBasket;
  
    ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTableProcess);

};

_ACS_END_NAMESPACE



#endif // _TaskTableProcess_H_


