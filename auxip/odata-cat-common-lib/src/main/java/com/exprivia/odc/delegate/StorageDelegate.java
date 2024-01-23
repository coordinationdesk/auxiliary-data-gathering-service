package com.exprivia.odc.delegate;

import java.util.List;
import java.util.Locale;
import java.util.Map;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Parameter;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.edm.EdmAction;
import org.apache.olingo.commons.api.edm.EdmEntitySet;
import org.apache.olingo.commons.api.edm.EdmEntityType;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.uri.UriParameter;
import org.apache.olingo.server.api.uri.queryoption.OrderByOption;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.item.StorageItem;
import com.exprivia.odc.edmprovider.ODCEdmAction;
import com.exprivia.odc.service.ODCReturnedEntity;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponse;
import com.exprivia.odc.util.Util;

/**
 * @author andrea
 * 
 * StorageDelegate class lets the service processors to read and write
 * OData entities in high-level way
 *
 */
@Component
public class StorageDelegate {

	@Autowired
	private StorageItemFactory factory;

	public EntityCollection readEntitySetData(EdmEntitySet edmEntitySet, JDBCQueryParams query, 
			List<String> orderByColumns, boolean withCount) throws ODataApplicationException {

		return factory.createFromName(edmEntitySet.getName()).findByFilter(query, orderByColumns, withCount);
	}

