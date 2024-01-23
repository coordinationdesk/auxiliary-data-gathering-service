/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
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
#include <sstream>
#include <disWorkOrder.h>
#include <StringUtils.h>

#include <disWorkOrderV.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
	disWorkOrderV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(disWorkOrder)

//!

disWorkOrder::device::device() :
	_Channel(0),
	_PathName(),
	_Type(),
	_FileName(),
	_DataType()
{
}

disWorkOrder::device::device(const disWorkOrder::device &r) :
	_Channel(r._Channel),
	_PathName(r._PathName),
	_Type(r._Type),
	_FileName(r._FileName),
	_DataType(r._DataType)
{}

disWorkOrder::device& disWorkOrder::device::operator=(const disWorkOrder::device &r) 
{
    if (this != &r) {
        _PathName = r._PathName;
        _Type = r._Type;
        _FileName = r._FileName;
        _Channel = r._Channel;
	    _DataType = r._DataType ;
    }
    return *this ;
}


disWorkOrder::device::~device()
{
}


void
disWorkOrder::device::show() const
{
	ostringstream os ;
    os <<  "PathName: " << _PathName << '\n';  
    os <<  "Type: "    << _Type << '\n';  
    os <<  "FileName: " << _FileName << '\n';
    os <<  "Channel: " << _Channel << '\n';  
	os <<  "DataType: " << _DataType << '\n' ;
	ACS_LOG_INFO(os.str())
}

// constructor
disWorkOrder::disWorkOrder(bool UseConfSet) :
	_confSet(ConfigurationSingleton::instance()->get()),
	_satellite(),
	_mission(1),
	_sensor(),
	_datarate(10),
	_acqstation(),
	_processingcentreid(),
	_messagequeuepath(),
	_processingstage(),
	_rfpathname(),	
	_rfpathid(1),
	_processid(),
	_starttime(),
	_stoptime(),
	_orbit(1),
	_duration(10),
	_odevices(),
	_idevices(),
	_ingestionMode(),
	_slotKey()
{
    if(UseConfSet)
    {
        _confSet.getValue("DIOrder.Satellite",_satellite);
        string mission;
        _confSet.getValue("DIOrder.Mission",mission);
        istringstream tmp(mission);
        tmp >> _mission;
        _confSet.getValue("DIOrder.DataRate",_datarate);
        _confSet.getValue("DIOrder.Sensor",_sensor);
        _confSet.getValue("DIOrder.AcqStation",_acqstation);
        _confSet.getValue("DIOrder.ProcessingCentreID",_processingcentreid);
        _confSet.getValue("DIOrder.MessageQueuePath",_messagequeuepath);
        _confSet.getValue("DIOrder.ProcessingStage",_processingstage);
		try {
		   _confSet.getValue("DIOrder.IngestionMode",_ingestionMode);
		}
		catch(exception& e){_ingestionMode = string("RealTime");}
		try {
		   _confSet.getValue("DIOrder.SlotKey",_slotKey);
		}
		catch(exception& e){_slotKey = string("");}
        _confSet.getValue("TelemetryInput.RFPath.ID",_rfpathid);
        _confSet.getValue("TelemetryInput.RFPath.Name",_rfpathname);
		{
			rsPushNSpace ListOfInputNs(_confSet,"TelemetryInput.ListOfInput");
        	size_t nindev = _confSet.getArraySize("Input");
        	for(unsigned int i=0; i < nindev; i++)
        	{
            	disWorkOrder::device dev;
            	dev._Channel = -1;
            	ostringstream key;
            	key << "Input[" << i << "]";
            	rsPushNSpace ns1(_confSet,key.str());
            	_confSet.getValue("Device.PathName",dev._PathName);
            	_confSet.getValue("Device.Type",dev._Type);
            	_confSet.getValue("FileName",dev._FileName);
				try {
				  _confSet.getValue("DataType",dev._DataType);
				}catch(exException& e)
				{
				  dev._DataType = "RAW" ;
				} ;
            	_confSet.getValue("Channel",dev._Channel);
            	if (dev._Channel >= 0) {
                	_idevices.push_back(dev);
				}
        	}
        }
        _confSet.getValue("TelemetryOutput.ProcessId",_processid);
		{
			rsPushNSpace ListOfOutputNs(_confSet,"TelemetryOutput.ListOfOutput");
        	size_t noutdev = _confSet.getArraySize("Output");
        	for(unsigned int i=0; i < noutdev; i++)
        	{
            	disWorkOrder::device dev;
            	dev._Channel = -1;
            	ostringstream key;
            	key << "Output[" << i << "]";
            	rsPushNSpace ns1(_confSet,key.str());
            	_confSet.getValue("Device.PathName",dev._PathName);
            	_confSet.getValue("Device.Type",dev._Type);
            	_confSet.getValue("FileName",dev._FileName);
				try {
				  _confSet.getValue("DataType",dev._DataType);
				}catch(exException& e)
				{
				  dev._DataType = "RAW" ;
				} ;
            	_confSet.getValue("Channel",dev._Channel);
            	if (dev._Channel >= 0) {
                	_odevices.push_back(dev);
				}
        	}
		}
        _confSet.getValue("Downlink.StartTime",_starttime);
        _confSet.getValue("Downlink.StopTime",_stoptime);
		_confSet.getValue("Downlink.Orbit",_orbit);
		unsigned int val;
		_confSet.getValue("Downlink.Duration",val);
        string unit;
        _confSet.getValue("Downlink.Duration.unit.value",unit);
        if(unit == "s")
            _duration =  val;
        else if (unit == "m") {
                _duration = val * 60;
			}
             else
             {
                string msg=" The duration unit must be <\"s\"|\"m\"> instead of ";
                msg += unit;
                exIllegalValueException ex(msg);
                ACS_THROW ( ex );
             }
    }
}

