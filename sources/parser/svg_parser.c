#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_types.h"
#include "svg_xml.h"
#include "svg_string.h"


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

svgError svgParseFile( xmlNodePtr ptXmlRoot, svgDrawing *ptDrawing )
{
    svgItem *ptNewItem; /* New parsed item */
    svgItem *ptLastItem; /* Last created item */
    svgItem *ptCurrentParent; /* Current parent, NULL if root level */
    svgItem *ptLastBrother; /* Last item that should be the brother of the new parsed item */
    svgItem *ptLastRootLevelItem; /* Last item on the root level */
    xmlNodePtr ptXmlNode;

    if( ptXmlRoot==NULL )
    	return svgSetLastError( SVG_ERR_INVALID_PARAMETER, "ptXmlRoot can't be NULL" );
    if( ptDrawing==NULL )
    	return svgSetLastError( SVG_ERR_INVALID_PARAMETER, "ptDrawing can't be NULL" );

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
    			ptNewItem = svgParseCircle( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_ELLIPSE )==0 ) {
    			//	Ellipse
    			ptNewItem = svgParseEllipse( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_LINE )==0 ) {
    			//	Line
    			ptNewItem = svgParseLine( ptXmlNode );
    		}
    		else if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_POLYLINE )==0 ) {
    			//	Polyline
    			ptNewItem = svgParsePolyline( ptXmlNode );
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

    return svgSetLastError( SVG_ERR_SUCCESS, NULL );
}



