package com.exprivia.odc.delegate.item;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceComplexProperty;
import org.apache.olingo.server.api.uri.UriResourcePrimitiveProperty;
import org.apache.olingo.server.api.uri.queryoption.OrderByItem;
import org.apache.olingo.server.api.uri.queryoption.OrderByOption;
import org.apache.olingo.server.api.uri.queryoption.expression.Expression;
import org.apache.olingo.server.core.uri.queryoption.expression.MemberImpl;

import com.exprivia.odc.exception.ODCApiNotImplementedException;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponse;

/**
 * @author andrea
 * 
 * Entity-based access to the storage
 * Classes implementing StorageItem are the bridges from the DTO objects
 * and the database itself.
 *
 */
public interface StorageItem<T> {
	Entity getById(String id);
	EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount);
	
	public Entity makeEntity(T dtoObject);
	
	default public EntityCollection makeEntityCollection(List<T> objList) {
		EntityCollection entitySet = new EntityCollection();
		for (T obj : objList) {
			entitySet.getEntities().add(makeEntity(obj));
		}

		return entitySet;
	}
	
	default ODCMediaResponse readMediaById(String id) throws ODataApplicationException {
		return null;
	}
	
	default Entity getRelatedEntity(Entity entity, String entityName) {
		return null;
	}
	
	default EntityCollection getRelatedEntities(Entity entity, String entityName, 
			JDBCQueryParams jdbcQueryParams, List<String> orderByColumns, boolean withCount) {
		return null;
	}
	
	default String convertColumnName(String name) {
		return name;
	}
	
	default String convertLiteralValue(String value) {
		return value;
	}
	
	default boolean isJsonProperty(String fieldName) {
		return false;
	}
	
	default void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand) {
		throw new ODCApiNotImplementedException(this.getClass().getCanonicalName() + " does not implements special json fields");
	}
	
	default Entity performEntityAction(String actionName, Entity newEntity, Entity boundEntity, String boundEntityType) throws ODataApplicationException {
		throw new ODCApiNotImplementedException("Action " + actionName + " not implemented in " + this.getClass().getCanonicalName());
	}
	
	default EntityCollection performEntityCollectionAction(String actionName, Entity newEntity, Entity boundEntity, String boundEntityType) throws ODataApplicationException {
		throw new ODCApiNotImplementedException("Action " + actionName + " not implemented in " + this.getClass().getCanonicalName());
	}
	
	default List<String> orderByColumns(OrderByOption orderBy) {
		if (orderBy == null) return null;
		
		List<String> orderByColumns = new ArrayList<String>();
		
		for (OrderByItem orderByItem : orderBy.getOrders()) {
			Expression expression = orderByItem.getExpression();
			
			String value = "";
			for (UriResource uriResource : ((MemberImpl)expression).getResourcePath().getUriResourceParts()) {
				if (uriResource instanceof UriResourcePrimitiveProperty) {
					value = value + uriResource.getSegmentValue();
				} else if (uriResource instanceof UriResourceComplexProperty) {
					value = value + uriResource.getSegmentValue() + ".";
				}
			}
			
			if (orderByItem.isDescending())
				orderByColumns.add(convertColumnName(value) + " desc");
			else
				orderByColumns.add(convertColumnName(value));
		}

		return orderByColumns;
	}
}
