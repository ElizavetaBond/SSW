#ifndef INTEGER_H_INCLUDED
#define INTEGER_H_INCLUDED

class Integer : public Command// базовый класс для целой арифметики
{
public:
    virtual int operation (int valA, int valB)=0;
    virtual void operator () (Processor &p) // одинаковые ействия для всех команд целой ариф.
    {
        structCommand code=p.getCodeCommand();
        int valA=p.Registers[code.reg], valB=getNumber(p, code.addr), valE; // считать данные
        valE=operation(valA, valB); // получить результат
        p.Registers[code.reg]=valE; // записать результат
        flags fl=p.PSW.Flags; // установить флаги
        fl.ZF=(valE==0)?1:0;
        fl.SF=(valE<0)?1:0;
        p.PSW.Flags=fl;
        p.PSW.IP+=3; // увеличить адрес
    }
};

class add : public Integer // 4 целое сложение
{
    virtual int operation (int a, int b) { return a+b; };
};

class sub : public Integer // 5 целое вычитание
{
    virtual int operation (int a, int b) { return a-b; }
};

class mul : public Integer // 6 целое умножение
{
    virtual int operation (int a, int b) { return a*b; }
};

class divs : public Integer// 7 целое деление
{
    virtual int operation (int a, int b) { return a/b; }
};

class rdiv: public Integer // 8 остаток от деления
{
    virtual int operation (int a, int b) { return a%b; }
};

#endif // INTEGER_H_INCLUDED
