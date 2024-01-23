// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libOrderCreator $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/02/11 12:50:20  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.4  2012/10/26 10:21:20  marpas
	TriggeringUserDistribution added to TriggeringFile
	
	Revision 2.3  2012/09/17 17:07:49  marpas
	new method to get conf values,
	new class TriggeringFile with triggering filename, fileversion and orderid - 0 if was null
	updated XMLOrderParameters class, writes also the generating orderid for the triggering file(s)
	update TemporaryOrder with new queries and a method to get the generating orderid for the triggering file
	
	Revision 2.2  2012/03/07 13:32:18  marpas
	parameters file management improved and i/f changed
	
	Revision 2.1  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	

*/
#include <XMLOrderParameters.h>
#include <algorithm>
#include <fstream>


_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;

XMLOrderParameters::XMLOrderParameters() :
    xmlos_(),
	xml_(xmlos_, "Parameter_File")
{
}


XMLOrderParameters::~XMLOrderParameters() throw() {	}

void XMLOrderParameters::write(ostream &os) 
{
    xml_.close() ; // cause closure of all tags
    os << xmlos_.str() ;
}

XMLOstream &XMLOrderParameters::xml() // PRQA S 4120
{
    return xml_ ; // PRQA S 4024
}

class tagInserter { // PRQA S 2109
public:
    tagInserter(XMLOstream *x, const std::string &user) : xml_(x),user_(user)  {}
    ~tagInserter() throw() {}
    void operator() (const TriggeringFile &tf) // PRQA S 4211
    {
        XMLOstream::Tag tfile(*xml_ , "TriggeringFile" , "");
        XMLOstream::Tag fname(*xml_ , "FileName" , tf.filename,"", true);
        XMLOstream::Tag fversion(*xml_ , "FileVersion" , tf.fileversion,"", true);

        if (not tf.filetype.empty()) {
            XMLOstream::Tag ftype(*xml_ , "FileType" , tf.filetype,"", true);
        }

        XMLOstream::Tag genorder(*xml_ , "GeneratingOrder" , tf.orderid,"", true);
        XMLOstream::Tag truserdistr(*xml_ , "TriggeringUserDistribution" , user_,"", true);
    }
    tagInserter(const tagInserter &r) : xml_(r.xml_), user_(r.user_) {}
private:
    tagInserter() ; // not implemented
    tagInserter &operator=(const tagInserter &r) ; // not implemented
   
private:
    XMLOstream *xml_ ;
    std::string user_ ;
} ;

void XMLOrderParameters::setTriggeringFiles(const vector<TriggeringFile > & tf, const string &user) 
{

    ostringstream oscount ;
    oscount << "count=\"" << tf.size() << "\"" ;
    XMLOstream::Tag listOfTriggering(xml_ , "List_of_TriggeringFiles" , "", oscount.str()); 
    for_each(tf.begin(), tf.end(), tagInserter(&xml_, user)) ;
}

_ACS_END_NAMESPACE
