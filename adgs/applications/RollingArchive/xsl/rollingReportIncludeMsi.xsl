<?xml version="1.0" encoding="utf-8"?>

<!-- Applicable to Rolling report: get only Granule and Datastrip items.
	Set the <XsltFormatConverter> section of RollingArchive configuration space -->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="xml" indent="yes"/>
	<xsl:strip-space elements="*"/>

	<xsl:variable name="params" select="document( 'rollReportParams.xml' )"/>

	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()" />
		</xsl:copy>
	</xsl:template>

	<xsl:template match="//Fixed_Header/File_Name">
		<xsl:element name="File_Name">
			<xsl:value-of select="substring-before($params//File_Name/text(), '.EOF')"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="ItemMetadataUpdate">
		<xsl:choose>
			<xsl:when test="contains( ./*[local-name() = 'Pdi-id']/text(), '_MSI_')">
				<xsl:element name="ItemMetadataUpdate">
					<xsl:apply-templates select="@*|node()" />
				</xsl:element>
			</xsl:when>
			<xsl:otherwise/><!-- skip this -->
		</xsl:choose>
	</xsl:template>
	
</xsl:stylesheet>

