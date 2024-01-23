package com.exprivia.auxip.util;

import java.util.HashMap;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.auxip.servlet.AUXIPConfig;

@Component
public class Version {
	@Autowired
	private AUXIPConfig auxipConfig;
	
	public Map<String, Object> getAsMap() {
		Map<String, Object> versionMap = new HashMap<String, Object>();
		versionMap.put("application", auxipConfig.getApplication());
		versionMap.put("version", auxipConfig.getVersion());
		versionMap.put("icd-compiance", auxipConfig.getIcdCompiance());
		
		versionMap.put("spring-datasource-url", auxipConfig.getSpringDatasourceUrl());
		versionMap.put("spring-datasource-username", auxipConfig.getSpringDatasourceUsername());
		
		versionMap.put("query-limit", auxipConfig.getDbDefaults().getQueryLimit());
		versionMap.put("eviction-time-postpone-interval", auxipConfig.getDbDefaults().getEvictionTimePostponeInterval());
		
		return versionMap;
	}
}
