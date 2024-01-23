UPDATE t_repositories
	SET remotepath='/%Y%m%d/:range(now,-1,0)'
	WHERE symbolicname='ECMWF TRO Repository';

UPDATE t_repositories
	SET remotepath='/reprocessed/ice/conc-cont-reproc/v3p0/%Y/%m:offset(now,-2)'
	WHERE symbolicname='S3 OSI Repository';
