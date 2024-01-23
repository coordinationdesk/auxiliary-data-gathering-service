package com.exprivia.odc.edmprovider.entitytypes;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlNavigationProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlPropertyRef;

import com.exprivia.odc.data.dto.User;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmETUser implements ODCEdmEntityType {
	public static final String ET_NAME = "User";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType() {
		//create EntityType properties
		CsdlProperty username = new CsdlProperty().setName(User.FIELD_USERNAME).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty email = new CsdlProperty().setName(User.FIELD_EMAIL).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty created = new CsdlProperty().setName(User.FIELD_CREATED).setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty defaultPriority = new CsdlProperty().setName(User.FIELD_DEFAULTPRIORITY)
				.setType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName()).setNullable(false);
		CsdlProperty maxPriority = new CsdlProperty().setName(User.FIELD_MAXPRIORITY).setType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName())
				.setNullable(false);

		// create CsdlPropertyRef for Key element
		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		propertyRef.setName(User.FIELD_USERNAME);

		CsdlNavigationProperty productNavPropRoles = new CsdlNavigationProperty().setName(User.FIELD_SYSTEMROLES)
				.setNullable(false).setType(ODCEdmETSystemRole.ET_FQN).setContainsTarget(true).setCollection(true);
		CsdlNavigationProperty productNavPropQuotas = new CsdlNavigationProperty().setName(User.FIELD_QUOTAS)
				.setNullable(false).setType(ODCEdmETQuota.ET_FQN).setContainsTarget(true).setCollection(true);
		List<CsdlNavigationProperty> navPropList = new ArrayList<CsdlNavigationProperty>();
		navPropList.add(productNavPropRoles);
		navPropList.add(productNavPropQuotas);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(username, email, created, defaultPriority, maxPriority));
		entityType.setKey(Collections.singletonList(propertyRef));
		entityType.setNavigationProperties(navPropList);

		return entityType;
	}
}
