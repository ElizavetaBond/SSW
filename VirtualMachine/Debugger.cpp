#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <conio.h>
#include "Debugger.h"
#include "Types.h"

using namespace std;

Debugger::Debugger(Processor pr, std::string file) // конструктор
{
    proc=pr;
    filename=file;
    protocol="Program debugging Protocol";
    Init();
}

int getTypeSymb (string str) // возвращает тип символа
{
    if (str=="d") return debugSymb::dec;
    else if (str=="x") return debugSymb::hexad;
    else if (str=="f") return debugSymb::fl;
    else if (str=="w") return debugSymb::word;
    else if (str=="b") return debugSymb::bytes;
    else return -1;
}

void Debugger::Init() // инициализация команд отладчика
{
    cmdTable.insert(make_pair("start", start));
    cmdTable.insert(make_pair("notStop", notStop));
    cmdTable.insert(make_pair("step", step));
    cmdTable.insert(make_pair("quit", quit));
    cmdTable.insert(make_pair("setBp", setBp));
    cmdTable.insert(make_pair("turnBp", turnBp));
    cmdTable.insert(make_pair("printReg", printReg));
    cmdTable.insert(make_pair("printMem", printMem));
    cmdTable.insert(make_pair("printBp", printBp));
    cmdTable.insert(make_pair("printCmd", printCmd));
    cmdTable.insert(make_pair("help", help));
    cmdTable.insert(make_pair("setReg", setReg));
    cmdTable.insert(make_pair("setMem", setMem));
    cmdTable.insert(make_pair("setIp", setIp));
}

void Debugger::inform()
{
    cout << "\nWelcome to the virtual machine debugger! Start address="
         << proc.PSW.IP;
}

void Debugger::start(queue <string> arg, Debugger &debugger) // до точки останова или до конца
{
    string String="";
    if (!debugger.proc.BreakPoints.empty()) debugger.proc.PSW.Flags.BF=1;
    debugger.proc.PSW.Flags.TF=0;
    try
    {
        debugger.proc.run();
        String+="\nThe program is executed up to the end!  IP= " + to_string(debugger.proc.PSW.IP);
    }
    catch (...)
    {
         String+="\nThe program is executed up to the breakpoint! IP= " + to_string(debugger.proc.PSW.IP);
    }
    cout << String;
    debugger.protocol+=String;
}

void Debugger::notStop(queue <string> arg, Debugger &debugger) // без остановок
{
    if (!arg.empty()) throw InvalidArgument();
    debugger.proc.PSW.Flags.BF=0;
    debugger.proc.PSW.Flags.TF=0;
    debugger.proc.run();
}

void Debugger::step(queue <string> arg, Debugger &debugger) // пошагово
{
    debugger.proc.PSW.Flags.BF=0;
    debugger.proc.PSW.Flags.TF=1;
    debugger.proc.run();
}

void Debugger::quit(queue <string> arg, Debugger &debugger) // завершить и выйти
{
    ofstream file(debugger.filename); // записать протокол
    string s="\nThe fixing is completed!\n";
    cout << s;
    debugger.protocol+=s;
    file << debugger.protocol;
    getch();
}

void Debugger::setBp(queue <string> arg, Debugger &debugger) // установить точки останова
{
    vector <int> temp;
    int value;
    if (arg.empty()) throw InvalidArgument();
    while (!arg.empty())
    {
        value=stoi(arg.front());
        arg.pop();
        if (value<0) throw InvalidArgument();
        temp.push_back(value); // сначала во временный, т к в цикле может быть иисключение
    }
    for (auto x: temp) debugger.proc.BreakPoints.push_back(x);
    debugger.proc.PSW.Flags.BF=1;
    string s="\nCommand successful!";
    cout << s;
    debugger.protocol+=s;
}

