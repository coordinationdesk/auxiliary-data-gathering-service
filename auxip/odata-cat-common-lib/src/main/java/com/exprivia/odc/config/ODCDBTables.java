package com.exprivia.odc.config;

import org.springframework.stereotype.Component;

import lombok.Getter;

@Component
@Getter
public class ODCDBTables {
	protected String inventory = "t_srv_inventory";
	
	protected String users = "t_srv_users";
	
	protected String userRoles = "t_srv_userroles";
	
	protected String usersXRoles = "t_srv_users_x_roles";
	
	protected String aipContent = "t_srv_aip_content";
	
	protected String aip = "t_srv_aip";
}
