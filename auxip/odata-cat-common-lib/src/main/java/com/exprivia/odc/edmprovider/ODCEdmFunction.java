package com.exprivia.odc.edmprovider;

import java.util.List;

import org.apache.olingo.commons.api.edm.provider.CsdlFunction;

public interface ODCEdmFunction {
	List<CsdlFunction> getFunctionList(); 
}
