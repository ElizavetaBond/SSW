#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <inttypes.h>
#pragma pack
const int countReg=8;
struct flags // регистр флагов
{
    bool SF:1;
    bool ZF:1;
    bool TF:1;
    bool BF:1;
    flags(): SF(0), ZF(0), TF(0), BF(0) {};
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
    uint8_t byte[4];
};
enum debugSymb // возвращает тип (формат) для отладки
{
    dec,    // десятичное целое, для ввода-вывода
    hexad,  // шестнадцатиричное целое, для ввода-вывода
    fl,     // дробное десятичное, для ввода-вывода
    word,   // слово 4 байта, для работы с памятью
    bytes   // байт, для работы с памятью
};
#endif // TYPES_H_INCLUDED
