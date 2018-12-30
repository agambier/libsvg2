#ifndef __svg_string_h__
#define __svg_string_h__

#ifdef __cplusplus
extern "C" {
#endif

const char* svgGetNextPathField( const char *szData, char *szField, int n );
const char* svgGetNextPointField( const char *szData, char *szField, int n );
char svgIsCommand( char c );
char svgIsRealNumber( const char *szString );

#ifdef __cplusplus
}
#endif


#endif	//	 __svg_string_h__
