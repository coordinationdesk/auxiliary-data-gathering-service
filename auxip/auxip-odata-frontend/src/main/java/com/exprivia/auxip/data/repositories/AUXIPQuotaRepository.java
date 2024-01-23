package com.exprivia.auxip.data.repositories;

import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.data.repositories.QuotaRepositoryJDBCImpl;

@Repository
@Primary
public class AUXIPQuotaRepository extends QuotaRepositoryJDBCImpl {
	private final static String [] quotaFields = {
			"'TotalDownloadsQuota' tp, 		downloadinterval itv, 	downloadsize sz",
			"'ParallelDownloadsQuota' tp, 	null::interval itv, 	downloadparallel sz"
		};
	
	protected String[] getQuotaFields() {
		return quotaFields;
	}
}
