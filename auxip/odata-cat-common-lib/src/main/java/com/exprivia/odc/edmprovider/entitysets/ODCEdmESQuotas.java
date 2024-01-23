package com.exprivia.odc.edmprovider.entitysets;

import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;

import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETQuota;

public class ODCEdmESQuotas implements ODCEdmEntitySet {
	public static final String ES_NAME = "Quotas";

	@Override
	public CsdlEntitySet getCsdlEntitySet() {
		CsdlEntitySet entitySet = new CsdlEntitySet();
		
		entitySet.setName(ODCEdmESQuotas.ES_NAME);
		entitySet.setType(ODCEdmETQuota.ET_FQN);

		return entitySet;
	}
	
	
}
