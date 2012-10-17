#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

svgItem* svgParseGroup( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;

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

