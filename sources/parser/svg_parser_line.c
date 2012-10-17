#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

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
		svgStringToCoordinate( szValue, &ptItem->tParameters.tLine.tX1 );
	//	Y1
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"y1" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tLine.tY1 );
	//	X2
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"x2" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tLine.tX2 );
	//	Y2
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"y2" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tLine.tY2 );


	return ptItem;
}

