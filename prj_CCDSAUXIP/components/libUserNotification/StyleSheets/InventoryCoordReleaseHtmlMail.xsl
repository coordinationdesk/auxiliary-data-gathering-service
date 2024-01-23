<?xml version="1.0" encoding="UTF-8"?>

<!-- Transform xml in html syntax -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="//MailNotification">
		<xsl:element name="html">
			<xsl:element name="head"/>
			<xsl:element name="body">
				<xsl:apply-templates select="//Filename|//List_of_Filetypes" />
			</xsl:element>	
		</xsl:element>
	</xsl:template>

	<xsl:template match="//Filename">
		<xsl:element name="p">
			<b><xsl:text>Inventoried Product: </xsl:text></b><xsl:value-of select='./text()'/><xsl:element name="br"/>
			<b><xsl:text>Filetype: </xsl:text></b><xsl:value-of select='../Filetype/text()'/><xsl:element name="br"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="//List_of_Filetypes">
		<xsl:element name="p">
			<b><xsl:text>Coordinated Release Group: </xsl:text></b><xsl:value-of select='../GroupName/text()'/><xsl:element name="br"/>
			<b><xsl:text>Group Description: </xsl:text></b><xsl:value-of select='../GroupDescription/text()'/><xsl:element name="br"/>
			<b><xsl:text>Filetypes in the group: </xsl:text></b><xsl:apply-templates select="//List_of_Filetypes/Filetype" />
		</xsl:element>
	</xsl:template>


	<xsl:template match="//List_of_Filetypes/Filetype">
		<LI><xsl:value-of select="text()"/><xsl:if test='text() = ../../Filetype/text()'><xsl:text> (x)</xsl:text></xsl:if></LI>
	</xsl:template>

</xsl:stylesheet>

