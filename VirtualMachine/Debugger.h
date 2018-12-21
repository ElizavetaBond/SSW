#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <map>
#include <queue>
#include "Processor.h"

class Debugger
{
    Processor proc; // процессор
    std::string protocol, filename; // протокол отладки и имя файла для записи протокола
    std::map <std::string, std::function<void(std::queue <std::string> arg, Debugger &debugger)>> cmdTable; // таблица команд отладчика
    void Init(); // инициализирует таблицу команд отладчика
    void inform();
    std::queue <std::string> getCmdArg(std::string str); // разбирает строку на команду и аргументы по пробелам

    //===========команды отладчика============
    // команды управления процессом работы интпретатора

    // выполнить команды до точки прерывания (до конца, если точки не установлены)
    static void start(std::queue <std::string> arg, Debugger &debugger);
    // выполинть без остановок
    static void notStop(std::queue <std::string> arg, Debugger &debugger);
    // выполнять команды пошагово
    static void step(std::queue <std::string> arg, Debugger &debugger);
    // остановка и выход
    static void quit(std::queue <std::string> arg, Debugger &debugger);

    // команды работы с точками прерывания

    // установить точки прерывания по заданным адресам
    static void setBp(std::queue <std::string> arg, Debugger &debugger);
    // удалить точку прерывания по адресу (если нет адреса, то все)
    static void turnBp(std::queue <std::string> arg, Debugger &debugger);

    // команды вывода данных

    // вывести содержимое регистров
    static void printReg(std::queue <std::string> arg, Debugger &debugger);
    // вывести содержимое памяти
    static void printMem(std::queue <std::string> arg, Debugger &debugger);
    // вывести информаию о точках прерывания
    static void printBp(std::queue <std::string> arg, Debugger &debugger);
    // вывести команду процессора
    static void printCmd(std::queue <std::string> arg, Debugger &debugger);
    // вывести информацию о командах отладчика
    static void help(std::queue <std::string> arg, Debugger &debugger);

    // команды присвоения значений данных

    // установить значение регистра
    static void setReg(std::queue <std::string> arg, Debugger &debugger);
    // установить значение памяти
    static void setMem(std::queue <std::string> arg, Debugger &debugger);
    // установить значение IP
    static void setIp(std::queue <std::string> arg, Debugger &debugger);

    class InvalidArgument{}; // для исключений

public:
    Debugger(Processor pr, std::string file);
    void Run(); // функция запускающая отладчик
};

#endif // DEBUGGER_H
