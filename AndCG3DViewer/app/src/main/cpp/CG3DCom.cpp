//
// Created by jun on 2021/06/19.
//
#include "CG3DCom.h"

namespace CG3D {
    void com::format_internal(std::stringstream& sout)
    {
    }

    template<typename First, typename... Rest>
    void com::format_internal(std::stringstream& sout, const First& first, const Rest&... rest)
    {
        sout << first;
        concat_internal(sout, rest...);
    }


    template<typename... Args>
    std::string com::format(const Args&... args)
    {
        std::stringstream sout;
        concat_internal(sout, args...);
        return sout.str();
    }

#ifndef __ANDROID__
    void com::TRACE(LPCSTR pszFormat, ...)
    {
        va_list	argp;
        int len;
        char* pszBuf;

        va_start(argp, pszFormat);
        len = _vscprintf(pszFormat, argp) + 1;
        if ((pszBuf = (char*)malloc(sizeof(char) * len)) != NULL) {
            vsprintf_s(pszBuf, len, pszFormat, argp);
            va_end(argp);
            OutputDebugStringA(pszBuf);
            free(pszBuf);
        }
        else {
            va_end(argp);
            OutputDebugStringA("ERROR: out of memory.\n");
        }
    }
#endif /* __ANDROID__*/
}
