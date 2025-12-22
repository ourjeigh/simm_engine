#ifndef __CONFIG_H__
#define __CONFIG_H__
#pragma once

// internal defines for common ones
#ifdef _DEBUG
#define DEBUG
#endif 

#ifdef _WIN64
#define PLATFORM_WINDOWS
#endif // _WIN64

#ifdef DEBUG
#define IF_DEBUG(x) x
#define DEBUG_ONLY_MEMBER(x) x
#define DEBUG_ONLY_PARAM_LEFT_COMMA(x) , x
#define DEBUG_ONLY_PARAM_RIGHT_COMMA(x)  x ,
#else
#define IF_DEBUG(x)
#define DEBUG_ONLY_MEMBER(x)
#define DEBUG_ONLY_PARAM_LEFT_COMMA(x)
#define DEBUG_ONLY_PARAM_RIGHT_COMMA(x)
#endif


#endif //__CONFIG_H__