package com.exprivia.odc.internalservice.data.repositories;

import com.exprivia.odc.internalservice.data.dto.AipContent;

public interface AipContentRepository {
	/**
	 * @deprecated

	 * Returns a single record for an AIP content matching the ProductUUID,
	 * even if the DB tables can store multiple results
	 * 
	 * @param productUuid
	 * @return
	 */
	AipContent findByProductUuid(String productUuid);
	
	/**
	 * Returns the matching AIPContents by ProductUUID, AIPID
	 * (unique constraint upon ProductUUID, AIPID)
	 * @param productUuid
	 * @return
	 */
	AipContent findByProductUuidAndAipId(String productUuid, Long aipId);
	
	/**
	 * Returns the unique record matching the generator order id
	 * (unique constraint upon OrderID)
	 * @param orderUuid
	 * @return
	 */
	AipContent findByOrderUuid(String orderUuid);
	
	DownloadStatus checkDownloadStatus(String productId);
	
	public enum DownloadStatus {
		PRODUCT_NOT_EXISTS,					// 400 Bad Request (Product Id is not known in the LTA)
		ORDER_NOT_SCHEDULED,				// 404 Not Found (Product is not online)
		ORDER_SCHEDULED_PRODUCT_NOT_READY,	// 202 Accepted (Product is being retrieved as part of an existing order but is not yet online)
		PRODUCT_DOWNLOADING_OR_READY		// 200 OK
	}
}
