// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2015, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. libDataProvider Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.7  2017/02/23 16:36:58  nicvac
 *  S3EVOL-13: Distribution working area is not related anymore to download cache.
 *
 *  Revision 5.6  2017/02/21 17:02:44  nicvac
 *  S3EVOL-13: Inter process cache mode
 *
 *  Revision 5.5  2016/12/16 16:55:45  nicvac
 *  S3MPM-860: PackageExporter resources improvement.
 *
 *  Revision 5.4  2015/05/28 09:33:31  marpas
 *  useless parameters no more read from configuration (BufrExtension, BufrConverter, CompressScriptName)
 *  coding best practice applied
 *
 *  Revision 5.3  2014/06/05 18:47:12  marpas
 *  getting rid of asserts
 *
 *  Revision 5.2  2013/11/04 13:56:31  marpas
 *  coding best practices applied
 *  robustness improved
 *  performances improved
 *  compilation & qa warnings removed
 *
 *  Revision 5.1  2013/10/28 17:34:09  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:51:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.12  2013/05/17 15:33:01  nicvac
 *  S2PDGS-300: priority handling.
 *
 *  Revision 1.11  2013/05/02 09:31:31  danalt
 *  using exFPrivateInfo for some info, missing BufrExtension and BufrConverter in confspace declassed to Warning
 *
 *  Revision 1.10  2013/01/25 13:34:27  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.9  2012/12/07 09:52:38  nicvac
 *  skip delivery slip option is false by default.
 *
 *  Revision 1.8  2012/12/06 18:33:37  nicvac
 *  Delivery Slip can be skipped by configuration.
 *
 *  Revision 1.7  2012/12/05 10:33:38  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 1.6  2012/11/05 13:38:54  micmaz
 *  all data provider configuration parameters are now optional
 *
 *
 *
 */

#include <DataProviderConfigurer.h>

#include <FtpCurlConstants.h>

#include <ConfigurationSingleton.h>
#include <StringUtils.h>

#include <File.h>

#include <rsPushNSpace.h>

#include <DateTime.h>
#include <StringUtils.h>
#include <Filterables.h>



#include <boost/algorithm/string.hpp> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)
using namespace std;

bool DataProviderConfigurer::_onceForRun = true;
const string DataProviderConfigurer::spaceName = "libDataProvider.so";
const string DataProviderConfigurer::defaultProductPath = "/raid0/opcryosat/ProductsPath";
const string DataProviderConfigurer::defaultDownloadPath = "/raid0/opcryosat/downloadspace";

const bool DataProviderConfigurer::defaultLowerPriorityFirst = true;


void DataProviderConfigurer::confLoadedCallback(void *cp) // PRQA S 4020
{
    if (!onceForRun() || !cp) {
        return;
    }
    setOnceForRun(false);
    DataProviderConfigurer* c = reinterpret_cast<DataProviderConfigurer *>(cp); // PRQA S 3081
    c->load();
    c->dump();
}

ACS_CLASS_DEFINE_DEBUG_LEVEL (DataProviderConfigurer);

DataProviderConfigurer::DataProviderConfigurer():
	_productPath(defaultProductPath), 
    _maxDBRetry(defaultMaxDBRetry), 
    _lowerPriorityFirst(defaultLowerPriorityFirst),
    _downloadRepository(defaultDownloadPath), 
    _availableDownloadSpaceSize(defaultDownloadSpaceSize)
{
    ConfigurationSingleton::instance()->addCallback(confLoadedCallback, this);
}

