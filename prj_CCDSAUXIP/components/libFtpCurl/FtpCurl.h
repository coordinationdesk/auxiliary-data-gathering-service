// PRQA S 1050 EOF
/*

 Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 $Prod: A.C.S. libFtpCurl Library $

 $Id$

 $Author$

 $Log$
 Revision 5.37  2017/04/12 13:04:10  marfav
 Added configurable support to fast probing
 avoiding to "ls" the entire site but consuming two connections per probe

 Revision 5.36  2017/04/11 16:31:06  marfav
 work in progress

 Revision 5.35  2016/03/21 18:25:54  francesco.avanzi
 WORKINPROGRESS s2pdgs-1434

 Revision 5.34  2016/03/18 13:16:29  francesco.avanzi
  S2PDGS-1434 FtpCurl::put modified: a bool is now taken and passed to Curl::put to regulate target cleanup

 Revision 5.33  2016/03/07 15:33:56  marfav
 S2PDGS-1428, PDSEV-28
 Stop controllers managed during FTP file listing and collection

 Revision 5.32  2015/04/23 11:54:35  marpas
 coding best practices applied
 qa warning fixed

 Revision 5.31  2015/04/16 13:06:30  marpas
 statistical methods rationalization

 Revision 5.30  2015/04/09 15:55:38  marfav
 S1PDGS-31623 adding HTTP GET operation (on string)

 Revision 5.29  2015/03/10 14:51:50  lucio.pulvirenti
 S2PDGS-984: checkCURLcode added to IF. FtpCurlRecoverableErrorException and FtpCurlUnrecoverableErrorException defined to identify macro exc groups.
 Code analyzed to check where throwing FtpCurlUnrecoverableErrorException.
 throwCurlSpecificException: cURL codes checked to launch either FtpCurlRecoverableErrorException or FtpCurlUnrecoverableErrorException

 Revision 5.28  2015/03/09 17:00:20  lucio.pulvirenti
 S2PDGS-984: work in progress.

 Revision 5.27  2015/03/06 16:45:56  lucio.pulvirenti
 S2PDGS-984: work in progress.

 Revision 5.26  2014/12/09 11:40:45  francesco.avanzi
 modified to use new ContentType

 Revision 5.25  2014/11/18 15:53:45  lucio.pulvirenti
 S2PDGS-915: private methods added to manage http ls output.

 Revision 5.24  2014/11/14 16:34:23  lucio.pulvirenti
 S2PDGS-915: work in progress.

 Revision 5.23  2014/11/13 16:49:51  lucio.pulvirenti
 S2PDGS-915: Work in progress.

 Revision 5.22  2014/10/29 12:02:11  lucio.pulvirenti
 S2PDGS-955: setFtpProxy method removed: Proxy class introduced in RemoteSite.h

 Revision 5.21  2014/10/28 16:31:43  lucio.pulvirenti
 S2PDGS-955 work in progress.

 Revision 5.20  2014/08/07 08:08:53  lucio.pulvirenti
 S2PDGS-836: FtpCurlInitializationFailedException added to manage CURLE_FAILED_INIT error without emitting exception description when caught.

 Revision 5.19  2014/07/03 07:41:45  marpas
 useless method removed

 Revision 5.18  2014/06/27 13:46:48  marpas
 deprecated methods removed: setRenameTarget setCreateRemoteDirFlag

 Revision 5.17  2014/06/25 17:42:11  marpas
 qa warning fixed

 Revision 5.16  2014/06/20 14:52:34  marpas
 useless private variables removed
 statistics management (log) fixed
 statistics management (t_circulationstats) fixed
 operation are now an enum

 Revision 5.15  2014/06/19 18:04:22  francesco.avanzi
 minor changes

 Revision 5.14  2014/06/05 18:45:58  marpas
 getting rid of asserts
 interface robustness improved
 tests fixed

 Revision 5.13  2014/02/21 16:43:32  lucio.pulvirenti
 Useless defined removed and performance method of member used.

 Revision 5.12  2014/02/20 14:07:39  francesco.avanzi
 BandPriority.h now included

 Revision 5.11  2014/02/14 13:54:32  marpas
 attempt to refactor band shaping mechanism

 Revision 5.10  2014/02/11 10:14:49  marpas
 proning remote site is synchronized for each specific site

 Revision 5.9  2014/02/10 17:21:58  lucio.pulvirenti
 Mutex must be static because check static map access.

 Revision 5.8  2014/01/31 11:56:28  marpas
 interface rationalization, robustness improved

 Revision 5.7  2013/12/17 18:09:22  marpas
 minor changes

 Revision 5.6  2013/10/28 13:42:14  marpas
 cleaning macros and curl versions.

 Revision 5.5  2013/10/24 18:27:59  marpas
 work in progress

 Revision 5.4  2013/10/23 17:28:11  marpas
 work in progress

 Revision 5.3  2013/10/22 17:31:50  marpas
 work in progress

 Revision 5.2  2013/07/23 17:10:11  marpas
 classes rationalization and robustness improved

 Revision 5.1  2013/06/18 12:54:14  marpas
 debug macros rationalized

 Revision 5.0  2013/06/06 18:08:20  marpas
 adopting linException 4.x standards

 Revision 2.21  2013/05/29 10:53:58  marpas
 interface rationalization,
 coding best practices applied,
 qa rules

 Revision 2.20  2013/04/17 20:37:21  marpas
 interface robustness

 Revision 2.19  2013/04/05 09:44:15  marpas
 coding best practices applied
 qa warnings (partial)
 libDCUtility interface update

 Revision 2.18  2013/01/15 11:22:32  micmaz
 work in progress

 Revision 2.17  2012/11/29 16:28:20  micmaz
 quality tool warnings removed on destructors.

 Revision 2.16  2012/11/29 14:38:56  micmaz
 IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 http://jira.acsys.it/browse/S1PDGS-30117

 Revision 2.15  2012/11/27 13:29:33  micmaz
 added quality tool directives.

 Revision 2.14  2012/06/01 10:07:50  lucio.pulvirenti
 LP work in progress: overloaded get method added taking mode_t parameter

 Revision 2.13  2012/05/30 18:23:49  micmaz
 added chmod to FtpCurl::put()

 Revision 2.12  2012/03/28 12:59:18  micmaz
 work in progress

 Revision 2.11  2012/03/27 18:08:48  micmaz
 work in progress

 Revision 2.10  2012/03/27 10:52:21  micmaz
 work in progress

 Revision 2.9  2012/03/16 16:20:33  micmaz
 the remote directory test fails

 Revision 2.8  2012/03/08 14:22:46  micmaz
 updated tests to cppunit

 Revision 2.7  2012/03/08 13:11:40  micmaz
 added media functionality

 Revision 2.6  2012/02/09 16:48:24  micmaz
 added ftps CCC  configuration parameter.

 Revision 2.5  2012/02/08 16:18:10  micmaz
 added probe method in order to verify if the remote site (ftp/ftps) is passive/active.

 Revision 2.4  2012/02/07 10:25:47  micmaz
 removed warnings .

 Revision 2.3  2012/02/02 17:19:58  micmaz
 IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 http://jira.acsys.it/browse/S1PDGS-2769

 Revision 2.2  2012/01/20 16:06:20  micmaz
 added statistics.

 Revision 2.1  2011/12/12 17:52:37  micmaz
 IN PROGRESS - issue S1PDGS-2595: CirculationAgent problem with the temporary directory
 http://jira.acsys.it/browse/S1PDGS-2595

 Revision 2.0  2011/10/18 14:43:51  marpas
 new statistics adopted

 Revision 1.67  2011/09/29 18:33:04  marpas
 fixing codecheck crashes

 Revision 1.66  2011/09/22 15:08:52  micmaz
 updated tests.

 Revision 1.65  2011/09/16 13:20:59  micmaz
 reworked namespaces.

 Revision 1.64  2011/07/16 13:36:53  micmaz
 progress...

 Revision 1.63  2011/07/08 17:49:09  micmaz
 RESOLVED - issue S1PDGS-1650: CirculationAgent problem remove source
 http://jira.acsys.it/browse/S1PDGS-1650

 Revision 1.62  2011/07/01 17:04:20  micmaz
 added two deprecated methods in order to ensure compatibility.

 Revision 1.61  2011/07/01 16:42:11  micmaz
 changed the temporary file name strategy.

 Revision 1.60  2011/06/16 18:27:12  micmaz
 work in progress

 Revision 1.59  2011/06/13 17:33:14  micmaz
 work in progress

 Revision 1.58  2011/05/30 15:24:45  micmaz
 added localhost as valid remote host

 Revision 1.57  2011/05/12 09:28:58  micmaz
 work in progress

 Revision 1.56  2011/05/10 17:10:46  micmaz
 corrected bug of ProgressData callback: destroyed while working.

 Revision 1.55  2011/05/10 12:58:27  micmaz
 added getFullLocalFilename() and setFullLocalFilename() methods

 Revision 1.54  2011/05/06 16:31:21  micmaz
 work in progress...

 Revision 1.53  2011/05/05 17:52:33  micmaz
 work in progress...

 Revision 1.52  2011/05/04 09:52:10  micmaz
 added a method to decide if a  remote resource exits and if it is a file or directory.

 Revision 1.51  2011/04/29 17:20:42  micmaz
 check style changes

 Revision 1.50  2011/04/14 16:33:12  micmaz
 work in progress...

 Revision 1.49  2011/04/07 13:29:13  micmaz
 work in progress

 Revision 1.48  2011/04/07 09:00:52  micmaz
 work in progress..

 Revision 1.47  2011/04/05 17:15:09  micmaz
 *** empty log message ***

 Revision 1.46  2011/03/15 10:20:02  micmaz
 work in progress

 Revision 1.45  2011/03/03 14:28:46  micmaz
 work in progress

 Revision 1.44  2011/03/02 18:53:00  micmaz
 corrected crash when log performance during/after closing connection.

 Revision 1.43  2011/02/24 11:41:10  micmaz
 Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.

 Revision 1.42  2011/02/23 14:39:25  micmaz
 Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.

 Revision 1.41  2011/02/10 15:53:32  micmaz
 added statistics.

 Revision 1.36-1.40  2011/02/04 11:31:46  micmaz
 work in progress...

 Revision 1.35  2011/01/27 16:13:28  micmaz
 adapted the library to old libcURL 1.15

 Revision 1.31-1.34  2011/01/27 09:50:22  micmaz
 work in progress...

 Revision 1.30  2011/01/19 16:42:58  micmaz
 added the openConnection() and closeConnection() to cURL operations.

 Revision 1.29  2011/01/13 14:00:58  micmaz
 changed file permissions

 Revision 1.28  2011/01/13 10:57:28  micmaz
 added mput() and mget() method in order to open only one connection if you must trasfer more than one file to/from the same host.

 Revision 1.27  2010/12/20 16:10:08  lucio.pulvirenti
 _wrote attrubute added to FtpFile struct IF

 Revision 1.26  2010/12/20 11:28:17  micmaz
 renamed some methods.

 Revision 1.25  2010/12/17 15:52:29  lucio.pulvirenti
 Exception derived by FtpCurlException base class added
 open method added to FtpFile struct
 throwCurlSpecificException static method added

 Revision 1.24  2010/11/24 13:04:54  marpas
 FTP_PROXY env var used by curl now controlled via setFtpProxy method and set to empty string by default
 debug improved

 Revision 1.23  2010/11/11 16:17:24  micmaz
 added the rename remote directory function.

 Revision 1.22  2010/11/04 10:16:06  micmaz
 added the slowDown field in order to slow the file transfer.

 Revision 1.21  2009/10/06 15:33:10  marpas
 list method completed
 performances added

 Revision 1.20  2009/10/06 09:51:36  marpas
 passive mode implemented
 rules applied

 Revision 1.19  2008/07/14 13:24:46  marpas
 documentation improved

 Revision 1.18  2008/06/13 13:00:32  manuel.maccaroni
 First release of documnted version.
 Section usage to be added.

 Revision 1.17  2008/06/12 07:54:12  manuel.maccaroni
 Fixed an issue in the default constructor which generated segmentation fualts when the user did not introduce and set a custom progress callback.

 Introduced some exception handling inside the callbacks.

 Revision 1.16  2008/06/09 17:55:10  manuel.maccaroni
 some documentation

 Revision 1.15  2008/06/09 13:09:31  manuel.maccaroni
 Custom progress callback added.

 Revision 1.14  2008/06/09 12:19:30  manuel.maccaroni
 Fixed an issue that didn't permit to use paths ending without a /

 Revision 1.13  2008/06/09 08:08:50  manuel.maccaroni
 StopController check implemented inside callback.

 Revision 1.12  2008/06/06 14:24:02  manuel.maccaroni
 first implentation of stop controlling in a callback (to be refined).

 Revision 1.11  2008/06/06 13:27:13  manuel.maccaroni
 set(), get() and del() now use a StopController as a defaulted reference in the argument list.

 Revision 1.10  2008/06/06 08:39:40  manuel.maccaroni
 set(), get() and del() now use a StopController as a defaulted reference in the argument list.

 Revision 1.9  2008/06/05 11:16:08  manuel.maccaroni
 put function checks a flag to allow or not remote missing directory creation.
 This flag is a bool private class attribute (_createRemoteDirFlag) defaulted to false by the class constructor.

 Revision 1.8  2008/06/05 09:05:38  matteo.airoldi
 Modified set/get renameTarget

 Revision 1.7  2008/06/05 08:17:05  manuel.maccaroni
 More consistent order in constructor's list of arguments.
 Corrected a minor issue in put() for arguments check (exception mechanism)

 Revision 1.6  2008/06/03 14:18:41  manuel.maccaroni
 Better implementation of target renomination logic: a bool private attribute defaulted to true along with getter and setter methods are introduced.

 Revision 1.5  2008/06/03 13:04:16  manuel.maccaroni
 Introduced a bool parameter (defaulted to true) to control target file rename during a put.

 Revision 1.4  2008/06/03 11:36:49  manuel.maccaroni
 Introduced _localFile and _remoteFile private attributes, in order to make it possible to control over remote and
 local file resources; therefore _file attribute has been removed.
 Provided set and get methods for the two attributes defined above.
 Fixed minor incongruences in comments and exception trown messages.

 Revision 1.3  2008/06/03 09:52:35  manuel.maccaroni
 Default constructor added.

 Revision 1.2  2008/05/30 13:08:31  marpas
 minor changes, doxygen documentation fixed

 Revision 1.1.1.1  2008/05/14 12:44:00  matteo.airoldi
 libFtpCurl first Issue


 */

