package com.exprivia.auxip.servlet;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.olingo.commons.api.format.ContentType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Controller;
import org.springframework.util.FileCopyUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.server.ResponseStatusException;

@Controller
public class ODataWebClientController {
	@Value("classpath:web/static/odata-cat-web-client.html")
	private Resource indexHtml;

	@Value("classpath:web/static/odata-cat-web-client-light-green.css")
	private Resource indexCss;

	@Value("classpath:web/static/prip-web-client.json")
	private Resource configJson;
	
	@Autowired
	private AUXIPConfig config;

	@RequestMapping(path="/webclient/index.html")
	public void handleHtmlClient(HttpServletRequest req, HttpServletResponse resp) throws UnsupportedEncodingException, IOException {
		if (config.isWebClientEnabled())
			setResponse(indexHtml, resp);
		else {
			throw new ResponseStatusException(HttpStatus.NOT_FOUND);
		}
	}

	@RequestMapping(path="/webclient/odata-cat-web-client.css")
	public void handleHtmlClientCss(HttpServletRequest req, HttpServletResponse resp) throws UnsupportedEncodingException, IOException {
		setResponse(indexCss, resp);
	}
	
	@RequestMapping(path="/webclient/odata-cat-web-client.json")
	public void handleHtmlClientConfig(HttpServletRequest req, HttpServletResponse resp) throws UnsupportedEncodingException, IOException {
		resp.setContentType(ContentType.APPLICATION_JSON.toContentTypeString());
		setResponse(configJson, resp);
	}
	
	private void setResponse(Resource resource, HttpServletResponse resp) throws UnsupportedEncodingException, IOException {
		resp.getWriter().append(
				FileCopyUtils.copyToString(new InputStreamReader(resource.getInputStream(), "UTF-8"))
				);		
	}
}
