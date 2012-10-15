#include <stdio.h>
#include <string.h>
#include "svg_xml.h"

xmlNodePtr GetChild( xmlNodePtr ptXmlNode, xmlElementType tType, char *szChildName )
{
	xmlNodePtr ptXmlChild;

	if( ptXmlNode==NULL || szChildName==NULL )
		return NULL;

	ptXmlChild = ptXmlNode->children;
	while( ptXmlChild!=NULL )
	{
		if( ptXmlChild->type==tType && strcmp( ( char* )ptXmlChild->name, szChildName )==0 )
			break;
	}

	return ptXmlChild;
}
