// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.3  2013/10/24 18:28:00  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:21  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.2  2013/05/29 10:53:59  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.3  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.2  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.1  2011/04/05 17:15:09  micmaz
 *  *** empty log message ***
 *
 *
 *
 *
 */

#ifndef _FtpPerformance_H_
#define _FtpPerformance_H_

#include <RemoteSite.h>
#include <Timer.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;


/**
 * @brief Typedef of a pointer to user-defined transfer-progress callback function.
 *
 * The function, if defined by the user, is called by the progress_callback private method wich passes to it the four arguments.
 * By accessing these arguments one is able to get the current status of the transfer.
 *
 * @param dltotal total amount of data to be downloaded
 * @param dlnow current amount of data downloaded
 * @param ultotal amount of data to be uploaded
 * @param ulnow current amount of data uploaded
 *
 * Please note that the progress_callback private method has the form of the default progress callback to be used with libCurl and is always defined and used;
 * the function herein discussed is called within the default callback and provides a way to introduced custom behaviors.
 * To let this you define your custom callback in your source, for example you could state
 *
 * \code
 *  ...
 *  static void custom_progress_callback(double dltotal, double dlnow, double ultotal, double ulnow)
 *  {
 *  ...
 *  }
 *  ...
 * \endcode
 *
 * then when you instantiate an FtpCurl, say for example called FtpObject, you can use the proper set method to allow the object to use
 * your ad-hoc defined callback:
 *
 * \code
 *  ...
 *  FtpObject.setProgressCallback(custom_progress_callback)
 *  ...
 * \endcode
 *
 */
class FtpPerformance { // PRQA S 2109
public:
    FtpPerformance() = default ;

    FtpPerformance(const FtpPerformance &rhs) = delete ;
    FtpPerformance& operator=(FtpPerformance const & rhs) = delete ;

    explicit FtpPerformance(const curl::RemoteSite& site);

    void start();
    void stop();
    void progress(double transf, double total);
    int percentage() const noexcept ;
    std::string dump() const ;

    void setRemoteSite(const curl::RemoteSite& site) ;
    const curl::RemoteSite &getRemoteSite() const noexcept ;
    double getTotalBytes() const noexcept ;
    void setTotalBytes(double) noexcept ;
    double getTransfBytes() const noexcept ;
    void setTransfBytes(double) noexcept ;
    double getSpeedKbSec() const noexcept ;
    void setSpeedKbSec(double speed) noexcept ;
    double getRemainingSec() const noexcept ;
    void setRemainingSec(double sec) noexcept ;
    const std::string &getOperation() const noexcept ;
    void setOperation(std::string const &operation) ;

private:
    double _totalBytes = 0.0 ;
    double _transfBytes = 0.0 ;
    double _speedKbSec  = 0.0 ; // KB/sec
    double _remainingSec  = 0.0 ; // sec
    acs::Timer _timer = {} ;
    std::string _operation = "" ;
    curl::RemoteSite _site = curl::RemoteSite( "ftp" ) ;
    bool _running = false ; 
    ACS_CLASS_DECLARE_DEBUG_LEVEL(FtpPerformance) ;
};

_ACS_END_NAMESPACE // curl namespace 

    using ProgressCallback = void (*)(curl::FtpPerformance const& p);

_ACS_END_NAMESPACE // acs namespace 

#endif /* _FtpPerformance_H_ */
