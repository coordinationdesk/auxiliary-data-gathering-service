/*
Copyright 1995-2006, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.
*/

# include "pyepfnV.h"
using namespace acs;
static pyepfnV Version;

# include <string>
static std::string __revision__("$Id$");

# include "ESAProductFileName.h"
# include "pySgiIncludes.h"
# include <exStream.h>
# include <crcmodel.h>
# include <boost/python.hpp>




class pyESAProductFileNameBadReturnValueException : public std::exception {
    std::string _why;
public:
    pyESAProductFileNameBadReturnValueException(std::string const & why) : _why(why) {}
    virtual ~pyESAProductFileNameBadReturnValueException() throw() {}
};

class pyESAProductFileNameOutOfBufferException : public std::exception {
    std::string _why;
public:
    pyESAProductFileNameOutOfBufferException(std::string const & why) : _why(why) {}
    virtual ~pyESAProductFileNameOutOfBufferException() throw() {}
};




class pyESAProductFileName {
private:
    int doubleToSerial(char s[1024], double d) const ;
    int serialToDouble(double*d, char s[1024]) const ;
public:
    std::string doubleToStringSerial(double x) const;
    double stringSerialToDouble(std::string s) const ;
    std::string GetFileTypeName_DEPRECATED(const char * filetypename, const char * basePath) ;
    std::string GetFiletypeFromKeyword(const char * keyword, const char * basepath) ;
    std::string GetKeywordFromFiletype(const char * filetype, const char * basepath) ;
    std::string GetName(const char* satelliteName, const unsigned char missionNum, const char* filetype, const double validityStartTime, const double validityStopTime, const unsigned char originatingFacility, const unsigned int absoluteOrbitNum, const int safeManifestCrc, const char*fileExtension, const char* basePath) const;
    std::string GetNameFromTimesAsStrings(const char* satelliteName, const unsigned char missionNum, const char* filetype, const char* validityStartTime, const char* validityStopTime, const unsigned char originatingFacility, const unsigned int absoluteOrbitNum, const int safeManifestCrc, const char*fileExtension, const char* basePath) const;
    std::string GetNameFromStringSerials(const char* satelliteName, const unsigned char missionNum, const char* filetype, const char* validityStartTimeString, const char* validityStopTimeString, const unsigned char originatingFacility, const unsigned int absoluteOrbitNum, const int safeManifestCrc, const char*fileExtension, const char* basePath) const ;
    std::string GetNameFromStringSerials_ComputeCRC(const char* satelliteName, const unsigned char missionNum, const char* filetype, const char* validityStartTimeString, const char* validityStopTimeString, const unsigned char originatingFacility, const unsigned int absoluteOrbitNum, const char * fileToComputeCRCFrom, const signed long long int expSize, const char*fileExtension, const char* basePath) const ;
    std::string GetNameFromTimesAsStrings_ComputeCRC(const char* satelliteName, const unsigned char missionNum, const char* filetype, const char* validityStartTimeString, const char* validityStopTimeString, const unsigned char originatingFacility, const unsigned int absoluteOrbitNum, const char * fileToComputeCRCFrom, const signed int expSize, const char*fileExtension, const char* basePath) const ;
    double DateToJD50(int year, int month, int day, int hour, int min, int sec, int msec ) const ;
    std::string getParamsToStringSerials(std::string const & fileName, std::string const & basePath="") ;
    std::string getParams(std::string const & fileName, std::string const & basePath="") ;
#ifdef UNDEFINED_SINCE_MODIFICATIONS_BROUGHT_TO_PARENT_CC_LIBRARY
    int GetCRC(std::string const & filename) const ;
    std::string GetNameFromStatistics(pySgi_Data_Transcription_Statistic const & somestats) ;
#endif
  std::string GetNameFromStatistics_ComputeCRC(pySgi_Data_Transcription_Statistic const & somestats, std::string const & filename) ;
};


int                                                 /* OUT: 0 -> success */  
pyESAProductFileName::doubleToSerial(char s[1024],  /* OUT: serial, 0-terminated  */
                                     double d)      /*IN: input number */
