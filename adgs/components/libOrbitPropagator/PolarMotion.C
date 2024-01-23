// PRQA S 1050 EOF
/*

	Copyright 2017-2022, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2018/01/19 07:18:12  ricfer
	setFromIERSB updated.
	
	Revision 1.1  2017/12/21 08:21:39  ricfer
	First add.
	
	Revision 1.1  2017/12/19 09:43:04  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/

#include <cmath>
#include <limits>
#include <algorithm>
#include <Filterables.h>
#include <exException.h>
#include <PolarMotion.h>
#include <StringUtils.h>
#include <XmlDoc.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PolarMotion)

static string ns(const string& x) { return("*[local-name() = '"+x+"']"); }

/**
 * Default Class constructor.
 */	
PolarMotion::PolarMotion(const std::string& fileNameIERSbull) :
	utcTime_(),
	X_(0.0),
	Y_(0.0),
	UT1minusUTC_(0.0),
	TAIminusUTC_(0.0),
    fileNameIERSbull_(),
    bull_mjddate_(),
    bull_x_(),     
    bull_y_(),     
    bull_ut1_utc_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
    if(!fileNameIERSbull.empty())
    {
        const string type = checkIERSbullType(fileNameIERSbull);
        ACS_COND_THROW(type.empty(), PolarMotionException("Failed. File \""+fileNameIERSbull+"\" is not an IERS bulletin A or B"));
        if(type=="B")
        {
            readIERSB(fileNameIERSbull);
        }
        else
        {
            readIERSA(fileNameIERSbull);
        }
    }
} 

/**
 * Class destructor.
 */	
PolarMotion::~PolarMotion() throw()
{
}

/**
 * Class copy constructor. 
 */ 
PolarMotion::PolarMotion(const PolarMotion & inp) :
	utcTime_(inp.utcTime_),
	X_(inp.X_),
	Y_(inp.Y_),
	UT1minusUTC_(inp.UT1minusUTC_),
	TAIminusUTC_(inp.TAIminusUTC_),
    fileNameIERSbull_(inp.fileNameIERSbull_),
    bull_mjddate_(inp.bull_mjddate_),
    bull_x_(inp.bull_x_),     
    bull_y_(inp.bull_y_),     
    bull_ut1_utc_(inp.bull_ut1_utc_)
{
}

/**
 * Overload of the assign operator =.
 */ 
PolarMotion&  PolarMotion::operator=(const PolarMotion & inp)
{
	if (this != &inp) {
		utcTime_ = inp.utcTime_;
		X_ = inp.X_;
		Y_ = inp.Y_;
		UT1minusUTC_ = inp.UT1minusUTC_;
		TAIminusUTC_ = inp.TAIminusUTC_;
        fileNameIERSbull_ = inp.fileNameIERSbull_;
        bull_mjddate_ = inp.bull_mjddate_;
        bull_x_ = inp.bull_x_;    
        bull_y_ = inp.bull_y_;     
        bull_ut1_utc_ = inp.bull_ut1_utc_;
	}
	return *this ;
}

/**
 * Check if the provided file is an IERS bulletin A or B
 * @param[in] fileName IERS Bulletin file name
 * @return "A" if the file is a IERS bulletin A, "B" if bulletin B, otherwise empty
 */
std::string PolarMotion::checkIERSbullType(const string& fileName) 
{
	ifstream ifile(fileName.c_str()) ;
  	ACS_COND_THROW(!ifile.is_open(), PolarMotionException(" Failed. Opening file "+fileName)) ;
    
    string line;
    size_t found;
	while( getline(ifile, line) )
	{
		found = line.find("BULLETIN B") ;
		if(found!=std::string::npos)
		{
			return("B");
		}
        found = line.find("I E R S   B U L L E T I N - A") ;
        if(found!=std::string::npos)
        {
			
            return("A");
		}
	}
    ifile.close();
    
    /// Check if the bulletin is in xml format
    XmlDocSptr inxmlDocSptr( new XmlDoc );
    inxmlDocSptr->setParameters(XmlDoc::parDefault).initWithFile(fileName);
    
    string pName;
    inxmlDocSptr->getValue("//"+ns("version")+"//"+ns("product")+"/text()", pName, false);
    if(pName=="BulletinA") return("A");
    else if(pName=="Bulletin B") return("B");
    
    return("");
}

/**
 * Read EOPs and times from IERS bulletin A or B
 * @param[in] bul_file_name IERS Bulletin file name
 * @param[in] time2search UTC time to search (provided as DateTime object)
 */
