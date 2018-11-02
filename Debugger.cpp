#include "Debugger.hpp"

Debugger::Debugger(int pid): _pid(pid)
{

}

void Debugger::run()
{
    int wait_stat;
    waitpid(_pid, &wait_stat, 0);
    while(true)
    {
        string line;
        cout << ">> ";
        std::getline(cin, line);
        vector<string> splited = SplitLine(line," ");
        Commands command = ResolveCommand(splited);
        switch(command){
            case Commands::SHOW_REGISTER:
                {
                    Register reg_to_show = GetRegister(splited[1]);
                    std::cout<< "Register " << splited[1] << " value is: 0x" << hex << GetRegisterValue(reg_to_show) << std::endl;
                    break;
                }
            case Commands::SHOW_ALL:
            {
                vector<RegisterEntry> registers = GetAllRegisters();
                for(RegisterEntry reg : registers)
                {
                    std::cout << reg.GetName() << " = 0x" << hex << reg.GetValue() << std::endl;
                }
                break;
            }
            case Commands::SET_REGISTER:
            {
                Register register_to_set = GetRegister(splited[1]);
                int value = stoi(splited[2]);
                SetRegisterValue(register_to_set, value);
                break;
            }
            case Commands::CONTINUE:
            {
                ContinueExecutaion();
                break;
            }
            case Commands::SINGLE_STEP:
            {
                Step();
                break;
            }
            case Commands::SET_BREAKPOINT:
            {
                SetBreakPoint(stoi(splited[1]));
                break;
            }
            case Commands::UNSET_BREAKPOINT:
            {
                UnsetBreakPoint(stoi(splited[1]));
                break;
            }
            case Commands::DOESNT_EXISTS:
            {
                cout << "Command doesnt exists" << endl;
                break;
            }
        }
    }
}


Commands Debugger::ResolveCommand(const vector<string> &parts)
{
    string command_string = parts[0];
    if(boost::equals(command_string, SHOW_REGISTER_COMMAND))
    {
            if(parts.size() != 1)
        {
            return SHOW_REGISTER;
        }
        else
        {
            return SHOW_ALL;
        }
    }
    else if(boost::equals(command_string, SET_REGISTER_COMMAND))
    {
        if(parts.size() == 3)
        {
            return SET_REGISTER;
        }
    }
    else if(boost::equals(command_string, CONTINUE_COMMAND))
    {
        return CONTINUE;
    }
    else if(boost::equals(command_string, SINGLESTEP_COMMAND))
    {
        return SINGLE_STEP;
    }
    else if(boost::equals(command_string, SET_BREAKPOINT_COMMAND) && parts.size() == 2)
    {
        return  SET_BREAKPOINT;
    }
    else if(boost::equals(command_string, UNSET_BREAKPOINT_COMMAND) && parts.size() == 2)
    {
        return UNSET_BREAKPOINT;
    }
    return DOESNT_EXISTS;
}

vector<string> Debugger::SplitLine(string &line, const string delimiter)
{
    boost::algorithm::trim(line);
    vector<string> strs;
    boost::split(strs, line, boost::is_any_of(delimiter));
    vector<string> filterd_vector;
    for(int i = 0; i < strs.size(); i++)
    {
        if(!boost::equals(strs[i], ""))
        {
            filterd_vector.push_back(strs[i]);
        }
    }
    return filterd_vector;
}

Register Debugger::GetRegister(const string &register_name)
{
    if(boost::equals(register_name, REGISTERE_EAX_NAME))
    {
        return EAX;
    }
    else if(boost::equals(register_name, REGISTERE_EBX_NAME))
    {
        return EBX;
    }
    else if(boost::equals(register_name, REGISTERE_ECX_NAME))
    {
        return ECX;
    }
    else if(boost::equals(register_name, REGISTERE_EDX_NAME))
    {
        return EDX;
    }
    else if(boost::equals(register_name, REGISTERE_ESP_NAME))
    {
        return ESP;
    }
    else if(boost::equals(register_name, REGISTERE_EBP_NAME))
    {
        return EBP;
    }
    else if(boost::equals(register_name, REGISTERE_EDI_NAME))
    {
        return EDI;
    }
    else if(boost::equals(register_name, REGISTERE_ESI_NAME))
    {
        return ESI;
    }
    else if(boost::equals(register_name, REGISTERE_EIP_NAME))
    {
        return EIP;
    }
}

