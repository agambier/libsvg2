#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

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
		svgStringToCoordinate( szValue, &ptItem->tParameters.tRect.tX );
	//	y
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"y" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tRect.tY );
	//	width
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"width" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tRect.tWidth );
	//	height
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"height" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tRect.tHeight );
	//	rx
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"rx" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tRect.tRadiusX );
	//	ry
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"ry" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tRect.tRadiusY );

	return ptItem;
}