void Debugger::turnBp(queue <string> arg, Debugger &debugger) // отключить точки останова
{
    if (arg.size()==0) debugger.proc.BreakPoints.clear();
    else if (arg.size()==1)
    {
        int value=stoi(arg.front()), i;
        arg.pop();
        for (i=0; i<debugger.proc.BreakPoints.size()
            && debugger.proc.BreakPoints[i]!=value; i++);
        if (i==debugger.proc.BreakPoints.size()) throw InvalidArgument();
        else debugger.proc.BreakPoints.erase(debugger.proc.BreakPoints.begin()+i);
    }
    else throw InvalidArgument();
    if (debugger.proc.BreakPoints.size()==0) debugger.proc.PSW.Flags.BF=0;
    string s="\nCommand successful!";
    cout << s;
    debugger.protocol+=s;
}

void Debugger::printReg(queue <string> arg, Debugger &debugger) // вывод содержимого регистров
{
    string s_reg, type;
    string String=""; // строка для вывода и записи
    int code, lenght=0;
    if (arg.empty())  throw InvalidArgument(); // отсутствуют аргументы
    s_reg=arg.front(); // вытащить код регистра
    arg.pop();
    if (s_reg.size()==2 && s_reg[0]=='R' && isdigit(s_reg[1]))
    {
        code=stoi(s_reg.substr(1)); // получить код регистра
        if (code>countReg-1) throw InvalidArgument();
    }
    else  throw InvalidArgument();
    if (arg.empty()) { type="d"; lenght=1; } // формат: printReg reg
    else if (arg.size()==1) {type=arg.front(); arg.pop(); lenght=1; } // формат: printReg reg t
    else if (arg.size()==2) // формат: printReg reg t N
    {
        type=arg.front();
        arg.pop();
        lenght=stoi(arg.front());
        arg.pop();
        if (lenght<0 || lenght>(countReg-code-1)) throw InvalidArgument();
    }
    else throw InvalidArgument(); // неверное количество аргументов
    for (int i=code; i<code+lenght; i++) // вывод
    {
        switch (getTypeSymb(type))
        {
        case debugSymb::dec: // десятичное целое
            String+= " " + to_string(debugger.proc.Registers[i]);
            break;
        case debugSymb::hexad: // шестнадцатиричное целое
            String+=" " + to_string(debugger.proc.Registers[i]);
            cout << " " << hex << debugger.proc.Registers[i];
            break;
        case debugSymb::fl: // десятичное дробное
            unionIntFloat numbr;
            numbr.Int=debugger.proc.Registers[i];
            String+= " " + to_string(numbr.Float);
            break;
        default:
            throw InvalidArgument(); // неверный формат
        }
    }
    if (getTypeSymb(type)!=debugSymb::hexad) cout << String;
    String+="\nCommand successful!";
    cout << "\nCommand successful!";
    debugger.protocol+=String;
}

void Debugger::printMem(queue <string> arg, Debugger &debugger) // вывод содержимого адреса
{
    string String=""; // строка для вывода и записи
    string s_lenght, type;
    char c; // ддля вывода символа
    int addr, counts=0;
    unionIntFloat value;
    if (arg.size()<2 || arg.size()>4)  throw InvalidArgument(); // неверное количество
    s_lenght=arg.front(); // вытащить тип данных
    arg.pop();
    addr=stoi(arg.front()); // вытащить адрес
    arg.pop();
    if (arg.empty()) { type="d"; counts=1; } // формат: printMem L addr
    else if (arg.size()==1) {type=arg.front(); arg.pop(); counts=1; } // формат: printMem L addr t
    else if (arg.size()==2) // формат: printMem L addr t N
    {
        type=arg.front();
        arg.pop();
        counts=stoi(arg.front());
        arg.pop();
        if (counts<0) throw InvalidArgument();
    }
    else throw InvalidArgument(); // неверное количество аргументов
    for (int i=0; i<counts; i++) // вывод
    {
        int typeSymb=getTypeSymb(s_lenght);
        if (typeSymb==debugSymb::bytes){ value.Int=debugger.proc.memory[addr]; addr++; }// считать 1 байт
        else if (typeSymb==debugSymb::word) // считать 4 байта
        {
            for (int j=0; j<4; j++)
                value.byte[j]=debugger.proc.memory[addr+j];
            addr+=4;
        }
        else throw InvalidArgument();
        switch (getTypeSymb(type)) // формат вывода
        {
        case debugSymb::dec: // десятичное целое
            String+= " " +to_string(value.Int);
            break;
        case debugSymb::hexad: // шестнадцатиричное целое
            String+=  " " + to_string( value.Int);
            cout << hex << value.Int;
            break;
        case debugSymb::fl: // десятичное дробное
            String+= " " + to_string( value.Float);
            break;
        default:
            throw InvalidArgument(); // неверный формат
        }
    }
    if (getTypeSymb(type)!=debugSymb::hexad) cout << String;
    String+="\nCommand successful!";
    cout << "\nCommand successful!";
    debugger.protocol+=String;
}

