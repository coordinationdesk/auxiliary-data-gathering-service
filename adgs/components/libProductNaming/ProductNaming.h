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
	Revision 1.13  2014/05/28 12:25:01  damdec
	TileDescriptorKey added.
	
	Revision 1.12  2013/11/12 09:40:29  marpas
	coding best practices applied
	qa warnings and compilation warnings fixed
	
	Revision 1.11  2013/11/11 10:44:35  ricfer
	conf file handling changed.
	
	Revision 1.10  2013/07/19 15:53:41  enrcar
	EC:: Added tag CreationTimeKey
	
	Revision 1.9  2012/12/11 09:32:23  damdec
	Baseline and FrameCoord keys added.
	
	Revision 1.8  2012/10/19 09:10:11  damdec
	Timeliness field added
	
	Revision 1.7  2010/12/15 11:46:23  damdec
	Version and Instance ID keys added.
	
	Revision 1.6  2010/09/27 10:07:48  marant
	TimeFieldUtility modified to handle two kinds of special dates:
	1) 00000000T000000-like --> the returned jd50 is the min long double;
	2) 99999999T999999-like --> the returned jd50 is the max long double.
	
	Revision 1.5  2010/09/06 09:01:23  damdec
	Duration, Cycle and RelativeOrbit keys added.
	
	Revision 1.4  2010/06/25 10:34:01  damdec
	Specific exMatchNotFoundException exception introduced.
	
	Revision 1.3  2010/04/16 08:33:18  marant
	TimeFieldUtility::getJd50() modified: jd50 returned as long double. Moreover,
	in case of special input values like 99999999T99999999 the max long double value
	is returned.
	
	Revision 1.2  2010/04/12 15:41:56  marant
	TimeFieldUtility class added
	
	Revision 1.1.1.1  2010/04/12 10:02:06  marant
	Import libProductNaming
	
        
*/

#ifndef _ProductNaming_H_
#define _ProductNaming_H_

#include <exException.h>
#include <RegExp.h>
#include <rsResourceSet.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
*  This class is used to handle filenames (product and aux) following specific naming conventions.
 * The class ProductNaming provides the method getField() to retrieve pieces of the filename, such as the file type,
 * the validity start/stop, etc. provided that the filename matches with one of the configured naming rules.  
 * The mechanism implemented in this library is the following: the library is built around a configuration file
 * where naming conventions are provided in the form of a regular expression and a list of fields that can be extracted
 * from the filename using the matches with the sub-regexp enclosed in the parentesis. Thus for each
 * field, the name (which is used as a key) and the index of the sub-regexp it is related to must be provided. 
 * Given a filename, the library searches which configured naming convention is matched (the first that matches is chosen) 
 * and then uses the sub-regexp matches to return the required fields. 
 * Example: 
 * <NamingRule>
 *   <FilenameRegExp>^CS_([[:alpha:]]{4})_([A-Za-z0-9_]{10})_([[:digit:]]{8}T[[:digit:]]{6})_([[:digit:]]{8}T[[:digit:]]{6})_[[:digit:]]{4}[.][[:alnum:]]*$</FilenameRegExp>
 *   <List_Of_Fields count="4">
 *      <Field>
 *          <Name>FileClass</Name>
 *          <Index>1</Index>
 *      </Field>
 *      <Field>
 *          <Name>FileType</Name>
 *          <Index>2</Index>
 *      </Field>
 *      <Field>
 *          <Name>StartTime</Name>
 *          <Index>3</Index>
 *      </Field>
 *      <Field>
 *          <Name>StopTime</Name>
 *          <Index>4</Index>
 *      </Field>
 *  </List_Of_Fields>
 * </NamingRule>
 *
 * This naming rule allows to retieve the file class as the match with the first sub regexp; the file type is the result of the match with
 * the second sub-regexp, etc..
 * The list of possible field names are declared as static members of the class ProductNaming.
 * Note that a sub-regexp is enclosed in parentesis ()  
 * 
 * Each time a new field is needed, you must update the list of static members FieldKey and the method init() pushing the new key into the 
 * vector fieldKeys. The class NamingRules has not to be updated.
