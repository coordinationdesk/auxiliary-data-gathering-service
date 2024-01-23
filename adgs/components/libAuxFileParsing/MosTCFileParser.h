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

	$Prod: A.C.S. libAuxFileParsing$

	$Id$

	$Author$

	$Log$
	Revision 2.4  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.3  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 2.2  2009/07/13 13:33:55  marant
	Class MosTCFileParser re-implemented: an xml file is used to store the MOS
	time correlation info plus some other time info coming from the L0 processing
	
	Revision 2.1  2007/07/25 17:46:42  nicvac
	stable and tested.
	
        
*/

#ifndef _MosTCFileParser_H_
#define _MosTCFileParser_H_

#include <OBTFileParser.h>
#include <MosTimeCorrParams.h>
#include <set>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)
    
/**
 * \brief
 * This class specifics the parsing strategy for Mos Time Correlation File
 **/

class MosTCFileParser : public OBTFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	MosTCFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary TimeCorrelation file path
	 */	
	explicit MosTCFileParser(const std::string&);

	/**
	 * \brief  Class copy constructor 
	 */ 
	MosTCFileParser(const MosTCFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	MosTCFileParser &operator=(const MosTCFileParser &);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~MosTCFileParser() throw() ;

	/**
	 * \brief  Set the TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string& name);

	/**
	 * \brief  Remove the TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string& name);


	/**
	 * \brief  Return the TimeCorrelation file name path
	 */
	virtual std::vector<std::string> getFileNames() { return _fileNamesVec; } // PRQA S 2131


	/**
	 * \brief  Return the TimeCorrelation file type loaded from Configuration File 
	 */
	virtual std::string getFileType() { return _fileType; } // PRQA S 2131

	/**
	 * \brief  Parse the Time Correlation File using the MosTimeCorrParams vector in input to store
	 *		   the results.
	 * \param  A vector of MosTimeCorrParams to store params values during file parsing
	 */
	virtual void parse(std::vector<Parameters*>&);

    /**
	 * \brief  Prints the time correlations on the input stream formatted in XML as expected in the Mos Time Correlation file
	 * \param  os: ostream on which the paramters are written (in XML format) 		   
	 * \param  Parameters* the time parameters to be written; 
     */
    static void print(std::ostream*, MosTimeCorrParams*);


private:

	void buildParams();
	void parseFile(const std::string&);

	void cleanupParams();

	std::set<std::string>		_fileNames;
	std::string 				_fileType;
	std::vector<std::string>	_fileNamesVec;

/////////////////////////////////////////////////
//Variables used to extract parameters from file		
/////////////////////////////////////////////////
	std::vector<MosTimeCorrParams*>	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MosTCFileParser) ;
};


_ACS_END_NAMESPACE

#endif //_MosTCFileParser_H_

