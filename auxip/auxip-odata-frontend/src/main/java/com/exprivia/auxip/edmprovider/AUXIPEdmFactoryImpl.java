package com.exprivia.auxip.edmprovider;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.springframework.beans.factory.annotation.Autowired;

import com.exprivia.auxip.edmprovider.entitysets.AUXIPEdmESProducts;
import com.exprivia.auxip.edmprovider.entitytypes.AUXIPEdmETProduct;
import com.exprivia.odc.edmprovider.ODCEdmAction;
import com.exprivia.odc.edmprovider.ODCEdmActionImport;
import com.exprivia.odc.edmprovider.ODCEdmComplexType;
import com.exprivia.odc.edmprovider.ODCEdmEntitySet;
import com.exprivia.odc.edmprovider.ODCEdmEntityType;
import com.exprivia.odc.edmprovider.ODCEdmEnumType;
import com.exprivia.odc.edmprovider.ODCEdmFactory;
import com.exprivia.odc.edmprovider.ODCEdmFunction;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.edmprovider.actions.ODCEdmAProductsFilterList;
import com.exprivia.odc.edmprovider.complextypes.ODCEdmCTCheckSum;
import com.exprivia.odc.edmprovider.complextypes.ODCEdmCTTimeRange;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESAttributes;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESQuotas;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESSystemRoles;
import com.exprivia.odc.edmprovider.entitysets.ODCEdmESUsers;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETAttribute;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETDownloadQuota;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETParallelDownloadsQuota;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETQuota;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETSystemRole;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETTotalDownloadsQuota;
import com.exprivia.odc.edmprovider.entitytypes.ODCEdmETUser;
import com.exprivia.odc.edmprovider.enumtypes.ODCEdmENRoleType;
import com.exprivia.odc.edmprovider.functions.ODCEdmFIntersects;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTBooleanAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTDateTimeOffsetAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTDoubleAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTIntegerAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTStringAttribute;
import com.exprivia.odc.metrics.edmprovider.entitysets.ODCEdmESMetrics;
import com.exprivia.odc.metrics.edmprovider.entitytypes.ODCEdmETMetric;
import com.exprivia.odc.metrics.edmprovider.enumtypes.ODCEdmENMetricType;
import com.exprivia.odc.subscriptions.frontend.edmprovider.actions.ODCEdmACancel;
import com.exprivia.odc.subscriptions.frontend.edmprovider.actions.ODCEdmAPause;
import com.exprivia.odc.subscriptions.frontend.edmprovider.actions.ODCEdmAResume;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitysets.ODCEdmESNotifications;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitysets.ODCEdmESSubscriptions;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETNotification;
import com.exprivia.odc.subscriptions.frontend.edmprovider.entitytypes.ODCEdmETSubscription;
import com.exprivia.odc.subscriptions.frontend.edmprovider.enumtypes.ODCEdmENSubscriptionStatus;

public class AUXIPEdmFactoryImpl implements ODCEdmFactory {
	@Autowired
	private ODCEdmENRoleType enRoleType;
	
