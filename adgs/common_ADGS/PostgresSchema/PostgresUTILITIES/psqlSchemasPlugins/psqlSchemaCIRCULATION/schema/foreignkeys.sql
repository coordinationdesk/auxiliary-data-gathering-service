/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema foreign keys script $

	$Id$

	$Author$
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  FOREIGN KEYS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_blockedcirculationsqueue
*/ ------------------------------
ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue02
		FOREIGN KEY (circulationmode_id) REFERENCES t_circulationmodes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue03
		FOREIGN KEY (relay_host_id) REFERENCES t_securityzonerelays (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue04
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue05
		FOREIGN KEY (circulationpolicy_id) REFERENCES t_circulationpolicies (id) 
			ON DELETE SET NULL ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue06
		FOREIGN KEY (dest_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue07
		FOREIGN KEY (tempname_id) REFERENCES t_tmpnamestrategy (id_tmpnamestrategy) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue08
		FOREIGN KEY (dest_protocol_id) REFERENCES t_transferprotocols (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue09
		FOREIGN KEY (media_circulation_id) REFERENCES t_circulationsmedia (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue10
		FOREIGN KEY (media_insertionmode_id) REFERENCES t_circulationsmedia_insmode (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue11
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circ_callbacks
*/ ------------------------------
ALTER TABLE t_circ_callbacks 
	ADD CONSTRAINT fk_circ_callbacks01
		FOREIGN KEY (circ_queue_id) REFERENCES t_circulationsqueue (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circulation_remove_queue
*/ ------------------------------
ALTER TABLE t_circulation_remove_queue 
	ADD CONSTRAINT fk_circulation_remove_queue01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circulationactivehosts
*/ ------------------------------
ALTER TABLE t_circulationactivehosts 
	ADD CONSTRAINT fk_circulationactivehosts01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies01
		FOREIGN KEY (circulator_node_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies02
		FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies03
		FOREIGN KEY (source_id) REFERENCES t_repositories (id_repository) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies04
		FOREIGN KEY (destination_id) REFERENCES t_repositories (id_repository) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies05
		FOREIGN KEY (agent_node_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies06
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies07
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;



/* ------------------------------
   t_circulationpolicies_monitor
*/ ------------------------------
ALTER TABLE t_circulationpolicies_monitor 
	ADD CONSTRAINT fk_t_circulationpolicies_monitor01
		FOREIGN KEY (circulationpolicy_id) 
			REFERENCES t_circulationpolicies (id) 
				ON DELETE CASCADE;



/* ------------------------------
	t_circulationsmedia
*/ ------------------------------
ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT fk_circulationsmedia01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue02
		FOREIGN KEY (circulationmode_id) REFERENCES t_circulationmodes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue03
		FOREIGN KEY (relay_host_id) REFERENCES t_securityzonerelays (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue04
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue05
		FOREIGN KEY (circulationpolicy_id) REFERENCES t_circulationpolicies (id) 
			ON DELETE SET NULL ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue06
		FOREIGN KEY (dest_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue07
		FOREIGN KEY (tempname_id) REFERENCES t_tmpnamestrategy (id_tmpnamestrategy) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue08
		FOREIGN KEY (dest_protocol_id) REFERENCES t_transferprotocols (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue09
		FOREIGN KEY (media_circulation_id) REFERENCES t_circulationsmedia (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue10
		FOREIGN KEY (media_insertionmode_id) REFERENCES t_circulationsmedia_insmode (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue11
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue12
		FOREIGN KEY (streamer_id) REFERENCES t_streamers (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circulationstats
*/ ------------------------------
ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats01
		FOREIGN KEY (circulationsqueue_id) REFERENCES t_circulationsqueue (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats02
		FOREIGN KEY (from_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats03
		FOREIGN KEY (to_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT fk_datastoragequeue02
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT fk_datastoragequeue03
		FOREIGN KEY (type_id) REFERENCES t_datastoragetypes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT fk_t_datastoragequeue 
		FOREIGN KEY (flowtag_id) REFERENCES t_flowtags (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue01
		FOREIGN KEY (datastoragequeue_id) REFERENCES t_datastoragequeue (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue03
		FOREIGN KEY (globalstatus_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue04
		FOREIGN KEY (circ_status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue05
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_securityzonerelays
*/ ------------------------------
ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT fk_securityzonerelays01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT fk_securityzonerelays02
		FOREIGN KEY (seczone_id) REFERENCES t_securityzones (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_knownhosts_limits
*/ ------------------------------
ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT fk_knownhosts_limits01
		FOREIGN KEY (circulator_node_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT fk_knownhosts_limits02
		FOREIGN KEY (dest_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;




