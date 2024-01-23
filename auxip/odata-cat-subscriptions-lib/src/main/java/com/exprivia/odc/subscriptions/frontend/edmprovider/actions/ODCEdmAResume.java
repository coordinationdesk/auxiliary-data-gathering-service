package com.exprivia.odc.subscriptions.frontend.edmprovider.actions;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlAction;
import org.apache.olingo.commons.api.edm.provider.CsdlParameter;
import org.apache.olingo.commons.api.edm.provider.CsdlReturnType;

import com.exprivia.odc.edmprovider.ODCEdmAction;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETSubscription;

public class ODCEdmAResume implements ODCEdmAction {

	public static final String AC_RESUME_NAME = "Resume";
	public static final FullQualifiedName AC_RESUME_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, AC_RESUME_NAME);
	public static final String PARAMETER_SUBSCRIPTION = "Subscription";

	@Override
	public List<CsdlAction> getActionList() {
		// It is allowed to overload actions, so we have to provide a list of Actions for each action name
		List<CsdlAction> actions = new ArrayList<>();
		
		// Create parameters
		List<CsdlParameter> parameters = new ArrayList<>();
		CsdlParameter parameterSubscription = new CsdlParameter();
		parameterSubscription.setName(PARAMETER_SUBSCRIPTION);
		parameterSubscription.setType(ODCEdmETSubscription.ET_FQN);
		parameterSubscription.setCollection(false);
		parameters.add(parameterSubscription);

		// Create the Csdl Action
		CsdlAction action = new CsdlAction();
		action.setName(AC_RESUME_FQN.getName());
		action.setParameters(parameters);
		action.setBound(true);
		action.setReturnType(new CsdlReturnType().setType(ODCEdmETSubscription.ET_FQN).setCollection(false));
		actions.add(action);
		
		return actions;
	}

}
