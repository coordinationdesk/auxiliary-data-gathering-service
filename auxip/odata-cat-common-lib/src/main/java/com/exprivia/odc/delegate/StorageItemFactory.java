package com.exprivia.odc.delegate;


import org.apache.olingo.commons.api.data.Entity;

import com.exprivia.odc.delegate.item.StorageItem;

public interface StorageItemFactory {
	StorageItem<?> createFromName(String name);
	StorageItem<?> createFromEntity(Entity entity);
}
