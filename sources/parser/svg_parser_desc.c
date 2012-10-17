#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_xml.h"
#include "svg_types.h"
#include "svg_string.h"

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
			ptItem->tParameters.tTitle.szText = strdup( ( char* )ptXmlValue->content );
	}

	return ptItem;
}

