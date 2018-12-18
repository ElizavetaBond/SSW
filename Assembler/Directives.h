#ifndef DIRECTIVES_H_INCLUDED
#define DIRECTIVES_H_INCLUDED

#include "Commands.h"
#include "Types.h"
#include "Data.h"

class fstring : public Command // тип - строки
{
    virtual int operator()(Operator &op, Data &data)
    {
        int lenght=0;
        std::string str;
        for (Arg arg: op.Arguments)
        {
            if (arg.type==TypeArg::String)
            {
                str=arg.stroka;
                str+='/'; // конец строки
                lenght+=str.size(); // для смещения
                op.Binary.push_back(Prefix::pString); // записываем код
                for (char c: str)
                    op.Binary.push_back(c);
            }
            else
            {
                op.nError=Error::illString;
                return lenght;
            }
        }
        return lenght;
    }
};

class fint : public Command // тип - целое
{
    virtual int operator()(Operator &op, Data &data)
    {
        int lenght=0;
        IntFloat number;
        for (Arg arg: op.Arguments)
        {
            if (arg.type==TypeArg::Int)
            {
                number.Int=arg.value;
                lenght+=sizeof(int);
                op.Binary.push_back(Prefix::pInt); // записываем код
                for (uint8_t byte: number.arr)
                    op.Binary.push_back(byte);
            }
            else
            {
                op.nError=Error::illInteger;
                return lenght;
            }
        }
        return lenght;
    }
};

class ffloat : public Command // тип - дробное
{
    virtual int operator()(Operator &op, Data &data)
    {
        int lenght=0;
        IntFloat number;
        for (Arg arg: op.Arguments)
        {
            if (arg.type==TypeArg::Float)
            {
                number.Int=arg.value;
                lenght+=sizeof(float);
                op.Binary.push_back(Prefix::pFloat); // записываем код
                for (uint8_t byte: number.arr)
                    op.Binary.push_back(byte);
            }
            else
            {
                op.nError=Error::illFloat;
                return lenght;
            }
        }
        return lenght;
    }
};

class reserve : public Command // резервирование памяти
{
    virtual int getSize()=0; // размер одного элемента
    virtual int operator()(Operator &op, Data &data)
    {
        int sizeElem=getSize();
        int counts=0;
        if (op.Arguments.size()!=1) op.nError=Error::illArgument;
        else if(op.Arguments[0].type!=TypeArg::Int) op.nError=Error::illInteger;
        else if (op.Arguments[0].value<0) op.nError=Error::signedInt;
        else
        {
            counts=op.Arguments[0].value;
        }
        return sizeElem*counts;
    }
};

class fmembyte : public reserve // резервирование байтов
{
    virtual int getSize(){return 1;}
};

class fmemint : public reserve // резервирование блоков по 4 байта
{
    virtual int getSize(){return 4;}
};

class fequ : public Command // определение константы
{
    virtual int operator()(Operator &op, Data &data)
    {
        bool error=false;
        Arg arg;
        if (op.Arguments[0].type==TypeArg::Expr)
        {
            op.Arguments[0].value=parsingExpression(op.Arguments[0].stroka, data, error, op.LC);
            op.Arguments[0].type=TypeArg::Int;
        }
        if (op.Arguments.size()!=1) op.nError=Error::illArgument;
        else if (op.Arguments[0].type!=TypeArg::Int) op.nError=Error::illInteger;
        else if (error) op.nError=Error::illExpression;
        else
        {
            int value=op.Arguments[0].value;
            if (data.IsDefined(op.Label)) data.Assign(op.Label, value);
            else op.nError=Error::undLabel;
        }
        return 0;
    }
};

class forg : public Command // задать счетчик размещения
{
    virtual int operator()(Operator &op, Data &data)
    {
        bool error=false;
        Arg arg;
        if (op.Arguments[0].type==TypeArg::Expr)
        {
            op.Arguments[0].value=parsingExpression(op.Arguments[0].stroka, data, error, op.LC);
            op.Arguments[0].type=TypeArg::Int;
        }
        if (op.Arguments.size()!=1) op.nError=Error::illArgument;
        else if (op.Arguments[0].type!=TypeArg::Int) op.nError=Error::illInteger;
        else if (error) op.nError=Error::illExpression;
        else if (op.Arguments[0].value<0) op.nError=Error::signedInt;
        else
        {
            Address addr;
            addr.word=op.Arguments[0].value;
            op.LC=addr.word;
            op.Binary.push_back(Prefix::pAddr);
            op.Binary.push_back(addr.arr[0]);
            op.Binary.push_back(addr.arr[1]);
        }
        return 0;
    }
};

class fproc : public Command // начало подпрограммы
{
    virtual int operator()(Operator &op, Data &data)
    {
        if (op.Arguments.size()!=0) op.nError=Error::illArgument;
        else if (op.Label=="") op.nError=Error::noLabel;
        return 0;
    }
};

class fendp : public Command // конец подпрограммы
{
    virtual int operator()(Operator &op, Data &data)
    {
        if (op.Arguments.size()!=0) op.nError=Error::illArgument;
        else if (op.Label=="") op.nError=Error::undLabel;
        return 0;
    }
};

class fbegin : public Command
{
    virtual int operator()(Operator &op, Data &data)
    {
        if (op.Arguments.size()!=1)
            op.nError=Error::illArgument;
        else if (op.Arguments[0].type!=TypeArg::Int) op.nError=Error::illInteger;
        else if (op.Arguments[0].value<0) op.nError=Error::signedInt;
        else
        {
            Address addr;
            addr.word=op.Arguments[0].value;
            op.Binary.push_back(Prefix::pBegin);
            op.Binary.push_back(addr.arr[0]);
            op.Binary.push_back(addr.arr[1]);
        }
        return 0;
    }
};

class fend : public Command
{
    virtual int operator()(Operator &op, Data &data)
    {
        if (op.Arguments.size()!=0 || op.Label!="")
            op.nError=Error::illArgument;
        else {op.Binary.push_back(Prefix::pEnd);}
        return 0;
    }
};

#endif // DIRECTIVES_H_INCLUDED
