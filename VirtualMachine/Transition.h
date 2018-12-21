#ifndef TRANSITION_H_INCLUDED
#define TRANSITION_H_INCLUDED

#include "Command.h"

class Transition: public Command // базовый класс для команд перехода
{
public:
    virtual bool check (flags fl)=0; // условие перехода
    virtual void operator () (Processor &p) // действия для всех команд
    {
        structCommand code=p.getCodeCommand();
        flags fl=p.PSW.Flags;
        if ( check(fl) )
        {
            switch (code.reg)
            {
            case 2:
                p.PSW.IP=p.Registers[code.addr]; // косвенный переход, регистр в поле адреса
                break;
            case 3:
                p.PSW.IP+=code.addr; // относительный переход
                break;
            default:
                p.PSW.IP=code.addr; // прямой переход
                break;
            }
        } else p.PSW.IP+=3;
    }
};

class jump : public Transition // 13 безусловный переход
{
    virtual bool check(flags fl) { return true; }
};

class jle : public Transition // 14 если меньше или равно
{
    virtual bool check(flags fl) { return (fl.ZF|| fl.SF ); }
};

class jl : public Transition // 15 если меньше
{
    virtual bool check(flags fl) { return (fl.SF && !fl.ZF); }
};

class je : public Transition // 16 если равно
{
    virtual bool check(flags fl) { return fl.ZF; }
};

class jne : public Transition //17 если не равно
{
    virtual bool check(flags fl) { return (!fl.ZF); }
};

class jge : public Transition // 18  если больши или равно
{
    virtual bool check (flags fl) { return ( !fl.SF || fl.ZF); }
};

class jg : public Transition // 19 если больше
{
    virtual bool check (flags fl) { return ( !fl.ZF && !fl.SF ); }
};

#endif // TRANSITION_H_INCLUDED
