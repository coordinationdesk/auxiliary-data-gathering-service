package com.exprivia.odc.delegate.item;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.apache.olingo.commons.api.data.ComplexValue;
import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.uri.queryoption.expression.BinaryOperatorKind;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.data.dto.Checksum;
import com.exprivia.odc.data.dto.Product;
import com.exprivia.odc.data.dto.TimeRange;
import com.exprivia.odc.data.repositories.ProductRepositoryJDBCImpl;
import com.exprivia.odc.edmprovider.actions.ODCEdmAProductsFilterList;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETAttribute;
import com.exprivia.odc.exception.ODCApiNotImplementedException;
import com.exprivia.odc.internalservice.data.dto.AipContent;
import com.exprivia.odc.internalservice.data.dto.AipContentStatus;
import com.exprivia.odc.internalservice.data.repositories.AipContentRepository.DownloadStatus;
import com.exprivia.odc.internalservice.data.repositories.AipContentRepositoryJDBCImpl;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponse;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponseAccepted;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponseBadRequest;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponseNotFound;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponseRedirect;
import com.exprivia.odc.util.Util;


@Component

public class StorageItemProduct extends StorageItemRelational implements StorageItem<Product> {

	@Autowired
	private ProductRepositoryJDBCImpl productRepository;
	
	@Autowired
	private StorageItemAttribute attributeStorage;
	
	@Autowired
	private AipContentRepositoryJDBCImpl aipContentRepository;
	
	private static Logger LOG = LoggerFactory.getLogger(StorageItemProduct.class);
	
	@Override
	public Entity getById(String id) {
		Product product = productRepository.findById(id);
		if (product != null) {
			return makeEntity(product);
		}
		return null;
	}

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(productRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(productRepository.getRecordsCount(queryParams));
		return ec;
	}
	
	@Override
	public EntityCollection getRelatedEntities(Entity entity, String entityName, JDBCQueryParams jdbcQueryParams, List<String> orderByColumns, boolean withCount) {
		if (entityName.equals(ODCEdmETAttribute.ET_NAME)) {
			// Attributes relationship
			StorageItemRelationalData data = getEntityRelationalData(entity);
			String uniqueId = convertColumnName("uuid");
			
			JDBCFilterExpression filter = null;
			Integer limit = null, offset = null;
			if (jdbcQueryParams != null) {
				limit = jdbcQueryParams.getLimitNumber();
				offset = jdbcQueryParams.getOffsetNumber();
				if (jdbcQueryParams.getJdbcFilterExpression() != null) {
					filter = JDBCFilterExpression.clone(jdbcQueryParams.getJdbcFilterExpression());
					filter.setPrepeparedWhere("(" + jdbcQueryParams.getJdbcFilterExpression() + ") and " + uniqueId + " = ?");
				}
			}
			if (filter == null) {
				filter = new JDBCFilterExpression(null);
				filter.setPrepeparedWhere(uniqueId + " = ?");
			}
			filter.addParameter(data.getReferenceKeys().get("productUuid"));
			// create new params, the jdbcQueryParams can be null
			JDBCQueryParams newParams = JDBCQueryParams.builder()
					.jdbcFilterExpression(filter)
					.limitNumber(limit)
					.offsetNumber(offset)
					.build();
			
			return attributeStorage.findByFilter(newParams, orderByColumns, withCount);
		}
		throw new RuntimeException("getRelatedEntities not implemented for Products/" + entityName);
	}
	
	@Override
	public EntityCollection performEntityCollectionAction(String actionName, Entity newEntity, Entity boundEntity, String boundEntityType) {
		if (actionName.equals(ODCEdmAProductsFilterList.AC_PRODUCTSFILTERLIST_NAME)) {
			if (newEntity.getProperties().size() != 1) {
				throw new RuntimeException("Wrong number of arguments");
			}
			
			EntityCollection inputEntities = (EntityCollection)(newEntity.getProperties().get(0).getValue());
			EntityCollection results = new EntityCollection();

			for (Entity requestedEntity : inputEntities.getEntities()) {
				Property prop = requestedEntity.getProperties().get(0);
				JDBCFilterExpression filter = new JDBCFilterExpression(BinaryOperatorKind.EQ);
				
				filter.setPrepeparedWhere(convertColumnName(prop.getName()) + " = ?");
				filter.addParameter(prop.getValue());
				
				JDBCQueryParams jdbcParams = JDBCQueryParams.builder().jdbcFilterExpression(filter).build();
				results.getEntities().addAll(
						findByFilter(jdbcParams, null, false).getEntities());
			}
			return results;
		}
		
		throw new ODCApiNotImplementedException("Action " + actionName + " not implemented in " + this.getClass().getCanonicalName());
	}

