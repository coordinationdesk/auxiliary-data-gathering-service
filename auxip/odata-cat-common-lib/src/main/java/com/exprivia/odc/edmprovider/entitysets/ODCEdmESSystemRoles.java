package com.exprivia.odc.edmprovider.entitysets;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETSystemRole;

public class ODCEdmESSystemRoles implements ODCEdmEntitySet {
	public static final String ES_NAME = "SystemRoles";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ODCEdmESSystemRoles.ES_NAME);
		entitySet.setType(ODCEdmETSystemRole.ET_FQN);

		/*
		TODO check navigation
		
		CsdlNavigationPropertyBinding navPropBinding = new CsdlNavigationPropertyBinding();
		navPropBinding.setTarget("Products"); // the target entity set, where the navigation property points to
		navPropBinding.setPath("Product"); // the path from entity type to navigation property
		List<CsdlNavigationPropertyBinding> navPropBindingList = new ArrayList<CsdlNavigationPropertyBinding>();
		navPropBindingList.add(navPropBinding);
		entitySet.setNavigationPropertyBindings(navPropBindingList);
		*/
		
		return entitySet;
	}
	
	
}
