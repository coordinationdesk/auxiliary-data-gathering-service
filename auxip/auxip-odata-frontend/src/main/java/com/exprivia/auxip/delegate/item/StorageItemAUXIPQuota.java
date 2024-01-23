package com.exprivia.auxip.delegate.item;

import java.util.List;

import org.apache.olingo.commons.api.data.EntityCollection;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.auxip.data.repositories.AUXIPQuotaRepository;
import com.exprivia.odc.delegate.item.StorageItemQuota;
import com.exprivia.odc.service.filter.JDBCQueryParams;

@Component
public class StorageItemAUXIPQuota extends StorageItemQuota {
	@Autowired
	AUXIPQuotaRepository auxipQuotaRepository;

	@Override
	public EntityCollection findByFilter(JDBCQueryParams queryParams, List<String> orderByColumns, boolean withCount) {
		EntityCollection ec = makeEntityCollection(auxipQuotaRepository.findWithParams(queryParams, orderByColumns));
		if (withCount)
			ec.setCount(auxipQuotaRepository.getRecordsCount(queryParams));
		return ec;
	}
}
