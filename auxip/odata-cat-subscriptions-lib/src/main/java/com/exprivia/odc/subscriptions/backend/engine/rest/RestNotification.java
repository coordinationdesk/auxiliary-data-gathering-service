package com.exprivia.odc.subscriptions.backend.engine.rest;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestClientException;
import org.springframework.web.client.RestTemplate;

import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.data.dto.AipContent;
import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.backend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.backend.data.dto.SubscriptionEvent;
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.experimental.FieldDefaults;

@Component
public class RestNotification {
	@Autowired
	private Config config;

	private RestTemplate rest = new RestTemplate();

	private static Logger LOG = LoggerFactory.getLogger(RestNotification.class);
	
	/**
	 * Send a batch of notifications to the notification service
	 * 
	 * @param notifications
	 * @return List<String> containing the notification service error messages
	 */
	public NotificationResponse sendNotifications(NotificationRequest notifications) {
		if (notifications.size() == 0) {
			LOG.debug("no notification to send");
			return new NotificationResponse();
		}
		
		ObjectMapper mapper = new ObjectMapper();

		// convert payload to json
		String body;
		try {
			body = mapper.writeValueAsString(notifications);
		} catch (JsonProcessingException e) {
			e.printStackTrace();
			throw new RestClientException(e.getMessage(), e);
		}
		
		// make the rest call
		String url = config.getNotificationUrl();
		
		LOG.debug("HTTP request: POST {}", url);
		HttpEntity<String> requestEntity = new HttpEntity<String>(body, null);
		ResponseEntity<String> responseEntity = rest.exchange(url, HttpMethod.POST, requestEntity, String.class);

		// parse results
		LOG.debug("HTTP response: {}", responseEntity.getBody());
		
		try {
			return mapper.readValue(responseEntity.getBody(), NotificationResponse.class);
		} catch (IOException e) {
			e.printStackTrace();
			throw new RestClientException(e.getMessage(), e);
		}
	}

	public static NotificationRequestItem createProductDownloadReadinessNotification(
			Subscription subscription, 
			AipContent aipContent,
			PendingInventory pendingInventory) {
		return NotificationRequestItem.builder()
				.subscriptionEvent(SubscriptionEvent.CREATED.toString().toLowerCase())
				.productId(aipContent.getProductUuid())
				.productName(pendingInventory.getName())
				.orderId(aipContent.getOrderId())
				.subscriptionId(subscription.getUuid().toString())
				.endpoint(subscription.getNotificationEndpoint())
				.endpointUser(subscription.getNotificationEpUser())
				.endpointPassword(subscription.getNotificationEpPassword())
				.build();
	}
	
	public static NotificationRequestItem createProductAvailability(Subscription subscription, PendingInventory pendingInventory) {
		return NotificationRequestItem.builder()
				.subscriptionEvent(SubscriptionEvent.CREATED.toString().toLowerCase())
				.productId(pendingInventory.getUuid().toString())
				.productName(pendingInventory.getName())
				.subscriptionId(subscription.getUuid().toString())
				.endpoint(subscription.getNotificationEndpoint())
				.endpointUser(subscription.getNotificationEpUser())
				.endpointPassword(subscription.getNotificationEpPassword())
				.build();
	}

	public static NotificationRequestItem createProductDeletionNotification(Subscription subscription, PendingInventory pendingInventory) {
		return NotificationRequestItem.builder()
				.subscriptionEvent(SubscriptionEvent.DELETED.toString().toLowerCase())
				.productId(pendingInventory.getUuid().toString())
				.productName(pendingInventory.getName())
				.subscriptionId(subscription.getUuid().toString())
				.endpoint(subscription.getNotificationEndpoint())
				.endpointUser(subscription.getNotificationEpUser())
				.endpointPassword(subscription.getNotificationEpPassword())
				.build();
	}
	
	@JsonSerialize(using = NotificationRequestSerializer.class)
	public static class NotificationRequest {
		private List<NotificationRequestItem> notifications = new ArrayList<NotificationRequestItem>();

		public NotificationRequest push(NotificationRequestItem notification) {
			notification.requestId = notifications.size();
			notifications.add(notification);
			return this;
		}
		
		public NotificationRequestItem get(int requestId) throws IndexOutOfBoundsException {
			return notifications.get(requestId);
		}
		
		public void remove(int requestId) {
			notifications.remove(requestId);
		}
		
