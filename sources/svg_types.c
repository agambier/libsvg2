#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svg_types.h"
#include "svg_errors.h"

const svgPathCommandFormat g_atPathCommandFormat[] = {
	{ 'M', 2, SVG_PATH_CMD_ID_MOVETO_ABS },
	{ 'm', 2, SVG_PATH_CMD_ID_MOVETO_REL },
	{ 'L', 2, SVG_PATH_CMD_ID_LINETO_ABS },
	{ 'l', 2, SVG_PATH_CMD_ID_LINETO_REL },
	{ 'V', 1, SVG_PATH_CMD_ID_VERTICAL_LINETO_ABS },
	{ 'v', 1, SVG_PATH_CMD_ID_VERTICAL_LINETO_REL },
	{ 'H', 1, SVG_PATH_CMD_ID_HORIZONTAL_LINETO_ABS },
	{ 'h', 1, SVG_PATH_CMD_ID_HORIZONTAL_LINETO_REL },
	{ 'Z', 0, SVG_PATH_CMD_ID_CLOSEPATH },
	{ 'z', 0, SVG_PATH_CMD_ID_CLOSEPATH },
	{ '\0', 0, 0 },
};


void svgStringToLength( const char *szValue, svgLength *ptLength )
{
	char szUnit[ 8 ] = "";

	if( szValue==NULL || ptLength==NULL )
		return;

	memset( ptLength, 0, sizeof( *ptLength ) );
	sscanf( szValue, "%lf%s", &ptLength->dValue, szUnit );

	SVG_DEBUG_PRINTF( "Value %#.4f\n", ptLength->dValue );

	ptLength->tUnit = SVG_LENGTH_UNIT_NONE;
	if( szUnit[ 0 ]=='\0' )
		return;

	if( strcmp( szUnit, "em" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_EM;
	else if( strcmp( szUnit, "ex" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_EX;
	else if( strcmp( szUnit, "in" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_IN;
	else if( strcmp( szUnit, "cm" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_CM;
	else if( strcmp( szUnit, "mm" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_MM;
	else if( strcmp( szUnit, "pt" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_PT;
	else if( strcmp( szUnit, "pc" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_PC;
	else if( strcmp( szUnit, "%" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_PERCENT;
	else if( strcmp( szUnit, "px" )==0 )
		ptLength->tUnit = SVG_LENGTH_UNIT_PX;
}

