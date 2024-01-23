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

	$Prod: A.C.S. libExporterBase$

	$Id$

	$Author$

	$Log$
	Revision 5.4  2016/05/11 08:58:42  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/03/18 17:34:14  marpas
	adapted to c2evo dev
	
	Revision 5.2  2013/06/20 12:32:48  marpas
	qa rules
	coding best practices applied
	instrumented to avoid qac++ analisys crash
	
	Revision 5.1  2013/06/14 16:27:21  nicvac
	ADC-182: Temp transder strategy implemented.
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.6  2013/03/20 17:13:22  nicvac
	load dump from stream
	
	Revision 1.5  2013/02/07 16:02:53  nicvac
	load and dump for serializable objects.
	
	Revision 1.4  2012/12/12 16:14:52  nicvac
	Refactored - not linked to Consumer Distribution class.
	
	Revision 1.3  2012/12/06 18:34:35  nicvac
	Removed dependency from libDataConsumer.
	
	Revision 1.2  2012/11/21 11:30:30  nicvac
	Serialization
	
	Revision 1.1  2012/11/20 18:05:51  nicvac
	DataPackage and Distribution serialization.
	
        
*/ 

#include <boost/archive/xml_oarchive.hpp> // PRQA S 1013 2
#include <boost/archive/xml_iarchive.hpp>

#include <DistributionSerializable.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DistributionSerializable)


DistributionSerializable::DistributionSerializable():
	_protocol         (""),
	_username         (""),
	_password         (""),
	_hostname         (""),
	_remotepath       (""),
	_tempStrategyName (""),
	_tempStrategyParam("")
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

DistributionSerializable::~DistributionSerializable() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called."); // PRQA S 4631
}

DistributionSerializable::DistributionSerializable(DistributionSerializable const& rhl ):
	_protocol          (rhl._protocol         ),
	_username          (rhl._username         ),
	_password          (rhl._password         ),
	_hostname          (rhl._hostname         ),
	_remotepath        (rhl._remotepath       ),
	_tempStrategyName  (rhl._tempStrategyName ),
	_tempStrategyParam (rhl._tempStrategyParam)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

DistributionSerializable & DistributionSerializable::operator=(const DistributionSerializable & rhl) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( this != &rhl ) {
		_protocol          = rhl._protocol         ;
		_username          = rhl._username         ;
		_password          = rhl._password         ;
		_hostname          = rhl._hostname         ;
		_remotepath        = rhl._remotepath       ;
		_tempStrategyName  = rhl._tempStrategyName ;
		_tempStrategyParam = rhl._tempStrategyParam;
	}
	return *this;
}


// PRQA S 4121 L1
void DistributionSerializable::setProtocol         (std::string const& protocol         ) { _protocol          = protocol         ; }
void DistributionSerializable::setUsername         (std::string const& username         ) { _username          = username         ; }
void DistributionSerializable::setPassword         (std::string const& password         ) { _password          = password         ; }
void DistributionSerializable::setHostname         (std::string const& hostname         ) { _hostname          = hostname         ; }
void DistributionSerializable::setRemotepath       (std::string const& remotepath       ) { _remotepath        = remotepath       ; }
void DistributionSerializable::setTempStrategyName (std::string const& tempStrategyName ) { _tempStrategyName  = tempStrategyName ; }
void DistributionSerializable::setTempStrategyParam(std::string const& tempStrategyParam) { _tempStrategyParam = tempStrategyParam; }
// PRQA L:L1

// PRQA S 4120 L2
std::string const& DistributionSerializable::getProtocol         () const throw() { return _protocol         ; }
std::string const& DistributionSerializable::getUsername         () const throw() { return _username         ; }
std::string const& DistributionSerializable::getPassword         () const throw() { return _password         ; }
std::string const& DistributionSerializable::getHostname         () const throw() { return _hostname         ; }
std::string const& DistributionSerializable::getRemotepath       () const throw() { return _remotepath       ; }
std::string const& DistributionSerializable::getTempStrategyName () const throw() { return _tempStrategyName ; }
std::string const& DistributionSerializable::getTempStrategyParam() const throw() { return _tempStrategyParam; }
// PRQA L:L2
void DistributionSerializable::dump(string const& filename) const {

	std::ofstream ofs(filename.c_str());

	ACS_COND_THROW( ! ofs.good(), exDistributionSerializableCriticalException(string("Cannot dump Distribution info on file ")+filename) ); // PRQA S 3081

	this->dump( ofs );
}

void DistributionSerializable::dump(ostream& oss) const {
	//Dump the datapackage
	boost::archive::xml_oarchive oa(oss);

	const DistributionSerializable& distribution = *this;

	// dump into the archive
	oa << BOOST_SERIALIZATION_NVP( distribution );
}

void DistributionSerializable::load(string const& filename) {

	std::ifstream ifs( filename.c_str() );
	ACS_COND_THROW( ! ifs.good(), exDistributionSerializableCriticalException(string("Cannot load Distribution info from file ")+filename) ); // PRQA S 3081

	this->load( ifs );
}

void DistributionSerializable::load(istream& iss) {

	//Load the datapackage
	boost::archive::xml_iarchive ia(iss);

	DistributionSerializable& distribution = *this;

	// restore from the archive
	ia >> BOOST_SERIALIZATION_NVP( distribution );
}

_ACS_END_NAMESPACE
