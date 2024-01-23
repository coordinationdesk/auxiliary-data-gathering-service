UPDATE t_repositories
	SET remotepath='/pub/gps/products/ionex/%Y/%j:range(now,-20,-5)'
	WHERE symbolicname='AGS TEC Repository';

UPDATE t_repositories
	SET remotepath='/pub/gps/products/%{gpsWeek}:range(now,-5,0)'
	WHERE symbolicname='AGS TEC Rep Backup';

UPDATE t_repositories
	SET remotepath='/%Y%m%d/:range(now,-1,0)'
	WHERE symbolicname='ECMWF TRO Repository';

UPDATE t_repositories
	SET remotepath='/reprocessed/ice/conc-cont-reproc/v3p0/%Y/%m:offset(now,-2)'
	WHERE symbolicname='S3 OSI Repository';

UPDATE t_repositories
	SET remotepath='/reprocessed/ice/conc-cont-reproc/v3p0/%Y/%m:offset(now,-16)'
	WHERE symbolicname='S3 OSI ReproRep';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_TCP_KEEPALIVE": 1, "CURLOPT_TCP_KEEPIDLE": 60, "CURLOPT_TCP_KEEPINTVL": 60}}'
	WHERE symbolicname='ECMWF TRO Repository';

UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "mpcip", "client_secret": "", "token_url": "https://auth.sar-mpc.eu/auth/realms/sarmpc/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "omit_format": true, "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "PublicationDate gt 2022-01-01T00:00:00Z"}'
	WHERE symbolicname='S1MCP Repository';

UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "s1pdgs_client", "client_secret": "9680267b-c068-4e79-a690-368c1d8a5cae", "token_url": "https://c1.ops.s1pdgs.eu/auth/realms/s1pdgs/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "PublicationDate gt 2022-02-15T00:00:00Z"}'
	WHERE symbolicname='S1PRIP Repository';

UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "auxip-client", "client_secret": "ee1648d5-7967-4ba7-a7d8-8469633afd26", "token_url": "https://cpodopsftp.gmv.com:8082/auth/realms/AUXIP/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "PublicationDate gt 2022-01-01T00:00:00Z"}'
	WHERE symbolicname='POD Repository';

UPDATE t_repositories
	SET remotepath='/pub/DATASETS/NOAA/G02156/GIS/1km/%Y/:range(now,-5,0)'
	WHERE symbolicname='NOAA Public Datasets';

UPDATE t_repositories SET attributes='{"remote_timezone": "America/Phoenix"}' WHERE symbolicname='NOAA Public Datasets';

UPDATE t_repositories SET attributes='{"remote_timezone": "Europe/Paris"}' WHERE symbolicname='IFREMER Repository';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSL_VERIFYHOST": 0,"CURLOPT_SSL_VERIFYPEER": 0}}'
	WHERE symbolicname='S2GIPP Repository';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_PROXY": "adgs-dns.adgs.int:3128", "CURLOPT_VERBOSE": 1}, "odata_filter": "(contains(Name,''GIP'') or contains(Name,''CAMS'') or contains(Name,''UT1UTC'') or contains(Name,''ECMWFD'')) and PublicationDate gt 2021-12-01T00:00:00Z"}'
	WHERE symbolicname='S2PRIP Repository';
	
UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "mpc-user-public", "client_secret": "", "token_url": "https://sso.adwaiseo-services.com/auth/realms/MPC/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "omit_format": true, "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "startswith(Name,''S2'') and PublicationDate gt 2022-01-01T00:00:00Z"}'
	WHERE symbolicname='S2MCP Repository';

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

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}',
	    port=8022
	WHERE symbolicname='S2A FOS NewEXTRepo';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}',
	    port=8022
	WHERE symbolicname='S2A FOS NewEXTRepo-2';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}',
	    port=8022
	WHERE symbolicname='S2B FOS NewEXTRepo';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_SSLKEY": "/home/opadgs/.ssh/id_rsa"}}',
	    port=8022
	WHERE symbolicname='S2B FOS NewEXTRepo-2';

UPDATE t_repositories
	SET attributes='{"curl_options": {"CURLOPT_PROXY": "adgs-dns.adgs.int:3128", "CURLOPT_VERBOSE": 1}, "odata_filter": "PublicationDate gt 2023-01-01T00:00:00Z"}'
	WHERE symbolicname='S1APRIP Serco';

UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "mpip-api", "client_secret": "3ufepo4rIriPU1IPZkxLMm4F8sVZRFCW", "token_url": "https://keycloak.s12mpip.com/realms/mpip/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}}'
	WHERE symbolicname='MP-IP Repository';
