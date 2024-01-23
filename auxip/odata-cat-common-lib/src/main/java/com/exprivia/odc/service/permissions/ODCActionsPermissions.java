package com.exprivia.odc.service.permissions;

import java.util.HashMap;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;

public class ODCActionsPermissions {
	
	static public enum Action {
		PRODUCT_FIND_BY_ID(1000),
		PRODUCT_FIND_WITH_PARAMS(1001),
		ROLE_FIND_BY_ID(1002),
		ROLE_FIND_WITH_PARAMS(1003),
		USER_FIND_BY_USERNAME(1004),
		USER_FIND_WITH_PARAMS(1005);
		
		private int action;
		 
	    private Action(int code) {
	        this.action = code;
	    }
	 
	    public int getCode() {
	        return action;
	    }
	};
	
	@Autowired
	private ODCUserPermissions permissions;
	
	private Map<Integer, ODCSystemRole[]> actionsPermissions = new HashMap<Integer, ODCSystemRole[]>();
	
	public ODCActionsPermissions add(int action, ODCSystemRole [] roles) {
		actionsPermissions.put(action, roles);
		return this;
	}
	
	public void checkCurrentUserGranted(int action) {
		if (actionsPermissions.containsKey(action)) {
			permissions.checkCurrentUserIsAuthorized(actionsPermissions.get(action));
		} else {
			// action not defined
		}
	}
	
	public String getRequestUsername() {
		return permissions.getRequestUsername();
	}
	
}
