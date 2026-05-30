#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

#define LOG_EVENT 11

void startTracePoint(char* output_file_name);
void endTracePoint(); 
void customTraceLog(int msgType, const char *text, va_list args); 
void writeEvent(int msgType, const char *text); 

#endif