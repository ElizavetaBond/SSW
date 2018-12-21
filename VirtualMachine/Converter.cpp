#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Types.h"

using namespace std;

void convertToBinary (string nameText, string nameBin)
{
    ifstream fileText(nameText);
    ofstream fileBin(nameBin, ios::binary);
    string s;
    char sign;
    if (fileText.is_open())
    {
        sign = 0;
        while (getline(fileText, s))
        {
            auto found=s.find('#');
            if (found!=string::npos) s=s.substr(0, found);
            sign = s[0];            // тип команды загрузчика
            unionIntFloat chislo;
            string stroka="";
            int ip, cop, reg, addr, startAddr;
            fileBin.write(&sign, sizeof(sign)); // запись в бинарника типа команды
            if (!s.empty())
            {
                istringstream Stream(s.substr(1));    // для перевода
                switch (sign)  // тип команды
                {
                    case 'a': // адрес
                        Stream >> ip;
                        fileBin.write((char *) &ip, sizeof(ip));
                        break;
                    case 'b': // начало программы
                        Stream >> startAddr;
                        fileBin.write((char *) &startAddr, sizeof(startAddr));
                        break;
                    case 'e': // конец программы
                        break;
                    case 'i':   //целое число
                        Stream >> chislo.Int;
                        fileBin.write((char *) &chislo.Int, sizeof(chislo.Int));
                        break;
                    case 'f':  //дробное число
                        Stream >> chislo.Float;
                        fileBin.write((char *) &chislo.Int, sizeof(chislo.Int));
                        break;
                    case 's': // строка символов
                        Stream >> stroka;
                        stroka+="/";
                        fileBin.write(stroka.c_str(), stroka.size());
                        break;
                    case 'c':  //команда
                        Stream >> cop >> reg >> addr;
                        fileBin.write((char *) &cop, sizeof(cop));
                        fileBin.write((char *) &reg, sizeof(reg));
                        fileBin.write((char *) &addr, sizeof(addr));
                        break;
                    default:
                        throw Error();
                        break;
                }
            }
        }
    } else throw Error();
    fileText.close();
    fileBin.close();
}
