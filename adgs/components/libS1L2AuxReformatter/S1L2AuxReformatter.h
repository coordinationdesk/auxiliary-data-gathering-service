// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.10  2013/10/30 15:01:54  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.9  2013/10/29 16:00:23  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.8  2013/10/24 14:05:27  nicvac
	S1PDGS-1899: file naming conversion.
	
	Revision 1.7  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.6  2012/04/23 08:06:36  damdec
	Design reviewed.
	
	Revision 1.5  2012/04/19 14:34:46  damdec
	Handling of multiple input files added for AUX_ICE filetype (Northern and Southern hemisphere). Facility info added to manifest.
	
	Revision 1.4  2012/04/17 16:03:50  damdec
	Class updated according to libACSSafeFormatter interface changes.
	
	Revision 1.3  2012/04/05 09:00:21  damdec
	Implementation finalized
	
	Revision 1.2  2012/03/29 13:59:59  damdec
	Work in progress
	
	Revision 1.1.1.1  2012/03/27 11:50:11  damdec
	Import libS1L2AuxReformatter
	
        
*/

#ifndef _S1L2AuxReformatter_H_
#define _S1L2AuxReformatter_H_

#include <exException.h>
#include <vector>
#include <SafeManifest.h>
#include <Processing.h>
#include <Platform.h>
#include <S1AuxGeneralInformation.h>

#include <UtilXml.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Class performing the Sentinel-1 L2 Auxiliary Product Reformatting in SAFE format 
**/

class S1L2AuxReformatter  { 
public:

	/** S1L2AuxReformatter Exceptions */
	exDECLARE_EXCEPTION(exS1L2AuxReformatterException, exException) ; // Base S1L2AuxReformatter Exception.
	exDECLARE_EXCEPTION(exS1L2AuxReformatterCriticalException, exS1L2AuxReformatterException) ; // Critical Exception.


	/** Class constructor */
	S1L2AuxReformatter();
	/** Destructor */
	virtual ~S1L2AuxReformatter() throw() ;
	
	virtual std::string createSafePackage(const std::string & processorName, const std::string& procVersion, const std::string & confFilename);
	void init(const std::vector<std::string> & infilename, const std::string& ioNamingRuleFilename, const std::string & outProdType, const std::string & safeVer, const std::string & outputPath="./");
	const std::vector <std::string> & getInFilename() const throw() { return _infilename; }
	const std::vector <std::string> & getDataFilename() const throw() { return _dataFilename; }
	const std::string &getOutProductType() const throw() { return _outProductType; }
	const std::string &getSafeVersion() const throw() { return _safeVersion; }
	const std::string &getValidity() const throw() { return _validity; }
	const std::string &getGeneration() const throw() { return _generation; }
	const std::string &getOutputPath() const throw() { return _outputPath; }
	const std::string &getStartTime() const throw() { return _startTime; }
	const std::string &getStopTime() const throw() { return _stopTime; }
	
private:  // copy constructor and operator= defined but not implemented
	S1L2AuxReformatter(const S1L2AuxReformatter & ); // not implemented
	S1L2AuxReformatter &operator=(const S1L2AuxReformatter &); // not implemented

	void applyNamingRules( std::vector< XmlDocSptr > const& actions, std::string& filename );
	
	void setProcessing(const std::string & processorName, const std::string& procVersion, const std::string & safeNamespace, const std::string& safePrefix);
	void setGeneralInfo(const std::string & safeNamespace);

protected:
#ifdef ENABLE_TEST
public:
	void generation(const std::string & g) {_generation = g;};
#endif
	void validity(const std::string &v) { _validity = v ; }
    
	virtual void handleDataFiles(SafeManifest & manifest, const std::string & dataDir);
	virtual void setValidity(const std::string &) {} ;
	virtual void setSoftware(ProcessLog & pLog, Software & sw) {};
	virtual void setPlatform(const std::string & safeNamespace, const std::string& safePrefix);
	virtual std::string buildOutputFilename();

private:
	std::vector <std::string> _infilename;
	std::vector <std::string> _dataFilename;
	std::string _outProductType;
	std::string _safeVersion;
	std::string _validity;
	std::string _generation;
	std::string _outputPath;
	std::string _startTime;
	std::string _stopTime;
	
	rsResourceSet& _rsConf;
	
	Platform * _platform;	// PlatformType metadata
	Processing * _processing;	// ProcessingType metadata
	S1AuxGeneralInformation * _generalInfo;	// GeneralProductInformationType metadata (specific for S1 L2 AUX files)
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1L2AuxReformatter)

};

_ACS_END_NAMESPACE

#endif //_S1L2AuxReformatter_H_

