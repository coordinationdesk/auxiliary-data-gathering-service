// PRQA S 1050 EOF
/*
	Copyright 2013-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.19  2017/07/28 09:23:18  lucio.pulvirenti
	ECICMF-108: ECShape2InstIdFilesPreInventoryFactory added.
	
	Revision 1.18  2017/07/14 14:45:58  lucio.pulvirenti
	ECICMF-108: ECDlinkFrameFilesPreInventoryFactory added. Old debug macros replaced.
	
	Revision 1.17  2016/05/17 15:10:24  massimo.ricci
	(1) Snavons preinventory added for ORC of SpaceNav project
	
	Revision 1.16  2016/04/19 09:16:29  francesco.avanzi
	factory corrected
	
	Revision 1.15  2016/04/18 19:26:46  francesco.avanzi
	S3ExelisFilenamePreInventory added
	
	Revision 1.14  2016/04/15 08:43:18  lucio.pulvirenti
	S3MPM-695: USItoS3MPMPreInventoryFactory class added.
	
	Revision 1.13  2016/04/04 15:29:28  francesco.avanzi
	introducing new preInventory S3ExtFilenamePreInventory.C
	
	Revision 1.12  2016/01/12 14:01:23  francesco.avanzi
	module modified to enhance FilenamePreInventory
	
	Revision 1.11  2015/08/04 16:02:54  francesco.avanzi
	file modified, still needs to be tested
	
	Revision 1.10  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: AlsatFilenamePreInventory class developed.
	
	Revision 1.9  2015/06/18 15:46:27  lucio.pulvirenti
	S3MPM-528: S3MPMReportsStdLikePreInventoryFactory added.
	
	Revision 1.8  2015/06/09 15:50:11  lucio.pulvirenti
	S3MPM-504: S3PMRequestProductPreInventoryFactory class added.
	
	Revision 1.7  2015/06/05 13:55:23  lucio.pulvirenti
	S3MPM-504: EOFlikePreInventoryFactory added.
	
	Revision 1.6  2014/09/30 15:47:59  lucio.pulvirenti
	S3SCCDBAuxPreInventoryFactory class added.
	
	Revision 1.5  2014/09/29 14:37:43  lucio.pulvirenti
	S3OpticalProductsPreInventoryFactory added.
	
	Revision 1.4  2014/03/12 11:16:52  lucio.pulvirenti
	JersSAROPSPreInventoryFactory and ESAStandardPreInventoryFactory added as per jira issue PDGSEVO-35.
	
	Revision 1.3  2014/03/11 17:02:14  lucio.pulvirenti
	Work in progress.
	
	Revision 1.2  2014/03/10 16:49:07  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
	
	Revision 1.1.1.1  2013/09/12 15:44:18  lucio.pulvirenti
	Import libFilenamePreInventory
	
	
        
*/

#ifndef _FilenamePreInventoryFactory_H_
#define _FilenamePreInventoryFactory_H_

#include <acs_c++config.hpp>
#include <PreInventoryFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 **/

class S3PDGSPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3PDGSPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3PDGSPreInventoryFactoryException, exException) ; // Base S3PDGSPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3PDGSPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3PDGSPreInventoryFactory();
	/** Destructor */
	virtual ~S3PDGSPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3PDGSPreInventoryFactory(const S3PDGSPreInventoryFactory & ); // not implemented
	S3PDGSPreInventoryFactory &operator=(const S3PDGSPreInventoryFactory &); // not implemented

private:
    static const std::string _S3PDGSPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3PDGSPreInventoryFactory)
};
	
class GHRSSTL4PreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** GHRSSTL4PreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exGHRSSTL4PreInventoryFactoryException, exException) ; // Base GHRSSTL4PreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exGHRSSTL4PreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	GHRSSTL4PreInventoryFactory();
	/** Destructor */
	virtual ~GHRSSTL4PreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	GHRSSTL4PreInventoryFactory(const GHRSSTL4PreInventoryFactory & ); // not implemented
	GHRSSTL4PreInventoryFactory &operator=(const GHRSSTL4PreInventoryFactory &); // not implemented

private:
    static const std::string _GHRSSTL4PreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(GHRSSTL4PreInventoryFactory)
};

class JersSAROPSPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** JersSAROPSPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exJersSAROPSPreInventoryFactoryException, exException) ; // Base JersSAROPSPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exJersSAROPSPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	JersSAROPSPreInventoryFactory();
	/** Destructor */
	virtual ~JersSAROPSPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	JersSAROPSPreInventoryFactory(const JersSAROPSPreInventoryFactory & ); // not implemented
	JersSAROPSPreInventoryFactory &operator=(const JersSAROPSPreInventoryFactory &); // not implemented