void Debugger::printBp(queue <string> arg, Debugger &debugger) // вывод точке останова
{
    string String=""; // строка для вывода и записи
    if (!arg.empty()) throw InvalidArgument();
    String+= "\nBreakpoints set at the following addresses: ";
    for (auto x:debugger.proc.BreakPoints) String+= "  " + to_string(x);
    String+="\nCommand successful!";
    cout << String;
    debugger.protocol+=String;
}

void Debugger::printCmd(queue <string> arg, Debugger &debugger) // вывод текущей команды на экран
{
    string String="";
    structCommand com=debugger.proc.getCodeCommand();
    String+= " " + to_string(com.cop) + " " + to_string(com.reg) + " " + to_string(com.addr);
    String+="\nCommand successful!";
    cout << String;
    debugger.protocol+=String;
}

void Debugger::help(queue <string> arg, Debugger &debugger) // вывод информации о командах отладчика
{
    string String;
    String = "Debugger commands:"
          "\nstart - The beginning of the execution until a breakpoint or until the end, if the breakpoint is not set"
          "\nresume - Continue until a breakpoint or until the end, if the breakpoint is not set"
          "\ngo [addr] - Run to specified address; if no address exists, run 1 command"
          "\nstep - Run the program step by step"
          "\nquit - Complete and exit"
          "\nsetBp addr… - Set breakpoints to the following addresses"
          "\nturnBp [addr] - To disable a breakpoint at the address if no address disable all"
          "\nprintReg reg [t] [N] - Print the register value, N indicates the number of displayed values"
          "\nprintMem L addr [t] [N] - Print the memory value, N indicates the number of output values"
          "\nprintBp - Display information about breakpoints"
          "\nprintCmd - Display information about the current command"
          "\nhelp - Information about debugger commands"
          "\nsetReg reg value [t] - Set the value of the register"
          "\nsetMem L addr value [t] - Set the value of memory";
    String+="\nCommand successful!";
    cout << String;
    debugger.protocol+=String;
}

void Debugger::setReg(queue <string> arg, Debugger &debugger) // установить значение в регистр
{
    size_t *error; // для перевода в число
    string s_reg, type, s_value;
    int code;
    unionIntFloat value;
    if (arg.size()<2 || arg.size()>3)  throw InvalidArgument(); // неверный размер
    s_reg=arg.front(); // вытащить код регистра
    arg.pop();
    if (s_reg.size()==2 && s_reg[0]=='R' && isdigit(s_reg[1]))
    {
        code=stoi(s_reg.substr(1)); // получить код регистра
        if (code>countReg-1) throw InvalidArgument();
    }
    else  throw InvalidArgument();
    s_value=arg.front(); // получаем значение регистра
    arg.pop();
    if (arg.empty()) { type="d";} // формат: printReg reg value
    else  {type=arg.front(); arg.pop(); } // формат: printReg reg value t
    switch (getTypeSymb(type))
    {
    case debugSymb::dec: // десятичное целое
        value.Int=stoi(s_value);
        break;
    case debugSymb::hexad: // шестнадцатиричное целое
        value.Int=stoi(s_value, error, 16);
        break;
    case debugSymb::fl: // десятичное дробное
        value.Float=stof(s_value);
        break;
    default:
        throw InvalidArgument(); // неверный формат
    }
    debugger.proc.Registers[code]=value.Int;
    string String="\nCommand successful!";
    cout << String;
    debugger.protocol+=String;
}

