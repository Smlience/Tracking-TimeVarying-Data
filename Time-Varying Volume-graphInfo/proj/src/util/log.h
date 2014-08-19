#ifndef LOG_H
#define LOG_H

//! LOG Level
enum ENUM_LOG_LEVEL
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_ALARM,
    LOG_LEVEL_FATAL,
};

extern void LOG(enum ENUM_LOG_LEVEL iLogType, const char *szFmt, ...);
// extern void LINFO(const char *szFmt, ...);
// extern void LWARN(const char *szFmt, ...);
// extern void LERROR(const char *szFmt, ...);
// extern void LALARM(const char *szFmt, ...);
// extern void LFATAL(const char *szFmt, ...);
// extern void LDEBUG(const char *szFmt, ...);

#endif // LOG_H