const {
    int i,k ;
    unsigned short int l=sizeof(d) ;
    char tmp[3] ;
    unsigned char* p = (unsigned char*) &d;
    memset(s, 0, 1024);

    for (i=0; i<l; i++) {
        k=i; /* could be used to swap endianness - unimplemented */
        sprintf(tmp, "%02x", (unsigned int)(p[k]) );
        memcpy(s+(2*i), tmp, 2) ;
    }

    return 0 ;
}

int                                                 /* OUT: 0 -> success */ 
    pyESAProductFileName::serialToDouble(double* d,     /* OUT: output number */
    char s[1024])   /* IN:  serial, 0-terminated  */
const {
    int i, k ;
    unsigned short int l=sizeof(*d) ;
    char tmp[3] ;
    unsigned char nc, *p = (unsigned char*) d;
    unsigned int n ;

    if (strlen(s) != size_t(2*l))
        return -1 ;	/* input is corrupted */

    for (i=0; i<l; i++) {
        memcpy(tmp, s+(2*i), 2); tmp[2]=0;
        sscanf(tmp, "%02x", &n);
        nc = (unsigned char) n;
        k=i; /* could be used to swap endianness - unimplemented */
        memcpy(p+k, &nc, 1);
    }

    return 0 ;
}

std::string pyESAProductFileName::doubleToStringSerial(double x) const {
    char out[1024];
    assert(0==doubleToSerial(&out[0], x));
    return std::string(out);
}

double pyESAProductFileName::stringSerialToDouble(std::string s) const {
    double out;
    char in[1024];
    assert(strncpy(&in[0], s.c_str(), 1024));
    assert(0==serialToDouble(&out, in));
    return out;
}

/*/@@@ nicvac
std::string pyESAProductFileName::GetFileTypeName_DEPRECATED(const char * filetypename, const char * basePath){
    char filetypekey[512];
    ACS_ESAProductFileName_GetFiletypeName(filetypename,basePath,filetypekey);
    return std::string(filetypekey);
}
//*/

std::string pyESAProductFileName::GetFiletypeFromKeyword(const char * keyword, const char * basepath) {
    char filetype[11];
    ACS_ESAProductFileName_GetFiletypeFromKeyword(keyword, basepath, filetype);
    return std::string(filetype);
} 

std::string pyESAProductFileName::GetKeywordFromFiletype(const char * filetype, const char * basepath) {
    char internalFiletype[11]={0,0,0, 0,0,0, 0,0,0, 0,0};
    assert(strlen(filetype)<=10);
    assert(strncpy(internalFiletype, filetype, 11));
    char keyword[4096];
    ACS_ESAProductFileName_GetKeywordFromFiletype(internalFiletype, basepath, keyword);
    return std::string(keyword);
}

std::string 
    pyESAProductFileName::GetName(const char* satelliteName,              /* IN: Satellite ACS name (key in private conf file) */
    const unsigned char missionNum,         /* IN: Mission number */
    const char* filetype,                   /* IN: ACS code for the filetype (key in private conf file) */
    const double validityStartTime,         /* IN: Validity Start (JD50) */
    const double validityStopTime,          /* IN: Validity Stop (JD50) */
    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from acsStations.h) */
    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
    const int safeManifestCrc,              /* IN: Optional: CRC , -1 if not used */
    const char *fileExtension,              /* IN: File extension */
    const char* basePath)                   /* IN: Optional base path for conf files. Otherwise default is /usr/acs/conf/local/ */
const {
    std::cerr << "pyESAProductFileName.GetName(): times = " 
        << std::setprecision(17) << validityStartTime << " " 
        << validityStopTime << std::endl;

    char internalFileExtension[5]={1,2,3,4,0};
    assert(strlen(fileExtension)<=4);
    assert(strncpy(internalFileExtension, fileExtension, 4));
    char out[70];
    int status = ACS_ESAProductFileName_GetName(satelliteName,
        missionNum,
        filetype,
        validityStartTime,
        validityStopTime,
        originatingFacility,
        absoluteOrbitNum,
        safeManifestCrc,
        internalFileExtension,
        basePath,
        out);
    if (not(status==0))
        throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetName()");

    return std::string(out);
}


