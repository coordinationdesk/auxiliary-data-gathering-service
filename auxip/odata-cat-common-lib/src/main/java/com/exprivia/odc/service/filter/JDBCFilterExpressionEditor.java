package com.exprivia.odc.service.filter;

import java.time.Duration;

import org.apache.olingo.commons.api.edm.EdmProperty;
import org.apache.olingo.server.api.uri.queryoption.expression.BinaryOperatorKind;

import com.exprivia.odc.delegate.item.StorageItem;

public class JDBCFilterExpressionEditor extends JDBCFilterExpression {
	private StorageItem<?> storageItem;

	public JDBCFilterExpressionEditor(BinaryOperatorKind operator, StorageItem<?> storageItem) {
		super(operator);
		this.storageItem = storageItem;
	}

	public JDBCFilterExpressionEditor addParameterInSql(Object param) {
		String condition = getPreparedWhere();
		
		if (param instanceof JDBCFilterExpression) {
			for (Object innerParam : ((JDBCFilterExpression)param).getParameters()) {
				addParameter(innerParam);
			}
			condition += ((JDBCFilterExpression)param).getPreparedWhere();
		} else if (param instanceof EdmProperty) {
			/*
			 * TODO this happens when a MethodCall is performed on a value inside an attribute lambda
			if (storageItem.isJsonProperty(((EdmProperty)param).getName())) {
				storageItem.processJsonCondition(expression, ((EdmProperty)param).getName(), 
						sqlOperator, right);
			} else {
			*/
				condition += storageItem.convertColumnName(((EdmProperty)param).getName());
			/*}*/
		} else if (param instanceof Duration) {
			/*
			 * JdbcTemplate + PostgreSql don't manage parametric interval fields,
			 * a conversion from the string value is mandatory 
			 */
			condition += "?::interval";
			addParameter(((Duration)param).toString());
		} else {
			condition += "?";
			addParameter(param);
		}
		setPrepeparedWhere(condition);
		
		return this;
	}
	
	public JDBCFilterExpressionEditor addSql(String sqlFragment) {
		setPrepeparedWhere(getPreparedWhere() + sqlFragment);
		
		return this;
	}
}
