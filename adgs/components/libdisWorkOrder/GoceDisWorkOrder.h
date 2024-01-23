/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DIWorkOrder Library $

	$Id$

	$Author$
	
	$Log$
	Revision 1.9  2012/04/20 07:04:01  giucas
	Compilation warnings removed
	
	Revision 1.8  2005/06/23 13:27:58  giucas
	Added getFileTypes method
	
	Revision 1.7  2004/12/03 10:36:59  giucas
	Added remove and get file methods
	
	Revision 1.6  2004/12/03 09:42:37  giucas
	Header fixed
	
	Revision 1.5  2004/11/17 15:39:39  giucas
	Changed handling and interfaces for Auxiliary Files
	
	Revision 1.4  2004/11/15 17:34:14  giucas
	Changed getAuxInputData sign and solved bug
	
	Revision 1.3  2004/11/15 15:54:46  giucas
	Added getAuxInputData method filtering for a specific FileType
	
	Revision 1.2  2004/10/14 09:37:46  giucas
	Added get and set AuxInputData methods
	
	Revision 1.1  2004/10/11 14:10:43  giucas
	First release
	

*/

#ifndef _GoceDisWorkOrder_H_
#define _GoceDisWorkOrder_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <rsResourceSet.h>
#include <XMLOstream.h>
#include <disWorkOrder.h>
#include <set>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)

//! the class GoceDisWorkOrder is in charge to manage  
//! the DIWorkOrder file for Goce when it is merged in the configuration 
//! space.
class GoceDisWorkOrder : public disWorkOrder
{
	public:
		/**
		 * \brief  Class constructor
		 * \param Flag to indicate using configuration from ConfigurationSingleton or not.
		 *		  If true (default value) the DIWorkOrder data are loaded from ConfigurationSingleton.
		 */
    	GoceDisWorkOrder(bool UseConfSet= true);  
		/**
		 * \brief  Class destructor
		 */
    	virtual ~GoceDisWorkOrder() throw();

		/**
		 * \brief  Base show method reimplemented to show the Goce specific info
		 */
    	void show();

		/**
		 * \brief  operator>>
		 */
    	GoceDisWorkOrder& operator>>(std::ostream & os);


		/**
		  * \brief Add a file to the list of time-ordered Auxiliary files.
		  * \param The file type
		  * \param The file name
		  * \param The file validity start time in TC_ASCII_CCSDSA_MICROSEC
		  * \param The file validity stop time in TC_ASCII_CCSDSA_MICROSEC
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
		  * \brief Return the filename of a given filetype in list which validity start/stop time interval covers
		  *		   the input interval and the start times interval is min
		  * \param The file type
		  * \param The file validity start time
		  * \param The file validity stop time
		  * \retval The file name
		  */	
		std::string getMinCoverFile(const std::string&, const double&, const double&);
	
		/**
		  * \brief Return the list of all files
		  */	
		std::vector<std::string> getFiles();

		/**
		  * \brief Return the list of all files for a given file type
		  * \param The input File Type
		  */	
		std::vector<std::string> getFiles(const std::string&);

		/**
		  * \brief Return the list of all filetypes
		  */	
		std::vector<std::string> getFileTypes();
	
	private:

		/**
		 * \brief  The AuxFileInfo stores data for Goce spcecific input Auxiliary Files
		 */
		class AuxFileInfo
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
				~AuxFileInfo();

				/**
				 * \brief  Copy constructor
				 */
				AuxFileInfo(const AuxFileInfo&);

			//operators
				AuxFileInfo& operator=(const AuxFileInfo&);
				bool operator<(const AuxFileInfo &) const ;
				bool operator<=(const AuxFileInfo &) const ;
				bool operator==(const AuxFileInfo &) const ;

			//attributes
				std::string	fileType;
				std::string	fileName;
				double		valStart;
				double		valStop;
				std::string	valStartString;
				std::string	valStopString;

			//methods
				std::string show() const ;
				void exportXML(XMLOstream &) const ;
				
			private:
				//convert input string in UTC TC_ASCII_ENVI_MICROSEC to UTC double MJD2000
    			double& convertTime(const std::string&, double&);

					
		};

		struct ltAuxInfo
		{
			bool operator()(const AuxFileInfo& x, const AuxFileInfo& y) const
			{
				return x < y;
			}
		};
		
		
		int numberOfFiles();
		
		rsResourceSet _confSet ;


		typedef std::set<AuxFileInfo,ltAuxInfo> TimeOrderedAuxList;
		//this map associate a list of "time-ordered" files to a specific file type
		std::map<std::string, TimeOrderedAuxList> _auxInfoData;
		
		//associate to a fileType the minValStart and maxValStop values
		std::map<std::string, std::pair<double,double> > _auxInfoDataRange;
		
		ACS_CLASS_DECLARE_DEBUG_LEVEL(GoceDisWorkOrder) ;
};

_ACS_END_NAMESPACE


#endif // _GoceDisWorkOrder_H_
