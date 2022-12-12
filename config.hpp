#pragma once

#if defined(_WIN32)
# define IC_SERVER_SYSTEM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
# include "TargetConditionals.h"
# if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#  define IC_SERVER_SYSTEM_IOS
# elif TARGET_OS_MAC
#  define IC_SERVER_SYSTEM_MACOS
# else
#  error This Apple operating system is not supported by IC_SERVER library
# endif
#elif defined(__unix__)
# if defined(__ANDROID__)
#  define IC_SERVER_SYSTEM_ANDROID
# elif defined(__linux__)
#  define IC_SERVER_SYSTEM_LINUX
# elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#  define IC_SERVER_SYSTEM_FREEBSD
# else
#  error This UNIX operating system is not supported by IC_SERVER library
# endif
#else
# error This operating system is not supported by IC_SERVER library
# endif

#if !defined(IC_SERVER_STATIC)
# if defined(IC_SERVER_SYSTEM_WINDOWS)
#  define IC_SERVER_SYMBOL_EXPORT __declspec(dllexport)
#  define IC_SERVER_SYMBOL_IMPORT __declspec(dllimport)
#  ifdef _MSC_VER
#   pragma warning(disable: 4251)
#  endif
# else // Linux, FreeBSD, Mac OS X
#  if __GNUC__ >= 4
#   define IC_SERVER_SYMBOL_EXPORT __attribute__ ((__visibility__ ("default")))
#   define IC_SERVER_SYMBOL_IMPORT __attribute__ ((__visibility__ ("default")))
#  else
#   define IC_SERVER_SYMBOL_EXPORT
#   define IC_SERVER_SYMBOL_IMPORT
#  endif
# endif
#else
# define IC_SERVER_SYMBOL_EXPORT
# define IC_SERVER_SYMBOL_IMPORT
#endif

#if defined(IC_SERVER_BUILDING_THE_LIB)
# define IC_SERVER_API IC_SERVER_SYMBOL_EXPORT
#else
# define IC_SERVER_API IC_SERVER_SYMBOL_IMPORT
#endif