void Debugger::setMem(queue <string> arg, Debugger &debugger) // записать значение в память
{
    size_t *error; // для перевода в число
    string s_lenght, type, s_value;
    char c; // ддля вывода символа
    int addr;
    unionIntFloat value;
    if (arg.size()<3 || arg.size()>4)  throw InvalidArgument(); // неверное количество
    s_lenght=arg.front(); // вытащить тип данных
    arg.pop();
    addr=stoi(arg.front()); // вытащить адрес
    arg.pop();
    if (addr<0) throw InvalidArgument();
    s_value=arg.front(); // вытащить значение
    arg.pop();
    if (arg.empty()) { type="d"; } // формат: printMem L addr value
    else {type=arg.front(); arg.pop();} // формат: printmMem L addr value t
    switch (getTypeSymb(type)) // в зависимости от формата введненого значения
    {
    case debugSymb::dec: // десятичное целое
        value.Int=stoi(s_value);
        break;
    case debugSymb::hexad: // шестнадцатиричное целое
        value.Int=stoi(s_value, error, 16);
        break;
    case debugSymb::fl: // десятичное дробное
        value.Float=stof(s_value);
        break;
    default:
        throw InvalidArgument(); // неверный формат
    }
    int typeSymb=getTypeSymb(s_lenght); // формат знfчения для записи в памяти(4 байта, 1 байт)
    if (typeSymb==debugSymb::bytes){ debugger.proc.memory[addr]=value.byte[0];}// считать 1 байт
    else if (typeSymb==debugSymb::word) // считать 4 байта
    {
        for (int j=0; j<4; j++)
            debugger.proc.memory[addr+j]=value.byte[j];
    }
    else throw InvalidArgument();
    string String="\nCommand successful!";
    cout << String;
    debugger.protocol+=String;
}

void Debugger::setIp(queue <string> arg, Debugger &debugger) // установить значение Ip
{
    size_t *error; // для перевода числа
    if (arg.size()<1 || arg.size()>2) throw InvalidArgument();
    string s_value=arg.front(); // извлекли значение
    arg.pop();
    string type;
    int value;
    if (arg.empty()) type="d";
    else {type=arg.front(); arg.pop();} // извлекаем формат (10е или 16е)
    switch (getTypeSymb(type))
    {
    case debugSymb::dec:
        value=stoi(s_value);
        break;
    case debugSymb::hexad:
        value=stoi(s_value, error, 16);
        break;
    default:
        throw InvalidArgument();
    }
    debugger.proc.PSW.IP=value;
    string String="\nCommand successful!";
    cout << String;
    debugger.protocol+=String;
}

void Debugger::Run() // основной цикл работы отладчика
{
    string str;
    string cmd="";
    queue <string> arg;
    inform();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int n=0;
    while (cmd!="quit")
    {
        n++;
        protocol+="\n" + to_string(n) + ": ";
        cout << "\n\nEnter the debugger command: ";
        getline(cin, str);
        protocol+=str;
        arg=getCmdArg(str);     // разбить строку на команду и аргументы
        if (!arg.empty())
        {
            cmd=arg.front();        // получить команду
            arg.pop();              // удалить коману из очереди
            auto it=cmdTable.find(cmd);  // найти команду в таблице коанд отладчика
            if (it==cmdTable.end())
            {
                string s="\nThis debugger command does not exist!";;
                cout << s;
                protocol+=s;
            }
            else // если команда найдена
            {
                try { (*it).second(arg, (*this)); }
                catch(...)
                {
                    string s="\nError command! Invalid type, value or number of argument!";
                    cout << s;
                    protocol+=s;
                }
            }
        }
    }
}

queue <string> Debugger::getCmdArg(string str) // делит строку по пробелам
{
    queue <string> res;
    string temp="";
    for (char ch: str)
    {
        if (isspace(ch) && temp.size()!=0)
        {
            res.push(temp);
            temp="";
        }
        else temp+=ch;
    }
    if (temp.size()!=0) res.push(temp);
    return res;
}
