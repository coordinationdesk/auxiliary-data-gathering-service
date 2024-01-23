#
#       Copyright 1995-2013, Exprivia SPA
#       Via Della Bufalotta, 378 - 00139 Roma - Italy
#
#       All Rights Reserved.
#

SHELL=/bin/bash


# Don't care about application listing order in ALL_APP
# By definition the applications are not dependent each other.
# a good order could be the lexicographycal one.

ALL_APP=\
		Scheduler:CCDSAUXIP:CCDSAUXIPQA \
		SRVInventory:CCDSAUXIP:CCDSAUXIPQA \
		SRVImporter:CCDSAUXIP:CCDSAUXIPQA \
		SRVAttributeExtractor:CCDSAUXIP:CCDSAUXIPQA \
		PRIPRolling:CCDSAUXIP:CCDSAUXIPQA \
		SRVNotificationDispatcher:CCDSAUXIP:CCDSAUXIPQA \
		SRVRemoveInventory:CCDSAUXIP:CCDSAUXIPQA \
		SRVDownloadInventory:CCDSAUXIP:CCDSAUXIPQA \
		Encrypter:CCDSAUXIP:CCDSAUXIPQA \
	    Decrypter:CCDSAUXIP:CCDSAUXIPQA \
		LTAQuotaService:CCDSAUXIP:CCDSAUXIPQA \
        GarbageCleaner:CCDSAUXIP:CCDSAUXIPQA \
		LTAInventoryMetricsTracker:CCDSAUXIP:CCDSAUXIPQA


