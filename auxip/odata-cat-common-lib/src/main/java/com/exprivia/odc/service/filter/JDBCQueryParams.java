package com.exprivia.odc.service.filter;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;
import lombok.experimental.FieldDefaults;

@Builder(toBuilder = true)
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class JDBCQueryParams {

	@Getter
	@Setter
	private JDBCFilterExpression jdbcFilterExpression;

	@Getter
	@Setter
	Integer limitNumber;

	@Getter
	@Setter
	Integer offsetNumber;
	
	public long getSafeQueryLimit(long maximumItems) {
		if (limitNumber == null)
			return maximumItems;
		else
			return Long.min(maximumItems, limitNumber);
	}

}
