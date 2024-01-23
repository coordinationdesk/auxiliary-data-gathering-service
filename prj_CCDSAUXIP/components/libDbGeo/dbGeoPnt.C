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

	$Prod: A.C.S. Database Geo Object Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2014/09/03 14:18:22  marfav
	Adding exp notation management to parseFloat as per S2PDGS-889
	
	Revision 5.7  2014/05/28 09:50:02  marpas
	fixed coordinate inversion regression, getSqlString out format
	
	Revision 5.6  2014/05/27 14:28:57  marpas
	regression in reading negative coordinates fixed
	generation of numeric strings fixed
	
	Revision 5.5  2014/05/27 10:40:45  marpas
	precision handling when reading data from postgres SQL result
	
	Revision 5.4  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.3  2014/05/06 12:25:58  marpas
	precision in getting sql string managed
	dbGeoObject interface has now methods to set/get precision and a defaultPrecision static constant
	precision must be greater than 2, note that precision will affect lenght of string that will be used in queries, limiting the maximum number of points as the query string has limited size
	
	Revision 5.2  2013/09/17 10:31:01  enrcar
	EC:: method intersect was unreliable if one of the poligons crossed 180. Fixed.
	
	Revision 5.1  2013/09/02 11:49:30  marpas
	wrong longitude normalization caused wrong response on dbGeoPnt intersect - fixed
	Actually the function sums to lognitude 360 if and only if longitude is <= -180 degrees
	
	Revision 5.0  2013/07/11 16:07:12  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.15  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.14  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.13  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.12  2011/03/31 15:16:15  marpas
	Oracle11 has the lon, lat convention in the strings
	
	Revision 2.11  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.10  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.9  2010/04/26 14:32:26  marpas
	implementing Oracle spatial/locator syntax. Work in progress
	
	Revision 2.8  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.7  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.6  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.5  2009/03/02 12:38:18  marpas
	CODECHECK warning and errors fixed - no regressions
	
	Revision 2.4  2007/07/03 10:32:12  marpas
	toString now has argument for decimal precision
	
	Revision 2.3  2007/07/03 09:19:30  marpas
	intersection computatio fixed
	
	Revision 2.2  2007/06/11 13:21:42  marpas
	toHtmlString methods added
	
	Revision 2.1  2006/05/12 16:32:07  marpas
	Bug fixed when north pole is inside the polygon
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.8  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.7  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.6  2003/07/31 17:12:06  marpas
	compilation warning fixed
	
	Revision 1.5  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.4  2003/04/15 18:35:04  marpas
	GCC3.2
	
	Revision 1.3  2002/12/16 11:13:50  paoscu
	Export and import methods modified.
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoPnt.h>
#include <dbGeoObject.h>
#include <rsResourceSet.h>
#include <XMLOstream.h>
#include <exException.h>
#include <sstream>
#include <iomanip>
#include <cmath>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoPnt)

dbGeoPnt::dbGeoPnt(double lat, double lon) :
	_lat(lat),
	_lon(lon)
{
}

dbGeoPnt::dbGeoPnt(const dbGeoPnt &p) :
	_lat(p._lat),
	_lon(p._lon)
{
}

dbGeoPnt::~dbGeoPnt() throw() 
{
}

dbGeoPnt &dbGeoPnt::operator=(const dbGeoPnt &p)
{
	if (this != &p) {
		_lat = p._lat ;
		_lon = p._lon ;
	}
	
	return *this ;
}

bool operator==(const dbGeoPnt &a, const dbGeoPnt &b) throw() // PRQA S 4020
{
	if (( a.lat() > b.lat() ) || (a.lat() < b.lat())) {
		return false ;
    }
	if (( a.lon() > b.lon() ) || (a.lon() < b.lon())) {
		return false ;
    }
			
	return true ;
}


bool dbGeoPnt::cross180(const dbGeoPnt &p) const 
{
	return fabs(_lon-p._lon) > 180 ; // PRQA S 4400
}

bool dbGeoPnt::cross180(const dbGeoPnt &a, const dbGeoPnt &b)  
{
	return ( fabs(b.lon()-a.lon()) > 180.0 ) ; // PRQA S 4400
}

