package com.exprivia.odc.subscriptions.backend.engine.rest;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Repository;
import org.springframework.web.client.RestClientException;
import org.springframework.web.client.RestTemplate;

import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.backend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.backend.data.repositories.SBESubscriptionRepository;
import com.fasterxml.jackson.databind.ObjectMapper;

@Repository
public class RestCheckMatchingSubscriptions {
	@Autowired
	private Config config;
	
	@Autowired
	private SBESubscriptionRepository subscriptionRepository;
	
	private RestTemplate rest = new RestTemplate();
	
	private static final Logger LOG = LoggerFactory.getLogger(RestCheckMatchingSubscriptions.class);
	
	public List<Subscription> getMatchingSubsctiptions(PendingInventory pendingInventory) {
		// setup url
		String url = config.getSubscriptionCheckUrl() + "?pendingInventoryId=" + pendingInventory.getId();
		LOG.debug("HTTP request: {}", url);

		// actually perform the rest call
		HttpEntity<String> requestEntity = new HttpEntity<String>("");
		ResponseEntity<String> responseEntity = rest.exchange(url, HttpMethod.GET, requestEntity, String.class);
		
		// parse results
		LOG.debug("HTTP response: {}", responseEntity.getBody());

		try {
			ObjectMapper mapper = new ObjectMapper();
			Long[] list = mapper.readValue(responseEntity.getBody(), Long[].class);
			
			List<Subscription> ret = new ArrayList<Subscription>();
			
			for (Long id : list) {
				Subscription subscription = subscriptionRepository.getById(id);
				if (subscription == null)
					throw new RuntimeException("Cannot find subscription with id " + id);
				
				ret.add(subscription);
				LOG.debug("pending inventory {} matching subscription subscription {}", pendingInventory, subscription);
			}

			return ret;

		} catch (IOException e) {
			e.printStackTrace();
			throw new RestClientException(e.getMessage(), e);
		}
	}
}
