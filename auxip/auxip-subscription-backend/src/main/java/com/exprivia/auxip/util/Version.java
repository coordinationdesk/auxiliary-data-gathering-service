package com.exprivia.auxip.util;

import java.util.HashMap;
import java.util.Map;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;

@Configuration
@Component
@PropertySource({"classpath:auxip.properties"})
public class Version {
	@Value("${auxip.application.name:auxip-subscription-backend}")
	public String application;

	@Value("${auxip.application.version:0.0}")
	public String version;
	
	public Map<String, String> getAsMap() {
		Map<String, String> versionMap = new HashMap<String, String>();
		versionMap.put("application", application);
		versionMap.put("version", version);
		
		return versionMap;
	}
}
