#ifndef _ITCAST_LOG_H_
#define _ITCAST_LOG_H_
#include <cstdarg>
/************************************************************************/
/* 
const char *file：文件名称
int line：文件行号
int level：错误级别
		0 -- 没有日志
		1 -- debug级别
		2 -- info级别
		3 -- warning级别
		4 -- err级别
int status：错误码
const char *fmt：可变参数
*/
/************************************************************************/
// 日志类
class ItcastLog
{
public:
    enum LogLevel{NOLOG, DEBUG, INFO, WARNING, ERROR};
    void Log(const char *file, int line, int level, int status, const char *fmt, ...);
    ItcastLog();
    ~ItcastLog();

private:
    int ITCAST_Error_GetCurTime(char* strTime);
    int ITCAST_Error_OpenFile(int* pf);
    void ITCAST_Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args);
};

#endif
