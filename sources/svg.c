#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <unistd.h>
#include "svg.h"
#include "svg_parser.h"

svgDrawing* svgOpenFile( const char *szFile )
{
	svgDrawing *ptDrawing = NULL;
	xmlDocPtr ptXml = NULL;
    xmlNodePtr ptXmlRoot;
    char *szValue;

	//	check parameters
	if( szFile==NULL ) {
		svgSetLastError( SVG_ERR_INVALID_PARAMETER, "szFile can't be NULL."  );
		goto _exit;
	}
	if( access( szFile, F_OK )!=0 ) {
		svgSetLastError( SVG_ERR_NO_SUCH_FILE, "No such file." );
		goto _exit;
	}

	//	Open SVG file
	ptXml = xmlParseFile( szFile );
	if( ptXml==NULL ) {
		svgSetLastError( SVG_ERR_BAD_FILE_FORMAT, "Can't parse XML file." );
		goto _exit;
	}

    ptXmlRoot = xmlDocGetRootElement( ptXml );
    if( ptXmlRoot==NULL ) {
		svgSetLastError( SVG_ERR_BAD_FILE_FORMAT, "Can't get XML root node." );
		goto _exit;
    }
    if( strcmp( ( char* )ptXmlRoot->name, "svg" )!=0 ) {
		svgSetLastError( SVG_ERR_BAD_FILE_FORMAT, "Can't get SVG root node." );
		goto _exit;
	}

	//	Initialize SVG drawing
	ptDrawing = ( svgDrawing* )malloc( sizeof( *ptDrawing ) );
	if( ptDrawing==NULL ) {
		svgSetLastError( SVG_ERR_NOT_ENOUGH_RAM, "Not enough RAM to create SVG drawing." );
		goto _exit;
	}
	memset( ptDrawing, 0, sizeof( *ptDrawing ) );

	//	SVG's version number
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"version" ) )!=NULL )
		ptDrawing->szVersion = strdup( szValue );
	//	SVG's ID
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"id" ) )!=NULL )
		ptDrawing->szId = strdup( szValue );
	//	SVG's X coordinate
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"x" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptDrawing->tX );
	//	SVG's Y coordinate
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"y" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptDrawing->tY );
	//	SVG's Width
	ptDrawing->tWidth.fValue = 100;
	ptDrawing->tWidth.tUnit = SVG_LENGTH_UNIT_PERCENT;
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"width" ) )!=NULL )
		svgStringToLength( szValue, &ptDrawing->tWidth );
	//	SVG's Height
	ptDrawing->tHeight.fValue = 100;
	ptDrawing->tHeight.tUnit = SVG_LENGTH_UNIT_PERCENT;
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"height" ) )!=NULL )
		svgStringToLength( szValue, &ptDrawing->tHeight );


	//	Parse the svg objects
	svgParseFile( ptXmlRoot, ptDrawing );

_exit:
	//	Free memory
	if( ptXml!=NULL )
		xmlFreeDoc( ptXml );

	return ptDrawing;
}

void svgFreeItem( svgItem *ptItem )
{
	svgPathCommand *ptPathCmd, *ptNextPathCmd;
	svgPoint *ptPoint, *ptNextPoint;
	if( ptItem==NULL )
		return;

	if( ptItem->szId!=NULL )
		free( ptItem->szId );

	switch( ptItem->tKind )
	{
		case SVG_ITEM_KIND_GROUP:
			break;
		case SVG_ITEM_KIND_PATH:
			ptPathCmd = ptItem->tParameters.tPath.ptFirstCommand;
			while( ptPathCmd!=NULL ) {
				ptNextPathCmd = ptPathCmd->ptNextCommand;
				free( ptPathCmd );
				ptPathCmd = ptNextPathCmd;
			}
			break;
		case SVG_ITEM_KIND_RECT:
			break;
		case SVG_ITEM_KIND_CIRCLE:
			break;
		case SVG_ITEM_KIND_ELLIPSE:
			break;
		case SVG_ITEM_KIND_LINE:
			break;
		case SVG_ITEM_KIND_POLYLINE:
			ptPoint = ptItem->tParameters.tPolyline.tFirstPoint.ptNextPoint;
			while( ptPoint!=NULL ) {
				ptNextPoint = ptPoint->ptNextPoint;
				free( ptPoint );
				ptPoint = ptNextPoint;
			}
			break;
		case SVG_ITEM_KIND_POLYGON:
			ptPoint = ptItem->tParameters.tPolygon.tFirstPoint.ptNextPoint;
			while( ptPoint!=NULL ) {
				ptNextPoint = ptPoint->ptNextPoint;
				free( ptPoint );
				ptPoint = ptNextPoint;
			}
			break;
		case SVG_ITEM_KIND_TITLE:
			if( ptItem->tParameters.tTitle.szText!=NULL )
				free( ptItem->tParameters.tTitle.szText );
			break;
		case SVG_ITEM_KIND_DESC:
			if( ptItem->tParameters.tTitle.szText!=NULL )
				free( ptItem->tParameters.tDesc.szText );
			break;
	}

	free( ptItem );
}

void svgFreeDrawing( svgDrawing *ptDrawing )
{
	svgItem *ptItem, *ptNextItem;
	if( ptDrawing==NULL )
		return;

	//	Common attributes
	if( ptDrawing->szId!=NULL )
		free( ptDrawing->szId );
	if( ptDrawing->szVersion!=NULL )
		free( ptDrawing->szVersion );

	//	Free items...We use the unsorted list to make it simpler
	ptItem = ptDrawing->tItemList.ptItem;
	while( ptItem!=NULL ){
		//	since we'll free the current item we must store the location to the next one
		ptNextItem = ptItem->ptNextUnsortedItem;
		svgFreeItem( ptItem );

		//	Next
		ptItem = ptNextItem;
	}

	free( ptDrawing );
}

