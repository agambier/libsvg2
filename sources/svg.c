#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <unistd.h>
#include "svg.h"

svgDrawing* svgParseFile( const char *szFile )
{
	svgDrawing *ptDrawing = NULL;
	xmlDocPtr ptXml = NULL;
    xmlNodePtr ptXmlRoot, ptXmlNode;
    char *szValue;

	//	check parameters
	if( szFile==NULL ) {
		svgSetLastError( SVG_ERR_INVALID_PARAMETER, "szFile can't be NULL."  );
		goto _exit;
	}
	if( access( szFile, F_OK )!=0 ) {
		svgSetLastError( SVG_ERR_NO_SUCH_FILE, "No such file." );
		goto _exit;
	}

	//	Open SVG file
	ptXml = xmlParseFile( szFile );
	if( ptXml==NULL ) {
		svgSetLastError( SVG_ERR_BAD_FILE_FORMAT, "Can't parse XML file." );
		goto _exit;
	}

    ptXmlRoot = xmlDocGetRootElement( ptXml );
    if( ptXmlRoot==NULL ) {
		svgSetLastError( SVG_ERR_BAD_FILE_FORMAT, "Can't get XML root node." );
		goto _exit;
    }
    if( strcmp( ( char* )ptXmlRoot->name, "svg" )!=0 ) {
		svgSetLastError( SVG_ERR_BAD_FILE_FORMAT, "Can't get SVG root node." );
		goto _exit;
	}

	//	Initialize SVG drawing
	ptDrawing = ( svgDrawing* )malloc( sizeof( *ptDrawing ) );
	if( ptDrawing==NULL ) {
		svgSetLastError( SVG_ERR_NOT_ENOUGH_RAM, "Not enough RAM to create SVG drawing." );
		goto _exit;
	}
	memset( ptDrawing, 0, sizeof( *ptDrawing ) );

	//	SVG's version number
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"version" ) )!=NULL )
		ptDrawing->szVersion = strdup( szValue );
	//	SVG's ID
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"id" ) )!=NULL )
		ptDrawing->szId = strdup( szValue );
	//	SVG's X coordinate
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"x" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptDrawing->tX );
	//	SVG's Y coordinate
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"y" ) )!=NULL )
		svgStringToCoordinate( szValue, &ptDrawing->tY );
	//	SVG's Width
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"width" ) )!=NULL )
		svgStringToLength( szValue, &ptDrawing->tWidth );
	//	SVG's Height
	if( ( szValue = ( char* )xmlGetProp( ptXmlRoot, ( xmlChar* )"height" ) )!=NULL )
		svgStringToLength( szValue, &ptDrawing->tHeight );

	//	Parse SVG file
    ptXmlNode = ptXmlRoot->xmlChildrenNode;
    while( ptXmlNode ) {
    	if( ptXmlNode->type == XML_ELEMENT_NODE ) {
    		if( strcmp( ( char* )ptXmlNode->name, "g" )==0 ) {
    			printf( "Group node\n" );
    		}
    		else
    			printf( "XmlNode Name = %s\n", ptXmlNode->name );
    	}

    	//	Next node
    	ptXmlNode = ptXmlNode->next;
    }

_exit:
	//	Free memory
	if( ptXml!=NULL )
		xmlFreeDoc( ptXml );

	return ptDrawing;
}

void svgFreeDrawing( svgDrawing *ptDrawing )
{
	if( ptDrawing==NULL )
		return;

	if( ptDrawing->szId!=NULL )
		free( ptDrawing->szId );
	if( ptDrawing->szVersion!=NULL )
		free( ptDrawing->szVersion );

	free( ptDrawing );
}

