INSERT INTO t_storagemanager (type,name,description,invrelationtable, parameters)
VALUES ( 'OBJSM','OBJ_SM','AWS S3 OBJ Storage Manager','t_objsminvrelation', 
'<SMConfigurationRoot>
	<SMConfiguration>
		<Provider>MINIO</Provider>
		<Key_ak>enrico_carta</Key_ak>
		<Key_sk>9hbP74va1rQG+2kn1AYb19D6V8nJEHlT7NWJjm+5</Key_sk>
		<!-- <Region></Region> -->
		<Endpoint>http://172.30.39.12:9000</Endpoint>
		<!-- <EndpointIsSecure>false</EndpointIsSecure> -->
		<!-- <Options></Options> -->
		<ConfigurationParametersList>
			<ConfigurationParameters>
				<RegExp>^.*zip</RegExp>
				<Bucket>bucket-test-perfo</Bucket>
				<ArchiveMode key="ECS-41">STATIC</ArchiveMode>
				<NamingAlgoKey>IDENTITY</NamingAlgoKey>
			</ConfigurationParameters>
		</ConfigurationParametersList>
		<!-- 
		<Signature>
			<Algo></Algo>
			<Compute>false</Compute>
			<Check>false</Check>
		</Signature>
		-->
		<!-- <RetryingInterval units="sec">5</RetryingInterval> -->
		<!-- <RetryingTimeout units="sec">10</RetryingTimeout> -->
		<!-- <PasswordEncryptionAlgo>0</PasswordEncryptionAlgo> -->
		<!-- <BucketPartitioning>NO_BKT_PART</BucketPartitioning> -->
		<!-- <HashtagComputationTimeout units="sec">600</HashtagComputationTimeout> -->
	</SMConfiguration>
</SMConfigurationRoot>'
); 
