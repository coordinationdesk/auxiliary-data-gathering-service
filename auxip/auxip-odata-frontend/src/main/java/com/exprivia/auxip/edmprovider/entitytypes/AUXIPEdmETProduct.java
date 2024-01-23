package com.exprivia.auxip.edmprovider.entitytypes;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlNavigationProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlPropertyRef;

import com.exprivia.auxip.data.dto.AUXIPProduct;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.complextypes.ODCEdmCTCheckSum;
import com.exprivia.odc.edmprovider.complextypes.ODCEdmCTTimeRange;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETAttribute;

public class AUXIPEdmETProduct implements ODCEdmEntityType {
	public static final String ET_NAME = "Product";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType() {
		//create EntityType properties
		CsdlProperty id = new CsdlProperty().setName(AUXIPProduct.FIELD_ID).setType(EdmPrimitiveTypeKind.Guid.getFullQualifiedName()).setNullable(false);
		CsdlProperty name = new CsdlProperty().setName(AUXIPProduct.FIELD_NAME).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty contentType = new CsdlProperty().setName(AUXIPProduct.FIELD_CONTENTTYPE).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty contentLength = new CsdlProperty().setName(AUXIPProduct.FIELD_CONTENTLENGTH)
				.setType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName()).setNullable(false);
		CsdlProperty originDate = new CsdlProperty().setName(AUXIPProduct.FIELD_ORIGINDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(true);
		CsdlProperty creationDate = new CsdlProperty().setName(AUXIPProduct.FIELD_PUBLICATIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);
		CsdlProperty evictionDate = new CsdlProperty().setName(AUXIPProduct.FIELD_EVICTIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName());
		CsdlProperty checksums = new CsdlProperty().setName(AUXIPProduct.FIELD_CHECKSUMS).setType(ODCEdmCTCheckSum.CT_FQN).setCollection(true)
				.setNullable(false);
		CsdlProperty contentDate = new CsdlProperty().setName(AUXIPProduct.FIELD_CONTENTDATE).setType(ODCEdmCTTimeRange.CT_FQN).setNullable(false);
		
		// create CsdlPropertyRef for Key element
		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		propertyRef.setName(AUXIPProduct.FIELD_ID);

		CsdlNavigationProperty attributesNavProp = new CsdlNavigationProperty().setName(AUXIPProduct.FIELD_ATTRIBUTES)
				.setNullable(false).setType(ODCEdmETAttribute.ET_FQN).setContainsTarget(true).setCollection(true);
		List<CsdlNavigationProperty> navPropList = new ArrayList<CsdlNavigationProperty>();
		navPropList.add(attributesNavProp);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(id, name, contentType, contentLength, originDate, creationDate, 
				evictionDate, checksums, contentDate/*, attributes*/));
		entityType.setKey(Collections.singletonList(propertyRef));
		entityType.setNavigationProperties(navPropList);

		entityType.setHasStream(true);

		return entityType;
	}
}
