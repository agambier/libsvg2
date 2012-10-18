#include <stdio.h>
#include <string.h>
#include "svg_parser.h"
#include "svg_types.h"
#include "svg_string.h"


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
		"SVG_PATH_CMD_ID_CUBIC_CURVETO_ABS",
		"SVG_PATH_CMD_ID_CUBIC_CURVETO_REL",
		"SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_ABS",
		"SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_REL",
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
						if( g_atPathCommandFormat[ uiCmdIdx ].tId==ptPathCmd->tId ) {
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
				for( iI = 0; ( iI < g_atPathCommandFormat[ uiCmdIdx ].i8NbrOfArgs ) && ( iI < ( int8 )SVG_ARRAY_SIZE( atArgs ) ); iI ++ ) {
					atArgs[ iI ].fValue = 0;
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

					//	Cubic CurveTo
					case SVG_PATH_CMD_ID_CUBIC_CURVETO_ABS:
					case SVG_PATH_CMD_ID_CUBIC_CURVETO_REL:
						ptPathCmd->tParameters.tCubicCurveTo.tX1 = atArgs[ 0 ];
						ptPathCmd->tParameters.tCubicCurveTo.tY1 = atArgs[ 1 ];
						ptPathCmd->tParameters.tCubicCurveTo.tX2 = atArgs[ 2 ];
						ptPathCmd->tParameters.tCubicCurveTo.tY2 = atArgs[ 3 ];
						ptPathCmd->tParameters.tCubicCurveTo.tX = atArgs[ 4 ];
						ptPathCmd->tParameters.tCubicCurveTo.tY = atArgs[ 5 ];
						break;

					//	Smooth Cubic CurveTo
					case SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_ABS:
					case SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_REL:
						ptPathCmd->tParameters.tSmoothCubicCurveTo.tX2 = atArgs[ 0 ];
						ptPathCmd->tParameters.tSmoothCubicCurveTo.tY2 = atArgs[ 1 ];
						ptPathCmd->tParameters.tSmoothCubicCurveTo.tX = atArgs[ 2 ];
						ptPathCmd->tParameters.tSmoothCubicCurveTo.tY = atArgs[ 3 ];
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
