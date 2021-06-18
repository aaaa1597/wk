#ifdef __ANDROID__
#else /*__ANDROID__*/
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#endif /*__ANDROID__*/

#define ANDROID_LOG_DEBUG 0
#define __android_log_print(m, n, fmt, ...)  {TRACE(fmt, __VA_ARGS__) ; TRACE("\n") ;}

void TRACE(LPCSTR pszFormat, ...);
