#ifndef LISTING_H
#define LISTING_H

#include <vector>
#include "Types.h"
#include "Data.h"

class Listing
{
    std::vector <Operator> Program;     // разобранная программа
    Data data;
    std::string getErrorMessage(int nError); // получить сообщение об ошибке
public:
    Listing(std::vector <Operator> prog, Data d) {Program=prog; data=d;} // конструктор
    void writeListing(); // формирует листинг
};

#endif // LISTING_H
