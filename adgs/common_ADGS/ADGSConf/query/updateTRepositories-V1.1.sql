UPDATE t_repositories SET host_id = (SELECT id FROM t_knownhosts WHERE host_name = 'adgs-dns.adgs.int') WHERE symbolicname='S1 FOS Repository';
UPDATE t_repositories SET host_id = (SELECT id FROM t_knownhosts WHERE host_name = 'adgs-dns.adgs.int') WHERE symbolicname='S2 FOS Repository';
