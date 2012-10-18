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
	{ 'C', 6, SVG_PATH_CMD_ID_CUBIC_CURVETO_ABS },
	{ 'c', 6, SVG_PATH_CMD_ID_CUBIC_CURVETO_REL },
	{ 'S', 4, SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_ABS },
	{ 's', 4, SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_REL },
	{ 'Q', 4, SVG_PATH_CMD_ID_QUADRATIC_CURVETO_ABS },
	{ 'q', 4, SVG_PATH_CMD_ID_QUADRATIC_CURVETO_REL },
	{ 'T', 2, SVG_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO_ABS },
	{ 't', 2, SVG_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO_REL },
	{ 'A', 7, SVG_PATH_CMD_ID_ARCTO_ABS },
	{ 'a', 7, SVG_PATH_CMD_ID_ARCTO_REL },
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
	sscanf( szValue, "%f%s", &ptLength->fValue, szUnit );

	SVG_DEBUG_PRINTF( "Value %#.4f\n", ptLength->fValue );

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

