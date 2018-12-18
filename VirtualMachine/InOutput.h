#ifndef INOUTPUT_H_INCLUDED
#define INOUTPUT_H_INCLUDED

#include "Command.h"

class in : public Command // 22 ввод целого числа
{
    virtual void operator() ( Processor &p)
    {
        structCommand code=p.getCodeCommand();
        int valC;
        std::cin >> valC;
        loadNumber(p, valC, code.addr);
        p.setIP(p.getIP()+3);
    }
};

class out : public Command // 23 вывод целого числа
{
    virtual void operator() ( Processor &p)
    {
        structCommand code=p.getCodeCommand();
        int valC=getNumber(p, code.addr);
        std::cout << valC;
        p.setIP(p.getIP()+3);
    }
};

class in_f : public Command // 24 ввод дробного числа
{
    virtual void operator() ( Processor &p)
    {
        structCommand code=p.getCodeCommand();
        unionIntFloat valC;
        std::cin >> valC.Float;
        loadNumber(p, valC.Int, code.addr);
        p.setIP(p.getIP()+3);
    }
};

class out_f : public Command // 25 вывод дробного числа
{
    virtual void operator() ( Processor &p )
    {
        structCommand code=p.getCodeCommand();
        unionIntFloat valC;
        valC.Int=getNumber(p, code.addr);
        std::cout << valC.Float;
        p.setIP(p.getIP()+3);
    }
};

class in_s : public Command // 26 ввод строки символов
{
    virtual void operator () (Processor &p)
    {
        structCommand code=p.getCodeCommand();
        std::string s;
        std::cin >> s;
        s+='/';
        for (int i=0; i<s.size(); i++, code.addr++)
            p.memory[code.addr]=s[i];
        p.setIP(p.getIP()+3);
    }
};

class out_s : public Command // 27 вывод строки символов
{
    virtual void operator() (Processor &p)
    {
        structCommand code=p.getCodeCommand();
        std::string s="";
        char c=0;
        for (int i=0; c!='/'; i++, code.addr++)
        {
            s+=c;
            c=p.memory[code.addr];
        }
        std::cout << s;
        p.setIP(p.getIP()+3);
    }
};

#endif // INOUTPUT_H_INCLUDED
