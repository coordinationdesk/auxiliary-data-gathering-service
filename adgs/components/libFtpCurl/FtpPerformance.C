// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.3  2013/10/24 18:27:59  marpas
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
 *  Revision 2.4  2013/04/27 14:46:27  marpas
 *  compilation warnings
 *
 *  Revision 2.3  2012/03/08 14:22:46  micmaz
 *  updated tests to cppunit
 *
 *  Revision 2.2  2012/03/08 13:11:40  micmaz
 *  added media functionality
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
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

#include <FtpPerformance.h>
#include <FtpCurlConstants.h>
#include <Curl.h>
#include <CurlStatistics.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

using namespace std;
ACS_CLASS_DEFINE_DEBUG_LEVEL(FtpPerformance);


FtpPerformance::FtpPerformance() :
    _totalBytes(),
    _transfBytes(),
    _speedKbSec(),
    _remainingSec(),
    _timer(),
    _operation(),
    _site("ftp"),
    _running()
{
    //empty.
}

FtpPerformance::FtpPerformance(curl::RemoteSite& site) : 
    _totalBytes(),
    _transfBytes(),
    _speedKbSec(),
    _remainingSec(),
    _timer(),
    _operation(),
    _site(site),
    _running() 
{
    //empty.
}

FtpPerformance::~FtpPerformance() throw () 
{
}

void FtpPerformance::start() {
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB);
    _totalBytes = 0;
    _transfBytes = 0;
    _timer.start();
    _running = true ;
}

void FtpPerformance::stop() {
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "is running " << boolalpha << _running);
    if (_running) {
        progress(_totalBytes, _totalBytes);

        string strPath = CurlStatistics::path(_site, _operation);
        strPath += "/performance/";

        string speedPath = strPath + "/speed(KB&#47;sec)";
        CurlStatistics::totalAvrg(speedPath, _speedKbSec);

        string bytesPath = strPath + "/Kbytes";
        CurlStatistics::totalAvrg(bytesPath, _totalBytes / double(Constants::Kilobyte)); // PRQA S 3081
        
        _running = false ;
    }
}

void FtpPerformance::progress(double transf, double total) 
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "is running " << boolalpha << _running << '(' << transf << '/' << total << ')');
    if (_running) {
        _transfBytes = transf;
        if (total > 0) {
            _totalBytes = total;
        }
        else {
            _totalBytes = _transfBytes ; 
        }
        _timer.stop();
        _speedKbSec = (_transfBytes / double(Constants::Kilobyte)) / (double(_timer.elapsed()) / 1000); // PRQA S 3081, 4400 2
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_speedKbSec: " << _speedKbSec);
        _remainingSec = ((_totalBytes - _transfBytes) / double(Constants::Kilobyte)) / _speedKbSec;
    }
}

int FtpPerformance::percentage() const throw() // PRQA S 4020
{
    if (0 != _totalBytes) { // PRQA S 3270
        return _transfBytes * 100 / _totalBytes ; // PRQA S 4400 // PRQA S 3011
    }
    return 0;
}

double FtpPerformance::getSpeedKbSec() const throw() // PRQA S 4120
{
    return _speedKbSec;
}

void FtpPerformance::setSpeedKbSec(double speed) throw() // PRQA S 4121
{
    _speedKbSec = speed;
}

double FtpPerformance::getRemainingSec() const throw() // PRQA S 4120
{
    return _remainingSec ;
}

void FtpPerformance::setRemainingSec(double sec)  throw() // PRQA S 4121
{
    _remainingSec = sec;
}

double FtpPerformance::getTransfBytes() const throw()  // PRQA S 4120
{
    return _transfBytes;
}

void FtpPerformance::setTransfBytes(double transferred) throw()  // PRQA S 4121
{
    _transfBytes = transferred;
}

double FtpPerformance::getTotalBytes() const throw() // PRQA S 4120
{
    return _totalBytes;
}

void FtpPerformance::setTotalBytes(double totalBytes) throw()  // PRQA S 4121
{
    _totalBytes = totalBytes;
}

void FtpPerformance::setOperation(const string &operation) // PRQA S 4121
{
    _operation= operation;
}

const string &FtpPerformance::getOperation() const throw()  // PRQA S 4120
{
    return _operation;
}

void FtpPerformance::setRemoteSite(const curl::RemoteSite& site) // PRQA S 4121
{
    _site = site;
}

const curl::RemoteSite &FtpPerformance::getRemoteSite() const throw() // PRQA S 4120
{
    return _site;
}

string FtpPerformance::dump() const {
    ostringstream os;
    if (_speedKbSec > Constants::Kilobyte) {
        os << " Speed: " << int ( _speedKbSec / 102.4) / 10.0 << " [MB/sec] "; // PRQA S 3011, 3081, 4400
    } 
    else {
        os << " Speed: " << int( _speedKbSec ) << " [KB/sec] "; // PRQA S 3081
    }
    os << " Percentage: " << percentage() << " % ";
    os << " Remaining: " << getRemainingSec() << " [sec] ";
    return os.str() ;
}

_ACS_END_NESTED_NAMESPACE
