<?xml version="1.0" encoding="UTF-8"?>

<!-- Transform xml in html syntax -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="/node()">
		<xsl:element name="html">
			<xsl:element name="head"/>
			<xsl:element name="body">
				<xsl:apply-templates select="@*|node()" />
			</xsl:element>	
		</xsl:element>
	</xsl:template>


	<!-- Elements handling -->
	<xsl:template match="node()">

		<xsl:if test='name() != ""'>
			<xsl:element name="p">
				<xsl:attribute name="text-indent">
					<xsl:value-of select='count(ancestor::*)'/><xsl:text>em;</xsl:text>
				</xsl:attribute>
				<B><xsl:value-of select="name()"/><xsl:text>: </xsl:text></B><xsl:value-of select="text()"/>
			</xsl:element>			
		</xsl:if>
			
		<xsl:apply-templates select="@*|node()" />
	</xsl:template>
	
	<!-- For element in the array -->
	<xsl:template match="*[../@count]"> <!-- any element whose parent contains the count attribute -->
		<xsl:element name="p">
			<xsl:attribute name="text-indent">
				<xsl:value-of select='count(ancestor::*)'/><xsl:text>em;</xsl:text>
			</xsl:attribute>
			<LI><xsl:value-of select="text()"/></LI>			
		</xsl:element>
	</xsl:template>

	<!-- Attributes handling - do nothing -->
	<xsl:template match="@*">
		<xsl:apply-templates select="@*|node()" />
	</xsl:template>

</xsl:stylesheet>