/*! \mainpage libFtpCurl DOCUMENTATION
 *
 * \section Introduction
 * This library represents an abstraction layer to the libCurl library, providing a frontend for FTP based transfers with ease of use and operation.
 * In the library a class is defined, its methods feature all the intermediate operations and settings that is necessary to make in order to
 * setup FTP transfers with libCurl; in this view this class lays in between you (the user) and a lower layer in which operates libCurl.
 * By doing this you are only required to set inside the class' internals a restricted set of parameters, the more strict ones
 * required to perform FTP transfers, namely:
 * <ul>
 *  <li> the host name or address of the remote machine
 *  <li> the user name to access the host
 *  <li> the password associated with the user
 *  <li> the working path on the remote machine
 *  <li> the name of the remote file located in the remote path
 *  <li> the path on the local machine
 *  <li> the name of the local file located in the local path
 * </ul>
 *
 * plus three more that represent options that could not be hided in the class implementation because it was found convenient to
 * make them available to the user.
 * Transfers are executed by invoking three methods, after having set all the necessary parameters to an object of the class you previously
 * instantiated.
 * Before briefly introducing those methods, always keep in mind that the public interface of the class has been designed to be the more transparent,
 * it was possible to design, for the user experience regarding FTP based operations. This way each method's name really reflects and resumes
 * the operations
 * it is going to perform, so that the higher abstraction and simplification level, respect to libCurl, featured in this library is definitely significant.
 * The three methods to be invoked to perform the transfers are the following:
 * <ul>
 *  <li> put() - performs upload: sends a file from the local path to the remote machine and saves it
 *  <li> get() - performs download: fetches a file from the remote path and downloads it in the local one
 *  <li> del() - performs remote deletion: removal of a specified file located in the remote path
 * </ul>
 * Before performing any transfer you have to be sure that the required parameters to perform that transfer have been set into the class
 * either upon construction (using the perameterized constructor) or using the relative set methods; here the get methods come to help at any point
 * by returning the value of the corresponding parameter internally set.
 * Also know that each one of the three method only requires a minimal subset of patrameters, so you are not forced to set all of them; the following table
 * displyas these requirements, although they can be inferred with a small work of the mind as they really are what you need to "tell" to the class,
 * for it to be able to
 * perform either uploads, downloads or deletions.
 *
 *
 * <TABLE border=1 cellspacing=0 cellpdadding=0 align="center">
 * <caption> required parameters for get(),put() and del() methods </caption>
 * <TR style="text-align:center;vertical-align: middle;">
 *    <TD> </TD> <TD colspan=7> required parameters to be set </TD>
 * </TR>
 * <TR style="text-align:center;vertical-align: middle;">
 *    <TD> method </TD> <TD> host name </TD> <TD> user login </TD> <TD> password </TD>
 *    <TD> local path </TD> <TD> local filename </TD> <TD> remote path </TD> <TD> remote filename</TD>
 * </TR>
 * <TR style="text-align:center;vertical-align: middle;">
 *    <TD align="left"> put() </TD>  <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> note 1</TD>
 * </TR>
 * <TR style="text-align:center;vertical-align: middle;">
 *    <TD align="left"> get() </TD>  <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> note 2 </TD> <TD> X </TD> <TD> X </TD>
 * </TR>
 * <TR style="text-align:center;vertical-align: middle;">
 *    <TD align="left"> del() </TD>  <TD> X </TD> <TD> X </TD> <TD> X </TD> <TD> - </TD> <TD> - </TD> <TD> X </TD> <TD> X </TD>
 * </TR>
 * </TABLE>
 *
 * note 1: if you do not specify a name for the file to upload to, put() will save a file in the remote path with the same name as the local source file name.
 *
 * note 2: if you do not specify a name for the file to download to,get() will save a file in the local path with the same name as the remote source file name.
 *
 *
 *
 *
 * \section Usage
 *
 * Before using the library, and therefore be able to create instances of FtpCurl class, you have to include in your code
 * the proper header file and set access to the acs namespace:
 *
 * \code
 *  ...
 *  #include <FtpCurl.h>
 *  using namespace acs ;
 *  ...
 * \endcode
 *
 * now, in some point in your code, you can begin instantiating objects:
 * \code
 *  ...
 *  FtpCurl AnObject ;
 *  FtpCurl AnotherObject(host, user, password, remote_path, remote_file_name, local_path, local_file_name) ;
 *  ...
 * \endcode
 *
 * In the example, both ways of constructing objects of FtpCurl class, namely with default constructor and with parameterized one, are shown; keep
 * always in mind that the default constructor sets all the parameters needed to perform transfers (see documentation for put(), get() and del() )
 * as empty strings, thus producing an error if you try to perform transfers without having set these parameters before.
 *
 *
 *
 * \subsection Upload How to upload a file
 *
 * File uploads to remote resources are performed by invoking calss's put() method.
 * The function doesn't require to pass any arguments at all, however it accesses some private attributes
 * that store all the necessary parameters, for the method itself to know were is the file to be uploaded to which remote path.
 *
 * These parameters can be set either by parameterized contruction or by set methods, all you need to specify is:
 * <ul>
 *  <li> the host name or address of the remote machine
 *  <li> the user name to access the host
 *  <li> the password associated with the user
 *  <li> the path on the local machine where the file to be read and sent is located
 *  <li> the name of the file in the locale path you wish to transfer
 *  <li> the path on the remote machine to wich store the uploaded data
 *  <li> optionally specify a name for the file in the remote path, otherwise uploaded data are saved to a file with the same name as the source.
 * </ul>
 *
 * For example if you are planning to upload a file and want to set all the stuff at construction time, you may want to code the following:
 *
 * \code
 *  ...
 *  FtpCurl UploadObject(host, user, password, remote_path, [optional:remote_file_name], local_path, local_file_name) ;
 *
 *  UploadObject.put() ;
 *  ...
 * \endcode
 *
 * (note that if you decide to not specify the optional argument you actually have to pass it as an empty string "", instead of neglecting
 * the argument, which is not allowed),
 * instead the following illustrates the usage of set methods in the case of a standard constructor call:
 *
 * \code
 *  ...
 *  FtpCurl UploadObject ;
 *
 *  UploadObject.setHost(host);
 *  UploadObject.setUser(user);
 *  UploadObject.setPwd(password);
 *  UploadObject.setRempath(remote_path);
 *  UploadObject.setRemoteFile(remote_file_name); //this is optional
 *  UploadObject.setLocalPath(local_path);
 *  UploadObject.setLocalFile(local_file_name);
 *
 *  UploadObject.put() ;
 *  ...
 * \endcode
 *
 * For more flexibility, and only for the put() method, the library provides the user with two more options that can be set; namely it is possible
 * to tell to the put() method, whether or not to create all the directories, in the remote path you specify, that are missing in the remote machine during
 * a call. By default each time you create an FtpCurl object this option is disabled, regardless of which constructor is called, meaning that if the remote path
 * you set contains any missing directory, the transfer is automatically aborted by returning an error.
 * You have to be careful when specifying remote paths if the option is disabled, however you can allow put() to create the missing directories by
 * setting the option in an enabled status:
 *
 * \code
 *  ...
 *
 *  UploadObject.setCreateRemoteDirFlag(true) ;

 ** ...
 * \endcode
 *
 * At any time you can fetch the status of the option by issuing the getCreateRemoteDirFlag() method, which will return a true boolean if the option
 * is enabled, false otherwise.
 *
 * The other option simply affects the way the uploaded file is stored remotely: if set to true put() will store data in a temporary file (which name is the one
 * you specified with a .ftptmp appended after), to be later renamed as remote_file_name once transfer is complete.
 * If, in turn, this option is set to false then put()
 * will directly store data in a file named as remote_file_name.
 * The default value, set upon construction, is true; again it is easy for you to change its value:
 *
 * \code
 *  ...
 *
 *  UploadObject.setRenameTarget(false) ;
 *
 *  ...
 * \endcode
 *
 * and to fetch the current value by checking the boolean return value of getRenameTarget() method.
 *
 *
 * \subsection Download How to download a file
 *
 * Performing a download requires you to specify to the class which remote file you want to fetch from the remote path
 * and where you want to store it. Then you invoke the get() method to start performing the operation.
 *
 * In a similar fashion to the put() method you are requested to communicate, either upon construction or using set methods,
 * all the following:
 *
 * <ul>
 *  <li> the host name or address of the remote machine
 *  <li> the user name to access the host
 *  <li> the password associated with the user
 *  <li> the path on the remote machine in which the file to be downloaded is located
 *  <li> the name of the remote file to fetch and download
 *  <li> the path on the local machine where you are going to store downloaded data
 *  <li> optionally the name of the file in which downloaded data will be saved, if no specified get() will save a file with the same name as the source one.
 * </ul>
 *
 * All the operations to be performed resemble those illustrated for the put() method, except, of course, the two options
 * regarding remote file renomination and missing directories creation, that are meaningless for downloads and so you don't need
 * to worry about them at all.
 *
 * Anyway, for the sake of thoroughness, exemples of how to perform the operations will be shown in the following.
 * If you decide to set everithing at the time of object construction you may consider this example:
 *
 * \code
 *  ...
 *  FtpCurl DownloadObject(host, user, password, remote_path, remote_file_name, local_path, [optional:local_file_name]) ;
 *
 *  DownloadObject.get() ;
 *  ...
 * \endcode
 *
 * (note that if you decide to not specify the optional argument you actually have to pass it as an empty string "", instead of neglecting
 * the argument, which is not allowed),
 * instead if you plan to make use of set methods, in conjunction with a standard constructor call, you may want to write:
 *
 * \code
 *  ...
 *  FtpCurl DownloadObject ;
 *
 *  DownloadObject.setHost(host);
 *  DownloadObject.setUser(user);
 *  DownloadObject.setPwd(password);
 *  DownloadObject.setRempath(remote_path);
 *  DownloadObject.setRemoteFile(remote_file_name);
 *  DownloadObject.setLocalPath(local_path);
 *  DownloadObject.setLocalFile(local_file_name); //this is optional
 *
 *  DownloadObject.get() ;
 *  ...
 * \endcode
 *
 * Finally bear in mind that, as always, the get methos provide you with a simple manner to check for the
 * parameters that have been set internally to the class.
 *
 *
 * \subsection Deletion Remote file deletion
 *
 * The del() method lets you perform a dletion of specified remote file, provided that you specify
 * the usual set of required parameters. In this case you have to set the following:
 *
 * <ul>
 *  <li> the host name or address of the remote machine
 *  <li> the user name to access the host
 *  <li> the password associated with the user
 *  <li> the path on the remote machine in which the file to be deleted is located
 *  <li> the name of the remote file to delete.
 * </ul>
 *
 * After this it is sufficient to invoke the method and the file will be removed.
 * Again we show some usage examples, we begin with the case of parameterized construction:
 *
 * \code
 *  ...
 *  FtpCurl DeletingObject(host, user, password, remote_path, remote_file_name) ;
 *
 *  DeletingObject.del() ;
 *  ...
 * \endcode
 *
 * while an example featuring standard construction with set methods follows here:
 *
 * \code
 *  ...
 *  FtpCurl DeletingObject ;
 *
 *       DeletingObject.setHost(host);
 *       DeletingObject.setUser(user);
 *       DeletingObject.setPwd(password);
 *       DeletingObject.setRempath(remote_path);
 *       DeletingObject.setRemoteFile(remote_file_name);
 *
 *  DeletingObject.del() ;
 *  ...
 * \endcode
 *
 * As usual you have also at your hand all the set of get methods for your checking up convenience.
 */

