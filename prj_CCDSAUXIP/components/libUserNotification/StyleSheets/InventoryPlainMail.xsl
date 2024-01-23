<?xml version="1.0" encoding="UTF-8"?>

<!-- Transform xml in ResourceSet syntax -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method = "text" omit-xml-declaration = "yes" indent = "no" />

	<!-- Ignore root -->
	<xsl:template match="/node()">
		<!-- Do nothing and proceed -->
		<xsl:apply-templates select="@*|node()" />
	</xsl:template>

	<!-- Elements handling -->
	<xsl:template match="node()">

		<xsl:if test='name() != ""'>

			<xsl:call-template name="printTabs">
				<xsl:with-param name="nodeParam" select="."/>
			</xsl:call-template>

			<xsl:value-of select="name()"/><xsl:text>: </xsl:text><xsl:value-of select="text()"/><xsl:text>&#10;</xsl:text>
		</xsl:if>
			
		<xsl:apply-templates select="@*|node()" />
	</xsl:template>
	
	<!-- For element in the array -->
	<xsl:template match="*[../@count]"> <!-- any element whose parent contains the count attribute -->
		<xsl:call-template name="printTabs">
			<xsl:with-param name="nodeParam" select="."/>
		</xsl:call-template>
		
		<xsl:text>- </xsl:text><xsl:value-of select="text()"/><xsl:text>&#10;</xsl:text>
	</xsl:template>

	<!-- Attributes handling - do nothing -->
	<xsl:template match="@*">
		<xsl:apply-templates select="@*|node()" />
	</xsl:template>


	<!-- Print a number of tabs = ancestors number -->
	<xsl:template name="printTabs">
		<xsl:param name="nodeParam"/>

		<xsl:variable name="parentName" select="name($nodeParam/..)"/>
		<xsl:choose>
			<xsl:when test='$parentName = name(/node())'> <!-- Root is ignored. As soon as I reach the root, stop the recursion -->
			</xsl:when>
			<xsl:otherwise>
				<!-- Continue recursion: not reached root node -->
				<xsl:call-template name="printTabs">
					<xsl:with-param name="nodeParam" select="$nodeParam/.."/>
				</xsl:call-template>
				<xsl:text>&#x9;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>


