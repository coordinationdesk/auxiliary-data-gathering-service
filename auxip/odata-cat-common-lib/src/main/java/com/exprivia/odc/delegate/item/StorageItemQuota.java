package com.exprivia.odc.delegate.item;

import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.data.dto.Quota;
import com.exprivia.odc.data.repositories.QuotaRepositoryJDBCImpl;
import com.exprivia.odc.service.filter.JDBCQueryParams;

@Component
public class StorageItemQuota implements StorageItem<Quota> {
	@Autowired
	QuotaRepositoryJDBCImpl quotaRepository;

	@Override
	public Entity getById(String id) {
		/*
		 * no direct quota reference is possible for now
		 */
		return null;
	}

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(quotaRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(quotaRepository.getRecordsCount(queryParams));
		return ec;
	}

	@Override
	public Entity makeEntity(Quota quota) {
		Entity entity = new Entity();
		entity.addProperty(new Property(null, Quota.FIELD_NUMBER, ValueType.PRIMITIVE, quota.getNumber()));
		
		if (quota.getDuration() != null)
			entity.addProperty(new Property(null, Quota.FIELD_DURATION, ValueType.PRIMITIVE, quota.getDuration().getSeconds()));
		
		// XXX Crucial part XXX
		// set the entity type of the specialized quota to let Olingo know which 
		// EntityType to use when deserialization takes place
		entity.setType(quota.getQuotaType().getFullQualifiedNameAsString());
		
		return entity;
	}
	
	@Override
	public String convertColumnName(String name) {
		return quotaRepository.convertColumnName(name);
	}
	
}
