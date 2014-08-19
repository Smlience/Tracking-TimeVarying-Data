#include "log.h"
#include <stdio.h>
#include <windows.h>

#ifdef LINUX

const static char cs_strColor[LOG_LEVEL_FATAL + 1][50] =
{
    "\e[0m",
    "\e[34m\e[1m",//hight blue
    "\e[33m", //yellow
    "\e[31m", //red
    "\e[32m", //green
    "\e[35m"
};

void shellOut()
{
    const char *text = "hello world";
    printf("%s%s\e[0m\n", cs_strColor[LOG_LEVEL_DEBUG], text);
    printf("%s%s\e[0m\n", cs_strColor[LOG_LEVEL_INFO], text);
    printf("%s%s\e[0m\n", cs_strColor[LOG_LEVEL_WARN], text);
    printf("%s%s\e[0m\n", cs_strColor[LOG_LEVEL_ERROR], text);
    printf("%s%s\e[0m\n", cs_strColor[LOG_LEVEL_ALARM], text);
    printf("%s%s\e[0m\n", cs_strColor[LOG_LEVEL_FATAL], text);
}

#endif // LINUX

void LOG(enum ENUM_LOG_LEVEL iLogType, const char *szFmt, ...)
{
    va_list                     argPtr;
    char                        szOut[1024 * 2];
    DWORD                       dwWs;
    HANDLE                      hConsoleOutput;
    CONSOLE_SCREEN_BUFFER_INFO  screenBufferInfo;
    WORD                        wAttributes;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOutput, &screenBufferInfo);

    switch ( iLogType )
    {
    case LOG_LEVEL_INFO:        // 一般信息
        wAttributes = FOREGROUND_INTENSITY;
        break;
    case LOG_LEVEL_WARN:        // 警告信息，黄色
        wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case LOG_LEVEL_ERROR:       // 错误信息，红色
    case LOG_LEVEL_FATAL:
        wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case LOG_LEVEL_ALARM:
        wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    default:
        wAttributes = screenBufferInfo.wAttributes;
    }

    va_start(argPtr, szFmt);
    vsprintf(szOut, szFmt, argPtr);
    va_end(argPtr);

//     char buf[512];
//     sprintf(buf, "[%s:%d(%s)]%s", __FILE__, __LINE__, __FUNCTION__, szOut);
// 	printf("%s", buf);
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szOut, strlen(szOut), &dwWs, NULL);
    SetConsoleTextAttribute(hConsoleOutput, screenBufferInfo.wAttributes);
}

// int main()
// {
//     const char *normal = "hello world";
//     int warning = 0xaec3ff;
//     unsigned error = 0x80001000;
// 
//     LOG(LOG_LEVEL_INFO, "%s\n", normal);
//     LOG(LOG_LEVEL_WARN, "0x%x\n", warning);
//     LOG(LOG_LEVEL_ERROR, "%u\n", error);
//     LOG(LOG_LEVEL_ALARM, "%s\n", normal);
//     printf("---------------------\n");
//     shellOut();
//     return 0;
// }
