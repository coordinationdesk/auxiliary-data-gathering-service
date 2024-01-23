package com.exprivia.odc.edmprovider.primitivetypes;

import java.util.Arrays;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;

import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETAttribute;

public class ODCEdmPrimitive {
	protected CsdlEntityType createAttribute(String attributeName, String valueName, EdmPrimitiveTypeKind type) {
		CsdlProperty value = new CsdlProperty().setName(valueName).setType(type.getFullQualifiedName())
				.setNullable(false);

		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(attributeName);
		entityType.setProperties(Arrays.asList(value));
		
		entityType.setBaseType(ODCEdmETAttribute.ET_FQN);
		//entityType.setAbstract(true);
		
		return entityType;
	}
}
