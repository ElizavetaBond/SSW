#ifndef SUBPROGRAME_H_INCLUDED
#define SUBPROGRAME_H_INCLUDED

#include "Command.h"

class halt : public Command // 0 остановка программы
{
    virtual void operator() (Processor &p) { exit(0); }
};

class call : public Command // 20 вызов подпрограммы
{
    virtual void operator() ( Processor &p)
    {
        structCommand code=p.getCodeCommand();
        int   nextAddr=p.getIP()+3; // запоминаем следующий адрес
        p.StackCall.push(nextAddr);
        p.setIP(code.addr);
    }
};

class ret : public Command // 21 возврат из подпрограммы
{
   virtual void operator() ( Processor &p )
   {
        int addr=p.StackCall.top();
        p.StackCall.pop();
        p.setIP(addr);
   }
};

#endif // SUBPROGRAME_H_INCLUDED
