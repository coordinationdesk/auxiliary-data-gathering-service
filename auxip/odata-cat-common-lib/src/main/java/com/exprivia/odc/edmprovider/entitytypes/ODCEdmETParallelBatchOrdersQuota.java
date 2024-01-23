package com.exprivia.odc.edmprovider.entitytypes;

import java.util.Arrays;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;

import com.exprivia.odc.data.dto.Quota;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmETParallelBatchOrdersQuota implements ODCEdmEntityType {
	public static final String ET_NAME = "ParallelBatchOrdersQuota";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);
	
	
	@Override
	public CsdlEntityType getCsdlEntityType() {
		CsdlProperty number = new CsdlProperty().setName(Quota.FIELD_NUMBER)
				.setType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName()).setNullable(false);
		
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(number));
		
		entityType.setBaseType(ODCEdmETBatchOrderQuota.ET_FQN);
		
		return entityType;
	}

}
