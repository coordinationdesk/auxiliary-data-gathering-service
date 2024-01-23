package com.exprivia.odc.edmprovider.entitysets;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;
import org.apache.olingo.commons.api.edm.provider.CsdlNavigationPropertyBinding;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;

public class ODCEdmESProducts implements ODCEdmEntitySet {
	public static final String ES_NAME = "Products";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(com.exprivia.odc.edmprovider.entitysets.ODCEdmESProducts.ES_NAME);
		entitySet.setType(com.exprivia.odc.edmprovider.entitytypes.ODCEdmETProduct.ET_FQN);

		CsdlNavigationPropertyBinding navPropBinding = new CsdlNavigationPropertyBinding();
		navPropBinding.setTarget("Attributes"); // the target entity set, where the navigation property points to
		navPropBinding.setPath("Attributes"); // the path from entity type to navigation property
		List<CsdlNavigationPropertyBinding> navPropBindingList = new ArrayList<CsdlNavigationPropertyBinding>();
		navPropBindingList.add(navPropBinding);
		entitySet.setNavigationPropertyBindings(navPropBindingList);
		
		return entitySet;
	}
	
	
}
