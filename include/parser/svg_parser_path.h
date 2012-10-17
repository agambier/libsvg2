#ifndef __svg_parser_path_h__
#define __svg_parser_path_h__

#include "svg.h"
#include "svg_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define	SVG_TAG_PATH		"path"

svgItem* svgParsePath( xmlNodePtr ptXmlNode );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_parser_path_h__
