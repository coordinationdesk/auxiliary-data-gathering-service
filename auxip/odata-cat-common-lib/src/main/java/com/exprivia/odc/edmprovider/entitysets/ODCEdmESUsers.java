package com.exprivia.odc.edmprovider.entitysets;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;
import org.apache.olingo.commons.api.edm.provider.CsdlNavigationPropertyBinding;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETUser;

public class ODCEdmESUsers implements ODCEdmEntitySet {
	public static final String ES_NAME = "Users";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ODCEdmESUsers.ES_NAME);
		entitySet.setType(ODCEdmETUser.ET_FQN);

		CsdlNavigationPropertyBinding navPropBindingSystemRoles = new CsdlNavigationPropertyBinding();
		navPropBindingSystemRoles.setTarget("SystemRoles"); // the target entity set, where the navigation property points to
		navPropBindingSystemRoles.setPath("SystemRoles"); // the path from entity type to navigation property

		CsdlNavigationPropertyBinding navPropBindingQuotas = new CsdlNavigationPropertyBinding();
		navPropBindingQuotas.setTarget("Quotas"); // the target entity set, where the navigation property points to
		navPropBindingQuotas.setPath("Quotas"); // the path from entity type to navigation property

		List<CsdlNavigationPropertyBinding> navPropBindingList = new ArrayList<CsdlNavigationPropertyBinding>();
		navPropBindingList.add(navPropBindingSystemRoles);
		navPropBindingList.add(navPropBindingQuotas);
		entitySet.setNavigationPropertyBindings(navPropBindingList);
		
		return entitySet;
	}
	
	
}
