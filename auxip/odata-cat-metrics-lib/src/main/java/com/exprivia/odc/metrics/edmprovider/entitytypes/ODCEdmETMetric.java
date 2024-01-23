package com.exprivia.odc.metrics.edmprovider.entitytypes;

import java.util.Arrays;
import java.util.Collections;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.apache.olingo.commons.api.edm.EdmPrimitiveTypeKind;

import org.apache.olingo.commons.api.edm.provider.CsdlProperty;
import org.apache.olingo.commons.api.edm.provider.CsdlEntityType;
import org.apache.olingo.commons.api.edm.provider.CsdlPropertyRef;

import com.exprivia.odc.metrics.data.dto.Metric;
import com.exprivia.odc.metrics.edmprovider.enumtypes.ODCEdmENMetricType;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;

public class ODCEdmETMetric implements ODCEdmEntityType
{
	public static final String ET_NAME           = "Metric";
	public static final FullQualifiedName ET_FQN = new FullQualifiedName(ODCEdmProvider.NAMESPACE, ET_NAME);

	@Override
	public CsdlEntityType getCsdlEntityType()
	{
		// Create EntityType Properties
		CsdlProperty name       = new CsdlProperty().setName(Metric.FIELD_NAME).setType       (EdmPrimitiveTypeKind.String.getFullQualifiedName()).setNullable        (false);
		CsdlProperty timestamp  = new CsdlProperty().setName(Metric.FIELD_TSTAMP).setType     (EdmPrimitiveTypeKind.DateTimeOffset.getFullQualifiedName()).setNullable(false);
		CsdlProperty metricType = new CsdlProperty().setName(Metric.FIELD_METRIC_TYPE).setType(ODCEdmENMetricType.EN_FQN).setNullable        (false);
		CsdlProperty gauge      = new CsdlProperty().setName(Metric.FIELD_GAUGE).setType      (EdmPrimitiveTypeKind.Double.getFullQualifiedName()).setNullable        (true);
		CsdlProperty counter    = new CsdlProperty().setName(Metric.FIELD_COUNTER).setType    (EdmPrimitiveTypeKind.Int64.getFullQualifiedName()).setNullable         (true);

		// Create CsdlPropertyRef for Key Element
		CsdlPropertyRef propertyRef = new CsdlPropertyRef();
		                propertyRef.setName(Metric.FIELD_NAME);

		// configure EntityType
		CsdlEntityType entityType = new CsdlEntityType();

		entityType.setName      (ET_NAME);
		entityType.setProperties(Arrays.asList(name, timestamp, metricType, gauge, counter));
		entityType.setKey       (Collections.singletonList(propertyRef));

		return entityType;
	}
}