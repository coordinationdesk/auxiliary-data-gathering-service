/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema indexes script $

	$Id$

	$Author$
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  INDEXES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_circ_callbacks
*/ ------------------------------
CREATE INDEX ix_t_blockedcirculationsqueue01 ON t_blockedcirculationsqueue USING BTREE (status_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue02 ON t_blockedcirculationsqueue USING BTREE (circulationmode_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue03 ON t_blockedcirculationsqueue USING BTREE (relay_host_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue04 ON t_blockedcirculationsqueue USING BTREE (host_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue05 ON t_blockedcirculationsqueue USING BTREE (circulationpolicy_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue06 ON t_blockedcirculationsqueue USING BTREE (dest_host_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue07 ON t_blockedcirculationsqueue USING BTREE (fromurl) ;
CREATE INDEX ix_t_blockedcirculationsqueue08 ON t_blockedcirculationsqueue USING BTREE (tourl) ;
CREATE INDEX ix_t_blockedcirculationsqueue09 ON t_blockedcirculationsqueue USING BTREE (dest_protocol_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue10 ON t_blockedcirculationsqueue USING BTREE (media_circulation_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue11 ON t_blockedcirculationsqueue USING BTREE (media_insertionmode_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue12 ON t_blockedcirculationsqueue USING BTREE (enddate) ;
CREATE INDEX ix_t_blockedcirculationsqueue13 ON t_blockedcirculationsqueue USING BTREE (localtransfermode_id) ;


/* ------------------------------
	t_circ_callbacks
*/ ------------------------------
CREATE INDEX ix_t_circ_callbacks01 ON t_circ_callbacks USING BTREE (circ_queue_id) ;
CREATE INDEX ix_t_circ_callbacks02 ON t_circ_callbacks USING BTREE (external_id) ;


/* ------------------------------
	t_circulation_remove_queue
*/ ------------------------------
CREATE INDEX ix_circulation_remove_queue01 ON t_circulation_remove_queue USING BTREE (host_id) ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
CREATE INDEX ix_t_circulationpolicies01 ON t_circulationpolicies USING BTREE (circulator_node_id) ;
CREATE INDEX ix_t_circulationpolicies02 ON t_circulationpolicies USING BTREE (filetype_id) ;
CREATE INDEX ix_t_circulationpolicies03 ON t_circulationpolicies USING BTREE (source_id) ;
CREATE INDEX ix_t_circulationpolicies04 ON t_circulationpolicies USING BTREE (destination_id) ;
CREATE INDEX ix_t_circulationpolicies05 ON t_circulationpolicies USING BTREE (agent_node_id) ;
CREATE INDEX ix_t_circulationpolicies06 ON t_circulationpolicies USING BTREE (localtransfermode_id) ;
CREATE INDEX ix_t_circulationpolicies07 ON t_circulationpolicies USING BTREE (status_id) ;


/* ------------------------------
	t_circulationsmedia
*/ ------------------------------
CREATE INDEX ix_t_circulationsmedia01 ON t_circulationsmedia USING BTREE (status_id) ;


/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueue01 ON t_circulationsqueue USING BTREE (status_id) ;
CREATE INDEX ix_t_circulationsqueue02 ON t_circulationsqueue USING BTREE (circulationmode_id) ;
CREATE INDEX ix_t_circulationsqueue03 ON t_circulationsqueue USING BTREE (relay_host_id) ;
CREATE INDEX ix_t_circulationsqueue04 ON t_circulationsqueue USING BTREE (host_id) ;
CREATE INDEX ix_t_circulationsqueue05 ON t_circulationsqueue USING BTREE (circulationpolicy_id) ;
CREATE INDEX ix_t_circulationsqueue06 ON t_circulationsqueue USING BTREE (dest_host_id) ;
CREATE INDEX ix_t_circulationsqueue07 ON t_circulationsqueue USING BTREE (fromurl) ;
CREATE INDEX ix_t_circulationsqueue08 ON t_circulationsqueue USING BTREE (tourl) ;
CREATE INDEX ix_t_circulationsqueue09 ON t_circulationsqueue USING BTREE (dest_protocol_id) ;
CREATE INDEX ix_t_circulationsqueue10 ON t_circulationsqueue USING BTREE (media_circulation_id) ;
CREATE INDEX ix_t_circulationsqueue11 ON t_circulationsqueue USING BTREE (media_insertionmode_id) ;
CREATE INDEX ix_t_circulationsqueue12 ON t_circulationsqueue USING BTREE (enddate) ;
CREATE INDEX ix_t_circulationsqueue13 ON t_circulationsqueue USING BTREE (localtransfermode_id) ;
CREATE INDEX ix_t_circulationsqueue14 ON t_circulationsqueue USING BTREE (streamer_id) ;


/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueuedwh01 ON t_circulationsqueuedwh USING BTREE (status_id) ;
CREATE INDEX ix_t_circulationsqueuedwh02 ON t_circulationsqueuedwh USING BTREE (circulationmode_id) ;
CREATE INDEX ix_t_circulationsqueuedwh03 ON t_circulationsqueuedwh USING BTREE (enddate) ;
CREATE INDEX ix_t_circulationsqueuedwh04 ON t_circulationsqueuedwh USING BTREE (creationdate) ;
CREATE INDEX ix_t_circulationsqueuedwh05 ON t_circulationsqueuedwh USING BTREE (circulation_id) ;
CREATE INDEX ix_t_circulationsqueuedwh06 ON t_circulationsqueuedwh USING BTREE (source_db_host) ;
CREATE INDEX ix_t_circulationsqueuedwh07 ON t_circulationsqueuedwh USING BTREE (modificationdate) ;
CREATE INDEX ix_t_circulationsqueuedwh08 ON t_circulationsqueuedwh USING BTREE (host_id) ;


/* ------------------------------
	t_circulationstats
*/ ------------------------------
CREATE INDEX ix_t_circulationstats01 ON t_circulationstats USING BTREE (circulationsqueue_id) ;
CREATE INDEX ix_t_circulationstats02 ON t_circulationstats USING BTREE (from_host_id) ;
CREATE INDEX ix_t_circulationstats03 ON t_circulationstats USING BTREE (to_host_id) ;


/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE INDEX ix_t_datastoragequeue01 ON t_datastoragequeue USING BTREE (status_id) ;
CREATE INDEX ix_t_datastoragequeue02 ON t_datastoragequeue USING BTREE (type_id) ;
CREATE INDEX ix_t_datastoragequeue03 ON t_datastoragequeue USING BTREE (md5) ;
CREATE INDEX ix_t_datastoragequeue04 ON t_datastoragequeue USING BTREE (flowtag_id) ;


/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
CREATE INDEX ix_t_dsqueuexcirqueue01 ON t_dsqueue_x_cirqueue USING BTREE (datastoragequeue_id) ;
CREATE INDEX ix_t_dsqueuexcirqueue02 ON t_dsqueue_x_cirqueue USING BTREE (circulation_id) ;
CREATE INDEX ix_t_dsqueuexcirqueue03 ON t_dsqueue_x_cirqueue USING BTREE (globalstatus_id) ;
CREATE INDEX ix_t_dsqueuexcirqueue04 ON t_dsqueue_x_cirqueue USING BTREE (circ_status_id) ;
CREATE INDEX ix_t_dsqueuexcirqueue05 ON t_dsqueue_x_cirqueue USING BTREE (localtransfermode_id) ;


