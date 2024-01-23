package com.exprivia.odc.edmprovider.primitivetypes;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;

import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmPTBooleanAttribute extends ODCEdmPrimitive implements ODCEdmEntityType {
	public static final String ET_NAME = "BooleanAttribute";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);
	public static final String ET_VALUE_NAME = "Value";

	@Override
	public CsdlEntityType getCsdlEntityType() {
		return createAttribute(ET_NAME, ET_VALUE_NAME, EdmPrimitiveTypeKind.Boolean);
	}

}
