#include <stdio.h>
#include "svg.h"

svgDrawingP svgParseFile( const char *szFile )
{
	svgDrawingP tDrawing = NULL;
	if( szFile==NULL )
		return NULL;
	
	return tDrawing;
}
	
void svgFreeDrawing( svgDrawingP tDrawing )
{
	if( tDrawing==NULL )
		return;
}
	
