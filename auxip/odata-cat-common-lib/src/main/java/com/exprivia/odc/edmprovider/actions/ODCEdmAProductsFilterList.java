package com.exprivia.odc.edmprovider.actions;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlAction;
import org.apache.olingo.commons.api.edm.provider.CsdlParameter;
import org.apache.olingo.commons.api.edm.provider.CsdlReturnType;

import com.exprivia.odc.edmprovider.ODCEdmAction;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETProduct;

public class ODCEdmAProductsFilterList implements ODCEdmAction {

	public static final String AC_PRODUCTSFILTERLIST_NAME = "FilterList";
	public static final FullQualifiedName AC_PRODUCTSFILTERLIST_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, AC_PRODUCTSFILTERLIST_NAME);
	public static final FullQualifiedName AC_PRODUCTSFILTERLIST_FQN_ALT1 = new FullQualifiedName("Products", AC_PRODUCTSFILTERLIST_NAME);
	public static final String PARAMETER_DUMMY = "dummy";
	public static final String PARAMETER_PRODUCTS = "FilterProducts";

	@Override
	public List<CsdlAction> getActionList() {
		// It is allowed to overload actions, so we have to provide a list of Actions for each action name
		List<CsdlAction> actions = new ArrayList<>();
		
		// Create parameters
		List<CsdlParameter> parameters = new ArrayList<>();
		
		// TODO check how to implement without this fake parameter
		CsdlParameter parameterDummy = new CsdlParameter();
		parameterDummy.setName(PARAMETER_DUMMY);
		parameterDummy.setType(ODCEdmETProduct.ET_FQN);
		parameterDummy.setCollection(true);
		parameters.add(parameterDummy);
		
		CsdlParameter parameterProduct = new CsdlParameter();
		parameterProduct.setName(PARAMETER_PRODUCTS);
		parameterProduct.setType(ODCEdmETProduct.ET_FQN);
		parameterProduct.setCollection(true);
		parameters.add(parameterProduct);

		// Create the Csdl Action
		CsdlAction action = new CsdlAction();
		action.setName(AC_PRODUCTSFILTERLIST_FQN.getName());
		action.setParameters(parameters);
		action.setBound(true);
		action.setReturnType(new CsdlReturnType().setType(ODCEdmETProduct.ET_FQN).setCollection(true));
		actions.add(action);
		
		CsdlAction action2 = new CsdlAction();
		action2.setName(AC_PRODUCTSFILTERLIST_FQN_ALT1.getName());
		action2.setParameters(parameters);
		action2.setBound(true);
		action2.setReturnType(new CsdlReturnType().setType(ODCEdmETProduct.ET_FQN).setCollection(true));
		actions.add(action2);
		
		return actions;
	}

}