std::string 
    pyESAProductFileName::GetNameFromTimesAsStrings(const char* satelliteName,              /* IN: Satellite ACS name (key in private conf file) */
    const unsigned char missionNum,         /* IN: Mission number */
    const char* filetype,                   /* IN: ACS code for the filetype (key in private conf file) */
    const char* validityStartTime,         /* IN: Validity Start (JD50) */
    const char* validityStopTime,          /* IN: Validity Stop (JD50) */
    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from acsStations.h) */
    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
    const int safeManifestCrc,              /* IN: Optional: CRC , -1 if not used */
    const char*fileExtension,               /* IN: File extension */
    const char* basePath)                   /* IN: Optional base path for conf files. Otherwise default is /usr/acs/conf/local/ */
const {
    std::cerr << "pyESAProductFileName.GetName(): times = " 
        << std::setprecision(17) << validityStartTime << " " 
        << validityStopTime << std::endl;

    char internalFileExtension[5]={1,2,3,4,0};
    assert(strlen(fileExtension)<=4);
    assert(strncpy(internalFileExtension, fileExtension, 4));
    char out[70];
    int status = ACS_ESAProductFileName_GetName(satelliteName,
        missionNum,
        filetype,
        atof(validityStartTime),
        atof(validityStopTime),
        originatingFacility,
        absoluteOrbitNum,
        safeManifestCrc,
        internalFileExtension,
        basePath,
        out);
    if (not(status==0))
        throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetName()");

    return std::string(out);
}

std::string 
pyESAProductFileName::GetNameFromStringSerials(const char* satelliteName,              /* IN: Satellite ACS name (key in private conf file) */
    const unsigned char missionNum,         /* IN: Mission number */
    const char* filetype,                   /* IN: ACS code for the filetype (key in private conf file) */
    const char* validityStartTimeString,    /* IN: Validity Start (JD50) */
    const char* validityStopTimeString,     /* IN: Validity Stop (JD50) */
    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from acsStations.h) */
    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
    const int safeManifestCrc,              /* IN: Optional: CRC , -1 if not used */
    const char*fileExtension,            /* IN: File extension */
    const char* basePath)                   /* IN: Optional base path for conf files. Otherwise default is /usr/acs/conf/local/ */
const {
    char internalFileExtension[5]={1,2,3,4,0};
    assert(strlen(fileExtension)<=4);
    assert(strncpy(internalFileExtension, fileExtension, 4));
    char out[70];
    double validityStartTime = stringSerialToDouble(validityStartTimeString);
    double validityStopTime = stringSerialToDouble(validityStopTimeString);
    int status = ACS_ESAProductFileName_GetName(satelliteName,
        missionNum,
        filetype,
        validityStartTime,
        validityStopTime,
        originatingFacility,
        absoluteOrbitNum,
        safeManifestCrc,
        internalFileExtension,
        basePath,
        out);
    if (not(status==0))
        throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetName()");
    return std::string(out);
}

double pyESAProductFileName::DateToJD50(int year, int month, int day, int hour, int min, int sec, int msec ) const {
    double out = ACS_ESAProductFileName_DateToJD50(year,month,day,hour,min,sec,msec) ;
    return out;
}

