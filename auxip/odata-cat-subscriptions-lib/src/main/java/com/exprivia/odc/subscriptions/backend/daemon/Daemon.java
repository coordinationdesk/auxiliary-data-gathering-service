package com.exprivia.odc.subscriptions.backend.daemon;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.engine.NotificationProcessor;

@Component
public class Daemon {
	@Autowired
	private Config configuration;
	@Autowired
	private NotificationProcessor notificationProcessor;

	private static final Logger LOG = LoggerFactory.getLogger(Daemon.class);
	
	public void run() {
		boolean loop = !configuration.isDryRun();
		
		if (configuration.isVerbose()) {
			ch.qos.logback.classic.Logger root = (ch.qos.logback.classic.Logger) org.slf4j.LoggerFactory.getLogger(ch.qos.logback.classic.Logger.ROOT_LOGGER_NAME);
			root.setLevel(ch.qos.logback.classic.Level.DEBUG);
		}
		
		do {
			LOG.debug("checking for new data");
			
			try {
				try {
					// call the business logic
					notificationProcessor.processPendingNotifications();
				} catch (RuntimeException e) {
					LOG.error(e.getMessage());
					e.printStackTrace();
				}
				
				// pause thread
				if (loop) 
					sleep();
			} catch (InterruptedException e) {
				LOG.warn("daemon interrupted: {}", e.getMessage());
				e.printStackTrace();
				loop = false;
			}
		} while (loop);
	}
	
	private void sleep() throws InterruptedException {
		Long millis = configuration.getDaemonPeriod() * 1000;
		Thread.sleep(millis);
	}
	
}
