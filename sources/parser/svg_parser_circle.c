#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

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

	ptItem->tKind = SVG_ITEM_KIND_CIRCLE;

	//	x
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"cx" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tCircle.tX );
	//	Y
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"cy" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tCircle.tY );
	//	radius
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"r" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tCircle.tRadius );


	return ptItem;
}

