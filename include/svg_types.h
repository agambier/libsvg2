#ifndef __svg_types_h__
#define __svg_types_h__

#ifdef __cplusplus
extern "C" {
#endif

#define SVG_UNUSED( a )			(void)(a);

#define SVG_ARRAY_SIZE( a )		( sizeof( ( a ) ) / sizeof( ( a )[ 0 ] ) )

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
	float fValue;
} svgLength ;

void svgStringToLength( const char *szValue, svgLength *ptLength );

//	-- SVG Coordinate --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/types.html#DataTypeCoordinate)
typedef svgLength svgCoordinate;
#define svgStringToCoordinate( _value_, _coordinate_ )	svgStringToLength( ( _value_ ), ( _coordinate_ ) )

//	-- Basic point --
typedef struct _svgPoint {
	svgCoordinate tX;
	svgCoordinate tY;
	struct _svgPoint *ptNextPoint;
} svgPoint ;

//	----- SVG Objects -----

//	-- Title --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/struct.html#DescriptionAndTitleElements)
typedef struct _svgTitle {
	char *szText;
} svgTitle ;

//	-- Description --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/struct.html#DescriptionAndTitleElements)
typedef struct _svgDesc {
	char *szText;
} svgDesc ;

//	-- Line --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/shapes.html#LineElement)
typedef struct _svgLine {
	svgCoordinate tX1;
	svgCoordinate tY1;
	svgCoordinate tX2;
	svgCoordinate tY2;
} svgLine ;

//	-- Rect --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/shapes.html#RectElement)
typedef struct _svgRect {
	svgCoordinate tX;
	svgCoordinate tY;
	svgLength tWidth;
	svgLength tHeight;
	svgLength tRadiusX;
	svgLength tRadiusY;
} svgRect ;

//	-- Circle --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/shapes.html#CircleElement)
typedef struct _svgCircle {
	svgCoordinate tX;
	svgCoordinate tY;
	svgLength tRadius;
} svgCircle ;

//	-- Ellipse --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/shapes.html#EllipseElement)
typedef struct _svgEllipse {
	svgCoordinate tX;
	svgCoordinate tY;
	svgLength tRadiusX;
	svgLength tRadiusY;
} svgEllipse ;

//	-- Polyline --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/shapes.html#PolylineElement)
typedef struct _svgPolyline {
	svgPoint tFirstPoint;
} svgPolyline ;

//	-- Path Command --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathData)
typedef enum _svgPathCommandId {
	SVG_PATH_CMD_ID_MOVETO_ABS = 0,
	SVG_PATH_CMD_ID_MOVETO_REL,
	SVG_PATH_CMD_ID_LINETO_ABS,
	SVG_PATH_CMD_ID_LINETO_REL,
	SVG_PATH_CMD_ID_VERTICAL_LINETO_ABS,
	SVG_PATH_CMD_ID_VERTICAL_LINETO_REL,
	SVG_PATH_CMD_ID_HORIZONTAL_LINETO_ABS,
	SVG_PATH_CMD_ID_HORIZONTAL_LINETO_REL,
	SVG_PATH_CMD_ID_CUBIC_CURVETO_ABS,
	SVG_PATH_CMD_ID_CUBIC_CURVETO_REL,
	SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_ABS,
	SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_REL,
	SVG_PATH_CMD_ID_QUADRATIC_CURVETO_ABS,
	SVG_PATH_CMD_ID_QUADRATIC_CURVETO_REL,
	SVG_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO_ABS,
	SVG_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO_REL,
	SVG_PATH_CMD_ID_ARCTO_ABS,
	SVG_PATH_CMD_ID_ARCTO_REL,
	SVG_PATH_CMD_ID_CLOSEPATH,
	SVG_PATH_CMD_ID_END_OF_ENUM
} svgPathCommandId ;

//	-- Commands format --
typedef struct _svgPathCommandFormat {
	char cCommand;
	int8 i8NbrOfArgs;
	svgPathCommandId tId;
} svgPathCommandFormat ;

