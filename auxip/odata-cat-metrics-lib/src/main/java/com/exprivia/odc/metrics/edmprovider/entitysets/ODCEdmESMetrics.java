package com.exprivia.odc.metrics.edmprovider.entitysets;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.metrics.edmprovider.entitytypes.ODCEdmETMetric;

public class ODCEdmESMetrics implements ODCEdmEntitySet
{
	public static final String ES_NAME = "Metrics";

	@Override
	public CsdlEntitySet getCsdlEntitySet()
	{
		CsdlEntitySet entitySet = new CsdlEntitySet();

		entitySet.setName(ES_NAME);
		entitySet.setType(ODCEdmETMetric.ET_FQN);

		return entitySet;
	}
}