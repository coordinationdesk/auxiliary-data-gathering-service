<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method='html' version='1.0' encoding='UTF-8' indent='yes'/>
	<xsl:template match="/">
		<html>
 			<body>
 				<h1><xsl:value-of select="//L0ProductDescriptor/SatelliteName"/> L0 Product Types Configuration</h1>
				<h2>Common Parameters</h2>
 				<table border="1" bordercolor="green">
					<tr bgcolor="#9acd32">
						<th align="left">Source Packet Header Size (bytes)</th>
						<th align="left">Source Packet Annotation Size (bytes)</th>
						<th align="left">Reference Document Name</th>
						<th align="left">Acquisition Report FileType</th>
						<th align="left">AnnotationIsInUTC flag</th>
						<th align="left">Number of Product types</th>
					</tr>
					<tr>
						<td align="right"><xsl:value-of select="//L0ProductDescriptor/SourcePacketHeaderSize"/></td>
						<td align="right"><xsl:value-of select="//L0ProductDescriptor/AnnotationSize"/></td>
						<td align="center"><xsl:value-of select="//L0ProductDescriptor/RefDocName"/></td>
						<td align="center"><xsl:value-of select="//L0ProductDescriptor/AcquisitionReportFileType"/></td>
						<td align="center"><xsl:value-of select="//L0ProductDescriptor/AnnotationIsInUTC"/></td>
						<td align="right"><xsl:value-of select="//L0ProductDescriptor/NumberOfProducts"/></td>
					</tr>
				</table>
				<h2>Product Types List</h2>	
 				<table border="1" bordercolor="green">
					<tr bgcolor="#9acd32">
						<th align="left">Product Type Id</th>
						<th align="left">Description</th>
						<th align="left">APID</th>
						<th align="left">Additional Fields</th>
						<th align="left">CCSDS Version</th>
						<th align="left">Segmented flag</th>
						<th align="left">Data Field Header length (bytes)</th>
						<th align="left">Application Data length (bytes)</th>
						<th align="left">PEC length (bytes)</th>
						<th align="left">Delta Nominal time (ms)</th>
						<th align="left">Gap Thresold (ms)</th>
						<th align="left">Slice Length (ms)</th>
						<th align="left">Slice Overlap (ms)</th>
						<th align="left">Channel</th>
						<th align="left">VCID</th>
						<th align="left">Sensor Name</th>
						<th align="left">Sensor Mode</th>
					</tr>
					<xsl:variable name="numOfProducts"><xsl:value-of select="//L0ProductDescriptor/NumberOfProducts"/></xsl:variable>
					<xsl:call-template name="productsLoop">
						<xsl:with-param name="init" select="1"/>
						<xsl:with-param name="final" select="$numOfProducts"/>
						<xsl:with-param name="inc" select="1"/>
					</xsl:call-template>					
				</table>
			</body>
		</html>
	</xsl:template>

	<xsl:template name="productsLoop">
		<xsl:param name="init"/>
		<xsl:param name="final"/>
		<xsl:param name="inc"/>
		<xsl:if test="$init &lt;= $final">
			<tr>
				<td><b><xsl:value-of select="(//L0ProductDescriptor//Id)[$init]"/></b></td>
				<td><xsl:value-of select="(//L0ProductDescriptor//Description)[$init]"/></td>					
				<td align="center">
					<xsl:for-each select="(//L0ProductDescriptor//List_of_APID)[$init]">
						<tr><td><b><i><xsl:copy-of select="."/></i></b></td></tr>
					</xsl:for-each>
				</td>
				<td align="center">
					<xsl:for-each select="(//L0ProductDescriptor//List_of_Fields)[$init]">
						<xsl:copy-of select="."/>
					</xsl:for-each>
				</td>
				<td align="center"><xsl:value-of select="(//L0ProductDescriptor//CCSDSVersion)[$init]"/></td>
				<td align="center"><xsl:value-of select="(//L0ProductDescriptor//SegmentedPacketsFlag)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//DataFieldHeaderLength)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//ApplicationDataLength)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//PacketErrorControlLength)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//DeltaNominalTime)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//GapThresold)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//Slice/Length)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//Slice/Overlap)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//Channel)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//VCID)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//SensorName)[$init]"/></td>
				<td align="right"><xsl:value-of select="(//L0ProductDescriptor//SensorMode)[$init]"/></td>
			</tr>
			<xsl:call-template name="productsLoop">
				<xsl:with-param name="init" select="$init+$inc"/>
				<xsl:with-param name="final" select="$final"/>
				<xsl:with-param name="inc" select="$inc"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="*">
	</xsl:template>
	
</xsl:stylesheet>
