
-- Version 12.09.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_emitted_order_events
*/ ------------------------------
CREATE TABLE t_emitted_order_events (
	id SERIAL NOT NULL,
	event_time TIMESTAMP NOT NULL,
	event_tag VARCHAR(64) NOT NULL,
	time_driven_start TIMESTAMP NOT NULL,
	time_driven_stop TIMESTAMP NOT NULL,
	processor_id INTEGER NOT NULL,
	order_type VARCHAR(8) NOT NULL,
	creation_date TIMESTAMP DEFAULT NOW()
) ;

ALTER TABLE t_emitted_order_events
	ADD CONSTRAINT pk_t_emitted_order_events
		PRIMARY KEY (id) ;

ALTER TABLE t_emitted_order_events 
	ADD CONSTRAINT uq_emitted_order_events01
		UNIQUE (event_time, event_tag, time_driven_start, time_driven_stop) ;

ALTER TABLE t_emitted_order_events
	ADD CONSTRAINT fk_emitted_order_events01
		FOREIGN KEY (processor_id)
			REFERENCES t_processors (id_processor)
				ON DELETE CASCADE ;

ALTER TABLE t_emitted_order_events
	ADD CONSTRAINT fk_emitted_order_events02
		FOREIGN KEY (order_type)
			REFERENCES t_ordertypes (ordertype)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_emitted_order_events01 ON t_emitted_order_events USING BTREE (event_time) ;
CREATE INDEX ix_t_emitted_order_events02 ON t_emitted_order_events USING BTREE (processor_id, order_type) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_temporaryorders
*/ ------------------------------
ALTER TABLE t_temporaryorders ADD time_driven_start TIMESTAMP ;
ALTER TABLE t_temporaryorders ADD time_driven_stop TIMESTAMP ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.09.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.09.00',
		'2018-04-12',
		3,
		2,
		'PDS_12-9-0',
		'Created table t_emitted_order_events. Added time driven fields on t_temporaryorders'
) ;


