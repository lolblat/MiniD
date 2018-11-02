#include <iostream>
#include "Debugger.hpp"
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Need program name to debug it" << std::endl;
        return -1;
    }
    auto program = argv[1];
    auto pid = fork();
    if(pid == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(program,program,0);
    }
    else
    {
        Debugger *debugger = new Debugger(pid);
        debugger->run();
        delete debugger;
    }
}
