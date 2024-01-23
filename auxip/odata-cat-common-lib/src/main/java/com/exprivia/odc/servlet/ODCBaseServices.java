package com.exprivia.odc.servlet;

import java.util.ArrayList;

import org.apache.olingo.commons.api.edmx.EdmxReference;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ODataHttpHandler;
import org.apache.olingo.server.api.ServiceMetadata;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Lazy;
import org.springframework.stereotype.Component;

import com.exprivia.odc.edmprovider.ODCEdmFactory;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.service.processor.ODCActionProcessor;
import com.exprivia.odc.service.processor.ODCComplexCollectionProcessor;
import com.exprivia.odc.service.processor.ODCEntityCollectionProcessor;
import com.exprivia.odc.service.processor.ODCEntityProcessor;
import com.exprivia.odc.service.processor.ODCPrimitiveProcessor;
import com.exprivia.odc.util.ODCParseUri;

import lombok.Getter;

@Component
@Getter
public class ODCBaseServices {
	@Autowired
	@Lazy
	private ODCEntityCollectionProcessor entityCollectionProcessor;

	@Autowired
	@Lazy
	private ODCEntityProcessor entityProcessor;

	@Autowired
	@Lazy
	private ODCPrimitiveProcessor primitiveProcessor;

	@Autowired
	@Lazy
	private ODCComplexCollectionProcessor complexCollectionProcessor;

	@Autowired
	@Lazy
	private ODCActionProcessor actionProcessor;

	protected OData odata;
	protected ServiceMetadata serviceMetadata;
	protected ODataHttpHandler httpHandler;
	
	@Autowired
	protected ODCParseUri uriParser;
	
	public ODCBaseServices(@Autowired ODCEdmFactory edmFactory) {
		odata = OData.newInstance();
		serviceMetadata = odata.createServiceMetadata(new ODCEdmProvider(edmFactory), new ArrayList<EdmxReference>());
		
		httpHandler = odata.createHandler(serviceMetadata);
	}
	
	public void registerDefaultProcessors() {
		httpHandler.register(entityCollectionProcessor);
		httpHandler.register(entityProcessor);
		httpHandler.register(primitiveProcessor);
		httpHandler.register(complexCollectionProcessor);
		httpHandler.register(actionProcessor);
	}
}
