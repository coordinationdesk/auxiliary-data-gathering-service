package com.exprivia.odc.service;

import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;
import org.springframework.stereotype.Component;

import lombok.AccessLevel;
import lombok.Data;
import lombok.experimental.FieldDefaults;

@Data
@Component("request-env")
@Scope(value="request", proxyMode=ScopedProxyMode.TARGET_CLASS)
@FieldDefaults(level = AccessLevel.PRIVATE)
public class ODCRequestEnvironment {
	String username;
}
