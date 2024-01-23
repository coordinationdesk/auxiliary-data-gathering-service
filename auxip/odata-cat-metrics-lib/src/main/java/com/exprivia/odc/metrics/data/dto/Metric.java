package com.exprivia.odc.metrics.data.dto;

import java.sql.Timestamp;

import lombok.Data;
import lombok.Builder;
import lombok.NonNull;
import lombok.AccessLevel;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Metric
{
	public final static String FIELD_ID          = "Id";
	public final static String FIELD_NAME        = "Name";
	public final static String FIELD_TSTAMP      = "Timestamp";
	public final static String FIELD_METRIC_TYPE = "MetricType";
	public final static String FIELD_GAUGE       = "Gauge";
	public final static String FIELD_COUNTER     = "Counter";

	@NonNull
	Long id;

	@NonNull
	String name;

	@NonNull
	Timestamp timestamp;

	@NonNull
	MetricType metricType;

	Double gauge;

	Long counter;
}