package com.exprivia.odc.edmprovider;

import java.util.List;

import org.apache.olingo.commons.api.edm.FullQualifiedName;

/**
 * @author andrea
 *
 * Creates objects delegated to provide OData schema parts for ODC api
 *
 */
public interface ODCEdmFactory {
	public ODCEdmEntityType createEntityType(FullQualifiedName entityTypeName);
	
	public ODCEdmEntitySet createEntitySet(FullQualifiedName entityContainer, String entitySetName);
	
	public ODCEdmComplexType createComplexType(final FullQualifiedName complexTypeName);
	
	public ODCEdmEnumType createEnumType(final FullQualifiedName enumTypeName);
	
	public ODCEdmEntitySet createMainEntitySet();
	
	public ODCEdmAction createAction(FullQualifiedName actionName);
	
	public ODCEdmFunction createFunction(FullQualifiedName functionName);
	
	public ODCEdmActionImport createActionImport(FullQualifiedName actionName);
	
	public List<FullQualifiedName> getEntityTypes();
	
	public List<FullQualifiedName> getComplexTypes();
	
	public List<FullQualifiedName> getActions();
	
	public List<FullQualifiedName> getFunctions();
	
	public List<FullQualifiedName> getActionImports();
	
	public List<FullQualifiedName> getEnumTypes();
}
