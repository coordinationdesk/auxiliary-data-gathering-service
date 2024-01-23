package com.exprivia.odc.edmprovider.entitysets;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETAttribute;

public class ODCEdmESAttributes implements ODCEdmEntitySet {
	public static final String ES_NAME = "Attributes";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ODCEdmESAttributes.ES_NAME);
		entitySet.setType(ODCEdmETAttribute.ET_FQN);

		/*
		CsdlNavigationPropertyBinding navPropBinding = new CsdlNavigationPropertyBinding();
		navPropBinding.setTarget("Products"); // the target entity set, where the navigation property points to
		navPropBinding.setPath("Products"); // the path from entity type to navigation property
		List<CsdlNavigationPropertyBinding> navPropBindingList = new ArrayList<CsdlNavigationPropertyBinding>();
		navPropBindingList.add(navPropBinding);
		entitySet.setNavigationPropertyBindings(navPropBindingList);
		*/
		
		return entitySet;

	}
}
