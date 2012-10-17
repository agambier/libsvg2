#ifndef __svg_parser_h__
#define __svg_parser_h__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "svg.h"
#include "svg_types.h"
#include "svg_parser_circle.h"
#include "svg_parser_desc.h"
#include "svg_parser_group.h"
#include "svg_parser_line.h"
#include "svg_parser_path.h"
#include "svg_parser_rect.h"
#include "svg_parser_title.h"
#include "svg_parser_ellipse.h"
#include "svg_parser_polyline.h"
#include "svg_parser_polygon.h"

#ifdef __cplusplus
extern "C"
{
#endif


svgItem* svgNewItem( xmlNodePtr ptXmlNode );
svgError svgParseFile( xmlNodePtr ptXmlNode, svgDrawing *ptDrawing );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_h__
