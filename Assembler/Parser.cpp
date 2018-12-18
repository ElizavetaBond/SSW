#include <iostream>
#include <ctype.h>
#include <cstring>
#include "Data.h"
#include "Parser.h"

using namespace std;

string deleteBlanks (string str) // удаляем пробелы
{
    string res="";
    for (char c: str)
    {
        if (!isspace(c)) res+=c;
    }
    return res;
}

vector <string> getArgs(string str) // разделяем строку с аргументами на части по запятым
{
    vector <string> res;
    string s="";
    for (char ch: str)
    {
        if (ch==',')
        {
            res.push_back(s);
            s="";
        } else s+=ch;
    }
    if (s.size()!=0) res.push_back(s);
    return res;
}

bool isRegister(string str, int &value) // если регистр - true, код регистра в value
{
    size_t *error;
    if (str[0]!='R') return false;
    try
    {
        value=stoi(str.substr(1), error); // получаем код регистра
        if (value>7 || error!=nullptr) return false;
    }
    catch (std::invalid_argument) {return false;};
    return true;
}

bool isInt(string str, int &value) // если целое - int, значение в value
{
    string::size_type error;
    int start=str[0]=='-'?1:0; // индекс первого элемента числа без знака
    try
    {
        if (str[start]!='0') value=stoi(str, &error); // если начинается не с 0, то 10ое
        else
        {
            if (str[start+1]=='x') value=stoi(str, &error, 16); //16ое
            else if (str[start+1]=='b') value=stoi(str, &error, 2); // 2ое
            else return false; // ошибка
        }
    }
    catch (invalid_argument) {return false;}
    return (str.substr(error).empty()); //была ли ошибка при переводе
}

bool isFloat(string str, int &value) // если вещественное число - true, значение в value
{
    IntFloat numb;
    string::size_type error;
    try
    {
        numb.Float=stof(str, &error);
        value=numb.Int;
    }
    catch (invalid_argument) {return false;}
    return (str.substr(error).empty());
}

bool isString(string str, string &value) // если строка - true, значение в stroka
{
    if (str[0]=='"' && str[str.size()-1]=='"')
    {
        value=str.substr(1, str.size()-2);
        return true;
    }
    return false;
}

vector <Arg> parseArguments(string str) // разобрать строку аргументов
{
    string stroka;
    vector <Arg> res; // вектор результата, тип аргумента, значение и налчие ошибки
    Arg temp;
    int value;
    str=deleteBlanks(str); // удалить пробелы
    vector <string> Args=getArgs(str); // разделить аргрументы по запятым
    for (string arg: Args) // получить тип и значение аргументов
    {
        stroka=arg; // для листинга
        value=0;
        if (isRegister(arg, value)) { temp.type=Reg;  } // регистр
        else if (isInt(arg, value)) {temp.type=Int;  } // целое число
        else if (isFloat(arg, value)) {temp.type=Float; } // дробное число
        else if(isString(arg, stroka)) {temp.type=TypeArg::String; } // строка
        else {temp.type=Expr; stroka=arg; } // выражение (метка, константа, счетчик, выражение),
                                            // сохраняем в строку для вычисления на 2 проходе
        temp.value=value;
        temp.stroka=stroka;
        res.push_back(temp);
    }
    return res;
}

Operator parseOperator(string str, Operator Op) // разбор строки оператора
{
    string Id, args;			// идентификатор: метка или операция
    enum State 			// состояния
    { start, WaitOp, Label, NoColon, CodeOp, WaitArg, Argum, finish, error };
	State states = start;
    char ch = 0; // текущий символ
    for(int i=0; i < str.size(); i++)		// до конца строки - если не было коммента
    {
        ch = str[i];
        switch(states)
        {
        case start:
            if (isspace(ch))  states = WaitOp;
            else if (isalpha(ch)|| ch=='_') 		// метка началась
            {
                Id = ch;
                states = Label;
            }
            else if (ch == '#')
            {
                Op.work = false;
                Op.Comment =str;
                states = finish;
            }
            else states = error;
            break;
        case WaitOp:
            if (isspace(ch))  states = WaitOp;
            else if (isalpha(ch))
            {
                Id = ch;
                states = CodeOp;
            }
            else if (ch=='#')
            {
                if (Op.Label.empty()) Op.work = false;	// метки не было
                Op.Comment = str;
                states = finish;	// конец
            }
            else states = error;
            break;
        case Label:				// метка в операторе
            if (isalpha(ch)||ch=='_'||isdigit(ch)) 	// продолжается
            {
                Id += ch;
                states = Label;
            }
            else if (ch==':')
            {
                Op.Label = Id;
                Id = "";
                states = WaitOp;
            }
            else if (isspace(ch))
            {
                Op.Label = Id;
                Id = "";
                states = NoColon;
            }
            else states = error;
            break;
        case CodeOp:
            if (isalnum(ch))
            {
                Id = Id + ch;
                states = CodeOp;
            }
            else if (isspace(ch))
            {
                Op.Code = Id;
                Id = "";
                states = WaitArg;	// ждем аргументов
            }
            else if(ch=='#')
            {
                Op.Code = Id;
                Id = "";
                Op.Comment = str.substr(i);
                states = finish;
            }
            else states = error;
            break;
        case WaitArg:
            if (isspace(ch)) states = WaitArg;
            else if (ch=='#')
            {
                Op.Comment = str.substr(i);
				states = finish;
            }
            else
            {
                Id = ch;
                states = Argum;
            }
            break;
        case Argum:
            if(ch == '#')
            {
                args = Id;
                Id = "";
                Op.Comment = str.substr(i);
				states = finish;
            }
            else
            {
                Id += ch;
                states = Argum;
            }
            break;
        case finish:
            Op.Arguments=parseArguments(args);
            return Op;
        case error:
            Op.nError = Error::illSymbol;
            Op.work = false;
            Op.Comment = str;
            return Op;
        case NoColon:
            Op.nError = Error::noColon;
            states = WaitOp;
        break;
        }
    }
    if (states == CodeOp)
    {
        Op.Code = Id;
    }
    else if (states == Argum)
    {
        args = Id;
        Op.Arguments=parseArguments(args);
    }
    else if (states == Label)
    {
        Op.nError = Error::illOperator;
        Op.work = false;
        Op.Comment = str;
    }
    return Op;
}