vector<RegisterEntry> Debugger::GetAllRegisters()
{
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, _pid, 0, &regs);
    vector<RegisterEntry> return_regs;
    return_regs.push_back(RegisterEntry("eax",regs.eax));
    return_regs.push_back(RegisterEntry("ebx",regs.ebx));
    return_regs.push_back(RegisterEntry("ecx",regs.ecx));
    return_regs.push_back(RegisterEntry("edx",regs.edx));
    return_regs.push_back(RegisterEntry("edi",regs.edi));
    return_regs.push_back(RegisterEntry("esi",regs.esi));
    return_regs.push_back(RegisterEntry("esp",regs.esp));
    return_regs.push_back(RegisterEntry("ebp",regs.ebp));
    return_regs.push_back(RegisterEntry("eip",regs.eip));
    return return_regs;

}
int Debugger::GetRegisterValue(Register reg)
{
    int res = 0;
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, _pid, 0, &regs);
    switch(reg)
    {
        case EAX:
            return regs.eax;
        case EBX:
            return regs.ebx;
        case ECX:
            return regs.ecx;
        case EDX:
            return regs.edx;
        case ESP:
            return regs.esp;
        case EDI:
            return regs.edi;
        case ESI:
            return regs.esi;
        case EBP:
            return regs.ebp;
        case EIP:
            return regs.eip;
    }
}

void Debugger::SetRegisterValue(Register reg, int value)
{
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, _pid, 0, &regs);
    switch(reg)
    {
        case EAX:
        {
            regs.eax = value;
            break;
        }
        case EBX:
        {
            regs.ebx = value;
            break;
        }

        case ECX:
        {
            regs.ecx = value;
            break;
        }

        case EDX:
        {
            regs.edx = value;
            break;
        }

        case ESI:
        {
            regs.esi = value;
            break;
        }

        case EDI:
        {
            regs.edi = value;
            break;
        }

        case EBP:
        {
            regs.ebp = value;
            break;
        }

        case ESP:
        {
            regs.esp = value;
            break;
        }

        case EIP:
        {
            regs.eip = value;
            break;
        }

    }
    ptrace(PTRACE_SETREGS,_pid, 0, &regs);
}

void Debugger::ContinueExecutaion()
{
    ptrace(PTRACE_CONT, _pid, 0,0);
    int wait;
    waitpid(_pid, &wait, 0);
    int eip_value = GetRegisterValue(EIP);
    cout << "Child stopped at: 0x" << hex << eip_value << endl;
    for(BreakPoint bp : _breakpoints)
    {
        if(bp.GetAddress() + 1 == eip_value)
        {
            cout << "In breakpoint number: " << bp.GetIndex() << endl;
            bp.ContinueFromBreakPoint();
        }
    }
}

void Debugger::Step()
{
    ptrace(PTRACE_SINGLESTEP, _pid, 0, 0);
}

void Debugger::SetBreakPoint(int address)
{
    BreakPoint bp(address,_breakpoints.size(), _pid);
    _breakpoints.push_back(bp);
}

void Debugger::UnsetBreakPoint(int number)
{
    vector<BreakPoint> breakpoint_filter;
    for(BreakPoint bp : _breakpoints)
    {
        if(bp.GetIndex() != number)
        {
            breakpoint_filter.push_back(bp);
        }
        else
        {
            bp.ContinueFromBreakPoint();
        }
    }
    _breakpoints = breakpoint_filter;
}