**/


class NamingRules;    //forward declaration
struct FilenameField; //forward declaration

class ProductNaming  {
   
    
public:

            
    typedef  std::string FieldKey;
    // PRQA S 2100 L1
    /** The following list defines the keys to be used to retrieve the fields of the filename**/
    static const FieldKey PlatformNameKey;
    static const FieldKey MissionNumberKey;
    static const FieldKey FileTypeKey;
    static const FieldKey FileClassKey;
    static const FieldKey StartTimeKey;
    static const FieldKey StopTimeKey;
	static const FieldKey CreationTimeKey;
	static const FieldKey OriginFacilityKey;
    static const FieldKey AbsoluteOrbitKey;
    static const FieldKey CrcKey;
    static const FieldKey ExtensionKey;
    static const FieldKey DurationKey;
    static const FieldKey CycleKey;
    static const FieldKey RelativeOrbitKey;
    static const FieldKey InstanceIdKey;
    static const FieldKey VersionKey;
    static const FieldKey TimelinessKey;
    static const FieldKey BaselineKey;
    static const FieldKey FrameCoordKey;
    static const FieldKey TileDescriptorKey;
    static const FieldKey ValidityKey;
    static const FieldKey ImageTypeKey;
    static const FieldKey ResolutionKey;
    // PRQA L:L1
    	/** ProductNaming Exceptions */
	exDECLARE_EXCEPTION(exProductNamingException, exException) ; // Base ProductNaming Exception. // PRQA S 2131, 2502 2
    exDECLARE_EXCEPTION(exMatchNotFoundException, exProductNamingException) ; // Exception raised when the filename doesn't match any configured regular exp // PRQA S 2153
    

	/** Class constructor: needs the filename to be parsed */
	explicit ProductNaming(const std::string& filename, const std::string& confFileName="");
	
	ProductNaming(const std::string& filename, rsResourceSet& rset, const std::string& rootTag = "");
	/** Destructor */
	virtual ~ProductNaming() throw() ;

    /** return the field of the filename corresponding to the input key. An empty string is returned if 
        the field is not foreseen in the applied naming convention.
     */
    std::string getField(const FieldKey&);
   	

protected:

private:
    
    ProductNaming(); // not implemented
    ProductNaming(const ProductNaming & ); // not implemented
    ProductNaming &operator=(const ProductNaming &); // not implemented

    //naming rules read from the configuration file
    NamingRules* _namingRules;

    //perform initialization
    void init(rsResourceSet& rset, const std::string& rootTag);
    //parse the filename according to the configured naming rules
    void parse(const std::string& filename) ;
    //ancillary method used by parse() 
    void retrieveFields(const RegExp::Matches& matches, const std::vector<FilenameField>& fields);
    //map used to store the results of the filename parsing in the form <field_key, field_value>
    std::map<std::string, std::string> _fieldsMap;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProductNaming)

};

/**
 * Utility Class to handle time fields
 * */
class TimeFieldUtility {

public:

    /** convert the input date string into jd50,
     *  return true if the input date is valid
     *  return false if the input date matches one of the special dates provided in the vectors
     *  _minSpecialDates/_maxSpecialDates. 
     *  If the input date is one of the _minSpecialDates (like 00000000T000000) the returned long double is the min long double;
     *  If the input date is one of the _maxSpecialDates (like 99999999T999999) the returned long double is the max long double;
     *  Throws an exception in case of date time not valid
     * */
    static bool getJd50(const std::string&, /*output*/ long double&);
private:
    TimeFieldUtility();

    static std::vector<std::string> _minSpecialDates;
    static std::vector<std::string> _maxSpecialDates;


    ACS_CLASS_DECLARE_DEBUG_LEVEL(TimeFieldUtility)
};


_ACS_END_NAMESPACE

#endif //_ProductNaming_H_

