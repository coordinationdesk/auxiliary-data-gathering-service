// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.12  2016/05/11 09:45:43  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.11  2014/07/04 13:32:09  tergem
	Fixing from S3PDGS-1890 comments
	
	Revision 1.10  2014/04/24 16:43:28  tergem
	Porting to head
	
	Revision 1.9  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.8  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.7  2013/10/16 06:51:45  ricfer
	Metadata spec v2.0 alignment.
	
	Revision 1.6  2013/02/27 17:27:29  damdec
	Tag frameSet replaced with measurementFrameSet. Tag frame removed. Compilation warnings fixed.
	
	Revision 1.5  2012/12/21 14:31:02  damdec
	Typo corrected.
	
	Revision 1.4  2012/11/29 09:15:37  damdec
	Tag for footprint renamed.
	
	Revision 1.3  2012/06/29 11:05:30  damdec
	readXmlStream method added.
	
	Revision 1.2  2012/04/17 16:00:16  damdec
	Safe namespace and srsName passed as parameters to the constructor.
	
	Revision 1.1  2012/03/16 15:00:51  damdec
	FrameSet class added.
	
        
*/ 
#include <FrameSet.h>

#include <Filterables.h>

#include <StringUtils.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(FrameSet)

FrameSet::FrameSet(const string & safeNamespace, 
                   const string & srsName, 
                   const std::vector<Tile> &tile, 
                   const std::vector<std::pair<double, double> > & footprint, 
                   double floor_value, 
                   double ceil_value, 
                   unsigned int orbit, 
                   unsigned int track,
                   unsigned int number, 
                   const std::pair<double, double> &center, 
                   const std::string & time,
                   const std::string& safePrefix) : 
    WrappedMetadata(safeNamespace, safePrefix),
	_srsName(srsName), 
    _floor(floor_value), 
    _ceil(ceil_value), 
    _isSetCloudSystem(true), 
    _orbit(orbit), 
    _track(track), 
    _number(number), 
    _center(center), 
    _footprint(footprint), 
    _time(time), 
    _tile(tile)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("FrameSet: empty namespace. ");
	}
	if (_srsName.empty())
	{
		ACS_LOG_WARNING("FrameSet: empty srsName. ");
	}
	FrameSet::writeXmlStream();
}

FrameSet::FrameSet(const string & safeNamespace, 
                   const string & srsName, 
                   const std::vector<Tile> &tile, 
                   const std::vector<std::pair<double, double> > & footprint, 
                   unsigned int orbit, 
                   unsigned int track,
                   unsigned int number, 
                   const std::pair<double, double> &center, 
                   const std::string & time,
                   const std::string& safePrefix) : 
    WrappedMetadata(safeNamespace, safePrefix),
	 _srsName(srsName), 
     _floor(DefaultFloor), 
     _ceil(DefaultCeil), 
     _isSetCloudSystem(false), 
     _orbit(orbit), 
     _track(track), 
     _number(number), 
     _center(center), 
     _footprint(footprint), 
     _time(time), 
     _tile(tile)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING("FrameSet: empty namespace. ") ;
	}
	if (_srsName.empty())
	{
		ACS_LOG_WARNING("FrameSet: empty srsName. ") ;
	}
	FrameSet::writeXmlStream();
}

FrameSet::FrameSet(const rsResourceSet & rs) :
    WrappedMetadata("", ""),
	_srsName(""), 
	_floor(DefaultFloor), 
	_ceil(DefaultCeil), 
	_isSetCloudSystem(false), 
	_orbit(), 
	_track(), 
	_number(), 
	_center(), 
	_footprint(), 
	_time(), 
	_tile()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	FrameSet::readXmlStream(rs);
}

FrameSet::~FrameSet() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

FrameSet::FrameSet(const FrameSet& r) : 
    WrappedMetadata(r),
    _srsName(r._srsName), 
    _floor(r._floor), 
    _ceil(r._ceil), 
    _isSetCloudSystem(r._isSetCloudSystem), 
    _orbit(r._orbit), 
    _track(r._track), 
    _number(r._number), 
    _center(r._center), 
    _footprint(r._footprint), 
    _time(r._time), 
    _tile(r._tile)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

}

FrameSet& FrameSet::operator = (const FrameSet& r) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (this != &r) {
		//<Attribute copy>
        WrappedMetadata::operator=(r),
		_srsName	= r._srsName;
		_floor 		= r._floor;
		_ceil		= r._ceil;
		_isSetCloudSystem = r._isSetCloudSystem;
		_orbit 		= r._orbit;
		_track 		= r._track;
		_number 	= r._number;
		_center 	= r._center;
		_footprint 	= r._footprint;
		_time 		= r._time;
		_tile 		= r._tile;
	}
	return *this;
}

