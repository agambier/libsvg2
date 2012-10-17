#ifndef __svg_parser_title_h__
#define __svg_parser_title_h__

#include "svg.h"
#include "svg_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SVG_TAG_TITLE		"title"

svgItem* svgParseTitle( xmlNodePtr ptXmlNode );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_title_h__
