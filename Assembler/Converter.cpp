#include <fstream>
#include <string>
#include "Types.h"

using namespace std;

bool ConvertToText(string nameBin, string nameText) // переводит бинарный файл в текстовый
{
    ifstream fileBin(nameBin, ios::binary);
    ofstream fileText(nameText);
    char sign = 0, // тип команды загрузчика
         ch=0; // для загрузки строки
    uint8_t cop, reg;
    uint16_t addr;
    IntFloat chislo;
    string str; // переменная для загрузки строки
    if (fileBin.is_open() && !fileBin.bad())
    {
        while (fileBin.read((char *)&sign, sizeof(sign)))
        {
            switch (sign)    // в зависимости от типа
            {
                case Prefix::pAddr:   //Адрес загрузки
                    fileBin.read((char *) &addr, sizeof(addr));
                    str="a " + to_string(addr);
                    break;
                case Prefix::pBegin:   // начало программы
                    fileBin.read((char *) &addr, sizeof(addr));
                    str="b " + to_string(addr);
                    break;
                case Prefix::pEnd:   // конец програмы
                    str="e";
                    break;
                case Prefix::pInt:   // целое число
                    fileBin.read((char *) &chislo.Int, sizeof(chislo.Int));
                    str="i " + to_string(chislo.Int);
                    break;
                case Prefix::pFloat:   // вещественное число
                    fileBin.read((char *) &chislo.Int, sizeof(chislo.Int));
                    str="f " + to_string(chislo.Float);
                    break;
                case Prefix::pString: // строка символов
                    str="s ";
                    ch=0;
                    while (ch!='/')
                    {
                        fileBin.read(&ch, sizeof(ch));
                        str+=ch;
                    }
                    break;
                case Prefix::pCmd: // код команды
                    fileBin.read((char *) &cop, sizeof(cop));
                    fileBin.read((char *) &reg, sizeof(reg));
                    fileBin.read((char *) &addr, sizeof(addr));
                    str="c " + to_string(cop) + " " + to_string(reg) + " " + to_string(addr);
                    break;
            }
            str+="\n";
            fileText << str;
        }
    } else return false;
    fileText.close();
    fileBin.close();
}