disWorkOrder::~disWorkOrder() throw()
{
}



string
disWorkOrder::getSlotKey() const
{
    return _slotKey;
}

string disWorkOrder::getSatelliteAcsName() const
{
    return StringUtils::capitalize(_satellite) ;
}



string
disWorkOrder::getAcqStation() const
{
    return _acqstation;
}

string
disWorkOrder::getIngestionMode() const
{
    return _ingestionMode;
}

string
disWorkOrder::getProcessingCentreID() const
{
    return _processingcentreid;
}

string
disWorkOrder::getMessageQueuePath() const
{
    return _messagequeuepath;
}
string
disWorkOrder::getProcessingStage() const
{
    return _processingstage;
}

string
disWorkOrder::getRFPathName() const
{
    return _rfpathname;
}


unsigned int
disWorkOrder::getRFPathID() const
{
    return _rfpathid;
}

unsigned int 
disWorkOrder::getDataRate() const
{
    return _datarate;
}

string
disWorkOrder::getProcessId() const
{
    return _processid;
}

string
disWorkOrder::getStartTime() const
{
    return _starttime;
}

string
disWorkOrder::getStopTime() const
{
    return _stoptime;
}

unsigned int
disWorkOrder::getOrbit() const
{
   return _orbit;
}

unsigned int
disWorkOrder::getDuration() const
{
    return _duration;
}

vector<disWorkOrder::device>  
disWorkOrder::getOutputDevices() const
{
    return _odevices;
}
vector<disWorkOrder::device>  
disWorkOrder::getInputDevices() const
{
    return _idevices;
}


