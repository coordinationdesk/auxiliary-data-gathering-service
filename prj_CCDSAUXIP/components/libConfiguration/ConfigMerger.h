// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2017/09/27 11:19:45  lucio.pulvirenti
	S2PDGS-1843: methods added to manage configuration in input filepath and relative attributes added. mergeResourceSpace and
	mergeResourceSet methods added to be used both in conf space case and in filepath one.
	
	Revision 5.0  2013/06/18 10:14:18  marpas
	adoption of libException 5.x standards
	
	Revision 2.7  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.6  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.5  2012/02/28 16:18:42  marpas
	minor change
	
	Revision 2.4  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.3  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.2  2008/11/13 13:47:04  marpas
	StopPredicate introduced
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
		

*/


#ifndef _ConfigMerger_H_
#define _ConfigMerger_H_ 

#include <ConfigurationSingleton.h>
#include <Statistics.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)


class rsResourceSet ;
class ResourceSpace;

/*! This class is used to retrieve some Configuration spaces and merge them into a single resource set.
*/
class ConfigMerger // PRQA S 2109
{
public:
	explicit ConfigMerger(const std::vector<std::pair<std::string, std::string> > &);
	ConfigMerger();
	virtual ~ConfigMerger() throw() ;
	
	/*! \brief fills the given resource set.
	
		It fills the given resource set with the content of all the configuration spaces 
		added with the addSpace method.
		If the given resource set is not empty, the method replaces the keys (uses rsResourceSet::ModeChange mode).
		See also rsResourceSet::MergeMode. The method could be time consuming, 
		so the predicate can be used if one would stop the method in case an external condition should be verified
		\param rs the resource set that will be filled
		\param stop the predicate that will stop the function if evaluates to true 
		
		In case of stop, throws a ConfigurationSingleton::Stopped exeption
	*/
	virtual void getResources(rsResourceSet &rs, const ConfigurationSingleton::StopPredicate &stop = ConfigurationSingleton::StopPredicate()) ;
	
	/*! \brief fills the given resource set
	
		It fills the given resource set with the content of all the file paths
		\param rs the resource set that will be filled
		\param stop the predicate that will stop the function if evaluates to true 
		
		In case of stop, throws a ConfigurationSingleton::Stopped exeption
	*/
	virtual void getOverrideResources(rsResourceSet &rs, bool clearReplacements, const ConfigurationSingleton::StopPredicate &stop = ConfigurationSingleton::StopPredicate()) ;
	
	/*! \brief adds a mandatory configuration space.
	*/
	void addSpace(const std::string &, const std::string & v="" ) ;
	/*! \brief adds a configuration space. 
		The mandatory parameter can be used to declare this conf space as mandatory or not.
	*/
	void addSpace(const std::string &, bool mandatory, const std::string & v="" ) ;
	void addSpace(const ConfigurationSingleton::ConfSpace& ) ;
	void addSpaces(const std::vector< ConfigurationSingleton::ConfSpace >& ) ;
	void addOrReplaceSpace(const ConfigurationSingleton::ConfSpace &) ;
	void clear() ;
	const std::vector< std::pair <std::string, std::string> > &getReplacements() const throw() ;
	
	/*! \brief returns the replacements of the conf spaces found.
	*/
	void getReplacements(std::vector< std::pair <std::string, std::string> > &) const ;
	void addFilepath(const std::string & );
	void addFilepaths(const std::vector< std::string > & );
	

private:
	// declared but not implemented to prevent their use
	ConfigMerger(const ConfigMerger &) ;
	ConfigMerger &operator=(const ConfigMerger &) ;

	void mergeResourceSpace(bool std_conf, ResourceSpace *, const std::string &spaceName, bool mandatory, const ConfigurationSingleton::StopPredicate &, math::StatNode &, rsResourceSet &);
	void mergeResourceSet(const rsResourceSet &in, rsResourceSet &out, const ConfigurationSingleton::StopPredicate &);
	
private:
	std::vector< ConfigurationSingleton::ConfSpace > _spaces ;
	std::vector< std::pair <std::string, std::string> > _replacements ;
	std::vector<std::string> _filepaths;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ConfigMerger) ;
	
} ; 

_ACS_END_NAMESPACE


#endif // _ConfigMerger_H_
