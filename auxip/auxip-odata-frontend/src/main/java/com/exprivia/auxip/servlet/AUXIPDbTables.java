package com.exprivia.auxip.servlet;

import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;

import com.exprivia.odc.config.ODCDBTables;

import lombok.Getter;

@Primary
@Component
@Getter
public class AUXIPDbTables extends ODCDBTables {
	protected String inventory = "t_srv_inventory";
	
	protected String inventoryDwh = "t_srv_inventory_dwh";
	
	protected String users = "t_srv_users";
	
	protected String userRoles = "t_srv_userroles";
	
	protected String usersXRoles = "t_srv_users_x_roles";
	
	protected String aipContent = "t_srv_aip_content";
	
	protected String aip = "t_srv_aip";
	
	protected boolean useDWH = false;
	
	public void setDWH(boolean dwh) {
		useDWH = dwh;
	}
	
	public String getInventory() {
		if (!useDWH) {
			return inventory;
		} else {
			return inventoryDwh;
		}
	}
}
