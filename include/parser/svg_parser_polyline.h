#ifndef __svg_parser_polyline_h__
#define __svg_parser_polyline_h__

#include "svg.h"
#include "svg_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SVG_TAG_POLYLINE	"polyline"

svgItem* svgParsePolyline( xmlNodePtr ptXmlNode );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_polyline_h__
