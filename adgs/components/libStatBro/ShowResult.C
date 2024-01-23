// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. StatBro Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2016/05/10 14:48:00  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.1  2015/06/12 15:01:40  marpas
	APF-269 work in progress
	
	Revision 5.0  2013/07/02 11:22:11  marpas
	adopting libException 5.x standards
	coding best practices
	qa warnings
	
	Revision 2.3  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.2  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.1  2010/04/26 10:26:03  marpas
	preparation for ORACLE integration. (need new libDbGeo ORACLE aware)
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2004/12/06 14:24:07  marpas
	headers fixed
	
	Revision 1.10  2004/10/15 16:54:03  marpas
	label IgnoredColumn supported to ignore a column an force MULTISET not empty
	trailing and leading spaces from labels stripped away
	
	Revision 1.9  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.8  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.7  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.6  2003/10/07 16:39:47  paoscu
	init method added.
	
	Revision 1.5  2003/04/17 14:09:15  marpas
	GCC3.2
	
	Revision 1.4  2003/03/07 10:23:35  marpas
	ShowResult rebirth
	
	Revision 1.1  2003/03/07 10:06:44  paoscu
	ShowResult moved from libStatBro
	
	Revision 1.2  2003/02/26 13:47:33  marpas
	stable
	
	Revision 1.1.1.1  2003/02/14 10:16:00  marpas
	Import libStatBro
	
	

*/

#include <ShowResult.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ShowResult)


ShowResult::ShowResult() :
    _labels(),
	_attributes(),
	_subRecords(),
	_queryClass(),
    _dateFormat(DateWUtc),
    _numOfShowQueries(),
    _numOfShowRecords()
{
}


ShowResult::~ShowResult() throw() 
{
}

void ShowResult::showResult(const string &) 
{
}

void ShowResult::setNumShowQueries(int n) 
{
    _numOfShowQueries = n ;
}


void ShowResult::init(rsResourceSet &)
{
	//Base class does nothing ;
}


void ShowResult::setDateFormat(XmlDateFormat f) throw()  // PRQA S 4121
{
    _dateFormat = f ;
}

void ShowResult::setDateFormat(const std::string &s) // PRQA S 4020
{
    {
        ostringstream os ; 
        os << PlainDateToSecZ ; 
        if (os.str() == s) {
            setDateFormat(PlainDateToSecZ) ;
            return ;
        }
    }    
    {
        ostringstream os ; 
        os << DateWUtc ; 
        if (os.str() == s) {
            setDateFormat(DateWUtc) ;
            return ;
        }
    } 
    
    ACS_THROW(InvalidUse("Invalid date format: \"" + s + "\n")) ; // PRQA S 3081
}

ShowResult::XmlDateFormat ShowResult::getDateFormat() const throw() // PRQA S 4120
{
    return _dateFormat ;
}


string ShowResult::getNthElement(unsigned long i,vector<string> const &elements) // PRQA S 4020
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream dbg ;
        dbg << exDebugSignature << "i="<<i<<" size="<< elements.size();
		if( i < elements.size() ) {
			dbg<<" label= "<<elements[i] ;
        }
        ACS_LOG_DEBUG(SimpleDebugSignature << dbg.str()) ;
		
	ACS_CLASS_END_DEBUG
	
	if( i < elements.size() ) {
		return elements[i];
    }
	return "" ;
}


void ShowResult::parseSet(rsResourceSet &rs, size_t index) 
{
	vector<string> v ;
	
	try{
		string labels ;
		rs.getValue("labels", labels) ;
		istringstream is(labels);
		char buffer[labels.size() + 1]; // PRQA S 3084
		while(is.good())
		{
			is.getline(buffer , sizeof(buffer) + 1 , ',') ; // PRQA S 3084
			if((is.gcount() > 0) || is.good() ) {
				string s = buffer ;
				v.push_back(rsResourceSet::strip(s)) ;
			}
		}
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
            size_t vsize = v.size() ;
			for(size_t i=0; i<vsize ; ++i) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[" << index << "] " << "label["<<i <<"] = "<< v[i]) ;
            }
		ACS_CLASS_END_DEBUG
	}
	catch(rsResourceSet::NotFoundException &e) {}

	_labels[index] = v ;


	v.clear() ;
	try{
		string attributes;
		rs.getValue("attributes", attributes) ;
		if (attributes!= "") {
			istringstream is(attributes);
			char buffer[attributes.size() + 1]; // PRQA S 3084
			while(is.good())
			{
				is.getline(buffer , sizeof(buffer) + 1 , '|') ; // PRQA S 3084
				if((is.gcount() > 0) || is.good() ) {
					string s = buffer ;
					v.push_back(rsResourceSet::strip(s)) ;
				}
			}

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
                size_t vsize = v.size() ;
				for(size_t i=0; i<vsize ; ++i) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[" << index << "] " << "attr["<<i <<"] = "<< v[i]) ;
                }
			ACS_CLASS_END_DEBUG
		}
	}
	catch(rsResourceSet::NotFoundException &e) {}

	_attributes[index] = v ;
	
	string record = "record" ;
	try {
		rs.getValue("recordName", record) ;
	}
	catch(rsResourceSet::NotFoundException &e) {}
	_subRecords[index] = record ;

}




void ShowResult::parse(rsResourceSet &rs, string const & nameSpace)
{
	_labels.clear() ;
	_attributes.clear() ;
	_subRecords.clear() ;
	
	rsPushNSpace nsp1(rs, nameSpace) ;

	
	parseSet(rs, 0) ;
	
	// multiset management
	size_t numMultiset = rs.getArraySize("multiset") ; // PRQA S 4412
	string *nameMultiset = new string[numMultiset] ;
	try {
		if (numMultiset > 0) {
			rs.getArray("multiset",nameMultiset,numMultiset) ;
		}
		for (size_t i=0; i < numMultiset; i++) {
			rsPushNSpace nsp(rs, nameMultiset[i]) ;
			parseSet(rs,i+1) ; // PRQA S 3084

		}
	}
	catch(rsResourceSet::NotFoundException &e)
	{
		;
	}
	
	delete [] nameMultiset ;
	

}

const vector<string> &ShowResult::getLabels(size_t i) 
{
	return _labels[i] ;
}

const vector<string> &ShowResult::getAttributes(size_t i) 
{
	return _attributes[i] ;
}

const string & ShowResult::getSubRecordLabel(size_t i) 
{
	return _subRecords[i] ;
}

void ShowResult::setQueryClass(const string &s) 
{
	_queryClass = s ;
}

const string ShowResult::getQueryClass() const 
{
	return _queryClass ;
}


std::ostream &operator << (std::ostream &o, ShowResult::XmlDateFormat f) 
{
    switch(f) {
    case ShowResult::DateWUtc: { o << "DateWUtc" ; break ; }
    case ShowResult::PlainDateToSecZ: { o << "PlainDateToSecZ"; break ; }
    }
    return o ; // PRQA S 4028
}

exostream &operator << (exostream &o, ShowResult::XmlDateFormat f)  // PRQA S 4222
{
    switch(f) {
    case ShowResult::DateWUtc: { o << "DateWUtc" ; break ; }
    case ShowResult::PlainDateToSecZ: { o << "PlainDateToSecZ"; break ; }
    }
    return o ; // PRQA S 4028
}

_ACS_END_NAMESPACE

