package com.exprivia.auxip;

import java.util.Map.Entry;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;
import org.springframework.context.ConfigurableApplicationContext;

import com.exprivia.auxip.util.Version;

@SpringBootApplication
@ServletComponentScan
public class AUXIPWebappApplication {
	
	private static final Logger LOG = (Logger) LoggerFactory.getLogger(AUXIPWebappApplication.class);
	
	public static void main(String[] args) {
		
		ConfigurableApplicationContext context = SpringApplication.run(AUXIPWebappApplication.class, args);
		
		Version version = context.getBean(Version.class);

		LOG.info("Configuration");
		for (Entry<String, Object> entry : version.getAsMap().entrySet()) {
			LOG.info(String.format("%-32s: %s", entry.getKey(), entry.getValue().toString()));
		}
	}

}
