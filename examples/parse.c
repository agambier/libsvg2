#include <stdio.h>
#include "svg.h"

int main( int iargc, char **argv )
{
	svgDrawing *ptSvg;

	if( iargc<2 )
		return -1;

	ptSvg = svgParseFile( argv[ 1 ] );

	svgFreeDrawing( ptSvg );

	return 0;
}
