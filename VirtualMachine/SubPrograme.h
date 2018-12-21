#ifndef SUBPROGRAME_H_INCLUDED
#define SUBPROGRAME_H_INCLUDED

#include "Command.h"

class halt : public Command // 0 остановка программы
{
    virtual void operator() (Processor &p) {  }
};

class call : public Command // 20 вызов подпрограммы
{
    virtual void operator() ( Processor &p)
    {
        structCommand code=p.getCodeCommand();
        int   nextAddr=p.PSW.IP+3; // запоминаем следующий адрес
        p.StackCall.push(nextAddr);
        p.PSW.IP=code.addr;
    }
};

class ret : public Command // 21 возврат из подпрограммы
{
   virtual void operator() ( Processor &p )
   {
        int addr=p.StackCall.top();
        p.StackCall.pop();
        p.PSW.IP=addr;
   }
};

#endif // SUBPROGRAME_H_INCLUDED
