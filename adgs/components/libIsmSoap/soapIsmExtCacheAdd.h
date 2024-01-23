class Ism__cacheConfiguration
{
	public:
		std::string configurationKey;
		std::string provider;
		std::string key_ak;
		std::string key_sk;
		std::string region;
		std::string endpoint;
		bool endpointIsSecure;
		std::string options;
	
} ;

class Ism__cacheConfigurationAdditionalParms
{
	public:
		uint16_t downloadPoolSize ;
		uint64_t downloadBufferSize ;
		uint16_t uploadPoolSize ;
		uint64_t uploadBufferSize ;
} ;


class Ism__uploadSingleFileRequest
{
	public:
		Ism__fileDefinition *file ;
		bool recursive ;
		bool checkPermissions = true;
		Ism__cacheConfiguration *cacheConfiguration ;
} ;



class Ism__uploadResponse
{
	public:
		Ism__fileDefinition *file ;
		std::string uploadUrl ;
		Ism__cacheConfiguration *cacheConfiguration ;
} ;


Ism__getCacheConfiguration(char *in, Ism__cacheConfiguration *out) ;

Ism__getCacheConfigurationAdditionalParms(char *in, Ism__cacheConfigurationAdditionalParms *out) ;
