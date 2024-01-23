package com.exprivia.odc.subscriptions.frontend.data.repositories;

import java.util.List;

import com.exprivia.odc.data.repositories.CountableRepository;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.subscriptions.frontend.data.dto.Notification;

public interface NotificationRepository extends CountableRepository {

	List<Notification> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}
