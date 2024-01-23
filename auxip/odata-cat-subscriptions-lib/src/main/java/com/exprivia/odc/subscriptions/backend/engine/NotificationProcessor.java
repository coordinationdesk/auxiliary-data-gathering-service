package com.exprivia.odc.subscriptions.backend.engine;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestClientException;

import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.backend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.backend.engine.rest.RestCheckMatchingSubscriptions;
import com.exprivia.odc.subscriptions.backend.engine.rest.RestNotification;
import com.exprivia.odc.subscriptions.backend.engine.rest.RestNotification.NotificationResponse;
import com.exprivia.odc.subscriptions.backend.engine.rest.RestNotification.NotificationResponseItem;

@Component
public class NotificationProcessor {
	@Autowired
	private QueueManager queueManager;
	
	@Autowired
	private RestCheckMatchingSubscriptions restCheckMatchingSubscriptions;
	
	@Autowired
	private RestNotification restNotification;
	
	@Autowired
	private SubscriptionHandler subscriptionHandler;
	
	@Autowired
	private Config config;
	
	private static final Logger LOG = LoggerFactory.getLogger(NotificationProcessor.class);

	public void processPendingNotifications() {
		RestNotification.NotificationRequest notificationList = new RestNotification.NotificationRequest();

		int queuedItemsSize = 0;
		do {
			// get the pending inventory items queue
			List<PendingInventory> queuedItems = queueManager.getQueuedItems();
			queuedItemsSize = queuedItems.size();
			
			List<PendingInventory> processedItems = new ArrayList<PendingInventory>();
			
			for (PendingInventory item : queuedItems) {
				// skip empty deleted items (when an unexpected "operation" field is found, for instance)
				if (item == null)
					continue;
				
				try {
					LOG.debug("processing pendingInventory item {}", item);
					// check matching subscriptions
					List<Subscription> matchingSubscriptions = restCheckMatchingSubscriptions.getMatchingSubsctiptions(item);
					
					for (Subscription subscription : matchingSubscriptions) {
						try {
							// handle notification/staging-order/completeness
							subscriptionHandler.processSubscription(subscription, item, notificationList);

						} catch (DataAccessException e) {
							LOG.error("DB error in processSubscription {} {}: {}", subscription, item, e.getMessage());
							e.printStackTrace();
						}
					}
					
					// if ok, add the item to the processedItems list
					processedItems.add(item);
	
				} catch (RestClientException e) {
					LOG.error("HTTP REST error checkMatchingSubscriptions: {} " + e.getMessage());
					e.printStackTrace();
				} catch (RuntimeException e) {
					LOG.error(e.getMessage());
					e.printStackTrace();
				}
				
				// send notification pages ready to be sent
				sendNotificationPages(notificationList, false);
			}
			
			if (config.isDryRun())
				LOG.warn("READ-ONLY MODE: {} pending inventory should be deleted", processedItems.size());
			else
				queueManager.wipeQueuedItems(processedItems);
			
		} while (queuedItemsSize > 0 && config.isDryRun() == false);
		
		// send remaining notifications, forcing incomplete pages
		sendNotificationPages(notificationList, true);
	}
	
	private void sendNotificationPages(RestNotification.NotificationRequest notificationList, boolean force) {
		
		LOG.debug("notifications list length {}", notificationList.size());
		
		RestNotification.NotificationRequest page = new RestNotification.NotificationRequest();
		
		int sentNotifications = 0;
		
		for (int a = 0; a < notificationList.size(); a++) {
			page.push(notificationList.get(a));
			
			// check if a page size is reached
			if ((a + 1) % config.getNotificationPageSize() == 0) {
				// send notification page
				sendNotifications(page);
				sentNotifications += page.size();
				
				// build a new notifications page
				page = new RestNotification.NotificationRequest();
			}
		}
		
		// remove sent notifications from the main queue
		for (int a = sentNotifications; a > 0; a--) {
			notificationList.remove(a - 1);
		}
		
		LOG.debug("remaining notifications {}", notificationList.size());

		// send remaining notifications, if any
		if (force && page.size() > 0) {
			sendNotifications(page);
		}
	}
	
	private void sendNotifications(RestNotification.NotificationRequest notificationList) {
		
		LOG.debug("sending {} notifications", notificationList.size());
		
		try {
			// TODO error handling must be discussed
			// send the batch of collected notifications
			NotificationResponse response = restNotification.sendNotifications(notificationList);
			
			for (NotificationResponseItem item: response) {
				LOG.error("Error sending notification {}: {}", 
						notificationList.get(item.getRequestId()),
						item.getErrorMessage()
						);
			}
			
		} catch (RestClientException e) {
			LOG.error("HTTP REST error notifications: {} " + e.getMessage());
			e.printStackTrace();
		} catch (IndexOutOfBoundsException e) {
			LOG.error("HTTP REST misleading notifications response: {} " + e.getMessage());
			e.printStackTrace();
		} catch (RuntimeException e) {
			LOG.error(e.getMessage());
			e.printStackTrace();
		}
	}
}
