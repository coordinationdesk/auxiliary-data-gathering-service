package com.exprivia.auxip.util;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.context.annotation.PropertySource;
import org.springframework.context.annotation.PropertySources;

import com.exprivia.odc.subscriptions.backend.config.Config;

@Configuration
@Primary
@PropertySources({
	@PropertySource("classpath:odc-subscriptions.properties"),
	@PropertySource("classpath:odc-subscriptions-${spring.profiles.active}.properties"), 
	@PropertySource("classpath:application.properties")
	})
public class AUXIPSubscriptionConfiguration extends Config {
	@Value("${odc.supscriptions.pendinginv.limit}")
	private Long _pendingInvLimit;

	@Value("${odc.subscriptions.eviction-time-postpone-interval}")
	private Long _evictionTimePostponeInterval;
	
	@Value("${odc.supscriptions.services.notification.url}")
	private String _notificationUrl;
	
	@Value("${odc.supscriptions.services.subscriptioncheck.url}")
	private String _subscriptionCheckUrl;
	
	@Value("${odc.supscriptions.daemon.period}")
	private Long _daemonPeriod;
	
	@Value("${spring.datasource.url}")
	private String _springDatasourceUrl;
	
	@Value("${odc.supscriptions.notification.pagesize}")
	private Long _notificationPageSize;

	@PostConstruct
	public void setConfiguration() {
		setDaemonPeriod(_daemonPeriod);
		setEvictionTimePostponeInterval(_evictionTimePostponeInterval);
		setNotificationPageSize(_notificationPageSize);
		setNotificationUrl(_notificationUrl);
		setPendingInvLimit(_pendingInvLimit);
		setSpringDatasourceUrl(_springDatasourceUrl);
		setSubscriptionCheckUrl(_subscriptionCheckUrl);
	}
}
