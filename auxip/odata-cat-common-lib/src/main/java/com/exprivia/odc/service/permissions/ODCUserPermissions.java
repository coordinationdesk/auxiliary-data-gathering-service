package com.exprivia.odc.service.permissions;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.exprivia.odc.data.dto.SystemRole;
import com.exprivia.odc.data.repositories.SystemRoleRepositoryJDBCImpl;
import com.exprivia.odc.exception.ODCApiUnauthorizedException;

public class ODCUserPermissions {
	@Autowired
	private SystemRoleRepositoryJDBCImpl systemRoleRepository;
	
	private static final Logger LOG = LoggerFactory.getLogger(ODCUserPermissions.class);
	
	private ODCSystemRoleList userPermissions;
	private String currentUsername;
	
	public void initializeUserPermissions(String username) {
		currentUsername = username;
		
		List<SystemRole> roles = systemRoleRepository.findWithParamsInternal(
				SystemRoleRepositoryJDBCImpl.addUserCondition(null, currentUsername), 
				null);
		userPermissions = new ODCSystemRoleList();
		for (SystemRole role : roles) {
			userPermissions.addValue(role.getValue());
		}
		
		LOG.info("User " + currentUsername + " permissions: " + userPermissions);
	}

	public void checkCurrentUserIsAuthorized(ODCSystemRole permission) {
		if (!currentUserHasPermission(permission))
			throw new ODCApiUnauthorizedException("User " + currentUsername + " does not have '" + permission.getName() + "' permission");
	}
	
	public void checkCurrentUserIsAuthorized(ODCSystemRole... permissions) {
		boolean authorized = false;
		for (ODCSystemRole permission : permissions) {
			authorized = authorized || currentUserHasPermission(permission);
		}
		if (authorized == false) {
			throw new ODCApiUnauthorizedException("User " + currentUsername + " does not have at least one of the following permissions: " + ODCSystemRoleList.toString(permissions));
		}
	}
	
	public boolean currentUserHasPermission(ODCSystemRole permission) {
		for (ODCSystemRole currentPermission : userPermissions.getValues()) {
			if (permission.getDefine().equals(currentPermission.getDefine()))
				return true;
		}
		return false;
	}
	
	public String getRequestUsername() {
		return currentUsername;
	}
}