#ifndef _FtpCurl_H_
#define _FtpCurl_H_

#include <acs_c++config.hpp>

#include <CirculationStatistics.h>
#include <File.h>
#include <Curl.h>
#include <CurlConfigurer.h>
#include <CurlErrorBehaviourSingleton.h>
#include <BandPriority.h>

#include <Timer.h>
#include <exException.h>
#include <StopController.h>
#include <ThreadSafe.h>
#include <FtpPerformance.h>
#include <FtpProgressData.h>
#include <HttpProgressData.h>
#include <WaitPid.h>

#include <vector>

_ACS_BEGIN_NAMESPACE(acs) 
_ACS_BEGIN_NAMESPACE(curl) 
class FtpListItem ;
class FtpListReader;
_ACS_END_NAMESPACE

enum RemoteResourceType { // NOSONAR - old enum
     RRTNotExist = 0, RRTIsFile = 1, RRTIsDir = 2
};

/*!
 * \brief Performs file trasfers to and from a remote host using Ftp connection.
 *
 * The class is based upon the open source libCurl library and uses its functionalities to perform all the operations.
 *
 */

class FtpCurl { // PRQA S 2109, 2153
public:


    /*! class FtpCurlException declaration */
    // generic excexption (base class) :
    exDECLARE_EXCEPTION(FtpCurlException, acs::curl::Curl::CurlException);
	// Exception due to a recoverable error
    exDECLARE_EXCEPTION(FtpCurlRecoverableErrorException, FtpCurlException);
	// Exception due to a definitive error
    exDECLARE_EXCEPTION(FtpCurlUnrecoverableErrorException, FtpCurlException);
    // Login Denied [e.g., bad user/password couple] (CURLE_LOGIN_DENIED)
    exDECLARE_EXCEPTION(FtpCurlLoginDeniedException, FtpCurlUnrecoverableErrorException);
    // Connection Failed [e.g., ftp service down]  (CURLE_COULDNT_CONNECT)
    exDECLARE_EXCEPTION(FtpCurlConnectionFailedException, FtpCurlRecoverableErrorException);
    // Host name cannot be resolved (CURLE_COULDNT_RESOLVE_HOST)
    exDECLARE_EXCEPTION(FtpCurlHostnameNotFoundException, FtpCurlUnrecoverableErrorException);

