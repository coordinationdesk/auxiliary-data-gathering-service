package com.exprivia.auxip.delegate;

import org.apache.olingo.commons.api.data.Entity;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.auxip.delegate.item.StorageItemAUXIPProduct;
import com.exprivia.auxip.delegate.item.StorageItemAUXIPQuota;
import com.exprivia.auxip.edmprovider.entitysets.AUXIPEdmESProducts;
import com.exprivia.auxip.edmprovider.entitytypes.AUXIPEdmETProduct;
import com.exprivia.odc.delegate.StorageItemFactory;
import com.exprivia.odc.delegate.item.StorageItem;
import com.exprivia.odc.delegate.item.StorageItemAttribute;
import com.exprivia.odc.delegate.item.StorageItemRelational;
import com.exprivia.odc.delegate.item.StorageItemSystemRole;
import com.exprivia.odc.delegate.item.StorageItemUser;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESAttributes;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESQuotas;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESSystemRoles;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESUsers;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETAttribute;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETQuota;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETSystemRole;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETUser;
import com.exprivia.odc.metrics.delegate.item.StorageItemMetric;
import com.exprivia.odc.metrics.edmprovider.entitysets.ODCEdmESMetrics;
import com.exprivia.odc.metrics.edmprovider.entitytypes.ODCEdmETMetric;
import com.exprivia.odc.subscriptions.frontend.delegate.item.StorageItemNotification;
import com.exprivia.odc.subscriptions.frontend.delegate.item.StorageItemSubscription;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitysets.ODCEdmESNotifications;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitysets.ODCEdmESSubscriptions;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETNotification;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETSubscription;

/**
 * @author andrea
 * 
 * Creates a {@link StorageItem} starting from the OData Entity name
 *
 */
@Component
public class StorageItemFactoryImpl implements StorageItemFactory {

	@Autowired
	private StorageItemAUXIPProduct storageItemAUXIPProduct;
	@Autowired
	private StorageItemUser storageItemUser;
	@Autowired
	private StorageItemSystemRole storageItemSystemRole;
	@Autowired
	private StorageItemAttribute storageItemAttribute;
	@Autowired
	private StorageItemAUXIPQuota storageItemAUXIPQuota;
	
	/*
	 * From the odata-cat-metrics-lib
	 */
	@Autowired
	private StorageItemMetric storageItemMetric;
	
	/*
	 * From the odata-cat-subscriptions-lib
	 */
	@Autowired
	private StorageItemSubscription storageItemSubscription;
	@Autowired
	private StorageItemNotification storageItemNotification;


	@Override
	public StorageItem<?> createFromName(String name) {
		if (name.equals(AUXIPEdmETProduct.ET_NAME) || name.equals(AUXIPEdmESProducts.ES_NAME)) {
			return storageItemAUXIPProduct;
		} else if (name.equals(ODCEdmETUser.ET_NAME) || name.equals(ODCEdmESUsers.ES_NAME)) {
			return storageItemUser;
		} else if (name.equals(ODCEdmETSystemRole.ET_NAME) || name.equals(ODCEdmESSystemRoles.ES_NAME)) {
			return storageItemSystemRole;
		} else if (name.equals(ODCEdmETAttribute.ET_NAME) || name.equals(ODCEdmESAttributes.ES_NAME)) {
			return storageItemAttribute;
		} else if (name.equals(ODCEdmETQuota.ET_NAME) || name.equals(ODCEdmESQuotas.ES_NAME)) {
			return storageItemAUXIPQuota;
		}
		/*
		 * From the odata-cat-metrics-lib
		 */
		else if (name.equals(ODCEdmETMetric.ET_NAME) || name.equals(ODCEdmESMetrics.ES_NAME) ) {
			return storageItemMetric;
		}
		/*
		 * From the odata-cat-subscriptions-lib
		 */
		else if (name.equals(ODCEdmETSubscription.ET_NAME) || name.equals(ODCEdmESSubscriptions.ES_NAME) ) {
			return storageItemSubscription;
		}
		else if (name.equals(ODCEdmETNotification.ET_NAME) || name.equals(ODCEdmESNotifications.ES_NAME) ) {
			return storageItemNotification;
		}
		return null;
	}

	public StorageItem<?> createFromEntity(Entity entity) {
		String className = StorageItemRelational.getEntityClass(entity);
		return createFromName(className);
	}
}
