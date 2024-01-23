// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
        
*/

#ifndef _ManifestParametersListBuilder_H_
#define _ManifestParametersListBuilder_H_

#include <UtilXml.h>
#include <rsResourceSet.h>
#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* This class, on the base of a versions set read from Manifest file, reads a Resource set object to build a parameters list with the proper xpath to extract from (or alternatevely default values)
**/

class ManifestParametersListBuilder 
{
public:

	/** ManifestParametersListBuilder Exceptions */
	exDECLARE_EXCEPTION(exManifestParametersListBuilderException, exException); // ManifestParametersListBuilder Exception.

	/*! \brief This structure contains the values extracted from xml Parameter nodes 	 
	*/
	class Parameter {
    	public:
		Parameter() ;
		~Parameter() throw() {} 
		Parameter(const Parameter& ) ;
		Parameter& operator=(const Parameter& ) ;
	    	void setname(const std::string & s ) throw() { _name = s ; }
	    	void setxpath(const std::string & s ) throw() { _xpath = s ; }
	    	void setdefaultVal(const std::string & s ) throw() { _defaultVal = s ; }

	protected:
	private:
		std::string _name;
		std::string _xpath;
		std::string _defaultVal;
	} ;
	
	typedef std::map< std::string, std::pair<std::string, std::string> >::const_iterator ParListIterator ;


	/** Default Class constructor */
	ManifestParametersListBuilder();
	/** Destructor */
	virtual ~ManifestParametersListBuilder() throw();

	 /**
	 * \brief  Read manifest file and extract versions from xfdu:XFDU node
	 * \param manifest path
	 */    	
	virtual void readManifestVersions(const std::string & manifestPath);
	
	 /**
	 * \brief  Depending on current versions set, build parameters list from input ResourceSet
	 * \param  rsResourceSet object to read
	 * \param  Additional key (e.g. filetype)
	 */    	
	void buildParametersList(const rsResourceSet &, const std::string & key = std::string());

	 /**
	 * \brief  returns parameter value as string, or empty string if not found
	 * \param  parName parameter name.
	 * \retval parameter value
	 */    	
	std::string getParameter(const std::string & parName) const;

	/**
	 * \brief  Returns iterator to begin of parameters list
	 * \retval iterator to begin of parameters list
	 */    	
	ParListIterator getBeginDataIterator() const throw() { return _parPaths.begin() ; }

	/**
	 * \brief  Returns iterator to end of parameters list
	 * \retval iterator to end of parameters list
	 */    	
	ParListIterator getEndDataIterator() const throw() { return _parPaths.end() ; }

	/**
	 * \brief  Get parameter name and return its value as a string, or empty string if not found
	 * \param  iterator parameters list
	 * \param  parName OUT parameter name
	 * \retval parameter value
	 */    	
	std::string getParameter(ParListIterator, std::string & parName) const;

protected:

private:
    	/* copy constructor and operator= defined but not implemented */
	/** Copy Constructor */
	ManifestParametersListBuilder(const ManifestParametersListBuilder & );
	/** Operator = */
	ManifestParametersListBuilder &operator=(const ManifestParametersListBuilder &);
	
	 /**
	 * \brief  Get version numbers from a single configuration file node
	 * \param  OUT VersionNumV
	 * \param  versions string with space separated versions
	 */    	
	void getVersionNum(std::vector<int> & VersionNumV, const std::string & versions);

	 /**
	 * \brief  it returns true if any version is greater than manifest version
	 * \param  versions string with space separated versions
	 */    	
	bool AnyGreaterThanManifestVersion(const std::string & currentVersions);

	std::vector<int> _versionNumV;
	XmlDoc _xmlDoc;
	std::map< std::string, std::pair<std::string, std::string> > _parPaths;
	static Mutex _mapMutex ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ManifestParametersListBuilder)

};

template<>
void rsResourceSet::getValue(const std::string &res, ManifestParametersListBuilder::Parameter &) const ;

_ACS_END_NAMESPACE

#endif //_ManifestParametersListBuilder_H_

