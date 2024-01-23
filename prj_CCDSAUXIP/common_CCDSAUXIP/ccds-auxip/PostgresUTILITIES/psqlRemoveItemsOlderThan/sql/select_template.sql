SELECT
	DATE_TRUNC('day', <FLD_NAME>),
	count(*)
FROM
	<TBL_NAME>
WHERE
	<FLD_NAME> < (now() - interval '<DAY_NUM>' day)
GROUP BY
	1
ORDER BY
	1 ASC
;
