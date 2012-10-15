#ifndef __svg_xml_h__
#define __svg_xml_h__

#include <libxml/parser.h>
#include <libxml/tree.h>


#ifdef __cplusplus
extern "C"
{
#endif

xmlNodePtr GetChild( xmlNodePtr ptXmlNode, xmlElementType tType, char *szChildName );


#ifdef __cplusplus
}
#endif


#endif	//	 __svg_xml_h__
