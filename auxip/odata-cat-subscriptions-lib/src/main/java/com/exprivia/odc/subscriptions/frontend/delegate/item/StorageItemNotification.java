package com.exprivia.odc.subscriptions.frontend.delegate.item;

import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.item.StorageItem;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.subscriptions.frontend.data.dto.Notification;
import com.exprivia.odc.subscriptions.frontend.data.repositories.NotificationRepositoryJDBCImpl;

@Component

public class StorageItemNotification implements StorageItem<Notification> {

	@Autowired
	private NotificationRepositoryJDBCImpl notificationRepository;
	
	@Override
	public Entity getById(String id) {
		throw new RuntimeException("Notification cannot be retrieved by id");
	}
	
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(notificationRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(notificationRepository.getRecordsCount(queryParams));
		return ec;
	}

	public EntityCollection findByFilterFromSubscription(String subscriptionId, JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		JDBCQueryParams newParams = NotificationRepositoryJDBCImpl.addNotificationsFromSubscriptionIdCondition(
				queryParams, 
				subscriptionId);
		EntityCollection ec = makeEntityCollection(notificationRepository.findWithParams(newParams, orderByColumns));
		if (withCount)
			ec.setCount(notificationRepository.getRecordsCount(newParams));
		return ec;
	}
	
	// Convert instance of Object object into an Olingo Entity
	public Entity makeEntity(Notification notification) {
		Entity entity = new Entity()
				.addProperty(new Property(null, Notification.FIELD_PRODUCTID, ValueType.PRIMITIVE, notification.getProductId()))
				.addProperty(new Property(null, Notification.FIELD_PRODUCTNAME, ValueType.PRIMITIVE, notification.getProductName()))
				.addProperty(new Property(null, Notification.FIELD_NOTIFICATIONDATE, ValueType.PRIMITIVE, notification.getNotificationDate()))
				;
		
		if (notification.getSubscriptionID() != null)
			entity.addProperty(new Property(null, Notification.FIELD_SUBSCRIPTIONID, ValueType.PRIMITIVE, notification.getSubscriptionID()));
		
		return entity;
	}

	public String convertColumnName(String name) {
		return notificationRepository.convertColumnName(name);
	}

}
