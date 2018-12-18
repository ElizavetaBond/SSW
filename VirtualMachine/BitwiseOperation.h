#ifndef BITWISEOPERATION_H_INCLUDED
#define BITWISEOPERATION_H_INCLUDED

class Bitwise : public Command// базовый класс для целой арифметики
{
public:
    virtual int operation (int valA, int valB)=0;
    virtual void operator () (Processor &p) // одинаковые ействия для всех команд целой ариф.
    {
        structCommand code=p.getCodeCommand();
        int valA=p.Registers[code.reg], valB=getNumber(p, code.addr), valE; // считать данные
        valE=operation(valA, valB); // получить результат
        p.Registers[code.reg]=valE; // записать результат
        p.setIP(p.getIP()+3); // увеличить адрес
    }
};

class Not : public Bitwise // 28 логическое отрицание
{
    virtual int operation (int a, int b) { return ~a; };
};

class And : public Bitwise // 29 логическое и
{
    virtual int operation (int a, int b) { return a&b; };
};

class Or : public Bitwise // 30 логическое или
{
    virtual int operation (int a, int b) { return a|b; };
};

class Xor : public Bitwise // 31 логическое исключающее или
{
    virtual int operation (int a, int b) { return a^b; };
};

#endif // BITWISEOPERATION_H_INCLUDED
