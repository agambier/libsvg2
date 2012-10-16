#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <unistd.h>
#include "svg.h"
#include "svg_parser.h"

svgDrawing* svgParseFile( const char *szFile )
{
	svgDrawing *ptDrawing = NULL;
	xmlDocPtr ptXml = NULL;
    xmlNodePtr ptXmlRoot, ptXmlNode;
    char *szValue;
    svgItem *ptNewItem; /* New parsed item */
    svgItem *ptLastItem; /* Last created item */
    svgItem *ptCurrentParent; /* Current parent, NULL if root level */
    svgItem *ptLastBrother; /* Last item that should be the brother of the new parsed item */
    svgItem *ptLastRootLevelItem; /* Last item on the root level */

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
	ptDrawing->tWidth.dValue = 100;
	ptDrawing->tWidth.tUnit = SVG_LENGTH_UNIT_PERCENT;
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"width" ) )!=NULL )
		svgStringToLength( szValue, &ptDrawing->tWidth );
	//	SVG's Height
	ptDrawing->tHeight.dValue = 100;
	ptDrawing->tHeight.tUnit = SVG_LENGTH_UNIT_PERCENT;
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"height" ) )!=NULL )
		svgStringToLength( szValue, &ptDrawing->tHeight );

	//	Parse SVG file
    ptCurrentParent = NULL;
    ptLastItem = NULL;
    ptLastBrother = NULL;
    ptLastRootLevelItem = NULL;
    ptXmlNode = ptXmlRoot->xmlChildrenNode;
    while( ptXmlNode ) {

    	ptNewItem = NULL;

    	if( ptXmlNode->type == XML_ELEMENT_NODE ) {
    		if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_TITLE )==0 ) {
    			//	Group
    			ptNewItem = svgParseTitle( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_DESC )==0 ) {
    			//	Group
    			ptNewItem = svgParseDesc( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_GROUP )==0 ) {
    			//	Group
    			ptNewItem = svgParseGroup( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_PATH )==0 ) {
    			//	Path
    			ptNewItem = svgParsePath( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_RECT )==0 ) {
    			//	Rectangle
    			ptNewItem = svgParseRect( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_CIRCLE )==0 ) {
    			//	Circle
    			SVG_DEBUG_PRINTF( "SVG Item : %s not implemented yet!\n", ptXmlNode->name );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_ELLIPSE )==0 ) {
    			//	Ellipse
    			SVG_DEBUG_PRINTF( "SVG Item : %s not implemented yet!\n", ptXmlNode->name );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_LINE )==0 ) {
    			//	Line
    			ptNewItem = svgParseLine( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_POLYLINE )==0 ) {
    			//	Polyline
    			SVG_DEBUG_PRINTF( "SVG Item : %s not implemented yet!\n", ptXmlNode->name );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_POLYGON )==0 ) {
    			//	Polygon
    			SVG_DEBUG_PRINTF( "SVG Item : %s not implemented yet!\n", ptXmlNode->name );
    		}
    		else {
    			SVG_DEBUG_PRINTF( "Unknown or unsupported SVG item : %s\n", ptXmlNode->name );
    		}

    		if( ptNewItem!=NULL )
    		{
    			//	Is it the first item ?
    			if( ptDrawing->tItemList.ptItem==NULL )
    				ptDrawing->tItemList.ptItem = ptNewItem;

    			//	Add it to the parent
    			if( ptCurrentParent!=NULL ) {
    				//	Link it to its brother "same level"
    				ptLastBrother = ptCurrentParent->ptLastChild;

    				if( ptCurrentParent->ptFirstChild==NULL )
    					ptCurrentParent->ptFirstChild = ptNewItem;

					ptCurrentParent->ptLastChild = ptNewItem;
    			}
    			else {
    				ptLastBrother = ptLastRootLevelItem;
    				ptLastRootLevelItem = ptNewItem;
    			}

				if( ptLastBrother!=NULL && ptLastBrother->ptParent==ptCurrentParent )
					ptLastBrother->ptNextItem = ptNewItem;

    			ptNewItem->ptParent = ptCurrentParent;

    			//	Link current item with previous one
    			if( ptLastItem!=NULL )
    				ptLastItem->ptNextUnsortedItem = ptNewItem;

    			ptLastItem = ptNewItem;
    			ptDrawing->tItemList.ui32Count ++;

    			//	We have to parse its children
    			if( ptXmlNode->xmlChildrenNode!=NULL ) {
    				ptXmlNode = ptXmlNode->xmlChildrenNode;
    				ptCurrentParent = ptNewItem;
    			}
    		}
    	}

    	//	Next node
    	if( ptXmlNode->next!=NULL )
    		ptXmlNode = ptXmlNode->next;
    	else {
    		//	Maybe we have get back to a upper level in the tree
    		if( ptXmlNode->parent!=NULL )
    			ptXmlNode = ptXmlNode->parent->next;
    		else
    			ptXmlNode = NULL;

    		if( ptCurrentParent!=NULL )
    			ptCurrentParent = ptCurrentParent->ptParent;
    	}
    }

_exit:
	//	Free memory
	if( ptXml!=NULL )
		xmlFreeDoc( ptXml );

	return ptDrawing;
}

void svgFreeItem( svgItem *ptItem )
{
	svgPathCommand *ptPathCmd, *ptNextPathCmd;
	if( ptItem==NULL )
		return;

	SVG_DEBUG_PRINTF( "Freeing item %s\n", ptItem->szId );

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
			break;
		case SVG_ITEM_KIND_POLYGON:
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

