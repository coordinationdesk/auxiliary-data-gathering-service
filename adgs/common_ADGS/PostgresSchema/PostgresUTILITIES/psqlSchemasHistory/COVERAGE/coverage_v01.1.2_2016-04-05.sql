
-- Version 1.1.2
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------- PROCEDURE CHANGES ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	f_cov_percentage_calculator
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_percentage_calculator(
	in_period_id BIGINT
) RETURNS INTEGER AS $$

DECLARE
	cov_sum_interval INTEGER ;
	cov_ref_interval INTEGER ;
	
BEGIN
	SELECT EXTRACT(epoch FROM (exp_val_stop - exp_val_start)) INTO cov_ref_interval FROM t_cov_periods WHERE id_interval = in_period_id ;
	
	CREATE TEMP TABLE tmp_cov_val_to_calculate (
		rec_id BIGINT,
		group_id BIGINT,
		val_start TIMESTAMP, 
		val_stop TIMESTAMP) 
	;
		
	INSERT INTO tmp_cov_val_to_calculate (
		rec_id,
		group_id,
		val_start, 
		val_stop)
	SELECT 
		sub.r_id, 
		MIN(sub.g_id) AS grp,
		sub.inv_val_start,
		sub.inv_val_stop
	FROM (
		SELECT 
			b.id_relevancy AS r_id, 
			LEAST(a.id_relevancy, b.id_relevancy) AS g_id,
			b.inv_val_start,
			b.inv_val_stop
		FROM
			t_cov_relevancies a
			INNER JOIN t_cov_relevancies b
				ON a.period_id = b.period_id 
					AND b.id_relevancy >= a.id_relevancy
		WHERE
			(a.inv_val_start, a.inv_val_stop) OVERLAPS (b.inv_val_start, b.inv_val_stop)
			AND a.period_id = in_period_id
		) sub
	GROUP BY 
		sub.r_id,
		sub.inv_val_start,
		sub.inv_val_stop
	;
	
	SELECT 
		SUM(t.int_list) 
	INTO 
		cov_sum_interval 
	FROM (
		SELECT
			EXTRACT( epoch FROM MAX(val_stop) - MIN(val_start))::INTEGER AS int_list
		FROM
			tmp_cov_val_to_calculate
		GROUP BY
			group_id
		) t ;
	
	DROP TABLE tmp_cov_val_to_calculate ;
	
	RETURN cov_sum_interval * 100 / cov_ref_interval ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'coverage-1.1.2' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'coverage-1.1.2',
		'2016-04-05',
		6,
		25,
		'COVERAGE_1-1-2',
		'Fixed bug in f_cov_percentage_calculator on period selection'
) ;