    // Remote file not found / wrong permissions (CURLE_FTP_COULDNT_RETR_FILE) :
    exDECLARE_EXCEPTION(FtpCurlRemoteRetrieveErrorException, FtpCurlUnrecoverableErrorException);
    // Access Denied on remote file (CURLE_FTP_ACCESS_DENIED)
    exDECLARE_EXCEPTION(FtpCurlRemoteWriteErrorException, FtpCurlUnrecoverableErrorException);
    // Couldn't read/access local file :
    exDECLARE_EXCEPTION(FtpCurlLocalReadErrorException, FtpCurlUnrecoverableErrorException);
    // Write error on local file system [e.g. wrong permission on local file/dir] (CURLE_WRITE_ERROR)
    exDECLARE_EXCEPTION(FtpCurlLocalWriteErrorException, FtpCurlUnrecoverableErrorException);
    // Initialization failed (CURLE_FAILED_INIT) [ e.g. Failure establishing ssh session]
    exDECLARE_EXCEPTION(FtpCurlInitializationFailedException, FtpCurlRecoverableErrorException);
    // http 404 error: not found
    exDECLARE_EXCEPTION(FtpCurlHttpNotFoundException, FtpCurlUnrecoverableErrorException);

    using  NextLineCallback = void (*)(const std::string &line, void* data);
    using  FileFilter = bool (*)(std::string const& filename);

public: // NOSONAR - for clarity
    enum Operation { // NOSONAR - old enum
		OpHttpPost,
		OpDel,
		OpDelTree,
		OpList,
		OpGet,
		OpPut,
		OpPutAsTar,
		OpPutTree,
		OpGetTree,
		OpRename,
		OpChMod,
		OpHttpGet,
		OpHttpPropFind,
		OpHttpPut,
		OpHttpDel,
		OpHttpPatch
	};


    static std::string toString(Operation) ; // PRQA S 2502
    static bool hasPerfo(Operation) ;
    
    explicit FtpCurl(const curl::BandPriority&);
    /*! \brief Parameterized class constuctor.
     *
     * Assigns the specified value to the corresponding string-private-attributes arguments, you can omit any one in the list by stating
     * introducing a "" in the argument list.
     *
     * \param host an std::string object containing the host name or address
     * \param user an std::string object containing the user name for the host
     * \param pwd an std::string object containing the password for the host
     * \param remotePath an std::string object containing the remote path
     * \param remoteFile an std::string object containing the name of the remote file (in the remote path)
     * \param localPath an std::string object containing the local path
     * \param localFile an std::string object containing the nale of the local file (in the local path)
     *
     * Keep also in mind that this constructors has got a default behavior: it executes the following assignments to private attributes:
     *
     *  _renameTarget = true ;
     *
     *  _createRemoteDirFlag = false;
     *
     * _progressCallback = 0;
     *
     * that is, for the first two assignments, during any transfer the put() method cannot create any directory contained in the remote path
     * if it is actually missing in the remote resource (so you have to be careful that the path you are using for uploading exists remotely),
     * and uploading occurs by firstly creating a dummy temporary file that is it then renamed when uploading is finished.
     * The last assignment is more clear if you have a look at the typedef for ProgressCallback, briefly it means that a pointer to
     * a function is set to zero upon class construction. This pointer should be set to point to a function defined by the user as a custom
     * progress callback to be called by the default one; if the user never defines this function or doesn't set the pointer, then
     * _progressCallback will always keep its zero value. This behavior ensures that if no custom callback is set then the custom one will
     * never attemp to access an undefined function becase it features a check for zero-pointers.
     *
     * Remember always about these default behaviors, because tey are actually implemented as well in the default constructor, so it is truly
     * hard-coded inside the class; if you do not want any of this then change the status of the falgs using the corrisponding set methods.
     */
    FtpCurl(const curl::BandPriority&, const std::string &host, const std::string &user, const std::string &pwd, const std::string &remotePath,
            const std::string &remoteFile, const std::string &localPath, const std::string &localFile);

