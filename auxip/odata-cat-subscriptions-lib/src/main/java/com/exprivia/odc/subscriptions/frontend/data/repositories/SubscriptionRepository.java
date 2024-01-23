package com.exprivia.odc.subscriptions.frontend.data.repositories;

import java.util.List;

import com.exprivia.odc.data.repositories.CountableRepository;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.subscriptions.frontend.data.dto.Subscription;

public interface SubscriptionRepository extends CountableRepository {

	Subscription findById(String id);

	List<Subscription> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns, boolean internalService);
	
	Subscription insert(Subscription subscriptionToCreate);
}
