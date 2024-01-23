package com.exprivia.odc.edmprovider;

import java.util.List;

import org.apache.olingo.commons.api.edm.provider.CsdlAction;

public interface ODCEdmAction {
	final String DEFAULT_ACTION_CREATE = "Create";
	final String DEFAULT_ACTION_UPDATE = "Update";
	final String DEFAULT_ACTION_DELETE = "Delete";
	
	List<CsdlAction> getActionList(); 
}
