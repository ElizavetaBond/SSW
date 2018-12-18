#include "Processor.h"
#include "Types.h"

using namespace std;

class Error{};

void loadNumber(Processor &p, int number, int address) // загрузка числа в  память
{
    unionNumberChar Union;
    Union.Int=number;
    for (int i=0; i<4; i++, address++)
        p.memory[address]=Union.byte[i];
};

void loadCodeCommand(Processor &p, structCommand Code, int address)
{
    unionCommand Union;
    Union.code=Code;
    for (int i=0; i<3; i++, address++)
        p.memory[address]=Union.byte[i];
}

void loadString (Processor &p, string str, int address) // загрузка строки в память
{
    char c;
    for (int i=0; i<str.size(); i++, address++)
    {
        c=str[i];
        p.memory[address]=c;
    }
}

void loader (string filename, Processor &p)
{
    ifstream fileBin(filename, ios::binary);
    char sign = 0, // тип команды загрузчика
         ch=0; // для загрузки строки
    int ip=0, chislo, cop, reg, addr, startAddr;
    string str; // переменная для загрузки строки
    structCommand Code; // код команды
    if (fileBin.is_open() && !fileBin.bad())
    {
        while (fileBin.read(&sign, sizeof(sign)))
        {
            switch (sign)    // в зависимости от типа
            {
                case 'a':   //Адрес загрузки
                    fileBin.read((char *) &ip, sizeof(ip));
                    break;
                case 'b':   // начало программы
                    fileBin.read((char *) &startAddr, sizeof(startAddr));
                    p.setStartAddr(startAddr);
                    break;
                case 'e': // конец програмы
                    p.memory[ip]=0;
                    p.memory[ip+1]=0;
                    p.memory[ip+2]=0;
                    ip+=3;
                    break;
                case 'i':   // целое число
                case 'f':   // вещественное число
                    fileBin.read((char *) &chislo, sizeof(chislo));
                    loadNumber(p, chislo, ip);
                    ip+=4;
                    break;
                case 's': // строка символов
                    str="";
                    ch=0;
                    while (ch!='/')
                    {
                        fileBin.read(&ch, sizeof(ch));
                        str+=ch;
                    }
                    loadString(p, str,  ip);
                    ip+=str.size();
                    break;
                case 'c': // код команды
                    fileBin.read((char *) &cop, sizeof(cop));
                    fileBin.read((char *) &reg, sizeof(reg));
                    fileBin.read((char *) &addr, sizeof(addr));
                    Code.cop=cop;
                    Code.reg=reg;
                    Code.addr=addr;
                    loadCodeCommand(p, Code, ip);
                    ip+=3;
                    break;
                default:
                    throw Error();
                    break;
            }
        }
    }
    p.memory[ip]=0; // загрузить команду остановки
    p.memory[ip+1]=0;
    p.memory[ip+2]=0;
    fileBin.close();
}