std::string pyESAProductFileName::getParamsToStringSerials(std::string const & fileName, std::string const & basePath) {
    char satelliteName[128], fileType[128], fileExtension[5]  ;
    unsigned char missionNum, originatingFacility ;
    int status, safeManifestCrc; 
    unsigned int absoluteOrbitNum ;
    double validityStartTime=0, validityStopTime=0;

    if (basePath!="")
        status = ACS_ESAProductFileName_GetParams(fileName.c_str(), 
        basePath.c_str(), 
        satelliteName, 
        &missionNum, 
        fileType, 
        &validityStartTime, 
        &validityStopTime, 
        &originatingFacility, 
        &absoluteOrbitNum, 
        &safeManifestCrc, 
        fileExtension);
    else
        status = ACS_ESAProductFileName_GetParams(fileName.c_str(), 
        0, 
        satelliteName, 
        &missionNum, 
        fileType, 
        &validityStartTime, 
        &validityStopTime, 
        &originatingFacility, 
        &absoluteOrbitNum, 
        &safeManifestCrc, 
        fileExtension);

    if (not(status==0))
        throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetName()");

    char out[1024];
    if (1024<snprintf(out, 1024, "%s\\%s\\%s\\%d\\%s\\%s\\%s\\%d\\%d\\%d\\%s", 
        fileName.c_str(), basePath.c_str(), 
        satelliteName, int(missionNum), fileType, 
        doubleToStringSerial(validityStartTime).c_str(), 
        doubleToStringSerial(validityStopTime).c_str(), 
        int(originatingFacility), int(absoluteOrbitNum), 
        int(safeManifestCrc), fileExtension))
        throw pyESAProductFileNameOutOfBufferException("pyESAPFNException: out of buffer in writeout");

    return std::string(out);
}

std::string pyESAProductFileName::getParams(std::string const & fileName, std::string const & basePath) {
    char satelliteName[128], fileType[128], fileExtension[5]  ;
    unsigned char missionNum, originatingFacility ;
    int status, safeManifestCrc; 
    unsigned int absoluteOrbitNum ;
    double validityStartTime=0, validityStopTime=0;
    if (basePath!="")
        status = ACS_ESAProductFileName_GetParams(fileName.c_str(), 
        basePath.c_str(), 
        satelliteName, 
        &missionNum, 
        fileType, 
        &validityStartTime, 
        &validityStopTime, 
        &originatingFacility, 
        &absoluteOrbitNum, 
        &safeManifestCrc, 
        fileExtension);
    else
        status = ACS_ESAProductFileName_GetParams(fileName.c_str(), 
        0, 
        satelliteName, 
        &missionNum, 
        fileType, 
        &validityStartTime, 
        &validityStopTime, 
        &originatingFacility, 
        &absoluteOrbitNum, 
        &safeManifestCrc, 
        fileExtension);
    if (not(status==0))
        throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetName()");
    char out[1024];

    if (1024<snprintf(out, 1024, "%s\\%s\\%s\\%d\\%s\\%64.32lf\\%64.32lf\\%d\\%d\\%d\\%s", 
        fileName.c_str(), basePath.c_str(), 
        satelliteName, int(missionNum), fileType, 
        validityStartTime, 
        validityStopTime, 
        int(originatingFacility), int(absoluteOrbitNum), 
        int(safeManifestCrc), fileExtension))
        throw pyESAProductFileNameOutOfBufferException("pyESAPFNException: out of buffer in writeout");

    return std::string(out);
}


#ifdef UNDEFINED_SINCE_MODIFICATIONS_BROUGHT_TO_PARENT_CC_LIBRARY
int pyESAProductFileName::GetCRC(std::string const & filename) const {
  ///////////////////////////////////////
  //Computing CRC from UserHeader file
  //////////////////////////////////////     
    crcmodel* p_cm ;
    int status ;
    p_cm = crcmodel_construct();
    crc_setup( p_cm, ACS_ESACRC_TB_WIDTH, ACS_ESACRC_TB_POLY, ACS_ESACRC_INIT, ACS_ESACRC_TB_REFIN, ACS_ESACRC_TB_REFOT, ACS_ESACRC_XOROT );

  /* CRC CODE INIT */    
    cm_ini(p_cm);    
    status = crcmodel_filedecode( p_cm, filename.c_str(), sizeof(Transcription_File_Description_Header) );
    if (status) {
        std::ostringstream msg ;
        msg << "ESAPFN. ERROR [" << status << "] running method crcmodel_filedecode on file "
            << filename << " (expected size: " << sizeof(Transcription_File_Description_Header)
            << ")" << std::endl ;
        throwException( exIllegalValueException(msg.str()) ) ;
    }
    int _crc = (int) cm_crc(p_cm) ;
    crcmodel_destruct(p_cm);
    return _crc;
}

