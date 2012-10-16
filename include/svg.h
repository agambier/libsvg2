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

typedef enum _svgItemKind {
	SVG_ITEM_KIND_GROUP = 0,
	SVG_ITEM_KIND_PATH,
	SVG_ITEM_KIND_RECT,
	SVG_ITEM_KIND_CIRCLE,
	SVG_ITEM_KIND_ELLIPSE,
	SVG_ITEM_KIND_LINE,
	SVG_ITEM_KIND_POLYLINE,
	SVG_ITEM_KIND_POLYGON,
	SVG_ITEM_KIND_TITLE,
	SVG_ITEM_KIND_DESC,
} svgItemKind ;

typedef struct _svgItem {
	svgItemKind tKind;
	char *szId;
	struct _svgItem *ptParent;				/* Parent item */
	struct _svgItem *ptFirstChild;			/* Next child, usually when item is a group */
	struct _svgItem *ptLastChild;			/* Last child, usually when item is a group */
	struct _svgItem *ptNextItem;			/* Next item stored as a tree */
	struct _svgItem *ptNextUnsortedItem;	/* Next item but not stored as a tree */

	union {
		svgTitle tTitle;	/* tKind==SVG_ITEM_KIND_TITLE */
		svgDesc tDesc;		/* tKind==SVG_ITEM_KIND_DESC */
		svgLine tLine;		/* tKind==SVG_ITEM_KIND_LINE */
		svgRect tRect;		/* tKind==SVG_ITEM_KIND_RECT */
		svgCircle tCircle;	/* tKind==SVG_ITEM_KIND_CIRCLE */
		svgPath tPath;		/* tKind==SVG_ITEM_KIND_PATH */
	} tParameters ;
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
