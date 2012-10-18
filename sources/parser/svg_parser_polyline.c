#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

svgItem* svgParsePolyline( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue, szField[ 16 ];
	const char *szFieldStart;
	svgPoint *ptNewPoint = NULL, *ptLastPoint = NULL;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_POLYLINE )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_POLYLINE;

	//	Points
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"points" ) )!=NULL ) {

		szFieldStart = svgGetNextPointField( szValue, szField );

		while( szFieldStart!=NULL ) {
			//	First point ?
			if( ptNewPoint==NULL )
				ptNewPoint = &ptItem->tParameters.tPolyline.tFirstPoint;
			else
				ptNewPoint = ( svgPoint* )malloc( sizeof( *ptNewPoint ) );
			memset( ptNewPoint, 0, sizeof( *ptNewPoint ) );

			//	X
			svgStringToCoordinate( szField, &ptNewPoint->tX );

			//	Y
			szFieldStart += strlen( szField );
			szFieldStart = svgGetNextPointField( szFieldStart, szField );
			svgStringToCoordinate( szField, &ptNewPoint->tY );

			//	Link with last point
			if( ptLastPoint!=NULL )
				ptLastPoint->ptNextPoint = ptNewPoint;
			ptLastPoint = ptNewPoint;

			//	Next X
			szFieldStart += strlen( szField );
			szFieldStart = svgGetNextPointField( szFieldStart, szField );
		}
	}

	return ptItem;
}
