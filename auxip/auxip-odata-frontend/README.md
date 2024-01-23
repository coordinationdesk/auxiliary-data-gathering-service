# auxip-odata-frontend

Provides a stand-alone service to serve OData http requests for the AUXIP Catalogue query and products download.  
Responses are always in Json format.

## Dependencies

	odata-cat-common-lib


## Configuration

Main configuration parameters.  

	spring.datasource.url=jdbc:postgresql://localhost:5432/srvdb  
	spring.datasource.username=srvuser  
	spring.datasource.password=12qwas  
	
	odata-cat.query.limit=1000  
	odata-cat.eviction-time-postpone-interval=2 DAY  
	
	auxip.webclient.enabled=false  
	auxip.download.location=/download


## Service control

The service is under the systemd control, and can be controlled as follows:

	systemctl start auxip-odata-frontend
	systemctl stop auxip-odata-frontend
	systemctl restart auxip-odata-frontend
	systemctl status auxip-odata-frontend

## Logs

The logs are managed by the systemd standard logging facility journald, and can be explored by the `journalctl` command

	journalctl -xf --unit=auxip-odata-frontend
	journalctl -xe --unit=auxip-odata-frontend


## ICD 1.2 Requests

### Products

*3.3.1.1 Query by Name (startswith)*

	{{service-root-uri}}/odata/v1/Products?$filter=startswith(Name,'S1A_AUX_')

*3.3.1.1 Query by Name (contains)*

	{{service-root-uri}}/odata/v1/Products?$filter=contains(Name,'S2__OPER_AUX_UT1UTC')

*3.3.1.2 Query by Product Publication Date*

	{{service-root-uri}}/odata/v1/Products?$filter=PublicationDate gt 2020-05-15T00:00:00.000Z

*3.3.1.3 Query by Validity Date*

	{{service-root-uri}}/odata/v1/Products?$filter=ContentDate/Start gt 2019-05-15T00:00:00.000Z and ContentDate/End lt 2019-05-16T00:00:00.000Z

*3.3.1.4 Query by Attributes*

	{{service-root-uri}}/odata/v1/Products?$filter=Attributes/OData.CSC.StringAttribute/any(att:att/Name 'productType' and att/OData.CSC.StringAttribute/Value eq 'AUX_ECMWFD') and Attributes/OData.CSC.StringAttribute/any(att:att/Name eq 'platformShortName' and att/OData.CSC.StringAttribute/Value eq 'SENTINEL-2')

*3.3.1.5 Additional Options ($orderby)*

	{{service-root-uri}}/odata/v1/Products?$orderby=PublicationDate desc

*3.3.1.5 Additional Options ($top)*
	
	{{service-root-uri}}/odata/v1/Products?$top=1000&$filter=startswith(Name,'S2')

*3.3.1.5 Additional Options ($skip)*

	{{service-root-uri}}/odata/v1/Products?$skip=100&$filter=startswith(Name,'S2')

*3.3.1.5 Additional Options ($count)*

	{{service-root-uri}}/odata/v1/Products?$count=true&$filter=startswith(Name,'S2')

*3.3.1.5 Additional Options (Operator "and")*

	{{service-root-uri}}/odata/v1/Products?$filter=startswith(Name,'S1') and endswith(Name,'.EOF.zip')

*3.3.1.5 Additional Options (Operator "or")*

	{{service-root-uri}}/odata/v1/Products?$filter=contains(Name,'AMH_ERRMAT') or contains(Name,'AMV_ERRMAT')

*3.3.1.5 Additional Options (Operator "not")*

	{{service-root-uri}}/odata/v1/Products?$filter=startswith(Name,'S1') and not contains(Name,'AUX_CAL')

*3.3.1.5 Additional Options (Operator "in")*

	{{service-root-uri}}/odata/v1/Products?$filter=Attributes/OData.CSC.StringAttribute/any(att:att/Name eq 'productType' and att/OData.CSC.StringAttribute/Value in ('AUX_WAV','AUX_ICE','AUX_WND'))

*3.3.3 Catalogue Export*

	{{service-root-uri}}/odata/v1/Products?$expand=Attributes&$format=json

*3.4 Product Download*

	{{service-root-uri}}/odata/v1/Products({{Id}})/$value

*3.5 Subscription Create*

	POST
	{{service-root-uri}}/odata/v1/Subscriptions
	{
	"@odata.context": "$metadata#Subscription/$entity",
	"FilterParam": "Products?$filter=contains(Name,'_AUX_ECMWFD_')and
	PublicationDate gt 2019-02-01T00:00:00.000Z and PublicationDate lt 2019-09-
	01T00:00:00.000Z",
	"NotificationEndpoint": "https://myservice/notification",
	"NotificationEpUsername": "myserviceuser",
	"NotificationEpPassword": "**********",
	}

*5.1.1 Reporting*

	{{service-root-uri}}/odata/v1/Products?$count=true&$filter=PublicationDate gt 2019-01-17T00:00:00.000Z and PublicationDate lt 2019-01-18T00:00:00.000Z&$select=Name,ContentLength,PublicationDate,ContentDate/Start

