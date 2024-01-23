/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    SRVInventory app

*/ 

#include <srvinvV.h>
#include <DefaultJobOrder.h>
#include <SRVInventory.h>
#include <NameIdAssociator.h>
#include <File.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

namespace {
	srvinvV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVInventory);

PROJECT_APP::GenericBuilder<SRVInventory> theApplicationBuilder("SRVInventory", "SRV"); // PRQA S 2311

/* Constructor */
SRVInventory::SRVInventory(const std::string& appname, const std::string& appsubsys) :
	PROJECT_APP(appname, appsubsys),	// PRQA S 4052
	mainThread_(pthread_self()),
	fileInventory_(0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

/* SRVInventory is a processor, the logname is generated always in the current directory */
std::string SRVInventory::getDefaultLogName() const {
	return File::getFileName(Application::executableName()) + ".log";
}

int SRVInventory::usage(const std::string &exename) const {
	excout << "Usage: " << exename << " joborder [-nv | --no-validity] [--historical]\n";
	excout << "    - joborder              : The joborder defining the input products/metadata to be inventoried.\n";
	excout << "    - [-nv | --no-validity] : Disables update to true for the validity flag\n";
	excout << "    - [--historical]        : Enables the historical product archiving mode. Last char of file group will be replaced with H\n";
	return 1;
}

void SRVInventory::customSigTermHandler() {
	fileInventory_->setAbortUploadFlag(true);
}

void SRVInventory::performInventory(const std::string& joborder, bool forceNotValid) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ACS_COND_THROW((!File::exists(joborder)), SRVInventoryException("JobOrder file " + joborder + " does not exists.")); 
	
	DefaultJobOrder jo;
	XMLIstream xst(joborder);
	xst >> jo;
	
	const Task& task = jo["INPUT_TASK"];	// To be chosen
	std::vector<InputDescriptor> inputs;
	task.getInputList(inputs);
	
	bool allFailed = true;
	
	size_t inputsNum = inputs.size();
	for (size_t idx = 0; idx < inputsNum; ++idx) {
		const std::vector<TaskTableVisitable*>& ttv = inputs[idx].getElements();
		
		size_t ttsize = ttv.size();
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found " << ttsize << " tasks on job order.");
		
		for (size_t jdx = 0; jdx < ttsize; ++jdx) {
			InputAlternative* alt = dynamic_cast<InputAlternative*>(ttv[jdx]);
			if (0 == alt) {
				ACS_THROW(SRVInventoryException("Cannot cast TaskTableVisitable object into InputAlternative"));
			}
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing job order input alternative " << jdx << ".");
			
			std::vector<std::string> filenames;
			alt->getPathNames(filenames);
			
			// Get the filetype id
			dbPersistent::IdType ftypeId = 0;
			{
				dbConnectionWrapper cw(*Application::instance()->getDefaultConnectionPool());
				dbConnection& conn(cw);
				
				NameIdAssociator<dbPersistent::IdType> ftypeAss("t_filetypes", "id_filetype", "filetype", conn.getKey());
				ftypeAss.load();
				ftypeId = ftypeAss.id(alt->getFileType());
			}
			
			size_t prodsNum = filenames.size();
			for (size_t pidx = 0; pidx < prodsNum; ++pidx) {
				
				if (hasStopModeChanged()) {
					// Immediate stop
					if (getCurrentStopMode() == ImmediateStop) {
						ACS_THROW(SRVInventoryImmediateStopException("SIGTERM received"));
					}
					
					// Clean stop
					else if (getCurrentStopMode() == CleanStop) {
						ACS_THROW(SRVInventoryCleanStopException("SIGINT received"));
					}
					
					// Phoenix restart
					else if (getCurrentStopMode() == PhoenixRestart) {
						// Not possible, lta inventory is not a daemon
						ACS_THROW(SRVInventoryCleanStopException("Unexpected phoenix restart"));
					}
				}
				
				std::string prodname = filenames[pidx];
				std::string metaname = prodname + ".METADATA";
				std::string attrname = prodname + ".ATTRIBUTES";
				
				// Consistency check
				ACS_COND_THROW(prodname.empty(), exIllegalValueException("Empty string for product file is not allowed."));
				ACS_COND_THROW(!File::exists(prodname), SRVInventoryException("Product file " + prodname + " does not exists."));
				ACS_COND_THROW(!File::exists(metaname), SRVInventoryException("Metadata file " + metaname + " does not exists."));
				ACS_COND_THROW(!File::exists(attrname), SRVInventoryException("Attributes file " + attrname + " does not exists."));
				
				LTAMetadata metadata(prodname, metaname, attrname, ftypeId);
				metadata.setForceNotValidFlag(forceNotValid);
				try {
					fileInventory_->inventoryProduct(metadata);		// Can be stopped by custom sigterm signal handler
					
					// Inventory success here
					allFailed = false;
				} catch (LTAFileInventory::LTAFileInventoryUploadAborted& e) {
					// Abort received
					ACS_LOG_INFO(e.what());
				} catch (LTAFileInventory::LTAFileNotFoundException& e) {
					ACS_LOG_NOTIFY_EX(e);
					ACS_LOG_WARNING("Error processing product " << prodname << ". Moving to the next one.");
				}
			}
		}
	}
	
	ACS_COND_THROW(allFailed, SRVInventoryException("The inventory of the requested products has failed"));
}

/*
 * This is the main function
 */
int SRVInventory::main(int argc, char const * const * argv, char const * const * env) { // PRQA S 4020
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	int retValue = 255;
	if (argc < 2) {
		usage(File::getFileName(Application::executableName()));
		Application::setStatus(Application::Exiting);
		return retValue;
	}
	
	try {
		addSpace(ConfigurationSingleton::ConfSpace("SRVInventory", "", false)); // PRQA S 3081
		loadConfiguration();
	} catch (dbConnection::ConnectionException& e) {
		ACS_LOG_NOTIFY_EX(e);
		Application::setStatus(Application::Exiting);
		return retValue;
	}

	ConfigurationSingleton* configSingl = ConfigurationSingleton::instance();

	rotateLog();
	
	ACS_LOG_INFO("SRVInventory started");
		
	fileInventory_ = new LTAFileInventory(*Application::instance()->getDefaultConnectionPool());
	ACS_COND_THROW(0 == fileInventory_, SRVInventoryException("Cannot initialize LTAFileInventory object."));
	
	try {
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing command line");
		std::string jofile = argv[1];
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Using joborder file " << jofile);
		
		bool forceNotValid = false;
		for (int idx = 2; idx < argc; ++idx) {
			if (StringUtils::equals(argv[idx], "-nv") || StringUtils::equals(argv[idx], "--no-validity")) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ValidityFlag for products in joborder will be forced to false");
				forceNotValid = true;
			}
			// LTA-168: added a conf space key 
			else if (StringUtils::equals(argv[idx], "--historical") ) {
				ACS_LOG_INFO ("Historical data mode requested.");
				configSingl->get().setValue ("SRVInventory.historical", true);
			}
		}
				
		// perform inventory
		performInventory(jofile, forceNotValid);

		ACS_LOG_INFO("Inventory completed successfully.");
		retValue = 0;
	} catch (SRVInventoryCleanStopException& e) {
		ACS_LOG_INFO(e.what());
		Application::setStatus(Application::Exiting);
		retValue = 0;
	} catch (SRVInventoryImmediateStopException& e) {
		ACS_LOG_INFO(e.what());
		Application::setStatus(Application::Exiting);
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
		Application::setStatus(Application::Exiting);
	}
	
	delete fileInventory_;
	
	return retValue;
}

_ACS_END_NAMESPACE