		public int size() {
			return notifications.size();
		}
	}
	
	public static class NotificationRequestSerializer extends StdSerializer<NotificationRequest> {
		/**
		 * 
		 */
		private static final long serialVersionUID = 6805770096295779643L;
		
		public NotificationRequestSerializer() {
			super(null, false);
		}

		protected NotificationRequestSerializer(Class<?> t, boolean dummy) {
			super(t, dummy);
		}

		@Override
		public void serialize(NotificationRequest value, JsonGenerator gen, SerializerProvider provider) throws IOException {
			gen.writeStartObject();
			gen.writeFieldName("notification_requests");
			gen.writeStartArray();
			for (NotificationRequestItem notification : value.notifications) {
				gen.writeObject(notification);
			}
			gen.writeEndArray();
			gen.writeEndObject();
		}
		
	}

	@Data
	@Builder
	@FieldDefaults(level = AccessLevel.PRIVATE)
	@JsonSerialize(using = NotificationRequestItemSerializer.class)
	public static class NotificationRequestItem {
		/*
		 * All the String Id fields are UUID values
		 */
		public Integer requestId;
		public String productId;
		public String productName;
		public String subscriptionId;
		public String orderId;
		public String batchOrderId;
		public String subscriptionEvent;
		public String endpoint;
		public String endpointUser;
		public String endpointPassword;
	}
	
	public static class NotificationRequestItemSerializer extends StdSerializer<NotificationRequestItem> {
		/**
		 * 
		 */
		private static final long serialVersionUID = -226270506733729515L;

		protected NotificationRequestItemSerializer() {
			super(null, false);
		}
		
		protected NotificationRequestItemSerializer(Class<?> t, boolean dummy) {
			super(t, dummy);
		}

		protected String nullSafe(String s) {
			return s == null ? "" : s;
		}

		@Override
		public void serialize(NotificationRequestItem value, JsonGenerator gen, SerializerProvider provider) throws IOException {
			gen.writeStartObject();
			gen.writeNumberField("RequestId", value.requestId);
			gen.writeStringField("ProductId", nullSafe(value.productId));
			gen.writeStringField("ProductName", nullSafe(value.productName));
			if (value.subscriptionId != null)
				gen.writeStringField("SubscriptionId", value.subscriptionId);
			if (value.orderId != null)
				gen.writeStringField("OrderId", value.orderId);
			if (value.batchOrderId != null)
				gen.writeStringField("BatchOrderId", value.batchOrderId);
			gen.writeStringField("SubscriptionEvent", nullSafe(value.subscriptionEvent));
			gen.writeStringField("Endpoint", nullSafe(value.endpoint));
			if (value.endpointUser != null)
				gen.writeStringField("EndpointUser", value.endpointUser);
			if (value.endpointPassword != null)
				gen.writeStringField("EndpointPassword", value.endpointPassword);
			gen.writeEndObject();
		}
	}
	
	@Data
	@Builder
	@FieldDefaults(level = AccessLevel.PRIVATE)
	public static class NotificationResponseItem {
		Integer requestId;
		String errorMessage;
	}
	
	@JsonDeserialize(using = NotificationResponseDeserializer.class)
	public static class NotificationResponse implements Iterable<NotificationResponseItem> {
		List<NotificationResponseItem> responses = new ArrayList<NotificationResponseItem>();
		
		public NotificationResponse push(Integer requestId, String errorMessage) {
			responses.add(
					NotificationResponseItem.builder()
					.requestId(requestId)
					.errorMessage(errorMessage)
					.build());
			return this;
		}
		
		public Iterator<NotificationResponseItem> iterator() {
			return responses.iterator();
		}
	}
	
	public static class NotificationResponseDeserializer extends StdDeserializer<NotificationResponse> {

		/**
		 * 
		 */
		private static final long serialVersionUID = 7658907032105464267L;
		
		public NotificationResponseDeserializer() {
			super((Class<?>)null);
		}

		protected NotificationResponseDeserializer(Class<?> vc) {
			super(vc);
		}

		@Override
		public NotificationResponse deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
			
			JsonNode root = p.getCodec().readTree(p);
			NotificationResponse response = new NotificationResponse();
			
			for (JsonNode node : (ArrayNode)root.get("notification_responses")) {
				response.push(
						node.get("RequestId").numberValue().intValue(),
						node.get("ErrorMessage").asText().toString()
						);
			}
			
			return response;
		}
		
	}
}
