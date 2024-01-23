package com.exprivia.odc.subscriptions.frontend.delegate.item;

import java.sql.Timestamp;
import java.time.Instant;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.UUID;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.core.uri.parser.UriParserException;
import org.apache.olingo.server.core.uri.validator.UriValidationException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.item.StorageItem;
import com.exprivia.odc.delegate.item.StorageItemRelational;
import com.exprivia.odc.edmprovider.ODCEdmAction;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.servlet.ODCBaseServices;
import com.exprivia.odc.subscriptions.frontend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.frontend.data.dto.SubscriptionStatus;
import com.exprivia.odc.subscriptions.frontend.data.repositories.SubscriptionRepositoryJDBCImpl;
import com.exprivia.odc.subscriptions.frontend.data.repositories.exception.SubscriptionBadStatusChangeException;
import com.exprivia.odc.subscriptions.frontend.edmprovider.actions.ODCEdmACancel;
import com.exprivia.odc.subscriptions.frontend.edmprovider.actions.ODCEdmAPause;
import com.exprivia.odc.subscriptions.frontend.edmprovider.actions.ODCEdmAResume;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETNotification;
import com.exprivia.odc.util.ParseQueryFilter;
import com.exprivia.odc.util.Util;


@Component

public class StorageItemSubscription extends StorageItemRelational implements StorageItem<Subscription> {

	@Autowired
	private SubscriptionRepositoryJDBCImpl subscriptionRepository;
	
	@Autowired
	private StorageItemNotification notificationStorage;
	
	@Autowired
	private ODCBaseServices baseServices;
	
