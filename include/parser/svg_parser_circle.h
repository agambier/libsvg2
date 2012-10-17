#ifndef __svg_parser_circle_h__
#define __svg_parser_circle_h__

#include "svg.h"
#include "svg_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SVG_TAG_CIRCLE		"circle"

svgItem* svgParseCircle( xmlNodePtr ptXmlNode );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_circle_h__
