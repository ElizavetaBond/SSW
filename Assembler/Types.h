#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <inttypes.h>
#include "Error.h"

const int lenghtCmd=3; // размер всех команд=3байта=24бита

union IntFloat
{
    int Int;
    float Float;
    uint8_t arr[4];
};

union Address // адрес 16 бит
{
    uint16_t word;
    uint8_t arr[2];
};

enum TypeArg // типы аргументов
{
    Reg,    // регистр
    Int,    // целое число
    Float,  // дробное число
    String, // строка
    Expr    // выражение
};

struct Arg // аргумент
{
    TypeArg type;   // тип аргумента
    int value;      // значение аргумента
    std::string stroka;  // строка, если тип - строка
    Arg(){};
    Arg (TypeArg t, int v, std::string str): type(t), value(v), stroka(str) {};
};

struct Operator
{
    bool work;				    // подлежит ли обработке
    int LC;				        // смещение - счетчик размещения
    std::vector <uint8_t> Binary;		// двоичная команда
    int number;				    // номер исходной строки
    std::string Label;			// метка оператора
    std::string Code;			// название операции или директивы
    std::vector <Arg> Arguments;	// разобранные аргументы
    std::string Comment;		// комметарий (для листинга)
    int nError;		// ошибка
    void clean() {work=true; Binary.clear(); Label=Code=Comment=""; Arguments.clear(); nError=Error::noError;}
};

struct Operation
{
    class Command *func;    // транслирующая функция
    uint8_t Code;           // код операции, у директив = 0
    uint8_t Length;         // размер команды, у директив = 0
    Operation(class Command *f, uint8_t code, uint8_t L): func(f), Code(code), Length(L){}
};

enum Prefix: char
{
    pBegin='b',
    pEnd='e',
    pString='s',
    pInt='i',
    pFloat='f',
    pCmd='c',
    pAddr='a'
};

enum TypeSize: const int
{
    intSize=4,
    floatSize=4,
    addrSize=2
};

#endif // TYPES_H
