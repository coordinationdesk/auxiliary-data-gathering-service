package com.exprivia.odc.service.processor;

import java.io.InputStream;
import java.util.List;
import java.util.Locale;

import org.apache.olingo.commons.api.data.ContextURL;
import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.edm.EdmComplexType;
import org.apache.olingo.commons.api.edm.EdmEntitySet;
import org.apache.olingo.commons.api.edm.EdmProperty;
import org.apache.olingo.commons.api.format.ContentType;
import org.apache.olingo.commons.api.http.HttpHeader;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.ODataLibraryException;
import org.apache.olingo.server.api.ODataRequest;
import org.apache.olingo.server.api.ODataResponse;
import org.apache.olingo.server.api.ServiceMetadata;
import org.apache.olingo.server.api.processor.ComplexCollectionProcessor;
import org.apache.olingo.server.api.serializer.ComplexSerializerOptions;
import org.apache.olingo.server.api.serializer.ODataSerializer;
import org.apache.olingo.server.api.serializer.SerializerResult;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.api.uri.UriParameter;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceEntitySet;
import org.apache.olingo.server.api.uri.UriResourceProperty;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.StorageDelegate;
import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.service.ODCJsonSerializer;
import com.exprivia.odc.util.Util;

import lombok.NoArgsConstructor;

@NoArgsConstructor
@Component
public class ODCComplexCollectionProcessor implements ComplexCollectionProcessor {
	
	private OData odata;
	private ServiceMetadata serviceMetadata;
	
	private static final Logger LOG = LoggerFactory.getLogger(ODCComplexCollectionProcessor.class);

	@Autowired
	private StorageDelegate storage;
	
	@Override
	public void init(OData odata, ServiceMetadata serviceMetadata) {
		this.odata = odata;
		this.serviceMetadata = serviceMetadata;
	}

	@Override
	public void readComplexCollection(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType responseFormat)
			throws ODataApplicationException, ODataLibraryException {
		try {
			readComplexCollectionInternal(request, response, uriInfo, responseFormat);
		} catch (ODCApiUnauthorizedException e) {
			throw Util.createUnauthorizedODataApplicationException(e);
		} catch (ODataApplicationException e) {
			if (e.getStatusCode() == 404 ||
				e.getStatusCode() == 429) {
				// minor errors handling
				LOG.warn(e.getMessage());
				throw e;
			} else {
				e.printStackTrace();
				LOG.error(e.getClass().getName() + ": " + e.getMessage());
				throw e;
			}
		} catch (Throwable e) {
			e.printStackTrace();
			LOG.error(e.getClass().getName() + ": " + e.getMessage());
			throw e;
		}
	}
	
	public void readComplexCollectionInternal(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType responseFormat)
			throws ODataApplicationException, ODataLibraryException {
		// 1. Retrieve info from URI
		// 1.1. retrieve the info about the requested entity set
		List<UriResource> resourceParts = uriInfo.getUriResourceParts();
		// Note: only in our example we can rely that the first segment is the EntitySet
		UriResourceEntitySet uriEntityset = (UriResourceEntitySet) resourceParts.get(0);
		EdmEntitySet edmEntitySet = uriEntityset.getEntitySet();
		// the key for the entity
		List<UriParameter> keyPredicates = uriEntityset.getKeyPredicates();

		// 1.2. retrieve the requested (Edm) property
		// the last segment is the Property
		UriResourceProperty uriProperty = (UriResourceProperty) resourceParts.get(resourceParts.size() - 1);
		EdmProperty edmProperty = uriProperty.getProperty();
		String edmPropertyName = edmProperty.getName();
		// in our example, we know we have only primitive types in our model
		EdmComplexType edmPropertyType = (EdmComplexType) edmProperty.getType();

		// 2. retrieve data from backend
		// 2.1. retrieve the entity data, for which the property has to be read
		Entity entity = storage.readEntityData(edmEntitySet, keyPredicates);
		if (entity == null) { // Bad request
			throw new ODataApplicationException("Entity not found", HttpStatusCode.NOT_FOUND.getStatusCode(),
					Locale.ENGLISH);
		}

		// 2.2. retrieve the property data from the entity
		Property property = entity.getProperty(edmPropertyName);
		if (property == null) {
			throw new ODataApplicationException("Property not found", HttpStatusCode.NOT_FOUND.getStatusCode(),
					Locale.ENGLISH);
		}

		// 3. serialize
		Object value = property.getValue();
		if (value != null) {
			// 3.1. configure the serializer
			ODataSerializer serializer = ODCJsonSerializer.createSerializer(this.odata, responseFormat);

			ContextURL contextUrl = ContextURL.with().entitySet(edmEntitySet).navOrPropertyPath(edmPropertyName)
					.build();
			ComplexSerializerOptions options = ComplexSerializerOptions.with().contextURL(contextUrl).build();
			// 3.2. serialize
			SerializerResult serializerResult = serializer.complexCollection(serviceMetadata, edmPropertyType, property,
					options);
			InputStream propertyStream = serializerResult.getContent();

			//4. configure the response object
			response.setContent(propertyStream);
			response.setStatusCode(HttpStatusCode.OK.getStatusCode());
			response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
		} else {
			// in case there's no value for the property, we can skip the serialization
			response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
		}
	}

	@Override
	public void updateComplexCollection(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {
		// TODO Auto-generated method stub
		throw new ODataApplicationException("Not supported.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(),
				Locale.ROOT);
	}

	@Override
	public void deleteComplexCollection(ODataRequest request, ODataResponse response, UriInfo uriInfo)
			throws ODataApplicationException, ODataLibraryException {
		// TODO Auto-generated method stub
		throw new ODataApplicationException("Not supported.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(),
				Locale.ROOT);
	}

}