	public Entity readEntityData(EdmEntitySet edmEntitySet, List<UriParameter> keyParams) throws ODataApplicationException {
		Entity requestedEntity = null;
		EdmEntityType edmEntityType = edmEntitySet.getEntityType();

		String id = Util.trimQuotes(keyParams.get(0).getText());
		
		requestedEntity = factory.createFromName(edmEntityType.getName()).getById(id);

		if (requestedEntity == null) {
			// this variable is null if our data doesn't contain an entity for the requested key
			// Throw suitable exception
			throw new ODataApplicationException("Entity for requested key doesn't exist", HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}

		return requestedEntity;
	}

	public ODCMediaResponse readEntityMedia(EdmEntitySet edmEntitySet, List<UriParameter> keyParams) throws ODataApplicationException {
		EdmEntityType edmEntityType = edmEntitySet.getEntityType();

		String id = Util.trimQuotes(keyParams.get(0).getText());
		return factory.createFromName(edmEntityType.getName()).readMediaById(id);
	}

	public Entity getRelatedEntity(Entity entity, EdmEntityType expandEdmEntityType) {
		return factory.createFromEntity(entity).getRelatedEntity(entity, expandEdmEntityType.getName());
	}
	
	public Entity getRelatedEntity(Entity entity, EdmEntityType expandEdmEntityType, List<UriParameter> keyPredicates) throws ODataApplicationException {
		Entity requestedEntity = null;
		String id = Util.trimQuotes(keyPredicates.get(0).getText());
		
		requestedEntity = factory.createFromName(expandEdmEntityType.getName()).getById(id);

		if (requestedEntity == null) {
			// this variable is null if our data doesn't contain an entity for the requested key
			// Throw suitable exception
			throw new ODataApplicationException("Entity for requested key doesn't exist", HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}

		return requestedEntity;
	}
	
	public EntityCollection getRelatedEntities(Entity entity, EdmEntityType expandEdmEntityType, 
			JDBCQueryParams jdbcQueryParams, List<String> orderByColumns, boolean withCount) {
		return factory.createFromEntity(entity)
				.getRelatedEntities(entity, expandEdmEntityType.getName(), jdbcQueryParams, orderByColumns, withCount);
	}
	
	public StorageItem<?> createStorageItem(EdmEntityType entityType) {
		return factory.createFromName(entityType.getName());
	}
	
	public ODCReturnedEntity processBoundActionEntity(EdmAction action, Map<String, Parameter> parameters, List<UriParameter> keyPredicates) throws ODataApplicationException {
		Entity boundEntity = null;
		String boundEntityType = null;
		
		if (keyPredicates != null && keyPredicates.size() > 0) {
			List<String> parameterNames = action.getParameterNames();
			boundEntityType = action.getParameter(parameterNames.get(0)).getType().getName();
	
			String id = Util.trimQuotes(keyPredicates.get(0).getText());
			
			StorageItem<?> storage = factory.createFromName(boundEntityType);
			if (storage == null) {
				throw new ODataApplicationException("Unknown Entity type " + boundEntityType, HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
			}
			
			boundEntity = null;
			if (id != null) {
				boundEntity = storage.getById(id);
				if (boundEntity == null) {
					throw new ODataApplicationException(boundEntityType + " with key " + id + " doesn't exist", HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
				}
			}
		}
		
		// insert new data into the storage
		Entity newEntity = new Entity();
		for (Parameter parameter : parameters.values()) {
			Property property = new Property();
			property.setName(parameter.getName());
			property.setValue(parameter.getValueType(), parameter.getValue());
			newEntity.addProperty(property);
		}
		
		if (action.getReturnType() != null) {
			newEntity.setType(action.getReturnType().getType().getFullQualifiedName().toString());
			return processEntityAction(action.getName(), newEntity, boundEntity, boundEntityType);
		} else {
			// perform void action (no return code)
			processVoidAction(action.getName(), newEntity, boundEntity, boundEntityType);
			return null;
		}
	}
	
	public ODCReturnedEntity processCreateEntity(Entity newEntity) throws ODataApplicationException {
		return processEntityAction(ODCEdmAction.DEFAULT_ACTION_CREATE, newEntity, null, null);
	}
	
	protected ODCReturnedEntity processEntityAction(String actionName, Entity newEntity, Entity boundEntity, String boundEntityType) throws ODataApplicationException {
		FullQualifiedName fqn = new FullQualifiedName(newEntity.getType());
		StorageItem<?> writeStorage = factory.createFromName(fqn.getName());
		if (writeStorage == null) {
			throw new ODataApplicationException("Unknown Entity type " + newEntity.getType(), HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}
		
		Entity createdEntity = writeStorage.performEntityAction(actionName, newEntity, boundEntity, boundEntityType);
		return ODCReturnedEntity.builder().created(true).entity(createdEntity).build();
	}

	protected void processVoidAction(String actionName, Entity additionalParameters, Entity boundEntity, String boundEntityType) throws ODataApplicationException {
		StorageItem<?> writeStorage = factory.createFromName(boundEntityType);
		if (writeStorage == null) {
			throw new ODataApplicationException("Unknown Entity type " + boundEntityType, HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}
		
		writeStorage.performEntityAction(actionName, additionalParameters, boundEntity, boundEntityType);
	}

	public ODCReturnedEntity processUpdateEntity(Entity entity) throws ODataApplicationException {
		FullQualifiedName fqn = new FullQualifiedName(entity.getType());
		StorageItem<?> writeStorage = factory.createFromName(fqn.getName());
		if (writeStorage == null) {
			throw new ODataApplicationException("Unknown Entity type " + entity.getType(), HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}
		
		Entity updatedEntity = writeStorage.performEntityAction(ODCEdmAction.DEFAULT_ACTION_UPDATE, entity, null, null);
		return ODCReturnedEntity.builder().created(false).entity(updatedEntity).build();
	}
	
	public EntityCollection processBoundActionEntityCollection(EdmAction action, Map<String, Parameter> parameters, List<UriParameter> keyPredicates) throws ODataApplicationException {
		List<String> parameterNames = action.getParameterNames();
		String boundEntityType = action.getParameter(parameterNames.get(0)).getType().getName();

		String id = null;
		if (keyPredicates.size() > 0) {
			id = Util.trimQuotes(keyPredicates.get(0).getText());
		}
		
		StorageItem<?> storage = factory.createFromName(boundEntityType);
		if (storage == null) {
			throw new ODataApplicationException("Unknown Entity type " + boundEntityType, HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}
		
		Entity boundEntity = null;
		if (id != null) {
			boundEntity = storage.getById(id);
			if (boundEntity == null) {
				throw new ODataApplicationException(boundEntityType + " with key " + id + " doesn't exist", HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
			}
		}
		
		String returnEntityName = action.getReturnType().getType().getName();
		StorageItem<?> writeStorage = factory.createFromName(returnEntityName);
		if (writeStorage == null) {
			throw new ODataApplicationException("Unknown Entity type " + returnEntityName, HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}
		
		// insert new data into the storage
		Entity newEntity = new Entity();
		for (Parameter parameter : parameters.values()) {
			Property property = new Property();
			property.setName(parameter.getName());
			property.setValue(parameter.getValueType(), parameter.getValue());
			newEntity.addProperty(property);
		}
		return storage.performEntityCollectionAction(action.getName(), newEntity, boundEntity, boundEntityType);
	}

	public List<String> orderByColumns(EdmEntitySet edmEntitySet, OrderByOption orderBy) {
		return createStorageItem(edmEntitySet.getEntityType()).orderByColumns(orderBy);
	}
	
	public List<String> orderByColumns(EdmEntityType edmEntityType, OrderByOption orderBy) {
		return createStorageItem(edmEntityType).orderByColumns(orderBy);
	}
}
