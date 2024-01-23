
-- Version 01.00.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
        t_stati
*/ ------------------------------
CREATE TABLE t_stati (
	id SMALLINT NOT NULL,
	status VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_stati
	ADD CONSTRAINT pk_t_stati
		PRIMARY KEY (id) ;

ALTER TABLE t_stati
	ADD CONSTRAINT uq_stati01
		UNIQUE (status) ;

INSERT INTO t_stati (id, status) VALUES (1,  'ABORTED') ;
INSERT INTO t_stati (id, status) VALUES (2,  'ACKNOWLEDGED') ;
INSERT INTO t_stati (id, status) VALUES (3,  'COMPLETED') ;
INSERT INTO t_stati (id, status) VALUES (4,  'DISTRIBUTION') ;
INSERT INTO t_stati (id, status) VALUES (5,  'DOWNLOADING') ;
INSERT INTO t_stati (id, status) VALUES (6,  'DOWNLOADITEM') ;
INSERT INTO t_stati (id, status) VALUES (7,  'ELIGIBLE') ;
INSERT INTO t_stati (id, status) VALUES (8,  'ERROR') ;
INSERT INTO t_stati (id, status) VALUES (9,  'FATALERROR') ;
INSERT INTO t_stati (id, status) VALUES (10, 'PRECONDITIONCHECK') ;
INSERT INTO t_stati (id, status) VALUES (11, 'PREPARING') ;
INSERT INTO t_stati (id, status) VALUES (12, 'PROCESSED') ;
INSERT INTO t_stati (id, status) VALUES (13, 'PROCESSING') ;
INSERT INTO t_stati (id, status) VALUES (14, 'PROGRESS') ;
INSERT INTO t_stati (id, status) VALUES (15, 'RECOVERABLEERROR') ;
INSERT INTO t_stati (id, status) VALUES (16, 'STANDBY') ;
INSERT INTO t_stati (id, status) VALUES (17, 'SUBMITTED') ;
INSERT INTO t_stati (id, status) VALUES (18, 'SUSPENDED') ;
INSERT INTO t_stati (id, status) VALUES (19, 'TRANSIENT') ;
INSERT INTO t_stati (id, status) VALUES (20, 'TRANSFERRING') ;
INSERT INTO t_stati (id, status) VALUES (21, 'UPLOADED') ;
INSERT INTO t_stati (id, status) VALUES (22, 'UPLOADING') ;
INSERT INTO t_stati (id, status) VALUES (23, 'UPLOADITEM') ;
INSERT INTO t_stati (id, status) VALUES (25, 'DELETED') ;
INSERT INTO t_stati (id, status) VALUES (26, 'DELETING') ;
INSERT INTO t_stati (id, status) VALUES (27, 'MEDIA_ACKNOWLEDGE') ;
INSERT INTO t_stati (id, status) VALUES (28, 'MEDIA_WAIT') ;
INSERT INTO t_stati (id, status) VALUES (29, 'NOTFOUNDERROR') ;
INSERT INTO t_stati (id, status) VALUES (30, 'INVALID') ;
INSERT INTO t_stati (id, status) VALUES (31, 'LTA_DOWNLOADING') ;
INSERT INTO t_stati (id, status) VALUES (32, 'WAITINGACK') ;
INSERT INTO t_stati (id, status) VALUES (33, 'REQUESTED') ;
INSERT INTO t_stati (id, status) VALUES (34, 'PLANNED') ;
INSERT INTO t_stati (id, status) VALUES (35, 'RE-PLANNED') ;
INSERT INTO t_stati (id, status) VALUES (36, 'EXECUTED') ;
INSERT INTO t_stati (id, status) VALUES (37, 'ABANDONED') ;
INSERT INTO t_stati (id, status) VALUES (38, 'LTA_PROCESSING') ;
INSERT INTO t_stati (id, status) VALUES (39, 'PARTIALLY_TRANSFERRED') ;
INSERT INTO t_stati (id, status) VALUES (40, 'TO_DELETE') ;
INSERT INTO t_stati (id, status) VALUES (41, 'BLOCKED') ;
INSERT INTO t_stati (id, status) VALUES (42, 'DISABLED') ;
INSERT INTO t_stati (id, status) VALUES (43, 'ENABLED') ;
INSERT INTO t_stati (id, status) VALUES (44, 'BLOCKING') ;
INSERT INTO t_stati (id, status) VALUES (45, 'ONLINE') ;
INSERT INTO t_stati (id, status) VALUES (46, 'ONGOING') ;
INSERT INTO t_stati (id, status) VALUES (47, 'EVICTED') ;
INSERT INTO t_stati (id, status) VALUES (48, 'BOOKED') ;
INSERT INTO t_stati (id, status) VALUES (49, 'EXPIRED') ;






-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------
DO $$
BEGIN
	IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 't_schema')
	THEN
		DELETE FROM t_schema WHERE version = 'common-02.00.00' ;

		INSERT INTO t_schema (
				version, 
				modificationdate,
				modificationtype,
				schematype,
				cvstag,
				description )
		VALUES	(
				'common-02.00.00',
				'2021-06-16',
				2,
				40,
				'COMMON_2-0-0',
				'Moved here PDS table t_stati'
		) ;
	END IF ;
END $$;


