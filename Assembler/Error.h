#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

enum Error // коды ошибок
{
    noError = 0,		// ошибок нет
    illSymbol,		    // недопустимый символ в одном из полей
    illString,          // некорректная переменная строка
    noColon,            // нет двоеточия после метки
    illOperator,        // неправильный формат оператора
    illInteger,		    // ошибка в целом числе
    illExpression,	    // неверный синтаксис выражения
    illFloat,           // некорректное дробное
    undLabel,		    // неизвестное имя в выражении
    signedInt,          // отрицательное целое
    reuLabel,		    // повторно определенная метка
    illOperation,		// недопустимый код операции
    illArgument,		// неправильное количество аргументов
    illRegister,		// некорректное обозначение регистра
    noLabel,		    // нет метки
    noEnd			    // нет директивы end
};
class ErrorFileNotFound{};

#endif // ERROR_H_INCLUDED
