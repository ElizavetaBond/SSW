#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <cinttypes>
#include <string>
#include <memory>
#include <stack>
#include "Types.h" // польз. типы

const size_t memorySize=65536;
const int countCommand=31;
using Funktors=class Command*;

class Processor
{
    std::vector <Funktors> Commands;    // вектор с функторами и кодами команд;
    structCommand CodeCommand;          // код команды
    structCommand loadCodeCommand();    // загрузка кода команды
    void Trace();                       // трассировка

public:
    psw PSW;  // Ip+флаги
    std::stack <uint16_t> StackCall; // стек для перехода и возврата из подпрограммы
    uint8_t *memory=new uint8_t [memorySize]; // память
    int Registers[8];               // регистры общего назначения
    std::vector <int> BreakPoints;  // точки останова
    Processor();                    // конструктор
    structCommand getCodeCommand(); // возвращает код команды (24 бита)
    void run();                     // запуск процессора
};

#endif // PROCESSOR_H
