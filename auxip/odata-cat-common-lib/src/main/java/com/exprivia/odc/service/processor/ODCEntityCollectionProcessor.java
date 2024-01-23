package com.exprivia.odc.service.processor;

import java.io.InputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;

import org.apache.olingo.commons.api.data.ContextURL;
import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Link;
import org.apache.olingo.commons.api.edm.EdmElement;
import org.apache.olingo.commons.api.edm.EdmEntitySet;
import org.apache.olingo.commons.api.edm.EdmEntityType;
import org.apache.olingo.commons.api.edm.EdmNavigationProperty;
import org.apache.olingo.commons.api.edm.EdmNavigationPropertyBinding;
import org.apache.olingo.commons.api.format.ContentType;
import org.apache.olingo.commons.api.http.HttpHeader;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.ODataRequest;
import org.apache.olingo.server.api.ODataResponse;
import org.apache.olingo.server.api.ServiceMetadata;
import org.apache.olingo.server.api.processor.EntityCollectionProcessor;
import org.apache.olingo.server.api.serializer.EntityCollectionSerializerOptions;
import org.apache.olingo.server.api.serializer.ODataSerializer;
import org.apache.olingo.server.api.serializer.SerializerException;
import org.apache.olingo.server.api.serializer.SerializerResult;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.api.uri.UriParameter;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceEntitySet;
import org.apache.olingo.server.api.uri.UriResourceNavigation;
import org.apache.olingo.server.api.uri.queryoption.CountOption;
import org.apache.olingo.server.api.uri.queryoption.ExpandItem;
import org.apache.olingo.server.api.uri.queryoption.ExpandOption;
import org.apache.olingo.server.api.uri.queryoption.FilterOption;
import org.apache.olingo.server.api.uri.queryoption.SelectOption;
import org.apache.olingo.server.api.uri.queryoption.SkipOption;
import org.apache.olingo.server.api.uri.queryoption.TopOption;
import org.apache.olingo.server.api.uri.queryoption.expression.Expression;
import org.apache.olingo.server.api.uri.queryoption.expression.ExpressionVisitException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessResourceFailureException;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.StorageDelegate;
import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.service.ODCJsonSerializer;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.filter.JDBCVisitor;
import com.exprivia.odc.util.Util;

import lombok.NoArgsConstructor;

@NoArgsConstructor
@Component
public class ODCEntityCollectionProcessor implements EntityCollectionProcessor {

	private OData odata;
	private ServiceMetadata serviceMetadata;
	
	private static final Logger LOG = LoggerFactory.getLogger(ODCEntityCollectionProcessor.class);

	@Autowired
	StorageDelegate storage;

	@Override
	public void init(OData odata, ServiceMetadata serviceMetadata) {
		this.odata = odata;
		this.serviceMetadata = serviceMetadata;
	}

