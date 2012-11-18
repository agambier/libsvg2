#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svg_string.h"
#include "svg_types.h"

const char* svgSkipWS( const char* szStart, const char *szEOF );
const char* svgSkipDigits( const char* szStart, const char* szEOF );


const char* svgGetNextPathField( const char *szData, char *szField, int n )
{
	const char *szStart = NULL, *szEnd = NULL, *szEOF = szData + n - 1;

	if( szData==NULL || szField==NULL )
		return NULL;

	szField[ 0 ] = '\0';

	//	Search for the start of the field
	szStart = szData;
	szStart = svgSkipWS(szStart, szEOF);
	if( szStart==szEOF || *szStart=='\0' )
		return NULL;
	
	if ( *szStart==',' ) {
		szStart ++;
		szStart = svgSkipWS(szStart, szEOF);
		if( szStart==szEOF || *szStart=='\0' )
			return NULL;
	} else if ( svgIsCommand( *szStart ) ) {
		szField[ 0 ] = *szStart;
		szField[ 1 ] = 0;
		return szStart;
	}

	//	Search for the end
	szEnd = szStart + 1;
	if (szEnd != szEOF && ( *szEnd=='-' || *szEnd =='+' ) )
		szEnd ++;
	szEnd = svgSkipDigits( szEnd, szEOF );
	if (szEnd != szEOF && *szEnd ==  '.') {
		szEnd ++;
		szEnd = svgSkipDigits( szEnd, szEOF );
	}
	// TODO: handle exponent
	
	assert( szEnd - szStart < n );
	strncpy( szField, szStart, ( size_t )( szEnd - szStart ) );
	szField[ szEnd - szStart ] = 0;

	return szStart;
}

const char* svgGetNextPointField( const char *szData, char *szField, int n )
{
	const char *szStart = NULL, *szEnd = NULL, *szEOF = szData + n - 1;
	
	if( szData==NULL || szField==NULL )
		return NULL;
	
	szField[ 0 ] = '\0';
	
	//	Search for the start of the field
	szStart = szData;
	while( *szStart!='\0' && isspace( *szStart ) ) {
		szStart ++;
	}
	
	if( *szStart=='\0' )
		return NULL;
	
	//	Search for the end
	szEnd = szStart + 1;
	if (szEnd != szEOF && ( *szEnd=='-' || *szEnd =='+' ) )
		szEnd ++;
	szEnd = svgSkipDigits( szEnd, szEOF );
	if (szEnd != szEOF && *szEnd ==  '.') {
		szEnd ++;
		szEnd = svgSkipDigits( szEnd, szEOF );
	}
	
	assert( szEnd - szStart < n );
	strncpy( szField, szStart, ( size_t )( szEnd - szStart ) );
	szField[ szEnd - szStart ] = 0;
	
	return szStart;
}

char svgIsCommand( char c )
{
	int i;
	for( i = 0; g_atPathCommandFormat[ i ].cCommand!='\0'; i ++ ) {
		if( g_atPathCommandFormat[ i].cCommand==c )
			return 1;
	}
	return 0;
}

char svgIsRealNumber( const char *szString )
{
	if( szString==NULL )
		return 0;
	
	if ( *szString=='-' || *szString =='+' )
		szString ++;
	
	szString = svgSkipDigits( szString, NULL );
	if (*szString ==  '.') {
		szString ++;
		szString = svgSkipDigits( szString, NULL );
	}

	//	If we reach the end of the string that means it was a real number
	if( *szString=='\0' )
		return 1;

	return 0;
}

const char* svgSkipWS( const char* szStart, const char *szEOF )
{
	const char* szEnd = szStart;
	while( szEnd!=szEOF && *szEnd!='\0' && isspace( *szEnd ) ) {
		szEnd ++;
	}
	return szEnd;
}

const char* svgSkipDigits( const char* szStart, const char* szEOF )
{
	const char* szEnd = szStart;
	while( szEnd!=szEOF && *szEnd!='\0' && isdigit( *szEnd ) ) {
		szEnd ++;
	}
	return szEnd;
}
