#ifndef EXPRINTERPRETER_H
#define EXPRINTERPRETER_H

#include "Data.h"

class Expression // общий класс для выражения
{
public:
    virtual int Evaluate(Data data) = 0; // вычисление
};

class Variable: public Expression // Переменная
{
    std::string name;
public:
    Variable() {};
    Variable(std::string str): name(str) {};
    virtual int Evaluate(Data data) {return data.getValue(name);}
    std::string getName() {return name;}
};

class Constant : public Variable // Константа
{
    int value;
public:
    Constant(int v) : value(v) {};
    virtual int Evaluate(Data data) {return value;} // Возвращает непосредственное значение
};

class ExprAddition: public Expression // сложение
{
    Expression* operand1;
    Expression* operand2;
public:
    ExprAddition(Expression* v1, Expression* v2){operand1=v1; operand2=v2; }
    virtual int Evaluate(Data data)
    {
        return (operand1->Evaluate(data) + operand2->Evaluate(data));
    }
};

class ExprSubstraction: public Expression // вычитание
{
    Expression* operand1;
    Expression* operand2;
public:
    ExprSubstraction(Expression* v1, Expression* v2){operand1=v1; operand2=v2; }
    virtual int Evaluate(Data data)
    {
        return (operand1->Evaluate(data) - operand2->Evaluate(data));
    }
};

class ExprMultiple : public Expression // умножение
{
    Expression* operand1;
    Expression* operand2;
public:
    ExprMultiple(Expression* v1, Expression* v2){operand1=v1; operand2=v2; }
    virtual int Evaluate(Data data)
    {
        return  (operand1->Evaluate(data) * operand2->Evaluate(data));
    }
};

class ExprDivision : public Expression // деление
{
    Expression* operand1;
    Expression* operand2;
public:
    ExprDivision(Expression* v1, Expression* v2){operand1=v1; operand2=v2; }
    virtual int Evaluate(Data data)
    {
        return  (operand1->Evaluate(data) / operand2->Evaluate(data));
    }
};

class ExprMod : public Expression // Остаток от деления
{
    Expression* operand1;
    Expression* operand2;
public:
    ExprMod(Expression* v1, Expression* v2){operand1=v1; operand2=v2; }
    virtual int Evaluate(Data data)
    {
        return (operand1->Evaluate(data) % operand2->Evaluate(data));
    }
};

int parsingExpression(std::string expr, Data data, bool &error, int LC);

class InvalidExpression{};

#endif // EXPRINTERPRETER_H
