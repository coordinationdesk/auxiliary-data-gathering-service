package com.exprivia.auxip;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

import com.exprivia.auxip.util.Version;
import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.daemon.Daemon;

@SpringBootApplication
@ComponentScan("com.exprivia.odc.subscriptions.backend")
@ComponentScan("com.exprivia.auxip")
public class AUXIPSubscriptionConsoleApplication implements CommandLineRunner {
	@Autowired
	private Daemon daemon;
	
	@Autowired
	private Config configuration;
	
	@Autowired
	private Version version;
	
	private static final Logger LOG = LoggerFactory.getLogger(AUXIPSubscriptionConsoleApplication.class);
    
	public static void main(String[] args) {
        SpringApplication.run(AUXIPSubscriptionConsoleApplication.class, args);
    }
    
	@Override
	public void run(String... args) throws Exception {
		boolean printVersion = false;
		
		for (String arg : args) {
			switch (arg) {
			case "-t":
				configuration.setDryRun(true);
				break;
				
			case "-v":
				configuration.setVerbose(true);
				break;
				
			case "--version":
				printVersion = true;
				break;
			}
		}
		
		if (printVersion) {
			printVersion();
		} else {
			LOG.info("starting notification application");
			
			LOG.info("datasourceurl: {}", configuration.getSpringDatasourceUrl());
			LOG.info("notification url: {}", configuration.getNotificationUrl());
			LOG.info("daemon period: {} sec", configuration.getDaemonPeriod());
			
			if (configuration.isDryRun())
				LOG.warn("RUNNING IN READ-ONLY MODE");
			
			daemon.run();
		}
		LOG.info("application done");
	}

	private void printVersion() {
		for (Map.Entry<String, String> entry : version.getAsMap().entrySet()) {
			LOG.info("Version info: {} -> {}", entry.getKey(), entry.getValue());
		}
	}
}
