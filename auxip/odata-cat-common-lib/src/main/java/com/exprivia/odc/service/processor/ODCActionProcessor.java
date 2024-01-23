package com.exprivia.odc.service.processor;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import org.apache.olingo.commons.api.data.ContextURL;
import org.apache.olingo.commons.api.data.ContextURL.Builder;
import org.apache.olingo.commons.api.data.ContextURL.Suffix;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Parameter;
import org.apache.olingo.commons.api.edm.EdmAction;
import org.apache.olingo.commons.api.edm.EdmActionImport;
import org.apache.olingo.commons.api.edm.EdmEntitySet;
import org.apache.olingo.commons.api.edm.EdmEntityType;
import org.apache.olingo.commons.api.format.ContentType;
import org.apache.olingo.commons.api.http.HttpHeader;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.ODataLibraryException;
import org.apache.olingo.server.api.ODataRequest;
import org.apache.olingo.server.api.ODataResponse;
import org.apache.olingo.server.api.ServiceMetadata;
import org.apache.olingo.server.api.deserializer.DeserializerException;
import org.apache.olingo.server.api.deserializer.ODataDeserializer;
import org.apache.olingo.server.api.prefer.Preferences.Return;
import org.apache.olingo.server.api.prefer.PreferencesApplied;
import org.apache.olingo.server.api.processor.ActionEntityCollectionProcessor;
import org.apache.olingo.server.api.processor.ActionEntityProcessor;
import org.apache.olingo.server.api.processor.ActionVoidProcessor;
import org.apache.olingo.server.api.serializer.EntityCollectionSerializerOptions;
import org.apache.olingo.server.api.serializer.EntitySerializerOptions;
import org.apache.olingo.server.api.serializer.ODataSerializer;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceAction;
import org.apache.olingo.server.api.uri.UriResourceEntitySet;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.StorageDelegate;
import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.service.ODCJsonSerializer;
import com.exprivia.odc.service.ODCReturnedEntity;
import com.exprivia.odc.util.Util;

import lombok.NoArgsConstructor;

@NoArgsConstructor
@Component
public class ODCActionProcessor implements ActionEntityProcessor, ActionEntityCollectionProcessor, ActionVoidProcessor {

	private OData odata;
	private ServiceMetadata serviceMetadata;
	
	private static final Logger LOG = LoggerFactory.getLogger(ODCActionProcessor.class);

	@Autowired
	private StorageDelegate storage;
	
	@Override
	public void init(OData odata, ServiceMetadata serviceMetadata) {
		this.odata = odata;
		this.serviceMetadata = serviceMetadata;
	}