	public ODCEdmEntityType createEntityType(FullQualifiedName entityTypeName) {
		if (entityTypeName.equals(AUXIPEdmETProduct.ET_FQN)) {
			return new AUXIPEdmETProduct();
		} else if (entityTypeName.equals(ODCEdmETAttribute.ET_FQN)) {
			return new ODCEdmETAttribute();
		} else if (entityTypeName.equals(ODCEdmPTStringAttribute.ET_FQN)) {
			return new ODCEdmPTStringAttribute();
		} else if (entityTypeName.equals(ODCEdmPTDateTimeOffsetAttribute.ET_FQN)) {
			return new ODCEdmPTDateTimeOffsetAttribute();
		} else if (entityTypeName.equals(ODCEdmPTIntegerAttribute.ET_FQN)) {
			return new ODCEdmPTIntegerAttribute();
		} else if (entityTypeName.equals(ODCEdmPTDoubleAttribute.ET_FQN)) {
			return new ODCEdmPTDoubleAttribute();
		} else if (entityTypeName.equals(ODCEdmPTBooleanAttribute.ET_FQN)) {
			return new ODCEdmPTBooleanAttribute();
		} else if (entityTypeName.equals(ODCEdmETUser.ET_FQN)) {
			return new ODCEdmETUser();
		} else if (entityTypeName.equals(ODCEdmETSystemRole.ET_FQN)) {
			return new ODCEdmETSystemRole();
		} else if (entityTypeName.equals(ODCEdmETQuota.ET_FQN)) {
			return new ODCEdmETQuota();
		} else if (entityTypeName.equals(ODCEdmETDownloadQuota.ET_FQN)) {
			return new ODCEdmETDownloadQuota();
		} else if (entityTypeName.equals(ODCEdmETTotalDownloadsQuota.ET_FQN)) {
			return new ODCEdmETTotalDownloadsQuota();
		} else if (entityTypeName.equals(ODCEdmETParallelDownloadsQuota.ET_FQN)) {
			return new ODCEdmETParallelDownloadsQuota();
		}
		/*
		 * From the odata-cat-metrics-lib
		 */
		else if (entityTypeName.equals(ODCEdmETMetric.ET_FQN)) {
			return new ODCEdmETMetric();
		}
		/*
		 * From the odata-cat-subscriptions-lib
		 */
		else if (entityTypeName.equals(ODCEdmETSubscription.ET_FQN)) {
			return new ODCEdmETSubscription();
		} else if (entityTypeName.equals(ODCEdmETNotification.ET_FQN)) {
			return new ODCEdmETNotification();
		}
		return null;
	}
	
	public ODCEdmEntitySet createEntitySet(FullQualifiedName entityContainer, String entitySetName) {
		if (entityContainer.equals(ODCEdmProvider.CONTAINER)) {
			if (entitySetName.equals(AUXIPEdmESProducts.ES_NAME)) {
				return new AUXIPEdmESProducts();
			} else if (entitySetName.equals(ODCEdmESAttributes.ES_NAME)) {
				return new ODCEdmESAttributes();
			} else if (entitySetName.equals(ODCEdmESUsers.ES_NAME)) {
				return new ODCEdmESUsers();
			} else if (entitySetName.equals(ODCEdmESSystemRoles.ES_NAME)) {
				return new ODCEdmESSystemRoles();
			} else if (entitySetName.equals(ODCEdmESQuotas.ES_NAME)) {
				return new ODCEdmESQuotas();
			} 
			/*
			 * From the odata-cat-metrics-lib
			 */
			else if (entitySetName.equals(ODCEdmESMetrics.ES_NAME)) {
				return new ODCEdmESMetrics();
			}
			/*
			 * From the odata-cat-subscriptions-lib
			 */
			else if (entitySetName.equals(ODCEdmESSubscriptions.ES_NAME) ) {
				return new ODCEdmESSubscriptions();
			} else if (entitySetName.equals(ODCEdmESNotifications.ES_NAME)) {
				return new ODCEdmESNotifications();
			}
		}
		return null;
	}
	
	public ODCEdmComplexType createComplexType(final FullQualifiedName complexTypeName) {
		if (complexTypeName.equals(ODCEdmCTCheckSum.CT_FQN)) {
			return new ODCEdmCTCheckSum();
		} else if (complexTypeName.equals(ODCEdmCTTimeRange.CT_FQN)) {
			return new ODCEdmCTTimeRange();
		}
		return null;
	}
	
	public ODCEdmEnumType createEnumType(final FullQualifiedName enumTypeName) {
		if (enumTypeName.equals(ODCEdmENRoleType.EN_FQN)) {
			return enRoleType;
		} 
		/*
		 * From the odata-cat-metrics-lib
		 */
		else if (enumTypeName.equals(ODCEdmENMetricType.EN_FQN)) {
			return new ODCEdmENMetricType();
		}
		/*
		 * From the odata-cat-subscriptions-lib
		 */
		else if (enumTypeName.equals(ODCEdmENSubscriptionStatus.EN_FQN)) {
			return new ODCEdmENSubscriptionStatus();
		}
		return null;
	}
	
