package com.exprivia.odc.service.processor;

import java.io.InputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;

import org.apache.olingo.commons.api.data.ContextURL;
import org.apache.olingo.commons.api.data.ContextURL.Builder;
import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Link;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.apache.olingo.commons.api.edm.EdmElement;
import org.apache.olingo.commons.api.edm.EdmEntitySet;
import org.apache.olingo.commons.api.edm.EdmEntityType;
import org.apache.olingo.commons.api.edm.EdmNavigationProperty;
import org.apache.olingo.commons.api.edm.EdmNavigationPropertyBinding;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.format.ContentType;
import org.apache.olingo.commons.api.http.HttpHeader;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.ODataLibraryException;
import org.apache.olingo.server.api.ODataRequest;
import org.apache.olingo.server.api.ODataResponse;
import org.apache.olingo.server.api.ServiceMetadata;
import org.apache.olingo.server.api.deserializer.ODataDeserializer;
import org.apache.olingo.server.api.prefer.Preferences.Return;
import org.apache.olingo.server.api.prefer.PreferencesApplied;
import org.apache.olingo.server.api.processor.EntityProcessor;
import org.apache.olingo.server.api.processor.MediaEntityProcessor;
import org.apache.olingo.server.api.serializer.EntitySerializerOptions;
import org.apache.olingo.server.api.serializer.ODataSerializer;
import org.apache.olingo.server.api.serializer.SerializerException;
import org.apache.olingo.server.api.serializer.SerializerResult;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.api.uri.UriParameter;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceEntitySet;
import org.apache.olingo.server.api.uri.UriResourceNavigation;
import org.apache.olingo.server.api.uri.queryoption.ExpandItem;
import org.apache.olingo.server.api.uri.queryoption.ExpandOption;
import org.apache.olingo.server.api.uri.queryoption.SelectOption;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessResourceFailureException;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.StorageDelegate;
import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.service.ODCJsonSerializer;
import com.exprivia.odc.service.ODCReturnedEntity;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponse;
import com.exprivia.odc.util.Util;

import lombok.NoArgsConstructor;

@NoArgsConstructor
@Component
public class ODCEntityProcessor implements EntityProcessor, MediaEntityProcessor {

	private OData odata;
	private ServiceMetadata serviceMetadata;

	private static final Logger LOG = LoggerFactory.getLogger(ODCEntityProcessor.class);

	@Autowired
	private StorageDelegate storage;

	@Override
	public void init(OData odata, ServiceMetadata serviceMetadata) {
		this.odata = odata;
		this.serviceMetadata = serviceMetadata;
	}

