UPDATE t_repositories SET attributes='{"remote_timezone": "America/Phoenix"}' WHERE symbolicname='NOAA Public Datasets';
UPDATE t_repositories SET attributes='{"remote_timezone": "Europe/Paris"}' WHERE symbolicname='IFREMER Repository';
UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSL_VERIFYHOST": 0,"CURLOPT_SSL_VERIFYPEER": 0}}'
	WHERE symbolicname='S2GIPP Repository';
UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}'
	WHERE symbolicname='S1A FOS EXTRepo';
UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}'
	WHERE symbolicname='S1B FOS EXTRepo';
UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}'
	WHERE symbolicname='S2A FOS EXTRepo';
UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}'
	WHERE symbolicname='S2B FOS EXTRepo';
UPDATE t_repositories
	SET attributes='{"client_id": "S1A\\S1A_AuxDataGathering", "client_secret": "=\"H6;DAG75", "token_url": "https://prip.s1b.dlr.copernicus.eu/proseo/prip/v1/token", "oauth2_header": "Authorization: Bearer ", "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "PublicationDate gt 2022-01-01T00:00:00Z"}'
	WHERE symbolicname='S1APRIP DLR Rep';
UPDATE t_repositories
	SET attributes='{"client_id": "S1B\\S1B_AuxDataGathering", "client_secret": "=\"H6;DAG75", "token_url": "https://prip.s1b.dlr.copernicus.eu/proseo/prip/v1/token", "oauth2_header": "Authorization: Bearer ", "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "PublicationDate gt 2022-01-01T00:00:00Z"}'
	WHERE symbolicname='S1BPRIP DLR Rep';