std::string pyESAProductFileName::GetNameFromStatistics(pySgi_Data_Transcription_Statistic const & somestats) {
    char out[1024];
    Data_Transcription_Statistic data_stats = somestats._dts.getStruct();
    int status=0;
    try {
        int _crc = GetCRC("DTUserHeader.dat");
        status = ACS_ESAProductFileName_GetNameFromStatistics(&data_stats,_crc, false, out);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    } 
    if (not(status==0))
        throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetNameFromStatistics()");
    std::string outstr(out);
    return outstr;
}
#endif

std::string pyESAProductFileName::GetNameFromStatistics_ComputeCRC(pySgi_Data_Transcription_Statistic const & somestats, std::string const & filename) {
    char out[4096];
    Data_Transcription_Statistic data_stats = somestats._dts.getStruct();
    int status=0;
    /*
      PLEASE NOTE:
        the Data_Transcription_Statistic structure has already been converted
	to LITTLE ENDIAN using the getStruct() method above.
	therefore, the BIG ENDIANNESS flag must be set to 0.
     */
    static const int _IS_BIG_ENDIAN = 0;
    try {
      status = ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC(&data_stats, filename.c_str(), _IS_BIG_ENDIAN, &out[0]);
    }
    catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      throw;
    } 
    if (not(status==0))
      throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetNameFromStatistics_ComputeCRC()");
    return std::string(out);
}



std::string 
pyESAProductFileName::GetNameFromStringSerials_ComputeCRC(
    const char* satelliteName,              /* IN: Satellite ACS name (key in private conf file) */
    const unsigned char missionNum,         /* IN: Mission number */
    const char* filetype,                   /* IN: ACS code for the filetype (key in private conf file) */
    const char* validityStartTimeString,    /* IN: Validity Start (JD50) */
    const char* validityStopTimeString,     /* IN: Validity Stop (JD50) */
    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from acsStations.h) */
    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
    const char * fileToGetCrcFrom,          /* IN: Optional: file to use to compute crc, or NULL to skip CRC */
    const signed long long int expSize,     /* IN: Optional: file size (will be checked), or -1 to skip check */
    const char*fileExtension,            /* IN: File extension */
    const char* basePath)                   /* IN: Optional base path for conf files. Otherwise default is /usr/acs/conf/local/ */
const {
    double validityStartTime = stringSerialToDouble(validityStartTimeString);
    double validityStopTime = stringSerialToDouble(validityStopTimeString);
    char out[70];
    char internalFileExtension[5]={1,2,3,4,0};
    assert(strlen(fileExtension)<=4);
    assert(strncpy(internalFileExtension, fileExtension, 4));
    int status = ACS_ESAProductFileName_GetName_ComputeCRC(
        satelliteName,
        missionNum,
        filetype,
        validityStartTime,
        validityStopTime,
        originatingFacility,
        absoluteOrbitNum,
        fileToGetCrcFrom,
	expSize,
	internalFileExtension,
        basePath,
        out);
    if (not(status==0)){
      std::cerr << "pyESAPFNException: " 
		<< " satelliteName " << satelliteName
		<< " missionNum " << int(missionNum)
		<< " filetype " << filetype
		<< " validityStartTime " << validityStartTime
		<< " validityStopTime " << validityStopTime
                << " originatingFacility " << int(originatingFacility)
                << " absoluteOrbitNum " << absoluteOrbitNum
                << " fileToGetCrcFrom " <<  fileToGetCrcFrom
                << " expSize " << expSize
                << " fileExtension " << fileExtension
                << " basePath " << basePath
                << " out " << out << std::endl;
      throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetNameFromStringSerials_ComputeCRC()");
    }
    return std::string(out);
}


