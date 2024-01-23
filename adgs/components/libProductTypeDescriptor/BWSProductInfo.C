// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
*/

#include <BWSProductInfo.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

BWSProductInfo::BWSProductInfo(const string& key) :
	_id(),
	_sensorName(),
	_sensorMode()
{
	rsResourceSet& rsConf = ConfigurationSingleton::instance()->get();

	string rsKey = key + ".Id";
	rsConf.getValue(rsKey, _id);

	rsKey = key + ".SensorName";
	rsConf.getValue(rsKey, _sensorName);

	rsKey = key + ".SensorMode";
	rsConf.getValue(rsKey, _sensorMode);
}

BWSProductInfo::~BWSProductInfo() throw()
{
}

/*
 * copy constructor
 */
BWSProductInfo::BWSProductInfo(const BWSProductInfo & e):
	_id(e._id),
	_sensorName(e._sensorName),
	_sensorMode(e._sensorMode)
{
}

/*
 * operator =
 */
BWSProductInfo&  BWSProductInfo::operator=(const BWSProductInfo & e)
{
	if (this != &e) {
		_id 						= e._id;
		_sensorName					= e._sensorName;
		_sensorMode					= e._sensorMode;
	}
	return *this ;
}

_ACS_END_NAMESPACE
