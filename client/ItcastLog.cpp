#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h>
#include "ItcastLog.h"
#include <string>
using namespace std;

const string ITCAST_DEBUG_FILE  = "itderlog.log";
const int ITCAST_MAX_STRING_LEN =  10240;

//Level的名称
const string ICLevelName[] = { "NOLOG", "DEBUG", "INFO", "WARNING", "ERROR" };
ItcastLog::ItcastLog()
{
}

ItcastLog::~ItcastLog()
{
}

int ItcastLog::ITCAST_Error_GetCurTime(char *strTime)
{
    struct tm*	tmTime = NULL;
    size_t		timeLen = 0;
    time_t		tTime = 0;

    tTime = time(NULL);
    tmTime = localtime(&tTime);
    //timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);
    timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);

    return timeLen;
}

int ItcastLog::ITCAST_Error_OpenFile(int *pf)
{
    char	fileName[1024];
    memset(fileName, 0, sizeof(fileName));

#ifdef WIN32
    sprintf(fileName, "d:\\%s", ITCAST_DEBUG_FILE.data());
#else
    sprintf(fileName, "./log/%s", ITCAST_DEBUG_FILE.data());
#endif
#if 0
    *pf = open(fileName, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (*pf < 0)
    {
        return -1;
    }
#endif
    return 0;
}

void ItcastLog::ITCAST_Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
    char str[ITCAST_MAX_STRING_LEN];
    int	 strLen = 0;
    char tmpStr[64];
    int	 tmpStrLen = 0;
    int  pf = 0;

    //初始化
    memset(str, 0, ITCAST_MAX_STRING_LEN);
    memset(tmpStr, 0, 64);

    //加入LOG时间
    tmpStrLen = ITCAST_Error_GetCurTime(tmpStr);
    tmpStrLen = sprintf(str, "[%s] ", tmpStr);
    strLen = tmpStrLen;

    //加入LOG等级
    tmpStrLen = sprintf(str + strLen, "[%s] ", ICLevelName[level].data());
    strLen += tmpStrLen;

    //加入LOG状态
    if (status != 0)
    {
        tmpStrLen = sprintf(str + strLen, "[ERRNO is %d] ", status);
    }
    else
    {
        tmpStrLen = sprintf(str + strLen, "[SUCCESS] ");
    }
    strLen += tmpStrLen;

    //加入LOG信息
    tmpStrLen = vsprintf(str + strLen, fmt, args);
    strLen += tmpStrLen;

    //加入LOG发生文件
    tmpStrLen = sprintf(str + strLen, " [%s]", file);
    strLen += tmpStrLen;

    //加入LOG发生行数
    tmpStrLen = sprintf(str + strLen, " [%d]\n", line);
    strLen += tmpStrLen;

    //打开LOG文件
    if (ITCAST_Error_OpenFile(&pf))
    {
        return;
    }
#if 0
    //写入LOG文件
    write(pf, str, strLen);
    //IC_Log_Error_WriteFile(str);

    //关闭文件
    close(pf);
#endif
    return;
}

void ItcastLog::Log(const char *file, int line, int level, int status, const char *fmt, ...)
{
    va_list args;
    //判断是否需要写LOG
    if (level == NOLOG)
    {
        return;
    }

    //调用核心的写LOG函数
    va_start(args, fmt);
    ITCAST_Error_Core(file, line, level, status, fmt, args);
    va_end(args);

    return;
}
