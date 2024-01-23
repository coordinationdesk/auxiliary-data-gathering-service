
-- Version 11.5.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inv_distributions
*/ --------------------------
CREATE TABLE t_inv_distributions (
    inv_id BIGINT NOT NULL,
	retain_counter INTEGER NOT NULL,
    last_update TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_inv_distributions
	ADD CONSTRAINT pk_t_inv_distributions
		PRIMARY KEY (inv_id) ;

ALTER TABLE t_inv_distributions 
	ADD CONSTRAINT fk_inv_distributions01 
		FOREIGN KEY (inv_id) 
			REFERENCES t_inventory (id_inv) 
				ON DELETE CASCADE ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_smactions
*/ --------------------------
ALTER TABLE t_smactions ADD inv_id BIGINT ;
UPDATE t_smactions SET inv_id = -1 WHERE 1 = 1 ;
UPDATE t_smactions SET inv_id = subq.id_inv FROM (
	SELECT id_inv, filename, fileversion FROM t_inventory) AS subq
		WHERE t_smactions.filename = subq.filename AND t_smactions.fileversion = subq.fileversion ;
ALTER TABLE t_smactions ALTER COLUMN inv_id SET NOT NULL ;


/* ------------------------------
	t_originators
*/ ------------------------------
ALTER TABLE t_inventory DROP CONSTRAINT fk_inventory09 ;
ALTER TABLE t_distruleoriginator DROP CONSTRAINT fk_t_distruleoriginator2 ;

ALTER TABLE t_originators DROP CONSTRAINT pk_t_originators;
ALTER TABLE t_originators ADD id2 INT2 ;
UPDATE t_originators SET id2 = id ;
ALTER TABLE t_originators ALTER COLUMN id2 SET NOT NULL ;
ALTER TABLE t_originators DROP COLUMN id ;
ALTER TABLE t_originators RENAME COLUMN id2 TO id ;
ALTER TABLE t_originators
	ADD CONSTRAINT pk_t_originators
		PRIMARY KEY (id) ;

ALTER TABLE t_inventory 
	ADD CONSTRAINT fk_inventory9 
		FOREIGN KEY (originator)
			REFERENCES t_originators (id) ;
ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT fk_t_distruleoriginator2
		FOREIGN KEY (originator_id)
			REFERENCES t_originators (id)
				ON DELETE CASCADE ;

INSERT INTO t_originators ("id","name") VALUES (41,'GSV Simulation');
INSERT INTO t_originators ("id","name") VALUES (42,'QCC');
INSERT INTO t_originators ("id","name") VALUES (43,'UKMO');


/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories DROP CONSTRAINT ck_repositories03 ;
ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories03
		CHECK ((tempname_id = 3 AND (tempname_value = '.' OR tempname_value = './')) != TRUE) ;


/* ------------------------------
	t_satellites
*/ ------------------------------
DELETE FROM t_satrelations WHERE satelliteid BETWEEN 34 AND 39 ;

UPDATE t_satellites SET satellitename = 'Sentinel-1', satelliteacronym = 'S1' WHERE satellitename = 'Sentinel-1A' ;
UPDATE t_satellites SET satellitename = 'Sentinel-1A', satelliteacronym = 'S1A' WHERE satellitename = 'Sentinel-1B' ;
UPDATE t_satellites SET satellitename = 'Sentinel-1B', satelliteacronym = 'S1B' WHERE satellitename = 'Sentinel-2A' ;
UPDATE t_satellites SET satellitename = 'Sentinel-2', satelliteacronym = 'S2_' WHERE satellitename = 'Sentinel-2B' ;
UPDATE t_satellites SET satellitename = 'Sentinel-2A', satelliteacronym = 'S2A' WHERE satellitename = 'Sentinel-3A' ;
UPDATE t_satellites SET satellitename = 'Sentinel-2B', satelliteacronym = 'S2B' WHERE satellitename = 'Sentinel-3B' ;

INSERT INTO t_satellites VALUES (40, 'Sentinel-3', 'S3') ;
INSERT INTO t_satellites VALUES (41, 'Sentinel-3A', 'S3A') ;
INSERT INTO t_satellites VALUES (42, 'Sentinel-3B', 'S3B') ;

----- SENTINEL -----
-- Sentinel-1 1
INSERT INTO t_satrelations VALUES (34, 1, 18) ;
-- Sentinel-1 2
INSERT INTO t_satrelations VALUES (34, 2, 18) ;
-- Sentinel-1A 1
INSERT INTO t_satrelations VALUES (35, 1, 18) ;
-- Sentinel-1B 1
INSERT INTO t_satrelations VALUES (36, 1, 18) ;
-- Sentinel-2 1
INSERT INTO t_satrelations VALUES (37, 1, 66) ;
-- Sentinel-2 2
INSERT INTO t_satrelations VALUES (37, 2, 66) ;
-- Sentinel-2A 1
INSERT INTO t_satrelations VALUES (38, 1, 66) ;
-- Sentinel-2B 1
INSERT INTO t_satrelations VALUES (39, 1, 66) ;
-- Sentinel-3 1
INSERT INTO t_satrelations VALUES (40, 1, 2) ;
INSERT INTO t_satrelations VALUES (40, 1, 20) ;
INSERT INTO t_satrelations VALUES (40, 1, 67) ;
INSERT INTO t_satrelations VALUES (40, 1, 68) ;
INSERT INTO t_satrelations VALUES (40, 1, 69) ;
INSERT INTO t_satrelations VALUES (40, 1, 70) ;
-- Sentinel-3 2
INSERT INTO t_satrelations VALUES (40, 2, 2) ;
INSERT INTO t_satrelations VALUES (40, 2, 20) ;
INSERT INTO t_satrelations VALUES (40, 2, 67) ;
INSERT INTO t_satrelations VALUES (40, 2, 68) ;
INSERT INTO t_satrelations VALUES (40, 2, 69) ;
INSERT INTO t_satrelations VALUES (40, 2, 70) ;
-- Sentinel-3A 1
INSERT INTO t_satrelations VALUES (41, 1, 2) ;
INSERT INTO t_satrelations VALUES (41, 1, 20) ;
INSERT INTO t_satrelations VALUES (41, 1, 67) ;
INSERT INTO t_satrelations VALUES (41, 1, 68) ;
INSERT INTO t_satrelations VALUES (41, 1, 69) ;
INSERT INTO t_satrelations VALUES (41, 1, 70) ;
-- Sentinel-3B 1
INSERT INTO t_satrelations VALUES (42, 1, 2) ;
INSERT INTO t_satrelations VALUES (42, 1, 20) ;
INSERT INTO t_satrelations VALUES (42, 1, 67) ;
INSERT INTO t_satrelations VALUES (42, 1, 68) ;
INSERT INTO t_satrelations VALUES (42, 1, 69) ;
INSERT INTO t_satrelations VALUES (42, 1, 70) ;

ALTER TABLE t_inventory DISABLE TRIGGER tr_inventoryupdate ;
UPDATE t_inventory SET satelliteid = 42 WHERE satelliteid = 39 ;
UPDATE t_inventory SET satelliteid = 41 WHERE satelliteid = 38 ;
UPDATE t_inventory SET satelliteid = 39 WHERE satelliteid = 37 ;
UPDATE t_inventory SET satelliteid = 38 WHERE satelliteid = 36 ;
UPDATE t_inventory SET satelliteid = 36 WHERE satelliteid = 35 ;
UPDATE t_inventory SET satelliteid = 35 WHERE satelliteid = 34 ;
ALTER TABLE t_inventory ENABLE TRIGGER tr_inventoryupdate ;

ALTER TABLE t_ordersqueue DISABLE TRIGGER tr_ordersqueueupdate ;
UPDATE t_ordersqueue SET satelliteid = 42 WHERE satelliteid = 39 ;
UPDATE t_ordersqueue SET satelliteid = 41 WHERE satelliteid = 38 ;
UPDATE t_ordersqueue SET satelliteid = 39 WHERE satelliteid = 37 ;
UPDATE t_ordersqueue SET satelliteid = 38 WHERE satelliteid = 36 ;
UPDATE t_ordersqueue SET satelliteid = 36 WHERE satelliteid = 35 ;
UPDATE t_ordersqueue SET satelliteid = 35 WHERE satelliteid = 34 ;
ALTER TABLE t_ordersqueue ENABLE TRIGGER tr_ordersqueueupdate ;

ALTER TABLE t_temporaryorders DISABLE TRIGGER tr_temporaryordersupdate ;
UPDATE t_temporaryorders SET ordersat = 42 WHERE ordersat = 39 ;
UPDATE t_temporaryorders SET ordersat = 41 WHERE ordersat = 38 ;
UPDATE t_temporaryorders SET ordersat = 39 WHERE ordersat = 37 ;
UPDATE t_temporaryorders SET ordersat = 38 WHERE ordersat = 36 ;
UPDATE t_temporaryorders SET ordersat = 36 WHERE ordersat = 35 ;
UPDATE t_temporaryorders SET ordersat = 35 WHERE ordersat = 34 ;
ALTER TABLE t_temporaryorders ENABLE TRIGGER tr_temporaryordersupdate ;

ALTER TABLE t_iorepository DISABLE TRIGGER tr_iorepositoryupdate ;
UPDATE t_iorepository SET satelliteid = 42 WHERE satelliteid = 39 ;
UPDATE t_iorepository SET satelliteid = 41 WHERE satelliteid = 38 ;
UPDATE t_iorepository SET satelliteid = 39 WHERE satelliteid = 37 ;
UPDATE t_iorepository SET satelliteid = 38 WHERE satelliteid = 36 ;
UPDATE t_iorepository SET satelliteid = 36 WHERE satelliteid = 35 ;
UPDATE t_iorepository SET satelliteid = 35 WHERE satelliteid = 34 ;
ALTER TABLE t_iorepository ENABLE TRIGGER tr_iorepositoryupdate ;

UPDATE t_filetypes_x_satellites SET satellite_id = 42 WHERE satellite_id = 39 ;
UPDATE t_filetypes_x_satellites SET satellite_id = 41 WHERE satellite_id = 38 ;
UPDATE t_filetypes_x_satellites SET satellite_id = 39 WHERE satellite_id = 37 ;
UPDATE t_filetypes_x_satellites SET satellite_id = 38 WHERE satellite_id = 36 ;
UPDATE t_filetypes_x_satellites SET satellite_id = 36 WHERE satellite_id = 35 ;
UPDATE t_filetypes_x_satellites SET satellite_id = 35 WHERE satellite_id = 34 ;


/* ------------------------------
	t_rollingpolicyactions
*/ ------------------------------
INSERT INTO t_rollingpolicyactions VALUES (5, 'INVALIDATEPARENT-REMOVECHILDREN') ;


/* ------------------------------
	t_rollingpoliciesalgo
*/ ------------------------------
INSERT INTO t_rollingpoliciesalgo VALUES (4, 'ParametersBased') ;


/* ------------------------------
	t_anomaly_types
*/ ------------------------------
UPDATE t_anomaly_types SET anomaly_type = 'FAULT' WHERE anomaly_type = 'CORRUPTION' ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_inv_dist_counter_by_items 
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inv_dist_counter_by_items(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	operation CHAR(3)) RETURNS INTEGER AS $$
	
DECLARE
	p_invid BIGINT ;
	
BEGIN
	-- Check operation input value validity
	IF operation NOT IN ('INS','DEL') THEN
		RAISE EXCEPTION 'p_inv_distributions_by_items: Invalid operation input value (INS/DEL).' ;
		RETURN 1 ;
	END IF ;
	
	-- Get the inventory ID
	SELECT id_inv INTO p_invid FROM t_inventory WHERE filename = in_fname AND fileversion = in_fversion ;
	
	IF operation = 'DEL' THEN
		UPDATE t_inv_distributions SET retain_counter = (retain_counter - 1) WHERE inv_id = p_invid AND retain_counter > 0 ;
	END IF ;
	
	IF operation = 'INS' THEN
		IF (SELECT count(*) FROM t_inv_distributions WHERE inv_id = p_invid) > 0 THEN
			UPDATE t_inv_distributions SET retain_counter = (retain_counter + 1) WHERE inv_id = p_invid ;
		ELSE
			INSERT INTO t_inv_distributions (inv_id, retain_counter) VALUES (p_invid, 1) ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_inv_dist_counter_by_queue 
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inv_dist_counter_by_queue(
	in_packid INTEGER,
	in_pre_status VARCHAR(32),
	in_post_status VARCHAR(32),
	operation CHAR(3)) RETURNS INTEGER AS $$
	
BEGIN
	-- Check operation input value validity
	IF operation NOT IN ('UPD','DEL') THEN
		RAISE EXCEPTION 'p_inv_distributions_by_queue: Invalid operation input value (UPD/DEL).' ;
		RETURN 1 ;
	END IF ;
	
	IF operation = 'DEL' OR (operation = 'UPD' AND (in_pre_status != in_post_status AND in_post_status = 'COMPLETED')) THEN
		UPDATE 
			t_inv_distributions 
		SET 
			retain_counter = (retain_counter - 1) 
		WHERE 
			retain_counter > 0
			AND inv_id IN (
		SELECT
			iv.id_inv
		FROM
			t_inventory iv
			INNER JOIN t_distributionitems di
				ON iv.filename = di.filename AND iv.fileversion = di.fileversion
			INNER JOIN t_distributionqueue dq
				ON dq.packageid = di.packageid
		WHERE
			dq.packageid = in_packid );
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_invdistributionsupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_invdistributionsupdate() RETURNS TRIGGER AS $$

BEGIN
	NEW.last_update := now()::TIMESTAMP ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_distributionitems_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionitems_manager() RETURNS TRIGGER AS $$

DECLARE
  	res INTEGER ;
	
BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		res := p_inv_dist_counter_by_items(NEW.filename, NEW.fileversion, 'INS') ;
		PERFORM p_events('t_distributionitems', 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_events('t_distributionitems', 'UPD') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		res := p_inv_dist_counter_by_items(OLD.filename, OLD.fileversion, 'DEL') ;
		PERFORM p_events('t_distributionitems', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_distributionqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_manager() RETURNS TRIGGER AS $$

DECLARE
  	res INTEGER ;
	
BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_events('t_distributionqueue', 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		res := p_distribution_manager(OLD.packageid, OLD.status, NEW.status) ;
		res := p_inv_dist_counter_by_queue(NEW.packageid, OLD.status, NEW.status, 'UPD') ;
		PERFORM p_events('t_distributionqueue', 'UPD') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		res := p_inv_dist_counter_by_queue(OLD.packageid, OLD.status, NULL, 'DEL') ;
		PERFORM p_events('t_distributionqueue', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inv_distributions
*/ ------------------------------
CREATE TRIGGER tr_invdistributionsupdate BEFORE UPDATE
	ON t_inv_distributions
		FOR EACH ROW EXECUTE PROCEDURE tf_invdistributionsupdate() ;








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATIONS ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionitems
*/ ------------------------------
DROP TRIGGER tr_distributionitemsinsert ON t_distributionitems ;
DROP TRIGGER tr_distributionitemsupdate ON t_distributionitems ;

CREATE TRIGGER tr_distributionitemsinsert AFTER INSERT
	ON t_distributionitems 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionitems_manager('INS' ) ;

CREATE TRIGGER tr_distributionitemsupdate AFTER UPDATE
	ON t_distributionitems 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionitems_manager('UPD' ) ;

CREATE TRIGGER tr_distributionitemsdelete AFTER DELETE
	ON t_distributionitems 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionitems_manager('DEL' ) ;


/* ------------------------------
	t_distributionqueue
*/ ------------------------------
DROP TRIGGER tr_distributionqueueinsert2 ON t_distributionqueue ;
DROP TRIGGER tr_distributionqueueupdate2 ON t_distributionqueue ;

CREATE TRIGGER tr_distributionqueueinsert2 AFTER INSERT
	ON t_distributionqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager('INS');

CREATE TRIGGER tr_distributionqueueupdate2 AFTER UPDATE
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager('UPD');

CREATE TRIGGER tr_distributionqueuedelete1 AFTER DELETE
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager('DEL');








---------------------------------------------------------------------------------
---------------------------- TRIGGERS FUNCTIONS DROP ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_upd_p_distributionstate
*/ ------------------------------
DROP FUNCTION tf_upd_p_distributionstate() ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.5.0',
		'2013-09-16',
		3,
		2,
		'PDS_11-5-0',
		'Created table t_inv_distributions and modified t_smactions and t_originators. Created p_inv_dist_counter_by_queue and p_inv_dist_counter_by_items procedures. Changed triggers for t_distributionitems and t_distributionqueue.'
) ;


