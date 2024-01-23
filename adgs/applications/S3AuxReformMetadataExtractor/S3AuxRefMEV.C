/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel3 Auxiliary Reformatter application $

	$Id$

	$Author$
	

*/

#include <auto_version_file>
#include <S3AuxRefMEV.h>

_ACS_BEGIN_NAMESPACE(acs)

const char* S3AuxRefMEV::_name = auto_version_file::name;
const char* S3AuxRefMEV::_tag = auto_version_file::tag;

S3AuxRefMEV::S3AuxRefMEV() throw() {}
S3AuxRefMEV::~S3AuxRefMEV() throw() {}

const char* S3AuxRefMEV::tag() throw() { return _tag ; }
const char* S3AuxRefMEV::name() throw() { return _name ; }

_ACS_END_NAMESPACE

