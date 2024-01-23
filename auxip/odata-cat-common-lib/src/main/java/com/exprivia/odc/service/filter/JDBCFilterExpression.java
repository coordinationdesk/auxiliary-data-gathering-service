package com.exprivia.odc.service.filter;

import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.server.api.uri.queryoption.expression.BinaryOperatorKind;

public class JDBCFilterExpression {
	private String preparedWhere;
	private List<Object> parameters;
	private BinaryOperatorKind operator;

	static public JDBCFilterExpression clone(JDBCFilterExpression copyFrom) {
		JDBCFilterExpression filter = new JDBCFilterExpression(null);
		
		filter.operator = copyFrom.operator;
		filter.preparedWhere = copyFrom.preparedWhere;
		filter.parameters = new ArrayList<Object>();
		for (Object parameter : copyFrom.parameters) {
			filter.parameters.add(parameter);
		}
		return filter;
	}
	
	public JDBCFilterExpression(final BinaryOperatorKind operator) {
		preparedWhere = "";
		parameters = new ArrayList<Object>();
		this.operator = operator;
	}

	public void addParameter(final Object parameter) {
		parameters.add(parameter);
	}

	public void setPrepeparedWhere(final String where) {
		preparedWhere = where;
	}
	
	public String getPreparedWhere() {
		return preparedWhere;
	}

	public List<Object> getParameters() {
		return parameters;
	}

	public BinaryOperatorKind getOperator() {
		return operator;
	}

	@Override
	public String toString() {
		return preparedWhere;
	}
}