std::string 
pyESAProductFileName::GetNameFromTimesAsStrings_ComputeCRC(
    const char* satelliteName,              /* IN: Satellite ACS name (key in private conf file) */
    const unsigned char missionNum,         /* IN: Mission number */
    const char* filetype,                   /* IN: ACS code for the filetype (key in private conf file) */
    const char* validityStartTimeString,    /* IN: Validity Start (JD50) */
    const char* validityStopTimeString,     /* IN: Validity Stop (JD50) */
    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from acsStations.h) */
    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
    const char * fileToGetCrcFrom,          /* IN: Optional: file to use to compute crc, or NULL to skip CRC */
    const signed int expSize,     /* IN: Optional: file size (will be checked), or -1 to skip check */
    const char*fileExtension,            /* IN: File extension */
    const char* basePath)                   /* IN: Optional base path for conf files. Otherwise default is /usr/acs/conf/local/ */
const {
    char internalFileExtension[5]={1,2,3,4,0};
    assert(strlen(fileExtension)<=4);
    assert(strncpy(internalFileExtension, fileExtension, 4));
    char out[70];
    int status = ACS_ESAProductFileName_GetName_ComputeCRC(
        satelliteName,
        missionNum,
        filetype,
        atof(validityStartTimeString),
        atof(validityStopTimeString),
        originatingFacility,
        absoluteOrbitNum,
        fileToGetCrcFrom,
        expSize,
        internalFileExtension,
        basePath,
        out);
    if (not(status==0)){
      std::cerr << "pyESAPFNException: " 
		<< " satelliteName " << satelliteName
		<< " missionNum " << int(missionNum)
		<< " filetype " << filetype
		<< " validityStartTimeString " << validityStartTimeString
		<< " validityStopTimeString " << validityStopTimeString
        << " originatingFacility " << int(originatingFacility)
        << " absoluteOrbitNum " << absoluteOrbitNum
        << " fileToGetCrcFrom " <<  fileToGetCrcFrom
        << " expSize " << expSize
        << " fileExtension " << fileExtension
        << " basePath " << basePath
        << " out " << out << std::endl;
      throw pyESAProductFileNameBadReturnValueException("pyESAPFNException: status!=0 in pyESAProductFileName::GetNameFromStringSerials_ComputeCRC()");
    }
    return std::string(out);
}


BOOST_PYTHON_MODULE(pyESAProductFileName) {
    boost::python::class_<pyESAProductFileName>("ESAProductFileName")
        .def("GetName_DEPRECATED",     &pyESAProductFileName::GetName)
        .def("GetNameFromTimesAsStrings",     &pyESAProductFileName::GetNameFromTimesAsStrings)
    /*/nicvac
        .def("GetFileTypeName_DEPRECATED", &pyESAProductFileName::GetFileTypeName_DEPRECATED)
    //*/
        .def("GetFiletypeFromKeyword", &pyESAProductFileName::GetFiletypeFromKeyword)
        .def("GetKeywordFromFiletype", &pyESAProductFileName::GetKeywordFromFiletype)
        .def("DateToJD50",  &pyESAProductFileName::DateToJD50)
        .def("GetParams_DEPRECATED",   &pyESAProductFileName::getParams)
        .def("GetParamsToStringSerials",   &pyESAProductFileName::getParamsToStringSerials)
#ifdef UNDEFINED_SINCE_MODIFICATIONS_BROUGHT_TO_PARENT_CC_LIBRARY
        .def("GetCRC",      &pyESAProductFileName::GetCRC)
        .def("GetNameFromStatistics",     &pyESAProductFileName::GetNameFromStatistics)
#endif
        .def("GetNameFromStatistics_ComputeCRC",     &pyESAProductFileName::GetNameFromStatistics_ComputeCRC)
        .def("GetNameFromTimesAsStrings_ComputeCRC",     &pyESAProductFileName::GetNameFromTimesAsStrings_ComputeCRC)
        .def("stringSerialToDouble", &pyESAProductFileName::stringSerialToDouble)
        .def("doubleToStringSerial", &pyESAProductFileName::doubleToStringSerial)
        .def("GetNameFromStringSerials", &pyESAProductFileName::GetNameFromStringSerials)
        .def("GetNameFromStringSerials_ComputeCRC", &pyESAProductFileName::GetNameFromStringSerials_ComputeCRC)
        ;
}
