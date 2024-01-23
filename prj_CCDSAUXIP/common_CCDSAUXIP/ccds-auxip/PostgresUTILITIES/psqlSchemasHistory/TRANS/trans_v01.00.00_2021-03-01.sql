
-- Version 01.00.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

DO $$
BEGIN
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
END $$;



/* ------------------------------
	t_statetransitions_flows
*/ ------------------------------
CREATE TABLE t_statetransitions_flows (
    id SMALLINT NOT NULL,
    flow_type VARCHAR(30) NOT NULL
) ;
ALTER TABLE t_statetransitions_flows
	ADD CONSTRAINT pk_t_statetransitions_flows
		PRIMARY KEY (id) ;

ALTER TABLE t_statetransitions_flows 
	ADD CONSTRAINT uq_statetransitions_flows01
		UNIQUE (flow_type) ;

INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (1, 'none') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (2, 'ARCHIVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (3, 'DELETE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (4, 'LTA_ARCHIVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (5, 'RETRIEVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (6, 'REPUBLISHING') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (7, 'RETRIEVEONLY') ;



/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
CREATE TABLE t_statetransitions_tables (
    id SERIAL NOT NULL,
    table_name VARCHAR(30) NOT NULL,
    flow_id SMALLINT DEFAULT 1 NOT NULL
) ;
ALTER TABLE t_statetransitions_tables
	ADD CONSTRAINT pk_t_statetransitions_tables
		PRIMARY KEY (id) ;

ALTER TABLE t_statetransitions_tables 
	ADD CONSTRAINT uq_statetransitions_tables01
		UNIQUE (table_name, flow_id) ;

ALTER TABLE t_statetransitions_tables 
	ADD CONSTRAINT fk_statetransitions_tables01
		FOREIGN KEY (flow_id)
			REFERENCES t_statetransitions_flows (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_statetransitions_tables01 ON t_statetransitions_tables USING BTREE (flow_id) ;



/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
CREATE TABLE t_statetransitions_allowed (
    id SERIAL NOT NULL,
    table_id INT NOT NULL,
	prev_state_id INT2 NOT NULL,
    post_state_id INT2 NOT NULL
) ;
ALTER TABLE t_statetransitions_allowed
	ADD CONSTRAINT pk_t_statetransitions_allowed
		PRIMARY KEY (id) ;

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT uq_statetransitions_allowed01
		UNIQUE (table_id, prev_state_id, post_state_id) ;

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT fk_statetransitions_allowed01
		FOREIGN KEY (table_id)
			REFERENCES t_statetransitions_tables (id)
				ON DELETE CASCADE ;

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT fk_statetransitions_allowed02
		FOREIGN KEY (prev_state_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT fk_statetransitions_allowed03
		FOREIGN KEY (post_state_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_statetrans_allowed01 ON t_statetransitions_allowed USING BTREE (table_id) ;
CREATE INDEX ix_t_statetrans_allowed02 ON t_statetransitions_allowed USING BTREE (prev_state_id) ;
CREATE INDEX ix_t_statetrans_allowed03 ON t_statetransitions_allowed USING BTREE (post_state_id) ;








-------------------------------------------------------------------------------
-------------------------------- VIEWS CREATION -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	v_state_transitions
*/ ------------------------------
CREATE OR REPLACE VIEW v_state_transitions (
	table_name, 
	flow, 
	prev_status,
	post_status)
AS SELECT 
	st.table_name, 
	sf.flow_type  AS flow,
	ss1.status AS pre_s,
	ss2.status AS pst_s
FROM 
	t_statetransitions_tables AS st
	INNER JOIN t_statetransitions_flows AS sf 
		ON st.flow_id = sf.id
	INNER JOIN t_statetransitions_allowed AS sa 
		ON st.id = sa.table_id
	INNER JOIN t_srv_statuses AS ss1 
		ON sa.prev_state_id = ss1.id
	INNER JOIN t_srv_statuses AS ss2 
		ON sa.post_state_id = ss2.id
ORDER BY 
	table_name, 
	flow, 
	pre_s,
	pst_s
;








-------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTIONS CREATION --------------------------
-------------------------------------------------------------------------------

/* --------------------------
	tf_check_status_changes
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_check_status_changes() RETURNS TRIGGER AS $$

DECLARE
	tbl_id INTEGER;
	pre_st_name VARCHAR(32);
	pst_st_name VARCHAR(32);
	no_table_ex_msg TEXT;
	no_trans_ex_msg TEXT;
	
BEGIN
	-- Retrieve the id of the table using the table name and the flow
	SELECT id INTO tbl_id FROM t_statetransitions_tables WHERE table_name = TG_TABLE_NAME AND flow_id = 1 ;
	
	-- Retrieve status name by id (just for error message)
	SELECT status INTO pre_st_name FROM t_srv_statuses WHERE id = OLD.status_id ;
	SELECT status INTO pst_st_name FROM t_srv_statuses WHERE id = NEW.status_id ;
		
	-- Set no table error messages
	no_table_ex_msg := 'p_StateTransitions_Check: table ' || TG_TABLE_NAME || ' not regestired into t_statetransitions_tables table.' ; 
	-- Set no transition error messages
	no_trans_ex_msg := 'p_StateTransitions_Check: table ' || TG_TABLE_NAME || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	
	-- Check for registered table
	IF tbl_id IS NULL THEN
		RAISE EXCEPTION '%', no_table_ex_msg ;
	END IF ;
	
	-- Check for allowed transition
	IF (SELECT count(*) FROM t_statetransitions_allowed WHERE table_id = tbl_id AND prev_state_id = OLD.status_id AND post_state_id = NEW.status_id) = 0 THEN
		RAISE EXCEPTION '%', no_trans_ex_msg ;
	END IF ;
		
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.tables WHERE table_name = 't_schema') = 1
	THEN
		DELETE FROM t_schema WHERE version = 'trans-01.00.00' ;

		INSERT INTO t_schema (
				version, 
				modificationdate,
				modificationtype,
				schematype,
				cvstag,
				description )
		VALUES	(
				'trans-01.00.00',
				'2021-03-01',
				2,
				35,
				'TRANS_1-0-0',
				'First development'
		) ;
	END IF ;
END $$;

