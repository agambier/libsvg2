#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svg_string.h"



const char* svgGetNextPathField( const char *szData, char *szField )
{
	const char *szStart = NULL, *szEnd = NULL;

	if( szData==NULL || szField==NULL )
		return NULL;

	szField[ 0 ] = '\0';

	//	Search for the start of the field
	szStart = szData;
	while( *szStart!='\0' && ( *szStart=='\t' || *szStart=='\r' || *szStart=='\n' || *szStart==' ' || *szStart==',' ) ) {
		szStart ++;
	}

	if( *szStart=='\0' )
		return NULL;

	//	Search for the end
	szEnd = szStart + 1;
	while( *szEnd!='\0' && *szEnd!='\t' && *szEnd!='\r' && *szEnd!='\n' && *szEnd!=' ' && *szEnd!=',' ) {
		szEnd ++;
	}

	strncpy( szField, szStart, ( size_t )( szEnd - szStart ) );
	szField[ szEnd - szStart ] = 0;

	return szStart;
}

char svgIsRealNumber( const char *szString )
{
	if( szString==NULL )
		return 0;

	while( *szString!='\0' && ( ( *szString>='0' && *szString<='9' ) || *szString=='.' || *szString=='+' || *szString=='-' ) ) {
		szString ++;
	}

	//	If we reach the end of the string that means it was a real number
	if( *szString=='\0' )
		return 1;

	return 0;
}
