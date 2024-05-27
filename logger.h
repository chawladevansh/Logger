#pragma once

#include <stdio.h>
#include<mutex>
#include<ctime>

enum LogPriority
{
    TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
};

class Logger 
{
private:
    static LogPriority priority;
    static std::mutex log_mutex;
    static const char* file_path;
    static FILE* file;

public:
    static void SetPriority(LogPriority new_priority)
    {
        priority = new_priority;
    }

    static void EnableFileOutput()
    {
        enable_file_outout();
    }

    static void EnableFileOutput(const char* new_filepath)
    {
        file_path = new_filepath;
        enable_file_outout();
    }

    static void closeFileOutOuput()
    {
        free_file();
    }

    template<typename... Args>
    static void Trace(const char* message, Args... args)
    {
        log("[Trace]\t", TracePriority, message, args...);
    }

    template<typename... Args>
    static void Debug(const char* message, Args... args)
    {
        log("[Debug]\t", DebugPriority, message, args...);
    }

    template<typename... Args>
    static void Info(const char* message, Args... args)
    {
        log("[Info]\t", InfoPriority, message, args...);
    }

    template<typename... Args>
    static void Warn(const char* message, Args... args)
    {
        log("[Warn]\t", WarnPriority, message, args...);
    }

    template<typename... Args>
    static void Error(const char* message, Args... args)
    {
        log("[Error]\t", ErrorPriority, message, args...);
    }

    template<typename... Args>
    static void Critical(const char* message, Args... args)
    {
        log("[Critical]\t", CriticalPriority, message, args...);
    }

private:

    template<typename ...Args>
    static void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args) 
    {
        if (priority <= message_priority)
        {
            std::time_t current_time = std::time(0);
            std::tm* timestamp = std::localtime(&current_time);
            char buffer[80];
            strftime(buffer, 80, "%c", timestamp);

            std::scoped_lock(log_mutex);
            
            printf("%s\t", buffer);
            printf(message_priority_str);
            printf(message, args...);
            printf("\n");

            if (file)
            {
                fprintf(file, message_priority_str);
                fprintf(file, message, args...);
                fprintf(file, "\n");
            }
        }
    }

    static void enable_file_outout()
    {
        if (file != 0)
        {
            fclose(file);
        }

        file = fopen(file_path, "a");

        if (file == 0)
        {
            printf("Failed to open file at %s", file_path);
        }
    }

    static void free_file()
    {
        fclose(file);
        file = 0;
    }
};

LogPriority Logger::priority = InfoPriority;
std::mutex Logger::log_mutex;
FILE* Logger::file = 0;
const char* Logger::file_path = "log.txt";