package com.exprivia.odc.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

import org.apache.olingo.commons.api.edm.geo.Geospatial;
import org.apache.olingo.commons.api.edm.geo.Geospatial.Dimension;
import org.apache.olingo.commons.api.edm.geo.GeospatialCollection;
import org.apache.olingo.commons.api.edm.geo.LineString;
import org.apache.olingo.commons.api.edm.geo.MultiLineString;
import org.apache.olingo.commons.api.edm.geo.MultiPoint;
import org.apache.olingo.commons.api.edm.geo.MultiPolygon;
import org.apache.olingo.commons.api.edm.geo.Point;
import org.apache.olingo.commons.api.edm.geo.Polygon;
import org.apache.olingo.commons.api.edm.geo.SRID;

public class EWKBToGeospatial {
	/**
	 * Converts a hexadecimal string to a byte array. The hexadecimal digit
	 * symbols are case-insensitive.
	 *
	 * @param hex
	 *            a string containing hex digits
	 * @return an array of bytes with the value of the hex string
	 */
	public static byte[] hexToBytes(String hex) {
		int byteLen = hex.length() / 2;
		byte[] bytes = new byte[byteLen];

		for (int i = 0; i < hex.length() / 2; i++) {
			int i2 = 2 * i;
			if (i2 + 1 > hex.length())
				throw new IllegalArgumentException("Hex string has odd length");

			int nib1 = hexToInt(hex.charAt(i2));
			int nib0 = hexToInt(hex.charAt(i2 + 1));
			byte b = (byte) ((nib1 << 4) + (byte) nib0);
			bytes[i] = b;
		}
		return bytes;
	}

	private static int hexToInt(char hex) {
		int nib = Character.digit(hex, 16);
		if (nib < 0)
			throw new IllegalArgumentException("Invalid hex digit: '" + hex + "'");
		return nib;
	}

	private static final String INVALID_GEOM_TYPE_MSG = "Invalid geometry type encountered in ";

	// default dimension - will be set on read
	private int inputDimension = 2;
	private boolean hasSRID = false;
	private int srid = 4326;
	/**
	 * true if structurally invalid input should be reported rather than
	 * repaired. At some point this could be made client-controllable.
	 */
	private boolean isStrict = true;
	private ByteBuffer bb = null;

	public Geospatial read(byte[] bytes) {
		bb = ByteBuffer.wrap(bytes);
		// possibly reuse the ByteArrayInStream?
		// don't throw IOExceptions, since we are not doing any I/O
		return readGeometry();
	}

	private Geospatial readGeometry() {

		// determine byte order
		byte byteOrderWKB = bb.get();

		// always set byte order, since it may change from geometry to geometry
		if (byteOrderWKB == 0x01) {
			bb.order(ByteOrder.LITTLE_ENDIAN);
		} else if (byteOrderWKB == 0x00) {
			bb.order(ByteOrder.BIG_ENDIAN);
		} else if (isStrict) {
			throw new RuntimeException("Unknown geometry byte order (not NDR or XDR): " + byteOrderWKB);
		}
		//if not strict and not XDR or NDR, then we just use the dis default set at the
		//start of the geometry (if a multi-geometry).  This  allows WBKReader to work
		//with Spatialite native BLOB WKB, as well as other WKB variants that might just
		//specify endian-ness at the start of the multigeometry.

		int typeInt = bb.getInt();
		// Adds %1000 to make it compatible with OGC 06-103r4
		int geometryType = (typeInt & 0xffff) % 1000;

		// handle 3D and 4D WKB geometries
		// geometries with Z coordinates have the 0x80 flag (postgis EWKB)
		// or are in the 1000 range (Z) or in the 3000 range (ZM) of geometry type (OGC 06-103r4)
		boolean hasZ = ((typeInt & 0x80000000) != 0 || (typeInt & 0xffff) / 1000 == 1 || (typeInt & 0xffff) / 1000 == 3);
		// geometries with M coordinates have the 0x40 flag (postgis EWKB)
		// or are in the 1000 range (M) or in the 3000 range (ZM) of geometry type (OGC 06-103r4)
		boolean hasM = ((typeInt & 0x40000000) != 0 || (typeInt & 0xffff) / 1000 == 2 || (typeInt & 0xffff) / 1000 == 3);
		//System.out.println(typeInt + " - " + geometryType + " - hasZ:" + hasZ);
		inputDimension = 2 + (hasZ ? 1 : 0) + (hasM ? 1 : 0);

		// determine if SRIDs are present
		hasSRID = (typeInt & 0x20000000) != 0;
		if (hasSRID) {
			srid = bb.getInt();
		}

		Geospatial geom = null;
		switch (geometryType) {
		case 0x0001:
			geom = readPoint();
			break;
		case 0x0002:
			geom = readLineString();
			break;
		case 0x0003:
			geom = readPolygon();
			break;
		case 0x0004:
			geom = readMultiPoint();
			break;
		case 0x0005:
			geom = readMultiLineString();
			break;
		case 0x0006:
			geom = readMultiPolygon();
			break;
		case 0x0007:
			geom = readGeometryCollection();
			break;
		default:
			throw new RuntimeException("Unknown WKB type " + geometryType);
		}
		return geom;
	}
	