	@Override
	public void processActionEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {
		try {
			processActionEntityInternal(request, response, uriInfo, requestFormat, responseFormat);
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

	public void processActionEntityInternal(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {

		EdmAction action = null;
		UriResourceEntitySet boundEntity = null;
		EdmEntitySet edmEntitySet = null;
		
		// DemoEntityActionResult is a custom object that holds the entity and the status 
		// as to whether the entity is created or just returned. This information is used 
		// to set the response status
		ODCReturnedEntity entityResult = null;
		if (requestFormat == null) {
			throw new ODataApplicationException("The content type has not been set in the request.", HttpStatusCode.BAD_REQUEST.getStatusCode(),
					Locale.ROOT);
		}

		final ODataDeserializer deserializer = odata.createDeserializer(requestFormat);
		final List<UriResource> resourcePaths = uriInfo.asUriInfoResource().getUriResourceParts();

		if (resourcePaths.get(0) instanceof UriResourceAction) {
			// unbound action that is called statically
			EdmActionImport actionImport = ((UriResourceAction)resourcePaths.get(0)).getActionImport();
			edmEntitySet = actionImport.getEntityContainer().getEntitySet(actionImport.getFullQualifiedName().toString());
			action = actionImport.getUnboundAction();
			Map<String, Parameter> parameters = parseParameters(request.getBody(), action, deserializer);
			entityResult = storage.processBoundActionEntity(action, parameters, null);
		} else {
			// bound action, with an invocation Entity
			boundEntity = (UriResourceEntitySet) resourcePaths.get(0);
			edmEntitySet = boundEntity.getEntitySet();
			if (resourcePaths.size() > 1) {
				// Checks if there is a navigation segment added after the binding parameter
				action = ((UriResourceAction) resourcePaths.get(1)).getAction();
				Map<String, Parameter> parameters = parseParameters(request.getBody(), action, deserializer);
				if (resourcePaths.get(0) instanceof UriResourceEntitySet ||
					resourcePaths.get(1) instanceof UriResourceAction) {
					entityResult = storage.processBoundActionEntity(action, parameters, boundEntity.getKeyPredicates());
				}
			}
		}
		
		EdmEntityType type = null;
		if (action.getReturnType() != null)
			type = (EdmEntityType) action.getReturnType().getType();
		
		if (entityResult == null || entityResult.getEntity() == null) {
			if (action.getReturnType() == null || action.getReturnType().isNullable()) {
				response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
			} else {
				// Not nullable return type so we have to give back a 500
				throw new ODataApplicationException("The action could not be executed.", HttpStatusCode.INTERNAL_SERVER_ERROR.getStatusCode(),
						Locale.ROOT);
			}
		} else {
			final Return returnPreference = odata.createPreferences(request.getHeaders(HttpHeader.PREFER)).getReturn();
			if (returnPreference == null || returnPreference == Return.REPRESENTATION) {
				ODataSerializer serializer = ODCJsonSerializer.createSerializer(this.odata, responseFormat);
				response.setContent(serializer
						.entity(serviceMetadata, type, entityResult.getEntity(), EntitySerializerOptions.with().contextURL(
								isODataMetadataNone(responseFormat) ? null : getContextUrl(action.getReturnedEntitySet(edmEntitySet), type, true))
								.build())
						.getContent());
				response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
				response.setStatusCode((entityResult.isCreated() ? HttpStatusCode.CREATED : HttpStatusCode.OK).getStatusCode());
			} else {
				response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
			}
			if (returnPreference != null) {
				response.setHeader(HttpHeader.PREFERENCE_APPLIED,
						PreferencesApplied.with().returnRepresentation(returnPreference).build().toValueString());
			}
			if (entityResult.isCreated()) {
				final String location = edmEntitySet != null ?
						request.getRawBaseUri() + '/' + odata.createUriHelper().buildCanonicalURL(edmEntitySet, entityResult.getEntity())
						: "";
				response.setHeader(HttpHeader.LOCATION, location);
				if (returnPreference == Return.MINIMAL) {
					response.setHeader(HttpHeader.ODATA_ENTITY_ID, location);
				}
			}
			if (entityResult.getEntity().getETag() != null) {
				response.setHeader(HttpHeader.ETAG, entityResult.getEntity().getETag());
			}
		}
	}
	
	@Override
	public void processActionEntityCollection(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {

		Map<String, Parameter> parameters = new HashMap<String, Parameter>();
		EdmAction action = null;
		EntityCollection collection = null;

		if (requestFormat == null) {
			throw new ODataApplicationException("The content type has not been set in the request.", HttpStatusCode.BAD_REQUEST.getStatusCode(),
					Locale.ROOT);
		}

		List<UriResource> resourcePaths = uriInfo.asUriInfoResource().getUriResourceParts();
		final ODataDeserializer deserializer = odata.createDeserializer(requestFormat);
		UriResourceEntitySet boundEntitySet = (UriResourceEntitySet) resourcePaths.get(0);
		if (resourcePaths.size() > 1) {
			// Check if there is a navigation segment added after the bound parameter
			if (resourcePaths.get(1) instanceof UriResourceAction) {
//				action = ((UriResourceAction) resourcePaths.get(2)).getAction();
//				throw new ODataApplicationException("Action " + action.getName() + " is not yet implemented.",
//						HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
				action = ((UriResourceAction) resourcePaths.get(1)).getAction();
				parameters = deserializer.actionParameters(request.getBody(), action).getActionParameters();
				collection = storage.processBoundActionEntityCollection(action, parameters, boundEntitySet.getKeyPredicates());
			} else {
				action = ((UriResourceAction) resourcePaths.get(1)).getAction();
				parameters = deserializer.actionParameters(request.getBody(), action).getActionParameters();
				collection = storage.processBoundActionEntityCollection(action, parameters, boundEntitySet.getKeyPredicates());
			}
		}
		// Collections must never be null.
		// Not nullable return types must not contain a null value.
		if (collection == null || collection.getEntities().contains(null) && !action.getReturnType().isNullable()) {
			throw new ODataApplicationException("The action could not be executed.", HttpStatusCode.INTERNAL_SERVER_ERROR.getStatusCode(),
					Locale.ROOT);
		}

		final Return returnPreference = odata.createPreferences(request.getHeaders(HttpHeader.PREFER)).getReturn();
		if (returnPreference == null || returnPreference == Return.REPRESENTATION) {
			final EdmEntitySet edmEntitySet = boundEntitySet.getEntitySet();
			final EdmEntityType type = (EdmEntityType) action.getReturnType().getType();
			final EntityCollectionSerializerOptions options = EntityCollectionSerializerOptions.with()
					.contextURL(isODataMetadataNone(responseFormat) ? null : getContextUrl(action.getReturnedEntitySet(edmEntitySet), type, false))
					.build();
			response.setContent(ODCJsonSerializer.createSerializer(odata, responseFormat).entityCollection(serviceMetadata, type, collection, options).getContent());
			response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
			response.setStatusCode(HttpStatusCode.OK.getStatusCode());
		} else {
			response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
		}
		if (returnPreference != null) {
			response.setHeader(HttpHeader.PREFERENCE_APPLIED,
					PreferencesApplied.with().returnRepresentation(returnPreference).build().toValueString());
		}
	}

	@Override
	public void processActionVoid(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat)
			throws ODataApplicationException, ODataLibraryException {
		try {
			processActionEntityInternal(request, response, uriInfo, requestFormat, null);
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

	//This method fetches the context URL
	private ContextURL getContextUrl(final EdmEntitySet entitySet, final EdmEntityType entityType, final boolean isSingleEntity)
			throws ODataLibraryException {
		Builder builder = ContextURL.with();
		builder = entitySet == null ? isSingleEntity ? builder.type(entityType) : builder.asCollection().type(entityType)
				: builder.entitySet(entitySet);
		builder = builder.suffix(isSingleEntity && entitySet != null ? Suffix.ENTITY : null);
		return builder.build();
	}

	protected boolean isODataMetadataNone(final ContentType contentType) {
		return contentType.isCompatible(ContentType.APPLICATION_JSON)
				&& ContentType.VALUE_ODATA_METADATA_NONE.equalsIgnoreCase(contentType.getParameter(ContentType.PARAMETER_ODATA_METADATA));
	}
	
	protected Map<String, Parameter> parseParameters(InputStream stream, EdmAction action, ODataDeserializer deserializer) throws DeserializerException {
		Map<String, Parameter> parameters = null;
		try {
			if (stream.available() > 0)
				parameters = deserializer.actionParameters(stream, action).getActionParameters();
			else
				parameters = new HashMap<String, Parameter>();
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
		return parameters;
	}

}