DataProviderConfigurer::~DataProviderConfigurer() throw () {
    // empty
}
void DataProviderConfigurer::load() {
    rsResourceSet& resultSet = ConfigurationSingleton::instance()->get();
    if (resultSet.hasKey("maxDbRetries")) {
        resultSet.getValue("maxDbRetries", _maxDBRetry);
    }
    rsPushNSpace rs(resultSet, "DataProvider");
    {
        if (resultSet.hasKey("ProductPath")) {
            resultSet.getValue("ProductPath", _productPath);
        }
		if (resultSet.hasKey("LowerPriorityFirst")) {
			resultSet.getValue("LowerPriorityFirst", _lowerPriorityFirst);
		}

    } // end NameSpace DownloadPackData

    if (resultSet.hasKey("Download")) {
        rsPushNSpace download(resultSet, "Download");
        if (resultSet.hasKey("DownloadPath")) {
            resultSet.getValue("DownloadPath", _downloadRepository);
        }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Download Path: " << _downloadRepository);

        if (resultSet.hasKey("SpaceSize")) {
            resultSet.getValue("SpaceSize", _availableDownloadSpaceSize);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Download Space size :" << _availableDownloadSpaceSize << " Megabytes.");
            _availableDownloadSpaceSize = _availableDownloadSpaceSize * acs::curl::Constants::Megabyte; //converted from Megabytes to bytes. // PRQA S 3084
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Download Space size not found in configuration file(s), using default.");
        }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Download Space size :" << _availableDownloadSpaceSize << " bytes.");


    }
    if (!File::exists(_downloadRepository)) {
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,  " download repository " << _downloadRepository<< " does not exists! Going to make it.");
        try {
            File::mkdir(_downloadRepository);
        } catch (exception const& ex) {
            ACS_LOG_WARNING("Cannot create the download repository folder: " << _downloadRepository);
            ACS_LOG_WARNING(
                    "Please, if you are going to import products, stop this agent, correct the problem and restart.");
            ACS_LOG_WARNING("if you are not going to import products, ignore this warning.");
        }
    }
}

void DataProviderConfigurer::dump() const 
{
    // PRQA S 4400 L1
    ostringstream os;
    os << "\n";
    os << left << setw(50) << "--------------------------------------------  " << "\n";
    os << left << setw(50) << "Data Provider configuration status            " << "\n";
    os << left << setw(50) << "--------------------------------------------  " << "\n";
    os << left << setw(50) << "Download repository path:" << right << setw(30)
            << _downloadRepository << "\n";
    os << left << setw(50) << "Download repository size (bytes):" << right << setw(30)
            << _availableDownloadSpaceSize << "\n";
    os << left << setw(50) << "                    (mega bytes):" << right << setw(30)
            << _availableDownloadSpaceSize / acs::curl::Constants::Megabyte << "\n"; // PRQA S 3084
    os << left << setw(50) << "                    (giga bytes):" << right << setw(30)
            << _availableDownloadSpaceSize / acs::curl::Constants::Gigabyte << "\n"; // PRQA S 3084
    os << left << setw(50) << "Product path:" << right << setw(30) << _productPath << "\n";
    os << left << setw(50) << "Max DB Retries:" << right << setw(30) << _maxDBRetry << "\n";
	os << left << setw(50) << "Lower Priority First:" << right << setw(30) << _lowerPriorityFirst << "\n";
    os << left << setw(50) << "--------------------------------------------  " << "\n";
    ACS_LOG_PRIVINFO(os.str()) ;
// PRQA L:L1
}

const string & DataProviderConfigurer::getDownloadRepository() const throw() // PRQA S 4120
{
    return _downloadRepository;
}

void DataProviderConfigurer::setDownloadRepository(string const& repositoryPath)  // PRQA S 4121
{
    _downloadRepository = repositoryPath;
}

unsigned long DataProviderConfigurer::getDownloadSpaceSize() const throw() // PRQA S 4120
{
    return _availableDownloadSpaceSize;
}

void DataProviderConfigurer::setDownloadSpaceSize(unsigned long size) throw() // PRQA S 4121
{
    _availableDownloadSpaceSize = size;
}


const string & DataProviderConfigurer::getProductPath() const throw() // PRQA S 4120
{
    return _productPath;
}


size_t DataProviderConfigurer::getMaxDBRetry() const  throw() // PRQA S 4120
{
    return _maxDBRetry;
}


bool DataProviderConfigurer::getLowerPriorityFirst() const throw() // PRQA S 4120 
{
    return _lowerPriorityFirst;
}



namespace {
    class DataProviderConfigurerLoader {
    public:
        DataProviderConfigurerLoader() {
            DPConfigurerSingleton::instance();
        }
    } _DataProviderConfigurerLoader;
}

_ACS_END_NESTED_NAMESPACE