	private SRID getSrid() {
		return SRID.valueOf(new Integer(srid).toString());
	}

	private Point readPoint() {
		Point p = new Point(Dimension.GEOGRAPHY, getSrid());
		p.setX(bb.getDouble());
		p.setY(bb.getDouble());
		if (inputDimension >= 3)
			p.setZ(bb.getDouble());
		if (inputDimension >= 4)
			// only consume a double
			bb.getDouble();
		return p;
	}

	private LineString readLineString() {
		return new LineString(Dimension.GEOGRAPHY, getSrid(), readLinearRing());
	}

	private List<Point> readLinearRing() {
		int size = bb.getInt();
		List<Point> list = new ArrayList<Point>();
		for (int a = 0; a < size; a++)
			list.add(readPoint());
		return list;
	}

	private Polygon readPolygon() {
		int numRings = bb.getInt();
		List<LineString> holes = new ArrayList<LineString>();

		LineString shell = readLineString();
		for (int i = 0; i < numRings - 1; i++) {
			holes.add(readLineString());
		}
		return new Polygon(Dimension.GEOGRAPHY, getSrid(), holes, shell);
	}

	private MultiPoint readMultiPoint() {
		int numGeom = bb.getInt();
		List<Point> points = new ArrayList<Point>();
		for (int i = 0; i < numGeom; i++) {
			Geospatial g = readGeometry();
			if (!(g instanceof Point))
				throw new RuntimeException(INVALID_GEOM_TYPE_MSG + "MultiPoint");
			points.add((Point) g);
		}
		return new MultiPoint(Dimension.GEOGRAPHY, getSrid(), points);
	}

	private MultiLineString readMultiLineString() {
		int numGeom = bb.getInt();
		List<LineString> geoms = new ArrayList<LineString>();
		for (int i = 0; i < numGeom; i++) {
			Geospatial g = readGeometry();
			if (!(g instanceof LineString))
				throw new RuntimeException(INVALID_GEOM_TYPE_MSG + "MultiLineString");
			geoms.add((LineString) g);
		}
		return new MultiLineString(Dimension.GEOGRAPHY, getSrid(), geoms);
	}

	private MultiPolygon readMultiPolygon() {
		int numGeom = bb.getInt();
		List<Polygon> geoms = new ArrayList<Polygon>();

		for (int i = 0; i < numGeom; i++) {
			Geospatial g = readGeometry();
			if (!(g instanceof Polygon))
				throw new RuntimeException(INVALID_GEOM_TYPE_MSG + "MultiPolygon");
			geoms.add((Polygon) g);
		}
		return new MultiPolygon(Dimension.GEOGRAPHY, getSrid(), geoms);
	}

	private GeospatialCollection readGeometryCollection() {
		int numGeom = bb.getInt();
		List<Geospatial> geoms = new ArrayList<>();
		for (int i = 0; i < numGeom; i++) {
			geoms.add(readGeometry());
		}
		return new GeospatialCollection(Dimension.GEOGRAPHY, getSrid(), geoms);
	}

}