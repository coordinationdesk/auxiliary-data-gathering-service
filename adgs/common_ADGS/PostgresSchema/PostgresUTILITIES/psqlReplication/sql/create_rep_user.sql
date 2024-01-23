DO
$body$
BEGIN
	IF NOT EXISTS (
		SELECT *
		FROM pg_catalog.pg_user
		WHERE usename = 'rep_usr') THEN
			CREATE USER rep_usr REPLICATION PASSWORD '12qwas';
	END IF;
END
$body$
