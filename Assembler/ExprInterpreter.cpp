#include <vector>
#include <stack>
#include <algorithm>
#include "ExprInterpreter.h"
#include "Parser.h"

using namespace std;

bool is_op(string op) // Является ли заданная строка операцией
{
    return op == "*" || op == "/" || op == "%" || op == "+" || op == "-" || op == "(" || op == ")";
};

vector<string> getLexems(string str) // разбивает строку на лексемы
{
    vector<string> res;
    int i = 0, n = str.size();
    string token="";
    while (i < n)
    {
        char ch = str[i];
        if (is_op(string(1, ch)) || ch == '@') // Операции
        {
            res.push_back(string(1, ch));
            i++;
        }
        else if (isalpha(ch) || ch == '_') // Идентификаторы
        {
            do
            {
                token += ch;
                i++;
                ch = str[i];
            } while ((isalpha(ch) || isdigit(ch) || (ch == '_')) && (i < n));
            res.push_back(token);
            token = "";
        }
        else if (isdigit(ch)) // Нашли число -- формат не проверяем
        {
            do
            {
                token += ch;
                i++;
                ch = str[i];
            } while ((isalpha(ch) || isdigit(ch)) && (i < n));
            res.push_back(token);
            token = "";
        }
        else throw InvalidExpression();
    }
    return res;
}

vector<string> get_notation(vector <string> V) // фомирует обратную польскую запись выражения
{
    vector <string> res;   // результирующая строка
    stack <string> Stack; // стек операций
    auto priority = [](string op) // Возвращает приоритет операции
    {
        if (op == "*" || op == "%" || op == "/") return 2;
        else if (op == "+" || op == "-") return 1;
        else return 0;
    };
    for (auto& elem: V)
    {
        if (elem == "(") Stack.push(elem);
        else if (elem == ")")
        {
            while (!Stack.empty() && Stack.top() != "(")
            {
                res.push_back(Stack.top());
                Stack.pop();
            }
            if (!Stack.empty()) Stack.pop();
            else throw InvalidExpression();
        }
        else if (is_op(elem)) // если операция
        {
            string op = elem;
            // Пока приоритет вершины стека больше чем искомой операции, кидаем из стека в результат
            while (!Stack.empty() && (priority(Stack.top()) >= priority(op)))
            {
                res.push_back(Stack.top());
                Stack.pop();
            }
            Stack.push(op);
        }
        else // здесь только операнды
        {
            res.push_back(elem);
        }
    }
    while (!Stack.empty())  // добавить в результат оставшиеся операции
    {
        res.push_back(Stack.top());
        Stack.pop();
    }
    if (find_if(begin(res), end(res), [](auto token){return token == "(" || token == ")";}) != end(res)) // нашли скобки -- неправильно поставили
        throw InvalidExpression();
    return res;
}

Expression* getTypeOp (std::string oper, Expression* operand1, Expression* operand2)
{
    Expression* res = nullptr;
    if (oper == "+") res = new ExprAddition(operand1, operand2);
    else if (oper == "-") res = new ExprSubstraction(operand1, operand2);
    else if (oper == "*") res = new ExprMultiple(operand1, operand2);
    else if (oper == "/") res = new ExprDivision(operand1, operand2);
    else if (oper == "%") res = new ExprMod(operand1, operand2);
    return res;
}

Expression* treeExpression(vector<string> vec_notation, Data data, int LC) // синтаксическое дерево для заданного выражения
{
    stack <Expression*> Stack; // стек операндов
    for(auto& elem: vec_notation)
    {
        if (is_op(elem))
        {
            if (Stack.size() >= 2)
            {
                Expression* op1 = Stack.top();
                Stack.pop();
                Expression* op2 = Stack.top();
                Stack.pop();
                Expression* expr = getTypeOp(elem, op1, op2);
                Stack.push(expr);
            }
            else throw InvalidExpression();
        }
        else if (isalpha(elem[0]))
        {
            if (data.IsDefined(elem)) // если переменная есть в таблице
                Stack.push(new Constant(data.getValue(elem)));
            else throw InvalidExpression();
        }
        else if (elem=="@") Stack.push(new Constant(LC));
        else if (isdigit(elem[0]))
        {
            int value=0;
            if (isInt(elem, value))
                Stack.push(new Constant(value));
            else throw InvalidExpression(); // Иначе -- неверный числовой формат!
        }
    }
    if (Stack.size() == 1)
        return Stack.top();
    else throw InvalidExpression();
}

int parsingExpression(string expr, Data data, bool &error, int LC) // управляющая функция
{
    int res;
    try
    {
        expr=deleteBlanks(expr);    // удалить пробелы
        auto tokens = getLexems(expr); // разбить на лексесы
        auto notation = get_notation(tokens); // получить обратную польскую запись
        Expression* expression = treeExpression(notation, data, LC); // сформировать дерево выражения
        res = expression->Evaluate(data); // вычислить выражение
    }
    catch (...)
    {
        error = true;
    }
    return res;
}
