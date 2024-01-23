package com.exprivia.odc.subscriptions.backend.config;

import org.springframework.context.annotation.Configuration;

@Configuration
public class Config {
	private Long pendingInvLimit;
	private Long evictionTimePostponeInterval;
	private String notificationUrl;
	private String subscriptionCheckUrl;
	private Long daemonPeriod;
	private String springDatasourceUrl;
	private Long notificationPageSize;
	private boolean dryRun = false;
	private boolean verbose = false;
	
	public Long getPendingInvLimit() {
		return pendingInvLimit;
	}
	
	public void setPendingInvLimit(Long pendingInvLimit) {
		this.pendingInvLimit = pendingInvLimit;
	}
	
	public Long getEvictionTimePostponeInterval() {
		return evictionTimePostponeInterval;
	}
	
	public void setEvictionTimePostponeInterval(Long evictionTimePostponeInterval) {
		this.evictionTimePostponeInterval = evictionTimePostponeInterval;
	}
	
	public String getNotificationUrl() {
		return notificationUrl;
	}
	
	public void setNotificationUrl(String notificationUrl) {
		this.notificationUrl = notificationUrl;
	}
	
	public String getSubscriptionCheckUrl() {
		return subscriptionCheckUrl;
	}
	
	public void setSubscriptionCheckUrl(String subscriptionCheckUrl) {
		this.subscriptionCheckUrl = subscriptionCheckUrl;
	}
	
	public Long getDaemonPeriod() {
		return daemonPeriod;
	}
	
	public void setDaemonPeriod(Long daemonPeriod) {
		this.daemonPeriod = daemonPeriod;
	}
	
	public String getSpringDatasourceUrl() {
		return springDatasourceUrl;
	}
	
	public void setSpringDatasourceUrl(String springDatasourceUrl) {
		this.springDatasourceUrl = springDatasourceUrl;
	}
	
	public Long getNotificationPageSize() {
		return notificationPageSize;
	}
	
	public void setNotificationPageSize(Long notificationPageSize) {
		this.notificationPageSize = notificationPageSize;
	}
	
	public boolean isDryRun() {
		return dryRun;
	}
	
	public void setDryRun(boolean dryRun) {
		this.dryRun = dryRun;
	}
	
	public boolean isVerbose() {
		return verbose;
	}
	
	public void setVerbose(boolean verbose) {
		this.verbose = verbose;
	}
}