void
disWorkOrder::show() const
{
    ostringstream os ;
	os << " Satellite " <<      getSatellite() << endl;
    os << " Mission   " <<      getMission()   << endl;
    os << " Sensor   "  <<      getSensor()    << endl;
    os << " AcqStation "  <<    getAcqStation() << endl;
    os << " IngestionMode "  <<    getIngestionMode() << endl;
    os << " SlotKey "  <<    getSlotKey() << endl;
    os << " ProcessingCentreID   "  <<  getProcessingCentreID() << endl;
    os << " MessageQueuePath   " <<  getMessageQueuePath()   << endl;
    os << " ProcessingStage   "     <<  getProcessingStage()    << endl;
    os << " RFPathID   "  <<  getRFPathID() << endl;
    os << " ProcessId  "  <<  getProcessId()<< endl;
    os << " StartTime  "  <<  getStartTime()<< endl;
    os << " StopTime   "  <<  getStopTime() << endl;
    os << " Orbit      "  <<  getOrbit()    << endl;
    os << " Duration   "  <<  getDuration() << endl;
	ACS_LOG_INFO(os.str())
    vector<disWorkOrder::device> outputs = getOutputDevices();
    for(vector<disWorkOrder::device>::iterator i = outputs.begin(); i < outputs.end(); i++)
        i->show();
}


void
disWorkOrder::setSatellite(string satellite)
{
    _satellite = satellite;
}

void
disWorkOrder::setSlotKey(string skey)
{
    _slotKey = skey;
}

void
disWorkOrder::setMission(unsigned int mission)
{
    _mission = mission;
}
void
disWorkOrder::setSensor(string sensor)
{
    _sensor = sensor;
}
void
disWorkOrder::setIngestionMode(string imode)
{
    _ingestionMode = imode;
}
void
disWorkOrder::setAcqStation(string acqstation)
{
    _acqstation = acqstation;
}
void
disWorkOrder::setProcessingCentreID(string processingcentreid)
{
    _processingcentreid=processingcentreid;
}

void
disWorkOrder::setMessageQueuePath(string messagequeuepath)
{
    _messagequeuepath = messagequeuepath;
}

void
disWorkOrder::setProcessingStage(string processingstage)
{
    _processingstage = processingstage;
}

void
disWorkOrder::setRFPathName(string rfpathname)
{
    _rfpathname = rfpathname;
}

void
disWorkOrder::setRFPathID(unsigned int id)
{
    _rfpathid = id;
}

void
disWorkOrder::setProcessId(string processid)
{
    _processid = processid;
}

void
disWorkOrder::setStartTime(string starttime)
{
    _starttime = starttime;
}

void
disWorkOrder::setStopTime(string stoptime)
{
    _stoptime = stoptime;
}

void
disWorkOrder::setOrbit(unsigned int orbit)
{
    _orbit = orbit;
}
void 
disWorkOrder::setDataRate(unsigned int datarate)
{
    _datarate = datarate;
}


void
disWorkOrder::setDuration(unsigned int duration)
{
    _duration = duration;
}

void  
disWorkOrder::setOutputDevices(vector<device>& odevices)
{
    _odevices = odevices;
}

void  
disWorkOrder::setInputDevices(vector<device>& idevices)
{
    _idevices = idevices;
}

void 
disWorkOrder::setOutputDevice(device& odev)
{
    _odevices.push_back(odev);
}
void 
disWorkOrder::setInputDevice(device& idev)
{
    _idevices.push_back(idev);

}



