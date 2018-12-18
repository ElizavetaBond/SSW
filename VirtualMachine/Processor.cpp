#include <iostream>
#include <fstream>
#include <string>
#include <cinttypes>
#include "Processor.h"
#include "InOutput.h"
#include "SubPrograme.h"
#include "Transfers.h"
#include "Transition.h"
#include "Integer.h"
#include "Float.h"
#include "BitwiseOperation.h"
#include "CodeCmd.h"

using namespace std;

Processor::Processor () // инициализация
{
    StartAddress=0;
    setIP(0);
    Commands.resize(countCommand);
    Commands[_halt]=new halt();
    Commands[_armov]=new armov();
    Commands[_rmmov]=new rmmov();
    Commands[_mrmov]=new mrmov();
    Commands[_add]=new add();
    Commands[_sub]=new sub();
    Commands[_mul]=new mul();
    Commands[_div]=new divs();
    Commands[_rdiv]=new rdiv();
    Commands[_add_f]=new add_f();
    Commands[_sub_f]=new sub_f();
    Commands[_mul_f]=new mul_f();
    Commands[_div_f]=new div_f();
    Commands[_jump]=new jump();
    Commands[_jle]=new jle();
    Commands[_jl]=new jl();
    Commands[_je]=new je();
    Commands[_jne]=new jne();
    Commands[_jge]=new jge();
    Commands[_jg]=new jg();
    Commands[_call]=new call();
    Commands[_ret]=new ret();
    Commands[_in]=new in();
    Commands[_out]=new out();
    Commands[_in_f]=new in_f();
    Commands[_out_f]=new out_f();
    Commands[_in_s]=new in_s();
    Commands[_out_s]=new out_s();
    Commands[_not]=new Not();
    Commands[_and]=new And();
    Commands[_or]=new Or();
    Commands[_xor]=new Xor();
};

void Processor::setIP(int ip) // установить ip
{
    PSW.IP=ip;
}

void Processor::setStartAddr(int addr) // установить стартовый адрес
{
    StartAddress=addr;
}

int Processor::getIP() // получить ip
{
    return PSW.IP;
}

void Processor::setFlags(flags f) // установить регистр флагов
{
    PSW.Flags=f;
}

flags Processor::getFlags() // получить решистр флагов
{
    return PSW.Flags;
}

structCommand Processor::getCodeCommand() // получить код команды
{
    return CodeCommand;
}

structCommand Processor::loadCodeCommand() // считать код команды
{
    unionCommand Union;
    for (int i=0; i<3; i++)
        Union.byte[i]=memory[PSW.IP+i];
    return Union.code;
}

void Processor::run() // главная функция, запускающая процесс
{
    cout << "\nStart address=" << StartAddress;
    CodeCommand.cop=1;
    setIP(StartAddress);
    while (CodeCommand.cop!=0)
    {
        CodeCommand=loadCodeCommand();
        Commands[CodeCommand.cop]->operator()(*this);
    }
};
