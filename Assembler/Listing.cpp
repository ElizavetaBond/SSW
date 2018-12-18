#include <iomanip>
#include <fstream>
#include <bitset>
#include "Listing.h"

using namespace std;

string Listing::getErrorMessage(int nError) // возвращает название ошибки
{
    string message;
    switch (nError)
    {
    case Error::noError:        message="Нет ошибок!"; break;
    case Error::illSymbol:      message="Недопустимый символ"; break;
    case Error::illString:      message="Некорректная строка"; break;
    case Error::noColon:        message="Нет двоеточия после метки"; break;
    case Error::illFloat:       message="Недопустимое дробное"; break;
    case Error::illOperator:    message="Неправильный формат оператора"; break;
    case Error::illInteger:     message="Недопустимое целое"; break;
    case Error::illExpression:  message="Некорректное выражение"; break;
    case Error::undLabel:       message="Неопределенная метка"; break;
    case Error::reuLabel:       message="Повторное определение метки"; break;
    case Error::illOperation:   message="Недопустимая операция"; break;
    case Error::illArgument:    message="Неправильное количество аргументов"; break;
    case Error::illRegister:    message="Несоответствующий регистр"; break;
    case Error::signedInt:      message="Недопустимо отрицательное число!"; break;
    case Error::noLabel:        message="Отсутствует метка"; break;
    case Error::noEnd:          message="Отсутствие директивы end"; break;
    }
    return message;
}

void Listing::writeListing()
{
    ofstream file ("Listing.txt");
    string argum;
    file << setw(5) << setfill(' ') << "№" << "|" << setw(7) << "LC" << "|"  << setw (8) << "Binary" << "|" << setw (10) << "Label" << "|"
         << setw(8) << "Code" << "|"   << setw(20) << "Arg" << "|"  << setw(34) << "Error" << "|" << setw(20) << "Comment"  ;
    for (Operator Op: Program)
    {
        file  << "\n" << setw(130) << setfill('-');
        argum="";
        for (Arg arg: Op.Arguments) argum+=arg.stroka+"  "; // собираем аргументы
        file << "\n" << setfill(' ') <<setw(5) << Op.number << "|" << setw(7) << Op.LC << "|";
        if (Op.Binary.size()==0) file << setw(9) << setfill(' ') << "|";
        else file << setw(8) << bitset<8>(Op.Binary[0]) << "|";
        file << setw (10) << Op.Label << "|" << setw(8) << Op.Code << "|"<< setw(20) << argum << "|"
             << setw(34) << getErrorMessage(Op.nError) << "|" << setw(20) << Op.Comment;
        if (Op.Binary.size()>1)
        {
            for (int i=1; i<Op.Binary.size(); i++)
            {
                file << "\n" << setw(6) << setfill(' ') << "|" << setw(8) << "|"
                     << setw(8) << bitset<8>(Op.Binary[i]) << "|"
                     << setw (11) << "|" << setw(9) << "|"  << setw(21) << "|"  << setw(35) << "|";
            }
        }
    }
    auto table=data.getTable();
    file << "\n\n\n" << "ТАБЛИЦА ИМЕН:";
    for (auto elem: table)
        file << "\n" << setw(15) << elem.first << "  =  " << elem.second;
    file.close();
}
