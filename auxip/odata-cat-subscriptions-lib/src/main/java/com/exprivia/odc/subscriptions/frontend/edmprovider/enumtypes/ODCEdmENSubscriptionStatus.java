package com.exprivia.odc.subscriptions.frontend.edmprovider.enumtypes;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEnumMember;
import org.apache.olingo.commons.api.edm.provider.CsdlEnumType;

import com.exprivia.odc.edmprovider.ODCEdmEnumType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.subscriptions.frontend.data.dto.SubscriptionStatus;

public class ODCEdmENSubscriptionStatus implements ODCEdmEnumType {
	public static final String EN_NAME = "SubscriptionStatus";
	public static final FullQualifiedName EN_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, EN_NAME);
	
	@Override
	public CsdlEnumType getCsdlEnumType() {
		
		List<CsdlEnumMember> enumMembers = new ArrayList<CsdlEnumMember>();
		for (SubscriptionStatus value : SubscriptionStatus.values()) {
			enumMembers.add(
					new CsdlEnumMember()
					.setName(value.getDescription())
					.setValue(String.valueOf(value.ordinal()))
					);
		}
		
		return new CsdlEnumType()
				.setName(EN_NAME)
				.setMembers(enumMembers)
				.setUnderlyingType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName());
	}
}
