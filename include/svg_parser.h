#ifndef __svg_parser_h__
#define __svg_parser_h__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "svg_types.h"
#include "svg.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define	SVG_TAG_GROUP		"g"
#define	SVG_TAG_PATH		"path"
#define SVG_TAG_RECT		"rect"
#define SVG_TAG_CIRCLE		"circle"
#define SVG_TAG_ELLIPSE		"ellipse"
#define SVG_TAG_LINE		"line"
#define SVG_TAG_POLYLINE	"polyline"
#define SVG_TAG_POLYGON		"polygon"
#define SVG_TAG_TITLE		"title"
#define SVG_TAG_DESC		"desc"

svgItem* svgParseTitle( xmlNodePtr ptXmlNode );
svgItem* svgParseDesc( xmlNodePtr ptXmlNode );
svgItem* svgParseGroup( xmlNodePtr ptXmlNode );
svgItem* svgParsePath( xmlNodePtr ptXmlNode );
svgItem* svgParseRect( xmlNodePtr ptXmlNode );
svgItem* svgParseLine( xmlNodePtr ptXmlNode );
svgItem* svgParseCircle( xmlNodePtr ptXmlNode );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_h__