ostream &dbGeoPnt::getSqlString(ostream &os, const string &dialectName, int precision) const // PRQA S 4020
{
	if (dialectName == "Informix9") {
		os << '(' << setprecision(precision) << _lat << ',' << _lon << ')' ;
		return os ; // PRQA S 4028
    }
    else if (dialectName == "Postgres8") {
		os << setprecision(precision) << _lon << ' ' <<  _lat ;
		return os ; // PRQA S 4028
	}
    else if (dialectName == "Oracle11") {
		os << setprecision(precision) << _lon << ',' << _lat ;
		return os ; // PRQA S 4028
	}
    else {
	    ACS_THROW (dbGeoObject::ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}



void dbGeoPnt::exportObj(rsResourceSet &s) const
{
	s.setValue("_lat",_lat) ;
	s.setValue("_lon",_lon) ;
}


void dbGeoPnt::importObj(rsResourceSet &s)
{
	s.getValue("_lat",_lat) ;
	s.getValue("_lon",_lon) ;
}



void dbGeoPnt::exportXML(XMLOstream &xst) const
{
	{
		ostringstream os ;
		os << fixed << setprecision(17) << _lat ; // PRQA S 4400
		XMLOstream::Tag txml(xst, "_lat", os.str()) ;
	}
	{
		ostringstream os ;
		os << fixed << setprecision(17) << _lon ; // PRQA S 4400
		XMLOstream::Tag txml(xst, "_lon", os.str()) ;
	}
}

double dbGeoPnt::distance(const dbGeoPnt &p) const 
{
	return hypot(lat()-p.lat(), lon()-p.lon()) ;
}



void dbGeoPnt::perpendicularPos(const dbGeoPnt & point1, const dbGeoPnt & point2, dbGeoPnt & perp) const
{ 
	// compute a point on the normal
	double v0 = point1.lon() -point2.lon();
	double v1 = point1.lat() -point2.lat();
	double v2 = v0*v0 + v1*v1 ;

	if (v2 == 0) { // same point // PRQA S 3270
		ACS_THROW(dbGeoObject::ExecuteException ("null length segment")) ; // PRQA S 3081
    }

	double n = v0*(lon()-point1.lon()) + v1*(lat()-point1.lat()) ;
	n /= v2;

	perp.lon(point1.lon() + n*v0);
	perp.lat(point1.lat() + n*v1);
}




// c inside [a,b]
bool dbGeoPnt::belongsToSegment(const dbGeoPnt &a, const dbGeoPnt &b, const dbGeoPnt &c)  // PRQA S 4020
{
	const double epsilon = 1e-15 ;

	double lon = 0 ; 
    double LON = 0 ;
    double l = 0 ;
	const bool cross_180 = cross180( a, b ) ;

	if (cross_180)
	{
		// Points crossing 180 -- the LEFT plane is translated to the RIGHT (e.g. 175..-175 becames: 175..185)
		double a1 = ( (a.lon()<0.0) ? (a.lon()+360.0) : a.lon() ) ; a1 = fmod(a1, 360.0) ; // PRQA S 3384, 4400 3
		double b1 = ( (b.lon()<0.0) ? (b.lon()+360.0) : b.lon() ) ; b1 = fmod(b1, 360.0) ; 
		double c1 = ( (c.lon()<0.0) ? (c.lon()+360.0) : c.lon() ) ; c1 = fmod(c1, 360.0) ; 

		lon = min(a1, b1) ; 
		LON = max(a1, b1) ;
		l = c1 ;
	} else {
		// Points not crossing 180 -- no normalization is required (e.g. -5..+5 remains -5..+5) 
		lon = min(a.lon(), b.lon()) ; 
		LON = max(a.lon(), b.lon()) ;
		l = c.lon() ;
	}

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "lon: " << lon << "  " << l << "  " << LON) ;
	if (((lon <= l) && (l <= LON)) || // lon is inside the interval
		(fabs(l - lon) < epsilon) || (fabs(l - LON) < epsilon)) { // edges
		double la = c.lat() ;
		
		double lat = std::min(a.lat(), b.lat()) ;
		double LAT = std::max(a.lat(), b.lat()) ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "lat: " << lat << "  " << la << "  " << LAT) ;
		if (((lat <=  la) && (la <=  LAT)) || // lat inside the interval
			(fabs(la - lat) < epsilon) || (fabs(la - LAT) < epsilon)) { // edges
			return true ;
        }
	}
	return false ;
}


