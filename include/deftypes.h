#ifndef __deftypes_h__
#define __deftypes_h__

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


#endif	//	__deftypes_h__