void FrameSet::writeXmlStream()
{

        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		const string &spfx = getSafePrefix() ;

        ostringstream xmlInfo;
        xmlInfo << "<xmlData>\n" ;
        xmlInfo << "\t<"<< spfx << "frameSet>\n" ;
/*        if (_isSetCloudSystem)
        {
                xmlInfo << "\t\t<cloudVoteNotationSystem floor=\""<< _floor << "\" ceil=\"" << _ceil << "\"/>\n" ;
        }


//      xmlInfo << "\t\t<frame>\n" ;
        if (0 != _orbit)
                xmlInfo << "\t\t<orbit>" << _orbit<< "</orbit>\n" ;
        if (0 != _track)
                xmlInfo << "\t\t<track>" << _track<< "</track>\n" ;
        if (0 != _number)
                xmlInfo << "\t\t<number>" << _number<< "</number>\n" ;
        //if (_center)
        {
                xmlInfo << "\t\t<center srsName=\"" << _srsName << "\">\n" ;
                xmlInfo << "\t\t\t<coordinates xmlns=\"http://www.opengis.net/gml\">" << _center.first << ", " << _center.second << "</coordinates>\n" ;
                xmlInfo << "\t\t</center>\n" ;
        }
*/
        if (!_footprint.empty())
        {
                ostringstream footprint;
		size_t fp_size = _footprint.size() ;
                for (uint i = 0; i < fp_size; i++)
                        footprint << _footprint.at(i).first << " " << _footprint.at(i).second << " " ;
                xmlInfo << "\t\t<"<< spfx << "footPrint srsName=\""<< _srsName << "\">\n" ;
                xmlInfo << "\t\t\t<gml:posList>" << footprint.str() << "</gml:posList>\n" ;
                xmlInfo << "\t\t</"<< spfx << "footPrint>\n" ;
        }
        if (!_time.empty())
                xmlInfo << "\t\t<time>" << _time<< "</time>\n" ;
    size_t tl_size = _tile.size();
        for (uint i = 0; i < tl_size; i++)
        {
                xmlInfo << "\t\t<tile row=\"" << _tile.at(i).row << "\" column=\""<< _tile.at(i).column << "\">\n" ;
                xmlInfo << "\t\t\t<cloudVote>" << _tile.at(i).cloudVote << "</cloudVote>\n" ;
                xmlInfo << "\t\t</tile>\n" ;
        }
//      xmlInfo << "\t\t</frame>\n" ;
        xmlInfo << "\t</"<< spfx << "frameSet>\n" ;
        xmlInfo << "</xmlData>\n" ;

        setXMLStream(xmlInfo.str()) ;

}


void FrameSet::readXmlStream(const rsResourceSet & rs)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (_isSetCloudSystem)
	{
		try{
			rs.getValue("xmlData.frameSet.cloudVoteNotationSystem.floor.value", _floor);
			rs.getValue("xmlData.frameSet.cloudVoteNotationSystem.ceil.value", _ceil);
		}
		catch(exException& e)
		{
			ostringstream os;
			os <<  "Mandatory field cloudVoteNotationSystem not found";
			ACS_THROW (exFrameSetException (e, os.str() ));
		}
	}
	
	if (rs.hasKey("xmlData.frameSet.orbit")) {
		rs.getValue("xmlData.frameSet.orbit", _orbit);
    }
	if (rs.hasKey("xmlData.frameSet.track")) {
		rs.getValue("xmlData.frameSet.track", _track);
    }
	if (rs.hasKey("xmlData.frameSet.number")) {
		rs.getValue("xmlData.frameSet.number", _number);
    }
	if (rs.hasKey("xmlData.frameSet.center.coordinates"))
	{
		string coord;
		rs.getValue("xmlData.frameSet.center.coordinates", coord);
		vector <string> tmp;
		StringUtils::Tokenize(coord, tmp, ",");
		
		ostringstream os; os << tmp.size();
		string cSize = os.str();
	
		ACS_COND_THROW(tmp.size()!=2, exFrameSetException("Unexpected coordinate dimension\"" + cSize + "\"") ); // PRQA S 3081
		_center = make_pair(atof(tmp[0].c_str()), atof(tmp[1].c_str()));
		
	}
	if (rs.hasKey("xmlData.frameSet.footPrint.posList"))
	{
		string coord;
		rs.getValue("xmlData.frameSet.footPrint.posList", coord);
		vector <string> tmp;
		StringUtils::Tokenize(coord, tmp, " ");
		
		ostringstream os; 
        os << tmp.size();
		ACS_COND_THROW(tmp.size()!=8, exFrameSetException("Unexpected coordinate dimension\"" + os.str() + "\"") ); // PRQA S 3081
		size_t c_size = tmp.size() ;
        for (size_t i = 0 ; i < c_size; i+=2) // PRQA S 3084, 4400
		{
			_footprint.push_back(make_pair(atof(tmp[i].c_str()), atof(tmp[i+1].c_str()))); // PRQA S 3084
		}
		
	}

	if (rs.hasKey("xmlData.frameSet.time")) {
		rs.getValue("xmlData.frameSet.time", _time);
    }

	if (rs.hasKey("xmlData.tile"))
	{
		size_t num = 0 ;
		rs.getValue("xmlData.frameSet.tile.count.value", num);
		for (size_t i = 0; i < num; i++)
		{		
			Tile tmp;
			rs.getValue("xmlData.frameSet.tile.row.value", tmp.row);
			rs.getValue("xmlData.frameSet.tile.column.value", tmp.column);
			rs.getValue("xmlData.frameSet.tile.cloudVote", tmp.cloudVote);
			_tile.push_back(tmp);
		}
	}
    
}


_ACS_END_NAMESPACE
