#include <iostream>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <vector>
#include <sys/wait.h>
#include <string>
#include <boost/algorithm/string.hpp>
using namespace std;
typedef unsigned char byte;

#define INT_3 0xCC
const string SHOW_REGISTER_COMMAND = "show";
const string SET_REGISTER_COMMAND = "set";
const string CONTINUE_COMMAND = "continue";
const string SET_BREAKPOINT_COMMAND = "bp" ;
const string UNSET_BREAKPOINT_COMMAND = "ubp";
const string SHOW_LIST_BREAKPOINTS_COMMAND = "lbp";
const string SINGLESTEP_COMMAND = "step";
const string REGISTERE_EAX_NAME = "eax";
const string REGISTERE_EBX_NAME = "ebx";
const string REGISTERE_ECX_NAME = "ecx";
const string REGISTERE_EDX_NAME = "edx";
const string REGISTERE_ESI_NAME = "esi";
const string REGISTERE_EDI_NAME = "edi";
const string REGISTERE_EBP_NAME = "ebp";
const string REGISTERE_ESP_NAME = "esp";
const string REGISTERE_EIP_NAME = "eip";
enum Register
{
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    EBP,
    ESP,
    EIP
};

enum Commands
{
    SHOW_ALL,
    SHOW_REGISTER,
    SET_REGISTER,
    CONTINUE,
    SINGLE_STEP,
    SET_BREAKPOINT,
    UNSET_BREAKPOINT,
    LIST_BREAKPOINTS,
    DOESNT_EXISTS
};

class RegisterEntry
{
    private:
        string _name;
        int _value;
    public:
        RegisterEntry(string name, int value): _name(name), _value(value)
        {
        }
        string GetName()
        {
            return _name;
        }
        int GetValue()
        {
            return _value;
        }
};
class Debugger
{
    private:
        class BreakPoint
        {
            private:
            int _address;
            int _number;
            int _cache;
            int _pid;
            public:
            BreakPoint(int address, int number, int pid):_address(address), _number(number), _pid(pid)
            {
                 // need to peek in address and save to cache
                 _cache = ptrace(PTRACE_PEEKTEXT, _pid, (void*)address, 0);
                 int new_data = _cache & 0xffffff00 | INT_3;
                 ptrace(PTRACE_POKETEXT, _pid, (void*)_address, (void*)new_data);
            }
            ~BreakPoint()
            {
                ContinueFromBreakPoint();
            }
            void ContinueFromBreakPoint()
            {
                ptrace(PTRACE_POKETEXT, _pid, (void*)_address, (void*)_cache);
            }
            const int GetAddress()
            {
                return _address;
            }
            const int GetNumber()
            {
                return _number;
            }
            const int GetIndex()
            {
                return _number;
            }
        };

        const int _pid;
        int _program_counter;
        vector<BreakPoint> _breakpoints;
        int GetRegisterValue(Register);

        void ContinueExecutaion();
        void SetRegisterValue(Register, int);
        void SetBreakPoint(int address);
        void UnsetBreakPoint(int address);
        void Step();
        void Help();

        vector<string> SplitLine(string &string_to_split, const string delimiter);
        Commands ResolveCommand(const vector<string> &parts);
        Register GetRegister(const string &register_string);
        vector<RegisterEntry> GetAllRegisters();
    public:
        Debugger(int pid);
        void run();
};
