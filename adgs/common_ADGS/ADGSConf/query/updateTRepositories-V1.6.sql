UPDATE t_repositories
	SET attributes='{"authentication_type": "OAuth2", "client_id": "mpip-api", "client_secret": "3ufepo4rIriPU1IPZkxLMm4F8sVZRFCW", "token_url": "https://keycloak.s12mpip.com/realms/mpip/protocol/openid-connect/token", "oauth2_header": "Authorization: Bearer ", "curl_options": {"CURLOPT_VERBOSE": 1, "CURLOPT_PROXY": "adgs-dns.adgs.int:3128"}}'
	WHERE symbolicname='MP-IP Repository';
