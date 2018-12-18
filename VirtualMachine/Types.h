#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <inttypes.h>
#pragma pack

struct flags // регистр флагов
{
    bool SF:1;
    bool ZF:1;
    flags(): SF(0), ZF(0) {};
};
struct structCommand // код команды = 24 бита
{
    unsigned int cop: 6;
    unsigned int reg: 3;
    unsigned int addr: 15;
};
struct psw // IP + FLAGS
{
    uint16_t IP: 16;
    flags Flags;
};
union unionCommand // код команды-массив из 3 байт
{
    uint8_t byte[3];
    structCommand code;
};
union unionIntFloat // целое-дробное
{
    int Int;
    float Float;
};
union unionNumberChar // целое -  массив из 4 байт
{
    uint8_t byte[4];
    int Int;
};

#endif // TYPES_H_INCLUDED
