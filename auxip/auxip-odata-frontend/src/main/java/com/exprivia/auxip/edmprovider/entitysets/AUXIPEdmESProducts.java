package com.exprivia.auxip.edmprovider.entitysets;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;
import org.apache.olingo.commons.api.edm.provider.CsdlNavigationPropertyBinding;

import com.exprivia.auxip.edmprovider.entitytypes.AUXIPEdmETProduct;
import com.exprivia.odc.edmprovider.ODCEdmEntitySet;

public class AUXIPEdmESProducts implements ODCEdmEntitySet {
	public static final String ES_NAME = "Products";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ES_NAME);
		entitySet.setType(AUXIPEdmETProduct.ET_FQN);

		CsdlNavigationPropertyBinding navPropBinding = new CsdlNavigationPropertyBinding();
		navPropBinding.setTarget("Attributes"); // the target entity set, where the navigation property points to
		navPropBinding.setPath("Attributes"); // the path from entity type to navigation property
		List<CsdlNavigationPropertyBinding> navPropBindingList = new ArrayList<CsdlNavigationPropertyBinding>();
		navPropBindingList.add(navPropBinding);
		entitySet.setNavigationPropertyBindings(navPropBindingList);
		
		return entitySet;
	}
	
	
}
