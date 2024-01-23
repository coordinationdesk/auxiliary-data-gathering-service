package com.exprivia.auxip.servlet;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.springframework.context.annotation.Primary;
import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;
import org.springframework.stereotype.Component;

import com.exprivia.odc.servlet.ODCCurrentHttpRequest;

@Component
@Primary
@Scope(value="request", proxyMode=ScopedProxyMode.TARGET_CLASS)
public class AUXIPCurrentHttpRequest extends ODCCurrentHttpRequest {

	final static protected List<String> auxipAllowedArguments = Arrays.asList(
		    "dwh");
	
	static protected List<String> allowedArguments = null;
	
	@Override
	protected List<String> getAllowedArguments() {
		if (allowedArguments == null) {
			allowedArguments = new ArrayList<>(super.getAllowedArguments());
			for (String item : auxipAllowedArguments) {
				allowedArguments.add(item);
			}
		}
		return allowedArguments;
	}
	
}
