// PRQA S 1050 EOF
/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. AuxFileList $

	$Id$

	$Author$
	
	$Log$
	Revision 2.1  2013/11/19 13:36:15  ricfer
	Compiler warnings removed. New logging macros adopted.
	
	Revision 2.0  2006/03/01 16:41:23  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.1.1.1  2006/01/31 13:44:10  seralb
	Imported libAuxFileHandlingBase
	
	Revision 1.1.1.1  2004/12/06 11:21:40  giucas
	Imported libAuxFileHandling
	

*/

#ifndef _AuxFileList_H_
#define _AuxFileList_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <XMLOstream.h>
#include <map>
#include <set>


_ACS_BEGIN_NAMESPACE(acs)

class AuxFileList // PRQA S 2109
{
public:

	/**
	 * \brief  Class constructor
	 */
    AuxFileList();  
	/**
	 * \brief  Class destructor
	 */
    ~AuxFileList() throw() ;

	/**
	 * \brief  Class copy constructor
	 */
    AuxFileList(const AuxFileList&);  

	/**
	 * \brief  Class operator =
	 */
    AuxFileList& operator=(const AuxFileList&);

	/**
	  * \brief Add a file to the list of time-ordered Auxiliary files.
	  * \param The file type
	  * \param The file name
	  * \param The file validity start time in TC_ASCII_COMPACT_MICROSEC (yyyymmdd_hhnnssuuuuuu)
	  * \param The file validity stop time in TC_ASCII_COMPACT_MICROSEC (yyyymmdd_hhnnssuuuuuu)
	  */	
	void addFile(const std::string&,const std::string&, const std::string&, const std::string&);

	/**
	  * \brief Remove a file to the list of time-ordered Auxiliary files.
	  * \param The file type
	  * \param The file name
	  */	
	void removeFile(const std::string&,const std::string&);

	/**
	  * \brief Remove all the files for a given FileType
	  * \param The file type
	  */	
	void removeFile(const std::string&);

	/**
	  * \brief Return the filename of a given filetype which validity start/stop time interval covers
	  *		   the input interval and the start times interval is min
	  * \param The file type
	  * \param The file validity start time
	  * \param The file validity stop time
	  * \retval The file name
	  */	
	std::string getMinCoverFile(const std::string&, long double, long double) const ;

	/**
	  * \brief Return the list of all files
	  */	
	std::vector<std::string> getFiles() const ;

	/**
	  * \brief Return the list of all files for a given file type
	  * \param The input File Type
	  */	
	std::vector<std::string> getFiles(const std::string&) const ;

	/**
	  * \brief Return the number of files added to list
	  */	
	size_t numberOfFiles() const ;

	/**
	  * \brief Return the Validity Start of the first element in list for a given file type
	  */	
	long double getMinValidityStart(const std::string&) const ;

	/**
	  * \brief Return the Validity Stop of the last element in list for a given file type
	  */	
	long double getMaxValidityStop(const std::string&) const ;

	/**
	  * \brief Return the File name, Validity Start and Stop times of the first element for a given file type
	  */	
	void getMinFileInfo(const std::string&, std::string&, long double&, long double&) const ;




	/**
	 * \brief  The AuxFileInfo stores data for Goce spcecific input Auxiliary Files
	 */
	class AuxFileInfo // PRQA S 2109
	{
		public:
			/**
			 * \brief  Default Class constructor
			 */
			AuxFileInfo();

			/**
			 * \brief  Fill object information
			 * \param File Type
			 * \param File Name
			 * \param File Validity Start
			 * \param File Validity Stop
			 */
			AuxFileInfo(const std::string&,const std::string&, const std::string&, const std::string&);

			/**
			 * \brief Class destructor
			 */
			~AuxFileInfo() throw() ;

			/**
			 * \brief  Copy constructor
			 */
			AuxFileInfo(const AuxFileInfo&);

		//operators
			AuxFileInfo& operator=(const AuxFileInfo&);

		//attributes
			std::string	fileType; // PRQA S 2100 6
			std::string	fileName;
			long double valStart;
			long double valStop;
			std::string	valStartString;
			std::string	valStopString;

		//methods
			std::string show() const ;
			void exportXML(XMLOstream &) const ;

		private:
			//convert input string in UTC TC_ASCII_ENVI_MICROSEC to UTC double MJD2000
    		long double& convertTime(const std::string&, long double&);


	};

private:
	struct ltAuxInfo
	{
		bool operator()(const AuxFileInfo& x, const AuxFileInfo& y) const; 
	};


	typedef std::set<AuxFileInfo,ltAuxInfo> TimeOrderedAuxList;
	//this map associate a list of "time-ordered" files to a specific file type
	std::map<std::string, TimeOrderedAuxList> _auxInfoData;

	//associate to a fileType the minValStart and maxValStop values
	std::map<std::string, std::pair<long double,long double> > _auxInfoDataRange;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AuxFileList);
};


inline bool operator<=(const AuxFileList::AuxFileInfo &a, const AuxFileList::AuxFileInfo &b) // PRQA S 2134 
{ 
	return a.valStart <= b.valStart ; 
}

inline bool operator<(const AuxFileList::AuxFileInfo &a, const AuxFileList::AuxFileInfo &b)  // PRQA S 2134 
{ 
	return a.valStart < b.valStart ; 
}

inline bool operator==(const AuxFileList::AuxFileInfo &a, const AuxFileList::AuxFileInfo &b)   // PRQA S 2134
{ 
	return (a.fileName == b.fileName) && (a.valStart == b.valStart) && (a.valStop == b.valStop); // PRQA S 3270 
}

inline	bool AuxFileList::ltAuxInfo::operator()(const AuxFileList::AuxFileInfo& x, const AuxFileList::AuxFileInfo& y) const // PRQA S 2133, 2134
{
	return x < y;
}

_ACS_END_NAMESPACE




#endif // _AuxFileList_H_