extern const svgPathCommandFormat g_atPathCommandFormat[];

//	-- Path : MoveTo --
typedef struct _svgPathCommand_MoveTo {
	svgCoordinate tX;
	svgCoordinate tY;
} svgPathCommand_MoveTo ;

//	-- Path : LineTo --
typedef struct _svgPathCommand_Lineto {
	svgCoordinate tX;	/* Unused if SVG_PATH_CMD_ID_VERTICAL_LINETO_xxx */
	svgCoordinate tY;	/* Unused if SVG_PATH_CMD_ID_HORIZONTAL_LINETO_xxx */
} svgPathCommand_Lineto ;

//	-- Path : CubicCurveTo --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathDataCubicBezierCommands)
typedef struct _svgPathCommand_CubicCurveTo {
	svgCoordinate tX;
	svgCoordinate tY;
	svgCoordinate tX1;
	svgCoordinate tY1;
	svgCoordinate tX2;
	svgCoordinate tY2;
} svgPathCommand_CubicCurveTo ;

//	-- Path : SmoothCubicCurveTo --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathDataCubicBezierCommands)
typedef struct _svgPathCommand_SmoothCubicCurveTo {
	svgCoordinate tX;
	svgCoordinate tY;
	svgCoordinate tX2;
	svgCoordinate tY2;
} svgPathCommand_SmoothCubicCurveTo ;

//	-- Path : QuadraticCurveTo --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathDataQuadraticBezierCommands)
typedef struct _svgPathCommand_QuadraticCurveTo {
	svgCoordinate tX;
	svgCoordinate tY;
	svgCoordinate tX1;
	svgCoordinate tY1;
} svgPathCommand_QuadraticCurveTo ;

//	-- Path : SmoothQuadraticCurveTo --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathDataQuadraticBezierCommands)
typedef struct _svgPathCommand_SmoothQuadraticCurveTo {
	svgCoordinate tX;
	svgCoordinate tY;
	svgCoordinate tX1;
	svgCoordinate tY1;
} svgPathCommand_SmoothQuadraticCurveTo ;

//	-- Path : Arc --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathDataEllipticalArcCommands)
typedef struct _svgPathCommand_ArcTo {
	svgCoordinate tX;
	svgCoordinate tY;
	svgCoordinate tRadiusX;
	svgCoordinate tRadiusY;
	svgCoordinate tXAxisAngle;
	svgCoordinate tLargeArcFlag;
	svgCoordinate tSweepFlag;
} svgPathCommand_ArcTo ;

//	-- Path Command --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathData)
typedef struct _svgPathCommand {
	svgPathCommandId tId;

	union {
			svgPathCommand_MoveTo tMoveTo;	/* tId==SVG_PATH_CMD_ID_MOVETO_xxx */
			svgPathCommand_Lineto tLineTo;	/* tId==SVG_PATH_CMD_ID_LINETO_xxx */
			svgPathCommand_CubicCurveTo tCubicCurveTo;	/* tId==SVG_PATH_CMD_ID_CUBIC_CURVETO_xxx */
			svgPathCommand_SmoothCubicCurveTo tSmoothCubicCurveTo;	/* tId==SVG_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO_xxx */
			svgPathCommand_QuadraticCurveTo tQuadraticCurveTo;	/* tId==SVG_PATH_CMD_ID_QUADRATIC_CURVETO_xxx */
			svgPathCommand_SmoothQuadraticCurveTo tSmoothQuadraticCurveTo;	/* tId==SVG_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO_xxx */
			svgPathCommand_ArcTo tArcTo;	/* tId==SVG_PATH_CMD_ID_ARCTO_xxx */
	} tParameters ;

	struct _svgPathCommand *ptNextCommand;
} svgPathCommand ;

//	-- Path --
//	(http://www.w3.org/TR/2011/REC-SVG11-20110816/paths.html#PathElement)
typedef struct _svgPath {
	svgPathCommand *ptFirstCommand;
} svgPath;

#ifdef __cplusplus
}
#endif

#endif	//	__svg_types_h__
