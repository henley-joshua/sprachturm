#include "raylib.h"
#include "engine/core/log.h"
#include <time.h> 
#include <fcntl.h> 
#include <unistd.h> 



void startTracePoint(char* output_file_name) {
    int file_desc = open(output_file_name, O_CREAT | O_TRUNC | O_WRONLY); 
    if (dup2(file_desc, STDOUT_FILENO) == -1) {
        printf("FILESAVE_ERROR"); 
    }
}

void endTracePoint() {
    if (dup2(1,STDOUT_FILENO) == -1) {
        printf("FILERETURN_ERROR"); 
    }
}

void writeEvent(int msgType, const char *text)
{
    va_list none; 
    customTraceLog(msgType,text,none); 
}

void customTraceLog(int msgType, const char *text, va_list args)
{
    char timeStr[64] = {0}; 
    time_t now = time(NULL); 
    struct tm *tm_info = localtime(&now); 


    strftime(timeStr, sizeof(timeStr),"%Y-%m-%d %H:%M:%S", tm_info); 
    printf("[%s]",timeStr); 

    switch (msgType)
    {
    case LOG_INFO:
        printf("[INFO] : ");
        break;
    case LOG_ERROR: 
        printf("[ERROR]: ");
        break; 
    case LOG_WARNING: 
        printf("[WARN] : "); 
        break; 
    case LOG_DEBUG: 
        printf("[DEBUG]: "); 
        break;
    case LOG_EVENT:
        printf("[EVENT]: "); 
        break; 
    default: 
        break; 
    }

    vprintf(text, args); 
    printf("\n"); 
}

