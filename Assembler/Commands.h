#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "ExprInterpreter.h"

class Command
{
public:
    virtual int operator()(Operator &op, Data &data);
};

class fNOARG : public Command // для команд без аргументов (стоп и возврат)
{
    virtual int operator()(Operator &op, Data &data)
    {
        if (op.Arguments.size()!=0) op.nError=Error::illArgument;
        else
        {
            Arg arg(TypeArg::Int, 0, "");
            op.Arguments.push_back(arg);
            op.Arguments.push_back(arg);
        }
        return lenghtCmd;
    }
};

class fTWOARG : public Command // для команд с двумя аргументами
{
    virtual int operator()(Operator &op, Data &data)
    {
        bool error=false;
        if (op.Arguments[1].type==TypeArg::Expr) // если 2й аргумент - выражение
        {
            op.Arguments[1].value=parsingExpression(op.Arguments[1].stroka, data, error, op.LC); // вычислить
            op.Arguments[1].type=TypeArg::Int;
        }
        if (op.Arguments.size()!=2) op.nError=Error::illArgument;
        else if (op.Arguments[0].type!=TypeArg::Reg) op.nError=Error::illRegister;
        else if (op.Arguments[1].type!=TypeArg::Int) op.nError=Error::illInteger;
        else if (op.Arguments[1].value<0) op.nError=Error::signedInt;
        else if (error) op.nError=Error::illExpression;
        return lenghtCmd;
    }
};

class ONEARG: public Command // для команд с одним аргументом (адресом)
{
    virtual int getCode()=0; // что записать в поле регистра
    virtual int operator()(Operator &op, Data &data) // общая процедура обработки
    {
        bool error=false;
        if (op.Arguments[0].type==TypeArg::Expr) // если  аргумент - выражение
        {
            op.Arguments[0].value=parsingExpression(op.Arguments[0].stroka, data, error, op.LC); // вычислить
            op.Arguments[0].type=TypeArg::Int;
        }
        if (op.Arguments.size()!=1)  op.nError=Error::illArgument;
        else if (op.Arguments[0].type!=TypeArg::Int) op.nError=Error::illInteger;
        else if (op.Arguments[0].value<0) op.nError=Error::signedInt;
        else if (error) op.nError=Error::illExpression;
        else
        {
            int addr=op.Arguments[0].value;
            op.Arguments[0].value=getCode(); // код, указывающий на то, что переход прямой
            op.Arguments.push_back(Arg(TypeArg::Int, addr, ""));
        }
        return lenghtCmd;
    }
};

class fDIRTRANS: public ONEARG // прямые переходы
{
    virtual int getCode() {return 1;}
};

class fINDIRTRANS: public ONEARG // косвенные переходы переходы
{
    virtual int getCode() {return 2;}
};

class fRELTRANS: public ONEARG // относительные переходы
{
    virtual int getCode() {return 3;}
};

class fCALL : public ONEARG // команда вызова
{
    virtual int getCode() {return 0;}
};

class fINOUT : public ONEARG // для команды ввода-вывода целых и дробных чисел
{
    virtual int getCode() {return 0;}
};

class fNOT : public Command // один аргумент (регистр)
{
    virtual int operator()(Operator &op, Data &data)
    {
        if (op.Arguments.size()!=1) op.nError=Error::illArgument;
        else if (op.Arguments[0].type!=TypeArg::Reg ) op.nError=illRegister;
        else
        {
            op.Arguments.push_back(Arg(TypeArg::Int, 0, ""));
        }
        return lenghtCmd;
    }
};

#endif // COMMANDS_H_INCLUDED
