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

	$Prod: $

	$Id$

	$Author$

    $Log$
*/

#ifndef _ProdTypeDescriptorFactory_H_
#define _ProdTypeDescriptorFactory_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class ProductTypeDescriptor;

class ProdTypeDescriptorFactory
{
public:

	ProdTypeDescriptorFactory();
	~ProdTypeDescriptorFactory() throw() ;
	static ProductTypeDescriptor* buildCryosatL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildEnvisatL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildGoceL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildCosmoL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildEosL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildLandsatL0ProductTypeDescriptor();
    static ProductTypeDescriptor* buildJersL0ProductTypeDescriptor();
    static ProductTypeDescriptor* buildMosL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildErsL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildSpotL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildOceansatL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildAlosL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildRadarsatL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildSacdL0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildSentinel1L0ProductTypeDescriptor();
	static ProductTypeDescriptor* buildL1ProductTypeDescriptor();
	static ProductTypeDescriptor* buildL2ProductTypeDescriptor();
	static ProductTypeDescriptor* buildBWSProductTypeDescriptor();

private:
	ProdTypeDescriptorFactory(const ProdTypeDescriptorFactory &); // not implemented
	ProdTypeDescriptorFactory& operator=(const ProdTypeDescriptorFactory &); // not implemented

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProdTypeDescriptorFactory) ;

};

_ACS_END_NAMESPACE

#endif // _ProdTypeDescriptorFactory_H_

