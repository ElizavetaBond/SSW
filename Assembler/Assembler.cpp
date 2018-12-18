#include <iostream>
#include <fstream>
#include <inttypes.h>
#include "Assembler.h"
#include "Commands.h"
#include "CodeCmd.h"
#include "Error.h"
#include "Directives.h"
#include "Listing.h"
#include "Parser.h"

using namespace std;

using nameOp=pair<string, Operation>;

Assembler::Assembler() // конструктор
{
    LC=0;
    initConstant();
    initTOperation();
}

void Assembler::initTOperation()
{
    // ------------------команды процессора--------------------
    TOperation.insert(nameOp("HALT", Operation(new fNOARG(), _HALT, lenghtCmd)));
    TOperation.insert(nameOp("LDA", Operation(new fTWOARG(), _LDA, lenghtCmd)));
    TOperation.insert(nameOp("MOVRM", Operation(new fTWOARG(), _MOVRM, lenghtCmd)));
    TOperation.insert(nameOp("MOVMR", Operation(new fTWOARG(), _MOVMR, lenghtCmd)));
    TOperation.insert(nameOp("ADD", Operation(new fTWOARG(), _ADD, lenghtCmd)));
    TOperation.insert(nameOp("SUB", Operation(new fTWOARG(), _SUB, lenghtCmd)));
    TOperation.insert(nameOp("MUL", Operation(new fTWOARG(), _MUL, lenghtCmd)));
    TOperation.insert(nameOp("DIV", Operation(new fTWOARG(), _DIV, lenghtCmd)));
    TOperation.insert(nameOp("RDIV", Operation(new fTWOARG(), _RDIV, lenghtCmd)));
    TOperation.insert(nameOp("FADD", Operation(new fTWOARG(), _FADD, lenghtCmd)));
    TOperation.insert(nameOp("FSUB", Operation(new fTWOARG(), _FSUB, lenghtCmd)));
    TOperation.insert(nameOp("FMUL", Operation(new fTWOARG(), _FMUL, lenghtCmd)));
    TOperation.insert(nameOp("FDIV", Operation(new fTWOARG(), _FDIV, lenghtCmd)));
    TOperation.insert(nameOp("JMP", Operation(new fDIRTRANS(), _JMP, lenghtCmd)));
    TOperation.insert(nameOp("IJMP", Operation(new fINDIRTRANS(), _IJMP, lenghtCmd)));
    TOperation.insert(nameOp("RJMP", Operation(new fRELTRANS(), _RJMP, lenghtCmd)));
    TOperation.insert(nameOp("JLE", Operation(new fDIRTRANS(), _JLE, lenghtCmd)));
    TOperation.insert(nameOp("IJLE", Operation(new fINDIRTRANS(), _IJLE, lenghtCmd)));
    TOperation.insert(nameOp("RJLE", Operation(new fRELTRANS(), _RJLE, lenghtCmd)));
    TOperation.insert(nameOp("JL", Operation(new fDIRTRANS(), _JL, lenghtCmd)));
    TOperation.insert(nameOp("IJL", Operation(new fINDIRTRANS(), _IJL, lenghtCmd)));
    TOperation.insert(nameOp("RJL", Operation(new fRELTRANS(), _RJL, lenghtCmd)));
    TOperation.insert(nameOp("JE", Operation(new fDIRTRANS(), _JE, lenghtCmd)));
    TOperation.insert(nameOp("IJE", Operation(new fINDIRTRANS(), _IJE, lenghtCmd)));
    TOperation.insert(nameOp("RJE", Operation(new fRELTRANS(), _RJE, lenghtCmd)));
    TOperation.insert(nameOp("JNE", Operation(new fDIRTRANS(), _JNE, lenghtCmd)));
    TOperation.insert(nameOp("IJNE", Operation(new fINDIRTRANS(), _IJNE, lenghtCmd)));
    TOperation.insert(nameOp("RJNE", Operation(new fRELTRANS(), _RJNE, lenghtCmd)));
    TOperation.insert(nameOp("JGE", Operation(new fDIRTRANS(), _JGE, lenghtCmd)));
    TOperation.insert(nameOp("IJGE", Operation(new fINDIRTRANS(), _IJGE, lenghtCmd)));
    TOperation.insert(nameOp("RJGE", Operation(new fRELTRANS(), _RJGE, lenghtCmd)));
    TOperation.insert(nameOp("JG", Operation(new fDIRTRANS(), _JG, lenghtCmd)));
    TOperation.insert(nameOp("IJG", Operation(new fINDIRTRANS(), _IJG, lenghtCmd)));
    TOperation.insert(nameOp("RJG", Operation(new fRELTRANS(), _RJG, lenghtCmd)));
    TOperation.insert(nameOp("CALL", Operation(new fCALL(), _CALL, lenghtCmd)));
    TOperation.insert(nameOp("RET", Operation(new fNOARG(), _RET, lenghtCmd)));
    TOperation.insert(nameOp("IIN", Operation(new fINOUT(), _IIN, lenghtCmd)));
    TOperation.insert(nameOp("IOUT", Operation(new fINOUT(), _IOUT, lenghtCmd)));
    TOperation.insert(nameOp("FIN", Operation(new fINOUT(), _FIN, lenghtCmd)));
    TOperation.insert(nameOp("FOUT", Operation(new fINOUT(), _FOUT, lenghtCmd)));
    TOperation.insert(nameOp("SIN", Operation(new fINOUT(), _SIN, lenghtCmd)));
    TOperation.insert(nameOp("SOUT", Operation(new fINOUT(), _SOUT, lenghtCmd)));
    TOperation.insert(nameOp("NOT", Operation(new fNOT(), _NOT, lenghtCmd)));
    TOperation.insert(nameOp("AND", Operation(new fTWOARG(), _AND, lenghtCmd)));
    TOperation.insert(nameOp("OR", Operation(new fTWOARG(), _OR, lenghtCmd)));
    TOperation.insert(nameOp("XOR", Operation(new fTWOARG(), _XOR, lenghtCmd)));

    //-----------------------директивы-----------------------------
    TOperation.insert(nameOp("int", Operation(new fint(), 0, 0)));
    TOperation.insert(nameOp("float", Operation(new ffloat(), 0, 0)));
    TOperation.insert(nameOp("string", Operation(new fstring(), 0, 0)));
    TOperation.insert(nameOp("membyte", Operation(new fmembyte(), 0, 0)));
    TOperation.insert(nameOp("memint", Operation(new fmemint(), 0, 0)));
    TOperation.insert(nameOp("equ", Operation(new fequ(), 0, 0)));
    TOperation.insert(nameOp("proc", Operation(new fproc(), 0, 0)));
    TOperation.insert(nameOp("endp", Operation(new fendp(), 0, 0)));
    TOperation.insert(nameOp("begin", Operation(new fbegin(), 0, 0)));
    TOperation.insert(nameOp("end", Operation(new fend(), 0, 0)));
    TOperation.insert(nameOp("org", Operation(new forg(), 0, 0)));
}

