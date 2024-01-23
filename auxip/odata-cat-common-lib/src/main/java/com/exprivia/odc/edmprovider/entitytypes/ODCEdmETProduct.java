package com.exprivia.odc.edmprovider.entitytypes;

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

import com.exprivia.odc.data.dto.Product;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.complextypes.ODCEdmCTCheckSum;
import com.exprivia.odc.edmprovider.complextypes.ODCEdmCTTimeRange;

public class ODCEdmETProduct implements ODCEdmEntityType {
	public static final String ET_NAME = "Product";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType() {
		//create EntityType properties
		CsdlProperty id = new CsdlProperty().setName(Product.FIELD_ID).setType(EdmPrimitiveTypeKind.Guid.getFullQualifiedName()).setNullable(false);
		CsdlProperty name = new CsdlProperty().setName(Product.FIELD_NAME).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty contentType = new CsdlProperty().setName(Product.FIELD_CONTENTTYPE).setType(EdmPrimitiveTypeKind.String.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty contentLength = new CsdlProperty().setName(Product.FIELD_CONTENTLENGTH)
				.setType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName()).setNullable(false);
		CsdlProperty originDate = new CsdlProperty().setName(Product.FIELD_ORIGINDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);
		CsdlProperty creationDate = new CsdlProperty().setName(Product.FIELD_PUBLICATIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);
		CsdlProperty modificationDate = new CsdlProperty().setName(Product.FIELD_MODIFICATIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);
		CsdlProperty online = new CsdlProperty().setName(Product.FIELD_ONLINE).setType(EdmPrimitiveTypeKind.Boolean.getFullQualifiedName())
				.setNullable(false);
		CsdlProperty evictionDate = new CsdlProperty().setName(Product.FIELD_EVICTIONDATE)
				.setType(EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName());
		CsdlProperty checksums = new CsdlProperty().setName(Product.FIELD_CHECKSUMS).setType(ODCEdmCTCheckSum.CT_FQN).setCollection(true)
				.setNullable(false);
		CsdlProperty contentDate = new CsdlProperty().setName(Product.FIELD_CONTENTDATE).setType(ODCEdmCTTimeRange.CT_FQN).setNullable(false);
		CsdlProperty footprint = new CsdlProperty().setName(Product.FIELD_FOOTPRINT).setType(EdmPrimitiveTypeKind.Geography.getFullQualifiedName());
		
		// create CsdlPropertyRef for Key element
		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		propertyRef.setName(Product.FIELD_ID);

		CsdlNavigationProperty attributesNavProp = new CsdlNavigationProperty().setName(Product.FIELD_ATTRIBUTES)
				.setNullable(false).setType(ODCEdmETAttribute.ET_FQN).setContainsTarget(true).setCollection(true);
		List<CsdlNavigationProperty> navPropList = new ArrayList<CsdlNavigationProperty>();
		navPropList.add(attributesNavProp);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();
		entityType.setName(ET_NAME);
		entityType.setProperties(Arrays.asList(id, name, contentType, contentLength, originDate, creationDate, modificationDate, online, evictionDate,
				checksums, contentDate, footprint/*, attributes*/));
		entityType.setKey(Collections.singletonList(propertyRef));
		entityType.setNavigationProperties(navPropList);

		entityType.setHasStream(true);

		return entityType;
	}
}
