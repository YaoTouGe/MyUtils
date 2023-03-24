#include <string>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <atomic>
#include <unistd.h>

const int slaveCount = 4;
int exitcode[slaveCount];

int runSlave(int idx)
{
    char tmp[128];
    snprintf(tmp, 128, "./test-slave %d",idx);
    std::cout << "execute cmd: " << tmp << std::endl; 
    std::system(tmp);
    exitcode[idx] = 0;
    return 0;
}

int main()
{
    for (int i = 0; i < slaveCount; ++i)
    {
        exitcode[i] = -1;
    }
    std::thread threads[slaveCount];
    for (int i = 0; i < slaveCount; ++i)
    {
        threads[i] = std::thread(runSlave, i);
    }

    for (int i = 0; i < slaveCount; ++i)
    {
        threads[i].detach();
    }

    int recordSuccess = 0;
    while (true)
    {
        recordSuccess = 0;

        for (int i = 0; i < slaveCount; i++)
        {
            if (exitcode[i] != -1)
                recordSuccess++;
        }

        if (recordSuccess >= slaveCount)
            break;

        usleep(1000000);
    }

    return 0;
}