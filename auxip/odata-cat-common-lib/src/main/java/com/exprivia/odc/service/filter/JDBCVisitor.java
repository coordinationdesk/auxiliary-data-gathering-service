package com.exprivia.odc.service.filter;

import java.sql.Timestamp;
import java.time.Duration;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import org.apache.olingo.commons.api.edm.Edm;
import org.apache.olingo.commons.api.edm.EdmEnumType;
import org.apache.olingo.commons.api.edm.EdmException;
import org.apache.olingo.commons.api.edm.EdmProperty;
import org.apache.olingo.commons.api.edm.EdmType;
import org.apache.olingo.commons.api.edm.provider.CsdlProperty;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.commons.core.edm.EdmPropertyImpl;
import org.apache.olingo.commons.core.edm.primitivetype.EdmBoolean;
import org.apache.olingo.commons.core.edm.primitivetype.EdmDateTimeOffset;
import org.apache.olingo.commons.core.edm.primitivetype.EdmDecimal;
import org.apache.olingo.commons.core.edm.primitivetype.EdmDuration;
import org.apache.olingo.commons.core.edm.primitivetype.EdmGeography;
import org.apache.olingo.commons.core.edm.primitivetype.EdmGeometry;
import org.apache.olingo.commons.core.edm.primitivetype.EdmGuid;
import org.apache.olingo.commons.core.edm.primitivetype.EdmString;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.uri.UriParameter;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceFunction;
import org.apache.olingo.server.api.uri.UriResourceLambdaAny;
import org.apache.olingo.server.api.uri.UriResourceLambdaVariable;
import org.apache.olingo.server.api.uri.UriResourceNavigation;
import org.apache.olingo.server.api.uri.UriResourcePrimitiveProperty;
import org.apache.olingo.server.api.uri.UriResourceProperty;
import org.apache.olingo.server.api.uri.queryoption.expression.Binary;
import org.apache.olingo.server.api.uri.queryoption.expression.BinaryOperatorKind;
import org.apache.olingo.server.api.uri.queryoption.expression.Expression;
import org.apache.olingo.server.api.uri.queryoption.expression.ExpressionVisitException;
import org.apache.olingo.server.api.uri.queryoption.expression.ExpressionVisitor;
import org.apache.olingo.server.api.uri.queryoption.expression.Literal;
import org.apache.olingo.server.api.uri.queryoption.expression.Member;
import org.apache.olingo.server.api.uri.queryoption.expression.Method;
import org.apache.olingo.server.api.uri.queryoption.expression.MethodKind;
import org.apache.olingo.server.api.uri.queryoption.expression.UnaryOperatorKind;
import org.apache.olingo.server.core.uri.queryoption.expression.LiteralImpl;

import com.exprivia.odc.delegate.item.StorageItem;
import com.exprivia.odc.util.Util;

public class JDBCVisitor implements ExpressionVisitor<Object> {
	private StorageItem<?> storageItem;
	private Edm edm;
	
	private Map<String, String> alias = new HashMap<String, String>();

	public JDBCVisitor(StorageItem<?> storageItem, Edm edm) {
		this.storageItem = storageItem;
		this.edm = edm;
	}
	
	@SuppressWarnings("serial")
	private static final Map<BinaryOperatorKind, String> BINARY_OPERATORS = new HashMap<BinaryOperatorKind, String>() {
		{
			put(BinaryOperatorKind.ADD, " + ");
			put(BinaryOperatorKind.AND, " AND ");
			put(BinaryOperatorKind.DIV, " / ");
			put(BinaryOperatorKind.EQ, " = ");
			put(BinaryOperatorKind.GE, " >= ");
			put(BinaryOperatorKind.GT, " > ");
			put(BinaryOperatorKind.LE, " <= ");
			put(BinaryOperatorKind.LT, " < ");
			put(BinaryOperatorKind.MOD, " % ");
			put(BinaryOperatorKind.MUL, " * ");
			put(BinaryOperatorKind.NE, " <> ");
			put(BinaryOperatorKind.OR, " OR ");
			put(BinaryOperatorKind.SUB, " - ");
			put(BinaryOperatorKind.IN, " IN ");
		}
	};
	