dbGeoPnt::Intersection dbGeoPnt::intersect( // PRQA S 4020
	const dbGeoPnt &a1, const dbGeoPnt &a2, 
	const dbGeoPnt &b1, const dbGeoPnt &b2, 
	dbGeoPnt *ci
) 
{
	dbGeoPnt c ;
	
	// compute lines parameters
	double aa = a2.lat()-a1.lat() ;
	double ba = b2.lat()-b1.lat() ;

	double a1_lon = 0 ; 
    double a2_lon = 0 ; 

	const bool a_cross_180 = cross180(a1,a2) ; 
	const bool b_cross_180 = cross180(b1,b2) ; 

	if (a_cross_180)
	{
		// Points crossing 180 -- the LEFT plane is translated to the RIGHT (e.g. 175..-175 becames: 175..185)
		a1_lon = ( (a1.lon()<0.0) ? (a1.lon()+360.0) : a1.lon() ) ; a1_lon = fmod(a1_lon, 360.0) ;   // PRQA S 3380, 4400 2
		a2_lon = ( (a2.lon()<0.0) ? (a2.lon()+360.0) : a2.lon() ) ; a2_lon = fmod(a2_lon, 360.0) ; 
	}
	else
	{
		// Points not crossing 180 -- no normalization is required (e.g. -5..+5 remains -5..+5) 
		a1_lon = a1.lon() ; 
		a2_lon = a2.lon() ; 
	}

    double b1_lon = 0 ;
    double b2_lon = 0 ; 
	if (b_cross_180)
	{
		// Points crossing 180 -- the LEFT plane is translated to the RIGHT (e.g. 175..-175 becames: 175..185)
		b1_lon = ( (b1.lon()<0.0) ? (b1.lon()+360.0) : b1.lon() ) ; b1_lon = fmod(b1_lon, 360.0) ;  // PRQA S 3380, 4400 2
		b2_lon = ( (b2.lon()<0.0) ? (b2.lon()+360.0) : b2.lon() ) ; b2_lon = fmod(b2_lon, 360.0) ; 
	}
	else
	{
		// Points not crossing 180 -- no normalization is required (e.g. -5..+5 remains -5..+5) 
		b1_lon = b1.lon() ; 
		b2_lon = b2.lon() ; 
	}

	const double ab = a1_lon-a2_lon ;	// After normalization
	const double bb = b1_lon-b2_lon ;	// After normalization
	
	const double epsilon = 1e-12 ;
	// now the determinant
	const double det = aa*bb-ab*ba ;	// After normalization
	if (fabs(det) < epsilon)  { // too small - close to zero
		return IntersectNone ;
    }
	
	const double ac = ab*a1.lat() + aa*a1_lon ;	// After normalization
	const double bc = bb*b1.lat() + ba*b1_lon ;	// After normalization
	
	// compute the intersection of the lines
	c.lon() = (ac*bb-ab*bc)/det ;
	c.lat() = (aa*bc-ac*ba)/det ;
	
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[ " << a1 << ',' << a2 << " ] [ " << b1 << ',' << b2 << " ] -> " << c) ;
	size_t ret = 0 ;
	
	if (belongsToSegment(a1,a2,c)) {
		ret |= 1 ;
    }
	if (belongsToSegment(b1,b2,c)) {
		ret |= 2 ; // PRQA S 4400
    }
	
	if (ci) { *ci = c ; }
	return Intersection(ret) ; // PRQA S 3013, 3081
}

string dbGeoPnt::to2LineString() const 
{
	return toString(_lat,Latitude) + "\n" + toString(_lon,Longitude) ;
}