    FtpCurl(const curl::BandPriority&, acs::curl::RemoteSite const& site);

    FtpCurl(const curl::BandPriority&, acs::curl::RemoteSite const& site, std::string const& localDir, std::string const& localFile);

    /*! \brief Class destructor. */
    virtual ~FtpCurl() noexcept ;
    
    struct FtpCurlPermissions {
        mode_t permissions = 0 ;
        uid_t userId = -1 ;
        gid_t groupId = -1 ;
    };
	
	class PropFindStatItem
	{

	public:
		PropFindStatItem() = default ;
		~PropFindStatItem() = default ;

		void setName( const std::string &) ;
		void setIsDir( const bool &);
		void setSize( const long long int & );
		void setCreationDate( const DateTime & );
		void setModificationDate( const DateTime & );
		
		std::string getName() const;
		bool getIsDir( bool &isSet ) const ;
		long long int getSize(bool &isSet) const ;
		DateTime getCreationDate( bool & isSet ) const ;
		DateTime getModificationDate( bool & isSet ) const;
		
	private:
	    std::string _name = "" ;
		
		bool _dir = false ;
		bool _dirSet = false ;
		
		long long int _size = 0 ;
		bool _sizeSet = false ;
		
   		DateTime _creationDate = {} ;
		bool _creationDateSet = false ;
		
   		DateTime _modificationDate = {}  ;
		bool _modificationDateSet = false ;

	} ;
    
	enum PropFindStatItemOrder // NOSONAR - old enum
	{ 
		PROPFINDITEMS_NONE_ORDER, 
		PROPFINDITEMS_NAME_ORDER, 
		PROPFINDITEMS_CREATIONDATE_ORDER, 
		PROPFINDITEMS_MODIFICATIONDATE_ORDER 
	} ;
	
	
	enum PropFindRequests // NOSONAR - old enum
	{
		PROPFINDREQ_NONE 			= 0x0000,
		PROPFINDREQ_TYPE			= 0x0001,
		PROPFINDREQ_LENGTH			= 0x0002,
		PROPFINDREQ_CREATION		= 0x0004,
		PROPFINDREQ_MODIFICATION	= 0x0008
	} ;

	
private:
    FtpCurl(); // not implemented
    FtpCurl(FtpCurl const& ); // not implemented
    FtpCurl& operator=(FtpCurl const& ) ;// not implemented
public:

    /*! \brief Download memeber function.
     *
     * Using this member function you can perform a download transfer from the remote resource.
     * The function fetches the remote file in the remote path and downloads it to the local path.
     * Of course it is strictly necessary to set the following members via the appropriate set methods or
     * using the constructor (both ways you are required to pass strings).
     *
     * <ul>
     *  <li> the host name or address of the remote machine (method setHost() )
     *  <li> the user name to access the host (method setUser() )
     *  <li> the password associated with the user (method setPwd() )
     *  <li> the path on the remote machine from which you wish to download (method setRempath() )
     *  <li> the name of the file in the remote path you wish to transfer to your local resource (method setRemoteFile() )
     *  <li> the path on the local machine to wich store the downloaded data (method setLocalPath() )
     *  <li> optionally specify a name for the file to store locally, otherwise dowloaded data are saved to a file with the
     * same name as the source (method setLocalFile() ).
     * </ul>
     *
     * Always make sure that you have set all of this in the object you are using if you wish to perform a download.
     *
     * \param stopController= StopController() a defaulted const reference to a #StopController, used to manage and perform thread-related stop requests.
     */
    void get(StopController const& stopController = StopController());
    void get(mode_t permissions, StopController const& stopController = StopController());
    void get(const FtpCurlPermissions& ftpCurlPerm, StopController const& stopController = StopController());

   /*! \brief Download memeber function with untar "on-the-fly".
     *
     * Using this member function you can perform a download transfer from the remote resource, applying a untar "on-the-fly"
     * The function fetches the remote file in the remote path, send the stream to the untar function and expand the tar to the local path.
     * Of course it is strictly necessary to set the following members via the appropriate set methods or
     * using the constructor (both ways you are required to pass strings).
     *
     * <ul>
     *  <li> the host name or address of the remote machine (method setHost() )
     *  <li> the user name to access the host (method setUser() )
     *  <li> the password associated with the user (method setPwd() )
     *  <li> the path on the remote machine from which you wish to download (method setRempath() )
     *  <li> the name of the file in the remote path you wish to transfer to your local resource (method setRemoteFile() )
     *  <li> the path on the local machine to wich store the downloaded data (method setLocalPath() )
     *  <li> optionally specify a name for the file to store locally, otherwise dowloaded data are saved to a file with the
     * same name as the source (method setLocalFile() ).
     * </ul>
     *
     * Always make sure that you have set all of this in the object you are using if you wish to perform a download.
     *
     * \param stopController= StopController() a defaulted const reference to a #StopController, used to manage and perform thread-related stop requests.
     */
    void getUnTar(const File::CompressionMode& cm, const mode_t& permissions, StopController const& stopController = StopController());
    void getUnTar(const File::CompressionMode& cm, const mode_t& permissions, \
					const std::vector <std::string>& decompress_options, StopController const& stopController = StopController());
    void getUnTar(const File::CompressionMode& cm, const FtpCurlPermissions& ftpCurlPerm, \
					const std::vector <std::string>& decompress_options, StopController const& stopController = StopController());

    /**
     * Tests if a remote or local file/directory exists.
     * @param testFile: true if we are testing a file, false if a directory
     * @param stopController: the stop controller used to stop the remote operation.
     * @param emitException: in case of failures return just not found in case no exception can be emitted
     */
    bool existDestinationFile(bool testFile = true, StopController const& stopController = StopController(), bool emitException = true);

    /**
     * Tests if a remote file/directory exists.
     * @param testFile: true if we are testing a file, false if a directory
     * @param stopController: the stop controller used to stop the remote operation.
     * @param emitException: in case of failures return just not found in case no exception can be emitted
     */
    bool existRemoteFile(bool testFile = true, StopController const& stopController = StopController(), bool emitException = true);

    /**
     * Tests if a remote resource exists, probing for Files and for Directories
     * @param stopController: the stop controller used to stop the remote operation.
     * @param emitException: in case of failures return just not found in case no exception can be emitted
     */
	bool existRemoteResource (bool& isDir, StopController const& sc = StopController(), bool emitException = true);

    /**
     * Tests if the remote resource exists .
     * @return RRTNotExist,RRTIsFile  or RRTIsDir.
     */
    RemoteResourceType probeRemoteResource(std::string &lastErrorMessage, StopController const& stopController = StopController(), bool emitException = false, bool logError = true);

