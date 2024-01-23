package com.exprivia.odc.delegate.item;

import java.util.HashMap;
import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.data.dto.User;
import com.exprivia.odc.data.repositories.QuotaRepositoryJDBCImpl;
import com.exprivia.odc.data.repositories.SystemRoleRepositoryJDBCImpl;
import com.exprivia.odc.data.repositories.UserRepositoryJDBCImpl;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETQuota;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETSystemRole;
import com.exprivia.odc.service.filter.JDBCQueryParams;

@Component

public class StorageItemUser extends StorageItemRelational implements StorageItem<User> {

	@Autowired
	UserRepositoryJDBCImpl userRepository;
	
	@Autowired
	StorageItemSystemRole storageItemSystemRole;
	
	@Autowired
	StorageItemQuota storageItemQuota;
	
	@Override
	public Entity getById(String username) {
		User ret = userRepository.findByUsername(username);
		if (ret != null) {
			return makeEntity(ret);
		}
		return null;
	}

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(userRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(userRepository.getRecordsCount(queryParams));
		return ec;
	}

	@Override
	public EntityCollection getRelatedEntities(Entity entity, String entityName, JDBCQueryParams jdbcQueryParams, List<String> orderByColumns, boolean withCount) {
		if (entityName.equals(ODCEdmETSystemRole.ET_NAME)) {
			return storageItemSystemRole.findByFilter(
					SystemRoleRepositoryJDBCImpl.addUserCondition(
							jdbcQueryParams, 
							entity.getProperty(User.FIELD_USERNAME).getValue().toString()),
					null, withCount);
		} else if (entityName.equals(ODCEdmETQuota.ET_NAME) ) {
			return storageItemQuota.findByFilter(
					QuotaRepositoryJDBCImpl.addUserCondition(
							jdbcQueryParams, 
							entity.getProperty(User.FIELD_USERNAME).getValue().toString()),
					null, withCount);
		}
		
		return null;
	}

	// Convert instance of Object object into an Olingo Entity
	public Entity makeEntity(User user) {

		Entity entity = new Entity().addProperty(new Property(null, User.FIELD_USERNAME, ValueType.PRIMITIVE, user.getUsername()))
				.addProperty(new Property(null, User.FIELD_EMAIL, ValueType.PRIMITIVE, user.getEmail()))
				.addProperty(new Property(null, User.FIELD_CREATED, ValueType.PRIMITIVE, user.getCreated()))
				.addProperty(new Property(null, User.FIELD_DEFAULTPRIORITY, ValueType.PRIMITIVE, user.getDefaultPriority()))
				.addProperty(new Property(null, User.FIELD_MAXPRIORITY, ValueType.PRIMITIVE, user.getMaxPriority()));

		setEntityRelationalData(entity, user.getClass().getSimpleName(), new HashMap<String, String>());		
		return entity;
	}

	@Override
	public String convertColumnName(String name) {
		switch (name.toLowerCase()) {
		case "username":
			return "\"name\"";
		default:
			return name;
		}
	}
}
