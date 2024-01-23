package com.exprivia.odc.delegate.item;

import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.data.dto.SystemRole;
import com.exprivia.odc.data.repositories.SystemRoleRepositoryJDBCImpl;
import com.exprivia.odc.service.filter.JDBCQueryParams;

@Component

public class StorageItemSystemRole implements StorageItem<SystemRole> {

	@Autowired
	SystemRoleRepositoryJDBCImpl systemRoleRepository;

	@Override
	public Entity getById(String name) {
		SystemRole ret = systemRoleRepository.findByName(name);
		if (ret != null) {
			return makeEntity(ret);
		}
		return null;
	}

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(systemRoleRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(systemRoleRepository.getRecordsCount(queryParams));
		return ec;
	}
	
	// Convert instance of Object object into an Olingo Entity
	public Entity makeEntity(SystemRole systemRole) {

		Entity entity = new Entity()
				.addProperty(new Property(null, SystemRole.FIELD_NAME, ValueType.PRIMITIVE, systemRole.getValue().getOrdinal()))
				.addProperty(new Property(null, SystemRole.FIELD_DESCRIPTION, ValueType.PRIMITIVE, systemRole.getDescription()));

		return entity;
	}

}
