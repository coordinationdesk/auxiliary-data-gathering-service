#
#	Copyright 1995-2012, Advanced Computer Systems , Inc.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Advanced Computer Systems, Inc.
#
#
#	$Author$
#
#	$Prod: A.C.S. ADGS Applications Makefile $
#
#	$Id$
#
#   $Log$
#

	
# Don't care about application listing order in ALL_APP
# By definition the applications are not dependent each other.
# a good order could be the lexicographycal one.
# WARNING: MkTclApp, if required, must be placed BEFORE TCL APPLICATIONS, since is required to compile them.
# (Trick, always place FIRST)

ALL_APP=\
	adgs_jiraissuedownloader:ADGS \
	ACSPhpLibLite:ADGS \
	ADGSBaselineReportGenerator:ADGS \
	ADGSCatalogueExport:ADGS \
	DataRetriever:ADGS \
	Decrypter:ADGS \
	DistributorGenerator:ADGS \
	DownloadInventoryFile:ADGS \
	Encrypter:ADGS \
	FileVersioningTask:ADGS \
	ICECONCPreInventoryRuby:ADGS \
	Importer:ADGS \
	Inventory:ADGS \
	MultiDownloadInventoryObject:ADGS \
	MultiRemoveInventoryObject:ADGS \
	OBMEMCPreInventoryRuby:ADGS \
	OrderGenerator:ADGS \
	PackageCheckCirculatorTask:ADGS \
	PackageDistCirculatorTask:ADGS \
	PackageDistCurlTask:ADGS \
	PackageDistFilesystemTask:ADGS \
	PackageExporter:ADGS \
	PackageFmtEOFpackTask:ADGS \
	PackagerOptimiser:ADGS \
	pds2_import_adgs:ADGS \
	RemoveInventoryObject:ADGS \
	RollingArchive:ADGS \
	S1AuxReformatter:ADGS \
	S1PreInventoryAuxTroTec:ADGS \
	S2CAMSReFormatterTask:ADGS \
	S2ECMWFReFormatterTask:ADGS \
	S2GIPPPreInventory:ADGS \
	S2PreInventoryReformatterTask:ADGS \
	S3AuxReformatterCore:ADGS \
	S3AuxReformMetadataExtractor:ADGS \
	Scheduler:ADGS \
	TaskInPreInventory:ADGS \
	TlDaemon:ADGS \
	TLPreInventory:ADGS \
	TTDecompress:ADGS \
	WSConfigBuilder:ADGS \
	WSConfigMerger:ADGS \

#	AntexValidatorTask:XXXXADGS \
	DbaseValidatorTask:XXXXADGS \
	EopValidatorTask:XXXXADGS \
	GRIBValidatorTask:XXXXADGS \
	IersEOValidatorTask:XXXXADGS \
	IERSValidatorTask:XXXXADGS \
	InstallConfSpace:XXXXADGS \
	LeapSecondValidatorTask:XXXXADGS \
	MSSLValidatorTask:XXXXADGS \
	NETCDFValidatorTask:XXXXADGS \
	NoaaSFValidatorTask:XXXXADGS \
	RinexValidatorTask:XXXXADGS \
	Sp3ValidatorTask:XXXXADGS \
	StatBroEngine:XXXXADGS \
	TimeSequenceErrorReport:XXXXADGS \
	XMLFmtValidatorTask:XXXXADGS \
	ADCReportGenerator:S3PDPADC \
	CoverageReport:S3PDPADC \
