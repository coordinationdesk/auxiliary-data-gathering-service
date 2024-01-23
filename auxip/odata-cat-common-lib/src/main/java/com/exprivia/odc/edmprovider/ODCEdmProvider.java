package com.exprivia.odc.edmprovider;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.provider.CsdlAbstractEdmProvider;
import org.apache.olingo.commons.api.edm.provider.CsdlAction;
import org.apache.olingo.commons.api.edm.provider.CsdlActionImport;
import org.apache.olingo.commons.api.edm.provider.CsdlComplexType;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityContainer;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityContainerInfo;
import org.apache.olingo.commons.api.edm.provider.CsdlEntitySet;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlEnumType;
import org.apache.olingo.commons.api.edm.provider.CsdlFunction;
import org.apache.olingo.commons.api.edm.provider.CsdlSchema;

/**
 * @author andrea
 * 
 * Provides easy access to the OData schema parts
 *
 */
public class ODCEdmProvider extends CsdlAbstractEdmProvider {
	protected ODCEdmFactory edmFactory = null;
	
	// Service Namespace
	public static final String NAMESPACE = "OData.CSC";

	// EDM Container
	public static final String CONTAINER_NAME = "Container";
	public static final FullQualifiedName CONTAINER = new FullQualifiedName(NAMESPACE, CONTAINER_NAME);


	public ODCEdmProvider(ODCEdmFactory edmFactory) {
		super();
		
		this.edmFactory = edmFactory;
	}

	@Override
	public CsdlEnumType getEnumType(FullQualifiedName enumTypeName) {
		ODCEdmEnumType odcEdmEnumType = edmFactory.createEnumType(enumTypeName);
		if (odcEdmEnumType != null)
			return odcEdmEnumType.getCsdlEnumType();
		else
			return null;
	}
	
	@Override
	public CsdlEntityType getEntityType(FullQualifiedName entityTypeName) {
		// this method is called for one of the EntityTypes that are configured in the Schema
		ODCEdmEntityType odcEdmEntityType = edmFactory.createEntityType(entityTypeName);
		if (odcEdmEntityType != null)
			return odcEdmEntityType.getCsdlEntityType();
		else
			return null;
	}

	@Override
	public CsdlEntitySet getEntitySet(FullQualifiedName entityContainer, String entitySetName) {
		ODCEdmEntitySet odcEdmEntitySet = edmFactory.createEntitySet(entityContainer, entitySetName);
		if (odcEdmEntitySet != null)
			return odcEdmEntitySet.getCsdlEntitySet();
		else
			return null;
	}

	@Override
	public CsdlComplexType getComplexType(final FullQualifiedName complexTypeName) {
		ODCEdmComplexType odcEdmComplexType = edmFactory.createComplexType(complexTypeName);
		if (odcEdmComplexType != null)
			return odcEdmComplexType.getCsdlComplexType();
		else
			return null;
	}
	
	@Override
	public List<CsdlAction> getActions(final FullQualifiedName actionName) {
		ODCEdmAction action = edmFactory.createAction(actionName);
		if (action != null)
			return action.getActionList();
		else
			return null;
	}
	
	@Override
	public CsdlActionImport getActionImport(FullQualifiedName entityContainer, String actionImportName) {
		ODCEdmActionImport actionImport = edmFactory.createActionImport(new FullQualifiedName(NAMESPACE, actionImportName));
		if (actionImport != null)
			return actionImport.getActionImport();
		else
			return null;
	}
	
	@Override
	public List<CsdlFunction> getFunctions(FullQualifiedName functionName) {
		ODCEdmFunction function = edmFactory.createFunction(functionName);
		if (function != null)
			return function.getFunctionList();
		else
			return null;
	}
	
	public CsdlEntityContainerInfo getEntityContainerInfo(FullQualifiedName entityContainerName) {
		// This method is invoked when displaying the Service Document at e.g. http://localhost:8080/DemoService/DemoService.svc
		if (entityContainerName == null || entityContainerName.equals(CONTAINER)) {
			CsdlEntityContainerInfo entityContainerInfo = new CsdlEntityContainerInfo();
			entityContainerInfo.setContainerName(CONTAINER);
			return entityContainerInfo;
		}

		return null;
	}

	public CsdlEntityContainer getEntityContainer() {
		// create EntitySets
		List<CsdlEntitySet> entitySets = new ArrayList<CsdlEntitySet>();
		entitySets.add(edmFactory.createMainEntitySet().getCsdlEntitySet());

		// create EntityContainer
		CsdlEntityContainer entityContainer = new CsdlEntityContainer();
		entityContainer.setName(CONTAINER_NAME);
		entityContainer.setEntitySets(entitySets);
		
		// add action imports
		List<CsdlActionImport> actionImports = new ArrayList<CsdlActionImport>();
		for (FullQualifiedName actionImportFqn : edmFactory.getActionImports()) {
			actionImports.add(edmFactory.createActionImport(actionImportFqn).getActionImport());
		}
		
		entityContainer.setActionImports(actionImports);

		return entityContainer;
	}

	public List<CsdlSchema> getSchemas() {
		// create Schema
		CsdlSchema schema = new CsdlSchema();
		schema.setNamespace(NAMESPACE);

		// add EntityTypes
		List<CsdlEntityType> entityTypes = new ArrayList<CsdlEntityType>();
		for (FullQualifiedName entityType: edmFactory.getEntityTypes()) {
			entityTypes.add(getEntityType(entityType));
		}
		schema.setEntityTypes(entityTypes);

		// add ComplexTypes
		List<CsdlComplexType> complexTypes = new ArrayList<>();
		for (FullQualifiedName complexType: edmFactory.getComplexTypes()) {
			complexTypes.add(getComplexType(complexType));
		}
		schema.setComplexTypes(complexTypes);
		
		// add Actions
		List<CsdlAction> actions = new ArrayList<>();
		for (FullQualifiedName action : edmFactory.getActions()) {
			actions.addAll(getActions(action));
		}
		schema.setActions(actions);
		
		// add Functions
		List<CsdlFunction> functions = new ArrayList<>();
		for (FullQualifiedName function : edmFactory.getFunctions()) {
			functions.addAll(getFunctions(function));
		}
		schema.setFunctions(functions);
		
		List<CsdlEnumType> enumTypes = new ArrayList<>();
		for (FullQualifiedName enumType : edmFactory.getEnumTypes()) {
			enumTypes.add(getEnumType(enumType));
		}
		schema.setEnumTypes(enumTypes);

		// add EntityContainer
		schema.setEntityContainer(getEntityContainer());

		// finally
		List<CsdlSchema> schemas = new ArrayList<CsdlSchema>();
		schemas.add(schema);

		return schemas;
	}
	
}
