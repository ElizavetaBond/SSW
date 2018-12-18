#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <unordered_map>

class Data
{
    std::unordered_map<std::string, int> nameTable; // таблица имен
public:
    int getValue(std::string str) { return nameTable.find(str)->second; } // получить значение метки
    void Insert(std::string str, int value) { nameTable.insert({str, value}); } // вставить новую метку со значением
    void Assign(std::string str, int value) { nameTable[str] = value; } // присвоить метке новое значение
    bool IsDefined(std::string str) { return nameTable.find(str) != end(nameTable); } // проверяет существует ли метка с таким именем
    std::unordered_map<std::string, int> getTable()  { return nameTable; }  // возвращает таблицу имен
};


#endif // DATA_H_INCLUDED
