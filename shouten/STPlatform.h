#ifndef _STPlatform_H_
#define _STPlatform_H_

#ifdef SHOUTEN_STATIC_LIB
#	define ShoutenExport
#else
#	ifdef SHOUTEN_NONCLIENT_BUILD
#		define ShoutenExport __declspec( dllexport )
#	else
#		define ShoutenExport __declspec( dllimport )
#	endif
#endif

#pragma warning(disable:4251)

#endif