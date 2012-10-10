#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "svg_errors.h"

static svgError g_tLastError = SVG_ERR_SUCCESS;
static char g_szLastErrorDesc[ 128 ] = "No error";

svgError svgSetLastError( svgError tError, const char *szDesc, ... )
{
	va_list tVaList;

	va_start( tVaList, szDesc );
	vsprintf( g_szLastErrorDesc, szDesc, tVaList );
	va_end( tVaList );

	g_tLastError = tError;
	return tError;
}

svgError svgGetLastError( void )
{
	return g_tLastError;
}

char* svgGetLastErrorDescription( void )
{
	return g_szLastErrorDesc;
}
