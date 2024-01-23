package com.exprivia.odc.subscriptions.frontend.edmprovider.entitysets;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;
import org.apache.olingo.commons.api.edm.provider.CsdlNavigationPropertyBinding;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETSubscription;

public class ODCEdmESSubscriptions implements ODCEdmEntitySet {
	public static final String ES_NAME = "Subscriptions";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ES_NAME);
		entitySet.setType(ODCEdmETSubscription.ET_FQN);
		
		CsdlNavigationPropertyBinding navNotificationPropBinding = new CsdlNavigationPropertyBinding();
		navNotificationPropBinding.setTarget("Notifications"); // the target entity set, where the navigation property points to
		navNotificationPropBinding.setPath("Notifications"); // the path from entity type to navigation property

		CsdlNavigationPropertyBinding navOrdersPropBinding = new CsdlNavigationPropertyBinding();
		navOrdersPropBinding.setTarget("Orders"); // the target entity set, where the navigation property points to
		navOrdersPropBinding.setPath("Orders"); // the path from entity type to navigation property
		
		List<CsdlNavigationPropertyBinding> navPropBindingList = new ArrayList<CsdlNavigationPropertyBinding>();
		navPropBindingList.add(navNotificationPropBinding);
		navPropBindingList.add(navOrdersPropBinding);
		entitySet.setNavigationPropertyBindings(navPropBindingList);
		
		return entitySet;
	}

}
