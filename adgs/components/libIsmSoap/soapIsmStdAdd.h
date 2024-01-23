class Ism__uploadSingleFileRequest
{
	public:
		Ism__fileDefinition *file ;
		bool recursive ;
		bool checkPermissions = true;
} ;



class Ism__uploadResponse
{
	public:
		Ism__fileDefinition *file ;
		std::string uploadUrl ;
} ;