private:
    static const std::string _JersSAROPSPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(JersSAROPSPreInventoryFactory)
};

class ESAStandardPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** ESAStandardPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exESAStandardPreInventoryFactoryException, exException) ; // Base ESAStandardPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exESAStandardPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	ESAStandardPreInventoryFactory();
	/** Destructor */
	virtual ~ESAStandardPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	ESAStandardPreInventoryFactory(const ESAStandardPreInventoryFactory & ); // not implemented
	ESAStandardPreInventoryFactory &operator=(const ESAStandardPreInventoryFactory &); // not implemented

private:
    static const std::string _ESAStandardPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ESAStandardPreInventoryFactory)
};


class S3OpticalProductsPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3OpticalProductsPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3OpticalProductsPreInventoryFactoryException, exException) ; // Base S3OpticalProductsPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3OpticalProductsPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3OpticalProductsPreInventoryFactory();
	/** Destructor */
	virtual ~S3OpticalProductsPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3OpticalProductsPreInventoryFactory(const S3OpticalProductsPreInventoryFactory & ); // not implemented
	S3OpticalProductsPreInventoryFactory &operator=(const S3OpticalProductsPreInventoryFactory &); // not implemented

private:
    static const std::string _S3OpticalProductsPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3OpticalProductsPreInventoryFactory)
};

	
class S3SCCDBAuxPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3SCCDBAuxPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3SCCDBAuxPreInventoryFactoryException, exException) ; // Base S3SCCDBAuxPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3SCCDBAuxPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3SCCDBAuxPreInventoryFactory();
	/** Destructor */
	virtual ~S3SCCDBAuxPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3SCCDBAuxPreInventoryFactory(const S3SCCDBAuxPreInventoryFactory & ); // not implemented
	S3SCCDBAuxPreInventoryFactory &operator=(const S3SCCDBAuxPreInventoryFactory &); // not implemented

private:
    static const std::string _S3SCCDBAuxPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3SCCDBAuxPreInventoryFactory)
};
	
	
class EOFlikePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** EOFlikePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exEOFlikePreInventoryFactoryException, exException) ; // Base EOFlikePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exEOFlikePreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	EOFlikePreInventoryFactory();
	/** Destructor */
	virtual ~EOFlikePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	EOFlikePreInventoryFactory(const EOFlikePreInventoryFactory & ); // not implemented
	EOFlikePreInventoryFactory &operator=(const EOFlikePreInventoryFactory &); // not implemented

private:
    static const std::string _EOFlikePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFlikePreInventoryFactory)
};

class S3PMRequestProductPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3PMRequestProductPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3PMRequestProductPreInventoryFactoryException, exException) ; // Base S3PMRequestProductPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3PMRequestProductPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3PMRequestProductPreInventoryFactory();
	/** Destructor */
	virtual ~S3PMRequestProductPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3PMRequestProductPreInventoryFactory(const S3PMRequestProductPreInventoryFactory & ); // not implemented
	S3PMRequestProductPreInventoryFactory &operator=(const S3PMRequestProductPreInventoryFactory &); // not implemented

private:
    static const std::string _S3PMRequestProductPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3PMRequestProductPreInventoryFactory)
};

	
class S3MPMReportsStdLikePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3MPMReportsStdLikePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3MPMReportsStdLikePreInventoryFactoryException, exException) ; // Base S3MPMReportsStdLikePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3MPMReportsStdLikePreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3MPMReportsStdLikePreInventoryFactory();
	/** Destructor */
	virtual ~S3MPMReportsStdLikePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3MPMReportsStdLikePreInventoryFactory(const S3MPMReportsStdLikePreInventoryFactory & ); // not implemented
	S3MPMReportsStdLikePreInventoryFactory &operator=(const S3MPMReportsStdLikePreInventoryFactory &); // not implemented

private:
    static const std::string _S3MPMReportsStdLikePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3MPMReportsStdLikePreInventoryFactory)
};
	
class AlsatFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** AlsatFilenamePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exAlsatFilenamePreInventoryFactoryException, exException) ; // Base AlsatFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exAlsatFilenamePreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	AlsatFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~AlsatFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	AlsatFilenamePreInventoryFactory(const AlsatFilenamePreInventoryFactory & ); // not implemented
	AlsatFilenamePreInventoryFactory &operator=(const AlsatFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _AlsatFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AlsatFilenamePreInventoryFactory)
};

	
class SpaceNavPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** SpaceNavPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exSpaceNavPreInventoryFactoryException, exException) ; // Base SpaceNavPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exSpaceNavPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	SpaceNavPreInventoryFactory();
	/** Destructor */
	virtual ~SpaceNavPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	SpaceNavPreInventoryFactory(const SpaceNavPreInventoryFactory & ); // not implemented
	SpaceNavPreInventoryFactory &operator=(const SpaceNavPreInventoryFactory &); // not implemented

