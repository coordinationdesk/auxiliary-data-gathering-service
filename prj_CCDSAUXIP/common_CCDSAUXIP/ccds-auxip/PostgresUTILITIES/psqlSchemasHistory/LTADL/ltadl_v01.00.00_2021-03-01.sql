
-- Version 01.00.00
-- POSTGRES





--------------------------------------------------------------------------------
------------------------------ EXTENSION CREATION ------------------------------
--------------------------------------------------------------------------------

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";








-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.tables WHERE table_name = 't_statetransitions_tables') = 1
	THEN
		IF (SELECT count(*) FROM information_schema.tables WHERE table_name = 't_srv_statuses') = 0
		THEN
			/* ------------------------------
				t_srv_statuses
			*/ ------------------------------
			CREATE TABLE t_srv_statuses (
				id BIGINT NOT NULL,
				status VARCHAR(32) NOT NULL
			) ;
			ALTER TABLE t_srv_statuses 
				ADD CONSTRAINT pk_t_srv_statuses
					PRIMARY KEY (id) ;

			ALTER TABLE t_srv_statuses 
				ADD CONSTRAINT uq_srv_statuses01
					UNIQUE (status) ;

			INSERT INTO t_srv_statuses (id, status) VALUES (1, 'BOOKED') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (2, 'CANCELLED') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (3, 'COMPLETED') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (4, 'CREATED') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (5, 'DISTRIBUTION') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (6, 'ELIGIBLE') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (7, 'ERROR') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (8, 'IN_PROGRESS') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (9, 'LTA_DOWNLOADING') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (10, 'TO_BE_CANCELLED') ;
			INSERT INTO t_srv_statuses (id, status) VALUES (11, 'RUNNING');
			INSERT INTO t_srv_statuses (id, status) VALUES (12, 'PAUSED');
			INSERT INTO t_srv_statuses (id, status) VALUES (13, 'ONLINE');
			INSERT INTO t_srv_statuses (id, status) VALUES (14, 'ONGOING');
			INSERT INTO t_srv_statuses (id, status) VALUES (15, 'EVICTED');
			INSERT INTO t_srv_statuses (id, status) VALUES (16, 'QUEUED');
			INSERT INTO t_srv_statuses (id, status) VALUES (17, 'UNRECOVERABLEERROR');
			INSERT INTO t_srv_statuses (id, status) VALUES (18, 'RECOVERABLEERROR');
			INSERT INTO t_srv_statuses (id, status) VALUES (19, 'TRANSFERRING');
			INSERT INTO t_srv_statuses (id, status) VALUES (20, 'BLOCKED');
			INSERT INTO t_srv_statuses (id, status) VALUES (21, 'PROCESSING');
			INSERT INTO t_srv_statuses (id, status) VALUES (22, 'ABORTED');
			INSERT INTO t_srv_statuses (id, status) VALUES (23, 'NOTFOUNDERROR');
			INSERT INTO t_srv_statuses (id, status) VALUES (24, 'INEVICTION');
			INSERT INTO t_srv_statuses (id, status) VALUES (25, 'VALID');
			INSERT INTO t_srv_statuses (id, status) VALUES (26, 'IN_USE');
			INSERT INTO t_srv_statuses (id, status) VALUES (27, 'SUBMITTED');
			INSERT INTO t_srv_statuses (id, status) VALUES (28, 'OFFLINE');
			INSERT INTO t_srv_statuses (id, status) VALUES (29, 'LOST'); 
		END IF ;
		
		
		
		/* ------------------------------
			t_ltadl_aip_sel_algos
		*/ ------------------------------
		CREATE TABLE t_ltadl_aip_sel_algos (
			id SERIAL NOT NULL,
			name VARCHAR(256) NOT NULL
		) ;
		ALTER TABLE t_ltadl_aip_sel_algos
			ADD CONSTRAINT pk_t_ltadl_aip_sel_algos
				PRIMARY KEY (id) ;

		ALTER TABLE t_ltadl_aip_sel_algos 
			ADD CONSTRAINT uq_ltadl_aip_sel_algos01
				UNIQUE (name) ;



		/* ------------------------------
			t_ltadl_remote_lta
		*/ ------------------------------
		CREATE TABLE t_ltadl_remote_lta (
			id SERIAL NOT NULL,
			lta_base_url VARCHAR(1024) NOT NULL,
			username VARCHAR(128) NOT NULL,
			passwd VARCHAR(1024) NOT NULL,
			name VARCHAR(128) NOT NULL
		) ;
		ALTER TABLE t_ltadl_remote_lta
			ADD CONSTRAINT pk_t_ltadl_remote_lta
				PRIMARY KEY (id) ;

		ALTER TABLE t_ltadl_remote_lta 
			ADD CONSTRAINT uq_ltadl_remote_lta01
				UNIQUE (name) ;



		/* ------------------------------
			t_ltadl_bulkorders
		*/ ------------------------------
		CREATE TABLE t_ltadl_bulkorders (
			id SERIAL NOT NULL,
			creation_date TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			aip_sel_algo_id BIGINT NOT NULL,
			filter_query VARCHAR(1024) NOT NULL,
			destination_url VARCHAR(1024) NOT NULL
		) ;
		ALTER TABLE t_ltadl_bulkorders
			ADD CONSTRAINT pk_t_ltadl_bulkorders
				PRIMARY KEY (id) ;

		ALTER TABLE t_ltadl_bulkorders 
			ADD CONSTRAINT fk_ltadl_bulkorders01
				FOREIGN KEY (aip_sel_algo_id)
					REFERENCES t_ltadl_aip_sel_algos (id)
						ON DELETE CASCADE ;

		CREATE INDEX ix_t_ltadl_bulkorders01 ON t_ltadl_bulkorders USING BTREE (aip_sel_algo_id) ;



		/* ------------------------------
			t_ltadl_batchqueue
		*/ ------------------------------
		CREATE TABLE t_ltadl_batchqueue (
			id SERIAL NOT NULL,
			status_id BIGINT NOT NULL,
			creation_date TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			modification_date TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			lta_batch_UUID VARCHAR(160) NOT NULL,
			lta_id BIGINT NOT NULL,
			ltadl_bulk_id INTEGER NOT NULL,
			extract_metadata BOOLEAN DEFAULT FALSE
		) ;
		ALTER TABLE t_ltadl_batchqueue
			ADD CONSTRAINT pk_t_ltadl_batchqueue
				PRIMARY KEY (id) ;

		ALTER TABLE t_ltadl_batchqueue 
			ADD CONSTRAINT fk_ltadl_batchqueue01
				FOREIGN KEY (status_id)
					REFERENCES t_srv_statuses (id)
						ON DELETE CASCADE ;

		ALTER TABLE t_ltadl_batchqueue 
			ADD CONSTRAINT fk_ltadl_batchqueue02
				FOREIGN KEY (lta_id)
					REFERENCES t_ltadl_remote_lta (id)
						ON DELETE CASCADE ;

		ALTER TABLE t_ltadl_batchqueue 
			ADD CONSTRAINT fk_ltadl_batchqueue03
				FOREIGN KEY (ltadl_bulk_id)
					REFERENCES t_ltadl_bulkorders (id)
						ON DELETE CASCADE ;

		CREATE INDEX ix_t_ltadl_batchqueue01 ON t_ltadl_batchqueue USING BTREE (status_id) ;
		CREATE INDEX ix_t_ltadl_batchqueue02 ON t_ltadl_batchqueue USING BTREE (modification_date) ;
		CREATE INDEX ix_t_ltadl_batchqueue03 ON t_ltadl_batchqueue USING BTREE (lta_batch_UUID) ;
		CREATE INDEX ix_t_ltadl_batchqueue04 ON t_ltadl_batchqueue USING BTREE (lta_id) ;
		CREATE INDEX ix_t_ltadl_batchqueue05 ON t_ltadl_batchqueue USING BTREE (ltadl_bulk_id) ;

		

		/* ------------------------------
			t_ltadl_filequeue
		*/ ------------------------------
		CREATE TABLE t_ltadl_filequeue (
			id SERIAL NOT NULL,
			filename VARCHAR(256) NOT NULL,
			lta_id BIGINT NOT NULL,
			lta_order_UUID VARCHAR(160),
			status_id BIGINT NOT NULL,
			ltadl_batch_id BIGINT,
			destination_url VARCHAR(1024) NOT NULL,
			creation_date TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			modification_date TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			extract_metadata BOOLEAN DEFAULT FALSE,
			lta_product_uuid VARCHAR(160),
			jobresponsible VARCHAR(64)
		) ;
		ALTER TABLE t_ltadl_filequeue
			ADD CONSTRAINT pk_t_ltadl_filequeue
				PRIMARY KEY (id) ;

		ALTER TABLE t_ltadl_filequeue 
			ADD CONSTRAINT fk_ltadl_filequeue01
				FOREIGN KEY (lta_id)
					REFERENCES t_ltadl_remote_lta (id)
						ON DELETE CASCADE ;

		ALTER TABLE t_ltadl_filequeue 
			ADD CONSTRAINT fk_ltadl_filequeue02
				FOREIGN KEY (status_id)
					REFERENCES t_srv_statuses (id)
						ON DELETE CASCADE ;

		ALTER TABLE t_ltadl_filequeue 
			ADD CONSTRAINT fk_ltadl_filequeue03
				FOREIGN KEY (ltadl_batch_id)
					REFERENCES t_ltadl_batchqueue (id)
						ON DELETE CASCADE ;

		CREATE INDEX ix_t_ltadl_filequeue01 ON t_ltadl_filequeue USING BTREE (lta_id) ;
		CREATE INDEX ix_t_ltadl_filequeue02 ON t_ltadl_filequeue USING BTREE (lta_order_UUID) ;
		CREATE INDEX ix_t_ltadl_filequeue03 ON t_ltadl_filequeue USING BTREE (status_id) ;
		CREATE INDEX ix_t_ltadl_filequeue04 ON t_ltadl_filequeue USING BTREE (modification_date) ;
		CREATE INDEX ix_t_ltadl_filequeue05 ON t_ltadl_filequeue USING BTREE (ltadl_batch_id) ;

		--NOTA: I campi lta_order_UUID  e ltadl_batch_id della tabella t_ltadl_filequeue non possono essere entrambi null o notnull.

				
		
		/* ------------------------------
			t_ltadl_activesubscriptions
		*/ ------------------------------
		CREATE TABLE t_ltadl_activesubscriptions (
			id BIGSERIAL NOT NULL,
			subscription_uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
			subscription_date TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			remote_lta_id INTEGER NOT NULL
		) ;
		ALTER TABLE t_ltadl_activesubscriptions
			ADD CONSTRAINT pk_t_ltadl_activesubscriptions
				PRIMARY KEY (id) ;
		
		ALTER TABLE t_ltadl_activesubscriptions 
			ADD CONSTRAINT uq_ltadl_activesubscriptions01
				UNIQUE (subscription_uuid, remote_lta_id) ;
		
		ALTER TABLE t_ltadl_activesubscriptions 
			ADD CONSTRAINT fk_ltadl_activesubscriptions01
				FOREIGN KEY (remote_lta_id)
					REFERENCES t_ltadl_remote_lta (id)
						ON DELETE CASCADE ;
		
		CREATE INDEX ix_t_ltadl_activesubscriptions01 ON t_ltadl_activesubscriptions USING BTREE (subscription_uuid) ;
		CREATE INDEX ix_t_ltadl_activesubscriptions02 ON t_ltadl_activesubscriptions USING BTREE (subscription_date) ;
		CREATE INDEX ix_t_ltadl_activesubscriptions03 ON t_ltadl_activesubscriptions USING BTREE (remote_lta_id) ;
		
		
		
		/* ------------------------------
			t_ltadl_offlinedata
		*/ ------------------------------
		CREATE TABLE t_ltadl_offlinedata (
			id BIGSERIAL NOT NULL,
			inv_id BIGINT NOT NULL,
			detection_time TIMESTAMP(3) NOT NULL DEFAULT NOW(),
			remote_lta_id INTEGER NOT NULL
		) ;
		ALTER TABLE t_ltadl_offlinedata
			ADD CONSTRAINT pk_t_ltadl_offlinedata
				PRIMARY KEY (id) ;
		
		ALTER TABLE t_ltadl_offlinedata 
			ADD CONSTRAINT fk_ltadl_offlinedata01
				FOREIGN KEY (remote_lta_id)
					REFERENCES t_ltadl_remote_lta (id)
						ON DELETE CASCADE ;
		
		CREATE INDEX ix_t_ltadl_offlinedata01 ON t_ltadl_offlinedata USING BTREE (inv_id) ;
		CREATE INDEX ix_t_ltadl_offlinedata02 ON t_ltadl_offlinedata USING BTREE (detection_time) ;
		CREATE INDEX ix_t_ltadl_offlinedata03 ON t_ltadl_offlinedata USING BTREE (remote_lta_id) ;
		
		
		
		
		
		
		
		
		-------------------------------------------------------------------------------
		-------------------------------- TABLES INSERT --------------------------------
		-------------------------------------------------------------------------------

		/* ------------------------------
			t_statetransitions_tables
		*/ ------------------------------
		INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_ltadl_batchqueue') ;
		INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_ltadl_filequeue') ;
		
		
		
		/* ------------------------------
			t_statetransitions_allowed
		*/ ------------------------------

		-- t_ltadl_batchqueue
		----------------------
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
			(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING')) ;

		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING'),
			(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING'),
			(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING'),
			(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;


		-- t_ltadl_filequeue
		----------------------
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE'),
			(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED')) ;

		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
			(SELECT id FROM t_srv_statuses WHERE status = 'TRANSFERRING')) ;
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
			(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'OFFLINE'),
			(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'OFFLINE'),
			(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE')) ;

		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'TRANSFERRING'),
			(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'TRANSFERRING'),
			(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;
		INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
			(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
			(SELECT id FROM t_srv_statuses WHERE status = 'TRANSFERRING'),
			(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE')) ;








		--------------------------------------------------------------------------------
		------------------------------- TRIGGER CREATION -------------------------------
		--------------------------------------------------------------------------------

		/* ------------------------------
			t_ltadl_batchqueue
		*/ ------------------------------
		CREATE TRIGGER tr_ltadl_batchqueue_check_status BEFORE UPDATE OF status_id
			ON t_ltadl_batchqueue
				FOR EACH ROW EXECUTE PROCEDURE tf_check_status_changes() ;


		/* ------------------------------
			t_ltadl_filequeue
		*/ ------------------------------
		CREATE TRIGGER tr_ltadl_filequeue_check_status BEFORE UPDATE OF status_id
			ON t_ltadl_filequeue
				FOR EACH ROW EXECUTE PROCEDURE tf_check_status_changes() ;








		-------------------------------------------------------------------------------
		--------------------------------- CONCLUSIONS ---------------------------------
		-------------------------------------------------------------------------------
		IF (SELECT count(*) FROM information_schema.tables WHERE table_name = 't_schema') = 1
		THEN
			DELETE FROM t_schema WHERE version = 'ltadl-01.00.00' ;

			INSERT INTO t_schema (
					version, 
					modificationdate,
					modificationtype,
					schematype,
					cvstag,
					description )
			VALUES	(
					'ltadl-01.00.00',
					'2021-03-01',
					2,
					37,
					'LTADL_1-0-0',
					'First development'
			) ;
		END IF;
		
		
		
	ELSE
		RAISE NOTICE '%', '*** ERROR!!! - Cannot install LTADL schema ***' || E'\n' || '>> Cause: TRANS (transition check) schema needed' ;
	
	
	END IF ;
END $$;