	@Override
	public Entity getById(String id) {
		Subscription subscription = subscriptionRepository.findById(id);
		if (subscription != null) {
			return makeEntity(subscription);
		}
		return null;
	}

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(subscriptionRepository.findWithParams(queryParams, orderByColumns, false));
		if (withCount)
			ec.setCount(subscriptionRepository.getRecordsCount(queryParams));
		return ec;
	}
	
	@Override
	public EntityCollection getRelatedEntities(Entity entity, String entityName, JDBCQueryParams jdbcQueryParams, List<String> orderByColumns, boolean withCount) {
		String subscriptionId = entity.getProperty("Id").getValue().toString();
		if (entityName.equals(ODCEdmETNotification.ET_NAME)) {
			return notificationStorage.findByFilterFromSubscription(subscriptionId, jdbcQueryParams, orderByColumns, withCount);
		}
		throw new RuntimeException("getRelatedEntities not implemented for Subscription/" + entityName);
	}
	
	@Override
	public Entity performEntityAction(String actionName, Entity newEntity, Entity boundEntity, String boundEntityType)
			throws ODataApplicationException {
		
		if (actionName.equals(ODCEdmAction.DEFAULT_ACTION_CREATE)) {
			String filterParam;
			try {
				filterParam = ParseQueryFilter.sanitizeQueryFilter(baseServices, newEntity.getProperty(Subscription.FIELD_FILTERPARAM).getValue().toString());
			} catch (UriParserException | UriValidationException e) {
				throw new ODataApplicationException("Wrong filter syntax", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
			}
			try {
				Subscription subscription = Subscription.builder()
						.id(UUID.randomUUID()).submissionDate(Timestamp.from(Instant.now()))
						.status(SubscriptionStatus.RUNNING)
						.filterParam(filterParam)
						.notificationEndpoint(newEntity.getProperty(Subscription.FIELD_NOTIFICATIONENDPOINT) == null ? null : newEntity.getProperty(Subscription.FIELD_NOTIFICATIONENDPOINT).getValue().toString())
						.notificationEpUsername(newEntity.getProperty(Subscription.FIELD_NOTIFICATIONEPUSERNAME) == null ? null : newEntity.getProperty(Subscription.FIELD_NOTIFICATIONEPUSERNAME).getValue().toString())
						.notificationEpPassword(newEntity.getProperty(Subscription.FIELD_NOTIFICATIONEPPASSWORD) == null ? null : newEntity.getProperty(Subscription.FIELD_NOTIFICATIONEPPASSWORD).getValue().toString())
						.build();
				return makeEntity(subscriptionRepository.insert(subscription));
			} catch (NullPointerException e) {
				throw new ODataApplicationException("Missing required property", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
			}
		} else if (actionName.equals(ODCEdmAction.DEFAULT_ACTION_UPDATE)) {
			changeStatusByEntity(newEntity, null);
			return null;
		} else if (actionName.equals(ODCEdmAPause.AC_PAUSE_NAME)) {
			changeStatusByEntity(boundEntity, SubscriptionStatus.PAUSED);
			return null;
		} else if (actionName.equals(ODCEdmAResume.AC_RESUME_NAME)) {
			changeStatusByEntity(boundEntity, SubscriptionStatus.RUNNING);
			return null;
		} else if (actionName.equals(ODCEdmACancel.AC_CANCEL_NAME)) {
			changeStatusByEntity(boundEntity, SubscriptionStatus.CANCELLED);
			return null;
		}
		
		return StorageItem.super.performEntityAction(actionName, newEntity, boundEntity, boundEntityType);
	}

	// Convert instance of Object object into an Olingo Entity
	public Entity makeEntity(Subscription subscription) {
		Entity entity = new Entity()
				.addProperty(new Property(null, Subscription.FIELD_ID, ValueType.PRIMITIVE, subscription.getId()))
				.addProperty(new Property(null, Subscription.FIELD_STATUS, ValueType.PRIMITIVE, subscription.getStatus().ordinal()))
				.addProperty(new Property(null, Subscription.FIELD_FILTERPARAM, ValueType.PRIMITIVE, subscription.getFilterParam()))
				.addProperty(new Property(null, Subscription.FIELD_SUBMISSIONDATE, ValueType.PRIMITIVE, subscription.getSubmissionDate()))
				.addProperty(new Property(null, Subscription.FIELD_NOTIFICATIONENDPOINT, ValueType.PRIMITIVE, subscription.getNotificationEndpoint()))
				;
		if (subscription.getLastNotificationDate() != null)
			entity.addProperty(new Property(null, Subscription.FIELD_LASTNOTIFICATIONDATE, ValueType.PRIMITIVE, subscription.getLastNotificationDate()));
			
		if (subscription.getNotificationEpUsername() != null)
			entity.addProperty(new Property(null, Subscription.FIELD_NOTIFICATIONEPUSERNAME, ValueType.PRIMITIVE, subscription.getNotificationEpUsername()));

		if (subscription.getNotificationEpPassword() != null)
			entity.addProperty(new Property(null, Subscription.FIELD_NOTIFICATIONEPPASSWORD, ValueType.PRIMITIVE, Util.hidePassword(subscription.getNotificationEpPassword())));
		
		setEntityRelationalData(entity, subscription.getClass().getSimpleName(), new HashMap<String, String>());
		
		return entity;
	}

	@Override
	public String convertColumnName(String name) {
		return subscriptionRepository.convertColumnName(name);
	}
	
	@Override
	public String convertLiteralValue(String value) {
		return subscriptionRepository.convertLiteralValue(value);
	}
	
	protected void changeStatusByEntity(Entity entity, SubscriptionStatus newStatus) throws ODataApplicationException {
		String id;
		SubscriptionStatus status = newStatus;
		if (status == null) {
			try {
				String statusString = entity.getProperty(Subscription.FIELD_STATUS).getValue().toString();
				status = SubscriptionStatus.fromString(statusString);
			} catch (NullPointerException e) {
				throw new ODataApplicationException("Missing required property", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
			} catch (RuntimeException e) {
				throw new ODataApplicationException("Bad status specified", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH, e);
			}
		}
		
		try {
			id = entity.getProperty(Subscription.FIELD_ID).getValue().toString();
		} catch (NullPointerException e) {
			throw new ODataApplicationException("Missing required property", HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
		}
		Subscription subscription = subscriptionRepository.findById(id);
		if (subscription == null) {
			throw new ODataApplicationException("Cannot find subscription with id " + id, HttpStatusCode.NOT_FOUND.getStatusCode(), Locale.ENGLISH);
		}
		try {
			switch (status) {
			case RUNNING: subscriptionRepository.resume(subscription); break;
			case PAUSED: subscriptionRepository.pause(subscription); break;
			case CANCELLED: subscriptionRepository.cancel(subscription); break;
			}
		} catch (SubscriptionBadStatusChangeException e) {
			throw new ODataApplicationException("Bad status change for subscription: " + e.getMessage(), HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH, e);
		}
	}
}
