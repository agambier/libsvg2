#ifndef __svg_parser_group_h__
#define __svg_parser_group_h__

#include "svg.h"
#include "svg_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define	SVG_TAG_GROUP		"g"

svgItem* svgParseGroup( xmlNodePtr ptXmlNode );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_group_h__