void PolarMotion::setFromIERSbulletin(
	const string& bul_file_name,
	const DateTime& time2search) 
{
    const string type = checkIERSbullType(bul_file_name);
    ACS_COND_THROW(type.empty(), PolarMotionException("Failed. File \""+bul_file_name+"\" is not an IERS bulletin A or B"));
    
    if(type=="B")
    {
        setFromIERSB(bul_file_name, time2search);
    }
    else
    {
        setFromIERSA(bul_file_name, time2search);
    }
}

/**
 * Read EOPs and times from IERS bulletin A or B
 * @param[in] time2search UTC time to search (provided as DateTime object)
 */
void PolarMotion::setFromIERSbulletin(const DateTime& time2search) 
{
    ACS_COND_THROW( fileNameIERSbull_.empty(), PolarMotionException("Failed. No IERS bulletin set yet."));
    
    setFromIERSbulletin(fileNameIERSbull_, time2search);
}

/**
 * Read EOPs and times from IERS bulletin A
 * @param[in] bula_file_name IERS Bulletin A file name
 * @param[in] time2search UTC time to search (provided as DateTime object)
 */
void PolarMotion::setFromIERSA(
	const string& bula_file_name,
	const DateTime& time2search) 
{
    ACS_COND_THROW(bula_file_name.empty(), PolarMotionException(" Failed. Empty file name passed in input.")) ;
    
    if( bula_file_name != fileNameIERSbull_)
    {
        /// Different IERS bulletin. Read the new one.
        readIERSA(bula_file_name);
    }
    
    if(time2search == utcTime_)
    {
        /// Requested time has been already stored. Do nothing.
        return;
    }
     
    /// Interpolate EOPs and relative UT1-UTC delta times, converting x/y from msec arc to arcsec and times in seconds.
    interpolateEOP(time2search, {1.0, 1.0, 1.0});
    
    /// Set the UTC time.
	utcTime(time2search);
    
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  dump());
}

/**
 * Read EOPs and times from IERS bulletin B
 * @param[in] bulb_file_name IERS Bulletin B file name
 * @param[in] time2search UTC time to search (provided as DateTime object)
 */
void PolarMotion::setFromIERSB(
	const string& bulb_file_name, // IN : IERS B Bulletin file name
	const DateTime& time2search) // IN : UTC time to search (represented as DateTime object)
{
    ACS_COND_THROW(bulb_file_name.empty(), PolarMotionException(" Failed. Empty file name passed in input.")) ;
    
    if( bulb_file_name != fileNameIERSbull_)
    {
        /// Different IERS bulletin. Read the new one.
        readIERSB(bulb_file_name);
    }
    
    if(time2search == utcTime_)
    {
        /// Requested time has been already stored. Do nothing.
        return;
    }
    
    /// Interpolate EOPs and relative UT1-UTC delta times, converting x/y from msec arc to arcsec and times in seconds.
    interpolateEOP(time2search, {1000.0, 1000.0, 1000.0});
    
    /// Set the UTC time.
	utcTime(time2search);
    
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  dump());
}

/**
 * This method read and register EOPs and times from IERS bulletin A.
 * @param[in] bula_file_name IERS Bulletin A file name.
 */
