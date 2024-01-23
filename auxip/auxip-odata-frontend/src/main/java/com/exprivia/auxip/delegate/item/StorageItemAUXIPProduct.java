package com.exprivia.auxip.delegate.item;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.apache.olingo.commons.api.data.ComplexValue;
import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Lazy;
import org.springframework.context.annotation.Primary;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.stereotype.Repository;

import com.exprivia.auxip.data.dto.AUXIPProduct;
import com.exprivia.auxip.data.repositories.AUXIPProductRepositoryJDBCImpl;
import com.exprivia.auxip.edmprovider.entitytypes.AUXIPEdmETProduct;
import com.exprivia.auxip.util.ProductUrlResolver;
import com.exprivia.odc.data.dto.Checksum;
import com.exprivia.odc.data.dto.TimeRange;
import com.exprivia.odc.delegate.item.StorageItemProduct;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponse;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponseNotFound;
import com.exprivia.odc.service.mediaresponse.ODCMediaResponseRedirect;

@Repository
@Primary
public class StorageItemAUXIPProduct extends StorageItemProduct {
	@Autowired
	private AUXIPProductRepositoryJDBCImpl auxipProductRepository;
	
	@Autowired
	@Lazy
	private ProductUrlResolver urlResolver;
	
	@Override
	public Entity getById(String id) {
		AUXIPProduct product = auxipProductRepository.findById(id);
		if (product != null) {
			return makeEntity(product);
		}
		return null;
	}
	
	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeAuxipProductEntityCollection(auxipProductRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(auxipProductRepository.getRecordsCount(queryParams));
		return ec;
	}
	
	private EntityCollection makeAuxipProductEntityCollection(List<AUXIPProduct> objList) {
		EntityCollection entitySet = new EntityCollection();
		for (AUXIPProduct obj : objList) {
			entitySet.getEntities().add(makeEntity(obj));
		}

		return entitySet;
	}
	
	// Convert instance of Object object into an Olingo Entity
	@SuppressWarnings("serial")
	public Entity makeEntity(AUXIPProduct product) {
		Entity entity = new Entity()
				.addProperty(new Property(null, AUXIPProduct.FIELD_ID, ValueType.PRIMITIVE, product.getId()))
				.addProperty(new Property(null, AUXIPProduct.FIELD_NAME, ValueType.PRIMITIVE, product.getName()))
				.addProperty(new Property(null, AUXIPProduct.FIELD_CONTENTTYPE, ValueType.PRIMITIVE,
						product.getContentType()))
				.addProperty(new Property(null, AUXIPProduct.FIELD_CONTENTLENGTH, ValueType.PRIMITIVE,
						product.getContentLength()))
				.addProperty(new Property(null, AUXIPProduct.FIELD_PUBLICATIONDATE, ValueType.PRIMITIVE,
						product.getPublicationDate()));

		// non mandatory fields
		if (product.getOriginDate() != null)
			entity.addProperty(new Property(null, AUXIPProduct.FIELD_ORIGINDATE, ValueType.PRIMITIVE, product.getOriginDate()));
		if (product.getEvictionDate() != null)
			entity.addProperty(new Property(null, AUXIPProduct.FIELD_EVICTIONDATE, ValueType.PRIMITIVE, product.getEvictionDate()));

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
				new Property(null, AUXIPProduct.FIELD_CHECKSUMS, ValueType.COLLECTION_COMPLEX, checksumsProps));

		ComplexValue complexTimeRangeValue = new ComplexValue();
		complexTimeRangeValue.getValue().add(new Property(null, TimeRange.FIELD_START, ValueType.PRIMITIVE,
				product.getContentDate().getStart()));
		complexTimeRangeValue.getValue().add(
				new Property(null, TimeRange.FIELD_END, ValueType.PRIMITIVE, product.getContentDate().getEnd()));
		entity.addProperty(
				new Property(null, AUXIPProduct.FIELD_CONTENTDATE, ValueType.COMPLEX, complexTimeRangeValue));
		entity.setMediaContentType(product.getContentType());

		// set the reference key to extract product data, if $expand=Product is requested
		setEntityRelationalData(entity, AUXIPEdmETProduct.ET_NAME, new HashMap<String, String>() {
			{
				put("productUuid", product.getId().toString());
			}
		});
		
		return entity;
	}
	
	@Override
	public String convertColumnName(String name) {
		return auxipProductRepository.convertColumnName(name);
	}
	
	@Override
	public void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand) {
		auxipProductRepository.processJsonCondition(expression, fieldName, sqlCondition, operand);
	}
	
	@Override
	public ODCMediaResponse readMediaById(String id) {
		try {
			return new ODCMediaResponseRedirect(urlResolver.getDownloadUrl(id).toString());
		} catch (EmptyResultDataAccessException e) {
			return new ODCMediaResponseNotFound();
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage(), e);
		}
	}
}
