package com.exprivia.odc.data.repositories;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.dto.Product;
import com.exprivia.odc.data.repositories.queryhelpers.ODCProductQueryHelper;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;

@Repository
public class ProductRepositoryJDBCImpl implements ProductRepository {

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCActionsPermissions actionsPermissions;
	
	private ODCProductQueryHelper productQueryHelper;

	public ProductRepositoryJDBCImpl(
			@Autowired DBTableTranslator tableTranslator,
			@Autowired ODCDBDefaults odcConfig
			) {
		productQueryHelper = new ODCProductQueryHelper(tableTranslator, odcConfig);
	}
	
	@Override
	public Product findById(String id) {
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.PRODUCT_FIND_BY_ID.getCode());
		
		return productQueryHelper.findById(jdbcTemplate, id);
	}

	@Override
	public List<Product> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.PRODUCT_FIND_WITH_PARAMS.getCode());
		
		return productQueryHelper.findWithParams(jdbcTemplate, jdbcQuery, orderByColumns);
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		return productQueryHelper.getRecordsCount(jdbcTemplate, jdbcQuery);
	};
	
	public String convertColumnName(String name) {
		return productQueryHelper.convertColumnName(name);
	}
	
	public void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand) {
		productQueryHelper.processJsonCondition(expression, fieldName, sqlCondition, operand);
	}
}