    /*! \brief Upload member function.
     *
     * Using this member function you can perform an upload transfer to the remote resource.
     * The function reads your local file in local path you specify and uploads it to the remote path.
     * Of course it is strictly necessary to set the following members via the appropriate set methods or
     * using the constructor (both ways you are required to pass strings).
     *
     * <ul>
     *  <li> the host name or address of the remote machine (method setHost() )
     *  <li> the user name to access the host (method setUser() )
     *  <li> the password associated with the user (method setPwd() )
     *  <li> the path on the local machine where the file to be read and sent is located (method setLocalPath() )
     *  <li> the name of the file in the locale path you wish to transfer (method setLocalFile() )
     *  <li> the path on the remote machine to wich store the uploaded data (method setRempath() )
     *  <li> optionally specify a name for the file in the remote path, otherwise uploaded data are saved to a
     * file with the same name as the source (method setRemoteFile() ).
     * </ul>
     *
     * Always make sure that you have set all of this in the object you are using if you wish to perform an upload.
     *
     * \param StopController&=StopController() a defaulted const reference to a #StopController, used to manage and perform thread-related stop requests.
     */
    void put(bool alsoRemove, StopController const& stopController = StopController());
    void put(bool alsoRemove, mode_t permissions,StopController const& stopController = StopController());
 
    /*! \brief Upload as tar member function.
     *
     * Using this member function you can perform an upload transfer to the remote resource, storing the tar of the original content.
     * The function reads your local file/directory in local path you specify, tar it "on-the-fly" and uploads the result to the remote path.
     * Of course it is strictly necessary to set the following members via the appropriate set methods or
     * using the constructor (both ways you are required to pass strings).
     *
     * <ul>
     *  <li> the host name or address of the remote machine (method setHost() )
     *  <li> the user name to access the host (method setUser() )
     *  <li> the password associated with the user (method setPwd() )
     *  <li> the path on the local machine where the file to be read and sent is located (method setLocalPath() )
     *  <li> Optionally, the name of the file in the locale path you wish to transfer (method setLocalFile()). Otherwise the whole directory will be sent
     *  <li> the path on the remote machine to wich store the uploaded data (method setRempath() )
     *  <li> optionally specify a name for the file in the remote path, otherwise uploaded data are saved to a
     * file with the same name as the source (method setRemoteFile() ).
     * </ul>
     *
     * Always make sure that you have set all of this in the object you are using if you wish to perform an upload.
     *
     * \param StopController&=StopController() a defaulted const reference to a #StopController, used to manage and perform thread-related stop requests.
     */
   void putAsTar(const File::CompressionMode& cm, const bool encapsulate_dir, const mode_t& permissions, StopController const& stopController = StopController());

    /**
     * Puts a whole directory to the remote site .
     * @param localDirectory: it is the local dir to put to the remote site;
     * @param remoteDirectory: it is the target directory. If empty the remote site path will be used;
     * @param fileFilter : filter to apply to the directory;
     * @param stopController: token used to stop the transfer.
     */
    void putDir(std::string const& localDirectory, std::string const& remoteDirectory="",
             FileFilter fileFilter = FtpCurl::noFileFilter,
            StopController const& stopController = StopController());

    /**
     * Gets a whole directory from the remote site .
     * @param localDirectory: it is the target directory;
     * @param remoteDirectory: it is the remote dir from where to get the files. If empty the RemoteSite path will be used;
     */
    void getDir(std::string const &localTargetDir, std::string const &remoteSourceDir="",
             StopController const& stopController = StopController());

    /**
     * Gets a whole directory from the remote site .
     * @param localDirectory: it is the target directory;
     * @param ftpCurlPerm: The permissions that will be set to each file
     * @param remoteDirectory: it is the remote dir from where to get the files. If empty the RemoteSite path will be used;
     */
    void getDir(std::string const &localTargetDir, const FtpCurlPermissions& ftpCurlPerm, std::string const &remoteSourceDir="",
             StopController const& stopController = StopController());

    void delDirTree(std::string const &remoteDirectory, StopController const& stopController = StopController(), bool logError = true);

    /**
     * Rename remote dir/file.
     */
    void rename(std::string const& remoteItem, std::string const& newRemoteItem, StopController const& stopController = StopController()); // PRQA S 2502

    void chmod(std::string const& remoteItem, mode_t permissions, StopController const& stopController = StopController()); // PRQA S 2502

    /*! \brief Remote file deletion member function.
     *
     * Using this member function you can perform deletions of files located on a remote machine.
     * Of course it is strictly necessary to set the following members via the appropriate set methods or
     * using the constructor (both ways you are required to pass strings).
     *
     * <ul>
     *  <li> the host name or address of the remote machine (method setHost() )
     *  <li> the user name to access the host (method setUser() )
     *  <li> the password associated with the user (method setPwd() )
     *  <li> the path on the remote machine where the file to delete is located (method setRempath() )
     *  <li> the name of the remote file you wish to delete (method setRemoteFile() ).
     * </ul>
     *
     * Always make sure that you have set all of this in the object you are using if you wish to perform an upload.
     *
     * \param StopController&=StopController() a defaulted const reference to a #StopController, used to manage and perform thread-related stop requests.
     */
    void del(StopController const& stopController = StopController());
    /**
     * Delete a remote directory.
     */
    void delDir(const StopController &stopController = StopController());
    /**
     * delete a Local file (local delete).
     */
    void ldel(const std::string &localFile);
    
    std::string httpPost(const curl::Curl::CurlHttpOpt& opt, StopController const& stopController= StopController());
    std::string httpPost(std::string const& uri, std::string const& postMessage, StopController const& stopController= StopController());
    std::string httpPost(std::string const& uri, std::string const& postMessage, const std::string &contentType,  StopController const& stopController= StopController());
    /**
     * \brief Implements http(s) POST method.
     * 
     * Differently from the other httpPost API, this can be used to retrieve the response payload from the server even if the http response code is >= 400.
     * It is very important to know that the function does not throw any exception when the http response code is >= 400. The management of such codes is left to the user.
     * 
     * \param opt: Class containing the options that must be used for the http request.
     * \param responsePayload: Reference to the output string that will be used to store the response payload.
     * \param stopController: StopController object
     * \return The http response code from the server.
     */
    long httpPost(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController= StopController());

    std::string httpGet(const curl::Curl::CurlHttpOpt& opt, StopController const& stopController= StopController());
    std::string httpGet (std::string const& uri, StopController const& stopController= StopController());
    /**
     * \brief Implements http(s) GET method.
     * 
     * Differently from the other httpGet API, this can be used to retrieve the response payload from the server even if the http response code is >= 400.
     * It is very important to know that the function does not throw any exception when the http response code is >= 400. The management of such codes is left to the user.
     * 
     * \param opt: Class containing the options that must be used for the http request.
     * \param responsePayload: Reference to the output string that will be used to store the response payload.
     * \param stopController: StopController object
     * \return The http response code from the server.
     */
    long httpGet(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController= StopController());

    std::string httpPut(const curl::Curl::CurlHttpOpt& opt, StopController const& stopController= StopController());
    /**
     * \brief Implements http(s) PUT method.
     * 
     * Differently from the other httpPut API, this can be used to retrieve the response payload from the server even if the http response code is >= 400.
     * It is very important to know that the function does not throw any exception when the http response code is >= 400. The management of such codes is left to the user.
     * 
     * \param opt: Class containing the options that must be used for the http request.
     * \param responsePayload: Reference to the output string that will be used to store the response payload.
     * \param stopController: StopController object
     * \return The http response code from the server.
     */
    long httpPut(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController= StopController());

    std::string httpDel(const curl::Curl::CurlHttpOpt& opt, StopController const& stopController= StopController());
    /**
     * \brief Implements http(s) DELETE method.
     * 
     * Differently from the other httpDel API, this can be used to retrieve the response payload from the server even if the http response code is >= 400.
     * It is very important to know that the function does not throw any exception when the http response code is >= 400. The management of such codes is left to the user.
     * 
     * \param opt: Class containing the options that must be used for the http request.
     * \param responsePayload: Reference to the output string that will be used to store the response payload.
     * \param stopController: StopController object
     * \return The http response code from the server.
     */
    long httpDel(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController= StopController());