void PolarMotion::readIERSA(const string& bula_file_name) 
{
    ifstream ifile(bula_file_name.c_str()) ;
  	ACS_COND_THROW(!ifile.is_open(), PolarMotionException(" Failed. Opening file "+bula_file_name)) ;
    
    /// Look for Earth Pole Parameters x/y and relative UT1-UTC delta times.
	string line;
    size_t found;
    while( getline(ifile, line) )
	{
		found = line.find("MJD      x    error") ;
		if(found!=std::string::npos) break ;
	}
    
    if(ifile.eof())
    {
        ifile.close();
        readIERSABinXML(bula_file_name);
    }
    else
    {
        getline(ifile, line);
	    bull_mjddate_.clear();
        bull_x_.clear();
        bull_y_.clear();
        bull_ut1_utc_.clear();
	    while( getline(ifile, line) )
	    {
		    vector<string> tokens ;
		    StringUtils::Tokenize(line, tokens, " ");
		    if(tokens.size()<9) break;
		    bull_mjddate_.push_back(atof(tokens.at(3).c_str())) ;
		    bull_x_.push_back(atof(tokens.at(4).c_str())) ;
		    bull_y_.push_back(atof(tokens.at(6).c_str())) ;
		    bull_ut1_utc_.push_back(atof(tokens.at(8).c_str())) ;
	    }
        
    }
    ACS_COND_THROW(bull_mjddate_.empty(), PolarMotionException("Failed. File "+bula_file_name+" is not a IERS A bulletin.")) ;
    
    if(ifile.is_open())
    {
        /// Look for TAI-UTC delta time. 
	    while( getline(ifile, line) )
	    {
		    found = line.find("TAI-UTC(") ;
		    if(found!=std::string::npos)
		    {
			    vector<string> tokens ;
		        StringUtils::Tokenize(line, tokens, " ");
                ACS_COND_THROW((tokens.size()<4), PolarMotionException(" Failed. File "+bula_file_name+" looking for TAI-UTC.")) ;
                TAIminusUTC_ = static_cast<double>(atof(tokens[3].c_str())) ;
			    break ;
		    }
	    }
        ifile.close();
	    if(TAIminusUTC_<1)
        {
            ACS_LOG_WARNING("PolarMotion::readIERSB(): WARNING. File \"" << bula_file_name << "\" does not contain TAI-UTC.");
	    }
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " TAI-UTC : " << TAIminusUTC_ << " sec");
    }
        
    X_ = bull_x_[0];
    Y_ = bull_y_[0];
    utcTime_ = DateTime(DateTime::MJD(bull_mjddate_[0]));
    UT1minusUTC_ = bull_ut1_utc_[0];
    
    /// Set IERS bulletin A file name.
    fileNameIERSbull_ = bula_file_name;
    
    ACS_LOG_INFO("PolarMotion::readIERSA(): IERS A bulletin file \"" << bula_file_name << "\" successfully read.");
}

/**
 * This method read and register EOPs and times from IERS bulletin B.
 * @param[in] bulb_file_name IERS Bulletin B file name.
 */
void PolarMotion::readIERSB(const string& bulb_file_name)
{
	ifstream ifile(bulb_file_name.c_str()) ;
  	ACS_COND_THROW(!ifile.is_open(), PolarMotionException(" Failed. Opening file "+bulb_file_name)) ;
	
    /// Look for Earth Pole Parameters x/y and relative UT1-UTC delta times.
  	string line ;
	size_t found ;
	while( getline(ifile, line) )
	{
		found = line.find("Mean formal error") ;
		if(found!=std::string::npos) break ;
	}
    
    if(ifile.eof())
    {
        ifile.close();
        readIERSABinXML(bulb_file_name);
    }
    else
    {
	    getline(ifile, line) ;
	    bull_mjddate_.clear();
        bull_x_.clear();
        bull_y_.clear();
        bull_ut1_utc_.clear();
	    while( getline(ifile, line) )
	    {
		    vector<string> tokens ;
		    StringUtils::Tokenize(line, tokens, " ");
		    if(tokens.size()<7) break;
		    bull_mjddate_.push_back(atof(tokens.at(3).c_str())) ;
		    bull_x_.push_back(atof(tokens.at(4).c_str())) ;
		    bull_y_.push_back(atof(tokens.at(5).c_str())) ;
		    bull_ut1_utc_.push_back(atof(tokens.at(6).c_str())) ;
	    }
    }   
    ACS_COND_THROW(bull_mjddate_.empty(), PolarMotionException(" Failed. File "+bulb_file_name+" is not a IERS B bulletin.")) ;
    
    if(ifile.is_open())
    {    
        /// Look for TAI-UTC delta time.
	    while( getline(ifile, line) )
	    {
		    found = line.find("TAI - UTC") ;
		    if(found!=std::string::npos)
		    {
			    size_t pos1 = line.find("=") ;
			    size_t pos2 = line.find("s.") ;
			    ACS_COND_THROW((pos2 <= pos1+1), PolarMotionException(" Failed. File "+bulb_file_name+" null TAI-UTC.")) ;
			    TAIminusUTC_ = static_cast<double>(atoi(line.substr(pos1+1, pos2-pos1-1).c_str())) ;
			    break ;
		    }
	    }
	    ifile.close();
        if(TAIminusUTC_<1)
        {
            ACS_LOG_WARNING("PolarMotion::readIERSA(): WARNING. File \"" << bulb_file_name << "\" does not contain TAI-UTC.");
	    }
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " TAI-UTC : " << TAIminusUTC_ << " sec");
        
    }
        
    X_ = bull_x_[0]/1000.0;
    Y_ = bull_y_[0]/1000.0;
    utcTime_ = DateTime(DateTime::MJD(bull_mjddate_[0]));
    UT1minusUTC_ = bull_ut1_utc_[0]/1000.0;
    
    /// Set IERS bulletin B file name.
    fileNameIERSbull_ = bulb_file_name;
    
    ACS_LOG_INFO("PolarMotion::readIERSA(): IERS B bulletin file \"" << bulb_file_name << "\" successfully read.");
}

