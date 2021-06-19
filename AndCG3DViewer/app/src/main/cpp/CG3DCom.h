//
// Created by jun on 2021/06/19.
//
#ifndef ANDCG3DVIEWER_CG3DCOM_H
#define ANDCG3DVIEWER_CG3DCOM_H

#ifdef __ANDROID__
#else /*__ANDROID__*/
#include <Windows.h>
#endif /*__ANDROID__*/
#include <string>
#include <sstream>

#ifndef __ANDROID__
#define ANDROID_LOG_DEBUG 0
#define __android_log_print(m, n, fmt, ...)  {CG3DCom::TRACE(fmt, __VA_ARGS__) ; CG3DCom::TRACE("\n") ;}
#endif /*__ANDROID__*/

class CG3DCom {
public:
    template<typename... Args>
    static std::string format(const Args&... args);
#ifndef __ANDROID__
    static void TRACE(LPCSTR pszFormat, ...);
#endif /*__ANDROID__*/

private:
    static void format_internal(std::stringstream &sout);
    template<typename First, typename... Rest>
    static void format_internal(std::stringstream &sout, const First &first, const Rest&... rest);
};


#endif //ANDCG3DVIEWER_CG3DCOM_H
