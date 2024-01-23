package com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes;

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

import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.subscriptions.frontend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.frontend.edmprovider.enumtypes.ODCEdmENSubscriptionStatus;

public class ODCEdmETSubscription implements ODCEdmEntityType {
	public static final String ET_NAME = "Subscription";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType() {
		//create EntityType properties
		CsdlProperty id = new CsdlProperty().setName(Subscription.FIELD_ID)
				.setType(EdmPrimitiveTypeKind.Guid.getFullQualifiedName()).setNullable(false);
		CsdlProperty status = new CsdlProperty().setName(Subscription.FIELD_STATUS)
				.setType(ODCEdmENSubscriptionStatus.EN_FQN).setNullable(false);
		CsdlProperty filterParam = new CsdlProperty().setName(Subscription.FIELD_FILTERPARAM)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(false);
		CsdlProperty submissionDate = new CsdlProperty().setName(Subscription.FIELD_SUBMISSIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);
		CsdlProperty lastNotificationDate = new CsdlProperty().setName(Subscription.FIELD_LASTNOTIFICATIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(true);
		CsdlProperty notificationEndpoint = new CsdlProperty().setName(Subscription.FIELD_NOTIFICATIONENDPOINT)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(false);
		CsdlProperty notificationEpUsername = new CsdlProperty().setName(Subscription.FIELD_NOTIFICATIONEPUSERNAME)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(true);
		CsdlProperty notificationEpPassword = new CsdlProperty().setName(Subscription.FIELD_NOTIFICATIONEPPASSWORD)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(true);

		// create CsdlPropertyRef for Key element
		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		propertyRef.setName(Subscription.FIELD_ID);

		CsdlNavigationProperty attributesNavProp = new CsdlNavigationProperty().setName(Subscription.FIELD_NOTIFICATIONS)
				.setNullable(false).setType(ODCEdmETNotification.ET_FQN).setContainsTarget(true).setCollection(true);
		List<CsdlNavigationProperty> navPropList = new ArrayList<CsdlNavigationProperty>();
		navPropList.add(attributesNavProp);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(id, status, filterParam, submissionDate,
				lastNotificationDate, notificationEndpoint, notificationEpUsername, notificationEpPassword));
		entityType.setKey(Collections.singletonList(propertyRef));
		entityType.setNavigationProperties(navPropList);
		
		return entityType;
	}
}
