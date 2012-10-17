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

svgPathCommand* svgNewPathCommand( svgPathCommandId tId )
{
	svgPathCommand *ptCmd = NULL;

#if !defined( NDEBUG )
	char t[ SVG_PATH_CMD_ID_CLOSEPATH + 1 ][ 128 ] = {
		"SVG_PATH_CMD_ID_MOVETO_ABS",
		"SVG_PATH_CMD_ID_MOVETO_REL",
		"SVG_PATH_CMD_ID_LINETO_ABS",
		"SVG_PATH_CMD_ID_LINETO_REL",
		"SVG_PATH_CMD_ID_VERTICAL_LINETO_ABS",
		"SVG_PATH_CMD_ID_VERTICAL_LINETO_REL",
		"SVG_PATH_CMD_ID_HORIZONTAL_LINETO_ABS",
		"SVG_PATH_CMD_ID_HORIZONTAL_LINETO_REL",
		"SVG_PATH_CMD_ID_CLOSEPATH"
	};
	SVG_DEBUG_PRINTF( "Adding Path COmmand %s\n", t[ tId ] );
#endif

	ptCmd = ( svgPathCommand* )malloc( sizeof( *ptCmd ) );
	if( ptCmd==NULL )
	{
		svgSetLastError( SVG_ERR_NOT_ENOUGH_RAM, "Not enough RAM to create path command." );
		return NULL;
	}
	memset( ptCmd, 0, sizeof( *ptCmd ) );

	ptCmd->tId = tId;

	return ptCmd;
}

svgItem* svgParseTitle( xmlNode * ptXmlNode )
{
	svgItem *ptItem = NULL;
	xmlNodePtr ptXmlValue;

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_TITLE )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_TITLE;

	if( ( ptXmlValue = GetChild( ptXmlNode, XML_TEXT_NODE, "text" ) )!=NULL ) {
		if( ptXmlValue->content!=NULL )
			ptItem->tParameters.tTitle.szText = strdup( ( char* )ptXmlValue->content );
	}

	return ptItem;
}

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

svgItem* svgParsePath( xmlNodePtr ptXmlNode )
{
	svgItem *ptItem = NULL;
	char *szValue, szField[ 16 ];
	const char *szFieldStart;
	svgPathCommand *ptPathCmd, *ptLastPathCmd = NULL;
	unsigned int uiCmdIdx;
	int8 iI;
	svgLength atArgs[ 12 ];

	if( ptXmlNode==NULL )
		return NULL;
	if( strcmp( ( char* )ptXmlNode->name, SVG_TAG_PATH )!=0 )
		return NULL;

	//	Read common values to all kind of item
	ptItem = svgNewItem( ptXmlNode );
	if( ptItem==NULL )
		return NULL;

	ptItem->tKind = SVG_ITEM_KIND_PATH;

	//	d
	if( ( szValue = ( char* )xmlGetProp( ptXmlNode, ( xmlChar* )"d" ) )!=NULL ) {

		SVG_DEBUG_PRINTF( "Path d = %s\n", szValue );
		ptLastPathCmd = NULL;
		szFieldStart = svgGetNextPathField( szValue, szField );
		while( szFieldStart!=NULL ) {

			ptPathCmd = NULL;

			//	What's this command ?
			if( svgIsRealNumber( szField )!= 0) {

				//	No command so use last command
				if( ptLastPathCmd!=NULL ) {

					switch( ptLastPathCmd->tId ) {
						//	MoveTo => LineTo
						case SVG_PATH_CMD_ID_MOVETO_ABS:
						case SVG_PATH_CMD_ID_MOVETO_REL:
							ptPathCmd = svgNewPathCommand( ( ptLastPathCmd->tId==SVG_PATH_CMD_ID_MOVETO_REL ) ? SVG_PATH_CMD_ID_LINETO_REL : SVG_PATH_CMD_ID_LINETO_ABS );
							break;
						//	Repeated command
						default:
							ptPathCmd = svgNewPathCommand( ptLastPathCmd->tId );
							break;
					}

					//	Search command format
					for( uiCmdIdx = 0; g_atPathCommandFormat[ uiCmdIdx ].cCommand!='\0'; uiCmdIdx ++ ) {
						if( g_atPathCommandFormat[ uiCmdIdx ].tId==ptLastPathCmd->tId ) {
							break;
						}
					}

					//	Since we already read the parameter of the command we don't want to skip when we'll parse the command's parameters.
					szField[ 0 ] = 0;
				}
			}
			else {
				//	Search command format
				for( uiCmdIdx = 0; g_atPathCommandFormat[ uiCmdIdx ].cCommand!='\0'; uiCmdIdx ++ ) {
					if( g_atPathCommandFormat[ uiCmdIdx ].cCommand==szField[ 0 ] ) {
						ptPathCmd = svgNewPathCommand( g_atPathCommandFormat[ uiCmdIdx ].tId );
						break;
					}
				}
			}

			if( ptPathCmd!=NULL ) {
				//	Parse args
				for( iI = 0; iI < g_atPathCommandFormat[ uiCmdIdx ].i8NbrOfArgs; iI ++ ) {
					szFieldStart += strlen( szField );
					if( ( szFieldStart = svgGetNextPathField( szFieldStart, szField ) )!=NULL ) {
						if( svgIsRealNumber( szField )!= 0)
							svgStringToCoordinate( szField, &atArgs[ iI ] );
					}
				}

				//	Parse the command parameters
				switch( ptPathCmd->tId ) {
					//	MoveTo
					case SVG_PATH_CMD_ID_MOVETO_ABS:
					case SVG_PATH_CMD_ID_MOVETO_REL:
						ptPathCmd->tParameters.tMoveTo.tX = atArgs[ 0 ];
						ptPathCmd->tParameters.tMoveTo.tY = atArgs[ 1 ];
						break;

					//	LineTo
					case SVG_PATH_CMD_ID_LINETO_ABS:
					case SVG_PATH_CMD_ID_LINETO_REL:
						ptPathCmd->tParameters.tLineTo.tX = atArgs[ 0 ];
						ptPathCmd->tParameters.tLineTo.tY = atArgs[ 1 ];
						break;

					//	Vertical LineTo
					case SVG_PATH_CMD_ID_VERTICAL_LINETO_ABS:
					case SVG_PATH_CMD_ID_VERTICAL_LINETO_REL:
						ptPathCmd->tParameters.tLineTo.tY = atArgs[ 0 ];
						break;

					//	Horizontal LineTo
					case SVG_PATH_CMD_ID_HORIZONTAL_LINETO_ABS:
					case SVG_PATH_CMD_ID_HORIZONTAL_LINETO_REL:
						ptPathCmd->tParameters.tLineTo.tX = atArgs[ 0 ];
						break;

					//	No parameters
					case SVG_PATH_CMD_ID_CLOSEPATH:
					default:
						break;
				}

				//	Store the new command into the command list
				if( ptItem->tParameters.tPath.ptFirstCommand==NULL )
					ptItem->tParameters.tPath.ptFirstCommand = ptPathCmd;

				if( ptLastPathCmd!=NULL )
					ptLastPathCmd->ptNextCommand = ptPathCmd;

				ptLastPathCmd = ptPathCmd;
			}

			//	Next
			szFieldStart += strlen( szField );
			szFieldStart = svgGetNextPathField( szFieldStart, szField );
		}
	}


	return ptItem;
}

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

	ptItem->tKind = SVG_ITEM_KIND_LINE;

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
