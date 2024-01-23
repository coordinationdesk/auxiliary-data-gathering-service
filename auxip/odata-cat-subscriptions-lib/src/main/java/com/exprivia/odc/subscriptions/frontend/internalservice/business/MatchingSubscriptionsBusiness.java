package com.exprivia.odc.subscriptions.frontend.internalservice.business;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.api.uri.queryoption.FilterOption;
import org.apache.olingo.server.api.uri.queryoption.expression.Expression;
import org.apache.olingo.server.api.uri.queryoption.expression.ExpressionVisitException;
import org.apache.olingo.server.core.uri.parser.UriParserException;
import org.apache.olingo.server.core.uri.validator.UriValidationException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.delegate.item.StorageItemProduct;
import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.filter.JDBCVisitor;
import com.exprivia.odc.servlet.ODCBaseServices;
import com.exprivia.odc.subscriptions.frontend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.frontend.data.dto.SubscriptionStatus;
import com.exprivia.odc.subscriptions.frontend.data.repositories.SubscriptionRepository;
import com.exprivia.odc.subscriptions.frontend.internalservice.business.exception.ItemNotFoundException;
import com.exprivia.odc.subscriptions.frontend.internalservice.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.frontend.internalservice.data.repositories.PendingInventoryRepository;
import com.exprivia.odc.util.ParseQueryFilter;
import com.exprivia.odc.util.Util;

@Component
public class MatchingSubscriptionsBusiness {
	@Autowired
	private PendingInventoryRepository pendingRepository;
	@Autowired
	private SubscriptionRepository subscriptionRepository;
	@Autowired
	private StorageItemProduct storageProduct;
	@Autowired
	private ODCBaseServices baseServices;
	@Autowired
	private SqlEditor sqlEditor;
	
	private static final Logger LOG = LoggerFactory.getLogger(MatchingSubscriptionsBusiness.class);
	
	@Component
	public class SqlEditor {
		public JDBCQueryParams editConditions(Subscription subscription, JDBCQueryParams queryParams) {
			/*
			String status = null;
			switch (subscription.getSubscriptionEvent()) {
			case CREATED:
				status = "INSERT";
				break;
			case DELETED:
				status = "DELETE";
				break;
			}
			JDBCQueryParams newQueryParams = Util.addArbitrarySQLCondition(
					queryParams, 
					"operation = ?", 
					Arrays.asList(status));
			*/
			return queryParams;
		}
	}

	public List<Long> getMatchingSubscriptions(long pendingInventoryId) throws ItemNotFoundException, ODCApiUnauthorizedException {
		PendingInventory pendingInventory = pendingRepository.findById(pendingInventoryId);
		if (pendingInventory == null) {
			throw new ItemNotFoundException("Pending inventory item not found");
		}

		List<Long> ret = new ArrayList<Long>();
		
		// setup the return list
		for (Subscription subscription : getMatchingSubscriptions(pendingInventory)) {
			ret.add(subscription.getUniqueId());
		}
		
		return ret;
	}
	
	protected List<Subscription> getMatchingSubscriptions(PendingInventory pendingInventory) {
		
		JDBCQueryParams params = Util.addArbitrarySQLCondition(null, "status = ?", Arrays.asList(SubscriptionStatus.RUNNING.name()));
		
		List<Subscription> subscriptions = subscriptionRepository.findWithParams(params, null, true);
		
		ArrayList<Subscription> ret = new ArrayList<Subscription>();
		// for each subscription:
		for (Subscription subscription : subscriptions) {
		
			try {
				// parse OData query
				UriInfo uriInfo = ParseQueryFilter.parseQueryFilter(baseServices, subscription.getFilterParam());
				
				// execute the filter and get back the matching pending inventories
				List<PendingInventory> products = executeFilter(
						uriInfo, 
						subscription,
						pendingInventory);
				
//				if (products.size() > 1) {
//					LOG.error("subscription {} more than one pending inv returned by filter {}", subscription.getId(), subscription.getFilterParam());
//				} else {
					if (products.size() == 1) {
						// if the pendingInventory is returned, add it to the return list
						ret.add(subscription);
					}
//				}
				
			} catch (UriParserException | UriValidationException | ExpressionVisitException | ODataApplicationException e) {
				LOG.warn("subscription {} filter '{}' error: {}", subscription.getId(), subscription.getFilterParam(), e.getMessage());
			}
			
		}
		
		return ret;
	}

	protected List<PendingInventory> executeFilter(UriInfo uriInfo, Subscription subscription, PendingInventory pendingInventory) throws ExpressionVisitException, ODataApplicationException {
		FilterOption filterOption = uriInfo.getFilterOption();
		if (filterOption != null) {
			Expression filterExpression = filterOption.getExpression();
			
			// convert the filter to sql
			JDBCFilterExpression jdbcFilterExpression = (JDBCFilterExpression) filterExpression.accept(
					new JDBCVisitor(
							// use storage product to work on t_srv_inventory fields
							storageProduct,
							baseServices.getServiceMetadata().getEdm()
							)
					);

			JDBCQueryParams queryParams = JDBCQueryParams.builder().jdbcFilterExpression(jdbcFilterExpression).build();
			// add condition to monitor only INSERT or DELETE t_srv_pendinginv records
			// add the condition to monitor only the current pendingInventory
			JDBCQueryParams newQueryParams = Util.addArbitrarySQLCondition(
					queryParams, 
					"uuid = ?", 
					Arrays.asList(pendingInventory.getUuid()));
			
			newQueryParams = sqlEditor.editConditions(subscription, newQueryParams);
			
			// search on pendingRepository to select on t_srv_pendinginventory
			return pendingRepository.findWithParams(newQueryParams, null);
		} else {
			return null;
		}
	}
}