string dbGeoPnt::toHtmlString(bool letterAtEnd, unsigned short secDec) const 
{
	return toHtmlString(_lat,Latitude, letterAtEnd, secDec) + " - " + toHtmlString(_lon,Longitude, letterAtEnd, secDec) ;
}



string dbGeoPnt::toString(unsigned short decimal) const 
{
	return toString(_lat,Latitude, decimal) + " " + toString(_lon,Longitude,decimal) ;
}

string dbGeoPnt::toString(
	double val,
	Type type,
	unsigned short decimal
)
{

	std::ostringstream os ;

	int degw = 3 ; // PRQA S 4400
	string s ;  
	
    switch(type) {
    case Latitude:
        {
		    degw = 2 ; // PRQA S 4400
		    s = (val > 0) ? "N" : "S" ; // north or south ? // PRQA S 3380
		    break ;
        }
    case Longitude:
        {
		    degw = 3 ; // PRQA S 4400
		    s = (val > 0) ? "E" : "W" ; // east or west ? // PRQA S 3380
		    break ;
        }
	}
		
	int ws = 2 ;
	int	deg ;  // PRQA S 4101 3
    int min ;
	double sec ;
    D2DMS(val, deg, min, sec); // to sexagesimal 
	// and then  N 035<deg sign> 01' 23.123"
	os << fixed << s 
	   << setfill('0') << setw(degw) << abs(deg) << "\260" 
	   << setfill('0') << setw(2) << min << "'" ; // PRQA S 4400
	if (decimal > 0 ) { // PRQA S 3000, 2
		os << setw(ws+1+decimal) << setprecision(decimal) << sec << "\"" ; // PRQA S 3000
	}
    else {
		os << setw(ws) << setprecision(2) << int(sec) << "\"" ; // PRQA S 3081, 4400
    }
	return os.str() ;
}

void dbGeoPnt::D2DMS(double value, int &d, int &m, double &s)
{
// converts to degree min sec (the sign is on the degree value)
    double absGeo = fabs(value);
    
    d = int(absGeo) ; // PRQA S 3081
    s = (absGeo - d) * 3600. ; // PRQA S 3011, 4400 
    m =  int(s / 60.) ; // PRQA S 3081, 4400 2
    s =  s - m * 60. ; // PRQA S 3011
    
    d = int(copysign(double(d), value)); // PRQA S 3081
}

string dbGeoPnt::toHtmlString(
	double val,
	Type type,
	bool letterAtEnd,
	unsigned short secDec
)
{
	ostringstream os ; 
	int degw = 0 ;
	string s ;  
	string ts ;
	
    switch(type) {
    case Latitude:
        {
		    degw = 2 ; // PRQA S 4400
		    s = (val > 0) ? "N" : "S" ; // north or south ? // PRQA S 3380
		    ts = "Lat." ;
		    break ;
        }
    case Longitude:
        {
		    degw = 3 ; // PRQA S 4400
		    s = (val > 0) ? "E" : "W" ; // east or west ? // PRQA S 3380
		    ts = "Lon." ;
		    break ;
        }
	}
		
	int	deg ; // PRQA S 4101 3
    int min ;
	double sec ;
    D2DMS(val, deg, min, sec);
	if (letterAtEnd) { // es | Lon. 10&deg; 05' 02.12" E |
		os << fixed 
		   << ts << " " << setfill('0') << setw(degw) << abs(deg) << "&deg; " 
		   << setfill('0') << setw(2) << min << "' " // PRQA S 4400 2
		   << setw(2 + ((0 != secDec) ? 1+secDec : 0)) << setprecision(secDec) << setiosflags(std::ios::right) << sec << "\" " << s ; // PRQA S 3000, 3380
	}
	else { // es | Lat. N 10&deg; 05' 02.12" |
		os << fixed << ts << " " << s << " " 
		   << setfill('0') << setw(degw) << abs(deg) << "&deg; " 
		   << setfill('0') << setw(2) << min << "' " // PRQA S 4400
		   << setw(2 + ((0 != secDec) ? 1+secDec : 0)) << setprecision(secDec) << setiosflags(std::ios::right) << sec << "\" " ; // PRQA S 3000, 3380, 4400
	}
	
	
	return os.str() ;
}