	@Override
	public void readEntityCollection(ODataRequest request, ODataResponse response, UriInfo uriInfo,
			ContentType responseFormat) throws ODataApplicationException, SerializerException {
		try {
			readEntityCollectionInternal(request, response, uriInfo, responseFormat);
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
	
	protected void readEntityCollectionInternal(ODataRequest request, ODataResponse response, UriInfo uriInfo,
			ContentType responseFormat) throws ODataApplicationException, SerializerException {

		// 1st: retrieve the requested EntitySet from the uriInfo (representation of the parsed URI)
		List<UriResource> resourcePaths = uriInfo.getUriResourceParts();
		// in our example, the first segment is the EntitySet
		UriResourceEntitySet uriResourceEntitySet = (UriResourceEntitySet) resourcePaths.get(0);

		JDBCFilterExpression filterResult = null;

		// $count
		CountOption countOption = uriInfo.getCountOption();

		Integer offsetNumber = null;
		SkipOption skipOption = uriInfo.getSkipOption();
		if (skipOption != null) {
			offsetNumber = skipOption.getValue();
		}

		Integer limitNumber = null;
		TopOption topOption = uriInfo.getTopOption();
		if (topOption != null) {
			limitNumber = topOption.getValue();
		}

		EntityCollection returnEntityCollection = null;

		Iterator<UriResource> iterator = resourcePaths.iterator();
		UriResource res = null;
		EdmEntitySet edmEntitySet = uriResourceEntitySet.getEntitySet();
		EdmEntityType entityType = null;
		List<UriParameter> keyPredicates = null;
		
		while (iterator.hasNext()) {
			res = iterator.next();
			if (res instanceof UriResourceEntitySet) {
				keyPredicates = ((UriResourceEntitySet)res).getKeyPredicates();
				if (resourcePaths.size() == 1) { 
					edmEntitySet = ((UriResourceEntitySet)res).getEntitySet();
					entityType = edmEntitySet.getEntityType();
			
					filterResult = handleFilterResult(uriInfo, entityType);

					JDBCQueryParams queryParams = JDBCQueryParams.builder().jdbcFilterExpression(filterResult)
							.limitNumber(limitNumber).offsetNumber(offsetNumber).build();
					
					// 2. retrieve the data from backend
					List<String> orderByColumns = storage.orderByColumns(edmEntitySet, uriInfo.getOrderByOption());
					returnEntityCollection = storage.readEntitySetData(edmEntitySet, queryParams, orderByColumns, countOption != null ? countOption.getValue() : false);
				}
			} else if (res instanceof UriResourceNavigation) {
				UriResourceNavigation uriResourceNavigation = (UriResourceNavigation)res;
				EdmNavigationProperty edmNavigationProperty = uriResourceNavigation.getProperty();
				entityType = edmNavigationProperty.getType();
				
				filterResult = handleFilterResult(uriInfo, entityType);

				JDBCQueryParams queryParams = JDBCQueryParams.builder().jdbcFilterExpression(filterResult)
						.limitNumber(limitNumber).offsetNumber(offsetNumber).build();
				
				Entity sourceEntity = storage.readEntityData(edmEntitySet, keyPredicates);
				// 2. retrieve the data from backend
		        if(sourceEntity == null) {
		            throw new ODataApplicationException("Entity not found.", HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ROOT);
		        }
		        // then fetch the entity collection where the entity navigates to
		        List<String> orderByColumns = storage.orderByColumns(entityType, uriInfo.getOrderByOption());
		        returnEntityCollection = storage.getRelatedEntities(sourceEntity, entityType, queryParams, orderByColumns, countOption != null ? countOption.getValue() : false);
		        
		        edmEntitySet = Util.getNavigationTargetEntitySet(uriInfo);
			}
		}
		
		
		// $expand
		ExpandOption expandOption = uriInfo.getExpandOption();
		if (expandOption != null) {
			handleExpandOptions(returnEntityCollection, expandOption, edmEntitySet, uriInfo);
		}
		
		// 3rd: apply system query options
		// Note: $select is handled by the lib, we only configure ContextURL + SerializerOptions
		// for performance reasons, it might be necessary to implement the $select manually
		SelectOption selectOption = uriInfo.getSelectOption();
		
		// 4th: create a serializer based on the requested format (json)
		ODataSerializer serializer = ODCJsonSerializer.createSerializer(this.odata, responseFormat);

		// and serialize the content: transform from the EntitySet object to InputStream
		ContextURL contextUrl = Util.createContextUrl(uriInfo);

		final String id = request.getRawBaseUri() + "/" + edmEntitySet.getName();
		EntityCollectionSerializerOptions opts = EntityCollectionSerializerOptions.with().contextURL(contextUrl).id(id)
				.count(countOption).expand(expandOption).select(selectOption).build();

		SerializerResult serializerResult = serializer.entityCollection(serviceMetadata, entityType,
				returnEntityCollection, opts);

		InputStream serializedContent = serializerResult.getContent();

		// 5th: configure the response object: set the body, headers and status code
		response.setContent(serializedContent);
		response.setStatusCode(HttpStatusCode.OK.getStatusCode());
		response.setHeader(HttpHeader.CONTENT_TYPE, responseFormat.toContentTypeString());
	}
	
	private JDBCFilterExpression handleFilterResult(UriInfo uriInfo, EdmEntityType entityType) throws ODataApplicationException {
		return getFilter(uriInfo.getFilterOption(), entityType);
	}
	
	private JDBCFilterExpression getFilter(FilterOption filterOption, EdmEntityType entityType) throws ODataApplicationException {
		JDBCFilterExpression filterResult = null;
		
		// 1a . handle filter
		if (filterOption != null) {
			// Apply $filter system query option
			try {
				Expression filterExpression = filterOption.getExpression();
				filterResult = (JDBCFilterExpression) filterExpression.accept(
						new JDBCVisitor(
								storage.createStorageItem(entityType),
								this.serviceMetadata.getEdm()
								)
						);

			} catch (ExpressionVisitException e) {
				throw new ODataApplicationException("Exception in filter evaluation",
						HttpStatusCode.INTERNAL_SERVER_ERROR.getStatusCode(), Locale.ENGLISH);
			}
		}
		return filterResult;
	}
	
	private void handleExpandOptions(EntityCollection entityCollection, ExpandOption expandOption, EdmEntitySet edmEntitySet, UriInfo uriInfo) throws ODataApplicationException {
		for (ExpandItem expandItem: expandOption.getExpandItems()) {
			handleExpandItem(entityCollection, expandItem, edmEntitySet, uriInfo);
		}
	}
	
	private void handleExpandItem(EntityCollection entityCollection, ExpandItem expandItem, EdmEntitySet edmEntitySet, UriInfo uriInfo) throws ODataApplicationException {
		
		String navPropName = null;
		EdmEntityType expandEdmEntityType = null;
		
		if(expandItem.isStar()) {
			List<EdmNavigationPropertyBinding> bindings = edmEntitySet.getNavigationPropertyBindings();
			// we know that there are navigation bindings
			// however normally in this case a check if navigation bindings exists is done
			if (!bindings.isEmpty()) {
				// can in our case only be 'Category' or 'Products', so we can take the first
				EdmNavigationPropertyBinding binding = bindings.get(0);
				EdmElement property = edmEntitySet.getEntityType().getProperty(binding.getPath());
				// we don't need to handle error cases, as it is done in the Olingo library
				if (property instanceof EdmNavigationProperty) {
					EdmNavigationProperty edmNavigationProperty = (EdmNavigationProperty) property;
					
					expandEdmEntityType = edmNavigationProperty.getType();
					navPropName = edmNavigationProperty.getName();
				}
			}
		} else {
			UriResource uriResource = expandItem.getResourcePath().getUriResourceParts().get(0);
			// we don't need to handle error cases, as it is done in the Olingo library
			if (uriResource instanceof UriResourceNavigation) {
				EdmNavigationProperty edmNavigationProperty = ((UriResourceNavigation) uriResource).getProperty();

				expandEdmEntityType = edmNavigationProperty.getType();
				navPropName = edmNavigationProperty.getName();
			}
		}
		
		JDBCQueryParams jdbcQueryParams = null;
		JDBCFilterExpression jdbcFilterExpression = getFilter(expandItem.getFilterOption(), expandEdmEntityType);
		if (jdbcFilterExpression != null) {
			jdbcQueryParams = JDBCQueryParams.builder().jdbcFilterExpression(jdbcFilterExpression).build();
		}
		
		if (expandEdmEntityType != null) {
			
			List<String> orderByColumns = storage.orderByColumns(expandEdmEntityType, expandItem.getOrderByOption());
			
			for (Entity entity : entityCollection.getEntities()) {
				Link link = new Link();
				link.setTitle(navPropName);
				
				Entity expandEntity = storage.getRelatedEntity(entity, expandEdmEntityType);
				if (expandEntity != null) {
					link.setInlineEntity(expandEntity);
				} else {
					EntityCollection expandEntityColl = storage.getRelatedEntities(entity, expandEdmEntityType, jdbcQueryParams, orderByColumns, false);
					link.setInlineEntitySet(expandEntityColl);
				}
				
				entity.getNavigationLinks().add(link);
			}
		}
	}
}
