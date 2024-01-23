package com.exprivia.odc.subscriptions.backend.engine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventoryEvent;
import com.exprivia.odc.subscriptions.backend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.backend.engine.rest.RestNotification;
import com.exprivia.odc.subscriptions.backend.engine.rest.RestNotification.NotificationRequest;

@Component
public class SubscriptionHandler {
	
	private static Logger LOG = LoggerFactory.getLogger(SubscriptionHandler.class);
	
	/*
	 * Simple subscription processing for PRIP/AUXIP
	 */
	public void processSubscription(
			Subscription subscription, 
			PendingInventory pendingInventory, 
			NotificationRequest notificationList) {

		LOG.info("Product {}/{}/{} subscription {}/{}/stage:{} ({} {})", 
				pendingInventory.getUuid(),
				pendingInventory.getOperation(),
				pendingInventory.getName(),
				subscription.getUuid(),
				subscription.getUsername(),
				subscription.getStageOrder(),
				subscription.getNotificationEndpoint(),
				subscription.getNotificationEpUser());
		
		/*
		 * Enqueue product availability notification
		 */
		
		if (pendingInventory.getOperation() == PendingInventoryEvent.INSERT) {
			LOG.info("enqueuing PRODUCT AVAILABILITY notification");
			notificationList.push(
					RestNotification.createProductAvailability(subscription, pendingInventory)
					);
		}
				
	}
	
	/*
	 * Complex subscription processing with order staging for LTA
	 */
//	public void processSubscription(
//			Subscription subscription, 
//			PendingInventory pendingInventory, 
//			NotificationRequest notificationList) {
//
//		LOG.info("Product {}/{}/{} subscription {}/{}/stage:{} ({} {})", 
//				pendingInventory.getUuid(),
//				pendingInventory.getOperation(),
//				pendingInventory.getName(),
//				subscription.getUuid(),
//				subscription.getUsername(),
//				subscription.getStageOrder(),
//				subscription.getNotificationEndpoint(),
//				subscription.getNotificationEpUser());
//		
//		if (pendingInventory.getOperation() == PendingInventoryEvent.DELETE) {
//			/*
//			 * Enqueue deletion notification
//			 */
//			
//			LOG.info("enqueuing DELETE notification");
//			notificationList.push(
//					RestNotification.createProductDeletionNotification(subscription, pendingInventory)
//					);
//			
//		} else {
//
//			if (subscription.getStageOrder() == false) {
//				/*
//				 * Enqueue product availability notification
//				 */
//				
//				LOG.info("enqueuing PRODUCT AVAILABILITY notification");
//				notificationList.push(
//						RestNotification.createProductAvailability(subscription, pendingInventory)
//						);
//				
//			} else {
//				AipContent aipContent = aipContentRepository.findByProductUuid(pendingInventory.getUuid());
//				if (aipContent != null) {
//					/*
//					 *  product already present in t_srv_aip_content
//					 */
//					LOG.debug("product already in AIP");
//					
//					// 1) update eviction date in t_srv_aip_content
//					LOG.debug("updating eviction date");
//					aipContentRepository.updateEvictionDate(aipContent);
//					
//					
//					// 2) find the t_srv_aip_content generating order
//					// 3) notify for download readiness
//					LOG.info("enqueuing DOWNLOAD READINESS notification");
//					notificationList.push(
//							RestNotification.createProductDownloadReadinessNotification(subscription, aipContent, pendingInventory)
//							);
//					
//				} else {
//					/*
//					 * Emit a new order
//					 */
//					LOG.info("emitting new ORDER");
//					
//					LTAOrderServicesFactory.getOrderHandler().generate(
//							new LTAOrderHandler.OrderBean(
//									pendingInventory.getUuid(), 
//									subscription.getUsername(), 
//									subscription.getPriority(), 
//									new LTAOrderHandler.OrderNotificationBean(
//											subscription.getNotificationEndpoint(), 
//											subscription.getNotificationEpUser(), 
//											subscription.getNotificationEpPassword()), 
//									null, 
//									null,
//									subscription.getUuid()), 
//							config.getEvictionTimePostponeInterval(), 
//							config.getDefaultAipName());
//				}
//			}
//		}
//	}
}
