/*

 Exprivia S.p.A.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.it

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
 the contents of this file may not be disclosed to third parties, copied
 or duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia S.p.A.

*/





--------------------------------------------------------------------------------
--                                                                            --
--                                CONSTRAINTS                                 --
--                                                                            --
--------------------------------------------------------------------------------


-------------------------------------------------
--------------- CHECK CONSTRAINTS ---------------
-------------------------------------------------

/* ------------------------------
	t_blockedcirculationsqueue
*/ ------------------------------
ALTER TABLE t_blockedcirculationsqueue
	ADD CONSTRAINT ck_blockedcirculationsqueue01			
		CHECK (char_length(trim(tempnamevalue)) != 0) ;

ALTER TABLE t_blockedcirculationsqueue
	ADD CONSTRAINT ck_blockedcirculationsqueue02
		CHECK ((tempname_id != 2 AND (tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;

ALTER TABLE t_blockedcirculationsqueue
	ADD CONSTRAINT ck_blockedcirculationsqueue03
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './')) != TRUE) ;


/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue01			
		CHECK (char_length(trim(tempnamevalue)) != 0) ;

ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue02
		CHECK ((tempname_id != 2 AND (tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;

ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue03
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './')) != TRUE) ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies
	ADD CONSTRAINT ck_circulationpolicies01			
		CHECK (source_id != destination_id) ;


/* ------------------------------
	t_circulationstats
*/ ------------------------------
ALTER TABLE t_circulationstats
	ADD CONSTRAINT ck_circulationstats01			
		CHECK ((from_host_id IS NOT NULL AND unkn_from_host IS NULL) OR (from_host_id IS NULL AND unkn_from_host IS NOT NULL));

ALTER TABLE t_circulationstats
	ADD CONSTRAINT ck_circulationstats02			
		CHECK ((to_host_id IS NOT NULL AND unkn_to_host IS NULL) OR (to_host_id IS NULL AND unkn_to_host IS NOT NULL));


/* ------------------------------
	t_knownhosts_limits
*/ ------------------------------
ALTER TABLE t_knownhosts_limits
  ADD CONSTRAINT ck_knownhosts_limits01
  	CHECK (max_upload_slots > 0) ;








-------------------------------------------------
--------------- UNIQUE CONSTRAINTS --------------
-------------------------------------------------


/* ------------------------------
	t_circ_callbacks
*/ ------------------------------
ALTER TABLE t_circ_callbacks 
	ADD CONSTRAINT uq_t_circ_callbacks01
		UNIQUE (circ_queue_id) ;


/* ------------------------------
	t_circulationmodes
*/ ------------------------------
ALTER TABLE t_circulationmodes 
	ADD CONSTRAINT uq_circulationmodes01
		UNIQUE (circulationmode) ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT uq_circulationpolicies01
		UNIQUE (filetype_id,circulator_node_id,source_id,destination_id) ;


/* ------------------------------
   t_circulationpolicies_monitor
*/ ------------------------------
ALTER TABLE t_circulationpolicies_monitor 
	ADD CONSTRAINT uq_t_circulationpolicies_monitor01
		UNIQUE (circulationpolicy_id);


/* ------------------------------
	t_circulationsmedia
*/ ------------------------------
ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT uq_circulationsmedia01
		UNIQUE (media_tag, source) ;


/* ------------------------------
	t_circulationsmedia_insmode 
*/ ------------------------------
ALTER TABLE t_circulationsmedia_insmode 
	ADD CONSTRAINT uq_circulationsmedia_insmode01
		UNIQUE (mode) ;


/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE UNIQUE INDEX uq_datastoragequeue01 ON t_datastoragequeue 
	USING btree (md5, type_id)
	WHERE status_id NOT IN (3, 8) AND md5 IS NOT NULL ;


/* ------------------------------
	t_datastoragetypes
*/ ------------------------------
ALTER TABLE t_datastoragetypes 
	ADD CONSTRAINT uq_datastoragetypes01
		UNIQUE (datastoragetype) ;


/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT uq_dsqueuexcirqueue01
		UNIQUE (datastoragequeue_id, circulation_id) ;


/* ------------------------------
	t_flowtags
*/ ------------------------------
ALTER TABLE t_flowtags 
	ADD CONSTRAINT uq_t_flowtags 
		UNIQUE (flowtagname);


/* ------------------------------
	t_knownhosts_limits
*/ ------------------------------
ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT uq_knownhosts_limits01
		UNIQUE (circulator_node_id,dest_host_id) ;


/* ------------------------------
	t_securityzones
*/ ------------------------------
ALTER TABLE t_securityzones 
	ADD CONSTRAINT uq_securityzones01
		UNIQUE (securityzone) ;


/* ------------------------------
	t_securityzonerelays
*/ ------------------------------
ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT uq_securityzonerelays01
		UNIQUE (host_id, seczone_id) ;


/* ------------------------------
	t_streamers
*/ ------------------------------
ALTER TABLE t_streamers 
	ADD CONSTRAINT uq_streamers01
		UNIQUE (streamtype) ;


