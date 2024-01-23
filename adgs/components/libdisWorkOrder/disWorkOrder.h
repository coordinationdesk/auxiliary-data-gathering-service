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

	$Prod: A.C.S. DIWorkOrder Library $

	$Id$

	$Author$

*/

#ifndef _disWorkOrder_H_
#define _disWorkOrder_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <rsResourceSet.h>
#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)

//! the class disWorkOrder is in charge to manage  
//! the DIWorkOrder file when it is merged in the configuration 
//! space.
class disWorkOrder // PRQA S 2109
{


public:
    //! the class device is used as a structure to 
    //! return to the user the list of output device 
    //! as we can see in the getOutputDevices() method
    class device
    {
    public:
        device();
        device(const device&);
        device &operator=(const device &) ; // not implemented
        ~device();
        int _Channel; // PRQA S 2100 5
        std::string _PathName;
        std::string _Type;
        std::string _FileName;
		std::string _DataType;
        void show() const ;
    };

public:
    explicit disWorkOrder(bool UseConfSet= true);  
    virtual ~disWorkOrder() throw();
    const std::string &getSatellite() const throw() { return _satellite; }
	
	/**
	  * \brief Return the Satellite name in ACS syntax(First Upper - Rest Lower)
	  */
	std::string getSatelliteAcsName() const;
    
	unsigned int getMission() const throw() { return _mission; }

    unsigned int getDataRate() const;
    const std::string &getSensor() const throw() { return _sensor ; }
    std::string getAcqStation() const;
    std::string getIngestionMode() const;
    std::string getProcessingCentreID() const;
    std::string getMessageQueuePath() const;
    std::string getProcessingStage() const;
    unsigned int getRFPathID() const;
    std::string getRFPathName() const;
    std::string getProcessId() const;
    std::string getStartTime() const;
    std::string getStopTime() const;
    std::string getSlotKey() const;
    unsigned int getOrbit() const;
    unsigned int getDuration() const;
    std::vector<device>  getOutputDevices() const;
    std::vector<device>  getInputDevices() const;
    void show() const;
    void generateXML(std::ostream &);
    void setSatellite(std::string);
    void setMission(unsigned int);
    void setSensor(std::string);
    void setAcqStation(std::string);
    void setIngestionMode(std::string);
    void setProcessingCentreID(std::string);
    void setMessageQueuePath(std::string);
    void setProcessingStage(std::string);
    void setRFPathName(std::string);
    void setRFPathID(unsigned int);
    void setProcessId(std::string);
    void setStartTime(std::string);
    void setStopTime(std::string);
    void setOrbit(unsigned int);
    void setDataRate(unsigned int);
    void setDuration(unsigned int);
    void setOutputDevices(std::vector<device>&);
    void setInputDevices(std::vector<device>&);
    void setOutputDevice(device&);
    void setInputDevice(device&);
    void setSlotKey(std::string);
private:
    disWorkOrder(const disWorkOrder &) ; // not implemented
    disWorkOrder& operator= (const disWorkOrder &) ; // not implemented
private:
    
    rsResourceSet _confSet ;
    std::string  _satellite;
    unsigned int _mission;
    std::string  _sensor;
    unsigned int _datarate; // Megabit/s
    std::string  _acqstation;
    std::string  _processingcentreid;
    std::string  _messagequeuepath;
    std::string  _processingstage;
    std::string  _rfpathname;
    unsigned int _rfpathid;
    std::string  _processid;
    std::string  _starttime;
    std::string  _stoptime;
    unsigned int _orbit;
    unsigned int _duration;
    std::vector<device> _odevices;
    std::vector<device> _idevices;
	std::string  		_ingestionMode;
	std::string  		_slotKey;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(disWorkOrder) ;
};

_ACS_END_NAMESPACE

#endif // _disWorkOrder_H_
