UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "mpc-user-public", "client_secret": "", "token_url": "https://sso.adwaiseo-services.com/auth/realms/MPC/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "omit_format": true, "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}, "odata_filter": "startswith(Name,''S2'') and PublicationDate gt 2022-01-01T00:00:00Z"}'
	WHERE symbolicname='S2MCP Repository';