	@Override
	public Object visitMember(final Member member) throws ExpressionVisitException, ODataApplicationException {
		
		final List<UriResource> uriResourceParts = member.getResourcePath().getUriResourceParts();
		
		if (uriResourceParts.size() >= 1 && uriResourceParts.get(0) instanceof UriResourceProperty) {
			List<String> pathParts = new ArrayList<String>();
			UriResource lastPart = null;
			
			for (UriResource uriPart : uriResourceParts) {
				if (uriPart instanceof UriResourceProperty) {
					pathParts.add(((UriResourceProperty) uriPart).getSegmentValue());
				} else if (uriPart instanceof UriResourceLambdaAny) {
					alias.put(((UriResourceLambdaAny)uriPart).getLambdaVariable(), String.join(".",  pathParts));
				}
				lastPart = uriPart;
			}

			if (lastPart instanceof UriResourceLambdaAny) {
				UriResourceLambdaAny lambdaAny = (UriResourceLambdaAny) lastPart;
				return visitLambdaExpression(lambdaAny.getSegmentValue(), lambdaAny.getLambdaVariable(), lambdaAny.getExpression());
			} else if (lastPart instanceof UriResourceProperty) {
				return new EdmPropertyImpl(edm, new CsdlProperty().setName(String.join(".", pathParts)));				
			}
			
			throw new ODataApplicationException("unexpected VisitMember path",
					HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
		} else if (uriResourceParts.size() >= 1 && uriResourceParts.get(0) instanceof UriResourceNavigation) {
		
			List<String> pathParts = new ArrayList<String>();
			UriResource lastPart = null;
			for (UriResource uriPart : uriResourceParts) {
				if (uriPart instanceof UriResourceNavigation) {
					pathParts.add(((UriResourceNavigation) uriPart).getSegmentValue());
				} else if (uriPart instanceof UriResourceLambdaAny) {
					alias.put(((UriResourceLambdaAny)uriPart).getLambdaVariable(), String.join(".",  pathParts));
				} else if (uriPart instanceof UriResourceProperty) {
					pathParts.add(((UriResourceProperty) uriPart).getSegmentValue());
				}
				lastPart = uriPart;
			}

			if (lastPart instanceof UriResourceLambdaAny) {
				UriResourceLambdaAny lambdaAny = (UriResourceLambdaAny) lastPart;
				return visitLambdaExpression(lambdaAny.getSegmentValue(), lambdaAny.getLambdaVariable(), lambdaAny.getExpression());
			} else if (lastPart instanceof UriResourceProperty) {
				return new EdmPropertyImpl(edm, new CsdlProperty().setName(String.join(".", pathParts)));				
			}
			
			throw new ODataApplicationException("unexpected VisitMember path",
					HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
		} else if (	uriResourceParts.size() == 2 && 
					uriResourceParts.get(0) instanceof UriResourceLambdaVariable && 
					uriResourceParts.get(1) instanceof UriResourcePrimitiveProperty) {
		
			UriResourceLambdaVariable lambdaVar = (UriResourceLambdaVariable) uriResourceParts.get(0);
			UriResourcePrimitiveProperty prop = (UriResourcePrimitiveProperty) uriResourceParts.get(1);
			
			String type = prop.getProperty().getType().getName();
			
			String column = alias.get(lambdaVar.getSegmentValue()) + "(" + lambdaVar.getSegmentValue() + ")";
			
			return new EdmPropertyImpl(edm, 
					new CsdlProperty().setName(
							column + "." + prop.getSegmentValue() + "::" + type)
					);
		} else if (uriResourceParts.size() >= 1 && uriResourceParts.get(0) instanceof UriResourceFunction) {
			UriResourceFunction function = ((UriResourceFunction)uriResourceParts.get(0));
			
			List<Object> params = new ArrayList<Object>();
			for (UriParameter param: function.getParameters()) {
				/*
				 * TODO for now, only literal arguments are supported
				 */
				String textValue = param.getText();
				EdmType tp = function.getFunction().getParameter(param.getName()).getType();
				
				params.add(resolveOperand(new LiteralImpl(textValue, tp)));
			}
			
			return resolveFunction(function.getFunction().getName(), params);
		} else {
			// The OData specification allows in addition complex properties and navigation properties 
			// with a target cardinality 0..1 or 1.
			// This means any combination can occur e.g. Supplier/Address/City
			//  -> Navigation properties  Supplier 
			//  -> Complex Property       Address
			//  -> Primitive Property     City
			// For such cases the resource path returns a list of UriResourceParts			
			throw new ODataApplicationException("Only primitive properties are implemented in filter expressions",
					HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
		}
	}

	@Override
	public Object visitLiteral(Literal literal) throws ExpressionVisitException, ODataApplicationException {
		// To keep this tutorial simple, our filter expression visitor supports only Edm.Int32 and Edm.String
		// In real world scenarios it can be difficult to guess the type of an literal.
		// We can be sure, that the literal is a valid OData literal because the URI Parser checks 
		// the lexicographical structure

		// String literals start and end with an single quotation mark
		String literalAsString = literal.getText();
		if (literal.getType() instanceof EdmString) {
			return storageItem.convertLiteralValue(Util.trimQuotes(literalAsString));
		} if (literal.getType() instanceof EdmDecimal) {
			return Double.valueOf(literalAsString);
		} else if (literal.getType() instanceof EdmDateTimeOffset) {
			return Util.parseDateString(literal.getText());
		} else if (literal.getType() instanceof EdmGeometry ||
				literal.getType() instanceof EdmGeography) {
			return literalAsString.replace("geometry'", "").replace("geography'", "").replace("'", "");
		} else if (literal.getType() instanceof EdmBoolean) {
			return Boolean.valueOf(literalAsString);
		} else if (literal.getType() instanceof EdmDuration) {
			return Duration.parse(literalAsString.replace("duration'", "").replace("'", ""));
		} else if (literal.getType() instanceof EdmGuid) {
			return literalAsString.replace("guid'", "").replace("'", "");
		} else {
			// Try to convert the literal into an Java Integer
			try {
				return Long.parseLong(literalAsString);
			} catch (NumberFormatException e) {
				throw new ODataApplicationException(
						"Unhandled literal " + literalAsString,
						HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
			}
		}
	}
	
	protected Object resolveOperand(Object operand) throws ODataApplicationException, ExpressionVisitException {
		if (operand instanceof Binary) {
			Binary binary = (Binary) operand;
			return visitBinaryOperator(
					binary.getOperator(), 
					resolveOperand(binary.getLeftOperand()), 
					resolveOperand(binary.getRightOperand() == null ? binary.getExpressions() : binary.getRightOperand()));
		} if (operand instanceof Member) {
			return visitMember((Member)operand);
		} else if (operand instanceof EdmProperty) {
			return operand;
		} else if (operand instanceof Literal) {
			return visitLiteral((Literal)operand);
		} else if (operand instanceof Timestamp) {
			return operand;
		} else if (operand instanceof Integer) {
			return operand;
		} else if (operand instanceof Long) {
			return operand;
		} else if (operand instanceof Double) {
			return operand;
		} else if (operand instanceof String) {
			return operand;
		} else if (operand instanceof JDBCFilterExpression) {
			return operand;
		} else if (operand instanceof Boolean) {
			return operand;
		} else if (operand instanceof Duration) {
			return operand;
		} else if (operand instanceof Method) {
			List<Object> parameters = new ArrayList<Object>();
			for (Expression expression : ((Method)operand).getParameters()) {
				parameters.add(resolveOperand(expression));
			}
			return visitMethodCall(((Method)operand).getMethod(), parameters);
		} else if (operand instanceof List<?>) {
			List<Object> list = new ArrayList<Object>();
			for (Object item: (List<?>)operand) {
				list.add(resolveOperand(item));
			}
			return list;
		}
		
		throw new ODataApplicationException(
				"Unexpected operand " + operand.toString(),
				HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
	}
	
	protected Object resolveFunction(String function, List<Object> parameters) throws ODataApplicationException {
		JDBCFilterExpression expression = new JDBCFilterExpression(null);
		
		switch (function) {
		case "Intersects":
			expression.setPrepeparedWhere("ST_Intersects(footprint, GeomFromEWKT(?))");
			expression.addParameter(parameters.get(0).toString());
			return expression;
		default:
			throw new ODataApplicationException(
					"Unimplemented function: " + function,
					HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
		}
	}

	@Override
	public Object visitBinaryOperator(BinaryOperatorKind operator, Object leftSide, Object rightSide)
			throws ExpressionVisitException, ODataApplicationException {

		//Transform the OData filter operator into an equivalent sql operator
		String sqlOperator = BINARY_OPERATORS.get(operator);
		if (sqlOperator == null) {
			throw new ODataApplicationException("Unsupported binary operation: " + operator.name(),
					operator == BinaryOperatorKind.HAS ? HttpStatusCode.NOT_IMPLEMENTED.getStatusCode()
							: HttpStatusCode.BAD_REQUEST.getStatusCode(),
					Locale.ENGLISH);
		}
		
		JDBCFilterExpressionEditor expression = new JDBCFilterExpressionEditor(operator, storageItem);
		try {
			Object left = resolveOperand(leftSide);
			Object right = resolveOperand(rightSide);

			if (left instanceof JDBCFilterExpression && right instanceof JDBCFilterExpression) {
				JDBCFilterExpression leftSideExpression = (JDBCFilterExpression) left;

				if (BinaryOperatorKind.AND.equals(leftSideExpression.getOperator())
						|| BinaryOperatorKind.OR.equals(leftSideExpression.getOperator())) {
					leftSideExpression.setPrepeparedWhere("(" + leftSideExpression.toString() + ")");
				}

				JDBCFilterExpression rightSideExpression = (JDBCFilterExpression) right;
				if (BinaryOperatorKind.AND.equals(rightSideExpression.getOperator())
						|| BinaryOperatorKind.OR.equals(rightSideExpression.getOperator())) {
					rightSideExpression.setPrepeparedWhere("(" + rightSideExpression.toString() + ")");
				}

				expression.setPrepeparedWhere(
						leftSideExpression.toString() + " " + sqlOperator + " " + rightSideExpression.toString());

				for (Object parameter : leftSideExpression.getParameters()) {
					expression.addParameter(parameter);
				}
				for (Object parameter : rightSideExpression.getParameters()) {
					expression.addParameter(parameter);
				}
			} else {
				if (left instanceof EdmProperty &&
					storageItem.isJsonProperty(((EdmProperty)left).getName())) {
					/*
					 * Special json field contents condition
					 */
					storageItem.processJsonCondition(expression, ((EdmProperty)left).getName(), sqlOperator, right);
				} else {
					expression.addParameterInSql(left);
					expression.addSql(sqlOperator);
					
					if (sqlOperator.equals(" IN ")) {
						if (!(right instanceof List<?>)) {
							throw new RuntimeException("wrong type of operand for IN operator: " + right.getClass().getCanonicalName());
						}
						List<?> operandList = (List<?>)right;
						StringBuilder sb = new StringBuilder("(");
						for (int a = 1; a <= operandList.size(); a++) {
							if (a != operandList.size())
								sb.append("?, ");
							else
								sb.append("?");
							expression.addParameter(operandList.get(a - 1));
						}
						sb.append(")");
						
						expression.addSql(sb.toString());
					} else {
						expression.addParameterInSql(right);
					}
				}
			}
		} catch (EdmException e) {
			throw new RuntimeException("EdmException occured");
		}
		return expression;
	}

	@Override
	public Object visitUnaryOperator(UnaryOperatorKind operator, Object operand)
			throws ExpressionVisitException, ODataApplicationException {
		// OData allows two different unary operators. We have to take care, that the type of the
		// operand fits to the operand

		if (operator == UnaryOperatorKind.NOT && operand instanceof JDBCFilterExpression) {
			JDBCFilterExpression oldExpr = (JDBCFilterExpression) operand;
			JDBCFilterExpression newExpr = new JDBCFilterExpression(null);
			
			newExpr.setPrepeparedWhere(" NOT " + oldExpr.getPreparedWhere());
			
			for (Object param : oldExpr.getParameters()) {
				newExpr.addParameter(param);
			}
			
			return newExpr;
		} else if (operator == UnaryOperatorKind.NOT && operand instanceof Boolean) {
			// 1.) boolean negation
			return ! (Boolean) operand;
		} else if (operator == UnaryOperatorKind.MINUS) {
			// 2.) arithmetic minus
			return "-"  + resolveOperand(operand);
		}

		// Operation not processed, throw an exception
		throw new ODataApplicationException("Invalid type for unary operator",
				HttpStatusCode.BAD_REQUEST.getStatusCode(), Locale.ENGLISH);
	}

	@Override
	public Object visitMethodCall(MethodKind methodCall, List<Object> parameters)
			throws ExpressionVisitException, ODataApplicationException {

		// create the output filter
		JDBCFilterExpressionEditor jdbcFilter = new JDBCFilterExpressionEditor(null, storageItem);
		switch (methodCall) {
		// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.645.1863&rep=rep1&type=pdf
		case CONTAINS:
			if (parameters.size() != 2) 
				throw new RuntimeException(methodCall + " requires 2 arguments");
			jdbcFilter.addSql("POSITION(")
				.addParameterInSql(parameters.get(1))
				.addSql(" in ")
				.addParameterInSql(parameters.get(0))
				.addSql(") != 0");
			break;
		// string toupper
		// toupper(CompanyName) eq 'ALFREDS FUTTERKISTE'
		case TOUPPER:
			if (parameters.size() != 1) 
				throw new RuntimeException(methodCall + " requires 1 argument");

			jdbcFilter.addSql("UPPER(")
				.addParameterInSql(parameters.get(0))
				.addSql(")");
			break;
		
		// string tolower
		// tolower(CompanyName) eq 'alfreds futterkiste'
		case TOLOWER:
			if (parameters.size() != 1) 
				throw new RuntimeException(methodCall + " requires 1 argument");

			jdbcFilter.addSql("LOWER(")
				.addParameterInSql(parameters.get(0))
				.addSql(")");
			break;
		// bool startswith
		// startswith(CompanyName,'Alfr')
		case STARTSWITH:
			if (parameters.size() != 2) 
				throw new RuntimeException(methodCall + " requires 2 argument");
			
			if (!(parameters.get(1) instanceof String))
				throw new RuntimeException(methodCall + " second argument shall be string");
			
			String startsWith = (String)parameters.get(1);

			jdbcFilter.addSql("(")
				.addParameterInSql(parameters.get(0))
				.addSql(" LIKE ")
				.addParameterInSql(startsWith.replace("_", "\\_").replace("%", "\\%") + "%")
				.addSql(")");
			break;
		
		// bool endswith
		// endswith(CompanyName,'Futterkiste')
		case ENDSWITH:
			if (parameters.size() != 2) 
				throw new RuntimeException(methodCall + " requires 2 argument");

			if (!(parameters.get(1) instanceof String))
				throw new RuntimeException(methodCall + " second argument shall be string");
			
			String endsWith = (String)parameters.get(1);
			
			jdbcFilter.addSql("(")
				.addParameterInSql(parameters.get(0))
				.addSql(" LIKE ")
				.addParameterInSql("%" + endsWith.replace("_", "\\_").replace("%", "\\%"))
				.addSql(")");
			break;
		// int length
		// length(CompanyName) eq 19
		case LENGTH:
			if (parameters.size() != 1) 
				throw new RuntimeException(methodCall + " requires 1 argument");

			jdbcFilter.addSql("LENGTH(")
				.addParameterInSql(parameters.get(0))
				.addSql(")");
			break;
		// int indexof
		// indexof(CompanyName,'lfreds') eq 1
		case INDEXOF:
			if (parameters.size() != 2) 
				throw new RuntimeException(methodCall + " requires 2 arguments");
			jdbcFilter.addSql("(POSITION(")
				.addParameterInSql(parameters.get(1))
				.addSql(" in ")
				.addParameterInSql(parameters.get(0))
				.addSql(") - 1 )");
			break;
		// string substring
		// substring(CompanyName,1) eq 'lfreds Futterkiste'
		case SUBSTRING:
			if (parameters.size() != 2 && parameters.size() != 3) 
				throw new RuntimeException(methodCall + " requires 2 or 3 arguments");
			jdbcFilter.addSql("SUBSTRING(")
				.addParameterInSql(parameters.get(0))
				.addSql(" from ")
				.addParameterInSql((Integer)parameters.get(1) + 1);
			if (parameters.size() == 3)
				jdbcFilter.addSql(" for ")
					.addParameterInSql(parameters.get(2));
			jdbcFilter.addSql(")");
			break;
		// string trim
		// trim(CompanyName) eq 'Alfreds Futterkiste'
		case TRIM:
			if (parameters.size() != 1) 
				throw new RuntimeException(methodCall + " requires 1 argument");
			jdbcFilter.addSql("TRIM(")
				.addParameterInSql(parameters.get(0))
				.addSql(")");
			break;
		// string concat
		// concat(concat(City,', '), Country) eq 'Berlin, Germany'
		case CONCAT:
			if (parameters.size() != 2)
				throw new RuntimeException(methodCall + " requires 2 arguments");
			jdbcFilter.addParameterInSql(parameters.get(0))
				.addSql(" || ")
				.addParameterInSql(parameters.get(1));
			break;
		// int year
		// year(BirthDate) eq 0
		// int month
		// month(BirthDate) eq 12
		// int day
		// day(StartTime) eq 8
		// int hour
		// hour(StartTime) eq 1
		// int minute
		// minute(StartTime) eq 0
		// int second
		// second(StartTime) eq 0
		case YEAR:
		case MONTH:
		case DAY:
		case HOUR:
		case MINUTE:
		case SECOND:
			if (parameters.size() != 1)
				throw new RuntimeException(methodCall + " requires 1 argument");
			jdbcFilter.addSql("EXTRACT(" + methodCall + " FROM ")
				.addParameterInSql(parameters.get(0))
				.addSql(")");
			break;
		// DateTimeOffset now
		// StartTime ge now()
		case NOW:
			if (parameters.size() != 0) 
				throw new RuntimeException(methodCall + " does not require arguments");

			jdbcFilter.addSql("now()");
			break;
		// double round
		// round(Freight) eq 32
		case ROUND:
			if (parameters.size() != 1)
				throw new RuntimeException(methodCall + " requires 1 argument");
			jdbcFilter.addSql("ROUND(").addParameterInSql(parameters.get(0)).addSql(")");
			break;
		// decimal floor
		// floor(Freight) eq 32
		case FLOOR:
			if (parameters.size() != 1)
				throw new RuntimeException(methodCall + " requires 1 argument");
			jdbcFilter.addSql("FLOOR(").addParameterInSql(parameters.get(0)).addSql(")");
			break;
		// double ceiling
		// ceiling(Freight) eq 33
		case CEILING:
			if (parameters.size() != 1)
				throw new RuntimeException(methodCall + " requires 1 argument");
			jdbcFilter.addSql("CEILING(").addParameterInSql(parameters.get(0)).addSql(")");
			break;
		// bool geo.intersects
		// geo.intersects(Position, TargetArea)
		case GEOINTERSECTS:
			if (parameters.size() != 2) 
				throw new RuntimeException(methodCall + " requires 2 arguments");
			boolean stringIntersectsGeom1 = parameters.get(0).toString().contains("SRID");
			boolean stringIntersectsGeom2 = parameters.get(1).toString().contains("SRID");
				
			jdbcFilter.addSql("ST_Intersects(")
				.addSql(stringIntersectsGeom1 ? "GeomFromEWKT(" : "")
				.addParameterInSql(parameters.get(0))	// always quote, if string
				.addSql(stringIntersectsGeom1 ? ")" : "")
				.addSql(", ")
				.addSql(stringIntersectsGeom2 ? "GeomFromEWKT(" : "")
				.addParameterInSql(parameters.get(1))
				.addSql(stringIntersectsGeom2 ? ")" : "")
				.addSql(")");
			break;
		// double geo.distance
		// geo.distance(CurrentPosition, TargetPosition)
		case GEODISTANCE:
			if (parameters.size() != 2) 
				throw new RuntimeException(methodCall + " requires 2 arguments");
			boolean stringDistanceGeom1 = parameters.get(0).toString().contains("SRID");
			boolean stringDistanceGeom2 = parameters.get(1).toString().contains("SRID");

			jdbcFilter.addSql("ST_Distance(")
				.addSql(stringDistanceGeom1 ? "GeomFromEWKT(" : "")
				.addParameterInSql(parameters.get(0))	// always quote, if string
				.addSql(stringDistanceGeom1 ? ")" : "")
				.addSql(", ")
				.addSql(stringDistanceGeom2 ? "GeomFromEWKT(" : "")
				.addParameterInSql(parameters.get(1))
				.addSql(stringDistanceGeom2 ? ")" : "")
				.addSql(")");
			break;
		// doublegeo.length
		// geo.length(DirectRoute)
		case GEOLENGTH:
			if (parameters.size() != 1) 
				throw new RuntimeException(methodCall + " requires 1 arguments");
			boolean stringLengthGeom1 = parameters.get(0).toString().contains("SRID");

			jdbcFilter.addSql("ST_Length(")
				.addSql(stringLengthGeom1 ? "GeomFromEWKT(" : "")
				.addParameterInSql(parameters.get(0))	// always quote, if string
				.addSql(stringLengthGeom1 ? ")" : "")
				.addSql(")");
			break;
		// decimal fractionalseconds
		// fractionalsecond(StartTime) eq 0
		case FRACTIONALSECONDS:
		// ?
		case TOTALSECONDS:
		// Date date
		// date(StartTime) ne date(EndTime)
		case DATE:
		// TimeOfDay time
		// time(StartTime) le StartOfDay
		case TIME:
		// int totaloffsetminutes
		// totaloffsetminutes(StartTime) eq 60
		case TOTALOFFSETMINUTES:
		// DateTimeOffset mindatetime
		// StartTime eq mindatetime()
		case MINDATETIME:
		// DateTimeOffset maxdatetime
		// EndTime eq maxdatetime()
		case MAXDATETIME:
		// any cast
		// cast(ShipCountry,Edm.String)
		case CAST:
		// bool isof
		// isof(NorthwindModel.Order)
		// bool isof
		// isof(ShipCountry,Edm.String)
		case ISOF:
			throw new RuntimeException(methodCall.name() + " not yet implemented");
			
		default:
			return null;
		}
		
		return jdbcFilter;
	}

	@Override
	public Object visitLambdaExpression(String lambdaFunction, String lambdaVariable, Expression expression)
			throws ExpressionVisitException, ODataApplicationException {
		
		if (expression instanceof Binary) {
			Binary binaryExpression = ((Binary) expression);
			return visitBinaryOperator(binaryExpression.getOperator(), binaryExpression.getLeftOperand(), binaryExpression.getRightOperand());
		}
		
		return null;
	}

	@Override
	public Object visitAlias(String aliasName) throws ExpressionVisitException, ODataApplicationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visitTypeLiteral(EdmType type) throws ExpressionVisitException, ODataApplicationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visitLambdaReference(String variableName) throws ExpressionVisitException, ODataApplicationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visitEnum(EdmEnumType type, List<String> enumValues)
			throws ExpressionVisitException, ODataApplicationException {
		return storageItem.convertLiteralValue(enumValues.get(0));
	}

	@Override
	public Object visitBinaryOperator(BinaryOperatorKind operator, Object left, List<Object> right)
			throws ExpressionVisitException, ODataApplicationException {
		return visitBinaryOperator(operator, left, (Object)right);
	}

}
