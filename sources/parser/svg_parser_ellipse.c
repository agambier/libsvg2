#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

svgItem* svgParseEllipse( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_ELLIPSE )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_ELLIPSE;

	//	cx
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"cx" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tEllipse.tX );
	//	cY
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"cy" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptItem->tParameters.tEllipse.tY );
	//	RadiusX
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"rx" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tEllipse.tRadiusX );
	//	RadiusY
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"ry" ) )!=NULL )
		svgStringToLength( szValue, &ptItem->tParameters.tEllipse.tRadiusY );


	return ptItem;
}