private:
    static const std::string _SpaceNavPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SpaceNavPreInventoryFactory)
};

class SnavonsPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** SnavonsPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exSnavonsPreInventoryFactoryException, exException) ; // Base SnavonsPreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exSnavonsPreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	SnavonsPreInventoryFactory();
	/** Destructor */
	virtual ~SnavonsPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	SnavonsPreInventoryFactory(const SnavonsPreInventoryFactory & ); // not implemented
	SnavonsPreInventoryFactory &operator=(const SnavonsPreInventoryFactory &); // not implemented

private:
    static const std::string _SnavonsPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SnavonsPreInventoryFactory)
};

class S3ExtFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3ExtFilenamePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3ExtFilenamePreInventoryFactoryException, exException) ; // Base S3ExtFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3ExtFilenamePreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3ExtFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~S3ExtFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3ExtFilenamePreInventoryFactory(const S3ExtFilenamePreInventoryFactory & ); // not implemented
	S3ExtFilenamePreInventoryFactory &operator=(const S3ExtFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _S3ExtFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3ExtFilenamePreInventoryFactory)
};
	
class S3ExelisFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3ExelisFilenamePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3ExelisFilenamePreInventoryFactoryException, exException) ; // Base S3ExelisFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3ExelisFilenamePreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3ExelisFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~S3ExelisFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3ExelisFilenamePreInventoryFactory(const S3ExelisFilenamePreInventoryFactory & ); // not implemented
	S3ExelisFilenamePreInventoryFactory &operator=(const S3ExelisFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _S3ExelisFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3ExelisFilenamePreInventoryFactory)
};

class S3DimsaxFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3DimsaxFilenamePreInventoryFactory */
	exDECLARE_EXCEPTION(exS3DimsaxFilenamePreInventoryFactoryException, exException) ; // Base S3DimsaxFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exS3DimsaxFilenamePreInventoryFactoryCriticalException, exCriticalException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	S3DimsaxFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~S3DimsaxFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3DimsaxFilenamePreInventoryFactory(const S3DimsaxFilenamePreInventoryFactory & ); // not implemented
	S3DimsaxFilenamePreInventoryFactory &operator=(const S3DimsaxFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _S3DimsaxFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3DimsaxFilenamePreInventoryFactory)
};


class USItoS3MPMPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** USItoS3MPMPreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exUSItoS3MPMPreInventoryFactoryException, exException) ; // Base USItoS3MPMPreInventoryFactory Exception. // PRQA S 2131, 2502

	/** Default Class constructor */
	USItoS3MPMPreInventoryFactory();
	/** Destructor */
	virtual ~USItoS3MPMPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	USItoS3MPMPreInventoryFactory(const USItoS3MPMPreInventoryFactory & ); // not implemented
	USItoS3MPMPreInventoryFactory &operator=(const USItoS3MPMPreInventoryFactory &); // not implemented

private:
    static const std::string _USItoS3MPMPreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(USItoS3MPMPreInventoryFactory)
};
	

class S3GPDFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S3GPDFilenamePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS3GPDFilenamePreInventoryFactoryException, exException) ; // Base S3GPDFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502

	/** Default Class constructor */
	S3GPDFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~S3GPDFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S3GPDFilenamePreInventoryFactory(const S3GPDFilenamePreInventoryFactory & ); // not implemented
	S3GPDFilenamePreInventoryFactory &operator=(const S3GPDFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _S3GPDFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3GPDFilenamePreInventoryFactory)
};
	
class S6MOG2DFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** S6MOG2DFilenamePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exS6MOG2DFilenamePreInventoryFactoryException, exException) ; // Base S6MOG2DFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502

	/** Default Class constructor */
	S6MOG2DFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~S6MOG2DFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	S6MOG2DFilenamePreInventoryFactory(const S6MOG2DFilenamePreInventoryFactory & ); // not implemented
	S6MOG2DFilenamePreInventoryFactory &operator=(const S6MOG2DFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _S6MOG2DFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S6MOG2DFilenamePreInventoryFactory)
};
	
_ACS_END_NAMESPACE

#endif //_FilenamePreInventoryFactory_H_

