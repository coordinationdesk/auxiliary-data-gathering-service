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
	Revision 5.2  2013/06/20 12:32:48  marpas
	qa rules
	coding best practices applied
	instrumented to avoid qac++ analisys crash
	
	Revision 5.1  2013/06/14 16:27:22  nicvac
	ADC-182: Temp transder strategy implemented.
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.6  2013/03/20 17:13:23  nicvac
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

#ifndef _DistributionSerializable_H_
#define _DistributionSerializable_H_

#include <acs_c++config.hpp>
#include <exException.h>

#ifndef PRQA_ANALYZER
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/utility.hpp>
#endif // PRQA_ANALYZER
#include <boost/shared_ptr.hpp> // PRQA S 1013 


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Serializable Distribution Info
**/

class DistributionSerializable { // PRQA S 2109

#ifndef PRQA_ANALYZER
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar &
			BOOST_SERIALIZATION_NVP( _protocol         ) &
			BOOST_SERIALIZATION_NVP( _username         ) &
			BOOST_SERIALIZATION_NVP( _password         ) &
			BOOST_SERIALIZATION_NVP( _hostname         ) &
			BOOST_SERIALIZATION_NVP( _remotepath       ) &
			BOOST_SERIALIZATION_NVP( _tempStrategyName ) &
			BOOST_SERIALIZATION_NVP( _tempStrategyParam) ;
	}
#endif // PRQA_ANALYZER

public:

	/** DistributionSerializable Exceptions */
	exDECLARE_EXCEPTION(exDistributionSerializableException, exException) ; // Base DistributionSerializable Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exDistributionSerializableCriticalException, exDistributionSerializableException) ; // Critical Exception. // PRQA S 2153

	/** Default Class constructor */
	DistributionSerializable();
	/** Destructor */
	virtual ~DistributionSerializable();

	/** Dump the Object */
	void dump(std::string const& filename) const;
	void dump(std::ostream& oss) const;

	/** Load the Object */
	void load(std::string const& filename);
	void load(std::istream& iss);


	/** Setter */
	void setProtocol         (std::string const& protocol         );
	void setUsername         (std::string const& username         );
	void setPassword         (std::string const& password         );
	void setHostname         (std::string const& hostname         );
	void setRemotepath       (std::string const& remotepath       );
	void setTempStrategyName (std::string const& tempStrategyName );
	void setTempStrategyParam(std::string const& tempStrategyParam);

	/** Getter */
	std::string const& getProtocol         () const throw() ;
	std::string const& getUsername         () const throw() ;
	std::string const& getPassword         () const throw() ;
	std::string const& getHostname         () const throw() ;
	std::string const& getRemotepath       () const throw() ;
	std::string const& getTempStrategyName () const throw() ;
	std::string const& getTempStrategyParam() const throw() ;

	/** Copy Constructor */
	DistributionSerializable(const DistributionSerializable & );
	/** Operator = */
	DistributionSerializable & operator=(const DistributionSerializable &);

private:
	std::string _protocol          ;
    std::string _username          ;
	std::string _password          ;
	std::string _hostname          ;
	std::string _remotepath        ;
	std::string _tempStrategyName  ;
	std::string _tempStrategyParam ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributionSerializable)

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<DistributionSerializable> DistributionSptr;
typedef boost::shared_ptr<const DistributionSerializable> DistributionConstSptr;

_ACS_END_NAMESPACE 

#endif //_DistributionSerializable_H_

