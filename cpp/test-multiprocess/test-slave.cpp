#include <iostream>
#include <thread>
#include <unistd.h>

int main(int argc, char** argv)
{
    int id = atoi(argv[1]);
    std::cout << "process " << id << " started" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        std::cout << id << ' ' << i << std::endl;
        usleep(1000000);
    }
}