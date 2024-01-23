package com.exprivia.odc.delegate.item;

import java.util.HashMap;
import java.util.Map;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;

public class StorageItemRelational {

	public void setEntityRelationalData(Entity entity, String itemClassName, Map<String, String> additionalReferenceData) {
		for (Map.Entry<String, String> entry : additionalReferenceData.entrySet()) {
			entity.addProperty(new Property(null, itemClassName + "." + entry.getKey(), ValueType.PRIMITIVE, entry.getValue()));
		}

		entity.addProperty(new Property(null, "EntityType", ValueType.PRIMITIVE, itemClassName));
	}

	public StorageItemRelationalData getEntityRelationalData(Entity entity) {
		String className = getEntityClass(entity);

		Map<String, String> referenceKeys = new HashMap<String, String>();

		for (Property property : entity.getProperties()) {
			if (property.getName().startsWith(className + ".")) {
				referenceKeys.put(property.getName().substring(className.length() + 1), property.getValue().toString());
			}
		}

		return new StorageItemRelationalData(className, referenceKeys);
	}
	
	public static String getEntityClass(Entity entity) {
		Property propertyEntityType = entity.getProperty("EntityType");
		if (propertyEntityType != null)
			return propertyEntityType.getValue().toString();
		else
			return null;
	}

	static public class StorageItemRelationalData {
		protected String entityType;
		protected Map<String, String> referenceKeys;

		StorageItemRelationalData(String entityType, Map<String, String> referenceKeys) {
			this.entityType = entityType;

			this.referenceKeys = new HashMap<String, String>();
			for (Map.Entry<String, String> entry : referenceKeys.entrySet()) {
				this.referenceKeys.put(entry.getKey(), entry.getValue());
			}
		}

		public String getEntityType() {
			return entityType;
		}

		public Map<String, String> getReferenceKeys() {
			return referenceKeys;
		}
	}
}
