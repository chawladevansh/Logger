#include "logger.h"
#include<iostream>
#include<stdio.h>
#include<thread>

void logTest(int n)
{
    Logger::Info("Test %d", n);
}

int main()
{
    std::thread threads[10];

    Logger::EnableFileOutput();

    for(int i = 0; i < 10; i++) {
        threads[i] = std::thread(logTest, i);
        threads[i].join();
    }
}