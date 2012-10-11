#ifndef __svg_errors_h__
#define __svg_errors_h__

#ifdef __cplusplus
extern "C" {
#endif

#if defined( NDEBUG )
	#define SVG_DEBUG_PRINTF( fmt, args... )	( (void ) 0 )
#else
	#define SVG_DEBUG_PRINTF( fmt, args... )	printf( "%s():%d:" fmt, __FUNCTION__, __LINE__, ##args )
#endif	//	NDEBUG


typedef enum _svgError {
	SVG_ERR_SUCCESS = 0,
	SVG_ERR_INVALID_PARAMETER,
	SVG_ERR_NO_SUCH_FILE,
	SVG_ERR_BAD_FILE_FORMAT,
	SVG_ERR_NOT_ENOUGH_RAM,
} svgError ;

svgError svgSetLastError( svgError, const char *szDesc, ... );
svgError svgGetLastError( void );
char* svgGetLastErrorDescription( void );

#ifdef __cplusplus
}
#endif

#endif	//	__svg_errors_h__