void Assembler::initConstant() // инициализация констант
{
    nameTable.Insert("INTSIZE", TypeSize::intSize);
    nameTable.Insert("FLOATSIZE", TypeSize::floatSize);
    nameTable.Insert("ADDRSIZE", TypeSize::addrSize);
}

void Assembler::MakeLabel(Operator &op) // занесение метки в таблицу имен
{
    if(!nameTable.IsDefined(op.Label))	// нет метки в таблице
    {
        nameTable.Insert(op.Label, LC);
    }
    else // повторное определение
    {
        op.nError=Error::reuLabel;
        op.work=false;
    }
}

void Assembler::translate(Operator &Last) // обработка оператора на 1 проходе
{
    int Size; // размер оператора
    if(!Last.Code.empty()) // если нет кода, то метка
    {
        auto cmd = TOperation.find(Last.Code); // ищем команду/директиву в таблице
        if(cmd != TOperation.end()) // если команда существует в таблице
        {
            Operation t = (*cmd).second;
            if (!Last.Label.empty()) MakeLabel(Last); // обработка метки
            if (t.Length==0) // если это директива
            {
                Size=t.func->operator()(Last, nameTable); // выполнить обработку директивы
                Last.work=false; // не выполнять на 2 проходе
                LC=Last.LC;
            }
            else Size=t.Length;
            LC+=Size;
        }
        else // ошибочный код операции
        {
            Last.nError = Error::illOperation;
            Last.work = false;
        }
    }
    else // нет кода
    {
        if (!Last.Label.empty()) MakeLabel(Last);
        else // нет метки
        {
            Last.nError=Error::illOperator;
            Last.work=false;
        }
    }
}

