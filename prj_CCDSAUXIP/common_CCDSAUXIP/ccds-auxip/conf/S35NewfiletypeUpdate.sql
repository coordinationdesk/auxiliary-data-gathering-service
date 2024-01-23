-- start a transaction
BEGIN;

INSERT INTO t_satellites (satelliteid,satelliteacronym,satellitename) VALUES
	 (46,'S5P','Sentinel-5P');

INSERT INTO t_filetypes (filetype,description,nameregularexpression,headerseparation,"group",invperiod,mngsngext,sngext,hdrext,dblext) VALUES
	 ('S5_VIRTUAL','S3__VIRTUAL LTA filetype',NULL,false,'group0',0,false,NULL,NULL,NULL);

INSERT INTO public.t_filetypes
(filetype, description, nameregularexpression, headerseparation, "group", invperiod, mngsngext, sngext, hdrext, dblext)
VALUES('S3_REP__SUP', 'S3_REP__SUP auxiliary file', '^S3[ABCP_]_.*_REP__SUP___[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\.', false, 'group0', 0, false, NULL, NULL, NULL);

INSERT INTO public.t_filetypes
(filetype, description, nameregularexpression, headerseparation, "group", invperiod, mngsngext, sngext, hdrext, dblext)
VALUES('S5_REP__SUP', 'S3_REP__SUP auxiliary file', '^S5[ABCP_]_.*_REP__SUP___[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\.EOF', false, 'group0', 0, false, NULL, NULL, NULL);

INSERT INTO t_filetypescomps (pfiletype,cfiletype) VALUES
	 ((select id_filetype from t_filetypes where filetype = 'S3_VIRTUAL'),(select id_filetype from t_filetypes where filetype = 'S3_REP__SUP'));

INSERT INTO t_filetypescomps (pfiletype,cfiletype) VALUES
	 ((select id_filetype from t_filetypes where filetype = 'S5_VIRTUAL'),(select id_filetype from t_filetypes where filetype = 'S5_REP__SUP'));

INSERT INTO t_srv_receptionrules (filetype_id,repository_id,priority,active,generateheader,reception_tt_id,tt_parameters,hostname,localpath,connretries,pollingperiod_sec,retryperiod_sec,connfailureaddress,procfailureaddress,lastvisit,lastgood,retrycounter,discoveryplugin_id,successnotificationaddress,php_timeout_sec,timewindow_sec) VALUES
	 ((select id_filetype from t_filetypes where filetype = 'S5_VIRTUAL'),1,1,true,false,1,NULL,'AUXIP','/data/auxipin/AUX_IN_BASKET',3,10,1,'','','2021-12-20 21:58:05.698','2021-12-20 21:58:05.698',0,1,NULL,NULL,NULL);

-- commit the change (or roll it back later)
COMMIT;