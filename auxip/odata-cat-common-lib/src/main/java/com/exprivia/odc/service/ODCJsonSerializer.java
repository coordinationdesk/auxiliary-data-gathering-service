package com.exprivia.odc.service;

import java.io.IOException;
import java.sql.Timestamp;
import java.util.List;
import java.util.Set;

import org.apache.olingo.commons.api.constants.Constantsv00;
import org.apache.olingo.commons.api.data.Linked;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.edm.EdmPrimitiveType;
import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeException;
import org.apache.olingo.commons.api.edm.EdmProperty;
import org.apache.olingo.commons.api.edm.geo.LineString;
import org.apache.olingo.commons.api.edm.geo.Polygon;
import org.apache.olingo.commons.api.format.ContentType;
import org.apache.olingo.commons.core.edm.primitivetype.EdmDateTimeOffset;
import org.apache.olingo.commons.core.edm.primitivetype.EdmGeographyLineString;
import org.apache.olingo.commons.core.edm.primitivetype.EdmGeographyPolygon;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ServiceMetadata;
import org.apache.olingo.server.api.serializer.ODataSerializer;
import org.apache.olingo.server.api.serializer.SerializerException;
import org.apache.olingo.server.api.uri.queryoption.ExpandOption;
import org.apache.olingo.server.core.serializer.json.ODataJsonSerializer;

import com.exprivia.odc.util.Util;
import com.fasterxml.jackson.core.JsonGenerator;

public class ODCJsonSerializer extends ODataJsonSerializer {

	static public ODataSerializer createSerializer(OData odata, ContentType responseFormat) throws SerializerException {
		if (responseFormat.isCompatible(ContentType.APPLICATION_JSON)) {
		      final String metadata = responseFormat.getParameter(ContentType.PARAMETER_ODATA_METADATA);
		      if (metadata == null
		          || ContentType.VALUE_ODATA_METADATA_MINIMAL.equalsIgnoreCase(metadata)
		          || ContentType.VALUE_ODATA_METADATA_NONE.equalsIgnoreCase(metadata)
		          || ContentType.VALUE_ODATA_METADATA_FULL.equalsIgnoreCase(metadata)) {
		        return new ODCJsonSerializer(responseFormat, new Constantsv00());
		      }
		}
		return odata.createSerializer(responseFormat);
	}
	
	public ODCJsonSerializer(ContentType contentType) {
		super(contentType);
	}

	public ODCJsonSerializer(ContentType responseFormat, Constantsv00 constantsv00) {
		super(responseFormat, constantsv00);
	}

	protected void writeProperty(final ServiceMetadata metadata,
		      final EdmProperty edmProperty, final Property property,
		      final Set<List<String>> selectedPaths, final JsonGenerator json, 
		      Set<List<String>> expandedPaths, Linked linked, ExpandOption expand)
		      throws IOException, SerializerException {
		if (property == null && edmProperty.isNullable()) {
			/*
			 * if value is null and property is null-able, don't write anything
			 * and skip also the property name
			 */
		} else {
			super.writeProperty(metadata, edmProperty, property, selectedPaths, json, expandedPaths, linked, expand);
		}
	}
	
	protected void writePrimitiveValue(final String name, final EdmPrimitiveType type, final Object primitiveValue,
		      final Boolean isNullable, final Integer maxLength, final Integer precision, final Integer scale,
		      final Boolean isUnicode, final JsonGenerator json) throws EdmPrimitiveTypeException, IOException {
		if (primitiveValue instanceof Timestamp &&
			type instanceof EdmDateTimeOffset) {
			json.writeString(Util.formatTimestamp((Timestamp) primitiveValue));
		} else if (primitiveValue instanceof LineString) {
			super.writePrimitiveValue(name, new EdmGeographyLineString(), primitiveValue, isNullable, maxLength, precision, scale, isUnicode, json);
		} else if (primitiveValue instanceof Polygon) {
			super.writePrimitiveValue(name, new EdmGeographyPolygon(), primitiveValue, isNullable, maxLength, precision, scale, isUnicode, json);
		} else if (primitiveValue instanceof ODCJsonPristineValue) {
			// special case, a value is a fragment of Json data, output as is
			json.writeRaw(":" + ((ODCJsonPristineValue)primitiveValue).getJsonPayload());
		} else {
			super.writePrimitiveValue(name, type, primitiveValue, isNullable, maxLength, precision, scale, isUnicode, json);
		}
	}
	
	public static final class ODCJsonPristineValue {
		private String jsonPayload;
		
		public ODCJsonPristineValue(String jsonPayload) {
			this.jsonPayload = jsonPayload;
		}
		
		public String getJsonPayload() {
			return jsonPayload;
		}
	}
}