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

	$Prod: A.C.S. Plugin Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.2  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.1  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/06/10 12:30:30  integrator
	interface slightly changed
	
	Revision 1.5  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.4  2003/07/11 12:59:19  paoscu
	exException::clone implemented
	
	Revision 1.3  2003/04/30 16:41:31  paoscu
	using namespace std no longer into acs includes
	
	Revision 1.2  2002/11/25 17:43:53  paoscu
	Now load method takes a PluginLoader reference.
	
	Revision 1.1.1.1  2002/10/29 18:24:47  paoscu
	Import libPlugin
	
	

*/

#ifndef _PluginSet_H_
#define _PluginSet_H_ 

#include <acs_c++config.hpp>
#include <PluginLoader.h>
#include <rsResourceSet.h>



_ACS_BEGIN_NAMESPACE(acs)

/*!
 * \class PluginSet
 * This class is able to load a PluginLoader object on the base of informations specified in a rsResourceSet object.	
 */
class PluginSet // PRQA S 2109
{
public:
	/*! class BadResourceFile declaration is thrown each time a shared object fails loading.
	 */
	exDECLARE_EXCEPTION(BadResourceFile,exException) ; // PRQA S 2131, 2502
		
public:
	PluginSet() ;
	virtual ~PluginSet() ACS_NOEXCEPT ;
	
	/**
	 * Fill a PluginLoader object.
	 * returns the number of plugins correctly loaded
	 * \exception BadResourceFile The rsResourceSet object contains an error, or a mandatory plugin could not be loaded. 
	 */	
	size_t load(rsResourceSet &, PluginLoader &loader);
	
private:
	PluginSet(const PluginSet & ) ;
	PluginSet &operator=(const PluginSet & ) ;	

	void getPluginParameters(rsResourceSet &, PluginLoader &, size_t ) ;

private:	

	ACS_CLASS_DECLARE_DEBUG_LEVEL(PluginSet) ;
} ;


_ACS_END_NAMESPACE

#endif // _PluginSet_H_
