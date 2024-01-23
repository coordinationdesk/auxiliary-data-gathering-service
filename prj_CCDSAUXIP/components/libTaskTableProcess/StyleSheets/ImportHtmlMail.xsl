<?xml version="1.0" encoding="UTF-8"?>

<!-- Transform xml in html syntax -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="//MailNotification">
		<xsl:element name="html">
			<xsl:element name="head"/>
			<xsl:element name="body">
				<xsl:element name="p">
					<xsl:text>This is an automatic message from Importer.</xsl:text><xsl:element name="br"/>
				</xsl:element>
				<xsl:element name="p">
					<xsl:apply-templates select="//Success" />
					<LI><xsl:value-of select='./Filename/text()'/><xsl:element name="br"/></LI>
				</xsl:element>
			</xsl:element>	
		</xsl:element>
	</xsl:template>
	

	<xsl:template match="//Success[./text() = 'true']">
		<xsl:text>The following Item has been imported correctly:</xsl:text><xsl:element name="br"/>
	</xsl:template>

	<xsl:template match="//Success[./text() = 'false']">
		<xsl:text>Error occured importing Item:</xsl:text><xsl:element name="br"/>
	</xsl:template>

</xsl:stylesheet>