	@Override
	public void readEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType responseFormat)
			throws ODataApplicationException, SerializerException {
		try {
			readEntityInternal(request, response, uriInfo, responseFormat);
		} catch (ODCApiUnauthorizedException e) {
			throw Util.createUnauthorizedODataApplicationException(e);
		} catch (DataAccessResourceFailureException e) {
			if (e.getMessage().contains("canceling statement due to user request"))
				throw Util.createSqlTimeoutODataApplicationException(e);
			else
				throw e;
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
	
	public void readEntityInternal(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType responseFormat)
			throws ODataApplicationException, SerializerException {

		// 1. retrieve the Entity Type
		List<UriResource> resourcePaths = uriInfo.getUriResourceParts();

		Iterator<UriResource> iterator = resourcePaths.iterator();
		UriResource res = null;
		Entity entity = null;
		EdmEntitySet edmEntitySet = null;
		EdmEntityType entityType = null;

		while (iterator.hasNext()) {
			res = iterator.next();
			if (res instanceof UriResourceEntitySet) {
				edmEntitySet = ((UriResourceEntitySet) res).getEntitySet();
				entityType = edmEntitySet.getEntityType();

				// 2. retrieve the data from backend
				List<UriParameter> keyPredicates = ((UriResourceEntitySet) res).getKeyPredicates();

				entity = storage.readEntityData(edmEntitySet, keyPredicates);
			} else if (res instanceof UriResourceNavigation) {
				UriResourceNavigation uriResourceNavigation = (UriResourceNavigation) res;
				List<UriParameter> keyPredicates = uriResourceNavigation.getKeyPredicates();
				EdmNavigationProperty edmNavigationProperty = uriResourceNavigation.getProperty();
				entityType = edmNavigationProperty.getType();

				// 2. retrieve the data from backend
				if (keyPredicates.isEmpty()) {
					// e.g. DemoService.svc/Products(1)/Category
					entity = storage.getRelatedEntity(entity, entityType);
				} else { // e.g. DemoService.svc/Categories(3)/Products(5)
					entity = storage.getRelatedEntity(entity, entityType, keyPredicates);
				}

				edmEntitySet = Util.getNavigationTargetEntitySet(uriInfo);
			}

		}

		// $expand
		ExpandOption expandOption = uriInfo.getExpandOption();
		if (expandOption != null) {
			handleExpandOptions(entity, expandOption, edmEntitySet);
		}

		// 3rd: apply system query options
		// Note: $select is handled by the lib, we only configure ContextURL + SerializerOptions
		// for performance reasons, it might be necessary to implement the $select manually
		SelectOption selectOption = uriInfo.getSelectOption();

		// 3. serialize
		ContextURL contextUrl = Util.createContextUrl(uriInfo);
		// expand and select currently not supported
		EntitySerializerOptions options = EntitySerializerOptions.with().contextURL(contextUrl).expand(expandOption).select(selectOption).build();

		ODataSerializer serializer = ODCJsonSerializer.createSerializer(this.odata, responseFormat);
		SerializerResult serializerResult = serializer.entity(serviceMetadata, entityType, entity, options);
		InputStream entityStream = serializerResult.getContent();

		//4. configure the response object
		response.setContent(entityStream);
		response.setStatusCode(HttpStatusCode.OK.getStatusCode());
		response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
	}

	private void handleExpandOptions(Entity entity, ExpandOption expandOption, EdmEntitySet edmEntitySet) {
		for (ExpandItem expandItem : expandOption.getExpandItems()) {
			handleExpandItem(entity, expandItem, edmEntitySet);
		}
	}

	private void handleExpandItem(Entity entity, ExpandItem expandItem, EdmEntitySet edmEntitySet) {
		if (expandItem.isStar()) {
			List<EdmNavigationPropertyBinding> bindings = edmEntitySet.getNavigationPropertyBindings();
			// we know that there are navigation bindings
			// however normally in this case a check if navigation bindings exists is done
			if (!bindings.isEmpty()) {
				for (EdmNavigationPropertyBinding binding : bindings) {
					EdmElement property = edmEntitySet.getEntityType().getProperty(binding.getPath());
					// we don't need to handle error cases, as it is done in the Olingo library
					if (property instanceof EdmNavigationProperty) {
						EdmNavigationProperty edmNavigationProperty = (EdmNavigationProperty) property;
	
						fillExpandedEntity(entity, edmNavigationProperty.getType(), edmNavigationProperty.getName());
					}
				}
			}
		} else {
			UriResource uriResource = expandItem.getResourcePath().getUriResourceParts().get(0);
			// we don't need to handle error cases, as it is done in the Olingo library
			if (uriResource instanceof UriResourceNavigation) {
				EdmNavigationProperty edmNavigationProperty = ((UriResourceNavigation) uriResource).getProperty();

				fillExpandedEntity(entity, edmNavigationProperty.getType(), edmNavigationProperty.getName());
			}
		}

	}
	
	private void fillExpandedEntity(Entity mainEntity, EdmEntityType expandedEntityType, String expandedEntityTitle) {
		Link link = new Link();
		link.setTitle(expandedEntityTitle);
		Entity expandEntity = storage.getRelatedEntity(mainEntity, expandedEntityType);
		if (expandEntity != null) {
			link.setInlineEntity(expandEntity);
		} else {
			EntityCollection expandEntityColl = storage.getRelatedEntities(mainEntity, expandedEntityType, null, null, false);
			link.setInlineEntitySet(expandEntityColl);
		}

		mainEntity.getNavigationLinks().add(link);
	}

	public void createEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat, ContentType responseFormat)
			throws ODataApplicationException, ODataLibraryException {
		try {
			createEntityInternal(request, response, uriInfo, requestFormat, responseFormat);
		} catch (ODCApiUnauthorizedException e) {
			throw Util.createUnauthorizedODataApplicationException(e);
		} catch (Throwable e) {
			e.printStackTrace();
			LOG.error(e.getClass().getName() + ": " + e.getMessage());
			throw e;
		}
	}
	
	public void createEntityInternal(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat, ContentType responseFormat)
			throws ODataApplicationException, ODataLibraryException {

		final ODataDeserializer deserializer = odata.createDeserializer(requestFormat);

		final List<UriResource> resourcePaths = uriInfo.asUriInfoResource().getUriResourceParts();
		EdmEntitySet edmEntitySet = ((UriResourceEntitySet)resourcePaths.get(0)).getEntitySet();
		FullQualifiedName fqn = edmEntitySet.getEntityType().getFullQualifiedName();
		EdmEntityType entityType = serviceMetadata.getEdm().getEntityType(fqn);
		
		// get entity from payload
		Entity newEntity = deserializer.entity(request.getBody(), entityType).getEntity();

		// actually create the Entity
		ODCReturnedEntity returnEntity = storage.processCreateEntity(newEntity);
		
		// handle return payload
		final Return returnPreference = odata.createPreferences(request.getHeaders(HttpHeader.PREFER)).getReturn();
		
		if (returnPreference == null || returnPreference == Return.REPRESENTATION) {
			response.setContent(ODCJsonSerializer.createSerializer(odata, responseFormat)
					.entity(
							serviceMetadata, 
							entityType, 
							returnEntity.getEntity(), 
							EntitySerializerOptions.with().contextURL(
									isODataMetadataNone(responseFormat) 
									? null : getContextUrl(edmEntitySet, entityType, true))
							.build())
					.getContent());
			response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
			response.setStatusCode((returnEntity.isCreated() ? HttpStatusCode.CREATED : HttpStatusCode.OK).getStatusCode());
		} else {
			response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
		}
		if (returnPreference != null) {
			response.setHeader(HttpHeader.PREFERENCE_APPLIED,
					PreferencesApplied.with().returnRepresentation(returnPreference).build().toValueString());
		}
		if (returnEntity.isCreated()) {
			final String location = edmEntitySet != null ?
					request.getRawBaseUri() + '/' + odata.createUriHelper().buildCanonicalURL(edmEntitySet, returnEntity.getEntity())
					: "";
			response.setHeader(HttpHeader.LOCATION, location);
			if (returnPreference == Return.MINIMAL) {
				response.setHeader(HttpHeader.ODATA_ENTITY_ID, location);
			}
		}
		if (returnEntity.getEntity().getETag() != null) {
			response.setHeader(HttpHeader.ETAG, returnEntity.getEntity().getETag());
		}		
	}

	public void updateEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat, ContentType responseFormat)
			throws ODataApplicationException, ODataLibraryException {
		final ODataDeserializer deserializer = odata.createDeserializer(requestFormat);

		final List<UriResource> resourcePaths = uriInfo.asUriInfoResource().getUriResourceParts();
		UriResourceEntitySet resourceEntitySet = (UriResourceEntitySet)resourcePaths.get(0);
		EdmEntitySet edmEntitySet = resourceEntitySet.getEntitySet();
		FullQualifiedName fqn = edmEntitySet.getEntityType().getFullQualifiedName();
		EdmEntityType entityType = serviceMetadata.getEdm().getEntityType(fqn);
		
		// get entity from payload
		Entity entity = deserializer.entity(request.getBody(), entityType).getEntity();
		
		// set the key predicates
		for (UriParameter key : resourceEntitySet.getKeyPredicates()) {
			Property keyProp = new Property();
			keyProp.setName(key.getName());
			keyProp.setValue(ValueType.PRIMITIVE, key.getText().replaceAll("^'|'$", ""));
			entity.addProperty(keyProp);
		}
		
		// actually create the Entity
		ODCReturnedEntity returnEntity = storage.processUpdateEntity(entity);
		
		// handle return payload
		if (returnEntity.getEntity() == null) {
			// no updated entity
			response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
			return;
		}
		
		final Return returnPreference = odata.createPreferences(request.getHeaders(HttpHeader.PREFER)).getReturn();
		
		if (returnPreference == null || returnPreference == Return.REPRESENTATION) {
			response.setContent(ODCJsonSerializer.createSerializer(odata, responseFormat)
					.entity(serviceMetadata, entityType, returnEntity.getEntity(), EntitySerializerOptions.with().contextURL(
							isODataMetadataNone(responseFormat) ? null : 
								getContextUrl(edmEntitySet, entityType, true))
							.build())
					.getContent());
			response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
			response.setStatusCode((returnEntity.isCreated() ? HttpStatusCode.CREATED : HttpStatusCode.OK).getStatusCode());
		} else {
			response.setStatusCode(HttpStatusCode.NO_CONTENT.getStatusCode());
		}
		if (returnPreference != null) {
			response.setHeader(HttpHeader.PREFERENCE_APPLIED,
					PreferencesApplied.with().returnRepresentation(returnPreference).build().toValueString());
		}
		if (returnEntity.isCreated()) {
			final String location = edmEntitySet != null ?
					request.getRawBaseUri() + '/' + odata.createUriHelper().buildCanonicalURL(edmEntitySet, returnEntity.getEntity())
					: "";
			response.setHeader(HttpHeader.LOCATION, location);
			if (returnPreference == Return.MINIMAL) {
				response.setHeader(HttpHeader.ODATA_ENTITY_ID, location);
			}
		}
		if (returnEntity.getEntity().getETag() != null) {
			response.setHeader(HttpHeader.ETAG, returnEntity.getEntity().getETag());
		}
	}

	public void deleteEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo) throws ODataApplicationException {
		throw new ODataApplicationException("Not supported.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ROOT);
	}

	@Override
	public void readMediaEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType responseFormat)
			throws ODataApplicationException, ODataLibraryException {
		// Since our scenario do not contain navigations from media entities. We can keep things simple and
		// check only the first resource path of the URI.
		final UriResource firstResoucePart = uriInfo.getUriResourceParts().get(0);
		if (firstResoucePart instanceof UriResourceEntitySet) {
			final EdmEntitySet edmEntitySet = Util.getEdmEntitySet(uriInfo);
			final UriResourceEntitySet uriResourceEntitySet = (UriResourceEntitySet) firstResoucePart;

			ODCMediaResponse mediaResponse = storage.readEntityMedia(edmEntitySet, uriResourceEntitySet.getKeyPredicates());
			if (mediaResponse == null) {
				throw new ODataApplicationException("No media content available", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
			}

			mediaResponse.setODataResponse(response);
		} else {
			throw new ODataApplicationException("Not implemented", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
		}
	}

	@Override
	public void createMediaEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {
		// TODO Auto-generated method stub
		throw new ODataApplicationException("Not supported.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ROOT);
	}

	@Override
	public void updateMediaEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {
		// TODO Auto-generated method stub
		throw new ODataApplicationException("Not supported.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ROOT);
	}

	@Override
	public void deleteMediaEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo)
			throws ODataApplicationException, ODataLibraryException {
		// TODO Auto-generated method stub
		throw new ODataApplicationException("Not supported.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ROOT);
	}
	
/*
	@Override
	public void processActionEntity(ODataRequest request, ODataResponse response, UriInfo uriInfo, ContentType requestFormat,
			ContentType responseFormat) throws ODataApplicationException, ODataLibraryException {
		// TODO Auto-generated method stub
		throw new ODataApplicationException("Not yet implemented.", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ROOT);
	}
	*/

	//This method fetches the context URL
	private ContextURL getContextUrl(final EdmEntitySet entitySet, final EdmEntityType entityType, final boolean isSingleEntity)
			throws ODataLibraryException {
		Builder builder = ContextURL.with();
//		builder.entitySet(entitySet);
		builder.type(entityType);
		//builder = entitySet == null ? isSingleEntity ? builder.type(entityType) : builder.asCollection().type(entityType)
//				: builder.entitySet(entitySet);
//		builder = builder.suffix(isSingleEntity && entitySet != null ? Suffix.ENTITY : null);
		return builder.build();
	}

	protected boolean isODataMetadataNone(final ContentType contentType) {
		return contentType.isCompatible(ContentType.APPLICATION_JSON)
				&& ContentType.VALUE_ODATA_METADATA_NONE.equalsIgnoreCase(contentType.getParameter(ContentType.PARAMETER_ODATA_METADATA));
	}
}
