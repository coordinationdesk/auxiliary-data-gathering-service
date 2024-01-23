INSERT INTO t_storagemanager (type,name,description,invrelationtable,parameters)
VALUES ( 'ACSISM','ACS_ISM','Standard ISM','t_acsisminvrelation',
'<SMConfigurationRoot>
	<SMConfiguration>
		<!-- ISM Server URL -->
		<ServerUrl>pds-arf2.cryosat</ServerUrl>
		<!-- Server port -->
		<port>7627</port>
		<port2>7625</port2>
		<!-- User -->
		<user>ismmgr</user>
		<!-- password -->
		<password></password>
		<group>ismmgr</group>
		<permission>666</permission>
		<FtpUser>ismmgr</FtpUser>
		<FtpPassword>12qwas</FtpPassword>
		<Passive>true</Passive>
		<TapeRetrievalTimeOut units="sec">600</TapeRetrievalTimeOut>
		<ConnectionTimeOut units="msec">3000</ConnectionTimeOut>
		<ftpTotalTimeOut units="sec">20</ftpTotalTimeOut>
		<ftpRetryingInterval units="sec">4</ftpRetryingInterval>
		<uploadModality>ftp</uploadModality>
		<downloadModality>ftp</downloadModality>
		<deleteUploadedFileFromDisk>false</deleteUploadedFileFromDisk>
		<PackingStrategy>
			<name>Plain</name>
		</PackingStrategy>
		<Signature>
			<Algo>md5sum</Algo>
			<Compute>false</Compute>
			<Check>false</Check>
		</Signature>
	</SMConfiguration>
</SMConfigurationRoot>'); 
