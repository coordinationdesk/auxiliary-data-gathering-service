package com.exprivia.odc.subscriptions.frontend.edmprovider.entitysets;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETNotification;

public class ODCEdmESNotifications implements ODCEdmEntitySet {
	public static final String ES_NAME = "Notifications";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ES_NAME);
		entitySet.setType(ODCEdmETNotification.ET_FQN);

		return entitySet;
	}
	
	
}