/**
 * This method read EOPs and times from IERS bulletin A in XML format.
 * @param[in] bula_file_name IERS Bulletin A file name.
 */
void PolarMotion::readIERSABinXML(const string& bula_file_name) 
{
    XmlDocSptr inxmlDocSptr( new XmlDoc );
    inxmlDocSptr->setParameters(XmlDoc::parDefault).initWithFile(bula_file_name);
    
    std::vector< XmlDocSptr > xmls = inxmlDocSptr->xPath( "//"+ns("data")+"//"+ns("timeSeries")+"[child::"+ns("dataEOP")+"//*[name() = 'pole']]");
    
    bull_mjddate_.resize(xmls.size());
    bull_x_.resize(xmls.size());
    bull_y_.resize(xmls.size());   
    bull_ut1_utc_.resize(xmls.size());
    for(size_t i=0; i<xmls.size(); ++i) 
    {
        xmls[i]->getValue("//"+ns("time")+"//"+ns("MJD")+"/text()", bull_mjddate_[i]);
        xmls[i]->getValue("//"+ns("dataEOP")+"//"+ns("pole")+"//"+ns("X")+"/text()", bull_x_[i]);
        xmls[i]->getValue("//"+ns("dataEOP")+"//"+ns("pole")+"//"+ns("Y")+"/text()", bull_y_[i]);
        xmls[i]->getValue("//"+ns("dataEOP")+"//"+ns("UT")+"//"+ns("UT1-UTC")+"/text()", bull_ut1_utc_[i]);
    }   
}

/**
 * This method interpolates EOP and relative UT1-UTC delta time at a given time.
 * @param[in] time2search UTC time to search (provided as DateTime object)
 * @param[in] scFact A vector of 3 scale factors, respectively for EOP x, EOP y and UT1-UTC delta time.
 */
void PolarMotion::interpolateEOP(const DateTime& time2search, const vector<double>& scFact)
{
    ACS_COND_THROW((bull_mjddate_.empty() || bull_x_.empty() || bull_y_.empty() || bull_ut1_utc_.empty()), 
        PolarMotionException(" Failed. Impossible to interpolate EOP, some buffer is empty.")) ;
    
    if(time2search == utcTime_)
    {
        /// The value we have is already at that time. Do nothing.
        return;
    }
    
    /// Look for the nearest EOPs to the provided "time2search".
    const double jd = DateTime::MJD(time2search).jd();   
    size_t k = 0;
    while(bull_mjddate_[k] < jd)
    {
        ++k;
        if(k==(bull_mjddate_.size()-1))
        {
            k = bull_mjddate_.size()-2;
            break;
        }    
    }
    
    /// Interpolate each EOP, converting them from msec arc to arcsec.
    const double A = (jd-bull_mjddate_[k+1])/(bull_mjddate_[k]-bull_mjddate_[k+1]); /// (jd-tb)/(ta-tb)
    const double B = (jd-bull_mjddate_[k])/(bull_mjddate_[k]-bull_mjddate_[k+1]); /// (jd-ta)/(ta-tb)
    double ya = bull_x_[k];
    double yb = bull_x_[k+1];    
    X( (A*ya - B*yb)/scFact[0] );
    ya = bull_y_[k];
    yb = bull_y_[k+1];
    Y( (A*ya - B*yb)/scFact[1] );
    
    /// Set the UT1-UTC time (in seconds), again by interpolation.
    ya = bull_ut1_utc_[k];
    yb = bull_ut1_utc_[k+1];
    UT1minusUTC((A*ya - B*yb)/scFact[2]) ;
}
 
/**
 * Dump PolarMotion object attributes
 */
string PolarMotion::dump() const 
{
	ostringstream msg;
    msg << "PolarMotion::dump(): X = " << std::fixed << std::setprecision(9) << X_ << " arcsec,"
                            << " Y = " << std::fixed << std::setprecision(9) << Y_ << " arcsec,"
                            << " date = " << utcTime_.asISO8601(6)
                            << " UT1-UTC = " << std::fixed << std::setprecision(9) << UT1minusUTC_ << " sec,"
                            << " TAI-UTC = " << TAIminusUTC_ << " sec., "
                            << " IERS bull. = \"" << fileNameIERSbull_ << "\"." << endl; // PRQA S 4400 L1
	
	// PRQA L:L1
	return msg.str();
}

_ACS_END_NAMESPACE
