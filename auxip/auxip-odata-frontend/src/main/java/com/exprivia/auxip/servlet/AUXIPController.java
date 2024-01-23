package com.exprivia.auxip.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

import com.exprivia.auxip.internalservice.processor.MatchingSubscriptionsProcessor;
import com.exprivia.auxip.internalservice.processor.VersionProcessor;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.servlet.request.ODCECSCustomServletRequest;

@Controller
@ComponentScan(value = "com.exprivia.odc")
public class AUXIPController {
	private static final Logger LOG = LoggerFactory.getLogger(AUXIPController.class);
	
	@Autowired
	private MatchingSubscriptionsProcessor matchingSubscriptionsProcessor;
	
	@Autowired
	private VersionProcessor versionProcessor;
	
	@Autowired
	private AUXIPConfig auxipConfig;
	
	/**
	 * This object initializes the user data, permissions and fixes the requestUri
	 */
	@Autowired
	private AUXIPCurrentHttpRequest currentRequest;
	
	@Autowired
	private AUXIPDbTables dbTables;
	
	@Autowired
	private DBTableTranslator dbTableTranslator;
	
	@RequestMapping(path="/odata/v1/**")
	public void handle(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		try {
			// https://stackoverflow.com/questions/46074131/spring-boot-test-not-recognizing-servlet
			// https://www.broadleafcommerce.com/blog/configuring-a-dynamic-context-path-in-spring-boot
			
			// initialize the current request
			currentRequest.initializeRequest(req);
			ODCECSCustomServletRequest newRequest = currentRequest.getCurrentServletRequest();
			
			// process request
			currentRequest.logRequest();
			
			if (currentRequest.checkRequestParams() == true) {
			
				LOG.info("original filter: {}", req.getParameter("$filter"));
				LOG.info("patched  filter: {}", newRequest.getParameter("$filter"));
	
				synchronized (this) {
					String dwhParam = currentRequest.getCurrentServletRequest().getParameter("dwh");
					if (dwhParam != null && dwhParam.equalsIgnoreCase("true")) {
						LOG.info("using DWH tables");
						dbTables.setDWH(true);
						dbTableTranslator.setQueryCaching(false);
					}
					
					auxipConfig.getODataHttpHandler().process(newRequest, resp);
					
					dbTables.setDWH(false);
					dbTableTranslator.setQueryCaching(true);
				}
			
			} else {
				resp.setStatus(HttpStatusCode.BAD_REQUEST.getStatusCode());
				LOG.error("bad parameters detected");
			}
			
			currentRequest.logResponse(resp);
			
		} catch (RuntimeException e) {
			LOG.error("Server Error", e);
			throw new ServletException(e);
		}
	}
	
	@RequestMapping(path="/odata/v1/matchingSubscriptions")
	public void handleMatchingSubscriptions(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		try {
			currentRequest.initializeRequest(req);

			currentRequest.logRequest();
			
			// process request
			matchingSubscriptionsProcessor.processRequest(req, resp);
			
			currentRequest.logResponse(resp);
			
		} catch (RuntimeException e) {
			LOG.error("Server Error", e);
			throw new ServletException(e);			
		}
	}
	
	@RequestMapping(path="/odata/v1/version")
	public void handleVersion(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		try {
			currentRequest.initializeRequest(req);
			
			currentRequest.logRequest();
			
			// process request
			versionProcessor.processRequest(currentRequest.getCurrentServletRequest(), resp);
			
			currentRequest.logResponse(resp);
			
		} catch (RuntimeException e) {
			LOG.error("Server Error", e);
			throw new ServletException(e);
		}
	}
}