    std::string httpPatch(const curl::Curl::CurlHttpOpt& opt, StopController const& stopController= StopController());
    /**
     * \brief Implements http(s) PATCH method.
     * 
     * Differently from the other httpPatch API, this can be used to retrieve the response payload from the server even if the http response code is >= 400.
     * It is very important to know that the function does not throw any exception when the http response code is >= 400. The management of such codes is left to the user.
     * 
     * \param opt: Class containing the options that must be used for the http request.
     * \param responsePayload: Reference to the output string that will be used to store the response payload.
     * \param stopController: StopController object
     * \return The http response code from the server.
     */
    long httpPatch(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController= StopController());

    /**
     * Implements http(s) PROPFIND method of WebDAV protocol
     */
	 /**
     * Use WebDAV http(s) PROPFIND method to retrieve properties defined on given path of given host (see http://www.webdav.org/specs/rfc4918.html). _site attribute must be set properly in advance.
     * @param uri  : the path to give properties of
     * @param depth : the depth level of requesting properties. Allowed values are 0: just the input path; 1: the contained entries if directory (i.e. collection); -1: infinity. If depth != 0 and uri is directory, contained entries only returned
     * @param output : PropFindStatItem vector with properties of items; name is always filled.
     * @param properties : bit mask corresponding to items properties to retrieve. It must be build with the desired value of PropFindRequests enum set in OR (e.g. PROPFINDREQ_TYPE | PROPFINDREQ_CREATION )
     * @param dirOnly : if set, directories only are returned
     * @param order : ordering criteria. Managed ordering are: by name, creation/modification date
     * @param reverse : if set, reverse ordering
     * @param isDir : if true, it is assumed path is a directory, a simple file otherwise.
     * @param stopController : StopController object
     */

	void propFind( std::string const& uri, int depth,  std::vector<PropFindStatItem> & output, 
					const PropFindRequests & properties = PROPFINDREQ_NONE,
					bool dirOnly = false , 
					const PropFindStatItemOrder & order = PROPFINDITEMS_NONE_ORDER,
					bool reverse = false,
					bool isDir = true,
					StopController const& stopController= StopController() );

    /**
     * No file filter.
     */
    static bool noFileFilter(std::string const& ) { return true; }

    /*! \brief list download member function.*/
    std::string list(const StopController& = StopController());

    void list(std::vector<acs::curl::FtpListItem> & items, StopController const& stopController= StopController());

    /**
     * Interactive list.
     * */
    void list(NextLineCallback nextLineCallback, void* data, StopController const& stopController= StopController());

    /*! \brief A set method for the host name.
     *
     * \param &host a ref to a std::string object containing the host name or address to be set inside the class
     */
    void setHost(const std::string &host);

    /*! \brief A get method for the host name.
     *
     * \return a const std::string object containing the host address or name of the remote
     * machine, it is empty if not defined trough setHost() or parameterized version of FtpCurl() (the class constructor).
     */
    std::string getHost() const;

    /*! \brief A get method for the host name and port (if any).
     *
     * \return a std::string object containing the host address or name of the remote machine and optionally the port,
     * it is empty if not defined trough setHost() or parameterized version of FtpCurl() (the class constructor).
     */
    std::string getHostAndPort() const;

    /*! \brief A set method for the user name.
     *
     * \param &user a ref to a std::string object containing the user name to be set inside the class
     */
    void setUser(const std::string &user);

    /*! \brief A get method for the user name.
     *
     * \return a const std::string object containing the user name to log on the remote
     * machine, it is empty if not defined trough setUser() or parameterized version of FtpCurl() (the class constructor).
     */
    std::string getUser() const;

    /*! \brief A set method for the password.
     *
     * \param &pwd a ref to a std::string object containing the password to be set inside the class
     */
    void setPwd(const std::string &pwd);

    /*! \brief A get method for the user name.
     *
     * \return a const std::string object containing the password corresponding to the user to log on the remote
     * machine, it is empty if not defined trough setPwd() or parameterized version of FtpCurl() (the class constructor).
     */
    std::string getPwd() const;

    /*! \brief A set method for the path on the remote machine.
     * @deprecated : use getFullFilename() and setFullFilename()
     * \param &remotePath ref to a std::string object containing the remote path to be set inside the class
     */
    void setRempath(const std::string &remotePath);

    /*! \brief A get method for the path on the remote machine.
     * @deprecated : use getFullFilename() and setFullFilename()
     * \return a const std::string object containing the path on the remote
     * machine, it is empty if not defined trough setRempath() or parameterized version of FtpCurl() (the class constructor).
     */
    const std::string &getRempath() const noexcept ;
    /**
     * Retrieves the full path and file name (remote resource).
     */
    const std::string &getFullFilename() const noexcept ;
    /**
     * Setters for the full path and file name (remote resource).
     */
    void setFullFilename(const std::string &filename);
    /**
     * Retrieves the full path and file name (local resource).
     */
    std::string getFullLocalFilename() const;
    /**
     * Setters for the full path and file name (local resource).
     */
    void setFullLocalFilename(const std::string &filename);
    /*! \brief A set method for the name of the remote file.
     * @deprecated : use getFullFilename() and setFullFilename()
     * \param &remoteFile a ref to a std::string object containing the name of the remote file to be set inside the class
     */
    void setRemoteFile(const std::string &remoteFile);

    /*! \brief A get method for the name of the remote file.
     * @deprecated : use getFullFilename() and setFullFilename()
     * \return a const std::string object containing the name of the remote file,
     * it is empty if not defined trough setRemoteFile() or parameterized version of FtpCurl() (the class constructor).
     */
    std::string getRemoteFile() const;

    /*!< \brief A set method for the path on the local machine.
     * @deprecated : use getFullLocalFilename() and setFullLocalFilename()
     * \param &localPath a ref to a std::string object containing the local path to be set inside the class
     */
    void setLocalPath(const std::string &);

    /*! \brief A get method for the path on the local machine.
     * @deprecated : use getFullLocalFilename() and setFullLocalFilename()
     * \return a const std::string object containing the path on the local
     * machine, it is empty if not defined trough setLocalPath() or parameterized version of FtpCurl() (the class constructor).
     */
    const std::string &getLocalPath() const noexcept ;

    /*!< \brief A set method for the name of the local file.
     * @deprecated : use getFullLocalFilename() and setFullLocalFilename()
     * \param &localFile a ref to a std::string object containing the name of the local file to be set inside the class
     */
    void setLocalFile(const std::string &localFile);

    /*! \brief A get method for the name of the local file.
     * @deprecated : use getFullLocalFilename() and setFullLocalFilename()
     * \return a const std::string object containing the name of the local file,
     * it is empty if not defined trough setLocalFile() or parameterized version of FtpCurl() (the class constructor).
     */
    const std::string &getLocalFile() const noexcept ;

    void setNextLineCallback(NextLineCallback callBack, void* data);

    NextLineCallback getNextLineCallback() const;

    /**
     * Open a new connection to the acs::curl::Curl internal property.
     * @param skipIfAlreadyCreated: if true and if the internal connection was
     * already created, this method does nothing.
     */
    void openConnection(bool skipIfAlreadyCreated = false);
    /**
     * Close a valid connection.
     */
    void closeConnection();
    /**
     * Rollbacks the current connection;
     */
    void rollbackConnection();

    /*! \brief A method to set passive mode. (default is true)*/
    void setPassiveMode(bool);

    /*! \brief A get method to know if passive mode is set. (default is true)*/
    bool getPassiveMode() const;

    /*! \brief A method to set only file names from list (default false)*/
    void setListFileNamesOnly(bool) noexcept;

    /*! \brief A get method to know if only file names from list. (default is false)*/
    bool getListFileNamesOnly() const noexcept ;

    /*! \brief Set the protocol of the underline RemoteSite to SFTP. */
    void setSFTP();

    /*! \brief Check if the protocol of the underline RemoteSite is SFTP.  */
    bool isSFTP() const;

    /*! \brief A set method for ftps.  */
    void setFtps();

    /*! \brief A set method for ftpes.  */
    void setFtpes();

    /*! \brief A set method for ftp.  */
    void setFtp();

    /*! \brief A get method for ftps  */
    bool getFtps() const noexcept ;

