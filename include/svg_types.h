#ifndef __svg_types_h__
#define __svg_types_h__

#ifdef __cplusplus
extern "C" {
#endif

#define SVG_UNUSED( a )		(void)(a);

#if defined( __GNUC__ )
#include <inttypes.h>
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
#else
#error Please define type sizes according to your architecture and compiler.
#endif	//	__GNUC__

//	----- SVG Types -----
//	Types defined by SVG standard (http://www.w3.org/TR/2011/REC-SVG11-20110816).

//	-- SVG length --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/types.html#Length)
typedef enum _svgLengthUnit {
	SVG_LENGTH_UNIT_NONE = 0,
	SVG_LENGTH_UNIT_PX,
	SVG_LENGTH_UNIT_EM,
	SVG_LENGTH_UNIT_EX,
	SVG_LENGTH_UNIT_IN,
	SVG_LENGTH_UNIT_CM,
	SVG_LENGTH_UNIT_MM,
	SVG_LENGTH_UNIT_PT,
	SVG_LENGTH_UNIT_PC,
	SVG_LENGTH_UNIT_PERCENT
} svgLengthUnit ;

typedef struct _svgLength {
	svgLengthUnit tUnit;
	double dValue;
} svgLength ;

void svgStringToLength( const char *szValue, svgLength *ptLength );

//	-- SVG Coordinate --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/types.html#DataTypeCoordinate)
typedef svgLength svgCoordinate;
#define svgStringToCoordinate( _value_, _coordinate_ )	svgStringToLength( ( _value_ ), ( _coordinate_ ) )


//	----- SVG Shapes -----

//	-- Line --
typedef struct _svgLine {
	svgCoordinate tX1;
	svgCoordinate tY1;
	svgCoordinate tX2;
	svgCoordinate tY2;
} svgLine ;

//	-- Rect --
typedef struct _svgRect {
	svgCoordinate tX;
	svgCoordinate tY;
	svgLength tWidth;
	svgLength tHeight;
	svgLength tRadiusX;
	svgLength tRadiusY;
} svgRect ;


#ifdef __cplusplus
}
#endif

#endif	//	__svg_types_h__
