package com.exprivia.auxip.data.repositories;

import java.util.List;

import com.exprivia.auxip.data.dto.AUXIPProduct;
import com.exprivia.odc.data.repositories.CountableRepository;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface AUXIPProductRepository extends CountableRepository {

	AUXIPProduct findById(String id);

	List<AUXIPProduct> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
	
	public void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand);
}
