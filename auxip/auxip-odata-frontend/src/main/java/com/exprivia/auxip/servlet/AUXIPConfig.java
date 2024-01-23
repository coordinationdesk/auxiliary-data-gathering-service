package com.exprivia.auxip.servlet;

import org.apache.olingo.server.api.ODataHttpHandler;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.edmprovider.ODCEdmFactory;
import com.exprivia.odc.metrics.service.permissions.ODCMetricsActions;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;
import com.exprivia.odc.service.permissions.ODCSystemRole;
import com.exprivia.odc.service.permissions.ODCSystemRoleList;
import com.exprivia.odc.service.permissions.ODCUserPermissions;
import com.exprivia.odc.servlet.ODCBaseServices;
import com.exprivia.odc.subscriptions.frontend.service.permissions.ODCSubscriptionsActions;
import com.exprivia.auxip.edmprovider.AUXIPEdmFactoryImpl;
import com.exprivia.auxip.service.permissions.AUXIPSystemRoles;

import lombok.Getter;

@Configuration
@PropertySource({ "classpath:auxip.properties", "classpath:auxip-${spring.profiles.active}.properties", "classpath:application.properties" })
@ComponentScan(value = "com.exprivia.odc")
@Getter
public class AUXIPConfig {
	@Value("${auxip.application.name:auxip-odata-frontend}")
	protected String application;

	@Value("${auxip.application.version:0.0}")
	protected String version;
	
	@Value("${auxip.application.icd-compiance:0.0}")
	protected String icdCompiance;
	
	@Value("${spring.datasource.url}")
	protected String springDatasourceUrl;
	
	@Value("${spring.datasource.username}")
	protected String springDatasourceUsername;
	
	@Value("${auxip.webclient.enabled}")
	protected boolean webClientEnabled;
	
	@Value("${auxip.download.location}")
	protected String downloadLocation;
	
	@Autowired
	protected ODCDBDefaults dbDefaults;
	
	@Autowired
	protected ODCBaseServices baseServices;
	
	@Bean
	public ODCEdmFactory odcEdmFactory() {
		// application-defined OData Edm schema
		return new AUXIPEdmFactoryImpl();
	}
	
	@Bean
	public ODCSystemRoleList systemRoles() {
		ODCSystemRoleList systemRoles = new ODCSystemRoleList();
		// initialize system roles
		systemRoles.addValue(AUXIPSystemRoles.DOWNLOAD);
		systemRoles.addValue(AUXIPSystemRoles.REPORTING);
		return systemRoles;
	}
	
	@Bean
	@Scope(value="request", proxyMode=ScopedProxyMode.TARGET_CLASS)
	public ODCUserPermissions permissions() {
		return new ODCUserPermissions();
	}
	
	@Bean
	public ODCActionsPermissions actionsPermissions() {
		ODCActionsPermissions actionPermissions = new ODCActionsPermissions();
		actionPermissions
			.add(ODCActionsPermissions.Action.PRODUCT_FIND_BY_ID.getCode(), 
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD, AUXIPSystemRoles.REPORTING})
			.add(ODCActionsPermissions.Action.PRODUCT_FIND_WITH_PARAMS.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD, AUXIPSystemRoles.REPORTING})
			
			.add(ODCActionsPermissions.Action.ROLE_FIND_BY_ID.getCode(), 
					new ODCSystemRole[] { AUXIPSystemRoles.REPORTING })
			.add(ODCActionsPermissions.Action.ROLE_FIND_WITH_PARAMS.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.REPORTING })
			
			.add(ODCActionsPermissions.Action.USER_FIND_BY_USERNAME.getCode(), 
					new ODCSystemRole[] { AUXIPSystemRoles.REPORTING })
			.add(ODCActionsPermissions.Action.USER_FIND_WITH_PARAMS.getCode(), 
					new ODCSystemRole[] { AUXIPSystemRoles.REPORTING })
			
			.add(ODCMetricsActions.METRIC_FIND_BY_ID.getCode(), 
					new ODCSystemRole[] { AUXIPSystemRoles.REPORTING })
			.add(ODCMetricsActions.METRIC_FIND_WITH_PARAMS.getCode(), 
					new ODCSystemRole[] { AUXIPSystemRoles.REPORTING })
			
			.add(ODCSubscriptionsActions.SUBSCRIPTION_INSERT.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD })
			.add(ODCSubscriptionsActions.SUBSCRIPTION_UPDATE.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD })
			.add(ODCSubscriptionsActions.SUBSCRIPTION_FIND_BY_ID.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD, AUXIPSystemRoles.REPORTING })
			.add(ODCSubscriptionsActions.SUBSCRIPTION_FIND_WITH_PARAMS.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD, AUXIPSystemRoles.REPORTING })
			.add(ODCSubscriptionsActions.NOTIFICATION_FIND_WITH_PARAMS.getCode(),
					new ODCSystemRole[] { AUXIPSystemRoles.DOWNLOAD, AUXIPSystemRoles.REPORTING })
			;
		
		return actionPermissions;
	}
	
    public ODataHttpHandler getODataHttpHandler() {
    	baseServices.registerDefaultProcessors();
    	return baseServices.getHttpHandler();
    }
}
