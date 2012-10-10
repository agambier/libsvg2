/*
	svg.h: Public interface for libsvg2

	Copyright (C) 2012 Alexandre Gambier <alex.gambier.dev@gmail.com>

	This program is free software and licensed under the GNU Lesser GPL (LGPL).

	This program is distributed WITHOUT ANY WARRANTY.

	FIXME: Finish this header...
	FIXME: Use doxygen to generate API documenation.
 */

#ifndef __svg_h__
#define __svg_h__

#include "svg_types.h"
#include "svg_errors.h"

typedef struct _svgDrawing {
	char *szVersion;
	char *szId;
	svgCoordinate tX;
	svgCoordinate tY;
	svgLength tWidth;
	svgLength tHeight;
} svgDrawing ;

svgDrawing* svgParseFile( const char *szFile );
void svgFreeDrawing( svgDrawing *ptDrawing );

#endif	//	__svg_h__
