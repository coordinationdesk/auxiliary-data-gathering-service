package com.exprivia.odc.edmprovider.complextypes;

import java.util.Arrays;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlComplexType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;

import com.exprivia.odc.data.dto.Checksum;
import com.exprivia.odc.edmprovider.ODCEdmComplexType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;

public class ODCEdmCTCheckSum implements ODCEdmComplexType {
	public static final String CT_NAME = "Checksums";
	public static final FullQualifiedName CT_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, CT_NAME);
	
	@Override
	public CsdlComplexType getCsdlComplexType() {
		CsdlComplexType complexType = new CsdlComplexType().setName(ODCEdmCTCheckSum.CT_NAME)
				.setProperties(Arrays.asList(
						new CsdlProperty().setName(Checksum.FIELD_ALGORITHM)
								.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()),
						new CsdlProperty().setName(Checksum.FIELD_VALUE)
								.setType(EdmPrimitiveTypeKind.String.getFullQualifiedName()),
						new CsdlProperty().setName(Checksum.FIELD_CHECKSUMDATE)
								.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName())));
		return complexType;
	}
}
