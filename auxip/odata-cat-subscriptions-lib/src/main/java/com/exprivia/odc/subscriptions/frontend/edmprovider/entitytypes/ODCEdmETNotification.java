package com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes;

import java.util.Arrays;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;

import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.subscriptions.frontend.data.dto.Notification;

public class ODCEdmETNotification implements ODCEdmEntityType {
	public static final String ET_NAME = "Notification";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType() {
		//create EntityType properties
		CsdlProperty productId = new CsdlProperty().setName(Notification.FIELD_PRODUCTID)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(false);
		CsdlProperty productName = new CsdlProperty().setName(Notification.FIELD_PRODUCTNAME)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(true);
		CsdlProperty subscriptionID = new CsdlProperty().setName(Notification.FIELD_SUBSCRIPTIONID)
				.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable(true);
		CsdlProperty notificationDate = new CsdlProperty().setName(Notification.FIELD_NOTIFICATIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);

		// create CsdlPropertyRef for Key element
//		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
//		propertyRef.setName(Order.FIELD_ID);

//		CsdlNavigationProperty attributesNavProp = new CsdlNavigationProperty().setName(Notification.FIELD_NOTIFICATIONS)
//				.setNullable(false).setType(LTAEdmETNotification.ET_FQN).setContainsTarget(true).setCollection(true);
//		List<CsdlNavigationProperty> navPropList = new ArrayList<CsdlNavigationProperty>();
//		navPropList.add(attributesNavProp);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(productId, productName, subscriptionID, notificationDate));
//		entityType.setKey(Collections.singletonList(propertyRef));
//		entityType.setNavigationProperties(navPropList);
		
		return entityType;
	}
}
