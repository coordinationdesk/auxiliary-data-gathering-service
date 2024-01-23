package com.exprivia.odc.edmprovider.entitytypes;

import java.util.Arrays;
import java.util.Collections;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlPropertyRef;

import com.exprivia.odc.data.dto.SystemRole;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.enumtypes.ODCEdmENRoleType;

public class ODCEdmETSystemRole implements ODCEdmEntityType {
	public static final String ET_NAME = "SystemRole";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType() {
		//create EntityType properties
		CsdlProperty name = new CsdlProperty().setName(SystemRole.FIELD_NAME).setType(ODCEdmENRoleType.EN_FQN)
				.setNullable(false);
		CsdlProperty description = new CsdlProperty().setName(SystemRole.FIELD_DESCRIPTION).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);

		// create CsdlPropertyRef for Key element
		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		propertyRef.setName(SystemRole.FIELD_NAME);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(name, description));
		entityType.setKey(Collections.singletonList(propertyRef));
		//entityType.setNavigationProperties(navPropList);

		return entityType;
	}
}
