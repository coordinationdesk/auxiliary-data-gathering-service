package com.exprivia.odc.data.repositories;

import java.util.List;

import com.exprivia.odc.data.dto.Product;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface ProductRepository extends CountableRepository {

	Product findById(String id);

	List<Product> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
	
	
	void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand);
}
