package com.exprivia.odc.edmprovider.entitytypes;

import java.util.Arrays;
import java.util.Collections;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlPropertyRef;

import com.exprivia.odc.data.dto.Attribute;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmETAttribute implements ODCEdmEntityType {
	public static final String ET_NAME = "Attribute";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);
	
	
	@Override
	public CsdlEntityType getCsdlEntityType() {
		CsdlProperty name = new CsdlProperty().setName(Attribute.FIELD_NAME)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(false);
		CsdlProperty contentType = new CsdlProperty().setName(Attribute.FIELD_VALUETYPE)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(false);

		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(name, contentType));

		entityType.setAbstract(true);

		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		propertyRef.setName(Attribute.FIELD_NAME);
		entityType.setKey(Collections.singletonList(propertyRef));
		
		return entityType;
	}
	
	
}