	// Convert instance of Object object into an Olingo Entity
	@SuppressWarnings("serial")
	public Entity makeEntity(Product product) {
		Entity entity = new Entity()
				.addProperty(new Property(null, Product.FIELD_ID, ValueType.PRIMITIVE, product.getId()))
				.addProperty(new Property(null, Product.FIELD_NAME, ValueType.PRIMITIVE, product.getName()))
				.addProperty(new Property(null, Product.FIELD_CONTENTTYPE, ValueType.PRIMITIVE,
						product.getContentType()))
				.addProperty(new Property(null, Product.FIELD_CONTENTLENGTH, ValueType.PRIMITIVE,
						product.getContentLength()))
				.addProperty(new Property(null, Product.FIELD_ORIGINDATE, ValueType.PRIMITIVE,
						product.getOriginDate()))
				.addProperty(new Property(null, Product.FIELD_PUBLICATIONDATE, ValueType.PRIMITIVE,
						product.getPublicationDate()))
				.addProperty(new Property(null, Product.FIELD_MODIFICATIONDATE, ValueType.PRIMITIVE,
						product.getModificationDate()))
				.addProperty(new Property(null, Product.FIELD_ONLINE, ValueType.PRIMITIVE, product.getOnline()));
		
		try {
			entity.setId(new URI(product.getId().toString()));
		} catch (URISyntaxException e) {
			throw new RuntimeException("Cannot create entity id", e);
		}
		
		// non mandatory fields
		if (product.getEvictionDate() != null)
			entity.addProperty(new Property(null, Product.FIELD_EVICTIONDATE, ValueType.PRIMITIVE, product.getEvictionDate()));
		if (product.getFootprint() != null)
			entity.addProperty(new Property(null, Product.FIELD_FOOTPRINT, ValueType.PRIMITIVE, product.getFootprint()));

		List<Checksum> checksums = product.getChecksums();
		List<ComplexValue> checksumsProps = new ArrayList<>();
		for (Checksum checksum : checksums) {
			ComplexValue complexValue = new ComplexValue();
			complexValue.getValue().add(
					new Property(null, Checksum.FIELD_ALGORITHM, ValueType.PRIMITIVE, checksum.getAlgorithm()));
			complexValue.getValue()
					.add(new Property(null, Checksum.FIELD_VALUE, ValueType.PRIMITIVE, checksum.getValue()));
			complexValue.getValue().add(new Property(null, Checksum.FIELD_CHECKSUMDATE, ValueType.PRIMITIVE,
					checksum.getChecksumDate()));
			checksumsProps.add(complexValue);
		}
		entity.addProperty(
				new Property(null, Product.FIELD_CHECKSUMS, ValueType.COLLECTION_COMPLEX, checksumsProps));

		ComplexValue complexTimeRangeValue = new ComplexValue();
		complexTimeRangeValue.getValue().add(new Property(null, TimeRange.FIELD_START, ValueType.PRIMITIVE,
				product.getContentDate().getStart()));
		complexTimeRangeValue.getValue().add(
				new Property(null, TimeRange.FIELD_END, ValueType.PRIMITIVE, product.getContentDate().getEnd()));
		entity.addProperty(
				new Property(null, Product.FIELD_CONTENTDATE, ValueType.COMPLEX, complexTimeRangeValue));
		entity.setMediaContentType(product.getContentType());

		// set the reference key to extract product data, if $expand=Product is requested
		setEntityRelationalData(entity, product.getClass().getSimpleName(), new HashMap<String, String>() {
			{
				put("productUuid", product.getId().toString());
			}
		});
		
		return entity;
	}

	@Override
	public ODCMediaResponse readMediaById(String id) throws ODataApplicationException {
		DownloadStatus ds = aipContentRepository.checkDownloadStatus(id);
		switch (ds) {
		case PRODUCT_NOT_EXISTS:
			// 400 Bad Request (Product Id is not known in the LTA)
			return new ODCMediaResponseBadRequest();
		case ORDER_NOT_SCHEDULED:
			// 404 Not Found (Product is not online)
			return new ODCMediaResponseNotFound();
		case ORDER_SCHEDULED_PRODUCT_NOT_READY:
			// 202 Accepted (Product is being retrieved as part of an existing order but is not yet online)
			return new ODCMediaResponseAccepted();
		case PRODUCT_DOWNLOADING_OR_READY:
			AipContent aipContent = aipContentRepository.findByProductUuid(id);
			if (aipContent == null || aipContent.getContentStatus() != AipContentStatus.ONLINE) {
				// 202 Accepted (Product is being retrieved as part of an existing order but is not yet online)
				return new ODCMediaResponseAccepted();
			}
			
			// perform a redirect
			String url = Util.getDownloadUrl(aipContent);
			
			LOG.info("Product {} located on AIP {}, url {}", id, aipContent.getAipName(), url);
			
			return new ODCMediaResponseRedirect(url);
		default:
			throw new RuntimeException("Unexpected download status: " + ds);
		}
	}
	
	public String convertColumnName(String name) {
		return productRepository.convertColumnName(name);
	}
	
	@Override
	public boolean isJsonProperty(String fieldName) {
		return fieldName.startsWith("Attributes") || fieldName.startsWith("Checksum");
	}
	
	@Override
	public void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand) {
		productRepository.processJsonCondition(expression, fieldName, sqlCondition, operand);
	}
}
