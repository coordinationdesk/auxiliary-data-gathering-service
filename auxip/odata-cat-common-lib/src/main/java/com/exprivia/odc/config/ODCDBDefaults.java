package com.exprivia.odc.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;

import lombok.Getter;

@Component
@PropertySource({ "classpath:odc.properties", "classpath:odc-${spring.profiles.active}.properties" })
@Getter
public class ODCDBDefaults {
	@Value("${odata-cat.query.limit}")
	protected Long queryLimit;
	
	@Value("${odata-cat.eviction-time-postpone-interval}")
	protected Long evictionTimePostponeInterval;
	
	@Value("${odata-cat.query-timeout}")
	protected Integer queryTimeout;
}
