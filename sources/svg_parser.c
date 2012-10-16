#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
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
	char cAddCmd;

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
		cAddCmd = 0;
		ptLastPathCmd = NULL;
		szFieldStart = svgGetNextPathField( szValue, szField );
		while( szFieldStart!=NULL ) {

			ptPathCmd = NULL;
			//	What's this command ?
			if( strcmp( szField, "M" ) == 0 ) {
				//	MoveTo ABS
				ptPathCmd = svgNewPathCommand( SVG_PATH_CMD_ID_MOVETO_ABS );

				//	X Value
				szFieldStart += strlen( szField );
				if( ( szFieldStart = svgGetNextPathField( szFieldStart, szField ) )!=NULL ) {
					if( svgIsRealNumber( szField )!= 0)
						svgStringToCoordinate( szField, &ptPathCmd->tParameters.tMoveTo.tX );
				}
				//	Y Value
				szFieldStart += strlen( szField );
				if( ( szFieldStart = svgGetNextPathField( szFieldStart, szField ) )!=NULL ) {
					if( svgIsRealNumber( szField )!= 0)
						svgStringToCoordinate( szField, &ptPathCmd->tParameters.tMoveTo.tY );
				}
			}
			else if( strcmp( szField, "m" ) == 0 ) {
				//	MoveTo REL
				ptPathCmd = svgNewPathCommand( SVG_PATH_CMD_ID_MOVETO_REL );

				//	X Value
				szFieldStart += strlen( szField );
				if( ( szFieldStart = svgGetNextPathField( szFieldStart, szField ) )!=NULL ) {
					if( svgIsRealNumber( szField )!= 0)
						svgStringToCoordinate( szField, &ptPathCmd->tParameters.tMoveTo.tX );
				}
				//	Y Value
				szFieldStart += strlen( szField );
				if( ( szFieldStart = svgGetNextPathField( szFieldStart, szField ) )!=NULL ) {
					if( svgIsRealNumber( szField )!= 0)
						svgStringToCoordinate( szField, &ptPathCmd->tParameters.tMoveTo.tY );
				}
			}
			else if( strcasecmp( szField, "z" ) == 0 ) {
				//	Close path
				ptPathCmd = svgNewPathCommand( SVG_PATH_CMD_ID_CLOSEPATH );
			}
			else if( svgIsRealNumber( szField )!= 0) {
				SVG_DEBUG_PRINTF( "Number = %s\n", szField );
			}
			else {
				SVG_DEBUG_PRINTF( "Not yet supported = %s\n", szField );
			}

			if( ptPathCmd!=NULL ) {

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
