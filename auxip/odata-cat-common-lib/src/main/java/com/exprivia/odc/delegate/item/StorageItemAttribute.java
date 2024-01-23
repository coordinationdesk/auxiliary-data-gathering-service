package com.exprivia.odc.delegate.item;

import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.data.dto.Attribute;
import com.exprivia.odc.data.repositories.AttributeRepositoryJDBCImpl;
import com.exprivia.odc.service.filter.JDBCQueryParams;

@Component

public class StorageItemAttribute implements StorageItem<Attribute<?>> {

	@Autowired
	AttributeRepositoryJDBCImpl attributeRepository;

	@Override
	public Entity getById(String name) {
		return null;
	}

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(attributeRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(attributeRepository.getRecordsCount(queryParams));
		return ec;
	}

	// Convert instance of Object object into an Olingo Entity
	public Entity makeEntity(Attribute<?> attribute) {

		Entity entity = new Entity()
				.addProperty(new Property(null, Attribute.FIELD_NAME, ValueType.PRIMITIVE, attribute.getName()))
				.addProperty(new Property(null, Attribute.FIELD_VALUETYPE, ValueType.PRIMITIVE, attribute.getValueType()))
				.addProperty(new Property(null, Attribute.FIELD_VALUE, ValueType.PRIMITIVE, attribute.getValue()));
		
		// XXX Crucial part XXX
		// set the entity type of the specialized attribute to let Olingo know which 
		// EntityType to use when deserialization takes place
		entity.setType(attribute.getAttributeType().getFullQualifiedNameAsString());
		
		return entity;
	}
	
	@Override
	public String convertColumnName(String name) {
		return attributeRepository.convertColumnName(name);
	}

}
