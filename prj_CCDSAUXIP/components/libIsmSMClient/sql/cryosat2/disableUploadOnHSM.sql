INSERT INTO t_storagepriority
SELECT (SELECT id FROM t_storagemanager WHERE name = 'ACS_HSM'), filetype, -1, 1, 1 FROM t_filetypes;