void disWorkOrder::generateXML(ostream & os) 
{
    XMLOstream xst(os,"DIWorkOrderFile");
        XMLOstream::Tag DIOrder(xst , "DIOrder");
            XMLOstream::Tag Satellite(xst , "Satellite",_satellite,"",true);
            {
                ostringstream tmp;
                tmp << _mission;
                XMLOstream::Tag Mission(xst , "Mission",tmp.str(),"",true);
            }
            {
                ostringstream tmp;
                tmp << _datarate;
                XMLOstream::Tag DataRate(xst , "DataRate",tmp.str(),"",true);
            }
            XMLOstream::Tag Sensor(xst , "Sensor",_sensor,"",true);
            XMLOstream::Tag AcqStation(xst , "AcqStation",_acqstation,"",true);
            XMLOstream::Tag ProcessingCentreID(xst , "ProcessingCentreID",_processingcentreid,"",true);
            XMLOstream::Tag MessageQueuePath(xst , "MessageQueuePath",_messagequeuepath,"",true);
            XMLOstream::Tag ProcessingStage(xst , "ProcessingStage",_processingstage,"",true);
            XMLOstream::Tag IMode(xst , "IngestionMode",_ingestionMode,"",true);
            XMLOstream::Tag sKey(xst , "SlotKey",_slotKey,"",true);
        DIOrder.close();
        XMLOstream::Tag TelemetryInput(xst , "TelemetryInput");
            XMLOstream::Tag RFPath(xst , "RFPath");
                {
                    ostringstream tmp;
                    tmp << _rfpathid;
                    XMLOstream::Tag ID(xst , "ID",tmp.str(),"",true);
               }
                XMLOstream::Tag Name(xst ,"Name",_rfpathname,"",true);
            RFPath.close();
            {
                ostringstream tmp;
                tmp << "count=\"" << _idevices.size() << "\"";
                XMLOstream::Tag ListOfInput(xst , "ListOfInput","",tmp.str());
                for(unsigned int i=0; i< _idevices.size(); i++)
                {
                    XMLOstream::Tag Input(xst , "Input");
                        ostringstream tmp1;
                        tmp1 << _idevices[i]._Channel;
                        XMLOstream::Tag Channel(xst , "Channel",tmp1.str(),"",true);
                        XMLOstream::Tag Device(xst , "Device");
                            XMLOstream::Tag PathName(xst , "PathName",_idevices[i]._PathName,"",true);
                            XMLOstream::Tag Type(xst , "Type",_idevices[i]._Type,"",true);
                        Device.close();
                        XMLOstream::Tag FileName(xst , "FileName",_idevices[i]._FileName,"",true);
                        XMLOstream::Tag dataType(xst , "DataType",_idevices[i]._DataType,"",true);
                    Input.close();
                }
                ListOfInput.close();
            }
        TelemetryInput.close();
        XMLOstream::Tag TelemetryOutput(xst , "TelemetryOutput");
            XMLOstream::Tag ProcessId(xst , "ProcessId",_processid,"",true);
            ostringstream tmp;
            tmp << "count=\"" << _odevices.size() << "\"";
            XMLOstream::Tag ListOfOutput(xst , "ListOfOutput","",tmp.str());
            for(unsigned int i=0; i< _odevices.size(); i++)
            {
                XMLOstream::Tag Output(xst , "Output");
                    ostringstream tmp1;
                    tmp1 << _odevices[i]._Channel;
                    XMLOstream::Tag Channel(xst , "Channel",tmp1.str(),"",true);
                    XMLOstream::Tag Device(xst , "Device");
                        XMLOstream::Tag PathName(xst , "PathName",_odevices[i]._PathName,"",true);
                        XMLOstream::Tag Type(xst , "Type",_odevices[i]._Type,"",true);
                    Device.close();
                    XMLOstream::Tag FileName(xst , "FileName",_odevices[i]._FileName,"",true);
                    XMLOstream::Tag dataType(xst , "DataType",_odevices[i]._DataType,"",true);
                Output.close();
            }
            ListOfOutput.close();
        TelemetryOutput.close();
        XMLOstream::Tag Downlink(xst , "Downlink");
            XMLOstream::Tag StartTime(xst , "StartTime",_starttime,"",true);
            XMLOstream::Tag StopTime(xst , "StopTime",_stoptime,"",true);
            ostringstream tempStr;
			tempStr << _orbit;
			XMLOstream::Tag Orbit(xst , "Orbit",tempStr.str(),"",true);
            ostringstream tempStr2;
            tempStr2 << _duration;
			XMLOstream::Tag Duration(xst , "Duration",tempStr2.str(), "unit=\"s\"",true);
        Downlink.close();
}
_ACS_END_NAMESPACE
