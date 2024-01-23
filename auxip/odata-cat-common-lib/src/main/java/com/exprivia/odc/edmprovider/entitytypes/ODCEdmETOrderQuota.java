package com.exprivia.odc.edmprovider.entitytypes;

import java.util.ArrayList;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;

import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmETOrderQuota implements ODCEdmEntityType {
	public static final String ET_NAME = "OrderQuota";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);
	
	
	@Override
	public CsdlEntityType getCsdlEntityType() {
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(new ArrayList<CsdlProperty>());
		
		entityType.setBaseType(ODCEdmETQuota.ET_FQN);
		entityType.setAbstract(true);
		
		return entityType;
	}
	
	
}
