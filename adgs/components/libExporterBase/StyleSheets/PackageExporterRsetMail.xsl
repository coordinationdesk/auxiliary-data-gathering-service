<?xml version="1.0" encoding="UTF-8"?>

<!-- Transform xml in ResourceSet syntax -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method = "text" omit-xml-declaration = "yes" indent = "no" />

	<!--xsl:template match="@*|node()">
		<xsl:if test='name() != ""'>
			<xsl:value-of select="name()"/><xsl:text>:</xsl:text><xsl:value-of select="text()"/><xsl:text>&#10;</xsl:text>
		</xsl:if>
		<xsl:apply-templates select="@*|node()" />
	</xsl:template-->


	<!-- Resource set Ignore root -->
	<xsl:template match="/node()">
		<!-- Do nothing and proceed -->
		<xsl:apply-templates select="@*|node()" />
	</xsl:template>

	<!-- Elements handling -->
	<xsl:template match="node()">

		<xsl:if test='name() != ""'>

			<!-- 
			AncestorKeys.Key:value 
			or			
			AncestorKeys.Key[i]:value
			-->
			<xsl:variable name="key">
				<xsl:call-template name="allAncestors">
					<xsl:with-param name="nodeParam" select="."/>
				</xsl:call-template>
			</xsl:variable>
			
			<xsl:choose>
				<xsl:when test="../@count">
					<xsl:value-of select="$key"/>[<xsl:value-of select="count(./preceding-sibling::*)"/>]:<xsl:value-of select="text()"/><xsl:text>&#10;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$key"/><xsl:text>:</xsl:text><xsl:value-of select="text()"/><xsl:text>&#10;</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			

			<!-- 
			AncestorKeys.Key.attributes[]:2
			AncestorKeys.Key.attributes[0]:attributeA
			AncestorKeys.Key.attributes[1]:attributeB
			-->
			<xsl:variable name="countAttr" select="count(@*)"/>
			<xsl:if test='$countAttr != 0'>
				<xsl:value-of select="$key"/><xsl:text>.attributes[]:</xsl:text><xsl:value-of select="$countAttr"/><xsl:text>&#10;</xsl:text>
			</xsl:if>

			<xsl:for-each select="@*">
				<xsl:value-of select="$key"/><xsl:text>.attributes[</xsl:text><xsl:value-of select="position()-1"/><xsl:text>]:</xsl:text><xsl:value-of select="name(.)"/><xsl:text>&#10;</xsl:text>
			</xsl:for-each>


			<!--
			Array case if "count" attribute defined
			
			AncestorKeys.Key[]:2
			-->
			<xsl:if test="@count">
				<xsl:variable name="countElem" select="count(./*)"/>
				<xsl:value-of select="$key"/><xsl:text>[]:</xsl:text><xsl:value-of select="$countElem"/><xsl:text>&#10;</xsl:text>
			</xsl:if>
		</xsl:if>


		<xsl:apply-templates select="@*|node()" />
	</xsl:template>


	<!-- Attributes handling -->
	<xsl:template match="@*">

		<!--		
		Ancestor.List_of_Files.count.value:2
		Ancestor.List_of_Files.count.type:CDATA
		-->

		<xsl:variable name="key">
			<xsl:call-template name="allAncestors">
				<xsl:with-param name="nodeParam" select="."/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:value-of select="$key"/><xsl:text>.value:</xsl:text><xsl:value-of select="."/><xsl:text>&#10;</xsl:text>
		<xsl:value-of select="$key"/><xsl:text>.type:CDATA</xsl:text><xsl:text>&#10;</xsl:text>

		<xsl:apply-templates select="@*|node()" />
	</xsl:template>


	<!-- Print all the ancestors -->
	<xsl:template name="allAncestors">

		<xsl:param name="nodeParam"/>

		<xsl:variable name="parentName" select="name($nodeParam/..)"/>
		<xsl:choose>
			<xsl:when test='$parentName = name(/node())'> <!-- Root is ignored in key. As soon as I reach the root, stop the recursion -->
				<xsl:value-of select="name($nodeParam/.)"/>
			</xsl:when>
			<xsl:otherwise>
				<!-- Continue recursion: not reached root node -->
				<xsl:call-template name="allAncestors">
					<xsl:with-param name="nodeParam" select="$nodeParam/.."/>
				</xsl:call-template>
				
				<xsl:text>.</xsl:text><xsl:value-of select="name($nodeParam/.)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>

<!--
UtcsTime:Thu Feb 14 17:50:48 CET 2013
UserId:testUset
Email:testUset@test.it
Repository:host/path/other/
Login:testLogin
Protocol:sftp
PackageId:001
List_of_Files:

List_of_Files.count.value:2
List_of_Files.count.type:CDATA
List_of_Files.attributes[]:1
List_of_Files.attributes[0]:count
List_of_Files.File[]:2
List_of_Files.File[0]:/dp01/file01.hdr
List_of_Files.File[1]:/dp01/file02.bin
-->

