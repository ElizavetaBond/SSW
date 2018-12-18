#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <map>
#include "Types.h"
#include "Error.h"
#include "Data.h"

class Assembler
{
    int LC;                     // смещение
    std::vector <Operator> Program;  // программа
    std::map <std::string, Operation> TOperation;   // таблица операций
    Data nameTable;         // таблица имен
    void initConstant();    // инициализация констант
    void initTOperation();  // инициализация таблицы операций
    void translate(Operator &Last); // трансляция одного оператора
    void MakeLabel(Operator &op);   // обработка меток
    void pass1(std::string filename); // 1 проход
    void pass2();                   // 2 проход
    void writeExecutableFile(std::string fileBin); // запист бинарного исполняемого файла
public:
    Assembler();
    bool isError(); // наличие ошибок в программе
    void start(std::string fileText, std::string fileBin); // запускает трансляцию
};
#endif // ASSEMBLER_H
