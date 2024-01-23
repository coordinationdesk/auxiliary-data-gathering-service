package com.exprivia.odc.metrics.delegate.item;

import java.util.List;

import org.apache.olingo.commons.api.data.Entity;
import org.apache.olingo.commons.api.data.EntityCollection;
import org.apache.olingo.commons.api.data.Property;
import org.apache.olingo.commons.api.data.ValueType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.item.StorageItem;
import com.exprivia.odc.metrics.data.dto.Metric;
import com.exprivia.odc.metrics.data.repositories.MetricRepositoryJDBCImpl;
import com.exprivia.odc.service.filter.JDBCQueryParams;

@Component
public class StorageItemMetric implements StorageItem<Metric>
{
	@Autowired
	private MetricRepositoryJDBCImpl metricRepository;

	public Entity getById(String id)
	{
		Entity entity = null;

		Metric metric = metricRepository.findById(id);

		if (metric != null)
		{
			entity = makeEntity(metric);
		}

		return entity;
	}

	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount)
	{
		EntityCollection ec = makeEntityCollection(metricRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(metricRepository.getRecordsCount(queryParams));
		return ec;
	}

	/*
	 * Here the answer is built!
	 * */
	public Entity makeEntity(Metric metric)
	{
		Entity entity = new Entity();

		entity.addProperty(new Property(null, Metric.FIELD_ID,          ValueType.PRIMITIVE, metric.getId()))
			  .addProperty(new Property(null, Metric.FIELD_NAME,        ValueType.PRIMITIVE, metric.getName()))
			  .addProperty(new Property(null, Metric.FIELD_TSTAMP,      ValueType.PRIMITIVE, metric.getTimestamp()))
			  .addProperty(new Property(null, Metric.FIELD_METRIC_TYPE, ValueType.PRIMITIVE, metric.getMetricType().ordinal()));

		if (metric.getGauge() != null)
		{
			entity.addProperty(new Property(null, Metric.FIELD_GAUGE, ValueType.PRIMITIVE, metric.getGauge()));
		}

		if (metric.getCounter() != null)
		{
			entity.addProperty(new Property(null, Metric.FIELD_COUNTER, ValueType.PRIMITIVE, metric.getCounter()));
		}

		return entity;
	}

	@Override
	public String convertColumnName(String name)
	{
		return metricRepository.convertColumnName(name);
	}

}
