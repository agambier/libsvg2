#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"


#ifdef __cplusplus
extern "C" {
#endif

svgItem* svgNewItem( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;

	ptItem = ( svgItem* )malloc( sizeof( *ptItem ) );
	if( ptItem==NULL )
	{
		svgSetLastError( SVG_ERR_NOT_ENOUGH_RAM, "Not enough RAM to create SVG drawing." );
		return NULL;
	}
	memset( ptItem, 0, sizeof( *ptItem ) );

	//	ID
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"id" ) )!=NULL )
		ptItem->szId = strdup( szValue );

	return ptItem;
}

svgItem* svgParseTitle( xmlNode * ptXmlNode )
{
	svgItem *ptItem = NULL;
	xmlNodePtr ptXmlValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_TITLE )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_TITLE;

	if( ( ptXmlValue = GetChild( ptXmlNode, XML_TEXT_NODE, "text" ) )!=NULL ) {
		if( ptXmlValue->content!=NULL )
			ptItem->tObject.tTitle.szText = strdup( ( char* )ptXmlValue->content );
	}

	return ptItem;
}

svgItem* svgParseDesc( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	xmlNodePtr ptXmlValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_DESC )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_DESC;

	if( ( ptXmlValue = GetChild( ptXmlNode, XML_TEXT_NODE, "text" ) )!=NULL ) {
		if( ptXmlValue->content!=NULL )
			ptItem->tObject.tTitle.szText = strdup( ( char* )ptXmlValue->content );
	}

	return ptItem;
}

svgItem* svgParseGroup( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_GROUP )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_GROUP;

	return ptItem;
}

svgItem* svgParsePath( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_PATH )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_PATH;

	return ptItem;
}

svgItem* svgParseRect( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_RECT )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_RECT;

	//	x
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"x" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tRect.tX );
	//	y
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"y" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tRect.tY );
	//	width
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"width" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tObject.tRect.tWidth );
	//	height
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"height" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tObject.tRect.tHeight );
	//	rx
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"rx" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tObject.tRect.tRadiusX );
	//	ry
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"ry" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tObject.tRect.tRadiusY );

	return ptItem;
}

svgItem* svgParseLine( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_LINE )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_LINE;

	//	x1
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"x1" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tLine.tX1 );
	//	Y1
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"y1" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tLine.tY1 );
	//	X2
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"x2" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tLine.tX2 );
	//	Y2
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"y2" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tLine.tY2 );


	return ptItem;
}

svgItem* svgParseCircle( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_CIRCLE )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_LINE;

	//	x
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"cx" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tCircle.tX );
	//	Y
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"cy" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tObject.tCircle.tY );
	//	radius
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"r" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tObject.tCircle.tRadius );


	return ptItem;
}
