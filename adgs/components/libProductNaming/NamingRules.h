// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libProductNaming$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2013/11/12 11:23:59  marpas
	class do not longer inherit from STL container
	
	Revision 1.3  2013/11/12 09:40:29  marpas
	coding best practices applied
	qa warnings and compilation warnings fixed
	
	Revision 1.2  2013/11/11 10:42:52  ricfer
	rsResourceSet including the productNaming conf file passed directly to the load NamingRules method.
	
	Revision 1.1.1.1  2010/04/12 10:02:06  marant
	Import libProductNaming
	
        
*/

#ifndef _NamingRules_H_
#define _NamingRules_H_

#include <exException.h>
#include <rsResourceSet.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* NamingRules is an ancillary class used by ProductNaming to load the configuration
**/
typedef std::string FilenameRegExp;

struct FilenameField { // PRQA S 2173, 2175
    FilenameField() : name(), index() {};
    FilenameField(const std::string & n, const unsigned char & i) : name(n), index(i) {};
    
    std::string name; // PRQA S 2100 2
    unsigned char index;
};

class Rule
{
public:
	Rule() : _regExp(), _fileFields(){};
	Rule(const FilenameRegExp &regExp, const std::vector<FilenameField> & fields) : _regExp(regExp), _fileFields(fields) {};
	virtual ~Rule() throw();
        Rule(const Rule & r) : _regExp(r._regExp), _fileFields(r._fileFields) {}; 
        const Rule& operator= (const Rule& i)
        {
            if (this != &i)
            {
                _regExp = i._regExp;
                _fileFields= i._fileFields;
            }
            return *this;
        }
	FilenameRegExp getRegExp() const {return _regExp;} ;
	const std::vector<FilenameField> & getFileFields() const{return _fileFields;};
private:
	FilenameRegExp _regExp;
	std::vector<FilenameField> _fileFields;
};

template<> inline
void rsResourceSet::getValue<FilenameField>(const std::string &res,  FilenameField&v) const // template spacialization // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
    std::string a("");
    unsigned char c(0);
    if (res != "")
    {
        getValue(res+".Name")->get(a) ;
        getValue(res+".Index")->get(c) ;
    }
    else
    {
        getValue("Name")->get(a) ;
        getValue("Index")->get(c) ;
    }
        
   v = FilenameField(a,c);     
}
template<> inline
void rsResourceSet::getValue<Rule>(const std::string &res,  Rule&v) const // template spacialization // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
    FilenameRegExp regExp;
    std::vector<FilenameField> fileFields;

    if (res != "")
    {
        getValue(res+".FilenameRegExp")->get(regExp) ;
        getArray<FilenameField>(res+".List_Of_Fields.Field", fileFields) ;
    }
    else
    {
        getValue("FilenameRegExp")->get(regExp) ;
        getArray<FilenameField>("List_Of_Fields.Field", fileFields) ;
    }
        
   v = Rule(regExp,fileFields);     
}

class NamingRules   // PRQA S 2109
{ 
public:

  
	/** NamingRules Exceptions */
	exDECLARE_EXCEPTION(exNamingRulesException, exException) ; // Base NamingRules Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exNamingRulesCriticalException, exNamingRulesException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	NamingRules();
	/** Destructor */
	virtual ~NamingRules() throw() ;

    void load(rsResourceSet& _rsConf); //read and load the configured naming rules

    //get the filename regexp of the n-th naming rule 
    FilenameRegExp getFilenameRegExp(size_t i) const 
    {
        return _rs_x_fn.at(i).first;
    };
    
    //get the fields info of the n-th naming rule
    const std::vector<FilenameField> &getFilenameFields(size_t i) const throw()
    {
        return _rs_x_fn.at(i).second;
    };

    size_t size() const throw() { return _rs_x_fn .size() ; }
    
private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	NamingRules(const NamingRules & ); // not implemented
	/** Operator = */
	NamingRules &operator=(const NamingRules &); // not implemented
	
    static void printFields(const FilenameField &) ;

private:

    //list of allowd field names
    //std::vector<std::string> _allowedFieldNames;
    std::vector<std::pair<FilenameRegExp, std::vector<FilenameField> > > _rs_x_fn ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(NamingRules)

};


_ACS_END_NAMESPACE

#endif //_NamingRules_H_