void Assembler::pass1(string filename) // первый проход
{
    ifstream file(filename);
    if (!file.is_open()) throw ErrorFileNotFound();
    string String;
    Operator Op;
    int n = 0;
    while(getline(file, String) && Op.Code!="end")
    {
        if (String.size() > 0)
        {
            Op.LC = LC;
            Op.number=n; // номер исходной строки
            Op.clean(); // очистить поля
            Op = parseOperator(String, Op);		//разбор оператора
            if(Op.work && Op.nError==Error::noError) translate(Op);
            Program.push_back(Op);
            n++;
        }
    }
    if (Op.Code!="end") // если нет директивы конца
    {
        Op.nError = Error::noEnd;
        Op.Label  = Op.Comment = Op.Code= "";
        Op.LC = 0;
        Op.number=n;
        Program.push_back(Op);
    }
}

void Assembler::pass2() // второй проход, выполняет команды
{
    Address addr;
    uint8_t reg;
    for (int i=0; i < Program.size(); i++)
    {
        LC = Program[i].LC;
        if(Program[i].work && Program[i].nError==Error::noError) // если это команда (без ошибок)
        {
            auto cmd = TOperation.find(Program[i].Code);
            Operation t = (*cmd).second;
            if (t.Code || Program[i].Code=="HALT")
            {
                int tmp = t.func->operator()(Program[i], nameTable);
                if (Program[i].nError==Error::noError) // если не было ошибок
                {
                    reg=Program[i].Arguments[0].value;
                    addr.word=Program[i].Arguments[1].value;
                    Program[i].Binary.push_back(Prefix::pCmd);
                    Program[i].Binary.push_back(t.Code);
                    Program[i].Binary.push_back(reg);
                    Program[i].Binary.push_back(addr.arr[0]);
                    Program[i].Binary.push_back(addr.arr[1]);
                }
            }
            else
            {
                Program[i].nError = Error::illOperation;
            }
        }
    }
}

void Assembler::writeExecutableFile(string fileBin) // записывает исполняемый файл
{
    ofstream file(fileBin, ios::binary);
    for (Operator Op: Program)
    {
        for (uint8_t byte: Op.Binary)
            file.write((char*)&byte, sizeof(byte));
    }
    file.close();
}

void Assembler::start(string fileText, string fileBin) // запуск ассемблера
{
    try
    {
        pass1(fileText); // 1 проход
        pass2(); // 2 подход
        writeExecutableFile(fileBin); // запись бинарного файла
        Listing listing(Program, nameTable);
        listing.writeListing(); // формирование листинга
    }
    catch (ErrorFileNotFound e)
    {
        cout << "\nОшибка! Файл не найден!";
    }
}

bool Assembler::isError() // наличие ошибки в программе (для интепретатора)
{
    for (Operator Op: Program)
    {
        if (Op.nError!=Error::noError) return false;
    }
    return true;
}

