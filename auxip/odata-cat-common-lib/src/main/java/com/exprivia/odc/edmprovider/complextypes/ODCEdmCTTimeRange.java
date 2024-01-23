package com.exprivia.odc.edmprovider.complextypes;

import java.util.Arrays;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlComplexType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;

import com.exprivia.odc.data.dto.TimeRange;
import com.exprivia.odc.edmprovider.ODCEdmComplexType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmCTTimeRange implements ODCEdmComplexType {
	public static final String CT_NAME = "TimeRange";
	public static final FullQualifiedName CT_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, CT_NAME);
	
	@Override
	public CsdlComplexType getCsdlComplexType() {
		CsdlComplexType complexType = new CsdlComplexType().setName(ODCEdmCTTimeRange.CT_NAME)
				.setProperties(Arrays.asList(
						new CsdlProperty().setName(TimeRange.FIELD_START)
								.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()),
						new CsdlProperty().setName(TimeRange.FIELD_END)
								.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName())));
		return complexType;
	}
}
