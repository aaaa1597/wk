#include <windows.h>
#include <stdio.h>

void TRACE(LPCSTR pszFormat, ...)
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