    /*! \brief A get method for ftpes  */
    bool getFtpes() const noexcept;

    /*! \brief A get method for ftp  */
    bool getFtp() const noexcept;

    const curl::RemoteSite &getSite() const noexcept ;

    void setSite(curl::RemoteSite const& site);

    void getStats(std::vector<curl::CirculationStatistics>& stats);
    
    long getHttpResponseCode() const;
    
    /**
     * ToString method.
     */
    std::string str() const;
	
	void checkCURLcode(CURLcode res, const std::string &msg, char const* function, int line) const;

    /*!
     * \brief .
     */
    void passiveActiveDiscover();

	/*
	 * Self test for tar/untar on-the-fly
	 */
	static bool self_test__compare_from_file(const std::string& s, const std::string& filename) ;
	static std::string self_test__read_from_file(const std::string& filename) ;
	static void self_test__write_to_file(const std::string& s, const std::string& filename) ;
	static int self_test__tar_untar(const curl::RemoteSite& remote_site) ;


#ifdef ACS_TEST
public:
#else
private:
#endif
    /**
     * Create a new Curl handle if it was not already created.
     */
    curl::Curl& createCurl();
	
	/*!
	 * \brief Handles the input curl error according to the default/configured behaviour associated to it.
	 **/
	static void __attribute__((noreturn)) handleCurlErrorBehaviour(CurlErrorBehaviourSingleton::CurlErrorBehaviour beh, const std::string& msg, const std::string& file, const std::string& func, int line);

    /*!
     * \brief Throw particular exception depending on curl code.
     **/
    static void throwCurlSpecificException(std::string const &, CURLcode, std::string const &, int);

    static void produceStringLine(FtpCurl*);

    /*!
     * \brief Stores data to be passed to default built-in progress callback.
     *
     * libCurl permits to define a custom callback to perform progress-related operations. The library allows to
     * pass to the callback a void* to some user-defined progress-related data to be used inside the callback.
     * Since the callback can be used to perform a stop request issued trough a StopController (when the callback returns non-zero values
     * any transfer in progress is aborted) we need to pass this object.
     * Along with a reference to the current StopController the struct can also contain a pointer to an user defined callback, the typedef
     * ProgressCallback.
     *
     * Please keep in mind that this struct is passed to the default built-inprogress callback, the custom user-defined one, if exists, is called only
     * within the former.
     *
     **/

    /*
     * follows the callbacks, for more detailed info consult the libCurl library documentation
     * at CURL_SET_OPT.
     *
     */
    static size_t readLine_callback(void *buffer, size_t size, size_t nmemb, void *data);

	static std::string generate_compression_ext(const File::CompressionMode& cm) ; 

    void performance(double bytes, double total);
    void throwConditionedError(CURLcode res, const std::string &url, char const* function, int line) const ;
    void fillStats(bool isFromURL) ;
    void fillGetStats() { fillStats(true) ; }
    void fillPutStats() { fillStats(false) ; }

    /*! \brief Spawning exename.
     * \return error code of exename 
     */
	int spawn(const std::string & exeName, const WaitPid::ProcArgs &, int timeout, bool & goodExit, std::string & spawnArgs) const ;

	void fillFtpListReader(const std::string & filepath, curl::FtpListReader &) const;
	
	void fillPropFindStatItems( std::string const& uri, int depth, const std::string & xmlIn, const std::vector<std::string> & properties, bool directoriesOnly, bool isDir, std::vector<PropFindStatItem> & ) const;
	
	void fillPropFindStatItemField( const rsResourceSet &, const std::vector<std::string> & properties, PropFindStatItem & ) const ;
	
	std::string buildHttpPropFindBody( const std::vector<std::string> & properties ) const ;
	
	void propFind( std::string const& uri, int depth, const std::vector<std::string> & properties, bool directoriesOnly, bool isDir, std::vector<PropFindStatItem> &, StopController const& stopController );

	std::vector<std::string> turnRequestsIntoProperties(const PropFindRequests &) const ;
	
	long httpRequest(const curl::Curl::CurlHttpOpt& opt, const Operation& op, std::string& responsePayload, bool failOnError, StopController const& stopController= StopController());
	
#ifdef ACS_TEST
public:
#else
private: // NOSONAR - for clarity
#endif
    void initializeCurl(Operation, StopController const& stopController);
    void finalizeCurl(CURLcode res, std::string const& target);
    void setOperation(Operation);
    void verifyRemoteSite(bool discover=false);
	
	// Set the attribute VerifyCertificate for the site (use the configuration, /see mustVerifyHostCertificate)
	// Returns true if the hostname was found in configuration, otherwise false
    bool setupVerifyHostCertificate(const std::string& hostname);
    
    // manage active/passive overrides by configuration
    // returns true if the override exists and stores it in the isPassive parameter
    bool mustOverrideFTPMode ( bool& isPassive ) const ;

    // manage verify/not_verify host certificate by configuration
    // returns true if the host was found ("to_verify" is filled).
	// if host not found, return false
    bool mustVerifyHostCertificate (const std::string& thehost, std::map<std::string, std::string>& parameters_M) const ; // NOSONAR - map transparent comp 

	// Load the configuration. Returns true if the configuration has been loaded now, false if it was already loaded
	static bool loadConfigurationOnce() ;

	// Get tar block factor
	static size_t get_tar_block_factor() { loadConfigurationOnce() ; return _tar_block_factor ; }
	
	// Get tar block factor threshold 
	static double get_tar_block_factor_threshold() { loadConfigurationOnce() ; return _tar_block_factor_threshold ; }


#ifdef ACS_TEST
public:
#else
private: // NOSONAR - for clarity 
#endif

    const curl::BandPriority &band_priority_ ;
    curl::RemoteSite _site;
    std::string _localPath = "" ;
    std::string _localFile = "" ;

    /**
     * @brief if true only file names are listed without their attributes.
     * This flag is valid both for ftp and for sftp mode.
     */
    bool _listFileNamesOnly = false ;

    NextLineCallback _nextLineCallback = nullptr ;

    /**
     * @brief temporary status variables
     */
    curl::FtpPerformance _perfo = {} ;

    std::string _currentLine = "" ;

    void* _data = nullptr ; // NOSONAR callback

    std::unique_ptr<curl::Curl> _curl = { nullptr };

    std::unique_ptr<curl::CurlTransaction> _transaction = { nullptr } ;

    std::shared_ptr<curl::FtpProgressData> _handleProgressData = { nullptr } ;
    std::shared_ptr<curl::HttpProgressData> _handleHttpProgressData= { nullptr } ;
    std::vector<curl::CirculationStatistics> _statistics = {} ;

    static std::map<std::string, acs::curl::RemoteSite, std::less<>> _sites ;
    static std::vector<std::string> _inDiscover;
    static Condition _discoverCondition;
    
    // FTP mode override configuration container
    static std::map<std::string, bool, std::less<>> _passiveFtpSiteOverrides;
    // Verification of host certificates.
    // A nested map is used. Outer map: hostname, parameters_map
    // Inner map: parameter key/values. The parameters are listed in: Curl::Confkey_verifycert_
    static std::map<std::string, std::map<std::string, std::string>, std::less<>> _verifyHostCertificate ; // NOSONAR - map transparent comp
    static Mutex _overridesMutex;
    static bool _config_loaded_OverrideFTPMode;
    static bool _config_loaded ;
    static const bool _default_VerifyHostCertificate ; // Value used in case of mismatch of: mustVerifyHostCertificate
    static size_t _tar_block_factor ;
    static double _tar_block_factor_threshold ;
    static const size_t _default_tar_block_factor ; 

    ACS_CLASS_DECLARE_DEBUG_LEVEL(FtpCurl) ;

};

std::ostream &operator<<(std::ostream &, const FtpCurl::PropFindStatItem &) ;

acs::exostream &operator<<(acs::exostream &, const FtpCurl::PropFindStatItem &) ;

_ACS_END_NAMESPACE

#endif // _FtpCurl_H_
