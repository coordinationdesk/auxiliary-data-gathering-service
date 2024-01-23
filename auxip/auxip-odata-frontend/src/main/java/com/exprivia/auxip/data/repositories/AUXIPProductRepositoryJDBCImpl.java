package com.exprivia.auxip.data.repositories;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Primary;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.exprivia.auxip.data.dto.AUXIPProduct;
import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;

@Repository
@Primary
public class AUXIPProductRepositoryJDBCImpl implements AUXIPProductRepository {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCActionsPermissions actionsPermissions;
	
	private AUXIPProductQueryHelper queryHelper;

	private final static Logger LOG = LoggerFactory.getLogger(AUXIPProductRepositoryJDBCImpl.class);
	
	public AUXIPProductRepositoryJDBCImpl(
			@Autowired DBTableTranslator tableTranslator, 
			@Autowired ODCDBDefaults odcConfig) {
		this.queryHelper = new AUXIPProductQueryHelper(tableTranslator, odcConfig);
	}

	public AUXIPProduct findById(String id) {
		LOG.warn("called AUXIP product repository");
		
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.PRODUCT_FIND_BY_ID.getCode());
		
		return queryHelper.findById(jdbcTemplate, id);
	}
	
	public List<AUXIPProduct> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.PRODUCT_FIND_WITH_PARAMS.getCode());
		
		return queryHelper.findWithParams(jdbcTemplate, jdbcQuery, orderByColumns);
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		return queryHelper.getRecordsCount(jdbcTemplate, jdbcQuery);
	}

	public String convertColumnName(String name) {
		return queryHelper.convertColumnName(name);
	}
	
	@Override
	public void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand) {
		queryHelper.processJsonCondition(expression, fieldName, sqlCondition, operand);
	}
}
