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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _svgItem {
	char *szId;
	struct _svgItem *ptParent;				/* Parent item */
	struct _svgItem *ptFirstChild;			/* Next child, usually when item is a group */
	struct _svgItem *ptLastChild;			/* Last child, usually when item is a group */
	struct _svgItem *ptNextItem;			/* Next item stored as a tree */
	struct _svgItem *ptNextUnsortedItem;	/* Next item but not stored as a tree */
} svgItem ;

typedef struct _svgItemList {
	svgItem *ptItem;	/* First item */
	uint32 ui32Count;	/* Number of items */
} svgItemList ;

typedef struct _svgDrawing {
	char *szVersion;
	char *szId;
	svgCoordinate tX;
	svgCoordinate tY;
	svgLength tWidth;
	svgLength tHeight;
	svgItemList tItemList;
} svgDrawing ;

svgDrawing* svgParseFile( const char *szFile );
void svgFreeDrawing( svgDrawing *ptDrawing );

#ifdef __cplusplus
}
#endif

#endif	//	__svg_h__
