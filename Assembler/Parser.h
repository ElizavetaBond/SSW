#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <Types.h>

std::string deleteBlanks (std::string str); // удаляем пробелы

std::vector <std::string> getArgs(std::string str); // разделяем строку с аргументами на части по запятым

bool isRegister(std::string str, int &value); // если регистр - true, код регистра в value

bool isInt(std::string str, int &value); // если целое - int, значение в value

bool isFloat(std::string str, int &value); // если вещественное число - true, значение в value

bool isString(std::string str, std::string &value); // если строка - true, значение в stroka

std::vector <Arg> parseArguments(std::string str); // разобрать строку аргументов

Operator parseOperator(std::string str, Operator Op); // разбор строки оператора

#endif // PARSER_H
