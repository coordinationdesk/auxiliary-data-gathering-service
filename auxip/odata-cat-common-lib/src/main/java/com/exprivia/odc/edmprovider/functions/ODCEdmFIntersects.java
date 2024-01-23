package com.exprivia.odc.edmprovider.functions;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlFunction;
import org.apache.olingo.commons.api.edm.provider.CsdlParameter;
import org.apache.olingo.commons.api.edm.provider.CsdlReturnType;

import com.exprivia.odc.edmprovider.ODCEdmFunction;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETProduct;

public class ODCEdmFIntersects implements ODCEdmFunction {
	
	public static final String FN_INTERSECTS_NAME = "Intersects";
	public static final FullQualifiedName FN_INTERSECTS_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, FN_INTERSECTS_NAME);
	public static final String PARAMETER_PRODUCT = "product"; 
	public static final String PARAMETER_AREA = "area"; 

	@Override
	public List<CsdlFunction> getFunctionList() {
		// Create function list
		List<CsdlFunction> functions = new ArrayList<CsdlFunction>();
		
		// Create parameters
		List<CsdlParameter> parameters = new ArrayList<>();

		// Create product parameter
		CsdlParameter parameterProduct = new CsdlParameter();
		parameterProduct.setName(PARAMETER_PRODUCT);
		parameterProduct.setType(ODCEdmETProduct.ET_FQN);
		parameterProduct.setCollection(true);
		
		parameters.add(parameterProduct);
		
		// Create product parameter
		CsdlParameter parameterArea = new CsdlParameter();
		parameterArea.setName(PARAMETER_AREA);
		parameterArea.setType(EdmPrimitiveTypeKind.Geography.getFullQualifiedName());
		parameterArea.setCollection(false);
		
		parameters.add(parameterArea);
		
		// create return type
		CsdlReturnType returnTypeIntersects = new CsdlReturnType();
		returnTypeIntersects.setCollection(false);
		returnTypeIntersects.setType(EdmPrimitiveTypeKind.Boolean.getFullQualifiedName());
		
		// create function
		CsdlFunction functionIntersects = new CsdlFunction();
		functionIntersects.setName(FN_INTERSECTS_NAME);
		functionIntersects.setParameters(parameters);
		functionIntersects.setBound(true);
		functionIntersects.setReturnType(returnTypeIntersects);
		
		// add Intersects to the list of functions
		functions.add(functionIntersects);
		
		return functions;
	}

}
