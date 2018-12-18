#ifndef TRANSFERS_H_INCLUDED
#define TRANSFERS_H_INCLUDED

#include "Command.h"

class armov : public Command // 1 пересылка адрес-регистр
{
    virtual void operator() ( Processor &p )
    {
        structCommand code=p.getCodeCommand();
        p.Registers[code.reg]=code.addr;
        p.setIP(p.getIP()+3);
    }
};

class rmmov : public Command // 2 пересылка регистр-память
{
    virtual void operator() ( Processor &p )
    {
        structCommand code=p.getCodeCommand();
        int  valA=p.Registers[code.reg];
        loadNumber(p, valA, code.addr);
        p.setIP(p.getIP()+3);
    }
};
class mrmov : public Command // 3 пересылка память - регистр
{
    virtual void operator() ( Processor &p )
    {
        structCommand code=p.getCodeCommand();
        int valE=getNumber(p, code.addr);
        p.Registers[code.reg]=valE;
        p.setIP(p.getIP()+3);
    }
};

#endif // TRANSFERS_H_INCLUDED