	public ODCEdmEntitySet createMainEntitySet() {
		return createEntitySet(ODCEdmProvider.CONTAINER, AUXIPEdmESProducts.ES_NAME);
	}
	
	public List<FullQualifiedName> getEntityTypes() {
		return Arrays.asList(
				AUXIPEdmETProduct.ET_FQN,
				ODCEdmETAttribute.ET_FQN,
				ODCEdmPTStringAttribute.ET_FQN,
				ODCEdmPTDateTimeOffsetAttribute.ET_FQN,
				ODCEdmPTIntegerAttribute.ET_FQN,
				ODCEdmPTDoubleAttribute.ET_FQN,
				ODCEdmPTBooleanAttribute.ET_FQN,
				ODCEdmETUser.ET_FQN,
				ODCEdmETSystemRole.ET_FQN,
				ODCEdmETQuota.ET_FQN,
				ODCEdmETDownloadQuota.ET_FQN,
				ODCEdmETTotalDownloadsQuota.ET_FQN,
				ODCEdmETParallelDownloadsQuota.ET_FQN,
				/*
				 * From the odata-cat-metrics-lib
				 */
				ODCEdmETMetric.ET_FQN,
				/*
				 * From the odata-cat-subscriptions-lib
				 */
				ODCEdmETSubscription.ET_FQN,
				ODCEdmETNotification.ET_FQN);
	}
	
	public List<FullQualifiedName> getComplexTypes() {
		return Arrays.asList(
				ODCEdmCTCheckSum.CT_FQN,
				ODCEdmCTTimeRange.CT_FQN);
	}

	@Override
	public ODCEdmAction createAction(FullQualifiedName actionName) {
		if (actionName.equals(ODCEdmAProductsFilterList.AC_PRODUCTSFILTERLIST_FQN) ||
				actionName.equals(ODCEdmAProductsFilterList.AC_PRODUCTSFILTERLIST_FQN_ALT1)) {
			return new ODCEdmAProductsFilterList();
		}
		/*
		 * From the odata-cat-subscriptions-lib
		 */
		else if (actionName.equals(ODCEdmACancel.AC_CANCEL_FQN)) {
			return new ODCEdmACancel();
		} else if (actionName.equals(ODCEdmAPause.AC_PAUSE_FQN)) {
			return new ODCEdmAPause();
		} else if (actionName.equals(ODCEdmAResume.AC_RESUME_FQN)) {
			return new ODCEdmAResume();
		}
		return null;
	}

	@Override
	public List<FullQualifiedName> getActions() {
		return Arrays.asList(
				ODCEdmAProductsFilterList.AC_PRODUCTSFILTERLIST_FQN_ALT1, // to be compliant to ICD-1.2
				/*
				 * From the odata-cat-subscriptions-lib
				 */
				ODCEdmACancel.AC_CANCEL_FQN,
				ODCEdmAPause.AC_PAUSE_FQN,
				ODCEdmAResume.AC_RESUME_FQN
				);
	}
	
	@Override
	public List<FullQualifiedName> getActionImports() {
		return new ArrayList<FullQualifiedName>();
	}
	
	public ODCEdmActionImport createActionImport(FullQualifiedName actionImportName) {
		return null;
	}

	@Override
	public ODCEdmFunction createFunction(FullQualifiedName functionName) {
		if (functionName.equals(ODCEdmFIntersects.FN_INTERSECTS_FQN)) {
			return new ODCEdmFIntersects();
		}
		return null;
	}

	@Override
	public List<FullQualifiedName> getFunctions() {
		return Arrays.asList(ODCEdmFIntersects.FN_INTERSECTS_FQN);
	}
	
	@Override
	public List<FullQualifiedName> getEnumTypes() {
		return Arrays.asList(
				ODCEdmENRoleType.EN_FQN,
				/*
				 * From the odata-cat-metrics-lib
				 */
				ODCEdmENMetricType.EN_FQN,
				/*
				 * From the odata-cat-subscriptions-lib
				 */
				ODCEdmENSubscriptionStatus.EN_FQN
				);
	}
}
