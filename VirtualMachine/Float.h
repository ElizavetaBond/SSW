#ifndef FLOAT_H_INCLUDED
#define FLOAT_H_INCLUDED

#include <limits>

class Float : public Command // базовый класс для дробной арифметики
{
public:
    virtual float operation(float a, float b)=0; // арифметическое действие
    virtual void operator () (Processor &p) // одинаковые дейстия для всех команд дроб. ариф.
    {
        structCommand code=p.getCodeCommand(); // получить код команды
        unionIntFloat valA, valB, valE;
        valA.Int=p.Registers[code.reg], valB.Int=getNumber(p, code.addr); // считать данные
        valE.Float=operation(valA.Float, valB.Float); // получить результат
        p.Registers[code.reg]=valE.Int; // записать результат
        flags fl=p.getFlags(); // расставить флаги
        fl.ZF=(fabs(valE.Float)< std::numeric_limits<float>::epsilon())?1:0;
        fl.SF=valE.Float<0?1:0;
        p.setFlags(fl);
        p.setIP(p.getIP()+3); // увеличить адрес
    }
};

class add_f : public Float // 9 дробное сложение
{
    virtual float operation (float a, float b) { return a+b; }
};

class sub_f : public Float // 10  дробное вычитание
{
    virtual float operation (float a, float b) { return a-b; }
};

class mul_f : public Float  // 11 дробное умножение
{
    virtual float operation (float a, float b)  { return a*b; }
};

class div_f : public Float // 12 дробное деление
{
    virtual float operation (float a, float b) { return a/b; }
};


#endif // FLOAT_H_INCLUDED
