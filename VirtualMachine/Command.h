#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <cmath>
#include "Types.h"

class Command // базовый класс command с перегруженной операцией operator()
{
public:
    virtual void operator() ( Processor &p) = 0;

    void loadNumber (Processor &p, int number, int address) // загрузка числа в память
    {
        unionNumberChar Union;
        Union.Int=number;
        for (int i=0; i<4; i++, address++)
            p.memory[address]=Union.byte[i];
    };

    int getNumber (Processor p, int address) // получение числа из памяти
    {
        unionNumberChar Union;
        for (int i=0; i<4; i++, address++)
            Union.byte[i]=p.memory[address];
        return Union.Int;
    };
};

#endif // COMMAND_H_INCLUDED