istream & dbGeoPnt::parseOracle(dbGeoPnt &p, istream &is) // PRQA S 4020
{
	if (!is.good()) { return is ; } // PRQA S 4028
	
	double lat; // PRQA S 4101 3
    double lon ;
	char comma ;
	is >> lon >> comma >> lat;
	if (!is.fail()) {
		p.lat(lat) ;
		p.lon(lon) ;
	}
	return is ; // PRQA S 4028
}

namespace {
// S2PDGS-889
// adding management of exp notation when parsing floats from postgres
    inline
    string parseFloat(istream &is, int &prec) // PRQA S 2134
    {
        string f ;
        std::istream::sentry s(is);
        bool dot = false ;
        bool got_e = false;
	bool managed_e = false;
        prec = 3 ; // at least 3 digits
        if (s) { // PRQA S 3050
            while (is.good()) {
                char c = is.get();
                if (std::isspace(c,is.getloc())) { 
                    if (not f.empty()) {
                        break; 
                    }
                }
		else if (got_e && !managed_e)
		{
			managed_e = true;
			if ( (c == '+') || ( c == '-' ) )
			{
				f+=string("e")+c+"0";
			}
			else
			{
				// After E or e only + or - are allowed
				is.putback (c);
				break;
			}
		}
                else if (std::isdigit(c,is.getloc())) { 
                    f +=c; 
                    if (dot && !managed_e) { ++prec; }
                }
		else if (((c == 'e') || (c == 'E')) && !got_e) 
		{
			got_e = true;	
		}
                else if (c == '.') { 
                    f += c ; 
                    dot = true ; 
                }
                else if (c == '-') { 
                    if (f.empty()) {
                        f += c ; 
                        ++prec ;
                    }
                    else {
                        is.putback(c) ; 
                        break; 
                    }
                }
                else { 
                    is.putback(c) ; 
                    break; 
                }
            }
        }
        return f;
    }
}

istream & dbGeoPnt::parsePostgres(dbGeoPnt &p, istream &is, int &precision) // PRQA S 4020
{
	if (!is.good()) { return is ; } // PRQA S 4028
	
    int lonp = 0 ;
    string lon = parseFloat(is, lonp) ;
    int latp = 0 ; 
    string lat = parseFloat(is, latp) ;
	if (!is.fail()) {
		p.lat(atof(lat.c_str())) ;
		p.lon(atof(lon.c_str())) ;
        precision = max(latp, lonp) ; // PRQA S 4400 
	}
	return is ; // PRQA S 4028
}




istream &
operator >> (istream &is, dbGeoPnt& p) // PRQA S 4020
{
	if (!is.good()) { return is ; } // PRQA S 4028
	
	double lat;  // PRQA S 4101 4 
    double lon ;
	char ell ; 
    char comma ;
	is >> ell >> lat >> comma >> lon >> ell;
	if (!is.fail()) {
		p.lat(lat) ;
		p.lon(lon) ;
	}
	return is ; // PRQA S 4028
}

exostream & operator << (exostream &os, const dbGeoPnt& gs) // PRQA S 4222
{
    std::ostringstream s ;
    gs.getSqlString(s, "Informix9", dbGeoObject::defaultPrecision) ;
    return os << s.str() ;
}

string toString(dbGeoPnt::Intersection i)  // PRQA S 4020
{
    switch(i) {
    case dbGeoPnt::IntersectNone: { return "IntersectNone" ; }  
    case dbGeoPnt::Intersect1st: { return "Intersect1st" ; }  
    case dbGeoPnt::Intersect2nd: { return "Intersect2nd" ; }  
    case dbGeoPnt::IntersectBoth: { return "IntersectBoth" ; }  
    default:
        {
            ostringstream os ; 
            os << "unrecognized Intersection <" << int(i) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

std::ostream & operator << (std::ostream &os, dbGeoPnt::Intersection i) 
{
    return os << toString(i) ;
}
exostream & operator << (exostream &os, dbGeoPnt::Intersection i) {// PRQA S 4222
    return os << toString(i) ;
}



_ACS_END_NAMESPACE

