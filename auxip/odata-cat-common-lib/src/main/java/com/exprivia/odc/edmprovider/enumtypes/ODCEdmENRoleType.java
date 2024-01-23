package com.exprivia.odc.edmprovider.enumtypes;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlEnumMember;
import org.apache.olingo.commons.api.edm.provider.CsdlEnumType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.edmprovider.ODCEdmEnumType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.service.permissions.ODCSystemRole;
import com.exprivia.odc.service.permissions.ODCSystemRoleList;

@Component
public class ODCEdmENRoleType implements ODCEdmEnumType {
	public static final String EN_NAME = "RoleType";
	public static final FullQualifiedName EN_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, EN_NAME);
	@Autowired
	protected ODCSystemRoleList systemRoleList;
	
	@Override
	public CsdlEnumType getCsdlEnumType() {
		
		List<CsdlEnumMember> enumMembers = new ArrayList<CsdlEnumMember>();
		for (ODCSystemRole value : systemRoleList.getValues()) {
			enumMembers.add(
					new CsdlEnumMember()
					.setName(value.getName())
					.setValue(String.valueOf(value.getOrdinal()))
					);
		}
		
		return new CsdlEnumType()
				.setName(EN_NAME)
				.setMembers(enumMembers)
				.setUnderlyingType(EdmPrimitiveTypeKind.Int64.getFullQualifiedName());
	}
}
